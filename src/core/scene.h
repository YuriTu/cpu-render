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
        bool intersect(const Ray& ray,SurfaceInteraction *isect ) const;
        bool intersectTr(Ray& ray,SurfaceInteraction *isect, Vector3f *tr) const;
        void buildBVH();
        void add(std::shared_ptr<GeometricPrimitive> obj);
        void add(std::vector<std::shared_ptr<GeometricPrimitive>> lists);
        void applyTemplate(const SceneBasic temp);
        
        std::vector<std::shared_ptr<GeometricPrimitive>> objects;
        std::vector<std::shared_ptr<GeometricPrimitive>> lights;
        BVHAccel *bvh;
        int depth;
        int height;
        int width;
        float fov;
        float aspect;
        float samples;
        Vector3f background;
        float maxDepth;
        float rrThreshold;
        Vector3f camPos;
    private:
        void _initLightList();
};

}

#endif //RT_SCENE_H