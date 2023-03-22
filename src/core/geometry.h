#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "r.h"
namespace r
{

template <typename T>
class Vector3
{
    public:
        Vector3():x(0),y(0),z(0) {}
        Vector3(T xx): x(xx),y(xx),z(xx) {}
        Vector3(T xx,T yy): x(xx),y(yy),z(0) {}
        Vector3(T xx,T yy, T zz): x(xx),y(yy),z(zz) {}
        
        T operator[](int i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        T &operator[](int i) {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        Vector3<T> operator+(const Vector3<T> &v) const{
            return Vector3(x + v.x, y + v.y, z + v.z);
        }
        Vector3<T>& operator+=(const Vector3<T> &v){
            x += v.x;
            y += v.y;
            z += v.z; 
            return *this;
        }

        Vector3<T> operator-(const Vector3<T> &v) const{
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }
        Vector3<T> operator-() const{
            return Vector3<T>(-x,-y,-z);
        }

        Vector3<T> operator*(const Vector3<T> &v) const {
            return Vector3<T>( x*v.x, y*v.y ,z*v.z);
        }

        template <typename U>
        Vector3<T> operator*(U f) const {
            return Vector3<T>( x*f, y*f, z*f);
        }

        template <typename U>
        Vector3<T> &operator*=(U s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        template <typename U>
        Vector3<T> operator/(U f) const {
            float inv = (float)1 / f;
            return Vector3<T>( x/f, y/f, z/f);
        }

        template <typename U>
        bool operator>(U f) const {
            bool rs = false;
            if (x > f.x && y > f.y && z > f.z) {
                rs = true;
            }
            return rs;
        }

        bool operator==(const Vector3<T> &v) const {
            return x == v.x && y == v.y && z == v.z;
        }
        bool operator!=(const Vector3<T> &v) const {
            return x != v.x || y != v.y || z != v.z;
        }

        bool isBlack() const {
            bool rs = false;
            if (x == 0 && y == 0 && z == 0) {
                rs = true;
            }
            if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
                rs = true;
            }
            return rs;
        }

        float lengthSquared() const { return x * x + y * y + z * z;}
        float length () const { return std::sqrt(lengthSquared());}

        T x,y,z;
};



typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

// template <typename T,typename U, typename=float>
// inline Vector3<T> operator*(U n, Vector3<T>& v) {
//     return v * n;
// }

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}


template <typename T>
inline Vector3<T> operator*(float n,const Vector3<T>& v) {
    return v * n;
}




template <typename T>
inline Vector3<T> normalize(const Vector3<T> &v) {
    float norm = v.lengthSquared();

    if (norm > 0) {
        return v / v.length();
    }
    return v;
}

template <typename T>
inline T Dot(const Vector3<T>v1, const Vector3<T>v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Vector3<T> &v2) {
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline T SafeDot(const Vector3<T>v1, const Vector3<T>v2) {
    return std::max(0.f,Dot(v1,v2));
}

template <typename T>
Vector3<T> Cross(const Vector3<T> &v1,const Vector3<T> &v2){
    T x = v1.y * v2.z - v1.z*v2.y;
    T y = v1.z * v2.x - v1.x*v2.z;
    T z = v1.x * v2.y - v1.y*v2.x;
    return Vector3<T>(x,y,z);
}

template <typename T>
Vector3<T> Min(const Vector3<T> &v1, const Vector3<T> &v2) {
    return Vector3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y),
                      std::min(v1.z, v2.z));
}

template <typename T>
Vector3<T> Max(const Vector3<T> &v1, const Vector3<T> &v2) {
    return Vector3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y),
                      std::max(v1.z, v2.z));
}

template <typename T>
Vector3<T> EXP(const Vector3<T> &v) {
    return Vector3<T>(std::exp(v.x),std::exp(v.y),std::exp(v.z));
}

template <typename T>
inline void CoordinateSystem(const Vector3<T> &v1, Vector3<T> *v2, Vector3<T> *v3) {
    if (std::abs(v1.x) > std::abs(v1.y)) {
        *v2 = Vector3<T>(-v1.z, 0.f, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    } else {
        *v2 = Vector3<T>(0.f, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    }
    *v3 = Cross(v1, *v2);
}

inline Vector3f SphericalDirection(float sinTheta, float cosTheta, float phi) {
    // sinTheta 类比方位角降维 变成了方位角的一个点 所以cos phi 就是x
    return Vector3f(
        sinTheta * std::cos(phi),
        sinTheta * std::sin(phi),
        cosTheta
    );
}

inline Vector3f SphericalDirection(float sinTheta, float cosTheta, float phi,
                                   const Vector3f&x,const Vector3f&y,const Vector3f&z) {
    // xyz 是三个坐标轴，即映射到xyz坐标轴的结果
    return sinTheta * std::cos(phi) * x +
        sinTheta * std::sin(phi) * y +
        cosTheta * z;
}


class Ray
{   
public:
    Ray(Vector3f _o,Vector3f _d);
    Ray(const Vector3f _o,const Vector3f _d, float tMax,const Medium *medium = nullptr);

    Vector3f operator()(float t) const { return o + d * t; }
    void setTMax(float t) { tMax = t;}

    Vector3f o;
    Vector3f d;
    Vector3f d_inv;
    float t,tMax;
    const Medium *medium;
};

}
#endif
