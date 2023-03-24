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
    Vector3f Llight = Vector3f();
    Vector3f Lbsdf = Vector3f();
    Vector3f Le = Vector3f();
    Vector3f brdf = Vector3f();
    float bsdfWeight = 0.f;
    float lightWeight = 0.f;
    // light pdf
    float lightPdf, scatteringPdf;
    SurfaceInteraction light_isect;
    light->Sample(light_isect, lightPdf);

    Vector3f wi_origin = light_isect.p - isect.p;
    Vector3f wi = normalize(wi_origin);
    Vector3f light_normal = light_isect.n;
    Vector3f p_normal = isect.n;

    Le = light_isect.primitive->getMaterial()->getEmission();
    // todo 后面要考虑直接sample到light的情况

    if (Le.isBlack() || lightPdf <= 0.f) {
        return L;
    }

    // light-sampling
    // light 的cos ，反一下从light出发的向量
    float light_cosTheta = AbsDot(-wi,light_normal);
    float dw2da = light_cosTheta / wi_origin.lengthSquared();
    
    if (isect.isSurfaceInteraction()) {
        float cosTheta = SafeDot(wi,p_normal);
        const SurfaceInteraction &it = (const SurfaceInteraction &)isect;
        brdf = it.bsdf->f(wi,it.wo);
        // fixme
        scatteringPdf = brdf.x;

        // Li * brdf * cos\omega
        // L = (Le *  dw2da / lightPdf)  * brdf * cosTheta;

        //fixme 适应下面的操作 
        brdf = dw2da * brdf  * cosTheta;
    } else {
        const MediumInteraction &mi = (const MediumInteraction &)isect;
        float p = mi.phase->p(mi.wo, wi);
        // 这里假设所有的光都是1.0过来的 本来就不会按照surface的cos处理
        brdf = Vector3f(p) * dw2da;
        scatteringPdf = p;
    }

    if (!brdf.isBlack()) {
        // 处理shadow
        // todo 
        if (!Le.isBlack()) {
            // lightWeight = PowerHeuristic(1, lightPdf, 1, scatteringPdf);
            lightWeight = 1.f;
            Llight = Le * brdf * lightWeight / lightPdf;
            //fixme 现在的light area太大，lightpdf的权重太低，先不用了
            return Llight;
        }
    }


    // brdf sampling
    if (isect.isSurfaceInteraction()) {
        const SurfaceInteraction &it = (const SurfaceInteraction &)isect;
        brdf = it.bsdf->Sample_f(it.wo, &wi, &scatteringPdf);
        float cosTheta = AbsDot(wi, isect.n);
        brdf *= cosTheta;
    } else {
        const MediumInteraction &mi = (const MediumInteraction &)isect;
        Vector3f uScatting = getRandom2D();
        float p = mi.phase->Sample_p(mi.wo,&wi, uScatting);
        brdf = Vector3f(p);
        scatteringPdf = p;
    }

    if (!brdf.isBlack()) {
        // get light pdf
        lightPdf = light->Pdf(isect, wi);
        if (lightPdf == 0) {
            L = Llight + Lbsdf;
            return L;
        };
        bsdfWeight = PowerHeuristic(1,scatteringPdf, 1, lightPdf );
        bsdfWeight = 1.f;

        Lbsdf = Le * brdf * bsdfWeight / scatteringPdf;
    }

    L = Llight + Lbsdf;

    return L;
}

Vector3f TracingRender::Li(Ray &r, const Scene &scene) {
    Vector3f radiance(0.f);
    Vector3f indirectRadiance(0.0);
    Vector3f directRadiance(0.0);
    Ray ray = Ray(r);
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
        // fixme 临时处理
        ray.tMax = isect.distance;

        MediumInteraction mi;
        if (ray.medium) beta = beta * ray.medium->Sample(ray, &mi);

        if (beta.hasNaNs() || beta.lengthSquared() > 1e4) {
            printf("beta nans");
        }

        if (beta.isBlack()) break;

        if (mi.isVaild()) {
            // medium term
            Vector3f irradiance = this->uniformSampleOneLight(mi, scene);

            if (irradiance.hasNaNs() || irradiance.lengthSquared() < EPSILON) {
                printf("irradiance nans");
            }
            directRadiance += beta * irradiance;

            // 采样phase 准备下次bounce
            Vector3f wo = -ray.d;
            Vector3f wi;
            float tvalue = getRandom();
            Vector3f u = Vector3f(tvalue, 1- tvalue,0.f);
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
            // 如果是空 material 直接穿过处理
            if (!isect.bsdf) {
                ray = isect.spawnRay(ray.d);
                bounces--;
                continue;
            }

            // 对于光源进行采样 转化为p的da积分
            Vector3f irradiance = this->uniformSampleOneLight(isect, scene);
            // 计算radiance 
            directRadiance += beta * irradiance;

            if (isect.n.isBlack()){
                printf("error n2! \n");
            }

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
            
            // DEBUG_MODE && printf("now: x:%i,y%i| li:%f,%f,%f \n",i,j,radiance.x,radiance.y,radiance.z);
            if (i == 48 && j == 16) {
                printf("debug");
            }
            if (radiance.hasNaNs()) {
                printf("radiance nan!");
            }
            frameBuffer[index] = radiance / spp;
        }
    }
    exportImg(frameBuffer, width,height);
}

}