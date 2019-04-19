# OpenCV学习笔记：创建/分配图像、图像读写保存、图像像素点访问修改 - Koma Hub - CSDN博客
2018年05月04日 13:27:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：71
个人分类：[OpenCV](https://blog.csdn.net/Rong_Toa/article/category/7627742)
                环境：CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
```
$ pkg-config --modversion opencv
2.4.13
```
以下程序的编译方法相同：
`$g++ main.cpp `pkg-config --cflags --libs opencv``
***1.创建\分配图像内存***
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
    /**
    IplImage* cvCreateImage(CvSize size, int depth, int channels);
     size: cvSize(width,height);
    depth: 像素深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,
                  IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F, 
                  IPL_DEPTH_64F
    channels: 像素通道数. Can be 1, 2, 3 or 4.
    各通道是交错排列的. 一幅彩色图像的数据排列格式如下：
        b0 g0 r0 b1 g1 r1 ...
    **/
    /*******分配内存给一个新图像******/
    //分配一个1channel的字节图像
    IplImage * img1 = cvCreateImage(cvSize(200,200), IPL_DEPTH_8U, 1);
    //分配一个3channel的浮点图像
    IplImage * img2 = cvCreateImage(cvSize(200,200), IPL_DEPTH_32F,3);
    //复制图像
    IplImage * img3 = cvCloneImage(img1);
    //释放图像
    cvReleaseImage(&img1);
    cvShowImage("win1",img3);
    cvWaitKey(0);
    return 0;
}
```
***2.图像读写、保存***
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
    /******图像读写******/
    //从文件读如图像
    IplImage *img4 = 0;
    //此种方法默认强制转换为三通道彩色图像
    img4 = cvLoadImage("wongrgb.jpg");
    /*
    img=cvLoadImage(fileName,flag);
        flag>0:三通道彩色图像
        flag=0:单通道灰度图像
        flag<0:读如的通道与图片相同
    */
    //强制转换为单通道图
    img4 = cvLoadImage("wongrgb.jpg",0);
    if(!img4)
        printf("ERROR load Image.\n");
    /******保存图像******/
    //cvSaveImage(outFileName,img);保存的格式由扩展名确定
    if(!cvSaveImage("wongrgb2.jpg",img4))
        printf("Could not save image.\n");
    //cvWaitKey(0);
    return 0;
}
```
***3.图像像素点访问与修改***
```cpp
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<cv.h>
#include<highgui.h>
#include<iostream>
using namespace std;
//首先定义一个 c++ wrapper ‘Image’，然后基于Image定义不同类型的图像:
template<class T> class Image
{
    private:
    IplImage* imgp;
    public:
    Image(IplImage* img=0) {imgp=img;}
    ~Image(){imgp=0;}
    void operator=(IplImage* img) {imgp=img;} 
    inline T* operator[](const int rowIndx) {
        return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};
typedef struct{
    unsigned char b,g,r;
} RgbPixel;
typedef struct{
    float b,g,r;
} RgbPixelFloat;
typedef Image<RgbPixel> RgbImage;
typedef Image<RgbPixelFloat> RgbImageFloat;
typedef Image<unsigned char> BwImage;
typedef Image<float> BwImageFloat;
int main(int argc,char**argv)
{
    /*****访问图像像素******/
    /*间接访问*/
    //对于单通道字节型图像
    IplImage *img5 = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,1);
    CvScalar s = cvGet2D(img5,50,50);//访问第50行第50列的像素
    printf("intensity = %f\n",s.val[0]);
    s.val[0] = 111;
    cvSet2D(img5,50,50,s);//设定（50，50）点的像素值
    cvShowImage("win1",img5);
    cvSaveImage("a.jpg",img5);
    //对多通道字节型/浮点型图像
    IplImage *img6 = cvCreateImage(cvSize(200,200),IPL_DEPTH_32F,3);
    CvScalar s6 = cvGet2D(img6,50,50);
    printf("blue=%f, green=%f, red=%f\n",s6.val[0],s6.val[1],s6.val[2]);
    s6.val[0] = 111;
    s6.val[1] = 111;
    s6.val[2] = 111;
    cvSet2D(img6,50,50,s);
    cvShowImage("win2",img6);
    /*直接访问*/
    //对于单通道字节型图像（效率高，但容易出错）
    IplImage* img7=cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,1);
    ((uchar *)(img7->imageData + 50*img7->widthStep))[40] = 255;//第50行40列
    cvShowImage("win3",img7);
    //对于多通道字节型图像:
    IplImage* img8=cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,3);
    ((uchar *)(img8->imageData + 50*img8->widthStep))[100*img8->nChannels + 0]=111; 
    ((uchar *)(img8->imageData + 50*img8->widthStep))[100*img8->nChannels + 1]=112; 
    ((uchar *)(img8->imageData + 50*img8->widthStep))[100*img8->nChannels + 2]=113;
    cvShowImage("win4",img8);
    //对于多通道浮点型图像:
    IplImage* img9=cvCreateImage(cvSize(200,200),IPL_DEPTH_32F,3);
    ((float *)(img9->imageData + 100*img9->widthStep))[50*img9->nChannels + 0]=111; 
    ((float *)(img9->imageData + 100*img9->widthStep))[50*img9->nChannels + 1]=112; 
    ((float *)(img9->imageData + 100*img9->widthStep))[50*img9->nChannels + 2]=113;
    cvShowImage("win5",img9);
    int i, j, k;
    i = 50;
    j = 90;
    k = 1;
    /*基于指针的直接访问*/
    //对于单通道字节型图像:
    IplImage* img10 = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,1);
    int height = img10->height;
    int width = img10->width;
    int step = img10->widthStep/sizeof(uchar);
    uchar* data = (uchar *)img10->imageData;
    data[i*step+j] = 111;
    cvShowImage("win6",img10);
    //对于多通道字节型图像:
    IplImage* img11 = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,1);
    int height11 = img11->height;
    int width11 = img11->width;
    int step11 = img11->widthStep/sizeof(uchar);
    uchar* data11 = (uchar *)img11->imageData;
    data11[i*step11+j] = 111;
    cvShowImage("win7",img11);
    //对于多通道浮点型图像:（假设图像数据采用4字节（32bit）行对齐方式）
    IplImage* img12 = cvCreateImage(cvSize(200,200),IPL_DEPTH_32F,1);
    int height12 = img12->height;
    int width12 = img12->width;
    int step12 = img12->widthStep/sizeof(float);
    float* data12 = (float *)img12->imageData;
    data12[i*step12+j] = 111;
    cvShowImage("win8",img12);
    /*基于 c++ wrapper 的直接访问: （更简单高效）：类：见程序开头*/
    //对于单通道字节型图像:
    IplImage* img13=cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,1);
    BwImage imgA(img13);
    imgA[i][j] = 111;
    cvShowImage("win9",img13);
    //对于多通道字节型图像:
    IplImage* img14=cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,3);
    RgbImage imgA14(img14);
    imgA14[i][j].b = 111;
    imgA14[i][j].g = 111;
    imgA14[i][j].r = 111;
    cvShowImage("win10",img14);
    //对于多通道浮点型图像:
    IplImage* img15=cvCreateImage(cvSize(200,200),IPL_DEPTH_32F,3);
    RgbImageFloat imgA15(img15);
    imgA15[i][j].b = 111;
    imgA15[i][j].g = 111;
    imgA15[i][j].r = 111;
    cvShowImage("win11",img15);
    cvWaitKey(0);
    return 0;
}
```
代码来源：《OpenCV中文参考手册》            
