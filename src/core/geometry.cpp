#include "geometry.h"

namespace r 
{
    Ray::Ray(Vector3f _o,Vector3f _d):o(_o),d(_d){
        d_inv = Vector3f(1.f / d.x, 1.f / d.y, 1.f/d.z);
        t_min = 0.f;
        t_max = TMAX;
    }
}