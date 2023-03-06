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
        void setModule(Vector3f position);
        void setView(Vector3f position);
        void add(Sphere &m);
        void add(Light &l);
        void sampleLight(Interaction& light,float& pdf);
        void render(int samples);
        bool intersect(Ray &ray);
        Vector3f getRadiance(Ray &ray, int bounce);
        Vector3f pathTracing(Ray &ray, int depth);


        int width;
        int height;
        int maxBounce;
        float fov;
        std::vector<Vector3f> frameBuffer;
        std::vector<float> depthBuffer;
        std::vector<Sphere> objects;
        std::vector<Light> lights;
        Vector3f background;
        
        
    private:
        
    };
}