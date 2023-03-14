#include "medium.h"
#include "utils.h"
#include "interaction.h"

namespace r {

Vector3f HomogeneousMedium::Tr(const Ray &ray) const {
    return EXP(-sigma_t * std::min(ray.tMax * ray.d.length(), MAXFloat));
}

Vector3f HomogeneousMedium::Sample(const Ray &ray, MediumInteraction *mi) const {
    Vector3f beta = Vector3f();
    int channel = std::ceil( getRandom(0.f,2.f) );
    // 粒子会影响的随机最小长度 [0,1] * 平均自由程
    float dist = -std::log( 1 - getRandom() ) / sigma_t[channel];
    // 路程/速度 = 时间
    float t = std::min(dist / ray.d.length(), ray.tMax);

    bool sampleMedium = t < ray.tMax;

    if (sampleMedium) {
        HenyeyGreenstein hg = HenyeyGreenstein(g);
        *mi = MediumInteraction(ray(t),-ray.d,this,&hg);
    }
    
    Vector3f Tr = EXP(-sigma_t * std::min(t * ray.d.length(), MAXFloat));
    // dLo = sigma_t * (Li) * tr
    Vector3f density = sampleMedium ? (sigma_t * Tr) : Tr;

    int channelCount = 3;
    float pdf = 0;
    for (int i = 0; i < channelCount; i++) {
        pdf += density[i];
    }
    pdf *= (float)(1 / channel);

    if (sampleMedium) {
        beta = Tr * sigma_s / pdf;
    } else {
        beta = Tr / pdf;
    }

    return beta;
}

float HenyeyGreenstein::p(const Vector3f &wo, const Vector3f &wi) const{
    return 0.f;
}

float HenyeyGreenstein::Sample_p(const Vector3f &wo, Vector3f *wi, const Vector3f &u) const{
    return 0.f;
}

}