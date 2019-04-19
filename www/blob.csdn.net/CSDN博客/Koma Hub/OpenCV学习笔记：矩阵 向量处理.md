# OpenCV学习笔记：矩阵/向量处理 - Koma Hub - CSDN博客
2018年05月04日 15:01:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：339
                环境：CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
```
$ pkg-config --modversion opencv
2.4.13
```
**总体上讲：1.OpenCV 使用C语言来进行矩阵操作。不过实际上有很多C++语言的替代方案可以更高效地完成。2.在OpenCV中向量被当做是有一个维数为1的N维矩阵.**
**3.矩阵按行-行方式存储，每行以4字节（32位）对齐.**
```cpp
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<cv.h>
#include<highgui.h>
#include<iostream>
using namespace std;
int main(int argc,char**argv)
{
/*******为新矩阵分配内存*******/
    /*CvMat* cvCreateMat(int rows, int cols, int type);
        type: 矩阵元素类型. 按CV_<bit_depth>(S|U|F)C<number_of_channels> 方式指定. 
        例如: CV_8UC1 、CV_32SC2. */
    CvMat* M1 = cvCreateMat(4,4,CV_32FC1);
    /*释放矩阵内存*/
    cvReleaseMat(&M1);
    /*复制矩阵*/
    CvMat* M2 = cvCreateMat(4,4,CV_32FC1);
    CvMat* M3;
    M3=cvCloneMat(M2);
    /*初始化矩阵*/
    double a4[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    CvMat M4 = cvMat(3, 4, CV_64FC1, a4);
    //等价于:
    CvMat M5;
    cvInitMatHeader(&M5, 3, 4, CV_64FC1, a4);
    /*初始化矩阵为单位矩阵*/
    CvMat* M6 = cvCreateMat(4,4,CV_32FC1);
    cvSetIdentity(M6); 
/*******访问矩阵元素*******/
    /*假设需要访问一个2D浮点型矩阵的第（i, j）个单元.*/
    /*间接访问*/
    int i=1, j=2;
    //void cvmSet(CvMat*, int, int, double)
    cvmSet(&M5,i,j,99.0);
    //double cvmGet(const CvMat*, int, int)
    double t = cvmGet(&M5,i,j);
    printf("%lf\n",t);
    /*直接访问（假设矩阵数据按4字节行对齐）*/
    CvMat* M7 = cvCreateMat(4,4,CV_32FC1);
    int n = M7->cols;
    float *data7 = M7->data.fl;
    data7[i*n+j] = 13.0;
    printf("%lf\n",cvmGet(M7,i,j));
    /*直接访问（当数据的行对齐可能存在间隙时 possible alignment gaps）:*/
    CvMat* M8 = cvCreateMat(4,4,CV_32FC1);
    int step8 = M8->step/sizeof(float);
    float *data8 = M8->data.fl;
    (data8+i*step8)[j] = 23.0;
    printf("%lf\n",cvmGet(M8,i,j));
    /*对于初始化后的矩阵进行直接访问*/
    double a9[16];
    CvMat M9 = cvMat(3, 4, CV_64FC1, a9);
    a9[i*4+j] = 32.0; // M9(i,j)=2.0;
    printf("%lf\n",cvmGet(&M9,i,j));
    return 0;
}
```
***编译与运行：***
```
$ make
g++ main.cpp `pkg-config --cflags --libs opencv`
$ ./a.out 
99.000000
13.000000
23.000000
32.000000
```
此外，OpenCV还提供了**矩阵之间的运算、矩阵之间的元素级运算、向量乘积、单一矩阵的运算（转置、求逆等）、非齐次线性方程求解、特征值与特征向量 (矩阵为方阵)、奇异值分解（SVD）**等操作，详情请见《OpenCV中文参考手册》            
