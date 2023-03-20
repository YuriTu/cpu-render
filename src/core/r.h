#ifndef RT_R_H
#define RT_R_H

#include <limits>
#include <iostream>

namespace r 
{
class Scene;
class Interaction;
class SurfaceInteraction;
class MediumInteraction;
class Medium;
class HomogeneousMedium;
struct MediumInterface;
class PhaseFunction;
class HenyeyGreenstein;
class Mesh;
class GeometricPrimitive;
class Material;
class Bounds3;
class BVHAccel;
class Ray;
template <typename T>
class Vector3;
class Shape;
class TracingRender;
class Triangle;
class MeshTriangle;
class BxDF;
class FresnelSpecular;
class LambertianReflection;
}


static constexpr float MAXFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();

static constexpr float PI = 3.1415926f;
static constexpr float M_PI = 3.1415926f;
static constexpr float InvPi = 0.31830988618379067154;
static constexpr float Inv2Pi = 0.15915494309189533577;
static constexpr float Inv4Pi = 0.07957747154594766788;
static constexpr float EPS = 1e-4;
static constexpr float EPSILON = 1e-4;
static constexpr float PiOver2 = 1.57079632679489661923;
static constexpr float PiOver4 = 0.78539816339744830961;

#endif