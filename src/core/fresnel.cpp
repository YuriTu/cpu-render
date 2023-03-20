#include "fresnel.h"
#include "utils.h"

namespace r 
{

// 根据fresnel equation 计算 fr/tr的能量比
float FrDielectric(float cosThetaI, float etaI, float etaT) {
    // <0 为 outside to inside 做一次归一化
    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // snell law: etaI * sinI = etaT * sinT
    float sinThetaI = std::sqrt(std::max(0.f, 1 - cosThetaI * cosThetaI));
    float sinThetaT = etaI / etaT * sinThetaI;

    // 没有tr的情况
    if (sinThetaT >= 1) return 1.f;

    float cosThetaT = std::sqrt(std::max(0.f, 1 - sinThetaT * sinThetaT));

    float r1 = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                ((etaT * cosThetaI) + (etaI * cosThetaT));
    float r2 = ((etaI * cosThetaT) - (etaT * cosThetaI)) /
                ((etaI * cosThetaT) + (etaT * cosThetaI));
    return (r1 * r1 + r2 * r2) / 2.f;
}

Vector3f FresnelDielectric::evaluate(float cosThetaI) const {
    return FrDielectric(cosThetaI, etaI, etaT);
}

Vector3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, float *pdf) const {
    *wi = CosineSampleHemisphere();
    if (wo.z < 0.f) {
        wi->z *= -1;
    }
    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

// warning 注意人家做了坐标系转换简化了计算，我们还没有做
float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const {
    // wi方向上的PDF是一个以法线为中心的圆形分布
    // 所以需要 位置 / 总面积（pi）
    return SameHemisphere(wi,wo) ? AbsCosTheta(wi) * InvPi : 0;
}

Vector3f FresnelSpecular::f(const Vector3f &wo, const Vector3f &wi) const {
    return Vector3f(0.f);
}

Vector3f FresnelSpecular::Sample_f(const Vector3f &wo, Vector3f *wi, float *pdf) const {
    float u = getRandom(0.8,1.0);
    float F = FrDielectric(CosTheta(wo), etaI, etaT);

    if (u < F) {
        // reflect
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        *pdf = F;
        return F * R / AbsCosTheta(*wi);
    } else {
        // tr
        bool entering = CosTheta(wo) > 0;
        float _etaI = entering ? etaI : etaT;
        float _etaT = entering ? etaI : etaT;

        if (!Refract(wo, Vector3f(0.f), _etaI / _etaT, wi)){
            return Vector3f(0.f);
        }
        Vector3f ft = T * (1 - F);

        *pdf = 1 - F;
        // radiance?
        ft *= (_etaI * _etaI) / (_etaT * _etaT);
        return ft / AbsCosTheta(*wi);
    }
}

float FresnelSpecular::Pdf(const Vector3f &wo, const Vector3f &wi) const {
    return 0.f;
}

Vector3f LambertianReflection::f(const Vector3f &wo, const Vector3f &wi) const {
    return R * InvPi;
}

}