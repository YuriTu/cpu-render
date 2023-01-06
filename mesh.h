#pragma once
#include "utils.h"
namespace r
{
    class mesh
    {
    public:
        mesh() = default;
        ~mesh() = default;
        int size() {
            return data.size();
        }
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
        Sphere(Vector4f _o, float _r, Vector4f _c):o(o), radius(_r), color(_c) {};
        float intersect(Ray &r);
        Vector4f o;
        float radius;
        Vector4f color;
    };
    
} // namespace r