# 【特征匹配】Fast原理及源码解析 - 勿在浮砂筑高台 - CSDN博客





2015年09月08日 18:34:43[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：7824
所属专栏：[特征匹配](https://blog.csdn.net/column/details/featuresdetect.html)









相关：[SIFT原理与源码解析](http://blog.csdn.net/luoshixian099/article/details/47377611)

[SURF原理与源码解析](http://blog.csdn.net/luoshixian099/article/details/47807103)

   在实时的视频流处理中，需要对每一帧特征提取，对算法处理速度上有很高的要求，传统的SIFT,Harris等特征点提取很难满足。由此提出Fast（Features from
 Accelerated Segment Test），由于不涉及尺度，梯度，等复杂运算，Fast检测器速度非常快。它使用一定邻域内像元的灰度值与中心点比较大小去判断是否为一个角点。但它的缺点是不具有方向性,尺度不变性。

转载请注明出处：http://blog.csdn.net/luoshixian099/article/details/48294967

Fast角点提取步骤（以Fast-12-16为例）：

![](https://img-blog.csdn.net/20150906221347280)


1.以固定半径为圆的边上取16个像素点（图中白色框出的位置），与中心点像素值Ip做差。

![](https://img-my.csdn.net/uploads/201203/30/1333094316_3973.png)


2.若边上存在连续的12（N>12,若为Fast-9,只需要N>9）个点满足  ( I(x)-I(p) )>threshold 或者 ( I(x)-I(p) ) < -threshold。(其中I(x)表示边上的像素值，I(p)为中心点像素值，threshold为设定的阈值。)则此点作为一个候选角点。如图上的虚线连接的位置。通常为了加速计算，直接比较1,5,9,13位置的差值，超过三个即视为一个候选点（存在连续的12个像元的必要条件），否则直接排除。

3.非极大值抑制，排除不稳定角点。采用强度响应函数：

![](https://img-blog.csdn.net/20150906222532671)


即一个角点强度值定义为中心点与边上的12个角点像素差值的绝对值累加和。

![](https://img-blog.csdn.net/20150908190335527)


opencv源码解析：

同上面原理部分不同，opencv中默认采用Fast-9-16（还包括Fast-5-8,Fast-7-12）.即在周围取16个像素点，若超过连续9个点与中心点差值大于阈值即成为候选角点。

角点强度计算方法不采用上面的公式所描述，而是采用最小的差值(见代码分析)作为其角点强度值。



```cpp
#include <stdio.h>
#include <iostream>
#include "cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
	Mat img_1 = imread( "F:\\Picture\\hotel.jpg");
	if( !img_1.data )
	{ 
		return -1; }
	FastFeatureDetector detector(50,true);  //第一个参数为阈值，第二个采用非最大值抑制
	std::vector<KeyPoint> keypoints_1;
	detector.detect( img_1, keypoints_1 );//调用FAST_t函数检测，见下面详细解析
	drawKeypoints(img_1,keypoints_1,img_1,Scalar::all(255));
	imshow("HOTEL",img_1);
	waitKey(0);
	return 0;
}
```



```cpp
void FAST_t(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold, bool nonmax_suppression)
{
    Mat img = _img.getMat();
    const int K = patternSize/2, N = patternSize + K + 1;

    int i, j, k, pixel[25];
    makeOffsets(pixel, (int)img.step, patternSize);

    keypoints.clear();

    threshold = std::min(std::max(threshold, 0), 255);//保证阈值在0-255之间。


    uchar threshold_tab[512];
    for( i = -255; i <= 255; i++ )
        threshold_tab[i+255] = (uchar)(i < -threshold ? 1 : i > threshold ? 2 : 0);  //分类成为darker、similar、brighter三种

    AutoBuffer<uchar> _buf((img.cols+16)*3*(sizeof(int) + sizeof(uchar)) + 128);
    uchar* buf[3];
    buf[0] = _buf; buf[1] = buf[0] + img.cols; buf[2] = buf[1] + img.cols;//保存对应角点强度值，否则为0
    int* cpbuf[3];
    cpbuf[0] = (int*)alignPtr(buf[2] + img.cols, sizeof(int)) + 1;//保存角点位置，+1为了存储这一行的角点总数
    cpbuf[1] = cpbuf[0] + img.cols + 1;
    cpbuf[2] = cpbuf[1] + img.cols + 1;
    memset(buf[0], 0, img.cols*3);

    for(i = 3; i < img.rows-2; i++)
    {
        const uchar* ptr = img.ptr<uchar>(i) + 3;
        uchar* curr = buf[(i - 3)%3];
        int* cornerpos = cpbuf[(i - 3)%3];
        memset(curr, 0, img.cols);
        int ncorners = 0;

        if( i < img.rows - 3 )
        {
            j = 3;
			/*采用9点分割测试，加快检测速度
			  检测任意一个直径两端的像素点，若同时与中心点相似，必定不是角点
			  因为至少要占一半的数量
			*/
            for( ; j < img.cols - 3; j++, ptr++ )
            {
                int v = ptr[0];
                const uchar* tab = &threshold_tab[0] - v + 255;
      int d = tab[ptr[pixel[0]]] | tab[ptr[pixel[8]]];//
                if( d == 0 )      // 加快检测速度[0]与[8]两个点都与中心点灰度值相近，排除这个点
                    continue;

                d &= tab[ptr[pixel[2]]] | tab[ptr[pixel[10]]];//直径两端两个点都相近，则为0
                d &= tab[ptr[pixel[4]]] | tab[ptr[pixel[12]]];//
                d &= tab[ptr[pixel[6]]] | tab[ptr[pixel[14]]];//每隔45度选取一个点

                if( d == 0 )    //
                    continue;

                d &= tab[ptr[pixel[1]]] | tab[ptr[pixel[9]]];
                d &= tab[ptr[pixel[3]]] | tab[ptr[pixel[11]]];
                d &= tab[ptr[pixel[5]]] | tab[ptr[pixel[13]]];
                d &= tab[ptr[pixel[7]]] | tab[ptr[pixel[15]]];

                if( d & 1 )   //     darker   中心值大，周围小的情况
                {
                    int vt = v - threshold, count = 0;

                    for( k = 0; k < N; k++ )   //且连续一半的像素点灰度差值( v-x > threshold )大于阈值
                    {
                        int x = ptr[pixel[k]];
                        if(x < vt)     
                        {
                            if( ++count > K )
                            {
                             cornerpos[ncorners++] = j;
                             if(nonmax_suppression)//非最大值抑制
                        curr[j] = (uchar)cornerScore<patternSize>(ptr, pixel, threshold);//计算角点的强度响应值,最小的差值（绝对值）
                            break;
                            }
                        }
                        else
                            count = 0;
                    }
                }

                if( d & 2 )//brighter 中心值小，周围值大的情况
                {
                    int vt = v + threshold, count = 0;

                    for( k = 0; k < N; k++ )   //连续一半的像素点灰度差值(  x-v < threshold )大于阈值
                    {
                        int x = ptr[pixel[k]];
                        if(x > vt)
                        {
                            if( ++count > K )
                            {
                            cornerpos[ncorners++] = j;
                             if(nonmax_suppression)
                        curr[j] = (uchar)cornerScore<patternSize>(ptr, pixel, threshold);//计算角点的强度响应值,最小的差值（绝对值）
                            break;
                            }
                        }
                        else
                            count = 0;
                    }
                }
            }
        }

        cornerpos[-1] = ncorners;//存储第i行上的角点总数量

        if( i == 3 )
            continue;
       /*与邻域的8个角点响应值做比较，非角点的响应值为0*/
        const uchar* prev = buf[(i - 4 + 3)%3];  //相邻的两行
        const uchar* pprev = buf[(i - 5 + 3)%3];//
        cornerpos = cpbuf[(i - 4 + 3)%3];//存储角点的列位置
        ncorners = cornerpos[-1]; //存储第i行上的角点总数量

        for( k = 0; k < ncorners; k++ )
        {
            j = cornerpos[k];
            int score = prev[j];
            if( !nonmax_suppression ||    //非极大值抑制,用角点强度值比较周围8个强度响应值
               (score > prev[j+1] && score > prev[j-1] &&
                score > pprev[j-1] && score > pprev[j] && score > pprev[j+1] &&
                score > curr[j-1] && score > curr[j] && score > curr[j+1]) )
            {
		keypoints.push_back(KeyPoint((float)j, (float)(i-1), 7.f, -1, (float)score));
            }
        }
    }
}
```

角点的强度计算方法：若采用Fast-9-16,计算连续的9个位置与中心位置的差值的绝对值，取最小的一个差值作为其强度值。



```cpp
int cornerScore<16>(const uchar* ptr, const int pixel[], int threshold)//角点强度计算
{
    const int K = 8, N = K*3 + 1;
    int k, v = ptr[0];
    short d[N];
    for( k = 0; k < N; k++ )    //计算与周围16个像素点的差值,保存在d[k]中
        d[k] = (short)(v - ptr[pixel[k]]);
    int a0 = threshold;
    for( k = 0; k < 16; k += 2 )  //周围像素小于中心点像素
    {
        int a = std::min((int)d[k+1], (int)d[k+2]);
        a = std::min(a, (int)d[k+3]);
        if( a <= a0 )
            continue;
        a = std::min(a, (int)d[k+4]);
        a = std::min(a, (int)d[k+5]);
        a = std::min(a, (int)d[k+6]);
        a = std::min(a, (int)d[k+7]);
        a = std::min(a, (int)d[k+8]);
        a0 = std::max(a0, std::min(a, (int)d[k]));
        a0 = std::max(a0, std::min(a, (int)d[k+9]));
    }

    int b0 = -a0;
    for( k = 0; k < 16; k += 2 )//周围像素点大于中心像素点
    {
        int b = std::max((int)d[k+1], (int)d[k+2]);
        b = std::max(b, (int)d[k+3]);
        b = std::max(b, (int)d[k+4]);
        b = std::max(b, (int)d[k+5]);
        if( b >= b0 )
            continue;
        b = std::max(b, (int)d[k+6]);
        b = std::max(b, (int)d[k+7]);
        b = std::max(b, (int)d[k+8]);

        b0 = std::min(b0, std::max(b, (int)d[k]));
        b0 = std::min(b0, std::max(b, (int)d[k+9]));
    }

    threshold = -b0-1;

    return threshold;
}
```



参考文章：Edward Rosten et.:Machine Learning for High-Speed Corner Detection

                http://blog.csdn.net/kezunhai/article/details/11290749

                http://www.edwardrosten.com/work/fast.html






