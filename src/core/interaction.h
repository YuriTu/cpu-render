#ifndef RT_INTERACTION_H
#define RT_INTERACTION_H
#include "geometry.h"
#include "material.h"
// #include "mesh.h"

namespace r
{

class Interaction
{
public:
    Interaction();
    Interaction(bool h);

    Vector3f Le() const;
    void ComputeScatteringFunction(const Ray &ray);

    bool happened;
    Vector3f coords;
    Vector3f tcoords;
    Vector3f normal;
    Vector3f emit;
    double distance;
    Material* m;
    const Mesh* primitive;
    Vector3f bsdf;
    
};

// struct SurfaceInteraction 
// {
//     SurfaceInteraction(){

//     }
// };

// struct Interaction
// {
//     Interaction() {
//         flag = false;
//         hitPoint = Vector3f();
//         hitObject = nullptr;
//     }
//     bool flag;
//     Vector3f hitPoint;
//     Vector3f normal;
//     float t;
//     r::Sphere* hitObject;
//     float distance;
//     Vector3f emit;

// };

// struct MediumInteraction
// {
//     MediumInteraction(){
        
//     }
// };



}
#endif //RAYTRACING_INTERACTION_H
