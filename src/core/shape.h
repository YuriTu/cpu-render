#ifndef RT_SHAPE_H
#define RT_SHAPE_H

#include "r.h"

namespace r
{

class Shape {
    public:
        virtual ~Shape() {}
        virtual Bounds3 WorldBound() const = 0;
        virtual bool intersect(const Ray& ray, SurfaceInteraction *interaction) const = 0;
        virtual float getArea() const =0;
        virtual void Sample(SurfaceInteraction &isect, float &pdf) = 0;
};

}

#endif //RT_SHAPE_H