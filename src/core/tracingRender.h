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
        void render(const Scene &scene);
        Vector3f Li(Ray &ray, const Scene &scene);
        Vector3f pathTracing(Ray &ray, int depth);
        Vector3f uniformSampleOneLight(Interaction &isect,const Scene &scene);
        Vector3f estimateDirect(Interaction &isect,const Scene &scene, std::shared_ptr<Mesh> light);

        int width;
        int height;
        int maxBounce;
        float fov;
        
        std::vector<float> depthBuffer;
        std::vector<Light> lights;
        
        
        
    private:
        
    };
}