#ifndef RT_SCENE_H
#define RT_SCENE_H
#include "BVH.h"
#include "geometry.h"

class Scene
{
    public:
        Scene(int w,int h):width(w),height(h){}
        BVHAccel *bvh;
        bool intersect(const Ray& ray) const;


        int depth;
        int height;
        int width;
};




#endif //RT_SCENE_H