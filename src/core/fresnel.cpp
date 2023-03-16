#include "fresnel.h"

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

}