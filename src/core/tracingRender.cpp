#include "tracingRender.h"
#include "utils.h"
#include "interaction.h"

#include <omp.h>

namespace r{

TracingRender::TracingRender(int w, int h): width(w),height(h) {
}

void TracingRender::setPerspectiveProjection(float near, float far, float aspect, float fov) {
    // ray dir?
}

void TracingRender::setModule(Vector3f position) {
    // ray dir?
}

void TracingRender::setView(Vector3f position) {
    // pos lookat updir
    // set camera pos
}

// void TracingRender::add(Sphere &m) {
//     objects.push_back(m);
// }
void TracingRender::add(Light &l) {
    lights.push_back(l);
}

void TracingRender::sampleLight(Interaction& light ,float& pdf) {
    // todo 仅考虑一个光源
    
    // for (int i=0; i < objects.size(); i++) {
    //     if (objects[i].hasEmit()) {
    //         objects[i].sampleSphereUniform(light,pdf);
    //     }
    // }
    
}
// scene::intersect
// bool TracingRender::intersect(Ray &ray) {

//     return this->bvh->Intersect(ray);
    // // ray 
    // Interaction ret;
    
    // float t = 1e10;
    // float tMin = t;
    // float index = 0;
    // for (int i = 0; i < objects.size(); i++) {
    //     bool flag = objects[i].intersect(ray, t);
        
    //     if (flag && t < tMin) {
            
    //         index = i;
    //         tMin = t;
    //     }
    // }

    // if (tMin == 1e10) {
    //     ret.flag = false;
    //     return ret;
    // }
    // ret.flag = true;
    // ret.hitPoint = ray.o + ray.dir * tMin;
    // ret.hitObject = &objects[index];
    // ret.normal = normalize(ret.hitPoint - ret.hitObject->o);
    // ret.distance = tMin;
    
