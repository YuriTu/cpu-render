#include "tracingRender.h"
#include "utils.h"
#include "interaction.h"
#include "fresnel.h"

#include <omp.h>

namespace r{

TracingRender::TracingRender(int w, int h): width(w),height(h) {
}

Vector3f TracingRender::uniformSampleOneLight(const Interaction &isect,const Scene &scene) {\
    // 先只考虑一个光源的标准情况
    int nLight = 0;
    return estimateDirect(isect, scene, scene.lights[nLight]);
}

Vector3f TracingRender::estimateDirect(const Interaction &isect,const Scene &scene, std::shared_ptr<GeometricPrimitive> light) {
    // sample light
    Vector3f L = Vector3f();
    Vector3f Le = Vector3f();
    // light pdf
    float lightPdf;
    SurfaceInteraction light_isect;
    light->Sample(light_isect, lightPdf);

    Vector3f wi_origin = light_isect.p - isect.p;
    Vector3f wi = normalize(wi_origin);
    Vector3f light_normal = light_isect.n;
    Vector3f p_normal = isect.n;
    // todo 后面要考虑直接sample到light的情况

    if (!light_isect.primitive || lightPdf <= 0.f) {
        return L;
    }

    Le = light_isect.primitive->getMaterial()->getEmission();
    // light 的cos ，反一下从light出发的向量
    float light_cosTheta = AbsDot(-wi,light_normal);
    float dw2da = light_cosTheta / wi_origin.lengthSquared();
    float cosTheta = SafeDot(wi,p_normal);

    const SurfaceInteraction &it = (const SurfaceInteraction &)isect;
    Vector3f brdf = it.bsdf->f(wi,it.wo);

    // Li * brdf * cos\omega
    L = (Le *  dw2da / lightPdf)  * brdf * cosTheta;
    return L;
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
    // todo 这种for的方法复合材质不好弄
    for (bounces = 0; bounces < scene.maxDepth; bounces++) {
        // scene.intersect(ray,)
        SurfaceInteraction isect;
        bool foundIntersection = scene.intersect(ray, &isect);
        MediumInteraction mi;
        if (ray.medium) beta = beta * ray.medium->Sample(ray, &mi);

        if (beta.isBlack()) break;

        if (mi.isVaild()) {
            // medium term
            Vector3f irradiance = this->uniformSampleOneLight(isect, scene);

            // 采样phase 准备下次bounce
            Vector3f wo = -ray.d;
            Vector3f wi;
            Vector3f u = Vector3f(getRandom(), getRandom(),0.f);
            mi.phase->Sample_p(wo, &wi, u);
            ray = mi.spawnRay(wi);

        } else {
            // emission term
            //  只考虑direct的情况
            if (bounces == 0 ) {
                if (foundIntersection) {
                    directRadiance += beta * isect.Le();
                    // todo 先默认只有light 才有emission
                    if (directRadiance > Vector3f()){
                        return directRadiance;
                    }    
                } else {
                    directRadiance = scene.background;
                    break;
                }
            }

            if (!foundIntersection) {
                break;
            }

            // 计算bsdf的情况
            isect.ComputeScatteringFunction(ray);

            // 对于光源进行采样 转化为p的da积分
            Vector3f irradiance = this->uniformSampleOneLight(isect, scene);
            // 计算radiance 
            directRadiance += beta * irradiance;

            // 采样bsdf 准备下一次bounce
            Vector3f wo = - ray.d;
            float pdf;
            // isect.bsdf sample(wi)
            //warning 如果是medium材质，这里要做处理 
            Vector3f wi =  isect.primitive->getMaterial()->sample(wo,isect.n,pdf);
            float cosTheta = AbsDot(wi,isect.n);
            // 这里还要算 间接光照的brdf 反射能量，先用之前的撮合一个
            // beta / pdf
            Vector3f temp = isect.bsdf->f(wi,wo) * ( cosTheta / pdf );
            beta = temp * beta;
            ray = isect.spawnRay(wi);
        }
        // rr 
        float randomValue = getRandom();
        if (randomValue < scene.rrThreshold) {
            beta = beta / scene.rrThreshold;
        } else {
            break;
        }
        // printf("bounce %i beta:%f %f %f \n",bounces, beta.x,beta.y,beta.z);
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
            int index = getIndex(i,j,width,height);
            Vector3f radiance;
            int spp = scene.samples;
            
            for (int k = 0; k < spp; k++) {
                Ray ray(cam,dir);
                radiance += this->Li(ray,scene);
            }
            
            // printf("now: x:%i,y%i| li:%f,%f,%f \n",i,j,radiance.x,radiance.y,radiance.z);
            // if (i == 5 && j == 5) {
            //     printf("debug");
            // }
            frameBuffer[index] = radiance / spp;
        }
    }
    exportImg(frameBuffer, width,height);
}

}