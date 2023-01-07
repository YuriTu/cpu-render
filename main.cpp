// #include "render.h"
#include "tracingRender.h"
#include "mesh.h"
#include <stdio.h>
#include <iostream>


constexpr int WIDTH = 400;
constexpr int HEIGHT = 300;

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
    r::Sphere mesh(Vector4f(.15,.55, 8.15), 1, Vector4f(1));

    r.add(mesh);
    r.setModule(Vector4f(0));
    r.setView(Vector4f(0,0,-1));
    r.setPerspectiveProjection(0.01, 1000, WIDTH / HEIGHT , 45);
    r.render();
    printf("gen done ");
    // std::cout << r.render() << std::endl;
    return 0;
}
