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

class Triangle : public Mesh
{
public:
    Vector3f v0, v1, v2; // vertices A, B ,C , counter-clockwise order
    Vector3f e1, e2;     // 2 edges v1-v0, v2-v0;
    Vector3f t0, t1, t2; // texture coords
    Vector3f normal;
    float area;
    Material* m;

    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, Material* _m = nullptr);

    bool intersect(const Ray& ray, Interaction *interaction) override;
    // Vector3f evalDiffuseColor(const Vector2f&) const override;
    Bounds3 getBounds() override;
    void Sample(Interaction &pos, float &pdf);
    float getArea();
    bool hasEmit(){
        return m->hasEmission();
    }
};

class MeshTriangle : public Mesh
{
public:
    MeshTriangle(const std::string& filename, Material *mt);
    
    bool intersect(const Ray& ray) { return true; }

    bool intersect(const Ray& ray, Interaction *interaction){
        bool hit = false;
        if (this->bvh) {
            hit = bvh->intersect(ray, interaction);
        }

        return hit;
    };

    bool intersect(const Ray& ray, float& tnear, uint32_t& index) const
    {
        bool intersect = false;
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

    // void getSurfaceProperties(const Vector3f& P, const Vector3f& I,
    //                           const uint32_t& index, const Vector2f& uv,
    //                           Vector3f& N, Vector2f& st) const
    // {
    //     const Vector3f& v0 = vertices[vertexIndex[index * 3]];
    //     const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
    //     const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
    //     Vector3f e0 = normalize(v1 - v0);
    //     Vector3f e1 = normalize(v2 - v1);
    //     N = normalize(Cross(e0, e1));
    //     const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
    //     const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
    //     const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
    //     st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    // }

    // Vector3f evalDiffuseColor(const Vector2f& st) const
    // {
    //     float scale = 5;
    //     float pattern =
    //         (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
    //     return lerp(Vector3f(0.815, 0.235, 0.031),
    //                 Vector3f(0.937, 0.937, 0.231), pattern);
    // }


    
    void Sample(Interaction &pos, float &pdf){
        bvh->Sample(pos, pdf);
        pos.emit = m->getEmission();
    }
    float getArea();
    bool hasEmit(){
        return m->hasEmission();
    }

    Bounds3 bounding_box;
    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    // std::unique_ptr<Vector2f[]> stCoordinates;

    std::vector<Triangle> triangles;

    BVHAccel* bvh;
    float area;

    Material* m;
};

inline Bounds3 Triangle::getBounds() { return Union(Bounds3(v0, v1), v2); }

}

#endif //RT_TRIANGLE_H