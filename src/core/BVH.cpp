#include <algorithm>
#include <cassert>
#include "BVH.h"

namespace r{

BVHAccel::BVHAccel(std::vector<std::shared_ptr<GeometricPrimitive>> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<std::shared_ptr<GeometricPrimitive>> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->WorldBound());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->WorldBound();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        node->area = objects[0]->getArea();
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector<std::shared_ptr<GeometricPrimitive>>{objects[0]});
        node->right = recursiveBuild(std::vector<std::shared_ptr<GeometricPrimitive>>{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->WorldBound().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->WorldBound().Centroid().x <
                       f2->WorldBound().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->WorldBound().Centroid().y <
                       f2->WorldBound().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->WorldBound().Centroid().z <
                       f2->WorldBound().Centroid().z;
            });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        auto leftshapes = std::vector<std::shared_ptr<GeometricPrimitive>>(beginning, middling);
        auto rightshapes = std::vector<std::shared_ptr<GeometricPrimitive>>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
    }

    return node;
}

bool BVHAccel::intersect(const Ray& ray, SurfaceInteraction *isect) const
{
    bool hit = false;
    if (!root) return hit;
    hit = BVHAccel::getIntersection(root, ray, isect);
    return hit;
}

bool BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray, SurfaceInteraction *isect) const
{
    SurfaceInteraction rs = SurfaceInteraction();

    std::array<int, 3> dirIsNeg;
    dirIsNeg[0] = int(ray.d.x >= 0);
    dirIsNeg[1] = int(ray.d.y >= 0);
    dirIsNeg[2] = int(ray.d.z >= 0);

    bool flag = node->bounds.IntersectP(ray,ray.d_inv,dirIsNeg);
    if (!flag) {
        // bounds 没交点，过
        return flag;
    }

    if (node->object) {
        SurfaceInteraction tempIsect;
        flag = node->object->intersect(ray, &tempIsect);
        rs = tempIsect;
        if (rs.n.isBlack() && flag){
                printf("error n0.5 !");
            }
    } else {
        SurfaceInteraction left = SurfaceInteraction();
        SurfaceInteraction right = SurfaceInteraction();
        bool b_left = getIntersection(node->left, ray,&left);
        bool b_right = getIntersection(node->right, ray, &right);

        if (b_left || b_right) {
            // 谁距离仅用谁
            if (left.distance < right.distance) {
                rs = left;
            } else {
                rs = right;
            }
        } else {
            flag = false;
        }
        
    }
    *isect = rs;

    return flag;
    
}


void BVHAccel::getSample(BVHBuildNode* node, float p, SurfaceInteraction &pos, float &pdf){
    if(node->left == nullptr || node->right == nullptr){
        node->object->Sample(pos, pdf);
        pdf *= node->area;
        return;
    }
    if(p < node->left->area){
        getSample(node->left, p, pos, pdf);
        
    } else {
        getSample(node->right, p - node->left->area, pos, pdf);
    } 
    // printf("bvh:getsample pdf %f \n", pdf);
    return ;
}

void BVHAccel::Sample(SurfaceInteraction &pos, float &pdf){
    float p = std::sqrt(getRandom()) * root->area;
    getSample(root, p, pos, pdf);
    // printf("bvh:sample pdf %f  \n", pdf);
    pdf /= root->area;
}

}