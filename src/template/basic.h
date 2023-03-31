#ifndef RT_TEMPLATE_BASIC_H
#define RT_TEMPLATE_BASIC_H

#include <memory>
#include "geometry.h"
#include "material.h"
#include "triangle.h"
#include "medium.h"

namespace r
{

struct SceneBasic
{
    // scene prototype;
    int width = DEBUG_MODE ? 200 : 500;
    int height = DEBUG_MODE ? 200 : 500;
    int sampleCount = DEBUG_MODE ? 1: 32;

    // perspectiveProjection
    Vector3f lookAt = Vector3f(0,0,-1);
    Vector3f transition = Vector3f(0,0,0);
    float near = .01;
    float far = 1000.f;
    float fov = 40.f;
    float aspect = width / (float)height;
    float maxDepth = DEBUG_MODE ? 16 : 64;
    Vector3f background = Vector3f(0.235294, 0.67451, 0.843137);
    Vector3f camPos = Vector3f(278, 273, -800);
    float rrThreshold = 0.8;
    std::vector<std::shared_ptr<GeometricPrimitive>> lists = createObject();

    std::vector<std::shared_ptr<GeometricPrimitive>> createObject() {
        Material* red = new Material(DIFFUSE, Vector3f(0.0f));
        red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
        Material* green = new Material(DIFFUSE, Vector3f(0.0f));
        green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
        Material* white = new Material(DIFFUSE, Vector3f(0.0f));
        white->Kd = Vector3f(0.725f, 0.71f, 0.68f);

        Material* jade = new Material(REFLECTION_AND_REFRACTION, Vector3f(0.0f));
        jade->Kd = Vector3f(0.325f, 0.529f, 0.415f);
        // names home mi
        HomogeneousMedium* volume = new HomogeneousMedium(
            Vector3f(.03,.03,.03), Vector3f(.07,.07,.07), -0.7f);
        MediumInterface mi = MediumInterface(volume, nullptr);

        Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
        light->Kd = Vector3f(0.65f);
        
        std::shared_ptr<GeometricPrimitive> floor = createMeshTriangle("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\floor.obj", white );
        std::shared_ptr<GeometricPrimitive> left = createMeshTriangle("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\left.obj", red);
        std::shared_ptr<GeometricPrimitive> right = createMeshTriangle("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\right.obj", green);

        // std::shared_ptr<GeometricPrimitive> shortbox = createMeshTriangle("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\shortbox.obj",white);
        std::shared_ptr<GeometricPrimitive> shortbox = createMeshTriangle(
            "D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\shortbox.obj",
            // "D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\duanbox.obj",
            nullptr,mi);
        std::shared_ptr<GeometricPrimitive> fo = createMeshTriangle(
            "D:\\workspace\\vulkan\\cpu-render\\models\\fo\\foo_update_1.obj", jade,mi
            );
        std::shared_ptr<GeometricPrimitive> light_ = createMeshTriangle("D:\\workspace\\vulkan\\cpu-render\\models\\cornellbox\\light.obj", light);
        
        std::vector<std::shared_ptr<GeometricPrimitive>> rs = {
            left,
            right,floor,
            shortbox,
            // fo,
            light_
        };
        return rs;
    }
};
}
#endif //RT_TEMPLATE_BASIC_H