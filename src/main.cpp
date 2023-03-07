#include <stdio.h>
#include <iostream>
#include <chrono>
#include "basic.h"
#include "tracingRender.h"

int main(int argc, char const *argv[])
{
    r::SceneBasic sceneTemplate;
    r::Scene scene(sceneTemplate.width,sceneTemplate.height);
    scene.add(sceneTemplate.lists);
    scene.applyTemplate(sceneTemplate);

    r::TracingRender r;
    auto start = std::chrono::system_clock::now();
    // r.render(scene);
    auto end = std::chrono::system_clock::now();
    printf("render complete: ");
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s\n" ;
    return 0;
}
