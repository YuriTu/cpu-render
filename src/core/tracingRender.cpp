#include "tracingRender.h"
#include "utils.h"
#include "interaction.h"

#include <omp.h>

namespace r{

TracingRender::TracingRender(int w, int h): width(w),height(h) {
}

Vector3f TracingRender::uniformSampleOneLight(Interaction &isect,const Scene &scene) {\
    // 先只考虑一个光源的标准情况
    int nLight = 0;
    return estimateDirect(isect, scene, scene.lights[nLight]);
}

Vector3f TracingRender::estimateDirect(Interaction &isect,const Scene &scene, std::shared_ptr<Mesh> light) {
    // sample light
    Vector3f L = Vector3f();
    Vector3f Le = Vector3f();
    float pdf;
    Interaction light_isect;
    light->Sample(light_isect, pdf);

    Vector3f wi_origin = light_isect.p - isect.p;
    Vector3f wi = normalize(wi_origin);
    Vector3f light_normal = light_isect.n;
    Vector3f p_normal = isect.n;
    // todo 后面要考虑直接sample到light的情况

    if (!light_isect.primitive) {
        return L;
    }
    Le = light_isect.primitive->getMaterial()->getEmission();
    // light 的cos ，反一下从light出发的向量
    float light_cosTheta = AbsDot(-wi,light_normal);
    float dw2da = light_cosTheta / wi_origin.lengthSquared();

    float cosTheta = SafeDot(wi,p_normal);
    
    L = (Le *  dw2da / pdf) * cosTheta;
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
    for (bounces = 0; bounces < scene.maxDepth; bounces++) {
        // scene.intersect(ray,)
        Interaction isect;
        bool foundIntersection = scene.intersect(ray, &isect);
        bool foundMediumIntersection = false;

        if (beta.isBlack()) break;

        if (foundMediumIntersection) {

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
                    printf("bound 0 not found!");
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
            directRadiance += beta * irradiance * isect.bsdf;

            // 采样bsdf 准备下一次bounce
            Vector3f wo = - ray.d;
            float pdf;
            // isect.bsdf sample(wi)
            Vector3f wi =  isect.primitive->getMaterial()->sample(wo,isect.n,pdf);
            float cosTheta = AbsDot(wi,isect.n);
            // 这里还要算 间接光照的brdf 反射能量，先用之前的撮合一个
            // beta / pdf
            Vector3f temp = isect.bsdf * ( cosTheta / pdf );
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
            // if (i == 5 && j == 28) {
            //     printf("debug");
            // }
            frameBuffer[index] = radiance / spp;
        }
    }
    exportImg(frameBuffer, width,height);
}

}