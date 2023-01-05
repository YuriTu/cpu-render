#pragma once

#include "utils.h"
#include "mesh.h"


namespace r
{
    class Render
    {
    public:
        Render() = default;
        Render(int w, int h);
        ~Render() = default;

        void setPerspectiveProjection(float near, float far, float aspect, float fov);
        void setModule(Vector4f position);
        void setView(Vector4f position);
        void add(triangle &m);
        void render();

        int width;
        int height;
        std::vector<Vector4f> frameBuffer;
        std::vector<float> depthBuffer;
        Matrix4x4 m;
        Matrix4x4 v;
        Matrix4x4 p;
        std::vector<Vector4f> points;
        std::vector<Vector4f> colors;
        
        
    private:
        void setMvp();
        void exportImg();
        void rasterization();
    };
}
