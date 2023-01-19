#include "tracingRender.h"
#include "utils.h"

#include <omp.h>

r::TracingRender::TracingRender(int w, int h): width(w),height(h) {
    frameBuffer.resize(width * height);
    background = Vector4f(0.3);
    maxBounce = 3;
    fov = 90.f;
}

void r::TracingRender::setPerspectiveProjection(float near, float far, float aspect, float fov) {
    // ray dir?
}

void r::TracingRender::setModule(Vector4f position) {
    // ray dir?
}

void r::TracingRender::setView(Vector4f position) {
    // pos lookat updir
    // set camera pos
}

void r::TracingRender::add(Sphere &m) {
    objects.push_back(m);
}
void r::TracingRender::add(Light &l) {
    lights.push_back(l);
}

void r::TracingRender::sampleLight(Interaction& light ,float& pdf) {
    // todo 仅考虑一个光源
    
    for (int i=0; i < objects.size(); i++) {
        if (objects[i].hasEmit()) {
            objects[i].sampleSphereUniform(light,pdf);
        }
    }
    
}

Interaction r::TracingRender::castRay(Ray &ray) {
    // ray 
    Interaction ret;
    
    float t = 1e10;
    float tMin = t;
    float index = 0;
    for (int i = 0; i < objects.size(); i++) {
        bool flag = objects[i].intersect(ray, t);
        
        if (flag && t < tMin) {
            
            index = i;
            tMin = t;
        }
    }

    if (tMin == 1e10) {
        ret.flag = false;
        return ret;
    }
    ret.flag = true;
    ret.hitPoint = ray.o + ray.dir * tMin;
    ret.hitObject = &objects[index];
    ret.normal = normalize(ret.hitPoint - ret.hitObject->o);
    ret.distance = tMin;
    
    return ret;
}

Vector4f r::TracingRender::getRadiance(Ray &ray, int bounce) {
    if (bounce > maxBounce) {
        return Vector4f(0.0);
    }

    Interaction interaction = castRay(ray);
    if (!interaction.flag) {
        // background
        return background;
    }

    // 1. mode witte style
    // 1. phone style specular diffuse + fresnel

    Vector4f ret(0.0,0,0);
    Vector4f hitPoint = interaction.hitPoint;
    r::Sphere* hitOBject = interaction.hitObject;
    Vector4f N;
    hitOBject->getSurfaceProperties(hitPoint, N);
    if (hitOBject->reflectType == utils::REFLECTION_AND_REFRACTION) {
        float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
        Vector4f reflectRayDir = normalize(reflect(ray.dir, N));
        Vector4f reflectRayOri = reflectRayDir.dot(N) > 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

        Vector4f refractRayDir = normalize(refract(ray.dir, N, hitOBject->ior));
        Vector4f refractRayOri = refractRayDir.dot(N) > 0? hitPoint + N * EPS : hitPoint - N * EPS;

        Ray reflectRay(reflectRayOri, reflectRayDir);
        Ray refractRay(refractRayOri, refractRayDir);
        ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient + getRadiance(refractRay, bounce + 1) * (1 - fresnelCoeffient);
    } else if (hitOBject->reflectType == utils::REFLECTION) {
        float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
        Vector4f reflectRayDir = normalize(reflect(ray.dir, N));
        Vector4f reflectRayOri = reflectRayDir.dot(N) > 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

        Ray reflectRay(reflectRayOri, reflectRayDir);
        ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient;
    } else if (hitOBject->reflectType == utils::DIFFUSE) {
        for (r::Light light : lights) {
            Vector4f lightDir = ( light.pos - hitPoint);
            float lightDistance2 = lightDir.dot(lightDir);
            lightDir = normalize(lightDir);
            
            Vector4f outgoingDir = reflect(lightDir, N);
            Vector4f halfVector = normalize(outgoingDir + lightDir);
            // polygon offset 类似shadow map避免精度问题
            Vector4f temporigin = ray.dir.dot(N) < 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
            Ray shadowRay(temporigin, lightDir);
            Interaction shadowRayInteraction = castRay(shadowRay);
            float diffuseCoefficient = std::max(lightDir.dot(N), 0.f) ;
            if (shadowRayInteraction.flag) {
                Vector4f tempShadowRayDir = shadowRay.o - shadowRayInteraction.hitPoint;
                float tempShandowRayDistance2 = tempShadowRayDir.dot(tempShadowRayDir);
                if (tempShandowRayDistance2 < lightDistance2) {
                    // shadow without diffuse
                    diffuseCoefficient = 0;
                }
            }

            Vector4f specularColor = light.intensity * std::pow(halfVector.dot(N),hitOBject->specularExponent); 

            if (diffuseCoefficient == 0) {
                Vector4f foo;
            }
            Vector4f diffuseRadiance = hitOBject->kd * hitOBject->diffuseColor * diffuseCoefficient;
            Vector4f specularRadiance = hitOBject->ks * specularColor;
            ret += diffuseRadiance + specularRadiance;
        }
    }

    return ret;
    // return Vector4f(1.0,0.0,0.0);

    // return radiance
}

