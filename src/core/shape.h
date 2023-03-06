#ifndef RT_SHAPE_H
#define RT_SHAPE_H

#include "geometry.h"
#include "interaction.h"

namespace r
{

class Shape {
    public:
        Shape(){}
        virtual ~Shape() {}
        virtual bool intersect(const Ray& ray) = 0;
        virtual bool intersect(const Ray& ray, float &, uint32_t &) const = 0;
        // virtual Interaction getIntersection(Ray _ray) = 0;
        virtual void getSurfaceProperties(const Vector3f &, const Vector3f &, const uint32_t &, const Vector2f &, Vector3f &, Vector2f &) const = 0;
        // virtual Vector3f evalDiffuseColor(const Vector2f &) const =0;
        // virtual Bounds3 getBounds()=0;
        virtual float getArea()=0;
        // virtual void Sample(Interaction &pos, float &pdf)=0;
        virtual bool hasEmit()=0;
};

}

#endif //RT_SHAPE_H