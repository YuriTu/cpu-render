#pragma once

#include <complex>
#include <vector>

#define PI 3.1415926;

inline float clamp(float max, float min, float v) {
    return std::min(max, std::max(min,v));
}


inline std::vector<float> getBarycentric2D(float x, float y, std::vector<Vector4f> points) {
    Vector4f point = Vector4f(x,y);
    Vector4f a = points[0] - points[1];
    Vector4f b = points[0] - points[2];
    Vector4f p = points[0] - point;
    Vector4f z = Vector4f(0,0,-1);
    float all = (b.cross(a)).dot(z);
    float r1 = (p.cross(a)).dot(z) / all;
    float r2 = (b.cross(p)).dot(z) / all;
    float r3 = 1 - r1 - r2;

    return {r1, r2, r3};
}

inline bool inTrangle(float x, float y, std::vector<Vector4f> points) {
    bool ret = false;
    Vector4f l0 = points[0] - points[1];
    Vector4f l1 = points[1] - points[2];
    Vector4f l2 = points[2] - points[0];

    Vector4f p = Vector4f(x,y,0.0);

    Vector4f l00 = p - points[1];
    Vector4f l11 = p - points[2];
    Vector4f l22 = p - points[0];

    float r0 = (l0.cross(l00)).z;
    float r1 = (l1.cross(l11)).z;
    float r2 = (l2.cross(l22)).z;

    ret = (r0 >=0 && r1 >=0 && r2 >=0 )|| (r0 <0 && r1 <0 && r2 <0);
    return ret;
}

class Vector4f {
    public:
        float x,y,z,w;
        Vector4f() :x(0),y(0),z(0),w(1) {}
        Vector4f(float xx) :x(xx),y(xx),z(xx),w(1) {}
        Vector4f(float xx, float yy) :x(xx),y(yy),z(1),w(1) {}
        Vector4f(float xx, float yy, float zz): x(xx),y(yy),z(zz),w(1) {}
        
        Vector4f cross(Vector4f &v){
            float _m[3];
            _m[0] = y * v.z - z*v.y;
            _m[1] = z * v.x - x*v.z;
            _m[2] = x * v.y - y*v.x;
            return Vector4f(_m[0],_m[1],_m[2]);
        }

        float dot(Vector4f &v) {
            return x*v.x + y*v.y + z*v.z;
        }

        Vector4f operator+(Vector4f &v) const{
            return Vector4f(x + v.x, y + v.y, z + v.z);
        }

        Vector4f operator-(Vector4f &v) const{
            return Vector4f(x - v.x, y - v.y, z - v.z);
        }

        Vector4f operator*(Vector4f &v) const {
            return Vector4f( x*v.x, y*v.y ,z*v.z);
        }

        Vector4f operator*(float v) const {
            return Vector4f( x*v, y*v, z*v);
        }
};

class Matrix4x4 {
    public:
        float m[4][4];
        Matrix4x4() {
            memset(m, 0, sizeof(float) * 16);
            m[1][1] = m[2][2] = m[3][3] = 1.0;
        }
        Matrix4x4(const float _m[4][4]) {
            memcpy(m, _m, sizeof(float) * 16);
        }
        Matrix4x4 operator*(const float v) const {
            Matrix4x4 ret;
            for (int i = 0; i < 4 ; i++) {
                for (int j = 0; j < 4; j++) {
                    ret.m[i][j] = m[i][j] * v;
                }
            }
            return ret;
        }
        Matrix4x4 operator*(Matrix4x4 &mat) const {
            Matrix4x4 ret;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4;j++) {
                    ret.m[i][j] = 0;
                    for (int k = 0; k < 4; k++) {
                        ret.m[i][j] += m[i][k] * mat.m[k][j];
                    }
                }
            }
            return ret;
        }
        Vector4f operator*(Vector4f &v) const {
            Vector4f ret;
            float _m[4];
            float _v[4] = {v.x, v.y, v.z, v.w};
            for (int i = 0; i < 4; i++) {
                _m[i] = 0;
                for (int j = 0; j < 4; j++) {
                    _m[i] += m[i][j] * _v[j];
                }
            }
            return Vector4f(_m[0],_m[1],_m[2]);
        }

        void setPos(Vector4f &v) {
            m[0][3] = v.x;
            m[1][3] = v.y;
            m[2][3] = v.z;
        }
        void setEyePos(Vector4f &v) {
            setPos(v * -1.0);
        }
};