# OpenCV的基本数据结构 - Koma Hub - CSDN博客
2018年05月03日 22:25:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：54
参考《OpenCV中文参考手册》
***1、图像数据结构***
```cpp
(1) IPL :
    IplImage
    |-- int nChannels;            // 颜色通道数目 (1,2,3,4)
    |-- int depth;                // 像素的位深:
    |                             // IPL_DEPTH_8U, IPL_DEPTH_8S,
    |                             // IPL_DEPTH_16U,IPL_DEPTH_16S,
    |                             // IPL_DEPTH_32S,IPL_DEPTH_32F,
    |                             // IPL_DEPTH_64F
    |-- int width;                // 图像宽度（像素为单位）
    |-- int height;               // 图像高度
    |-- char* imageData;          // 图像数据指针
    |                             // 注意彩色图像按BGR顺序存储数据
    |-- int dataOrder;            // 0 - 将像素点不同通道的值交错排在一起，形成单一像素平面
    |                             // 1 - 把所有像素同通道值排在一起，形成若干个通道平面，再把平面排列起来
    |                             // cvCreateImage 只能创建像素交错排列式的图像
    |-- int origin;               // 0 – 像素原点为左上角,
    |                             // 1 – 像素原点为左下角 (Windows bitmaps style)
    |-- int widthStep;            // 相邻行的同列点之间的字节数
    |-- int imageSize;            // 图像的大小（字节为单位） = height*widthStep
    |-- struct _IplROI *roi;      // 图像的感兴趣区域（ROI）. ROI非空时对图像的
    |                             // 处理仅限于ROI区域.
    |-- char *imageDataOrigin;    // 图像数据未对齐时的数据原点指针
    |                             // (需要正确地重新分配图像内存 )
    |                             // （needed for correct image deallocation）
    |-- int align;                // 图像数据的行对齐: 4 or 8 byte alignment
    |                             // OpenCV 中无此项，采用widthSt
```
***2、矩阵与向量***
```cpp
(1) 矩阵:
    CvMat                         // 2D 矩阵
    |-- int type;                 // 元素类型 (uchar,short,int,float,double) 与标志
    |-- int step;                 // 整行长度字节数
    |-- int rows, cols;           // 行、列数
    |-- int height, width;        // 矩阵高度、宽度，与rows、cols对应
    |-- union data;
        |-- uchar* ptr;           // data pointer for an unsigned char matrix
        |-- short* s;             // data pointer for a short matrix
        |-- int* i;               // data pointer for an integer matrix
        |-- float* fl;            // data pointer for a float matrix
        |-- double* db;           // data pointer for a double matrix
    CvMatND                       // N-维矩阵
    |-- int type;                 // 元素类型 (uchar,short,int,float,double) 与标志
    |-- int dims;                 // 矩阵维数
    |-- union data;
    |   |-- uchar* ptr;           // data pointer for an unsigned char matrix
    |   |-- short* s;             // data pointer for a short matrix
    |   |-- int* i;               // data pointer for an integer matrix
    |   |-- float* fl;            // data pointer for a float matrix
    |   |-- double* db;           // data pointer for a double matrix
    |
    |-- struct dim[];             // 各维信息
        |-- size;                 // 元素数目
        |-- step;                 // 元素间距（字节为单位）
    CvSparseMat                   // N-维稀疏矩阵
```
```java
(2)一般矩阵:
    CvArr*  // 仅作为函数定义的参数使用，
            // 表明函数可以接受不同类型的矩阵作为参数，
            // 例如：IplImage*, CvMat* 甚至是 CvSeq*.
            // 矩阵的类型通过矩阵头的前4个字节信息来确定
```
```cpp
(3)标量:
    CvScalar
        |-- double val[4];      //4D 向量
    初始化函数:
        // Example:
        CvScalar s = cvScalar(double val0, double val1=0, double val2=0, double val3=0); 
        CvScalar s = cvScalar(20.0);
        s.val[0]=20.0;
```
***3、其它结构类型***
```cpp
(1)点:
    CvPoint p = cvPoint(int x, int y);
    CvPoint2D32f p = cvPoint2D32f(float x, float y);
    CvPoint3D32f p = cvPoint3D32f(float x, float y, float z); //E.g.:
    p.x=5.0;
    p.y=5.0;
```
```cpp
(2)矩形框大小（以像素为精度）:
    CvSize r = cvSize(int width, int height);
    CvSize2D32f r = cvSize2D32f(float width, float height);
```
```cpp
(3)矩形框的偏置和大小:
    CvRect r = cvRect(int x, int y, int width, int height);
```
