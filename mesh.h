#pragma once
#include "utils.h"
namespace r
{
    class mesh
    {
    public:
        mesh() = default;
        ~mesh() = default;
    private:
        std::vector<Vector4f> data;
    };
    
    
    
    class triangle: public mesh
    {
    public:
        triangle(Vector4f a, Vector4f b, Vector4f c): data({a,b,c}){};
        // ~triangle();
        std::vector<Vector4f> getData();
        std::vector<Vector4f> getColor();
        void setColor(Vector4f c1);
        void setColor(Vector4f c1, Vector4f c2, Vector4f c3);
        
    private:
        std::vector<Vector4f> data;
        std::vector<Vector4f> color = {Vector4f(1),Vector4f(1),Vector4f(1)};
    };

    class Sphere
    {
    public:
        Sphere(Vector4f _o, float _r, Vector4f _c, utils::reflectType _t);
        bool intersect(Ray &r, float& tNear);
        void getSurfaceProperties(Vector4f &hitPoint, Vector4f &N);
        void sampleSphereUniform(Vector4f& wo, float& pdf);
        Vector4f o;
        float radius;
        Vector4f color;
        utils::reflectType reflectType;
        // b-phone mode
        Vector4f diffuseColor;
        float kd;
        float ks;
        float specularExponent;
        // reflect
        float ior;
        float area;
    };

    class Light
    {
    public:
        Light(Vector4f _pos, float _int):pos(_pos), intensity(_int){};

        Vector4f pos;


        float intensity;
    };

    
} // namespace r
