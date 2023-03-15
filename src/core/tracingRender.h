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
        
        void render(const Scene &scene);
        Vector3f Li(Ray &ray, const Scene &scene);
        Vector3f uniformSampleOneLight(const Interaction &isect,const Scene &scene);
        Vector3f estimateDirect(const Interaction &isect,const Scene &scene, std::shared_ptr<GeometricPrimitive> light);

        int width;
        int height;
        int maxBounce;
        float fov;
        
        std::vector<float> depthBuffer;
        
    private:
        
    };
}