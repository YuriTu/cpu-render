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
        std::vector<Vector4f> data;
    };
    
    
    
    class triangle: public mesh
    {
    public:
        triangle(Vector4f a, Vector4f b, Vector4f c): data({a,b,c}){};
        std::vector<Vector4f> data;
        // ~triangle();
    };
    
} // namespace r
