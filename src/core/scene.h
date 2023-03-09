#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "r.h"
#include "BVH.h"
#include "geometry.h"
#include "mesh.h"
#include "basic.h"


namespace r
{

class Scene
{
    public:
        Scene(int w,int h);
        bool intersect(const Ray& ray,Interaction *isect ) const;
        void buildBVH();
        void add(std::shared_ptr<Mesh> obj);
        void add(std::vector<std::shared_ptr<Mesh>> lists);
        void applyTemplate(const SceneBasic temp);
        
        std::vector<std::shared_ptr<Mesh>> objects;
        BVHAccel *bvh;
        int depth;
        int height;
        int width;
        float fov;
        float aspect;
        float samples;
        Vector3f background;
        float maxDepth;
        Vector3f camPos;
};

}

#endif //RT_SCENE_H