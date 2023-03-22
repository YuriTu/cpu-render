#include "triangle.h"
#include "geometry.h"
namespace r
{

Triangle::Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2)
: v0(_v0), v1(_v1), v2(_v2)
{
    e1 = v1 - v0;
    e2 = v2 - v0;
    Vector3f square2 = Cross(e1, e2);
    normal = normalize(square2);
    area =  square2.length()*0.5f;
}

float Triangle::getArea() const {
    return area;
}

bool Triangle::intersect(const Ray& ray,SurfaceInteraction *interaction) const
{
    SurfaceInteraction inter;
    bool hit = false;
    if (Dot(ray.d, normal) > 0)
        return hit;
    double u, v, t_tmp = 0;
    Vector3f pvec = Cross(ray.d, e2);
    double det = Dot(e1, pvec);
    if (fabs(det) < EPSILON)
        return hit;

    double det_inv = 1. / det;
    Vector3f tvec = ray.o - v0;
    u = Dot(tvec, pvec) * det_inv;
    if (u < 0 || u > 1)
        return hit;
    Vector3f qvec = Cross(tvec, e1);
    v = Dot(ray.d, qvec) * det_inv;
    if (v < 0 || u + v > 1)
        return hit;
    t_tmp = Dot(e2, qvec) * det_inv;

    hit = (t_tmp>0) && (u>0) && (v>0) && (1-u-v>0);
    inter.p = ray(t_tmp);
    inter.n = this->normal;
    inter.wo = -ray.d;
    // 在mesh 层面处理primitive

    inter.distance = t_tmp;
    *interaction = inter;

    // printf("inter x %f y %f z %f length %f -- \n", inter.n.x,inter.n.y,inter.n.z,inter.n.lengthSquared());

    if (t_tmp < EPSILON) {
        hit = false;
    }

    return hit;
}

void Triangle::Sample(SurfaceInteraction &isect, float &pdf){
    float x = std::sqrt(getRandom()), y = getRandom();
    // 1-x + x - xy + xy = 1 所以不会超过重心坐标系
    isect.p = v0 * (1.0f - x) + v1 * (x * (1.0f - y)) + v2 * (x * y);
    isect.n = this->normal;
    // primitive 基类处理
    pdf = 1.0f / area;
}


Bounds3 Triangle::WorldBound() const {
    return Union(Bounds3(v0, v1), v2);
}

MeshTriangle::MeshTriangle(BVHAccel *_bvh, Bounds3 _bounding, float _area) {
    bvh = _bvh;
    bounding_box = _bounding;
    area = _area;
}

float MeshTriangle::getArea() const {
    return area;
}

bool MeshTriangle::intersect(const Ray& ray, SurfaceInteraction *interaction) const {
    bool hit = false;
    if (this->bvh) {
        hit = bvh->intersect(ray, interaction);
    }

    return hit;
};

Bounds3 MeshTriangle::WorldBound() const {
    return bounding_box;
}

void MeshTriangle::Sample(SurfaceInteraction &isect, float &pdf) {
    this->bvh->Sample(isect, pdf);
}

std::vector<std::shared_ptr<Triangle>> createMeshTriangleShape(const std::string& filename, Vector3f *minVert, Vector3f *maxVert) {
    objl::Loader loader;
    loader.LoadFile(filename);
    assert(loader.LoadedMeshes.size() == 1);
    auto mesh = loader.LoadedMeshes[0];
    std::vector<std::shared_ptr<Triangle>> triangles;

    Vector3f min_vert = Vector3f{std::numeric_limits<float>::infinity(),
                                    std::numeric_limits<float>::infinity(),
                                    std::numeric_limits<float>::infinity()};
    Vector3f max_vert = Vector3f{-std::numeric_limits<float>::infinity(),
                                    -std::numeric_limits<float>::infinity(),
                                    -std::numeric_limits<float>::infinity()};
    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<Vector3f, 3> face_vertices;

        for (int j = 0; j < 3; j++) {
            auto vert = Vector3f(mesh.Vertices[i + j].Position.X,
                                    mesh.Vertices[i + j].Position.Y,
                                    mesh.Vertices[i + j].Position.Z);
            face_vertices[j] = vert;

            min_vert = Vector3f(std::min(min_vert.x, vert.x),
                                std::min(min_vert.y, vert.y),
                                std::min(min_vert.z, vert.z));
            max_vert = Vector3f(std::max(max_vert.x, vert.x),
                                std::max(max_vert.y, vert.y),
                                std::max(max_vert.z, vert.z));
        }

        auto tri = std::make_shared<Triangle>(face_vertices[0],face_vertices[1],
                                face_vertices[2]);

        triangles.push_back(tri);
    }
    *minVert = min_vert;
    *maxVert = max_vert;
    return triangles;
}

}