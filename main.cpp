// #include "render.h"
#include "tracingRender.h"
#include "mesh.h"
#include <stdio.h>
#include <iostream>

//todo 
// path
// denoise
// subsurface
// module loader

constexpr int WIDTH = 400;
constexpr int HEIGHT = 400;

std::vector<Vector4f> getPoints(){
    std::vector<Vector4f> ret = {
        Vector4f(0.0, 50.0, -1.0),
        Vector4f(100.0, 50.0, -1.0),
        Vector4f(50.0, 100.0, -1.0),
    };
    return ret;
}


int main(int argc, char const *argv[])
{
    // r::Render r(WIDTH, HEIGHT);
    // r::triangle tri(
    //     Vector4f(0.0, 50.0, -1.0),
    //     Vector4f(100.0, 50.0, -1.0),
    //     Vector4f(50.0, 100.0, -1.0)
    // );
    r::TracingRender r(WIDTH, HEIGHT);
    r::Sphere mesh(Vector4f(.15,.55, 6.15), 1, Vector4f(1),utils::REFLECTION);
    r::Sphere mesh2(Vector4f(-2.65,-2.55, 5.15), 1, Vector4f(1),utils::DIFFUSE);
    r::Light light(Vector4f(-15,15, 25), .5);
    r::Light light2(Vector4f(15,15, 25), .5);

    r.add(mesh);
    r.add(mesh2);
    r.add(light);
    // r.add(light2);
    r.setModule(Vector4f(0));
    r.setView(Vector4f(0,0,-1));
    r.setPerspectiveProjection(0.01, 1000, WIDTH / HEIGHT , 45);
    r.render();
    printf("done ");
    return 0;
}
