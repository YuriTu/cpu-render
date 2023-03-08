#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <limits>
namespace r
{

const double TMAX = std::numeric_limits<double>::max();

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
        Vector3<T> operator/(U f) const {
            float inv = (float)1 / f;
            return Vector3<T>( x/f, y/f, z/f);
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

struct Ray
{   
    Ray(Vector3f _o,Vector3f _d);
    Vector3f o;
    Vector3f d;
    Vector3f d_inv;
    float t,t_min,t_max;
};

}
#endif
