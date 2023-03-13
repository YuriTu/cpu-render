#include "interaction.h"

namespace r 
{

Interaction::Interaction() {
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
}

Ray Interaction::spawnRay(const Vector3f &d) const {
    Vector3f o = this->p;
    return Ray(o,d);
}

// ------------ SurfaceInteraction -------------

SurfaceInteraction::SurfaceInteraction() {
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
    primitive = nullptr;
    bsdf = Vector3f(0.0);
}

SurfaceInteraction::SurfaceInteraction(const Vector3f &_p, const Vector3f &_n,const Vector3f &_wo) {
    p = _p;
    n = _n;
    wo = _wo;
    distance= std::numeric_limits<double>::max();
}

Vector3f SurfaceInteraction::Le() const{
    Vector3f rs(0.f);
    if (primitive->getMaterial()) {
        // todo 这里要考虑一下cos 背面insect light 不能贡献能量
        rs = primitive->getMaterial()->getEmission();
    }
    return rs;
}

void SurfaceInteraction::ComputeScatteringFunction(const Ray &ray) {
    return primitive->ComputeScatteringFunction(this);
}

// ------------ MediumInteraction -------------

MediumInteraction::MediumInteraction() {
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
}

}