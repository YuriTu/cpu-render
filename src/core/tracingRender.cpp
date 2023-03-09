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


void TracingRender::sampleLight(Interaction& light ,float& pdf) {
    // todo 仅考虑一个光源
    
    // for (int i=0; i < objects.size(); i++) {
    //     if (objects[i].hasEmit()) {
    //         objects[i].sampleSphereUniform(light,pdf);
    //     }
    // }
    
}

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

Vector3f TracingRender::Li(Ray &ray, const Scene &scene) {
    Vector3f radiance(0.f);
    Vector3f indirectRadiance(0.0);
    Vector3f directRadiance(0.0);
    int depth = 0;
    int bounces;
    // tr系数
    Vector3f beta(1.f);
    //todo 根据radiacen 质量做terminal 去掉具体的samples
    for (bounces = 0; bounces < scene.samples; bounces++) {
        // scene.intersect(ray,)
        Interaction isect;
        bool foundIntersection = scene.intersect(ray, &isect);
        bool foundMediumIntersection = false;

        if (foundMediumIntersection) {

        } else {
            if (!foundIntersection) {
                directRadiance = scene.background;
                break;
            }

            // emission term
            //  只考虑direct的情况
            if (bounces == 0 ) {
                directRadiance += isect.Le();
            }

            // 计算bsdf的情况
            isect.ComputeScatteringFunction(ray);

            // 对于光源进行采样
                // 得到 emission 和 cos 
            // 计算radiance 
            directRadiance += isect.bsdf;

            // 采样bsdf 准备下一次bounce
        }
        // rr 只在indiecrt处理
    }
    radiance = directRadiance + indirectRadiance;
    return radiance;
}

void TracingRender::render(const Scene &scene)
{
    
    int width = scene.width;
    int height = scene.height;
    
    std::vector<Vector3f> frameBuffer(width * height);
    Vector3f cam = scene.camPos;
    float scale = tan(deg2rad(scene.fov * 0.5));  
    float imageRadio = scene.aspect;

    #pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            float x = (2.0 * (i + 0.5f) / (float)width - 1.f) * imageRadio * scale;
            float y =(2.0 * (j + 0.5f) / (float)height - 1.f) * scale;
            Vector3f _dir(-x,y,1);
            Vector3f dir = normalize(_dir);
            Ray ray(cam,dir);
            int index = getIndex(i,j,width,height);
            Vector3f radiance = this->Li(ray,scene);
            // printf("now: x:%i,y%i| li:%f,%f,%f \n",i,j,radiance.x,radiance.y,radiance.z);
            frameBuffer[index] = radiance / scene.samples;
        }
    }
    exportImg(frameBuffer, width,height);
}

}