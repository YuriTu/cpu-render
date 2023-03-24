#ifndef RT_SHAPE_H
#define RT_SHAPE_H

#include "r.h"
#include "geometry.h"

namespace r
{

class Shape {
    public:
        virtual ~Shape() {}
        virtual Bounds3 WorldBound() const = 0;
        virtual bool intersect(const Ray& ray, SurfaceInteraction *interaction) const = 0;
        virtual float getArea() const =0;
        virtual void Sample(SurfaceInteraction &isect, float &pdf) = 0;

        virtual float Pdf(const Interaction &ref) const;
        virtual float Pdf(const Interaction &ref, const Vector3f &wi) const;
};

}

#endif //RT_SHAPE_H