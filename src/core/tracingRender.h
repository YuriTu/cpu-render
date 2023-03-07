#pragma once

#include "utils.h"
#include "mesh.h"
#include "scene.h"

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
        // void add(Sphere &m);
        void add(Light &l);
        void sampleLight(Interaction& light,float& pdf);
        void render(const Scene &scene);
        bool intersect(Ray &ray);
        Vector3f getRadiance(Ray &ray, int bounce);
        Vector3f pathTracing(Ray &ray, int depth);


        int width;
        int height;
        int maxBounce;
        float fov;
        
        std::vector<float> depthBuffer;
        std::vector<Light> lights;
        
        
        
    private:
        
    };
}