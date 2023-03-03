// #include "render.h"
// #include "tracingRender.h"
// #include "mesh.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

constexpr int WIDTH = 200;
constexpr int HEIGHT = 150;
constexpr int sampleCount = 50;


int main(int argc, char const *argv[])
{
    
    // init 
    // r.setModule(Vector3f(0));
    // r.setView(Vector3f(0,0,-1));
    // r.setPerspectiveProjection(0.01, 1000, WIDTH / HEIGHT , 45);

    auto start = std::chrono::system_clock::now();
    
    // r.render(sampleCount);

    auto end = std::chrono::system_clock::now();
    printf("render complete: ");
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s\n" ;
    return 0;
}
