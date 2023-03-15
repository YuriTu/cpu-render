#ifndef RT_MESH_H
#define RT_MESH_H

#include "r.h"
#include "utils.h"
#include "interaction.h"
#include "bounds3.h"
#include "shape.h"
#include "medium.h"


namespace r
{
class Mesh
{
public:
    Mesh() = default;
    virtual ~Mesh() = default;
    virtual Bounds3 WorldBound() const = 0; // WorldBound
    virtual bool intersect(const Ray& ray, SurfaceInteraction *interaction) = 0;
    virtual const Material* getMaterial() const = 0;
    virtual void ComputeScatteringFunction(SurfaceInteraction *isect) const = 0;
   
};

class GeometricPrimitive : public Mesh 
{
public:
    GeometricPrimitive(std::shared_ptr<Shape> shape,
                       std::shared_ptr<Material> material)
                       :shape(shape), material(material), mediumInterface(nullptr){};
    GeometricPrimitive(const std::shared_ptr<Shape> shape,
                       const std::shared_ptr<Material> material,
                       MediumInterface mediumInterface)
                       :shape(shape), material(material), mediumInterface(mediumInterface){};
    Bounds3 WorldBound() const;
    bool intersect(const Ray& ray, SurfaceInteraction *interaction);
    const Material* getMaterial() const;
    void ComputeScatteringFunction(SurfaceInteraction *isect) const;

    void Sample(SurfaceInteraction &isect, float &pdf);
    float getArea() const;
    

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Shape> shape;
    MediumInterface mediumInterface;
};
    
} // namespace r



#endif