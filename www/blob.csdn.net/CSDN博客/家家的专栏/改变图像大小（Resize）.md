# 改变图像大小（Resize） - 家家的专栏 - CSDN博客





2010年08月25日 19:59:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3923








Resize
图像大小变换 

void cvResize( const CvArr* src, CvArr* dst, int interpolation=CV_INTER_LINEAR );
src 
输入图像. 
dst 
输出图像. 
interpolation 
插值方法: 
CV_INTER_NN - 最近邻插值, 
CV_INTER_LINEAR - 双线性插值 (缺省使用) 
CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于 CV_INTER_NN 方法.. 
CV_INTER_CUBIC - 立方插值. 
函数 cvResize 将图像 src 改变尺寸得到与 dst 同样大小。若设定 ROI，函数将按常规支持 ROI. 



#include <cv.h>
#include <highgui.h>
#include <math.h>

int main(int argc, char* argv[])
{
    IplImage *src = 0;			//源图像指针
    IplImage *dst = 0;			//目标图像指针
    float scale = 0.618;		//缩放倍数为0.618倍
    CvSize dst_cvsize;			//目标图像尺寸

    /* the first command line parameter must be image file name */
    if ( argc == 2 && (src = cvLoadImage(argv[1], -1))!=0 )
    {
        //如果命令行传递了需要打开的图片就无须退出，所以注释掉下面一行！
	//return -1;
    }
    else
    {
        src = cvLoadImage("tiger.jpg");    //载入工作目录下文件名为“tiger.jpg”的图片。
    }

    dst_cvsize.width = src->width * scale;		//目标图像的宽为源图象宽的scale倍
    dst_cvsize.height = src->height * scale;	//目标图像的高为源图象高的scale倍

    dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	//构造目标图象
cvResize(src, dst, CV_INTER_LINEAR);	//缩放源图像到目标图像

    cvNamedWindow( "src",   CV_WINDOW_AUTOSIZE );	//创建用于显示源图像的窗口
    cvNamedWindow( "dst",   CV_WINDOW_AUTOSIZE );	//创建用于显示目标图像的窗口

    cvShowImage( "src", src );		//显示源图像
    cvShowImage( "dst", dst );		//显示目标图像

    cvWaitKey(-1);		//等待用户响应

    cvReleaseImage(&src);	//释放源图像占用的内存
    cvReleaseImage(&dst);	//释放目标图像占用的内存
    cvDestroyWindow( "src" );	//销毁窗口“src”
    cvDestroyWindow( "dst" );	//销毁窗口“dst”
    //void cvDestroyAllWindows(void);

    return 0;
}



