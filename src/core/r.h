#ifndef RT_R_H
#define RT_R_H

#include <limits>
#include <iostream>

namespace r 
{
class Scene;
class Interaction;
class Mesh;
class Material;
class Bounds3;
class BVHAccel;
struct Ray;
template <typename T>
class Vector3;
class Shape;
class TracingRender;
class Triangle;
class MeshTriangle;
}


static constexpr float PI = 3.1415926f;
static constexpr float M_PI = 3.1415926f;
static constexpr float InvPi = 0.31830988618379067154;
static constexpr float Inv2Pi = 0.15915494309189533577;
static constexpr float Inv4Pi = 0.07957747154594766788;
static constexpr float EPS = 1e-4;
static constexpr float EPSILON = 1e-4;

#endif