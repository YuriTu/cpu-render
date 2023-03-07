#include "triangle.h"
#include "geometry.h"
namespace r
{

float Triangle::getArea() {
    return area;
}

bool Triangle::intersect(const Ray& ray,Interaction *interaction)
{
    // 这里写的有问题，导致光源没有被判断到相交
    Interaction inter;

    // if (Dot(ray.d, normal) > 0)
    //     return inter;
    // double u, v, t_tmp = 0;
    // Vector3f pvec = Cross(ray.d, e2);
    // double det = Dot(e1, pvec);
    // if (fabs(det) < EPSILON)
    //     return inter;

    // double det_inv = 1. / det;
    // Vector3f tvec = ray.origin - v0;
    // u = Dot(tvec, pvec) * det_inv;
    // if (u < 0 || u > 1)
    //     return inter;
    // Vector3f qvec = Cross(tvec, e1);
    // v = Dot(ray.direction, qvec) * det_inv;
    // if (v < 0 || u + v > 1)
    //     return inter;
    // t_tmp = Dot(e2, qvec) * det_inv;

    // // TODO find ray triangle Interaction
    // inter.happened = (t_tmp>0) && (u>0) && (v>0) && (1-u-v>0);
    // // inter.coords = ray(t_tmp); // 
    // // inter.normal = this->normal;
    // inter.distance = t_tmp;
    // // inter.obj = this;
    // inter.m = m;
    // inter.emit = m->getEmission();

    // interaction = &inter;

    return inter.happened;
}

MeshTriangle::MeshTriangle(const std::string& filename, Material *mt) 
{
        objl::Loader loader;
        loader.LoadFile(filename);
        area = 0;
        m = mt;
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

        std::vector<Mesh*> ptrs;
        for (auto& tri : triangles){
            ptrs.push_back(&tri);
            area += tri.area;
        }
        bvh = new BVHAccel(ptrs);
    }

float MeshTriangle::getArea() {
    return area;
}

}