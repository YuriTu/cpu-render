#ifndef R_MEDIUM_H
#define R_MEDIUM_H

#include "r.h"
#include "geometry.h"

namespace r {

class Medium {
public:
    Medium() = default;
    virtual ~Medium(){}
    virtual Vector3f Tr(const Ray &ray) const = 0;
    virtual Vector3f Sample(const Ray &ray,MediumInteraction *mi) const = 0;
};

class HomogeneousMedium : public Medium {
public:
    HomogeneousMedium(const Vector3f &sigma_a, const Vector3f &sigma_s, float g) 
        : sigma_a(sigma_a), sigma_s(sigma_s), sigma_t(sigma_s + sigma_a), g(g) {}
    Vector3f Tr(const Ray &ray) const;
    Vector3f Sample(const Ray &ray, MediumInteraction *mi) const;
    
private:
    const Vector3f sigma_a,sigma_s,sigma_t;
    const float g;

};

struct MediumInterface
{
    MediumInterface() : inside(nullptr), outside(nullptr){}
    MediumInterface(const Medium *medium) : inside(medium), outside(medium) {}
    MediumInterface(const Medium *inside, const Medium *outside) : inside(inside), outside(outside) {}
    
    bool isMediumTransition() const {
        return inside != outside;
    }

    const Medium *inside, *outside;
};



class PhaseFunction {
public:
    virtual ~PhaseFunction(){};
    virtual float p(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual float Sample_p(const Vector3f &wo, Vector3f *wi, const Vector3f &u) const = 0;
};

class HenyeyGreenstein : public PhaseFunction {
public:
    HenyeyGreenstein(float g): g(g) {}
    float p(const Vector3f &wo, const Vector3f &wi) const;
    float Sample_p(const Vector3f &wo, Vector3f *wi, const Vector3f &u) const;

private:
    const float g;

};
}

#endif