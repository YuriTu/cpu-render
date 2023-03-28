#include "light.h"
#include "interaction.h"
#include "scene.h"
#include "geometry.h"

namespace r 
{
    
// visibility test function
// p0 起点 p1 终点
bool VisibilityTester::isOccluded(const Scene &scene) const {
    Ray ray = p0.spawnRayTo(p1);
    SurfaceInteraction isect = SurfaceInteraction();
    bool flag = scene.intersect(ray, &isect);

    if (flag) {
        float distance = (isect.p - p1.p).lengthSquared();
        // 打到终点了，就算没occluded
        if (distance < EPSILON) {
            flag = false;
        }
    }

    return flag;
}

Vector3f VisibilityTester::tr(const Scene &scene) const {
    Ray ray = Ray(p0.spawnRayTo(p1));

    Vector3f tr = Vector3f(1.f);

    int count = 0;
    while (true) {
        SurfaceInteraction isect = SurfaceInteraction();
        bool hitSurface = scene.intersect(ray, &isect);
        bool hitLightflag = (isect.p - p1.p).lengthSquared() < EPSILON;

        // 如果bsdf == null 就是media的外部geometryPrimitive 正常计算透射情况,如果是光源则根据ray的情况处理
        // 否则就是有实体阻挡，按照无能量处理
        if (hitSurface) {
            isect.ComputeScatteringFunction(ray);
            
            if (isect.bsdf != nullptr && !hitLightflag) {
                return Vector3f(0.f);
            }
        }
        // 有medium就累计tr，没有就找下一个media
        if (ray.medium) {
            tr *= ray.medium->Tr(ray);
        }

        // 没有阻挡可以直接到达光源，直接计算透射
        if (!hitSurface || hitLightflag) break;

        // 存在media，更新起点继续累计tr前进
        ray = isect.spawnRayTo(p1);

        count++;
        // fixme 现在场景简单理论上最多两次就能出去
        if (count > 10) {
            printf("warning, visibility loop not stop!");
            break;
        }
    }

    return tr;
}
}