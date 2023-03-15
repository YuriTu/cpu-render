#ifndef RT_BVH_H
#define RT_BVH_H

#include <atomic>
#include <vector>
#include <memory>
#include <ctime>
#include "r.h"
#include "shape.h"
#include "Bounds3.h"
#include "geometry.h"
#include "mesh.h"

namespace r
{

struct BVHBuildNode;
// BVHAccel Forward Declarations
struct BVHPrimitiveInfo;

class BVHAccel {

public:
    // BVHAccel Public Types
    enum class SplitMethod { NAIVE, SAH };
    // todo 原本用的share_prt
    BVHAccel(std::vector<std::shared_ptr<GeometricPrimitive>> p, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);

    bool intersect(const Ray& ray, SurfaceInteraction *isect) const;
    bool getIntersection(BVHBuildNode* node, const Ray& ray, SurfaceInteraction *isect)const;
    // bool IntersectP(const Ray &ray) const;
    BVHBuildNode* root;

    // BVHAccel Private Methods
    BVHBuildNode* recursiveBuild(std::vector<std::shared_ptr<GeometricPrimitive>> objects);

    // BVHAccel Private Data
    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<std::shared_ptr<GeometricPrimitive>> primitives;

    void getSample(BVHBuildNode* node, float p, SurfaceInteraction &pos, float &pdf);
    void Sample(SurfaceInteraction &pos, float &pdf);
};

struct BVHBuildNode {
    Bounds3 bounds;
    BVHBuildNode *left;
    BVHBuildNode *right;
    std::shared_ptr<GeometricPrimitive> object;
    float area;

public:
    int splitAxis=0, firstPrimOffset=0, nPrimitives=0;
    // BVHBuildNode Public Methods
    BVHBuildNode(){
        bounds = Bounds3();
        left = nullptr;right = nullptr;
        object = nullptr;
    }
};


}

#endif //RT_BVH_H
