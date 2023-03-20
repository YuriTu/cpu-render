#ifndef RT_INTERACTION_H
#define RT_INTERACTION_H
#include "r.h"
#include "geometry.h"
#include "material.h"
#include "mesh.h"
#include "medium.h"

namespace r
{

class Interaction
{
public:
    Interaction();
    Interaction(const Vector3f &p, const Vector3f &wo, 
        const MediumInterface &mediumInterface);

    Ray spawnRay(const Vector3f &d) const;
    bool isSurfaceInteraction() const;
    bool isMediumInteraction() const;
    const Medium* getMedium(const Vector3f &w) const;
    const Medium* getMedium() const;

    double distance;

    Vector3f p;
    Vector3f n;
    Vector3f wo;

    MediumInterface mediumInterface;
};

class SurfaceInteraction : public Interaction
{   
public:
    SurfaceInteraction();
    SurfaceInteraction(const Vector3f &p, const Vector3f &n,const Vector3f &wo);
    Vector3f Le() const;
    void ComputeScatteringFunction(const Ray &ray);
    Mesh* primitive;
    BxDF bsdf;
};

class MediumInteraction : public Interaction
{
public:
    MediumInteraction() : phase(nullptr){}
    MediumInteraction(const Vector3f &p, const Vector3f &wo,
                     const Medium *medium, const PhaseFunction *phase)
        : Interaction(p,wo, medium), phase(phase){}
    bool isVaild() const { return phase != nullptr; }
    
    const PhaseFunction *phase;
};

}
#endif //RAYTRACING_INTERACTION_H
