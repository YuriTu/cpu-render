#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "r.h"
#include "BVH.h"
#include "interaction.h"
#include "material.h"
#include "geometry.h"
#include "OBJ_Loader.h"
#include <cassert>
#include <array>

namespace r
{
class Triangle : public Shape
{
public:
    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2);
    Bounds3 WorldBound() const override;
    bool intersect(const Ray& ray, SurfaceInteraction *interaction) const override;
    float getArea() const override;
    void Sample(SurfaceInteraction &isect, float &pdf);

    Vector3f v0, v1, v2; // vertices A, B ,C , counter-clockwise order
    Vector3f e1, e2;     // 2 edges v1-v0, v2-v0;
    Vector3f t0, t1, t2; // texture coords
    Vector3f normal;
private:
    float area;
};

class MeshTriangle : public Shape
{
public:
    MeshTriangle(BVHAccel *_bvh, Bounds3 _bounding, float _area);
    
    bool intersect(const Ray& ray, SurfaceInteraction *interaction) const override;
    Bounds3 WorldBound() const override;
    float getArea() const override;
    void Sample(SurfaceInteraction &isect, float &pdf);

    BVHAccel* bvh;
    float area;
    Bounds3 bounding_box;
};

std::vector<std::shared_ptr<Triangle>> createMeshTriangleShape(const std::string& filename, Vector3f *minVert, Vector3f *maxVert);
std::shared_ptr<GeometricPrimitive> createMeshTriangle(const std::string& filename, Material *mt);

}

#endif //RT_TRIANGLE_H