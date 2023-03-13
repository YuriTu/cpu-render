#ifndef RT_INTERACTION_H
#define RT_INTERACTION_H
#include "r.h"
#include "geometry.h"
#include "material.h"
#include "mesh.h"

namespace r
{

class Interaction
{
public:
    Interaction();
    Ray spawnRay(const Vector3f &d) const;

    double distance;

    Vector3f p;
    Vector3f n;
    Vector3f wo;

};

class SurfaceInteraction : public Interaction
{   
public:
    SurfaceInteraction();
    SurfaceInteraction(const Vector3f &p, const Vector3f &n,const Vector3f &wo);
    Vector3f Le() const;
    void ComputeScatteringFunction(const Ray &ray);
    Mesh* primitive;
    Vector3f bsdf;
};


// struct MediumInteraction
// {
//     MediumInteraction(){
        
//     }
// };



}
#endif //RAYTRACING_INTERACTION_H
