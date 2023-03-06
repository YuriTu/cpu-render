#include <stdio.h>
#include <iostream>
#include <chrono>
#include "basic.h"
#include "tracingRender.h"

int main(int argc, char const *argv[])
{
    SceneBasic sceneTemplate;
    r::TracingRender r(scene.width,scene.height);
    
    r.setModule(scene.transition);
    r.setView(scene.lookAt);
    r.setPerspectiveProjection(scene.near,scene.far,scene.aspect,scene.fov);

    auto start = std::chrono::system_clock::now();
    r.render(scene.sampleCount);
    auto end = std::chrono::system_clock::now();
    printf("render complete: ");
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s\n" ;
    return 0;
}
