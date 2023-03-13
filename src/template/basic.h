#ifndef RT_TEMPLATE_BASIC_H
#define RT_TEMPLATE_BASIC_H

#include <memory>
#include "geometry.h"
#include "material.h"
#include "triangle.h"

namespace r
{

struct SceneBasic
{
    // scene prototype;
    int width = 500;
    int height = 500;
    int sampleCount = 1;
    // primitive list  light

    // perspectiveProjection
    Vector3f lookAt = Vector3f(0,0,-1);
    Vector3f transition = Vector3f(0,0,0);
    float near = .01;
    float far = 1000.f;
    float fov = 40.f;
    float aspect = width / (float)height;
    float maxDepth = 5;
    Vector3f background = Vector3f(0.235294, 0.67451, 0.843137);
    Vector3f camPos = Vector3f(278, 273, -800);
    float rrThreshold = 0.8;
    std::vector<std::shared_ptr<Mesh>> lists = createObject();

    std::vector<std::shared_ptr<Mesh>> createObject() {
        Material* red = new Material(DIFFUSE, Vector3f(0.0f));
        red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
        
        Material* green = new Material(DIFFUSE, Vector3f(0.0f));
        green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
        Material* white = new Material(DIFFUSE, Vector3f(0.0f));
        white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
        Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
        light->Kd = Vector3f(0.65f);
        
        auto floor = std::make_shared<MeshTriangle>("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\floor.obj", white);
        auto left = std::make_shared<MeshTriangle>("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\left.obj", red);
        auto right = std::make_shared<MeshTriangle>("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\right.obj", green);
        // MeshTriangle shortbox("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\shortbox.obj", white);
        // MeshTriangle tallbox("D:\\workspace\\vulkan\\cpu-render\\models\\fo\\foo_update_1.obj", white);
        auto light_ = std::make_shared<MeshTriangle>("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\light.obj", light);
        // std::vector<Mesh *> rs = {&floor, &left, &right ,&light_};
        // std::shared_ptr<Mesh> leftptr(left);
        
        // std::vector<std::shared_ptr<Mesh>> rs = {floor,left, right};
        std::vector<std::shared_ptr<Mesh>> rs = {
            left,right,floor,
         light_};
        return rs;
    }
    
    
};

}
#endif //RT_TEMPLATE_BASIC_H