#ifndef RT_MESH_H
#define RT_MESH_H

#include "utils.h"
#include "interaction.h"

namespace r
{
    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;
    private:
        std::vector<Vector3f> data;
    };
    
    
    
    class triangle: public Mesh
    {
    public:
        triangle(Vector3f a, Vector3f b, Vector3f c): data({a,b,c}){};
        // ~triangle();
        std::vector<Vector3f> getData();
        std::vector<Vector3f> getColor();
        void setColor(Vector3f c1);
        void setColor(Vector3f c1, Vector3f c2, Vector3f c3);
        
    private:
        std::vector<Vector3f> data;
        std::vector<Vector3f> color = {Vector3f(1),Vector3f(1),Vector3f(1)};
    };

    class Sphere
    {
    public:
        Sphere(Vector3f _o, float _r, Vector3f _c, utils::reflectType _t);
        bool intersect(Ray &r, float& tNear);
        void getSurfaceProperties(Vector3f &hitPoint, Vector3f &N);
        // void sampleSphereUniform(Interaction& ret, float& pdf);
        Vector3f evalBRDF(const Vector3f& wo, const Vector3f& N);
        bool hasEmit();
        Vector3f o;
        float radius;
        float radius2;
        Vector3f color;
        utils::reflectType reflectType;
        // b-phone mode
        Vector3f diffuseColor;
        float kd;
        float ks;
        float specularExponent;
        // reflect
        float ior;
        float area;
        Vector3f emit;

    };

    class Light
    {
    public:
        Light(Vector3f _pos, float _int):pos(_pos), intensity(_int){};

        Vector3f pos;


        float intensity;
    };
} // namespace r



#endif