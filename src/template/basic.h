#ifndef RT_TEMPLATE_BASIC_H
#define RT_TEMPLATE_BASIC_H

#include "geometry.h"
#include "material.h"
#include "triangle.h"

Material* red = new Material(DIFFUSE, Vector3f(0.0f));
red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
Material* green = new Material(DIFFUSE, Vector3f(0.0f));
green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
Material* white = new Material(DIFFUSE, Vector3f(0.0f));
white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
light->Kd = Vector3f(0.65f);

MeshTriangle floor("D:\\workspace\\games101-hw\\pa7\\models\\cornellbox\\floor.obj", white);
// MeshTriangle shortbox("D:\\workspace\\games101-hw\\pa7\\models\\cornellbox\\shortbox.obj", white);
MeshTriangle tallbox("D:\\workspace\\games101-hw\\pa7\\models\\fo\\foo_update_1.obj", white);
MeshTriangle left("D:\\workspace\\games101-hw\\pa7\\models\\cornellbox\\left.obj", red);
MeshTriangle right("D:\\workspace\\games101-hw\\pa7\\models\\cornellbox\\right.obj", green);
MeshTriangle light_("D:\\workspace\\games101-hw\\pa7\\models\\cornellbox\\light.obj", light);


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