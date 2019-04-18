# C++读取、旋转和保存 bmp 图像文件编程实现 - 文章 - 伯乐在线
原文出处： [江南烟雨，2011-07-26](http://blog.csdn.net/xiajun07061225/article/details/6633938)
通过我这些天用C++读写bmp图像的经历，摸索再摸索，终于对bmp文件的结构、操作有了一定的了解，下面就大概介绍bmp图片纯C++的读取、旋转和保存的实现过程。
要用C++读取bmp图片文件，首先要弄清楚bmp格式图片文件的结构。可以参考这篇文章：[http://blog.csdn.net/xiajun07061225/article/details/5813726](http://blog.csdn.net/xiajun07061225/article/details/5813726)
有几点需要注意的是：
在读取bmp图片的时候，一定要注意内存对齐的问题，譬如文件头，否则无法读取出正确结果。
关于图片的像素数据，每一行的像素的字节数必须是4的整数倍。如果不是，则需要补齐。一般来说，bmp图像文件的数据是从下到上，从左到右的。即从文件中最先读到的是图像最下面一行的左边第一个像素，然后是坐标第二个…..接下来是倒数第二行的第一个像素。
采用的编译环境是VS2008。
关于图像旋转，并不难。只需要搞清楚像素坐标变换公式就行。我以图像的中心点为坐标原点。先把像素在目标图像中的位置变化为坐标系中的位置，做旋转变换求出变换之前的在坐标系中的坐标，再变换为在图片中的位置。
公式：(x1,y1)是变换之前的坐标系中的坐标，(x2,y2)是变换之后的坐标系中的坐标。angle为逆时针旋转的角度数。
x1 = cos(angle)*x2-sin(angle)*y2;
y1 = sin(angle)*x2-cos(angle)*y2;
我的代码分为两个版本：灰度图的和彩色图的。
#### 灰度图：
灰度图是只含亮度信息，不含彩色信息的图像。bmp格式文件中并没有灰度图这个概念，但是我们很容易地用bmp文件来表示灰度图。方法是用256色的调色板，只不过这个调色板有点特殊，每一项的RGB值都是相同的，从(0,0,0),(1,1,1),…,一直到(255,255,255)。这样，灰度图就可以用256色图来表示了。其图像数据就是调色板索引值，也就是实际的RGB的亮度值。另外因为是256色的调色板，所以图像数据中的一个字节代表一个像素。如果是彩色的256色图，图像处理后可能会产生不属于这256色的颜色，所以，图像处理一般采用灰度图。这也可以更好地将重点放在算法上。
下面是灰度图旋转代码，能处理任意尺寸的bmp灰度图，以及旋转任意角度(逆时针)。
代码包括两个文件：BmpRot.h和BmpRot.cpp
BmpRot.h：


```
typedef unsigned char BYTE;  
typedef unsigned short WORD;  
typedef unsigned int DWORD;  
typedef long LONG;  
  
//位图文件头定义;  
//其中不包含文件类型信息（由于结构体的内存结构决定，  
//要是加了的话将不能正确读取文件信息）  
typedef struct  tagBITMAPFILEHEADER{  
    //WORD bfType;//文件类型，必须是0x424D，即字符“BM”  
    DWORD bfSize;//文件大小  
    WORD bfReserved1;//保留字  
    WORD bfReserved2;//保留字  
    DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数  
}BITMAPFILEHEADER;  
  
typedef struct tagBITMAPINFOHEADER{  
    DWORD biSize;//信息头大小  
    LONG biWidth;//图像宽度  
    LONG biHeight;//图像高度  
    WORD biPlanes;//位平面数，必须为1  
    WORD biBitCount;//每像素位数  
    DWORD  biCompression; //压缩类型  
    DWORD  biSizeImage; //压缩图像大小字节数  
    LONG  biXPelsPerMeter; //水平分辨率  
    LONG  biYPelsPerMeter; //垂直分辨率  
    DWORD  biClrUsed; //位图实际用到的色彩数  
    DWORD  biClrImportant; //本位图中重要的色彩数  
}BITMAPINFOHEADER; //位图信息头定义  
  
typedef struct tagRGBQUAD{  
    BYTE rgbBlue; //该颜色的蓝色分量  
    BYTE rgbGreen; //该颜色的绿色分量  
    BYTE rgbRed; //该颜色的红色分量  
    BYTE rgbReserved; //保留值  
}RGBQUAD;//调色板定义  
  
//像素信息  
typedef struct tagIMAGEDATA  
{  
    BYTE blue;  
    //BYTE green;  
    //BYTE red;  
}IMAGEDATA;
```
BmpRot.cpp：


```
#include <stdio.h>  
#include "BmpRot.h"  
#include "stdlib.h"  
#include "math.h"  
#include <iostream>  
  
#define PI 3.14159//圆周率宏定义  
#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度  
  
using namespace std;  
  
//变量定义  
BITMAPFILEHEADER strHead;  
RGBQUAD strPla[256];//256色调色板  
BITMAPINFOHEADER strInfo;  
  
//显示位图文件头信息  
void showBmpHead(BITMAPFILEHEADER pBmpHead){  
    cout<<"位图文件头:"<<endl;  
    cout<<"文件大小:"<<pBmpHead.bfSize<<endl;  
    cout<<"保留字_1:"<<pBmpHead.bfReserved1<<endl;  
    cout<<"保留字_2:"<<pBmpHead.bfReserved2<<endl;  
    cout<<"实际位图数据的偏移字节数:"<<pBmpHead.bfOffBits<<endl<<endl;  
}  
  
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){  
    cout<<"位图信息头:"<<endl;  
    cout<<"结构体的长度:"<<pBmpInforHead.biSize<<endl;  
    cout<<"位图宽:"<<pBmpInforHead.biWidth<<endl;  
    cout<<"位图高:"<<pBmpInforHead.biHeight<<endl;  
    cout<<"biPlanes平面数:"<<pBmpInforHead.biPlanes<<endl;  
    cout<<"biBitCount采用颜色位数:"<<pBmpInforHead.biBitCount<<endl;  
    cout<<"压缩方式:"<<pBmpInforHead.biCompression<<endl;  
    cout<<"biSizeImage实际位图数据占用的字节数:"<<pBmpInforHead.biSizeImage<<endl;  
    cout<<"X方向分辨率:"<<pBmpInforHead.biXPelsPerMeter<<endl;  
    cout<<"Y方向分辨率:"<<pBmpInforHead.biYPelsPerMeter<<endl;  
    cout<<"使用的颜色数:"<<pBmpInforHead.biClrUsed<<endl;  
    cout<<"重要颜色数:"<<pBmpInforHead.biClrImportant<<endl;  
}  
  
  
int main(){  
    char strFile[LENGTH_NAME_BMP];//bmp文件名  
    IMAGEDATA *imagedata = NULL;//动态分配存储原图片的像素信息的二维数组  
    IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组  
    int width,height;//图片的宽度和高度  
    cout<<"请输入所要读取的文件名:"<<endl;  
    cin>>strFile;  
    FILE *fpi,*fpw;  
    fpi=fopen(strFile,"rb");  
    if(fpi != NULL){  
        //先读取文件类型  
        WORD bfType;  
        fread(&bfType,1,sizeof(WORD),fpi);  
        if(0x4d42!=bfType)  
        {  
            cout<<"the file is not a bmp file!"<<endl;  
            return NULL;  
        }  
        //读取bmp文件的文件头和信息头  
        fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);  
        //showBmpHead(strHead);//显示文件头  
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);  
        //showBmpInforHead(strInfo);//显示文件信息头  
  
        //读取调色板  
        for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
        {  
            fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);  
        }  
  
        width = strInfo.biWidth;  
        height = strInfo.biHeight;  
        //图像每一行的字节数必须是4的整数倍  
        width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;  
        //imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));  
        imagedata = (IMAGEDATA*)malloc(width * height);  
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));  
        //初始化原始图片的像素数组  
        for(int i = 0;i < height;++i)  
        {  
            for(int j = 0;j < width;++j)  
            {  
                (*(imagedata + i * width + j)).blue = 0;  
                //(*(imagedata + i * width + j)).green = 0;  
                //(*(imagedata + i *  width + j)).red = 0;  
            }  
        }  
        //初始化旋转后图片的像素数组  
        for(int i = 0;i < 2 * height;++i)  
        {  
            for(int j = 0;j < 2 * width;++j)  
            {  
                (*(imagedataRot + i * 2 * width + j)).blue = 0;  
                //(*(imagedataRot + i * 2 * width + j)).green = 0;  
                //(*(imagedataRot + i * 2 * width + j)).red = 0;  
            }  
        }  
        //fseek(fpi,54,SEEK_SET);  
        //读出图片的像素数据  
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);  
        fclose(fpi);  
    }  
    else  
    {  
        cout<<"file open error!"<<endl;  
        return NULL;  
    }  
  
    //图片旋转处理  
    int RotateAngle;//要旋转的角度数  
    double angle;//要旋转的弧度数  
    int midX_pre,midY_pre,midX_aft,midY_aft;//旋转所围绕的中心点的坐标  
    midX_pre = width / 2;  
    midY_pre = height / 2;  
    midX_aft = width;  
    midY_aft = height;  
    int pre_i,pre_j,after_i,after_j;//旋转前后对应的像素点坐标  
    cout<<"输入要旋转的角度（0度到360度，逆时针旋转）："<<endl;  
    cin>>RotateAngle;  
    angle = 1.0 * RotateAngle * PI / 180;  
    for(int i = 0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            after_i = i - midX_aft;//坐标变换  
            after_j = j - midY_aft;  
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;  
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;  
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//在原图范围内  
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);  
        }  
    }  
  
    //保存bmp图片  
    if((fpw=fopen("b.bmp","wb"))==NULL)  
    {  
        cout<<"create the bmp file error!"<<endl;  
        return NULL;  
    }  
    WORD bfType_w=0x4d42;  
    fwrite(&bfType_w,1,sizeof(WORD),fpw);  
    //fpw +=2;  
    fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);  
    strInfo.biWidth = 2 * width;  
    strInfo.biHeight = 2 * height;  
    fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);  
    //保存调色板数据  
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
    {  
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
    }  
    //保存像素数据  
    for(int i =0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            fwrite( &((*(imagedataRot + i * 2 * width + j)).blue),1,sizeof(BYTE),fpw);  
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);  
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);  
        }  
    }  
    fclose(fpw);  
  
    //释放内存  
    delete[] imagedata;  
    delete[] imagedataRot;  
}
```
数据测试：
旋转前和旋转后的对比（45度）：
![](http://jbcdn2.b0.upaiyun.com/2016/12/fd3f343ca82f9ba0eddecc7a69ccf19a.gif)![](http://jbcdn2.b0.upaiyun.com/2016/12/89394a84a0d929342cdd595e89ad9e6c.gif)
![](http://jbcdn2.b0.upaiyun.com/2016/12/2867c4427d0acd70f83460baf15d0e5e.gif)
![](http://jbcdn2.b0.upaiyun.com/2016/12/5f3069fec5bd6fa8edf3f8c1157e54e0.gif)
#### 彩色图：
彩色图的处理和灰度图略有不一样。主要是像素数据不同。由于每行数据的字节数必须是4的整数倍，这个地方处理起来要比灰度图麻烦很多，多以暂时还 没做好。本程序的局限性就是只能处理尺寸是4的整数倍的图片，可以旋转任意角度(逆时针)。
参考代码：分两个文件：BmpRot.h和BmpRot.cpp
BmpRot.h：


```
typedef unsigned char BYTE;  
typedef unsigned short WORD;  
typedef unsigned int DWORD;  
typedef long LONG;  
  
//位图文件头定义;  
//其中不包含文件类型信息（由于结构体的内存结构决定，  
//要是加了的话将不能正确读取文件信息）  
typedef struct  tagBITMAPFILEHEADER{  
    //WORD bfType;//文件类型，必须是0x424D，即字符“BM”  
    DWORD bfSize;//文件大小  
    WORD bfReserved1;//保留字  
    WORD bfReserved2;//保留字  
    DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数  
}BITMAPFILEHEADER;  
  
typedef struct tagBITMAPINFOHEADER{  
    DWORD biSize;//信息头大小  
    LONG biWidth;//图像宽度  
    LONG biHeight;//图像高度  
    WORD biPlanes;//位平面数，必须为1  
    WORD biBitCount;//每像素位数  
    DWORD  biCompression; //压缩类型  
    DWORD  biSizeImage; //压缩图像大小字节数  
    LONG  biXPelsPerMeter; //水平分辨率  
    LONG  biYPelsPerMeter; //垂直分辨率  
    DWORD  biClrUsed; //位图实际用到的色彩数  
    DWORD  biClrImportant; //本位图中重要的色彩数  
}BITMAPINFOHEADER; //位图信息头定义  
  
typedef struct tagRGBQUAD{  
    BYTE rgbBlue; //该颜色的蓝色分量  
    BYTE rgbGreen; //该颜色的绿色分量  
    BYTE rgbRed; //该颜色的红色分量  
    BYTE rgbReserved; //保留值  
}RGBQUAD;//调色板定义  
  
//像素信息  
typedef struct tagIMAGEDATA  
{  
    BYTE red;  
    BYTE green;  
    BYTE blue;  
}IMAGEDATA;
```
BmpRot.cpp：


```
#include <stdio.h>  
#include "BmpRot.h"  
#include "stdlib.h"  
#include "math.h"  
#include <iostream>  
  
#define PI 3.14159//圆周率宏定义  
#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度  
  
using namespace std;  
  
//变量定义  
BITMAPFILEHEADER strHead;  
RGBQUAD strPla[256];//256色调色板  
BITMAPINFOHEADER strInfo;  
  
//显示位图文件头信息  
void showBmpHead(BITMAPFILEHEADER pBmpHead){  
    cout<<"位图文件头:"<<endl;  
    cout<<"文件大小:"<<pBmpHead.bfSize<<endl;  
    cout<<"保留字_1:"<<pBmpHead.bfReserved1<<endl;  
    cout<<"保留字_2:"<<pBmpHead.bfReserved2<<endl;  
    cout<<"实际位图数据的偏移字节数:"<<pBmpHead.bfOffBits<<endl<<endl;  
}  
  
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){  
    cout<<"位图信息头:"<<endl;  
    cout<<"结构体的长度:"<<pBmpInforHead.biSize<<endl;  
    cout<<"位图宽:"<<pBmpInforHead.biWidth<<endl;  
    cout<<"位图高:"<<pBmpInforHead.biHeight<<endl;  
    cout<<"biPlanes平面数:"<<pBmpInforHead.biPlanes<<endl;  
    cout<<"biBitCount采用颜色位数:"<<pBmpInforHead.biBitCount<<endl;  
    cout<<"压缩方式:"<<pBmpInforHead.biCompression<<endl;  
    cout<<"biSizeImage实际位图数据占用的字节数:"<<pBmpInforHead.biSizeImage<<endl;  
    cout<<"X方向分辨率:"<<pBmpInforHead.biXPelsPerMeter<<endl;  
    cout<<"Y方向分辨率:"<<pBmpInforHead.biYPelsPerMeter<<endl;  
    cout<<"使用的颜色数:"<<pBmpInforHead.biClrUsed<<endl;  
    cout<<"重要颜色数:"<<pBmpInforHead.biClrImportant<<endl;  
}  
  
  
int main(){  
    char strFile[LENGTH_NAME_BMP];//bmp文件名  
    IMAGEDATA *imagedata = NULL;//动态分配存储原图片的像素信息的二维数组  
    IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组  
    int width,height;//图片的宽度和高度  
    cout<<"请输入所要读取的文件名:"<<endl;  
    cin>>strFile;  
    FILE *fpi,*fpw;  
    fpi=fopen(strFile,"rb");  
    if(fpi != NULL){  
        //先读取文件类型  
        WORD bfType;  
        fread(&bfType,1,sizeof(WORD),fpi);  
        if(0x4d42!=bfType)  
        {  
            cout<<"the file is not a bmp file!"<<endl;  
            return NULL;  
        }  
        //读取bmp文件的文件头和信息头  
        fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);  
        //showBmpHead(strHead);//显示文件头  
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);  
        //showBmpInforHead(strInfo);//显示文件信息头  
  
        //读取调色板  
        for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
        {  
            //存储的时候，一般去掉保留字rgbReserved  
            fread((char *)&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpi);  
            fread((char *)&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpi);  
            fread((char *)&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpi);  
            cout<<"strPla[nCounti].rgbBlue"<<strPla[nCounti].rgbBlue<<endl;  
            cout<<"strPla[nCounti].rgbGreen"<<strPla[nCounti].rgbGreen<<endl;  
            cout<<"strPla[nCounti].rgbRed"<<strPla[nCounti].rgbRed<<endl;  
        }  
  
        width = strInfo.biWidth;  
        height = strInfo.biHeight;  
        imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));  
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));  
        //初始化原始图片的像素数组  
        for(int i = 0;i < height;++i)  
        {  
            for(int j = 0;j < width;++j)  
            {  
                (*(imagedata + i * width + j)).blue = 0;  
                (*(imagedata + i * width + j)).green = 0;  
                (*(imagedata + i *  width + j)).red = 0;  
            }  
        }  
        //初始化旋转后图片的像素数组  
        for(int i = 0;i < 2 * height;++i)  
        {  
            for(int j = 0;j < 2 * width;++j)  
            {  
                (*(imagedataRot + i * 2 * width + j)).blue = 0;  
                (*(imagedataRot + i * 2 * width + j)).green = 0;  
                (*(imagedataRot + i * 2 * width + j)).red = 0;  
            }  
        }  
        //fseek(fpi,54,SEEK_SET);  
        //读出图片的像素数据  
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);  
        /* 
        for(int i = 0;i < height;++i) 
        { 
            fread(imagedata + i * width * sizeof(IMAGEDATA),sizeof(struct tagIMAGEDATA) * width,height,fpi); 
        }*/  
        fclose(fpi);  
    }  
    else  
    {  
        cout<<"file open error!"<<endl;  
        return NULL;  
    }  
  
    //图片旋转处理  
    int RotateAngle;//要旋转的角度数  
    double angle;//要旋转的弧度数  
    int midX_pre,midY_pre,midX_aft,midY_aft;//旋转所围绕的中心点的坐标  
    midX_pre = width / 2;  
    midY_pre = height / 2;  
    midX_aft = width;  
    midY_aft = height;  
    int pre_i,pre_j,after_i,after_j;//旋转前后对应的像素点坐标  
    cout<<"输入要旋转的角度（0度到360度，逆时针旋转）："<<endl;  
    cin>>RotateAngle;  
    angle = 1.0 * RotateAngle * PI / 180;  
    for(int i = 0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            after_i = i - midY_aft;//坐标变换  
            after_j = j - midX_aft;  
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midY_pre;  
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midX_pre;  
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//在原图范围内  
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);  
        }  
    }  
  
    //保存bmp图片  
    if((fpw=fopen("b.bmp","wb"))==NULL)  
    {  
        cout<<"create the bmp file error!"<<endl;  
        return NULL;  
    }  
    WORD bfType_w=0x4d42;  
    fwrite(&bfType_w,1,sizeof(WORD),fpw);  
    //fpw +=2;  
    fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);  
    strInfo.biWidth = 2 * width;  
    strInfo.biHeight = 2 * height;  
    fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);  
    //保存调色板数据  
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
    {  
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
    }  
    //保存像素数据  
    for(int i =0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            fwrite( &(*(imagedataRot + i * 2 * width + j)).red,1,sizeof(BYTE),fpw);//注意三条语句的顺序：否则颜色会发生变化  
            fwrite( &(*(imagedataRot + i * 2 * width + j)).green,1,sizeof(BYTE),fpw);  
            fwrite( &(*(imagedataRot + i * 2 * width + j)).blue,1,sizeof(BYTE),fpw);  
        }  
    }  
    fclose(fpw);  
  
    //释放内存  
    delete[] imagedata;  
    delete[] imagedataRot;  
}
```
数据测试：（旋转10°）
![](http://jbcdn2.b0.upaiyun.com/2016/12/cb7871e28454ec38d9fc0295eae14f57.gif)![](http://img.blog.csdn.net/20130620091842031)
注意：颜色问题已解决。请看代码中注释部分。
