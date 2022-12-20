#include <vector>
// #include <stdio.h>
#include <math.h>
#include <complex>


const float WIDTH = 800.0;
const float HEIGHT = 600.0;

#define PI 3.1415926;
// #define WIDTH = 800.0;
// #define HEIGHT = 600.0;

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
        float x(){
            return m[0];
        }
        float y(){
            return m[1];
        }
        float z(){
            return m[2];
        }
        float w(){
            return m[3];
        }
        float cross(Vector4 &v){
            float _m[4];
            _m[0] = m[1] * v.m[2] - m[2]*v.m[1];
            _m[1] = m[2] * v.m[0] - m[0]*v.m[2];
            _m[2] = m[0] * v.m[1] - m[1]*v.m[0];
        }

        Vector4 operator-(Vector4 &v){
            float _m[4];
            for (int i = 0; i < 4; i++) {
                _m[i] = m[i] - v.m[i];
            }
            return _m;
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



class BmpGenerator {
    public:
        int width;
        int height;
        const int BYTES_PER_PIXEL = 3;
        const int FILE_HEADER_SIZE = 14;
        const int INFO_HEADER_SIZE = 40;
        BmpGenerator(int _width, int _height){
            width = _width;
            height = _height;
        }

        unsigned char* createBitmapFileHeader(int stride){
            int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride  * height);
            static unsigned char fileHeader[] = {
                0,0,
                0,0,0,0,
                0,0,0,0,
                0,0,0,0,
            };

            fileHeader[ 0] = (unsigned char)('B');
            fileHeader[ 1] = (unsigned char)('M');
            fileHeader[ 2] = (unsigned char)(fileSize      );
            fileHeader[ 3] = (unsigned char)(fileSize >>  8);
            fileHeader[ 4] = (unsigned char)(fileSize >> 16);
            fileHeader[ 5] = (unsigned char)(fileSize >> 24);
            fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

            return fileHeader;
        }

        unsigned char* createBitmapInfoHeader(){
            static unsigned char infoHeader[] = {
                0,0,0,0, /// header size
                0,0,0,0, /// image width
                0,0,0,0, /// image height
                0,0,     /// number of color planes
                0,0,     /// bits per pixel
                0,0,0,0, /// compression
                0,0,0,0, /// image size
                0,0,0,0, /// horizontal resolution
                0,0,0,0, /// vertical resolution
                0,0,0,0, /// colors in color table
                0,0,0,0, /// important color count
            };

            infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
            infoHeader[ 4] = (unsigned char)(width      );
            infoHeader[ 5] = (unsigned char)(width >>  8);
            infoHeader[ 6] = (unsigned char)(width >> 16);
            infoHeader[ 7] = (unsigned char)(width >> 24);
            infoHeader[ 8] = (unsigned char)(height      );
            infoHeader[ 9] = (unsigned char)(height >>  8);
            infoHeader[10] = (unsigned char)(height >> 16);
            infoHeader[11] = (unsigned char)(height >> 24);
            infoHeader[12] = (unsigned char)(1);
            infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

            return infoHeader;
        }

        void export(std::vector<std::vector<std::vector<float>>> *buffer, char* fileName) {
            int widthInBytes = width * BYTES_PER_PIXEL;

            unsigned char padding[3] = {0,0,0};
            int paddingSize = (4 - (widthInBytes) % 4) % 4;

            int stride = (widthInBytes) + paddingSize;

            FILE* imageFile = fopen(fileName, "wb");

            unsigned char* fileHeader = createBitmapFileHeader(stride);
            fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

            unsigned char* infoHeader = createBitmapInfoHeader();
            fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

            
            for (int i = 0; i < height; i++) {
                fwrite(buffer + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
                fwrite(padding, 1, paddingSize, imageFile);
            }

            fclose(imageFile);
        }
};

bool inTrangle(float x, float y, std::vector<Vector4> points) {
    bool ret = false;
    Vector4 l0 = points[0] - points[1];
    Vector4 l1 = points[1] - points[2];
    Vector4 l2 = points[2] - points[0];

    float r0 = l0.cross(l1);
    float r1 = l1.cross(l2);
    float r2 = l2.cross(l0);

    if ((r0 >=0 && r1 >=0 && r2 >=0 )|| (r0 <0 && r1 <0 && r2 <0)) {
        ret = true;
    }
    return ret;
}

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

void rasterization (std::vector<std::vector<std::vector<float>>> &frameBuffer, std::vector<Vector4> points) {
    std::vector<float> tempColor = {255.0,255.0,255.0};
    float x_min = 0;
    float x_max = 0;
    float y_min = 0;
    float y_max = 0;
    for (int i = 0; i < points.size(); i++) {
        Vector4 point = points[i];
        x_min = std::min(x_min, point.x());
        x_max = std::max(x_max, point.x());
        y_min = std::min(y_min, point.y());
        y_max = std::max(y_max, point.y());
    }

    float buffer[800][600];

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++){
            bool flag = false;
            // aabb judge
            if (i < x_max && i > x_min && j < y_max && j > y_min) {
                if (inTrangle(i,j, points)) {
                    flag = true;
                    frameBuffer[i][j] = tempColor;
                }
            }



            if (flag) {
                // cal 重力坐标

                // cal z value 

                // update depth buffer

                // get color
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
    std::vector<std::vector<std::vector<float>>> frameBuffer;
    // float *** frameBuffer;
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
    rasterization(frameBuffer, points);

    // 根据三角形重力坐标计算颜色的插值

    // 根据depth buffer 判断是否需要把color buffer更新

    // 输出结果

    BmpGenerator bmp = BmpGenerator(WIDTH, HEIGHT);
    bmp.export(frameBuffer,"test.bmp");

    

    /* code */
    return 0;
}