    // return ret;
// }

// Vector3f TracingRender::getRadiance(Ray &ray, int bounce) {
//     if (bounce > maxBounce) {
//         return Vector3f(0.0);
//     }

//     Interaction interaction = castRay(ray);
//     if (!interaction.flag) {
//         // background
//         return background;
//     }

//     // 1. mode witte style
//     // 1. phone style specular diffuse + fresnel

//     Vector3f ret(0.0,0,0);
//     Vector3f hitPoint = interaction.hitPoint;
//     Sphere* hitOBject = interaction.hitObject;
//     Vector3f N;
//     hitOBject->getSurfaceProperties(hitPoint, N);
//     if (hitOBject->reflectType == utils::REFLECTION_AND_REFRACTION) {
//         float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
//         Vector3f reflectRayDir = normalize(reflect(ray.dir, N));
//         Vector3f reflectRayOri = Dot(reflectRayDir,N) > 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

//         Vector3f refractRayDir = normalize(refract(ray.dir, N, hitOBject->ior));
//         Vector3f refractRayOri = Dot(refractRayDir,N) > 0? hitPoint + N * EPS : hitPoint - N * EPS;

//         Ray reflectRay(reflectRayOri, reflectRayDir);
//         Ray refractRay(refractRayOri, refractRayDir);
//         ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient + getRadiance(refractRay, bounce + 1) * (1 - fresnelCoeffient);
//     } else if (hitOBject->reflectType == utils::REFLECTION) {
//         float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
//         Vector3f reflectRayDir = normalize(reflect(ray.dir, N));
//         Vector3f reflectRayOri = Dot(reflectRayDir, N)> 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

//         Ray reflectRay(reflectRayOri, reflectRayDir);
//         ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient;
//     } else if (hitOBject->reflectType == utils::DIFFUSE) {
//         for (Light light : lights) {
//             Vector3f lightDir = ( light.pos - hitPoint);
//             float lightDistance2 = lightDir.lengthSquared();
//             lightDir = normalize(lightDir);
            
//             Vector3f outgoingDir = reflect(lightDir, N);
//             Vector3f halfVector = normalize(outgoingDir + lightDir);
//             // polygon offset 类似shadow map避免精度问题
//             Vector3f temporigin = Dot(ray.dir, N) < 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
//             Ray shadowRay(temporigin, lightDir);
//             Interaction shadowRayInteraction = castRay(shadowRay);
//             float diffuseCoefficient = std::max(Dot(lightDir, N), 0.f) ;
//             if (shadowRayInteraction.flag) {
//                 Vector3f tempShadowRayDir = shadowRay.o - shadowRayInteraction.hitPoint;
//                 float tempShandowRayDistance2 = tempShadowRayDir.lengthSquared();
//                 if (tempShandowRayDistance2 < lightDistance2) {
//                     // shadow without diffuse
//                     diffuseCoefficient = 0;
//                 }
//             }

//             Vector3f specularColor = light.intensity * std::pow(Dot(halfVector,N),hitOBject->specularExponent); 

//             if (diffuseCoefficient == 0) {
//                 Vector3f foo;
//             }
//             Vector3f diffuseRadiance = hitOBject->diffuseColor * diffuseCoefficient * hitOBject->kd;
//             Vector3f specularRadiance =  specularColor * hitOBject->ks;
//             ret += diffuseRadiance + specularRadiance;
//         }
//     }

//     return ret;
//     // return Vector3f(1.0,0.0,0.0);

//     // return radiance
// }

Vector3f TracingRender::pathTracing(Ray &ray, int depth) {
    Vector3f ret(0.0);
//     Vector3f indirectRadiance(0.0);
//     Vector3f directRadiance(0.0);
    
//     Interaction interaction = castRay(ray);

//     if (!interaction.flag) {
//         ret = background;
//         return ret;
//     }

//     Sphere* hitObject = interaction.hitObject;
//     // 打到光上了直接过
//     if (hitObject->hasEmit()){
//         return hitObject->emit;
//     }
//     // p incident point
//     Vector3f p = interaction.hitPoint;
//     Vector3f N = interaction.normal;
//     Vector3f L_wo = normalize(-ray.dir);
//     if (hitObject->reflectType == utils::DIFFUSE) {

//         float pdf_light;
//         // 在光源上随机采个样
//         Interaction inter_light;
//         // 方位角随机数  pdf 就是整个面积的uniform没有特殊处理
//         // 均匀对于半球进行采样 按照pdf 进行N次采样
//         this->sampleLight(inter_light,pdf_light);
//         // castRay 看是否有intersection
//         // 随机采样出来的射线 从p点到x 方向
//         Vector3f x = inter_light.hitPoint;
//         Vector3f L_wi_origin = x - p;
//         Vector3f L_wi = normalize(L_wi_origin);
//         Vector3f L_wi_N = normalize(inter_light.normal);

//         // 直接光照，对area进行采样
//         Vector3f L_dir_Li = inter_light.emit;
//         Vector3f brdf = hitObject->evalBRDF(L_wo,N);
//         // 0就是光源照不到
//         float cosIncidentPoint = std::max(0.f, Dot(N,L_wi));
//         // 光源转到正向
//         float cosLight = std::max(0.f, Dot(L_wi_N,-L_wi));

//         float daTodw = (cosLight) / (L_wi_origin.lengthSquared()) / pdf_light;
        
//         // todo 直接光照中间被挡住
//         Ray testShadowray(p,L_wi);
//         Interaction shadowray = castRay(testShadowray);
//         if (shadowray.flag && (shadowray.hitPoint - inter_light.hitPoint).length() < EPS) {
//             directRadiance = L_dir_Li * brdf * cosIncidentPoint * daTodw;
//         }
        
//         // 间接光照部分
        
//         float rr =0.8f;
//         float randomValue = getRandom(0,1);
//         if (randomValue > rr) {

//             float pdf_indir = 1 / (PI * 2);
//             Vector3f L_indir_wo = p + getVecFromSampleSphereUniform();
//             Ray indir_ray(p, L_indir_wo);
//             Vector3f indir_brdf = hitObject->evalBRDF(L_indir_wo,N);
//             float indir_cosWiN = std::max(0.f, Dot(L_indir_wo,N));

//             indirectRadiance = pathTracing(indir_ray, depth++) * indir_brdf * indir_cosWiN / pdf_indir / rr;
//         }
        
//         ret = directRadiance + indirectRadiance;
//         return ret;
//     }

    return ret;

}

void TracingRender::render(const Scene &scene)
{
    
    
    int width = scene.width;
    int height = scene.height;
    int depth = 0;
    std::vector<Vector3f> frameBuffer(width * height);
    Vector3f cam(10, 10, -150);
    float scale = tan(deg2rad(scene.fov * 0.5));  
    float imageRadio = scene.aspect;

    #pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Vector3f radiance;

            float x = 2.0 * (((i + 0.5f) / width) - .5f) * imageRadio * scale;
            float y = 2.0 * (((j + 0.5f) / height) - .5f) * scale;
            Vector3f _dir(x,y,1);
            Vector3f dir = normalize(_dir);
            Ray ray(cam,dir);

            int index = getIndex(i,j,width,height);

            for (int k = 0; k < scene.samples; k++) {
                // Vector3f _radiance = getRadiance(ray,0);
                // todo gama normliaz
                Vector3f _radiance =  pathTracing(ray,depth);
                radiance += _radiance;
            }
            frameBuffer[index] = radiance / scene.samples;
        }
    }
    exportImg(frameBuffer, width,height);
}

}