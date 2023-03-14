#include "triangle.h"
#include "geometry.h"
namespace r
{

Triangle::Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, Material* _m)
: v0(_v0), v1(_v1), v2(_v2), material(_m)
{
    e1 = v1 - v0;
    e2 = v2 - v0;
    Vector3f square2 = Cross(e1, e2);
    normal = normalize(square2);
    area =  square2.length()*0.5f;
}

float Triangle::getArea() {
    return area;
}

bool Triangle::intersect(const Ray& ray,SurfaceInteraction *interaction)
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
    
    inter.primitive = this;

    inter.distance = t_tmp;
    *interaction = inter;

    return hit;
}

void Triangle::Sample(SurfaceInteraction &isect, float &pdf){
    float x = std::sqrt(getRandom()), y = getRandom();
    // 1-x + x - xy + xy = 1 所以不会超过重心坐标系
    isect.p = v0 * (1.0f - x) + v1 * (x * (1.0f - y)) + v2 * (x * y);
    isect.n = this->normal;
    isect.primitive = this;
    pdf = 1.0f / area;
    // printf("triangle:sample pdf %f  \n", pdf);
}

void Triangle::ComputeScatteringFunction(SurfaceInteraction *isect) const {
    return this->material->ComputeScatteringFunction(isect);
};

Material* Triangle::getMaterial() {
    return this->material;
};

Bounds3 Triangle::getBounds() {
    return Union(Bounds3(v0, v1), v2);
}


MeshTriangle::MeshTriangle(const std::string& filename, Material *mt) {
    objl::Loader loader;
    loader.LoadFile(filename);
    area = 0;
    material = mt;
    assert(loader.LoadedMeshes.size() == 1);
    auto mesh = loader.LoadedMeshes[0];

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

        triangles.emplace_back(face_vertices[0], face_vertices[1],
                                face_vertices[2], mt);
    }

    bounding_box = Bounds3(min_vert, max_vert);

    std::vector<std::shared_ptr<Mesh>> ptrs;
    for (auto& tri : triangles){
        // todo 这里需要和上面emplace 部分优化一下
        auto ptr = std::make_shared<Triangle>(tri.v0,tri.v1,tri.v2,tri.material);
        ptrs.push_back(ptr);
        area += tri.area;
    }
    bvh = new BVHAccel(ptrs);
}

float MeshTriangle::getArea() {
    return area;
}

bool MeshTriangle::intersect(const Ray& ray, SurfaceInteraction *interaction){
    bool hit = false;
    if (this->bvh) {
        hit = bvh->intersect(ray, interaction);
    }

    return hit;
};

void MeshTriangle::ComputeScatteringFunction(SurfaceInteraction *isect) const {
    return this->material->ComputeScatteringFunction(isect);
};

Material* MeshTriangle::getMaterial() {
    return this->material;
};

void MeshTriangle::Sample(SurfaceInteraction &isect, float &pdf) {
    this->bvh->Sample(isect, pdf);
}

}