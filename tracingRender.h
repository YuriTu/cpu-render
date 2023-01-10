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
        void add(Light &l);
        void render();
        Vector4f getRadiance(Ray &ray, int bounce);


        int width;
        int height;
        int maxBounce;
        std::vector<Vector4f> frameBuffer;
        std::vector<float> depthBuffer;
        std::vector<Sphere> objects;
        std::vector<Light> lights;
        Vector4f background;
        
        
    private:
        Interaction castRay(Ray &ray);
    };
}