Vector4f r::TracingRender::pathTracing(Ray &ray, int depth) {
    Vector4f ret(0.0);
    Vector4f indirectRadiance(0.0);
    Vector4f directRadiance(0.0);
    
    Interaction interaction = castRay(ray);

    if (!interaction.flag) {
        ret = background;
        return ret;
    }

    Sphere* hitObject = interaction.hitObject;
    // 打到光上了直接过
    if (hitObject->hasEmit()){
        return hitObject->emit;
    }
    // p incident point
    Vector4f p = interaction.hitPoint;
    Vector4f N = interaction.normal;
    Vector4f L_wo = normalize(-ray.dir);
    if (hitObject->reflectType == utils::DIFFUSE) {

        float pdf_light;
        // 在光源上随机采个样
        Interaction inter_light;
        // 方位角随机数  pdf 就是整个面积的uniform没有特殊处理
        // 均匀对于半球进行采样 按照pdf 进行N次采样
        this->sampleLight(inter_light,pdf_light);
        // castRay 看是否有intersection
        // 随机采样出来的射线 从p点到x 方向
        Vector4f x = inter_light.hitPoint;
        Vector4f L_wi_origin = x - p;
        Vector4f L_wi = normalize(L_wi_origin);
        Vector4f L_wi_N = normalize(inter_light.normal);

        // 直接光照，对area进行采样
        Vector4f L_dir_Li = inter_light.emit;
        Vector4f brdf = hitObject->evalBRDF(L_wo,N);
        // 0就是光源照不到
        float cosIncidentPoint = std::max(0.f, N.dot(L_wi));
        // 光源转到正向
        float cosLight = std::max(0.f, L_wi_N.dot(-L_wi));

        if (cosLight > 0.f && cosIncidentPoint > 0.f) {
            //  printf("%f",cosLight);
        }

        float daTodw = (cosLight) / (L_wi_origin.norm() * L_wi_origin.norm()) / pdf_light;
        
        // todo 直接光照中间被挡住
        Ray testShadowray(p,L_wi);
        Interaction shadowray = castRay(testShadowray);
        if (shadowray.flag && (shadowray.hitPoint - inter_light.hitPoint).norm() < EPS) {
            directRadiance = L_dir_Li * brdf * cosIncidentPoint * daTodw;
        }
        
        // 间接光照部分
        
        // float rr =0.8f;
        // float randomValue = getRandom(0,1);
        // if (randomValue > rr) {

        //     float pdf_indir = 1 / (PI * 2);
        //     Vector4f L_indir_wo = p + getVecFromSampleSphereUniform();
        //     Ray indir_ray(p, L_indir_wo);
        //     Vector4f indir_brdf = hitObject->evalBRDF(L_indir_wo,N);
        //     float indir_cosWiN = std::max(0.f, (L_indir_wo).dot(N));

        //     indirectRadiance = pathTracing(indir_ray, depth++) * indir_brdf * indir_cosWiN / pdf_indir / rr;
        // }
        
        ret = directRadiance + indirectRadiance;
        return ret;
    }

    return ret;

}

void r::TracingRender::render(int samples)
{

    Vector4f cam(10, 10, -150);
    float scale = tan(deg2rad(fov * 0.5));  
    float imageRadio = width / (float)height;
    #pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Vector4f radiance;

            float x = 2.0 * (((i + 0.5f) / width) - .5f) * imageRadio * scale;
            float y = 2.0 * (((j + 0.5f) / height) - .5f) * scale;
            Vector4f _dir(x,y,1);
            Vector4f dir = normalize(_dir);
            Ray ray(cam,dir);

            int index = getIndex(i,j,width,height);

            for (int k = 0; k < samples; k++) {
                // Vector4f _radiance = getRadiance(ray,0);
                // todo gama normliaz
                Vector4f _radiance = pathTracing(ray,0);
                if (_radiance.x > 0) {
                    radiance;
                }
                radiance += _radiance;
            }
            frameBuffer[index] = radiance / samples;
        }
    }
    exportImg(frameBuffer, width,height);
}