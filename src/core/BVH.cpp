#include <algorithm>
#include <cassert>
#include "BVH.h"

namespace r{

BVHAccel::BVHAccel(std::vector<std::shared_ptr<Mesh>> p, int maxPrimsInNode,
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

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<std::shared_ptr<Mesh>> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        node->area = objects[0]->getArea();
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector<std::shared_ptr<Mesh>>{objects[0]});
        node->right = recursiveBuild(std::vector<std::shared_ptr<Mesh>>{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                       f2->getBounds().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                       f2->getBounds().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                       f2->getBounds().Centroid().z;
            });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        auto leftshapes = std::vector<std::shared_ptr<Mesh>>(beginning, middling);
        auto rightshapes = std::vector<std::shared_ptr<Mesh>>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
    }

    return node;
}

bool BVHAccel::intersect(const Ray& ray, Interaction *isect) const
{
    if (!root) return false;
    bool hit = false;
    *isect = BVHAccel::getIntersection(root, ray);
    hit = isect->happened;
    return hit;
}

Interaction BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection

    Interaction rs = Interaction();

    std::array<int, 3> dirIsNeg;
    dirIsNeg[0] = int(ray.d.x >= 0);
    dirIsNeg[1] = int(ray.d.y >= 0);
    dirIsNeg[2] = int(ray.d.z >= 0);

    bool flag = node->bounds.IntersectP(ray,ray.d_inv,dirIsNeg);
    if (!flag) {
        // bounds 没交点，过
        return rs;
    }

    if (node->object) {
        Interaction tempIsect;
        bool temp = node->object->intersect(ray, &tempIsect);
        if (temp || tempIsect.happened) {
            if (temp != tempIsect.happened)
            printf("error!");
        }
        rs = tempIsect;
    } else {
        Interaction left = getIntersection(node->left, ray);
        Interaction right = getIntersection(node->right, ray);

        // 谁距离仅用谁
        if (left.distance < right.distance) {
            rs = left;
        } else {
            rs = right;
        }
    }
    return rs;
    
}


void BVHAccel::getSample(BVHBuildNode* node, float p, Interaction &pos, float &pdf){
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

void BVHAccel::Sample(Interaction &pos, float &pdf){
    float p = std::sqrt(getRandom()) * root->area;
    getSample(root, p, pos, pdf);
    // printf("bvh:sample pdf %f  \n", pdf);
    pdf /= root->area;
}

}