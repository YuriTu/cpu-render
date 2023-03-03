#ifndef RT_TEMPLATE_BASIC_H
#define RT_TEMPLATE_BASIC_H

#include "geometry.h"

// constexpr int WIDTH = 200;
// constexpr int HEIGHT = 150;
// constexpr int SampleCount = 50;

struct SceneBasic
{
    // scene prototype;
    int width = 200;
    int height = 150;
    int sampleCount = 50;
    // primitive list  light

    // perspectiveProjection
    Vector3f lookAt = Vector3f(0,0,-1);
    Vector3f transition = Vector3f(0,0,0);
    float near = .01;
    float far = 1000.f;
    float fov = 45;
    float aspect = width / height;
    
};


#endif //RT_TEMPLATE_BASIC_H