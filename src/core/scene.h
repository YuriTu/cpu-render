#ifndef RT_SCENE_H
#define RT_SCENE_H
#include "BVH.h"
#include "geometry.h"
#include "mesh.h"

namespace r
{

class Scene
{
    public:
        Scene(int w,int h);
        
        bool intersect(const Ray& ray) const;
        void buildBVH();
        
        std::vector<Mesh* > objects;
        BVHAccel *bvh;
        int depth;
        int height;
        int width;
};

}




#endif //RT_SCENE_H