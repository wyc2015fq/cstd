# 前景检测算法（二）--codebook和平均背景法 - 工作笔记 - CSDN博客





2016年07月05日 07:58:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6980
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

# 原文：[http://www.cnblogs.com/tornadomeet/archive/2012/04/08/2438158.html](http://www.cnblogs.com/tornadomeet/archive/2012/04/08/2438158.html)

# [前景检测算法_1(codebook和平均背景法)](http://www.cnblogs.com/tornadomeet/archive/2012/04/08/2438158.html)


      前景分割中一个非常重要的研究方向就是背景减图法，因为背景减图的方法简单，原理容易被想到，且在智能视频监控领域中，摄像机很多情况下是固定的，且背景也是基本不变或者是缓慢变换的，在这种场合背景减图法的应用驱使了其不少科研人员去研究它。

      但是背景减图获得前景图像的方法缺点也很多：比如说光照因素，遮挡因素，动态周期背景，且背景非周期背景，且一般情况下我们考虑的是每个像素点之间独立，这对实际应用留下了很大的隐患。

      这一小讲主要是讲简单背景减图法和codebook法。



**一、简单背景减图法的工作原理。**

      在视频对背景进行建模的过程中，每2帧图像之间对应像素点灰度值算出一个误差值，在背景建模时间内算出该像素点的平均值，误差平均值，然后在平均差值的基础上+-误差平均值的常数(这个系数需要手动调整)倍作为背景图像的阈值范围，所以当进行前景检测时，当相应点位置来了一个像素时，如果来的这个像素的每个通道的灰度值都在这个阈值范围内，则认为是背景用0表示，否则认为是前景用255表示。

      下面的一个工程是learning opencv一书中作者提供的源代码，关于简单背景减图的代码和注释如下：

***avg_background.h文件：***

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 2 // Accumulate average and ~std (really absolute difference) image and use this to detect background and foreground
 3 //
 4 // Typical way of using this is to:
 5 //     AllocateImages();
 6 ////loop for N images to accumulate background differences
 7 //    accumulateBackground();
 8 ////When done, turn this into our avg and std model with high and low bounds
 9 //    createModelsfromStats();
10 ////Then use the function to return background in a mask (255 == foreground, 0 == background)
11 //    backgroundDiff(IplImage *I,IplImage *Imask, int num);
12 ////Then tune the high and low difference from average image background acceptance thresholds
13 //    float scalehigh,scalelow; //Set these, defaults are 7 and 6. Note: scalelow is how many average differences below average
14 //    scaleHigh(scalehigh);
15 //    scaleLow(scalelow);
16 ////That is, change the scale high and low bounds for what should be background to make it work.
17 ////Then continue detecting foreground in the mask image
18 //    backgroundDiff(IplImage *I,IplImage *Imask, int num);
19 //
20 //NOTES: num is camera number which varies from 0 ... NUM_CAMERAS - 1.  Typically you only have one camera, but this routine allows
21 //          you to index many.
22 //
23 #ifndef AVGSEG_
24 #define AVGSEG_
25 
26 
27 #include "cv.h"                // define all of the opencv classes etc.
28 #include "highgui.h"
29 #include "cxcore.h"
30 
31 //IMPORTANT DEFINES:
32 #define NUM_CAMERAS   1              //This function can handle an array of cameras
33 #define HIGH_SCALE_NUM 7.0            //How many average differences from average image on the high side == background
34 #define LOW_SCALE_NUM 6.0        //How many average differences from average image on the low side == background
35 
36 void AllocateImages(IplImage *I);
37 void DeallocateImages();
38 void accumulateBackground(IplImage *I, int number=0);
39 void scaleHigh(float scale = HIGH_SCALE_NUM, int num = 0);
40 void scaleLow(float scale = LOW_SCALE_NUM, int num = 0);
41 void createModelsfromStats();
42 void backgroundDiff(IplImage *I,IplImage *Imask, int num = 0);
43 
44 #endif

