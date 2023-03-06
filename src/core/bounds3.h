
#ifndef RT_BOUNDS3_H
#define RT_BOUNDS3_H
#include "geometry.h"
#include <limits>
#include <array>

class Bounds3
{
  public:
    Vector3f pMin, pMax; // two points to specify the bounding box
    Bounds3()
    {
        double minNum = std::numeric_limits<double>::lowest();
        double maxNum = std::numeric_limits<double>::max();
        pMax = Vector3f(minNum, minNum, minNum);
        pMin = Vector3f(maxNum, maxNum, maxNum);
    }
    Bounds3(const Vector3f p) : pMin(p), pMax(p) {}
    Bounds3(const Vector3f p1, const Vector3f p2)
    {
        pMin = Vector3f(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        pMax = Vector3f(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
    }

    Vector3f Diagonal() const { return pMax - pMin; }
    int maxExtent() const
    {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    double SurfaceArea() const
    {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    Vector3f Centroid() { return 0.5 * pMin + 0.5 * pMax; }
    Bounds3 Intersect(const Bounds3& b)
    {
        return Bounds3(Vector3f(fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),
                                fmax(pMin.z, b.pMin.z)),
                       Vector3f(fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),
                                fmin(pMax.z, b.pMax.z)));
    }

    Vector3f Offset(const Vector3f& p) const
    {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o.z /= pMax.z - pMin.z;
        return o;
    }

    bool Overlaps(const Bounds3& b1, const Bounds3& b2)
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    bool Inside(const Vector3f& p, const Bounds3& b)
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
    inline const Vector3f& operator[](int i) const
    {
        return (i == 0) ? pMin : pMax;
    }

    inline bool IntersectP(const Ray& ray, const Vector3f& invDir,
                           const std::array<int, 3>& dirisNeg) const;
};



inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // 这里写的有问题导致取样不对 右侧所有物体的相交判断有错

    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic
    // TODO test if ray bound intersects
    // Vector3f o = ray.o;

    // Vector3f t_input = ( pMin - o ) * invDir;
    // Vector3f t_output = ( pMax - o ) * invDir;

    // Vector3f t_input_min (
    //     std::min(t_input.x,t_output.x),
    //     std::min(t_input.y,t_output.y),
    //     std::min(t_input.z,t_output.z)
    // );
    // Vector3f t_output_max (
    //     std::max(t_input.x,t_output.x),
    //     std::max(t_input.y,t_output.y),
    //     std::max(t_input.z,t_output.z)
    // );
    
    // float t_input_v = std::max({t_input_min.x,t_input_min.y,t_input_min.z});
    // float t_output_v = std::min({t_output_max.x,t_output_max.y,t_output_max.z});

    // bool flag = t_input_v < t_output_v && (t_input_v >= 0);
    
    // return flag;



    double t1 = 0;
    double t2 = 0;
    t1 = (pMin.x - ray.o.x) * invDir.x;
    t2 = (pMax.x - ray.o.x) * invDir.x;
    double txmin = (dirIsNeg[0]>0)?t1:t2;
    double txmax = (dirIsNeg[0]>0)?t2:t1;
    t1 = (pMin.y - ray.o.y) * invDir.y;
    t2 = (pMax.y - ray.o.y) * invDir.y;
    double tymin = (dirIsNeg[1]>0)?t1:t2;
    double tymax = (dirIsNeg[1]>0)?t2:t1;
    t1 = (pMin.z - ray.o.z) * invDir.z;
    t2 = (pMax.z - ray.o.z) * invDir.z;
    double tzmin = (dirIsNeg[2]>0)?t1:t2;
    double tzmax = (dirIsNeg[2]>0)?t2:t1;
    
    if((std::max(std::max(txmin,tymin),tzmin) <= std::min(std::min(txmax,tymax),tzmax)) && (std::min(std::min(txmax,tymax),tzmax)>0))
    return true;
    else
    return false;
}

inline Bounds3 Union(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Min(b1.pMin, b2.pMin);
    ret.pMax = Max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 Union(const Bounds3& b, const Vector3f& p)
{
    Bounds3 ret;
    ret.pMin = Min(b.pMin, p);
    ret.pMax = Max(b.pMax, p);
    return ret;
}

#endif // RT_BOUNDS3_H
