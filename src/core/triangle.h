#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "BVH.h"
#include "interaction.h"
#include "material.h"
#include "mesh.h"
#include "geometry.h"
#include "OBJ_Loader.h"
#include <cassert>
#include <array>

namespace r
{
class Triangle : public Mesh
{
public:
    Vector3f v0, v1, v2; // vertices A, B ,C , counter-clockwise order
    Vector3f e1, e2;     // 2 edges v1-v0, v2-v0;
    Vector3f t0, t1, t2; // texture coords
    Vector3f normal;
    float area;
    Material* material;

    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, Material* _m = nullptr);

    bool intersect(const Ray& ray, SurfaceInteraction *interaction) override;
    void ComputeScatteringFunction(SurfaceInteraction *isect) const;
    Bounds3 getBounds() override;
    void Sample(SurfaceInteraction &isect, float &pdf);
    float getArea();
    Material* getMaterial();
};

class MeshTriangle : public Mesh
{
public:
    MeshTriangle(const std::string& filename, Material *mt);

    bool intersect(const Ray& ray, SurfaceInteraction *interaction);

    bool intersect(const Ray& ray, float& tnear, uint32_t& index) const
    {
        bool intersect = false;
        printf("illegal function triangle");
        // for (uint32_t k = 0; k < numTriangles; ++k) {
        //     const Vector3f& v0 = vertices[vertexIndex[k * 3]];
        //     const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
        //     const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
        //     float t, u, v;
        //     if (rayTriangleIntersect(v0, v1, v2, ray.origin, ray.direction, t,
        //                              u, v) &&
        //         t < tnear) {
        //         tnear = t;
        //         index = k;
        //         intersect |= true;
        //     }
        // }

        return intersect;
    }

    Bounds3 getBounds() { return bounding_box; }
    void ComputeScatteringFunction(SurfaceInteraction *isect) const override;
    Material* getMaterial();
    void Sample(SurfaceInteraction &isect, float &pdf);
    float getArea();

    Bounds3 bounding_box;
    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;

    std::vector<Triangle> triangles;

    BVHAccel* bvh;
    float area;

    Material* material;
};

inline bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1,
                          const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    Vector3f pvec = Cross(dir, edge2);
    float det = Dot(edge1, pvec);
    if (det == 0 || det < 0)
        return false;

    Vector3f tvec = orig - v0;
    u = Dot(tvec, pvec);
    if (u < 0 || u > det)
        return false;

    Vector3f qvec = Cross(tvec, edge1);
    v = Dot(dir, qvec);
    if (v < 0 || u + v > det)
        return false;

    float invDet = 1 / det;

    tnear = Dot(edge2, qvec) * invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

}

#endif //RT_TRIANGLE_H