#ifndef RT_MESH_H
#define RT_MESH_H

#include "r.h"
#include "utils.h"
#include "interaction.h"
#include "bounds3.h"


namespace r
{
    class Mesh
    {
    public:
        Mesh() = default;
        virtual ~Mesh() = default;
        virtual Bounds3 getBounds() = 0;
        // virtual void sample(Interaction &interaction, float &pdf) = 0;
        virtual bool intersect(const Ray& ray, Interaction *interaction) = 0;
        virtual float getArea() = 0;
        virtual void ComputeScatteringFunction(Interaction *isect) const = 0;
        float area;
        Material* material;
        
    };
    
    

    // class Sphere
    // {
    // public:
    //     Sphere(Vector3f _o, float _r, Vector3f _c, reflectType _t);
    //     bool intersect(Ray &r, float& tNear);
    //     void getSurfaceProperties(Vector3f &hitPoint, Vector3f &N);
    //     // void sampleSphereUniform(Interaction& ret, float& pdf);
    //     Vector3f evalBRDF(const Vector3f& wo, const Vector3f& N);
    //     bool hasEmit();
    //     Vector3f o;
    //     float radius;
    //     float radius2;
    //     Vector3f color;
    //     reflectType reflectType;
    //     // b-phone mode
    //     Vector3f diffuseColor;
    //     float kd;
    //     float ks;
    //     float specularExponent;
    //     // reflect
    //     float ior;
    //     float area;
    //     Vector3f emit;

    // };

    class Light
    {
    public:
        Light(Vector3f _pos, float _int):pos(_pos), intensity(_int){};

        Vector3f pos;


        float intensity;
    };
} // namespace r



#endif