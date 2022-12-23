#include <vector>
#include <stdio.h>
#include <math.h>
#include <complex>
#include <iostream>
#include <cstring>


constexpr float WIDTH = 800.0;
constexpr float HEIGHT = 600.0;

#define PI 3.1415926;

inline float clamp(float max, float min, float v) {
    return std::min(max, std::max(min,v));
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
        {near, 0.0,0.0,0.0},
        {0.0, near,0.0,0.0},
        {0.0, 0.0,near + far,-(near * far)},
        {0.0, 0.0,near,0.0},
    };

    memcpy(orthoToPerspect.m, o2p, sizeof(float) * 16);

    return ortho * orthoToPerspect;
}

void exportImg(std::vector<Vector4f> frameBuffer){
    FILE* fp = fopen("img.ppm","wb");
    int w = WIDTH;
    int h = HEIGHT;
    (void)fprintf(fp, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < frameBuffer.size(); i++) {
        static unsigned char color[3];
        
        Vector4f pixel = frameBuffer[i];
        color[0] = (char)(255 * clamp(1.0,0., pixel.x));
        color[1] = (char)(255 * clamp(1.0,0., pixel.y));
        color[2] = (char)(255 * clamp(1.0,0., pixel.z));
        // if (pixel.x > 0 || pixel.y > 0) {
        //     std::cout << " frame: x:" << pixel.x << "y:" << pixel.y << 255.0 * clamp(1.0,0., pixel.x) << std::endl;
        //     std::cout << " frame: index:" << i << std::endl;
        //     std::cout << " color: x:" << color[0] << "y:" << color[1] << std::endl;
        // }
        
        fwrite(color, 1, 3, fp);
    }

    fclose(fp);
}

bool inTrangle(float x, float y, std::vector<Vector4f> points) {
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

std::vector<Vector4f> getPoints(){
    std::vector<Vector4f> ret = {
        Vector4f(0.0, 50.0, -1.0),
        Vector4f(100.0, 50.0, -1.0),
        Vector4f(50.0, 100.0, -1.0),
    };
    return ret;
}

void rasterization (std::vector<Vector4f> &frameBuffer, std::vector<float> &depthBuffer, std::vector<Vector4f> &points, std::vector<Vector4f> &colors) {
    Vector4f defaultColor = Vector4f(1.0,1.0,1.0);
    float x_min = 0;
    float x_max = 0;
    float y_min = 0;
    float y_max = 0;
    for (int i = 0; i < points.size(); i++) {
        Vector4f point = points[i];
        x_min = std::min(x_min, point.x);
        x_max = std::max(x_max, point.x);
        y_min = std::min(y_min, point.y);
        y_max = std::max(y_max, point.y);
    }
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++){
            bool flag = false;
            int index = (HEIGHT - j) * WIDTH + i;
            // aabb judge
            if (i < x_max && i > x_min && j < y_max && j > y_min) {
                if (inTrangle(i,j, points)) {
                    flag = true;
                    // (frameBuffer[index]) = defaultColor;
                }
            }

            if (flag) {
                // compute 重力坐标
                std::vector<float> bray = getBarycentric2D(i,j,points);
                float a = bray[0];
                float b = bray[1];
                float g = bray[2];
                // cal z value 
                float cur_z = points[0].z * a + points[1].z * a + points[2].z * g;
                // judge depth buffer
                if (depthBuffer[index] - cur_z >= .0001) {
                    depthBuffer[index] = cur_z;
                    // get color
                    // Vector4f color = ( colors[0] * a + colors[1] * b + colors[2] * g);
                    Vector4f tem1 = colors[0] * a;
                    Vector4f tem2 = colors[1] * b;
                    Vector4f tem3 = colors[2] * g;
                    Vector4f color = tem1 + tem2 + tem3;
                    frameBuffer[index] = color;
                }
            }
        }
    }
}

// scene流程
// mvp变换

// 后处理
// depth test
// anti-aliasing


// 产出结果


int main(int argc, char const *argv[])
{
    std::vector<Vector4f> frameBuffer(WIDTH * HEIGHT);
    std::vector<float> depthBuffer(WIDTH * HEIGHT,0);
    

    std::vector<Vector4f> points = getPoints();
    std::vector<Vector4f> colors = {
        Vector4f(1.0,0.0,0.0),
        Vector4f(0.0,1.0,0.0),
        Vector4f(0.0,0.0,1.0),
    };
    
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
        1.0, 0.0, 0.0, -(0.0),
        0.0, 1.0, 0.0, -(0.0),
        0.0, 0.0, 1.0, -(-1.0),
        0.0, 0.0, 0.0, 1.0
    };
    Matrix4x4 view = Matrix4x4(viewMatrix);
    
    //p matrix
    // set nar far aspect fov
    Matrix4x4 projection = getProjection(0.01, 1000, WIDTH / HEIGHT , 45);
    

    // render 
    // 获得转换后的本地坐标系坐标
    std::vector<Vector4f> localPoints;
    for (int i = 0; i < points.size(); i++) {
        localPoints.push_back(projection * points[i]);
    }


    // rasterasing
    // 根据aabb判断哪些像素点需要被着色
    rasterization(frameBuffer,depthBuffer, points, colors);

    // 根据三角形重力坐标计算颜色的插值

    // 根据depth buffer 判断是否需要把color buffer更新
    
    exportImg(frameBuffer);
    // 输出结果
    printf("gen done ");
    

    /* code */
    return 0;
}





