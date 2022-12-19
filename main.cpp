#include <vector>
// #include <stdio.h>
#include <math.h>
#include <complex>


const float WIDTH = 800.0;
const float HEIGHT = 600.0;

#define PI 3.1415926;

class Vector4 {
    public:
        float m[4];
        Vector4() {
            memset(m, 0, sizeof(float) * 4);
            m[3] = 1.0;
        }
        Vector4(const float _m[4]){
            memcpy(m, _m, sizeof(float) * 4);
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
        Vector4 operator*(Vector4 &v) const {
            Vector4 ret;
            for (int i = 0; i < 4; i++) {
                ret.m[i] = 0;
                for (int j = 0; j < 4; j++) {
                    ret.m[i] += m[i][j] * v.m[j];
                }
            }
            return ret;
        }
};



Matrix4x4 getProjection(float near, float far, float aspect, float fov) {
    Matrix4x4 ortho;
    Matrix4x4 orthoTranslate;
    Matrix4x4 orthoScale;
    Matrix4x4 orthoToPerspect;

    float depth = near - far;
    float rad = (fov / 2.0 ) / 180.0 * PI;
    float halfHeight = std::tan(rad) * -near;
    float t = halfHeight;
    float b = -t;
    float l = aspect * t;
    float r = -l;


    float trans[4][4] = {
        {1.0, 0.0,0.0,-(l+r) /2},
        {0.0, 0.0,0.0,-(t+b) /2},
        {0.0, 0.0,0.0,-(near+far) /2},
        {0.0, 0.0,0.0,1.0},
    };

    memcpy(orthoTranslate.m, trans, sizeof(float) * 16);

    float scale[4][4] = {
        {2/ (r-l), 0.0,0.0,0.0},
        {0.0, 2/ (t - b),0.0,0.0},
        {0.0, 0.0,2 / (far - near),0.0},
        {0.0, 0.0,0.0,1.0},
    };

    memcpy(orthoScale.m, scale, sizeof(float) * 16);

    ortho = orthoScale * orthoTranslate;

    float o2p[4][4] = {
        {near, 0.0,0.0,-(l+r) /2},
        {0.0, near,0.0,-(t+b) /2},
        {0.0, 0.0,near + far,-(near * far)},
        {0.0, 0.0,near,0.0},
    };

    memcpy(orthoToPerspect.m, o2p, sizeof(float) * 16);

    return ortho * orthoToPerspect;
}


class Scene {
    std::vector<float> colorBuffer;
    std::vector<float> depthBuffer;
};

std::vector<Vector4> getPoints(){
    std::vector<Vector4> ret;
    std::vector<float[4]> points = {
        {0.0, 0.0, 0.0, 1.0 },
        {100.0, 0.0, 0.0, 1.0},
        {50.0, 50.0, 0.0, 1.0}
    };
    for (int i = 0; i < points.size(); i++) {
        ret[i] = Vector4(points[i]);
    }
    return ret;
}


// scene流程
// mvp变换

// 后处理
// depth test
// anti-aliasing


// 产出结果


int main(int argc, char const *argv[])
{
    // vector 1 2 3
    
    std::vector<Vector4> points = getPoints();
    
    // scene
    // m matrix 设置坐标点 为原点
    float moduleMatrix[4][4] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    Matrix4x4 module = Matrix4x4(moduleMatrix);

    // v matrix
    // set look at(0,0,0) \ up direct (0,1,0) \ position (0,0,-1)
    float viewMatrix[4][4] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -(-1.0),
        0.0, 0.0, 0.0, 1.0
    };
    Matrix4x4 view = Matrix4x4(viewMatrix);
    
    //p matrix
    // set nar far aspect fov
    Matrix4x4 projection = getProjection(0.01, 1000, WIDTH / HEIGHT , 45);
    

    // render 
    // 获得转换后的本地坐标系坐标
    std::vector<Vector4> localPoints = {};
    for (int i = 0; i < points.size(); i++) {
        localPoints[i] =  projection * points[i];
    }


    // rasterasing
    // 根据aabb判断哪些像素点需要被着色

    // 根据三角形重力坐标计算颜色的插值

    // 根据depth buffer 判断是否需要把color buffer更新

    // 输出结果

    

    /* code */
    return 0;
}





