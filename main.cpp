#include <vector>
// #include <stdio.h>


const float WIDTH = 800.0;
const float HEIGHT = 600.0;

class Matrix4x4 {
    public:
        float m[4][4];
        Matrix4x4() {
            memset(m, 0, sizeof(float) * 16);
            m[1][1] = m[2][2] = m[3][3] = 1.0;
        }
        const Matrix4x4 operator*(float v) const {
            Matrix4x4 ret;
            for (int i = 0; i < 4 ; i++) {
                for (int j = 0; j < 4; j++) {
                    ret.m[i][j] = m[i][j] * v;
                }
            }
            return ret;
        }
        Matrix4x4 operator*(Matrix4x4 &mat) const {
            
        }
 

};


// vertex
struct Vec3
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

struct Mat4
{
    std::vector<float> matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
};

class Scene {
    std::vector<float> colorBuffer;
    std::vector<float> depthBuffer;
};



// scene流程
// mvp变换

// 后处理
// depth test
// anti-aliasing


// 产出结果


int main(int argc, char const *argv[])
{
    // vector 1 2 3
    std::vector<Vec3> points = {
        {0.0, 0.0, 0.0 },
        {100.0, 0.0, 0.0},
        {50.0, 50.0, 0.0}
    };
    // scene
    // m matrix 设置坐标点 为原点
    Mat4 module;
    module.matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // v matrix
    // set look at(0,0,0) \ up direct (0,1,0) \ position (0,0,-1)
    Mat4 view;
    view.matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -(-1.0),
        0.0, 0.0, 0.0, 1.0
    };
    
    //p matrix
    Mat4 projection;
    // set nar far aspect fov
    projection.matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    // ortho matrix
    Mat4 ortho;
    Mat4 orthoToPerspect;
    // ortho scale
    Mat4 orthoScale;
    orthoScale.matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    Mat4 orthoPos;
    orthoPos.matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // ortho = orthoScale * orthoPos;
    // projection =  ortho * orthoToPerspect;

    


    // render 
    // 获得转换后的本地坐标系坐标
    std::vector<float> localPoints = {};


    // rasterasing
    // 根据aabb判断哪些像素点需要被着色

    // 根据三角形重力坐标计算颜色的插值

    // 根据depth buffer 判断是否需要把color buffer更新

    // 输出结果

    

    /* code */
    return 0;
}





