#ifndef RT_R_H
#define RT_R_H

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
static constexpr float EPS = 1e-4;
static constexpr float EPSILON = 1e-4;

#endif