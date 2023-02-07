// #include "render.h"
#include "tracingRender.h"
#include "mesh.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

//todo 
// denoise
// subsurface
// module loader

constexpr int WIDTH = 200;
constexpr int HEIGHT = 150;
constexpr int sampleCount = 50;

std::vector<Vector3f> getPoints(){
    std::vector<Vector3f> ret = {
        Vector3f(0.0, 50.0, -1.0),
        Vector3f(100.0, 50.0, -1.0),
        Vector3f(50.0, 100.0, -1.0),
    };
    return ret;
}


int main(int argc, char const *argv[])
{
    // r::Render r(WIDTH, HEIGHT);
    // r::triangle tri(
    //     Vector3f(0.0, 50.0, -1.0),
    //     Vector3f(100.0, 50.0, -1.0),
    //     Vector3f(50.0, 100.0, -1.0)
    // );
    r::TracingRender r(WIDTH, HEIGHT);
    r::Sphere mesh(Vector3f(27,-20.5,47), 26.5, Vector3f(0.,1.0,0.0),utils::DIFFUSE);
    r::Sphere light(Vector3f(50,118+681.6-.27,81.6),600, Vector3f(),utils::DIFFUSE);
    light.emit = Vector3f(1e18f);
    r::Sphere planes[] = {
        //left
        r::Sphere(Vector3f(1e5+200, 40.8, 81.6), 1e5, Vector3f(.75,.25,.25),utils::DIFFUSE),
        //right
        r::Sphere(Vector3f(-1e5-200,40.8,81.6), 1e5, Vector3f(.25,.25,.75),utils::DIFFUSE),
        // back
        r::Sphere(Vector3f(50,40.8, 1e5 + 200), 1e5, Vector3f(.75,.75,.75),utils::DIFFUSE),
        // front
        r::Sphere(Vector3f(50,40.8, -1e5-200), 1e5, Vector3f(0.0),utils::DIFFUSE),
        // top
        r::Sphere(Vector3f(50, 1e5 + 200, 81.6), 1e5, Vector3f(.75,.75,.75),utils::DIFFUSE),
        // bottom
        r::Sphere(Vector3f(50,-1e5 - 200,81.6), 1e5, Vector3f(.75,.75,.75),utils::DIFFUSE),
        light,
        mesh
    };

    for (auto& item : planes) {
        r.add(item);
    }

    r.setModule(Vector3f(0));
    r.setView(Vector3f(0,0,-1));
    r.setPerspectiveProjection(0.01, 1000, WIDTH / HEIGHT , 45);

    auto start = std::chrono::system_clock::now();
    r.render(sampleCount);
    auto end = std::chrono::system_clock::now();
    printf("render complete: ");
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s\n" ;
    return 0;
}
