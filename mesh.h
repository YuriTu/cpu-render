#pragma once
#include "utils.h"
namespace r
{
    class mesh
    {
    public:
        mesh() = default;
        ~mesh() = default;
    protected:
        std::vector<Vector4f> data;
    };
    
    
    mesh::mesh(/* args */)
    {
    }
    
    mesh::~mesh()
    {
    }

    class triangle: public mesh
    {
    protected:
        std::vector<Vector4f> data;
    public:
        triangle(Vector4f a, Vector4f b, Vector4f c): data({a,b,c}){};
        // ~triangle();
    };
    
} // namespace r
