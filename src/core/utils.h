#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <complex>
#include <vector>
#include <cstring>
#include <random>
#include "r.h"
#include "geometry.h"

namespace r
{


inline float clamp(float max, float min, float v) {
    return std::min(max, std::max(min,v));
}

inline float deg2rad(float deg) {
    return deg / 180.0 * PI;
}

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
        Vector3f operator*(Vector3f &v) const {
            Vector3f ret;
            float _m[3];
            float _v[3] = {v.x, v.y, v.z};
            for (int i = 0; i < 4; i++) {
                _m[i] = 0;
                for (int j = 0; j < 4; j++) {
                    _m[i] += m[i][j] * _v[j];
                }
            }
            return Vector3f(_m[0],_m[1],_m[2]);
        }

        void setPos(Vector3f &v) {
            m[0][3] = v.x;
            m[1][3] = v.y;
            m[2][3] = v.z;
        }
        void setEyePos(Vector3f &v) {
            Vector3f _v = v * -1;
            setPos(_v);
        }
};


inline std::vector<float> getBarycentric2D(float x, float y, std::vector<Vector3f> points) {
    Vector3f point = Vector3f(x,y);
    Vector3f a = points[0] - points[1];
    Vector3f b = points[0] - points[2];
    Vector3f p = points[0] - point;
    Vector3f z = Vector3f(0,0,-1);
    float all = Dot((Cross(a,b)),z);
    float r1 = Dot((Cross(p,a)),z);
    float r2 = Dot((Cross(b,p)),z);
    float r3 = 1 - r1 - r2;

    return {r1, r2, r3};
}

inline bool inTrangle(float x, float y, std::vector<Vector3f> points) {
    bool ret = false;
    Vector3f l0 = points[0] - points[1];
    Vector3f l1 = points[1] - points[2];
    Vector3f l2 = points[2] - points[0];

    Vector3f p = Vector3f(x,y,0.0);

    Vector3f l00 = p - points[1];
    Vector3f l11 = p - points[2];
    Vector3f l22 = p - points[0];

    float r0 = (Cross(l0,l00)).z;
    float r1 = (Cross(l1,l11)).z;
    float r2 = (Cross(l2,l22)).z;

    ret = (r0 >=0 && r1 >=0 && r2 >=0 )|| (r0 <0 && r1 <0 && r2 <0);
    return ret;
}

inline int getIndex(int i, int j, int width, int height) {
    return (height -1 - j) * width + i;
}

inline Vector3f reflect(Vector3f I, Vector3f N) {
    return I - 2 * Dot(I,N) * N ;
}

inline Vector3f refract(Vector3f I, Vector3f N, float ior) {
    float n1 = 1;
    float n2 = ior;
    float eta = n1 / n2;
    float cos_i = clamp(1, -1, Dot(I,N));
    float k = 1.f - eta * eta * (1.0 - cos_i * cos_i);
    if (k < 0.0) {
        return Vector3f(0.0);
    }
    return eta * I - (eta * cos_i + sqrtf(k)) * N;
}

inline bool Refract(const Vector3f &wi, const Vector3f &n, float eta, Vector3f *wt) {
    float cosThetaI = Dot(wi, n);
    float sin2ThetaI = std::max(0.f, (float)(1 - cosThetaI * cosThetaI));
    float sin2ThetaT = eta * eta * sin2ThetaI;
    float k = 1.f - sin2ThetaT;
    if (k <= 0.f) return false;

    float cosThetaT = std::sqrt(k);
    *wt = eta * -wi + (eta * cosThetaI - cosThetaT) * n;
    return true;
}

inline void exportImg(std::vector<Vector3f> frameBuffer,int width, int height)
{
    FILE* fp = fopen("img.ppm","wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (int i = 0; i < frameBuffer.size(); i++) {
        static unsigned char color[3];
        
        Vector3f pixel = frameBuffer[i];
        color[0] = (char)(255 * clamp(1.0,0., pixel.x));
        color[1] = (char)(255 * clamp(1.0,0., pixel.y));
        color[2] = (char)(255 * clamp(1.0,0., pixel.z));
        
        fwrite(color, 1, 3, fp);
    }

    fclose(fp);
    
}
// deprecated 
inline float fresnel(Vector3f I, Vector3f N, float ior) {
    float theta_input = Dot(I,N);
    float n1 = 1;
    float n2 = ior;
    if (theta_input < 0) {
        std::swap(n1,n2);
    }
    theta_input = fabsf(theta_input);
    float sin_input = sqrtf(std::max(0.f, 1 - theta_input * theta_input));
    float sin_output = (n1 / n2) * sin_input;

    if (sin_output >= 1) {
        return 1;
    }
    float theta_output = sqrtf(std::max(0.f,1 - sin_output * sin_output));

    float rs = (n1 * theta_input - n2 * theta_output) / (n1 * theta_input + n2 * theta_output);
    float rp = (n1 * theta_output - n2 * theta_input) / (n1 * theta_output + n2 * theta_input);

    return (rs*rs + rp*rp) / 2;
}

inline float getRandom(float min = 0.f, float max = 1.f) {
    std::random_device rd;
    std::mt19937 dev(rd());
    std::uniform_real_distribution<float> range(min,max);
    return range(dev);
}

inline Vector3f getVecFromSampleSphereUniform(){
    float phi =  getRandom(0,1) * 2 * PI;
    float theta = getRandom(0,1) * PI;
    float r = sin(theta);
    float z = cos(theta);
    float x = r * cos(phi);
    float y = r * sin(phi);
    return normalize(Vector3f(x,y,z));
}

inline Vector3f concentricSampleDisk() {
    Vector3f temp = Vector3f(getRandom(),getRandom(),0.f);
    Vector3f uOffset = 2.f * temp  - Vector3f(1,1,0);

    if (uOffset.x == 0 && uOffset.y == 0) return Vector3f(0.f);

    float theta, r;
    if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
        r = uOffset.x;
        theta = PiOver4 * (uOffset.y / uOffset.x);
    } else {
        r = uOffset.y;
        theta = PiOver2 - PiOver4 * (uOffset.x / uOffset.y);
    }
    return r * Vector3f(std::cos(theta), std::sin(theta), 0.f);
}

inline Vector3f CosineSampleHemisphere() {
    Vector3f d = concentricSampleDisk();
    float z = std::sqrt(std::max((float)0.f, 1 - (d.x * d.x + d.y * d.y)));
    return Vector3f(d.x, d.y, z);
}

} // namespace r
#endif //RT_UTILS_H