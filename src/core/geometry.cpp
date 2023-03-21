#include "geometry.h"

namespace r 
{
    Ray::Ray(Vector3f _o,Vector3f _d):o(_o),d(_d){
        d_inv = Vector3f(1.f / d.x, 1.f / d.y, 1.f/d.z);
        tMax = MAXFloat;
        medium = nullptr;
    }
    Ray::Ray(const Vector3f _o,const Vector3f _d, float _tMax,const Medium *_medium) {
        o = _o;
        d = _d;
        d_inv = Vector3f(1.f / d.x, 1.f / d.y, 1.f/d.z);
        tMax = _tMax;
        medium = _medium;
    }
}