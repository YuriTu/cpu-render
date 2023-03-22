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
        auto hg = std::make_shared<HenyeyGreenstein>(g);
        *mi = MediumInteraction(ray(t),-ray.d,this,hg);
    }
    
    Vector3f Tr = EXP(-sigma_t * std::min(t * ray.d.length(), MAXFloat));
    // dLo = sigma_t * (Li) * tr
    Vector3f density = sampleMedium ? (sigma_t * Tr) : Tr;

    int channelCount = 3;
    float pdf = 0;
    for (int i = 0; i < channelCount; i++) {
        pdf += density[i];
    }
    pdf *= 1.f / (float)channelCount;

    if (sampleMedium) {
        beta = Tr * sigma_s / pdf;
    } else {
        beta = Tr / pdf;
    }

    if (beta.lengthSquared() > 1e4 || beta.lengthSquared() < EPSILON) {
        printf("error medium");
    }

    return beta;
}

float HenyeyGreenstein::p(const Vector3f &wo, const Vector3f &wi) const{
    return PhaseHG(Dot(wo, wi), g);
}

float HenyeyGreenstein::Sample_p(const Vector3f &wo, Vector3f *wi, const Vector3f &u) const{
    float cosTheta;
    if (std::abs(g) < 1e-3) {
        // g =0 的时候 随机在2pi (cos = [-1, 1])中取一个方向
        cosTheta = 1 - 2 * u[0];
    } else {
        float g2 = g *g;
        float sqrTerm = (1 - g2) / (1 + g - 2*g*u[0]);
        cosTheta = - (1 + g2 - sqrTerm * sqrTerm ) / (2 * g);
    }

    // cos phi平面角转方位角
    float sinTheta = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
    float phi = 2 * PI * u[1];
    Vector3f v1,v2;
    CoordinateSystem(wo, &v1, &v2);
    // wo作为z是为了 转化为平面 
    *wi = SphericalDirection(sinTheta, cosTheta,phi, v1,v2,wo);
    return PhaseHG(cosTheta, g);
}

}