![复制代码](http://common.cnblogs.com/images/copycode.gif)



 


     ***avg_background.cpp文件:***


```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

  1 // avg_background.cpp : 定义控制台应用程序的入口点。
  2 //
  3 
  4 #include "stdafx.h"
  5 #include "avg_background.h"
  6 
  7 
  8 //GLOBALS
  9 
 10 IplImage *IavgF[NUM_CAMERAS],*IdiffF[NUM_CAMERAS], *IprevF[NUM_CAMERAS], *IhiF[NUM_CAMERAS], *IlowF[NUM_CAMERAS];
 11 IplImage *Iscratch,*Iscratch2,*Igray1,*Igray2,*Igray3,*Imaskt;
 12 IplImage *Ilow1[NUM_CAMERAS],*Ilow2[NUM_CAMERAS],*Ilow3[NUM_CAMERAS],*Ihi1[NUM_CAMERAS],*Ihi2[NUM_CAMERAS],*Ihi3[NUM_CAMERAS];
 13 
 14 float Icount[NUM_CAMERAS];
 15 
 16 void AllocateImages(IplImage *I)  //I is just a sample for allocation purposes
 17 {
 18     for(int i = 0; i<NUM_CAMERAS; i++){
 19         IavgF[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 20         IdiffF[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 21         IprevF[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 22         IhiF[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 23         IlowF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3 );
 24         Ilow1[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 25         Ilow2[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 26         Ilow3[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 27         Ihi1[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 28         Ihi2[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 29         Ihi3[i] = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 30         cvZero(IavgF[i]  );
 31         cvZero(IdiffF[i]  );
 32         cvZero(IprevF[i]  );
 33         cvZero(IhiF[i] );
 34         cvZero(IlowF[i]  );        
 35         Icount[i] = 0.00001; //Protect against divide by zero
 36     }
 37     Iscratch = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 38     Iscratch2 = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 3 );
 39     Igray1 = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 40     Igray2 = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 41     Igray3 = cvCreateImage( cvGetSize(I), IPL_DEPTH_32F, 1 );
 42     Imaskt = cvCreateImage( cvGetSize(I), IPL_DEPTH_8U, 1 );
 43 
 44     cvZero(Iscratch);
 45     cvZero(Iscratch2 );
 46 }
 47 
 48 void DeallocateImages()
 49 {
 50     for(int i=0; i<NUM_CAMERAS; i++){
 51         cvReleaseImage(&IavgF[i]);
 52         cvReleaseImage(&IdiffF[i] );
 53         cvReleaseImage(&IprevF[i] );
 54         cvReleaseImage(&IhiF[i] );
 55         cvReleaseImage(&IlowF[i] );
 56         cvReleaseImage(&Ilow1[i]  );
 57         cvReleaseImage(&Ilow2[i]  );
 58         cvReleaseImage(&Ilow3[i]  );
 59         cvReleaseImage(&Ihi1[i]   );
 60         cvReleaseImage(&Ihi2[i]   );
 61         cvReleaseImage(&Ihi3[i]  );
 62     }
 63     cvReleaseImage(&Iscratch);
 64     cvReleaseImage(&Iscratch2);
 65 
 66     cvReleaseImage(&Igray1  );
 67     cvReleaseImage(&Igray2 );
 68     cvReleaseImage(&Igray3 );
 69 
 70     cvReleaseImage(&Imaskt);
 71 }
 72 
 73 // Accumulate the background statistics for one more frame
 74 // We accumulate the images, the image differences and the count of images for the 
 75 //    the routine createModelsfromStats() to work on after we're done accumulating N frames.
 76 // I        Background image, 3 channel, 8u
 77 // number    Camera number
 78 void accumulateBackground(IplImage *I, int number)
 79 {
 80     static int first = 1;
 81     cvCvtScale(I,Iscratch,1,0); //To float;#define cvCvtScale cvConvertScale #define cvScale cvConvertScale
 82     if (!first){
 83         cvAcc(Iscratch,IavgF[number]);//将2幅图像相加：IavgF[number]=IavgF[number]+Iscratch，IavgF[]里面装的是时间序列图片的累加
 84         cvAbsDiff(Iscratch,IprevF[number],Iscratch2);//将2幅图像相减：Iscratch2=abs(Iscratch-IprevF[number]);
 85         cvAcc(Iscratch2,IdiffF[number]);//IdiffF[]里面装的是图像差的累积和
 86         Icount[number] += 1.0;//累积的图片帧数计数
 87     }
 88     first = 0;
 89     cvCopy(Iscratch,IprevF[number]);//执行完该函数后，将当前帧数据保存为前一帧数据
 90 }
 91 
 92 // Scale the average difference from the average image high acceptance threshold
 93 void scaleHigh(float scale, int num)//设定背景建模时的高阈值函数
 94 {
 95     cvConvertScale(IdiffF[num],Iscratch,scale); //Converts with rounding and saturation
 96     cvAdd(Iscratch,IavgF[num],IhiF[num]);//将平均累积图像与误差累积图像缩放scale倍然后再相加
 97     cvCvtPixToPlane( IhiF[num], Ihi1[num],Ihi2[num],Ihi3[num], 0 );//#define cvCvtPixToPlane cvSplit,且cvSplit是将一个多通道矩阵转换为几个单通道矩阵
 98 }
 99 
100 // Scale the average difference from the average image low acceptance threshold
101 void scaleLow(float scale, int num)//设定背景建模时的低阈值函数
102 {
103     cvConvertScale(IdiffF[num],Iscratch,scale); //Converts with rounding and saturation
104     cvSub(IavgF[num],Iscratch,IlowF[num]);//将平均累积图像与误差累积图像缩放scale倍然后再相减
105     cvCvtPixToPlane( IlowF[num], Ilow1[num],Ilow2[num],Ilow3[num], 0 );
106 }
107 
108 //Once you've learned the background long enough, turn it into a background model
109 void createModelsfromStats()
110 {
111     for(int i=0; i<NUM_CAMERAS; i++)
112     {
113         cvConvertScale(IavgF[i],IavgF[i],(double)(1.0/Icount[i]));//此处为求出累积求和图像的平均值
114         cvConvertScale(IdiffF[i],IdiffF[i],(double)(1.0/Icount[i]));//此处为求出累计误差图像的平均值
115         cvAddS(IdiffF[i],cvScalar(1.0,1.0,1.0),IdiffF[i]);  //Make sure diff is always something，cvAddS是用于一个数值和一个标量相加
116         scaleHigh(HIGH_SCALE_NUM,i);//HIGH_SCALE_NUM初始定义为7，其实就是一个倍数
117         scaleLow(LOW_SCALE_NUM,i);//LOW_SCALE_NUM初始定义为6
118     }
119 }
120 
121 // Create a binary: 0,255 mask where 255 means forground pixel
122 // I        Input image, 3 channel, 8u
123 // Imask    mask image to be created, 1 channel 8u
124 // num        camera number.
125 //
126 void backgroundDiff(IplImage *I,IplImage *Imask, int num)  //Mask should be grayscale
127 {
128     cvCvtScale(I,Iscratch,1,0); //To float;
129 //Channel 1
130     cvCvtPixToPlane( Iscratch, Igray1,Igray2,Igray3, 0 );
131     cvInRange(Igray1,Ilow1[num],Ihi1[num],Imask);//Igray1[]中相应的点在Ilow1[]和Ihi1[]之间时，Imask中相应的点为255(背景符合)
132 //Channel 2
133     cvInRange(Igray2,Ilow2[num],Ihi2[num],Imaskt);//也就是说对于每一幅图像的绝对值差小于绝对值差平均值的6倍或者大于绝对值差平均值的7倍被认为是前景图像
134     cvOr(Imask,Imaskt,Imask);
135     //Channel 3
136     cvInRange(Igray3,Ilow3[num],Ihi3[num],Imaskt);//这里的固定阈值6和7太不合理了，还好工程后面可以根据实际情况手动调整！
137     cvOr(Imask,Imaskt,Imask);
138     //Finally, invert the results
139     cvSubRS( Imask, cvScalar(255), Imask);//前景用255表示了，背景是用0表示
140 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)



 


 **二、codebook算法工作原理**


     考虑到简单背景减图法无法对动态的背景建模，有学者就提出了codebook算法。


     该算法为图像中每一个像素点建立一个码本，每个码本可以包括多个码元，每个码元有它的学习时最大最小阈值，检测时的最大最小阈值等成员。在背景建模期间，每当来了一幅新图片，对每个像素点进行码本匹配，也就是说如果该像素值在码本中某个码元的学习阈值内，则认为它离过去该对应点出现过的历史情况偏离不大，通过一定的像素值比较，如果满足条件，此时还可以更新对应点的学习阈值和检测阈值。如果新来的像素值对码本中每个码元都不匹配，则有可能是由于背景是动态的，所以我们需要为其建立一个新的码元，并且设置相应的码元成员变量。因此，在背景学习的过程中，每个像素点可以对应多个码元，这样就可以学到复杂的动态背景。


     关于codebook算法的代码和注释如下：


     ***cv_yuv_codebook.h文件：***


```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 2 // Accumulate average and ~std (really absolute difference) image and use this to detect background and foreground
 3 //
 4 // Typical way of using this is to:
 5 //     AllocateImages();
 6 ////loop for N images to accumulate background differences
 7 //    accumulateBackground();
 8 ////When done, turn this into our avg and std model with high and low bounds
 9 //    createModelsfromStats();
10 ////Then use the function to return background in a mask (255 == foreground, 0 == background)
11 //    backgroundDiff(IplImage *I,IplImage *Imask, int num);
12 ////Then tune the high and low difference from average image background acceptance thresholds
13 //    float scalehigh,scalelow; //Set these, defaults are 7 and 6. Note: scalelow is how many average differences below average
14 //    scaleHigh(scalehigh);
15 //    scaleLow(scalelow);
16 ////That is, change the scale high and low bounds for what should be background to make it work.
17 ////Then continue detecting foreground in the mask image
18 //    backgroundDiff(IplImage *I,IplImage *Imask, int num);
19 //
20 //NOTES: num is camera number which varies from 0 ... NUM_CAMERAS - 1.  Typically you only have one camera, but this routine allows
21 //          you to index many.
22 //
23 #ifndef AVGSEG_
24 #define AVGSEG_
25 
26 
27 #include "cv.h"                // define all of the opencv classes etc.
28 #include "highgui.h"
29 #include "cxcore.h"
30 
31 //IMPORTANT DEFINES:
32 #define NUM_CAMERAS   1              //This function can handle an array of cameras
33 #define HIGH_SCALE_NUM 7.0            //How many average differences from average image on the high side == background
34 #define LOW_SCALE_NUM 6.0        //How many average differences from average image on the low side == background
35 
36 void AllocateImages(IplImage *I);
37 void DeallocateImages();
38 void accumulateBackground(IplImage *I, int number=0);
39 void scaleHigh(float scale = HIGH_SCALE_NUM, int num = 0);
40 void scaleLow(float scale = LOW_SCALE_NUM, int num = 0);
41 void createModelsfromStats();
42 void backgroundDiff(IplImage *I,IplImage *Imask, int num = 0);
43 
44 #endif

![复制代码](http://common.cnblogs.com/images/copycode.gif)



 


     ***cv_yuv_codebook.cpp文件：***


```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

  1 ////////YUV CODEBOOK
  2 // Gary Bradski, July 14, 2005
  3 
  4 
  5 #include "stdafx.h"
  6 #include "cv_yuv_codebook.h"
  7 
  8 //GLOBALS FOR ALL CAMERA MODELS
  9 
 10 //For connected components:
 11 int CVCONTOUR_APPROX_LEVEL = 2;   // Approx.threshold - the bigger it is, the simpler is the boundary
 12 int CVCLOSE_ITR = 1;                // How many iterations of erosion and/or dialation there should be
 13 //#define CVPERIMSCALE 4            // image (width+height)/PERIMSCALE.  If contour lenght < this, delete that contour
 14 
 15 //For learning background
 16 
 17 //Just some convienience macros
 18 #define CV_CVX_WHITE    CV_RGB(0xff,0xff,0xff)
 19 #define CV_CVX_BLACK    CV_RGB(0x00,0x00,0x00)
 20 
 21 
 22 ///////////////////////////////////////////////////////////////////////////////////
 23 // int updateCodeBook(uchar *p, codeBook &c, unsigned cbBounds)
 24 // Updates the codebook entry with a new data point
 25 //
 26 // p            Pointer to a YUV pixel
 27 // c            Codebook for this pixel
 28 // cbBounds        Learning bounds for codebook (Rule of thumb: 10)
 29 // numChannels    Number of color channels we're learning
 30 //
 31 // NOTES:
 32 //        cvBounds must be of size cvBounds[numChannels]
 33 //
 34 // RETURN
 35 //    codebook index
 36 int cvupdateCodeBook(uchar *p, codeBook &c, unsigned *cbBounds, int numChannels)
 37 {
 38 
 39     if(c.numEntries == 0) c.t = 0;//说明每个像素如果遍历了的话至少对应一个码元
 40     c.t += 1;        //Record learning event，遍历该像素点的次数加1
 41 //SET HIGH AND LOW BOUNDS
 42     int n;
 43     unsigned int high[3],low[3];
 44     for(n=0; n<numChannels; n++)//为该像素点的每个通道设置最大阈值和最小阈值，后面用来更新学习的高低阈值时有用
 45     {
 46         high[n] = *(p+n)+*(cbBounds+n);
 47         if(high[n] > 255) high[n] = 255;
 48         low[n] = *(p+n)-*(cbBounds+n);
 49         if(low[n] < 0) low[n] = 0;
 50     }
 51     int matchChannel;
 52     //SEE IF THIS FITS AN EXISTING CODEWORD
 53     int i;
 54     for(i=0; i<c.numEntries; i++)//需要对所有的码元进行扫描
 55     {
 56         matchChannel = 0;
 57         for(n=0; n<numChannels; n++)
 58         {
 59             //这个地方要非常小心，if条件不是下面表达的
 60 //if((c.cb[i]->min[n]-c.cb[i]->learnLow[n] <= *(p+n)) && (*(p+n) <= c.cb[i]->max[n]+c.cb[i]->learnHigh[n]))
 61 //原因是因为在每次建立一个新码元的时候，learnHigh[n]和learnLow[n]的范围就在max[n]和min[n]上扩展了cbBounds[n]，所以说
 62 //learnHigh[n]和learnLow[n]的变化范围实际上比max[n]和min[n]的大
 63             if((c.cb[i]->learnLow[n] <= *(p+n)) && (*(p+n) <= c.cb[i]->learnHigh[n])) //Found an entry for this channel
 64             {
 65                 matchChannel++;
 66             }
 67         }
 68         if(matchChannel == numChannels) //If an entry was found over all channels，找到了该元素此刻对应的码元
 69         {
 70             c.cb[i]->t_last_update = c.t;
 71             //adjust this codeword for the first channel
 72 //更新每个码元的最大最小阈值，因为这2个阈值在后面的前景分离过程要用到
 73             for(n=0; n<numChannels; n++)
 74             {
 75                 if(c.cb[i]->max[n] < *(p+n))//用该点的像素值更新该码元的最大值，所以max[n]保存的是实际上历史出现过的最大像素值
 76                 {
 77                     c.cb[i]->max[n] = *(p+n);//因为这个for语句是在匹配成功了的条件阈值下的，所以一般来说改变后的max[n]和min[n]
 78 //也不会过学习的高低阈值，并且学习的高低阈值也一直在缓慢变化  
 79                 }
 80                 else if(c.cb[i]->min[n] > *(p+n))//用该点的像素值更新该码元的最小值，所以min[n]保存的是实际上历史出现过的最小像素值
 81                 {
 82                     c.cb[i]->min[n] = *(p+n);
 83                 }
 84             }
 85             break;//一旦找到了该像素的一个码元后就不用继续往后找了，加快算法速度。因为最多只有一个码元与之对应
 86         }
 87     }
 88 
 89     //OVERHEAD TO TRACK POTENTIAL STALE ENTRIES
 90     for(int s=0; s<c.numEntries; s++)
 91     {
 92         //This garbage is to track which codebook entries are going stale
 93         int negRun = c.t - c.cb[s]->t_last_update;//negRun表示码元没有更新的时间间隔
 94         if(c.cb[s]->stale < negRun) c.cb[s]->stale = negRun;//更新每个码元的statle
 95     }
 96 
 97 
 98     //ENTER A NEW CODE WORD IF NEEDED
 99     if(i == c.numEntries)  //No existing code word found, make a new one，只有当该像素码本中的所有码元都不符合要求时才满足if条件
100     {
101         code_element **foo = new code_element* [c.numEntries+1];//创建一个新的码元序列
102         for(int ii=0; ii<c.numEntries; ii++)
103         {
104             foo[ii] = c.cb[ii];//将码本前面所有的码元地址赋给foo
105         }
106         foo[c.numEntries] = new code_element;//创建一个新码元并赋给foo指针的下一个空位
107         if(c.numEntries) delete [] c.cb;//？
108         c.cb = foo;
109         for(n=0; n<numChannels; n++)//给新建立的码元结构体元素赋值
110         {
111             c.cb[c.numEntries]->learnHigh[n] = high[n];//当建立一个新码元时，用当前值附近cbBounds范围作为码元box的学习阈值
112             c.cb[c.numEntries]->learnLow[n] = low[n];
113             c.cb[c.numEntries]->max[n] = *(p+n);//当建立一个新码元时，用当前值作为码元box的最大最小边界值
114             c.cb[c.numEntries]->min[n] = *(p+n);
115         }
116         c.cb[c.numEntries]->t_last_update = c.t;
117         c.cb[c.numEntries]->stale = 0;//因为刚建立，所有为0
118         c.numEntries += 1;//码元的个数加1
119     }
120 
121     //SLOWLY ADJUST LEARNING BOUNDS
122     for(n=0; n<numChannels; n++)//每次遍历该像素点就将每个码元的学习最大阈值变大，最小阈值变小，但是都是缓慢变化的
123     {                           //如果是新建立的码元，则if条件肯定不满足
124         if(c.cb[i]->learnHigh[n] < high[n]) c.cb[i]->learnHigh[n] += 1;                
125         if(c.cb[i]->learnLow[n] > low[n]) c.cb[i]->learnLow[n] -= 1;
126     }
127 
128     return(i);//返回所找到码本中码元的索引
129 }
130 
131 ///////////////////////////////////////////////////////////////////////////////////
132 // uchar cvbackgroundDiff(uchar *p, codeBook &c, int minMod, int maxMod)
133 // Given a pixel and a code book, determine if the pixel is covered by the codebook
134 //
135 // p        pixel pointer (YUV interleaved)
136 // c        codebook reference
137 // numChannels  Number of channels we are testing
138 // maxMod    Add this (possibly negative) number onto max level when code_element determining if new pixel is foreground
139 // minMod    Subract this (possible negative) number from min level code_element when determining if pixel is foreground
140 //
141 // NOTES:
142 // minMod and maxMod must have length numChannels, e.g. 3 channels => minMod[3], maxMod[3].
143 //
144 // Return
145 // 0 => background, 255 => foreground
146 uchar cvbackgroundDiff(uchar *p, codeBook &c, int numChannels, int *minMod, int *maxMod)
147 {
148     int matchChannel;
149     //SEE IF THIS FITS AN EXISTING CODEWORD
150     int i;
151     for(i=0; i<c.numEntries; i++)
152     {
153         matchChannel = 0;
154         for(int n=0; n<numChannels; n++)
155         {
156             if((c.cb[i]->min[n] - minMod[n] <= *(p+n)) && (*(p+n) <= c.cb[i]->max[n] + maxMod[n]))
157             {
158                 matchChannel++; //Found an entry for this channel
159             }
160             else
161             {
162                 break;//加快速度，当一个通道不满足时提前结束
163             }
164         }
165         if(matchChannel == numChannels)
166         {
167             break; //Found an entry that matched all channels，加快速度，当一个码元找到时，提前结束
168         }
169     }
170     if(i >= c.numEntries) return(255);//255代表前景，因为所有的码元都不满足条件
171     return(0);//0代表背景，因为至少有一个码元满足条件
172 }
173 
174 
175 //UTILITES/////////////////////////////////////////////////////////////////////////////////////
176 /////////////////////////////////////////////////////////////////////////////////
177 //int clearStaleEntries(codeBook &c)
178 // After you've learned for some period of time, periodically call this to clear out stale codebook entries
179 //
180 //c        Codebook to clean up
181 //
182 // Return
183 // number of entries cleared
184 int cvclearStaleEntries(codeBook &c)//对每一个码本进行检查
185 {
186     int staleThresh = c.t>>1;//阈值设置为访问该码元的次数的一半，经验值
187     int *keep = new int [c.numEntries];
188     int keepCnt = 0;
189     //SEE WHICH CODEBOOK ENTRIES ARE TOO STALE
190     for(int i=0; i<c.numEntries; i++)
191     {
192         if(c.cb[i]->stale > staleThresh)//当在背景建模期间有一半的时间内，codebook的码元条目没有被访问，则该条目将被删除
193             keep[i] = 0; //Mark for destruction
194         else
195         {
196             keep[i] = 1; //Mark to keep，为1时，该码本的条目将被保留
197             keepCnt += 1;//keepCnt记录了要保持的codebook的数目
198         }
199     }
200     //KEEP ONLY THE GOOD
201     c.t = 0;                        //Full reset on stale tracking
202     code_element **foo = new code_element* [keepCnt];//重新建立一个码本的双指针
203     int k=0;
204     for(int ii=0; ii<c.numEntries; ii++)
205     {
206         if(keep[ii])
207         {
208             foo[k] = c.cb[ii];//要保持该码元的话就要把码元结构体复制到fook
209             foo[k]->stale = 0;        //We have to refresh these entries for next clearStale，不被访问的累加器stale重新赋值0
210             foo[k]->t_last_update = 0;//
211             k++;
212         }
213     }
214     //CLEAN UP
215     delete [] keep;
216     delete [] c.cb;
217     c.cb = foo;
218     int numCleared = c.numEntries - keepCnt;//numCleared中保存的是被删除码元的个数
219     c.numEntries = keepCnt;//最后新的码元数为保存下来码元的个数
220     return(numCleared);//返回被删除的码元个数
221 }
222 
223 /////////////////////////////////////////////////////////////////////////////////
224 //int countSegmentation(codeBook *c, IplImage *I)
225 //
226 //Count how many pixels are detected as foreground
227 // c    Codebook
228 // I    Image (yuv, 24 bits)
229 // numChannels  Number of channels we are testing
230 // maxMod    Add this (possibly negative) number onto max level when code_element determining if new pixel is foreground
231 // minMod    Subract this (possible negative) number from min level code_element when determining if pixel is foreground
232 //
233 // NOTES:
234 // minMod and maxMod must have length numChannels, e.g. 3 channels => minMod[3], maxMod[3].
235 //
236 //Return
237 // Count of fg pixels
238 //
239 int cvcountSegmentation(codeBook *c, IplImage *I, int numChannels, int *minMod, int *maxMod)
240 {
241     int count = 0,i;
242     uchar *pColor;
243     int imageLen = I->width * I->height;
244 
245     //GET BASELINE NUMBER OF FG PIXELS FOR Iraw
246     pColor = (uchar *)((I)->imageData);
247     for(i=0; i<imageLen; i++)
248     {
249         if(cvbackgroundDiff(pColor, c[i], numChannels, minMod, maxMod))//对每一个像素点都要检测其是否为前景，如果是的话，计数器count就加1
250             count++;
251         pColor += 3;
252     }
253     return(count);//返回图像I的前景像素点的个数
254 }
255 
256 
257 ///////////////////////////////////////////////////////////////////////////////////////////
258 //void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int *num, CvRect *bbs, CvPoint *centers)
259 // This cleans up the forground segmentation mask derived from calls to cvbackgroundDiff
260 //
261 // mask            Is a grayscale (8 bit depth) "raw" mask image which will be cleaned up
262 //
263 // OPTIONAL PARAMETERS:
264 // poly1_hull0    If set, approximate connected component by (DEFAULT) polygon, or else convex hull (0)
265 // perimScale     Len = image (width+height)/perimScale.  If contour len < this, delete that contour (DEFAULT: 4)
266 // num            Maximum number of rectangles and/or centers to return, on return, will contain number filled (DEFAULT: NULL)
267 // bbs            Pointer to bounding box rectangle vector of length num.  (DEFAULT SETTING: NULL)
268 // centers        Pointer to contour centers vectore of length num (DEFULT: NULL)
269 //
270 void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int *num, CvRect *bbs, CvPoint *centers)
271 {
272 static CvMemStorage*    mem_storage    = NULL;
273 static CvSeq*            contours    = NULL;
274 //CLEAN UP RAW MASK
275 //开运算作用：平滑轮廓，去掉细节,断开缺口
276     cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_OPEN, CVCLOSE_ITR );//对输入mask进行开操作，CVCLOSE_ITR为开操作的次数，输出为mask图像
277 //闭运算作用：平滑轮廓，连接缺口
278     cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_CLOSE, CVCLOSE_ITR );//对输入mask进行闭操作，CVCLOSE_ITR为闭操作的次数，输出为mask图像
279 
280 //FIND CONTOURS AROUND ONLY BIGGER REGIONS
281     if( mem_storage==NULL ) mem_storage = cvCreateMemStorage(0);
282     else cvClearMemStorage(mem_storage);
283 
284     //CV_RETR_EXTERNAL=0是在types_c.h中定义的，CV_CHAIN_APPROX_SIMPLE=2也是在该文件中定义的
285     CvContourScanner scanner = cvStartFindContours(mask,mem_storage,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
286     CvSeq* c;
287     int numCont = 0;
288     while( (c = cvFindNextContour( scanner )) != NULL )
289     {
290         double len = cvContourPerimeter( c );
291         double q = (mask->height + mask->width) /perimScale;   //calculate perimeter len threshold
292         if( len < q ) //Get rid of blob if it's perimeter is too small
293         {
294             cvSubstituteContour( scanner, NULL );
295         }
296         else //Smooth it's edges if it's large enough
297         {
298             CvSeq* c_new;
299             if(poly1_hull0) //Polygonal approximation of the segmentation
300                 c_new = cvApproxPoly(c,sizeof(CvContour),mem_storage,CV_POLY_APPROX_DP, CVCONTOUR_APPROX_LEVEL,0);
301             else //Convex Hull of the segmentation
302                 c_new = cvConvexHull2(c,mem_storage,CV_CLOCKWISE,1);
303             cvSubstituteContour( scanner, c_new );
304             numCont++;
305         }
306     }
307     contours = cvEndFindContours( &scanner );
308 
309 // PAINT THE FOUND REGIONS BACK INTO THE IMAGE
310     cvZero( mask );
311     IplImage *maskTemp;
312     //CALC CENTER OF MASS AND OR BOUNDING RECTANGLES
313     if(num != NULL)
314     {
315         int N = *num, numFilled = 0, i=0;
316         CvMoments moments;
317         double M00, M01, M10;
318         maskTemp = cvCloneImage(mask);
319         for(i=0, c=contours; c != NULL; c = c->h_next,i++ )
320         {
321             if(i < N) //Only process up to *num of them
322             {
323                 cvDrawContours(maskTemp,c,CV_CVX_WHITE, CV_CVX_WHITE,-1,CV_FILLED,8);
324                 //Find the center of each contour
325                 if(centers != NULL)
326                 {
327                     cvMoments(maskTemp,&moments,1);
328                     M00 = cvGetSpatialMoment(&moments,0,0);
329                     M10 = cvGetSpatialMoment(&moments,1,0);
330                     M01 = cvGetSpatialMoment(&moments,0,1);
331                     centers[i].x = (int)(M10/M00);
332                     centers[i].y = (int)(M01/M00);
333                 }
334                 //Bounding rectangles around blobs
335                 if(bbs != NULL)
336                 {
337                     bbs[i] = cvBoundingRect(c);
338                 }
339                 cvZero(maskTemp);
340                 numFilled++;
341             }
342             //Draw filled contours into mask
343             cvDrawContours(mask,c,CV_CVX_WHITE,CV_CVX_WHITE,-1,CV_FILLED,8); //draw to central mask
344         } //end looping over contours
345         *num = numFilled;
346         cvReleaseImage( &maskTemp);
347     }
348     //ELSE JUST DRAW PROCESSED CONTOURS INTO THE MASK
349     else
350     {
351         for( c=contours; c != NULL; c = c->h_next )
352         {
353             cvDrawContours(mask,c,CV_CVX_WHITE, CV_CVX_BLACK,-1,CV_FILLED,8);
354         }
355     }
356 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)



 


**三、2种算法进行对比。**


     Learning Opencv的作者将这两种算法做了下对比，用的视频是有风吹动树枝的动态背景，一段时间过后的前景是视频中移动的手。


     当然在这个工程中，作者除了体现上述简单背景差法和codobook算法的一些原理外，还引入了很多细节来优化前景分割效果。比如说误差计算时的方差和协方差计算加速方法，消除像素点内长时间没有被访问过的码元，对检测到的粗糙原始前景图用连通域分析法清楚噪声，其中引入了形态学中的几种操作，使用多边形拟合前景轮廓等细节处理。


     在看作者代码前，最好先看下下面几个变量的物理含义。


     maxMod[n]：用训练好的背景模型进行前景检测时用到，判断点是否小于max[n] + maxMod[n])。


     minMod[n]：用训练好的背景模型进行前景检测时用到，判断点是否小于min[n] -minMod[n])。


     cbBounds*：训练背景模型时用到，可以手动输入该参数，这个数主要是配合high[n]和low[n]来用的。


     learnHigh[n]：背景学习过程中当一个新像素来时用来判断是否在已有的码元中，是阈值的上界部分。


     learnLow[n]：背景学习过程中当一个新像素来时用来判断是否在已有的码元中，是阈值的下界部分。


     max[n]： 背景学习过程中每个码元学习到的最大值，在前景分割时配合maxMod[n]用的。


     min[n]： 背景学习过程中每个码元学习到的最小值，在前景分割时配合minMod[n]用的。


     high[n]：背景学习过程中用来调整learnHigh[n]的，如果learnHigh[n]<high[n],则learnHigh[n]缓慢加1


     low[n]： 背景学习过程中用来调整learnLow[n]的，如果learnLow[n]>Low[n],则learnLow[缓慢减1


     ***该工程带主函数部分代码和注释如下：***


```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

#include "stdafx.h"

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "avg_background.h"
#include "cv_yuv_codebook.h"

//VARIABLES for CODEBOOK METHOD:
codeBook *cB;   //This will be our linear model of the image, a vector 
                //of lengh = height*width
int maxMod[CHANNELS];    //Add these (possibly negative) number onto max 
                        // level when code_element determining if new pixel is foreground
int minMod[CHANNELS];     //Subract these (possible negative) number from min 
                        //level code_element when determining if pixel is foreground
unsigned cbBounds[CHANNELS]; //Code Book bounds for learning
bool ch[CHANNELS];        //This sets what channels should be adjusted for background bounds
int nChannels = CHANNELS;
int imageLen = 0;
uchar *pColor; //YUV pointer

void help() {
    printf("\nLearn background and find foreground using simple average and average difference learning method:\n"
        "\nUSAGE:\n  ch9_background startFrameCollection# endFrameCollection# [movie filename, else from camera]\n"
        "If from AVI, then optionally add HighAvg, LowAvg, HighCB_Y LowCB_Y HighCB_U LowCB_U HighCB_V LowCB_V\n\n"
        "***Keep the focus on the video windows, NOT the consol***\n\n"
        "INTERACTIVE PARAMETERS:\n"
        "\tESC,q,Q  - quit the program\n"
        "\th    - print this help\n"
        "\tp    - pause toggle\n"
        "\ts    - single step\n"
        "\tr    - run mode (single step off)\n"
        "=== AVG PARAMS ===\n"
        "\t-    - bump high threshold UP by 0.25\n"
        "\t=    - bump high threshold DOWN by 0.25\n"
        "\t[    - bump low threshold UP by 0.25\n"
        "\t]    - bump low threshold DOWN by 0.25\n"
        "=== CODEBOOK PARAMS ===\n"
        "\ty,u,v- only adjust channel 0(y) or 1(u) or 2(v) respectively\n"
        "\ta    - adjust all 3 channels at once\n"
        "\tb    - adjust both 2 and 3 at once\n"
        "\ti,o    - bump upper threshold up,down by 1\n"
        "\tk,l    - bump lower threshold up,down by 1\n"
        );
}

//
//USAGE:  ch9_background startFrameCollection# endFrameCollection# [movie filename, else from camera]
//If from AVI, then optionally add HighAvg, LowAvg, HighCB_Y LowCB_Y HighCB_U LowCB_U HighCB_V LowCB_V
//
int main(int argc, char** argv)
{
     IplImage* rawImage = 0, *yuvImage = 0; //yuvImage is for codebook method
    IplImage *ImaskAVG = 0,*ImaskAVGCC = 0;
    IplImage *ImaskCodeBook = 0,*ImaskCodeBookCC = 0;
    CvCapture* capture = 0;

    int startcapture = 1;
    int endcapture = 30;
    int c,n;

    maxMod[0] = 3;  //Set color thresholds to default values
    minMod[0] = 10;
    maxMod[1] = 1;
    minMod[1] = 1;
    maxMod[2] = 1;
    minMod[2] = 1;
    float scalehigh = HIGH_SCALE_NUM;//默认值为6
    float scalelow = LOW_SCALE_NUM;//默认值为7
    
    if(argc < 3) {//只有1个参数或者没有参数时，输出错误，并提示help信息，因为该程序本身就算进去了一个参数
        printf("ERROR: Too few parameters\n");
        help();
    }else{//至少有2个参数才算正确
        if(argc == 3){//输入为2个参数的情形是从摄像头输入数据
            printf("Capture from Camera\n");
            capture = cvCaptureFromCAM( 0 );
        }
        else {//输入大于2个参数时是从文件中读入视频数据
            printf("Capture from file %s\n",argv[3]);//第三个参数是读入视频文件的文件名
    //        capture = cvCaptureFromFile( argv[3] );
            capture = cvCreateFileCapture( argv[3] );
            if(!capture) { printf("Couldn't open %s\n",argv[3]); return -1;}//读入视频文件失败
        }
        if(isdigit(argv[1][0])) { //Start from of background capture
            startcapture = atoi(argv[1]);//第一个参数表示视频开始的背景训练时的帧，默认是1
            printf("startcapture = %d\n",startcapture);
        }
        if(isdigit(argv[2][0])) { //End frame of background capture
            endcapture = atoi(argv[2]);//第二个参数表示的结束背景训练时的，默认为30
            printf("endcapture = %d\n"); 
        }
        if(argc > 4){ //See if parameters are set from command line，输入多于4个参数表示后面的算法中用到的参数在这里直接输入
            //FOR AVG MODEL
            if(argc >= 5){
                if(isdigit(argv[4][0])){
                    scalehigh = (float)atoi(argv[4]);
                }
            }
            if(argc >= 6){
                if(isdigit(argv[5][0])){
                    scalelow = (float)atoi(argv[5]);
                }
            }
            //FOR CODEBOOK MODEL, CHANNEL 0
            if(argc >= 7){
                if(isdigit(argv[6][0])){
                    maxMod[0] = atoi(argv[6]);
                }
            }
            if(argc >= 8){
                if(isdigit(argv[7][0])){
                    minMod[0] = atoi(argv[7]);
                }
            }
            //Channel 1
            if(argc >= 9){
                if(isdigit(argv[8][0])){
                    maxMod[1] = atoi(argv[8]);
                }
            }
            if(argc >= 10){
                if(isdigit(argv[9][0])){
                    minMod[1] = atoi(argv[9]);
                }
            }
            //Channel 2
            if(argc >= 11){
                if(isdigit(argv[10][0])){
                    maxMod[2] = atoi(argv[10]);
                }
            }
            if(argc >= 12){
                if(isdigit(argv[11][0])){
                    minMod[2] = atoi(argv[11]);
                }
            }
        }
    }

    //MAIN PROCESSING LOOP:
    bool pause = false;
    bool singlestep = false;

    if( capture )
    {
      cvNamedWindow( "Raw", 1 );//原始视频图像
        cvNamedWindow( "AVG_ConnectComp",1);//平均法连通区域分析后的图像
        cvNamedWindow( "ForegroundCodeBook",1);//codebook法后图像
        cvNamedWindow( "CodeBook_ConnectComp",1);//codebook法连通区域分析后的图像
         cvNamedWindow( "ForegroundAVG",1);//平均法后图像
        int i = -1;
        
        for(;;)
        {
                if(!pause){
//                if( !cvGrabFrame( capture ))
//                    break;
//                rawImage = cvRetrieveFrame( capture );
                rawImage = cvQueryFrame( capture );
                ++i;//count it
//                printf("%d\n",i);
                if(!rawImage) 
                    break;
                //REMOVE THIS FOR GENERAL OPERATION, JUST A CONVIENIENCE WHEN RUNNING WITH THE SMALL tree.avi file
                if(i == 56){//程序开始运行几十帧后自动暂停，以便后面好手动调整参数
                    pause = 1;
                    printf("\n\nVideo paused for your convienience at frame 50 to work with demo\n"
                    "You may adjust parameters, single step or continue running\n\n");
                    help();
                }
            }
            if(singlestep){
                pause = true;
            }
            //First time:
            if(0 == i) {
                printf("\n . . . wait for it . . .\n"); //Just in case you wonder why the image is white at first
                //AVG METHOD ALLOCATION
                AllocateImages(rawImage);//为算法的使用分配内存
                scaleHigh(scalehigh);//设定背景建模时的高阈值函数
                scaleLow(scalelow);//设定背景建模时的低阈值函数
                ImaskAVG = cvCreateImage( cvGetSize(rawImage), IPL_DEPTH_8U, 1 );
                ImaskAVGCC = cvCreateImage( cvGetSize(rawImage), IPL_DEPTH_8U, 1 );
                cvSet(ImaskAVG,cvScalar(255));
                //CODEBOOK METHOD ALLOCATION:
                yuvImage = cvCloneImage(rawImage);
                ImaskCodeBook = cvCreateImage( cvGetSize(rawImage), IPL_DEPTH_8U, 1 );//用来装前景背景图的，当然只要一个通道的图像即可
                ImaskCodeBookCC = cvCreateImage( cvGetSize(rawImage), IPL_DEPTH_8U, 1 );
                cvSet(ImaskCodeBook,cvScalar(255));
                imageLen = rawImage->width*rawImage->height;
                cB = new codeBook [imageLen];//创建一个码本cB数组，每个像素对应一个码本
                for(int f = 0; f<imageLen; f++)
                {
                     cB[f].numEntries = 0;//每个码本的初始码元个数赋值为0
                }
                for(int nc=0; nc<nChannels;nc++)
                {
                    cbBounds[nc] = 10; //Learning bounds factor，初始值为10
                }
                ch[0] = true; //Allow threshold setting simultaneously for all channels
                ch[1] = true;
                ch[2] = true;
            }
            //If we've got an rawImage and are good to go:                
            if( rawImage )
            {
                cvCvtColor( rawImage, yuvImage, CV_BGR2YCrCb );//YUV For codebook method
                //This is where we build our background model
                if( !pause && i >= startcapture && i < endcapture  ){
                    //LEARNING THE AVERAGE AND AVG DIFF BACKGROUND
                    accumulateBackground(rawImage);//平均法累加过程
                    //LEARNING THE CODEBOOK BACKGROUND
                    pColor = (uchar *)((yuvImage)->imageData);//yuvImage矩阵的首位置
                    for(int c=0; c<imageLen; c++)
                    {
                        cvupdateCodeBook(pColor, cB[c], cbBounds, nChannels);//codebook算法建模过程
                        pColor += 3;
                    }
                }
                //When done, create the background model
                if(i == endcapture){
                    createModelsfromStats();//平均法建模过程
                }
                //Find the foreground if any
                if(i >= endcapture) {//endcapture帧后开始检测前景
                    //FIND FOREGROUND BY AVG METHOD:
                    backgroundDiff(rawImage,ImaskAVG);
                    cvCopy(ImaskAVG,ImaskAVGCC);
                    cvconnectedComponents(ImaskAVGCC);//平均法中的前景清除
                    //FIND FOREGROUND BY CODEBOOK METHOD
                    uchar maskPixelCodeBook;
                    pColor = (uchar *)((yuvImage)->imageData); //3 channel yuv image
                    uchar *pMask = (uchar *)((ImaskCodeBook)->imageData); //1 channel image
                    for(int c=0; c<imageLen; c++)
                    {
                         maskPixelCodeBook = cvbackgroundDiff(pColor, cB[c], nChannels, minMod, maxMod);//前景返回255，背景返回0
                        *pMask++ = maskPixelCodeBook;//将前景检测的结果返回到ImaskCodeBook中
                        pColor += 3;
                    }
                    //This part just to visualize bounding boxes and centers if desired
                    cvCopy(ImaskCodeBook,ImaskCodeBookCC);    
                    cvconnectedComponents(ImaskCodeBookCC);//codebook算法中的前景清除
                }
                //Display
                   cvShowImage( "Raw", rawImage );//除了这张是彩色图外，另外4张都是黑白图
                cvShowImage( "AVG_ConnectComp",ImaskAVGCC);
                   cvShowImage( "ForegroundAVG",ImaskAVG);
                 cvShowImage( "ForegroundCodeBook",ImaskCodeBook);
                 cvShowImage( "CodeBook_ConnectComp",ImaskCodeBookCC);

                //USER INPUT:
                 c = cvWaitKey(10)&0xFF;
                //End processing on ESC, q or Q
                if(c == 27 || c == 'q' | c == 'Q')
                    break;
                //Else check for user input
                switch(c)
                {
                    case 'h':
                        help();
                        break;
                    case 'p':
                        pause ^= 1;
                        break;
                    case 's':
                        singlestep = 1;
                        pause = false;
                        break;
                    case 'r':
                        pause = false;
                        singlestep = false;
                        break;
                    //AVG BACKROUND PARAMS
                    case '-'://调整scalehigh的参数，scalehigh的物理意义是误差累加的影响因子，其倒数为缩放倍数，加0.25实际上是减小其影响力
                        if(i > endcapture){
                            scalehigh += 0.25;
                            printf("AVG scalehigh=%f\n",scalehigh);
                            scaleHigh(scalehigh);
                        }
                        break;
                    case '='://scalehigh减少2.5是增加其影响力
                        if(i > endcapture){
                            scalehigh -= 0.25;
                            printf("AVG scalehigh=%f\n",scalehigh);
                            scaleHigh(scalehigh);
                        }
                        break;
                    case '[':
                        if(i > endcapture){//设置设定背景建模时的低阈值函数，同上
                            scalelow += 0.25;
                            printf("AVG scalelow=%f\n",scalelow);
                            scaleLow(scalelow);
                        }
                        break;
                    case ']':
                        if(i > endcapture){
                            scalelow -= 0.25;
                            printf("AVG scalelow=%f\n",scalelow);
                            scaleLow(scalelow);
                        }
                        break;
                //CODEBOOK PARAMS
                case 'y':
                case '0'://激活y通道
                        ch[0] = 1;
                        ch[1] = 0;
                        ch[2] = 0;
                        printf("CodeBook YUV Channels active: ");
                        for(n=0; n<nChannels; n++)
                                printf("%d, ",ch[n]);
                        printf("\n");
                        break;
                case 'u':
                case '1'://激活u通道
                        ch[0] = 0;
                        ch[1] = 1;
                        ch[2] = 0;
                        printf("CodeBook YUV Channels active: ");
                        for(n=0; n<nChannels; n++)
                                printf("%d, ",ch[n]);
                        printf("\n");
                        break;
                case 'v':
                case '2'://激活v通道
                        ch[0] = 0;
                        ch[1] = 0;
                        ch[2] = 1;
                        printf("CodeBook YUV Channels active: ");
                        for(n=0; n<nChannels; n++)
                                printf("%d, ",ch[n]);
                        printf("\n");
                        break;
                case 'a': //All
                case '3'://激活所有通道
                        ch[0] = 1;
                        ch[1] = 1;
                        ch[2] = 1;
                        printf("CodeBook YUV Channels active: ");
                        for(n=0; n<nChannels; n++)
                                printf("%d, ",ch[n]);
                        printf("\n");
                        break;
                case 'b':  //both u and v together
                        ch[0] = 0;
                        ch[1] = 1;
                        ch[2] = 1;
                        printf("CodeBook YUV Channels active: ");
                        for(n=0; n<nChannels; n++)
                                printf("%d, ",ch[n]);
                        printf("\n");
                        break;
                case 'i': //modify max classification bounds (max bound goes higher)
                    for(n=0; n<nChannels; n++){//maxMod和minMod是最大值和最小值跳动的阈值
                        if(ch[n])
                            maxMod[n] += 1;
                        printf("%.4d,",maxMod[n]);
                    }
                    printf(" CodeBook High Side\n");
                    break;
                case 'o': //modify max classification bounds (max bound goes lower)
                    for(n=0; n<nChannels; n++){
                        if(ch[n])
                            maxMod[n] -= 1;
                        printf("%.4d,",maxMod[n]);
                    }
                    printf(" CodeBook High Side\n");
                    break;
                case 'k': //modify min classification bounds (min bound goes lower)
                    for(n=0; n<nChannels; n++){
                        if(ch[n])
                            minMod[n] += 1;
                        printf("%.4d,",minMod[n]);
                    }
                    printf(" CodeBook Low Side\n");
                    break;
                case 'l': //modify min classification bounds (min bound goes higher)
                    for(n=0; n<nChannels; n++){
                        if(ch[n])
                            minMod[n] -= 1;
                        printf("%.4d,",minMod[n]);
                    }
                    printf(" CodeBook Low Side\n");
                    break;
                }
                
            }
        }        
      cvReleaseCapture( &capture );
      cvDestroyWindow( "Raw" );
        cvDestroyWindow( "ForegroundAVG" );
        cvDestroyWindow( "AVG_ConnectComp");
        cvDestroyWindow( "ForegroundCodeBook");
        cvDestroyWindow( "CodeBook_ConnectComp");
        DeallocateImages();//释放平均法背景建模过程中用到的内存
        if(yuvImage) cvReleaseImage(&yuvImage);
        if(ImaskAVG) cvReleaseImage(&ImaskAVG);
        if(ImaskAVGCC) cvReleaseImage(&ImaskAVGCC);
        if(ImaskCodeBook) cvReleaseImage(&ImaskCodeBook);
        if(ImaskCodeBookCC) cvReleaseImage(&ImaskCodeBookCC);
        delete [] cB;
    }
    else{ printf("\n\nDarn, Something wrong with the parameters\n\n"); help();
    }
    return 0;
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)



 


     运行结果截图如下：


     训练过程视频原图截图：


     ![](http://pic002.cnblogs.com/images/2012/381513/2012040822322941.png)


 


     测试过程视频原图截图：


     ![](http://pic002.cnblogs.com/images/2012/381513/2012040822325448.png)


 


     前景检测过程截图：


     ![](http://pic002.cnblogs.com/images/2012/381513/2012040822331544.png)


 


     可以看到左边2幅截图的对比，codebook算法的效果明显比简单减图法要好，手型比较清晰些。


 


** 四、参考文献**


      Bradski, G. and A. Kaehler (2008). Learning OpenCV: Computer vision with the OpenCV library, O'Reilly Media.
```
```
```
```
```




