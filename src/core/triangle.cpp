#include "triangle.h"
#include "geometry.h"
namespace r
{

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



}