#pragma once

#include "utils.h"
#include "mesh.h"

namespace r
{
    class TracingRender
    {
    public:
        TracingRender() = default;
        TracingRender(int w, int h);
        ~TracingRender() = default;

        void setPerspectiveProjection(float near, float far, float aspect, float fov);
        void setModule(Vector4f position);
        void setView(Vector4f position);
        void add(Sphere &m);
        void render();
        Vector4f getRadiance(Ray &ray);


        int width;
        int height;
        std::vector<Vector4f> frameBuffer;
        std::vector<float> depthBuffer;
        std::vector<Sphere> objects;
        Vector4f background;
        
        
    private:
        void rasterization();
        bool castRay(Ray &ray);
    };
}