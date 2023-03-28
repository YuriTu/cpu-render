#include "interaction.h"

namespace r 
{

Interaction::Interaction() {
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
}

Interaction::Interaction(const Vector3f &p, const Vector3f &wo, const MediumInterface &mediumInterface)
    :p(p), wo(wo), mediumInterface(mediumInterface)
{
    n = Vector3f();
    distance= std::numeric_limits<double>::max();
}

bool Interaction::isSurfaceInteraction() const {
    return n != Vector3f();
}

bool Interaction::isMediumInteraction() const {
    return !isSurfaceInteraction();
}

const Medium* Interaction::getMedium(const Vector3f &w) const {
    if (Dot(w,n) > 0) {
        return mediumInterface.outside;
    } else {
        return mediumInterface.inside;
    }
}

const Medium* Interaction::getMedium() const {
    return mediumInterface.inside;
}

Ray Interaction::spawnRay(const Vector3f &d) const {
    Vector3f o = this->p;
    return Ray(o,d, Infinity, getMedium(d));
}

Ray Interaction::spawnRayTo(const Interaction &it) const {
    Vector3f o = offsetRayOrigin(this->p, this->n, it.p - this->p);
    Vector3f target = offsetRayOrigin(it.p, it.n, o - it.p);
    Vector3f d = normalize( target - o );
    // fixme tmax这么设计应该是为了限定为一个单位长度
    return Ray(o, d,1 - EPSILON, getMedium(d));
}

// ------------ SurfaceInteraction -------------

SurfaceInteraction::SurfaceInteraction() {
    p = Vector3f();
    n = Vector3f();
    wo = Vector3f();
    distance= std::numeric_limits<double>::max();
    primitive = nullptr;
    bsdf = nullptr;
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

}