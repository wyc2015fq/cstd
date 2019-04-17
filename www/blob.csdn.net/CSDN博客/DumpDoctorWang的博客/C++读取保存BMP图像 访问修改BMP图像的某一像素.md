# C++读取保存BMP图像 访问修改BMP图像的某一像素 - DumpDoctorWang的博客 - CSDN博客





2018年05月15日 11:20:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1997








本篇博客贴上一个实现了读取、保存、新建BMP图像和修改访问BMP某一像素的类。自己实现的数据结构，没有使用其他头文件，理论上是跨平台的。需要注意的是文件编码，VS需要使用UTF-8 with BOM，gcc或者Linux平台要使用UTF-8。

一、实现过程

说下我的思路，首先查阅BMP图像的文件格式，根据格式写出BMP图像的文件头、信息头等数据结构，然后在写出读BMP图像的代码；然后再写出保存BMP图像的代码；然后在写出创建BMP图像的代码；最后写出修改访问BMP图像的代码；

给出参考链接：

[BMP格式详解](https://blog.csdn.net/u012877472/article/details/50272771) (准确，非常推荐)、[BMP文件格式](https://blog.csdn.net/lanbing510/article/details/8176231) （写出了字节数等信息，第4点有错误）

二、BMP组成

BMP图像文件是二进制文件，组成为：BMP文件头(BMPFileHeader)、BMP信息头(BMPInfoHeader)、调色板(Color Palette，可选，不是必有的)、图像的实际数据。所以读取和保存BMP图像的顺序为：BMP文件头、BMP信息头、调色板(如果有)、图像数据。

三、代码

本文代码不在更新，最新代码在https://gitee.com/Mannix1994/BMPImage

1、BMPImage类头文件

```cpp
//BMPImage.h
#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <string>

/** ------------------数据类型重定义------------------------ */
typedef long int4;              // 4 bytes
typedef unsigned int uint;     // 4 bytes
typedef unsigned char uchar;   // 1 byte
typedef unsigned long uint4;   // 4 bytes
typedef unsigned long ulong;   // 4 bytes
typedef unsigned short uint2;  // 2 bytes
/** -------------------------------------------------------- */

/** ------------------颜色结构体重定义---------------------- */
// Color Palette的结构体，也是RGBA颜色结构体
typedef struct {
    uchar b;
    uchar g;
    uchar r;
    uchar a;
}BGRA,ColorPalette;

// RGB颜色结构体
typedef struct {
    uchar b;
    uchar g;
    uchar r;
}BGR;
/** -------------------------------------------------------- */

class BMPImage {
private:
/** -------------BMP文件头和BMP信息头数据结构定义----------- */
//参考链接如下
//https://blog.csdn.net/u012877472/article/details/50272771
//https://blog.csdn.net/lanbing510/article/details/8176231
//必须要加这一行，否则BMPFileHeader的size不等于14字节
#pragma  pack(2)
    //bmp文件头（bmp file header）：共14字节；
    typedef struct {
        uint2 bfType;           //2字节，文件类型；总应是0x4d42('BM')
        uint4 bfSize;           //4字节，文件大小；字节为单位。
        uint2 bfReserved1;      //2字节，保留，必须设置为0
        uint2 bfReserved2;      //2字节，保留，必须设置为0
        uint4 bfOffBits;        //4字节的偏移，表示从文件头到位图数据的偏移
    } BMPFileHeader;

    //位图信息头（bitmap information）：共40字节；
    typedef struct {
        uint4 biSize;           //4字节，信息头的大小，即40；
        int4 biWidth;           //4字节，以像素为单位说明图像的宽度；
        int4 biHeight;          //4字节，以像素为单位说明图像的高度；如果为正，说明位图倒立
        uint2 biPlanes;         //2字节，为目标设备说明颜色平面数，总被设置为1；
        uint2 biBitCount;       //2字节，说明比特数/像素数，值有1、2、4、8、16、24、32；
        uint4 biCompression;    //4字节，说明图像的压缩类型，0(BI_RGB)表示不压缩；
        uint4 biSizeImage;      //4字节，说明位图数据的大小，当用BI_RGB格式时，可以设置为0；
        int4 biXPelsPerMeter;   //4字节，表示水平分辨率，单位是像素/米；
        int4 biYPelsPerMeter;   //4字节，表示垂直分辨率，单位是像素/米；
        uint4 biClrUsed;        //4字节，位图使用的调色板中的颜色索引数，为0说明使用所有；
        uint4 biClrImportant;   //4字节，对图像显示有重要影响的颜色索引数，为0说明都重要；
    } BMPInfoHeader;
// 恢复默认
#pragma  pack(8)
/** -------------------------------------------------------- */

/** ---------------------公有函数区------------------------- */
public:
    /**
     * 构造函数，初始化一个空的BMPImage
     */
    explicit BMPImage();
    /**
     * 构造函数，载入一张BMP图像,打开失败时会抛出异常
     * @param bmp_file_path 图像路径
     */
    explicit BMPImage(const char* bmp_file_path);
    /**
     * 构造函数，创建大小为width*height的24位的黑色图像，
     * 创建失败时会抛出异常。
     * @param width 宽度
     * @param height 高度
     * @param depth 深度，目前仅支持24位和32位图像
     */
    explicit BMPImage(int4 width, int4 height, uint2 depth = 24);
    /**
     * 析构函数
     */
    virtual ~BMPImage();
    /**
      * 创建大小为width*height的24位的黑色图像
      * @param width 宽度
      * @param height 高度
      * @param depth 深度，目前仅支持24位图像
      * @return 成功返回true，否则false
      */
    bool create(int4 width,int4 height,uint2 depth=24);
    /**
     * 载入一张BMP图像
     * @param bmp_file_path 图像路径
     * @return 成功返回true，否则false
     */
    bool open(const char* bmp_file_path);
    /**
     * 保存BMP图像
     * @param save_path 保存路径
     * @return 成功返回true，否则false
     */
    bool save(const char* save_path);
    /**
     * 返回(row,col)处RGB值(对于24位BMP图像)的引用
     * @param row 行
     * @param col 列
     * @return RGB值的引用
     */
    BGR & rgb(uint row, uint col);
    /**
     * 返回(row,col)处RGBA值(对于32位BMP图像)的引用
     * @param row 行
     * @param col 列
     * @return RGBA值的引用
     */
    BGRA & rgba(uint row, uint col);
    /**
     * 清空图像
     * @return 成功返回true，否则false
     */
    bool clear();

    /**
     * 返回图像宽度
     * @return 宽度
     */
    int4 width();
    /**
     * 返回图像高度
     * @return  高度
     */
    int4 height();
    /**
     * 返回图像深度
     * @return 深度
     */
    uint2 depth();
/** -------------------------------------------------------- */

/** ----------------------私有变量区------------------------ */
private:
    int4 m_width;       //图像宽度
    int4 m_height;      //图像深度
    uint2 m_depth;      //图像深度
    ulong m_row_size;   //每行字节数
    uchar *m_data;      //图像数据存储区

    BMPFileHeader m_file_header;    //BMP图像的文件头
    BMPInfoHeader m_info_header;    //BMP图像的信息头
    uint m_color_palette_size;      //BMP的调色板数组大小
    ColorPalette *m_color_palette;  //调色板(Color Palette),可选；
};
/** -------------------------------------------------------- */

#endif //BMPIMAGE_H
```

2、BMPImage的实现

```cpp
//BMPImage.cpp
#include "BMPImage.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

/**
 * condition为false时抛出错误，错误信息为error_message
 */
#define ASSERT(condition,error_message) ((condition)?0:(assertion(__FILE__, __func__, __LINE__,error_message),-1))

inline void assertion(const std::string &filePath, const std::string &function,
                      int line, const std::string &info){
    //获取文件名
    unsigned long long int pos=filePath.find_last_of('/');
    std::string filename(filePath.substr(pos+1));
    std::string err = filename+" "+function+" "+std::to_string(line)+">>  "+info;
    //抛出错误
    throw std::runtime_error(err);
}

BMPImage::BMPImage():m_width(0),m_height(0),m_depth(8),m_row_size(0),
                     m_data(nullptr),m_color_palette_size(0),m_color_palette(nullptr) {
    ASSERT(sizeof(BMPFileHeader)==14,"The size of BMPFileHeader is not 14 bytes");
    ASSERT(sizeof(BMPInfoHeader)==40,"The size of BMPInfoHeader is not 40 bytes");
}

BMPImage::BMPImage(const char* bmp_file_path):BMPImage() {
    char buffer[500];
    sprintf(buffer,"Failed to open bmp image %s\n\0",bmp_file_path);
    ASSERT(open(bmp_file_path),buffer);
}

BMPImage::BMPImage(int4 width, int4 height, uint2 depth):BMPImage() {
    ASSERT(create(width , height, depth),"Failed to open bmp image\n");
}

BMPImage::~BMPImage() {
    delete[] m_data;
    delete[] m_color_palette;
}

bool BMPImage::open(const char* bmp_file_path) {
    clear();
    ifstream im_file(bmp_file_path,ios::binary);
    // 检查文件是否打开
    if(!im_file.is_open()){
        printf("Failed to open file %s\n",bmp_file_path);
        return false;
    }
    // -------------------读取BMPFileHeader-------------------------
    im_file.read((char *)(&m_file_header), sizeof(BMPFileHeader));
    // 判断是否是bmp图像
    if (m_file_header.bfType != 0x4d42) // 0x4d42 = 'BM'
    {
        printf("File %s is not bmp file\n",bmp_file_path);
        return false;
    }
    // --------------------读取BMPInfoHeader------------------------
    im_file.read((char *)(&m_info_header), sizeof(BMPInfoHeader));
    // ---------判断是否有调色板,如果有，则读入调色板数据-----------
    if (m_file_header.bfOffBits == 54){//说明没有调色板
        m_color_palette = nullptr;
    } else{
        // 计算调色板数量
        m_color_palette_size = (m_file_header.bfOffBits - 54)/4;
        m_color_palette = new ColorPalette[m_color_palette_size];
        // 读取调色板数据
        im_file.read((char*)(m_color_palette), m_color_palette_size * sizeof(ColorPalette));
    }
    // 给属性赋值
    m_width = m_info_header.biWidth;
    m_height = m_info_header.biHeight;
    m_depth = m_info_header.biBitCount;
    // 计算每行的字节数
    m_row_size = 4ul * (m_info_header.biBitCount * m_info_header.biWidth/32ul);
    // ---------------------------读取图像数据--------------------------------
    // 申请内存空间并同时初始化为0
    m_data = new uchar[m_info_header.biSizeImage]();
    // 读取图像数据到内存
    im_file.read((char *)m_data,m_info_header.biSizeImage);
    // 关闭文件
    im_file.close();
    return true;
}

bool BMPImage::save(const char* save_path) {
    if (strlen(save_path) == 0){
        printf("Your file path is empty");
        return false;
    }
    ofstream out_image(save_path,ios::binary);
    if (!out_image.is_open()){
        printf("Failed to save image %s",save_path);
        return false;
    }
    // ---------------------保存BMP文件头-----------------------
    out_image.write((char*)&m_file_header, sizeof(BMPFileHeader));
    // ---------------------保存BMP信息头-----------------------
    out_image.write((char*)&m_info_header, sizeof(BMPInfoHeader));
    // ---------------------保存图像数据------------------------
    out_image.write((char*)m_data, m_info_header.biSizeImage);
    out_image.close();
    return true;
}

bool BMPImage::create(int4 width, int4 height, uint2 depth) {
    // 目前不支持调色板
    clear();
    ulong bytes_per_pixel = 0;
    switch (depth){
        case 24:
            bytes_per_pixel = 3;
            break;
        case 32:
            bytes_per_pixel = 4;
            break;
        default:
            ASSERT(false,"The depth must be 24 or 32");
    }
    // 目前不支持调色板
    m_width = width;
    m_height = height;
    m_depth = depth;
    // 计算每行的字节数，每行的字节数需要是4的倍数
    m_row_size = 4ul * ((bytes_per_pixel * m_width+4ul-1ul)/4ul);
    // ---------------------初始化BMP文件头---------------------
    m_file_header.bfType = 0x4d42;
    m_file_header.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    m_file_header.bfSize = m_file_header.bfOffBits + m_height * m_row_size;
    m_file_header.bfReserved1 = 0;
    m_file_header.bfReserved2 = 0;
    // ---------------------初始化BMP信息头----------------------
    m_info_header.biSize = sizeof(BMPInfoHeader);//Should be 40
    m_info_header.biWidth = m_width;
    m_info_header.biHeight = m_height;
    m_info_header.biPlanes = 1;
    m_info_header.biBitCount = m_depth;
    m_info_header.biCompression = 0;
    m_info_header.biSizeImage = m_height * m_row_size;
    m_info_header.biXPelsPerMeter = 3780;
    m_info_header.biYPelsPerMeter = 3780;
    m_info_header.biClrUsed = 0;
    m_info_header.biClrImportant = 0;
    // 申请内存并初始化为0
    m_data = new uchar[m_info_header.biSizeImage]();
    return true;
}

BGR & BMPImage::rgb(uint row, uint col) {
    if(row >= m_height){
        clear();
        printf("Parameter row is big than picture height\n");
        ASSERT(false, "Parameter row is big than picture height");
    }
    if(col >= m_width){
        clear();
        printf("Parameter col is big than picture width\n");
        ASSERT(false, "Parameter col is big than picture width");
    }
    if(m_depth != 24){
        clear();
        printf("Error: Not a 24bit image\n");
        ASSERT(false, "Not a 24bit image");
    }
    BGR *rgb = nullptr;
    size_t elem_size = 3; //24位图像的的每个像素位占三个字节,24位
    // 计算索引
    size_t index = 0;
    if (m_height > 0){
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的最后一部分
        index = (m_height-row-1) * m_row_size + col*elem_size;
    }else{
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的第一部分
        index = row * m_row_size + col * elem_size;
    }
    rgb = (BGR*)(m_data+index);
    return *rgb;
}

BGRA &BMPImage::rgba(uint row, uint col) {
    if(row >= m_height){
        clear();
        printf("Parameter row is big than picture height\n");
        ASSERT(false, "Parameter row is big than picture height");
    }
    if(col >= m_width){
        clear();
        printf("Parameter col is big than picture width\n");
        ASSERT(false, "Parameter col is big than picture width");
    }
    if(m_depth != 32){
        clear();
        printf("Error: Not a 32bit image\n");
        ASSERT(false, "Not a 32bit image");
    }
    BGRA *rgba = nullptr;
    size_t elem_size = 4; //24位图像的的每个像素位占4个字节,32位
    // 计算索引
    size_t index = 0;
    if (m_height > 0){
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的最后一部分
        index = (m_height-row-1) * m_row_size + col*elem_size;
    }else{
        // 图像数据的存储顺序是从左下到右上，图片上的第一行存在m_data的第一部分
        index = row * m_row_size + col * elem_size;
    }
    rgba = (BGRA*)(m_data+index);
    return *rgba;
}

bool BMPImage::clear() {
    delete [] m_data;
    delete [] m_color_palette;
    return true;
}

int4 BMPImage::width() {
    return abs(m_width);
}

int4 BMPImage::height() {
    return abs(m_height);
}

uint2 BMPImage::depth() {
    return m_depth;
}
```

3、测试代码

```cpp
//Main.cpp
#include <iostream>
#include "BMPImage.h"

void testBMP(){
    // -----------创建一张24位纯黑的图像----------
    BMPImage bmp24_101;
    bmp24_101.create(101,101);
    bmp24_101.save("../24_101.bmp");
    BGR green = {0,255,0};
    // 将左上的50*20像素修改为绿色
    for(uint4 i = 0;i<50;i++){
        for(uint4 j = 0;j<20;j++){
            bmp24_101.rgb(i, j) = green;
        }
    }
    // 保存
    bmp24_101.save("../24_101_1.bmp");

    // -----------创建一张24位纯黑图像--------
    BMPImage bmp_24_500(500,500);
    // 将左上的50*20像素修改为绿色
    for(uint4 i = 0;i<50;i++){
        for(uint4 j = 0;j<20;j++){
            bmp_24_500.rgb(i, j) = green;
        }
    }
    //保存
    bmp_24_500.save("../24_500.bmp");

    // -----------创建一张32位的纯黑图像------
    BMPImage bmp_32(233,233,32);
    BGRA red{0,0,255,255};
    // 将左上的50*20像素修改为红色
    for(uint4 i = 0;i<50;i++){
        for(uint4 j = 0;j<20;j++){
            bmp_32.rgba(i, j) = red;
        }
    }
    bmp_32.save("../32.bmp");

    // ----------打开一张位图--------------
    BMPImage bmpImage1("../No.bmp");
    // 将一块区域修改为绿色
    for(uint4 i = 100;i<200;i++){
        for(uint4 j = 100;j<200;j++){
            bmpImage1.rgb(i, j) = green;
        }
    }
    // 保存
    bmpImage1.save("../2.bmp");

    // ----------打开一张32位图--------------
    BMPImage bmp_o_32("../32.bmp");
    // 将一块区域修改为蓝色
    BGRA blue{255,0,0,255};
    for(uint4 i = 100;i<200;i++){
        for(uint4 j = 100;j<200;j++){
            bmp_o_32.rgba(i, j) = blue;
        }
    }
    // 保存
    bmp_o_32.save("../32_1.bmp");
}

int main() {

    testBMP();
    std::cout << "Hello, World!" << std::endl;
    //getchar();
    return 0;
}
```







