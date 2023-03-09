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
    Interaction(bool h);

    Vector3f Le() const;
    void ComputeScatteringFunction(const Ray &ray);
    Ray spawnRay(const Vector3f &d) const;

    bool happened;
    double distance;

    Vector3f p;
    Vector3f n;
    Vector3f wo;

    Mesh* primitive;
    Vector3f bsdf;
};

// struct SurfaceInteraction 
// {
//     SurfaceInteraction(){

//     }
// };


// struct MediumInteraction
// {
//     MediumInteraction(){
        
//     }
// };



}
#endif //RAYTRACING_INTERACTION_H
