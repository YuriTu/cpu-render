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
            Sphere lightObject = objects[i];
            lightObject.sampleSphereUniform(light,pdf);
            light.hitObject = &lightObject;
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
    // printf("index %d", index);
    ret.hitObject = &objects[index];
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


    Vector4f hitPoint = interaction.hitPoint;
    float rr = 0.3;

    if (hitObject->reflectType == utils::DIFFUSE) {
        // rr准入
        // if !rr return 0
        
        float randomValue = getRandom(0,1);
        if (randomValue > rr) {
            return ret / rr;
        }
        float pdf;
        // 在光源上随机采个样
        
        Interaction lightInteraction;
        // 方位角随机数  pdf 就是整个面积的uniform没有特殊处理
        // 均匀对于半球进行采样 按照pdf 进行N次采样
        sampleLight(lightInteraction,pdf);
        // castRay 看是否有intersection
        Vector4f lightHitPoint = lightInteraction.hitPoint;
        Vector4f lightDir = hitPoint - lightHitPoint;
        Vector4f lightN;
        Sphere* LightObject = lightInteraction.hitObject;
        LightObject->getSurfaceProperties(lightHitPoint,lightN);
        // 直接光照，对area进行采样
        // brdf = fresnel ？  或者直接phone
        Vector4f Lo = hitObject->diffuseColor;
        
        float brdf = hitObject->evalBRDF();
        Vector4f N;
        hitObject->getSurfaceProperties(hitPoint, N);
        float cosLightWo = std::max(0.f,N.dot(-lightDir)) ;
        // printf("coslight wo %f", cosLightWo);
        // 0就是光源照不到
        // todo 直接光照中间被挡住

        float cosDArea = std::max(0.f, lightN.dot(lightDir));
        Vector4f lightDistance = lightHitPoint - hitPoint;
        float distance2 = lightDistance.dot(lightDistance);
        Vector4f directRadiance = Lo * brdf * cosLightWo * (cosDArea / distance2);

        // 间接光照部分
        if (lightInteraction.flag){
            // is light 不算  根据material de emit
        }

        Vector4f indirectRadiance(0.0);

        // indirectRadiance = pathTracing(lightRay) * brdf * cos;
        // 是其他东西 = indir
        // L = Lo * brdf * cos

        //L =  Ld + Lindir /n * 1/pdf * 1/rr
        ret = directRadiance + indirectRadiance;
        // n在外面处理
        if (ret.dot(ret) > 0) {
            // printf("has color");
        }
        return ret / pdf / rr;

    }

    return ret;

}

void r::TracingRender::render()
{
    int samples = 15;

    Vector4f cam(0, 0, -100);
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
                Vector4f _radiance = pathTracing(ray,0);
                radiance += _radiance;
            }
            frameBuffer[index] = radiance;
        }
    }
    exportImg(frameBuffer, width,height);
}