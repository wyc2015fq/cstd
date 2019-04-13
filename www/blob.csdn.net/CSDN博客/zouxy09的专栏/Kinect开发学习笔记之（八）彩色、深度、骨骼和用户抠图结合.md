
# Kinect开发学习笔记之（八）彩色、深度、骨骼和用户抠图结合 - zouxy09的专栏 - CSDN博客


2012年11月08日 17:30:43[zouxy09](https://me.csdn.net/zouxy09)阅读数：20551个人分类：[Kinect																](https://blog.csdn.net/zouxy09/article/category/1273380)[OpenCV																](https://blog.csdn.net/zouxy09/article/category/1218759)[计算机视觉																](https://blog.csdn.net/zouxy09/article/category/1218765)[
							](https://blog.csdn.net/zouxy09/article/category/1218759)
[
																								](https://blog.csdn.net/zouxy09/article/category/1273380)


**Kinect开发学习笔记之（八）彩色、深度、骨骼和用户抠图结合**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
我的Kinect开发平台是：
Win7 x86 + VS2010 + Kinect for Windows SDK v1.6 + OpenCV2.3.0
开发环境的搭建见上一文：
http://blog.csdn.net/zouxy09/article/details/8146055
本学习笔记以下面的方式组织：编程前期分析、代码与注释和重要代码解析三部分。
**要实现目标：**整合前面所说的彩色图像、深度图像和骨骼图像，就是实现和SDK自带的那个SkeletonView例程差不多的功能。另外，再增加用户抠图功能，这个功能和SDK自带的那个Green Screen功能差不多。我们通过OpenCV来显示。
**一、编程前期分析**
这里因为是整合前面几个学习笔记提到的功能，所以要了解的东西前面都已经了解了，这里就不赘述了。这里唯一有一个新的点就是用户抠图，我们再学习学习。
那么叫抠图啊？用过Photoshop或者AE等一般的图像或者视频处理软件的人应该不会陌生。所谓抠图，就是把图片或影像的某一部分从原始图片或影像中分离出来成为单独的图层。主要功能是为了后期的合成做准备。如果还不了解的话，我们看过科幻电影或者一个特效视频吧？某个人在恐龙旁边、某个人在悬崖边打架、某个人在天上飞等等，这些场景大家有没有想过它是怎样实现的啊？难道真的是实景拍的？你都会说了“怎么可能！”。实际上很大一部分就是采用抠图这个技术的，如果搞视频特效处理的话，这个技术行业内好像叫“键控”。它在电视制作和电影制作中很常见，这种技术叫做绿屏抠像，就是演员或者播音员站在绿色底板前，然后录完节目后，绿色背景抠出，换成其他场景，在一些科幻电影中演员不可能在实景中表演时常采用的造景手法。
在Kinect中我们也可以达到类似的效果。Kinect SDK使得这个很容易实现。这实际上就是现实增强的一个基本例子，现实增强应用非常有趣而且能够获得非常好的用户体验。许多艺术家使用Kinect来进行现实增强交互展览。另外，这种技术也通常作为广告和营销的工具。
**二、代码与注释**
```cpp
#include <windows.h>
#include <iostream> 
#include <NuiApi.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
bool tracked[NUI_SKELETON_COUNT]={FALSE}; 
CvPoint skeletonPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)}; 
CvPoint colorPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)}; 
 
void getColorImage(HANDLE &colorEvent, HANDLE &colorStreamHandle, Mat &colorImage); 
void getDepthImage(HANDLE &depthEvent, HANDLE &depthStreamHandle, Mat &depthImage); 
void getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage, Mat &colorImage, Mat &depthImage); 
void drawSkeleton(Mat &image, CvPoint pointSet[], int witchone); 
void getTheContour(Mat &image, int whichone, Mat &mask);
int main(int argc, char *argv[])
{
	Mat colorImage;
	colorImage.create(480, 640, CV_8UC3); 
	Mat depthImage;
	depthImage.create(240, 320, CV_8UC3); 
	Mat skeletonImage;
	skeletonImage.create(240, 320, CV_8UC3); 
	Mat mask;
	mask.create(240, 320, CV_8UC3); 
 
    HANDLE colorEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 
    HANDLE depthEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 
    HANDLE skeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 
 
    HANDLE colorStreamHandle = NULL; 
    HANDLE depthStreamHandle = NULL; 
 
    HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);   
    if( hr != S_OK )   
    {   
        cout<<"NuiInitialize failed"<<endl;   
        return hr;   
    } 
 
    hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, NULL, 4, colorEvent, &colorStreamHandle); 
    if( hr != S_OK )   
    {   
        cout<<"Open the color Stream failed"<<endl; 
        NuiShutdown(); 
        return hr;   
    } 
    hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_320x240, NULL, 2, depthEvent, &depthStreamHandle); 
    if( hr != S_OK )   
    {   
        cout<<"Open the depth Stream failed"<<endl; 
        NuiShutdown(); 
        return hr;   
    } 
    hr = NuiSkeletonTrackingEnable( skeletonEvent, 0 );//打开骨骼跟踪事件   
    if( hr != S_OK )   
    {   
        cout << "NuiSkeletonTrackingEnable failed" << endl;   
        NuiShutdown(); 
        return hr;   
    } 
  
    namedWindow("mask", CV_WINDOW_AUTOSIZE);
	namedWindow("colorImage", CV_WINDOW_AUTOSIZE);
	namedWindow("depthImage", CV_WINDOW_AUTOSIZE);
	namedWindow("skeletonImage", CV_WINDOW_AUTOSIZE);
	
    while (1) 
    { 
        if(WaitForSingleObject(colorEvent, 0)==0) 
            getColorImage(colorEvent, colorStreamHandle, colorImage); 
        if(WaitForSingleObject(depthEvent, 0)==0) 
            getDepthImage(depthEvent, depthStreamHandle, depthImage); 
		//这里使用INFINITE是为了避免没有激活skeletonEvent而跳过此代码出现colorimage频闪的现象 
        if(WaitForSingleObject(skeletonEvent, INFINITE)==0)
            getSkeletonImage(skeletonEvent, skeletonImage, colorImage, depthImage); 
          
        for (int i=0; i<6; i++)  
        { 
            if(tracked[i] == TRUE) 
            { 
                mask.setTo(0);
                getTheContour(depthImage, i, mask); 
                tracked[i] = FALSE; 
                break; 
            } 
        } 
 
        imshow("mask", mask); 
        imshow("colorImage", colorImage); 
        imshow("depthImage", depthImage); 
        imshow("skeletonImage", skeletonImage); 
 
        if(cvWaitKey(1)==27) 
            break; 
    } 
 
    NuiShutdown(); 
    return 0; 
}

void getColorImage(HANDLE &colorEvent, HANDLE &colorStreamHandle, Mat &colorImage) 
{ 
    const NUI_IMAGE_FRAME *colorFrame = NULL; 
 
    NuiImageStreamGetNextFrame(colorStreamHandle, 0, &colorFrame); 
    INuiFrameTexture *pTexture = colorFrame->pFrameTexture;   
 
    NUI_LOCKED_RECT LockedRect; 
    pTexture->LockRect(0, &LockedRect, NULL, 0);   
 
    if( LockedRect.Pitch != 0 ) 
    { 
		for (int i=0; i<colorImage.rows; i++) 
        {
			uchar *ptr = colorImage.ptr<uchar>(i);  //第i行的指针					
			//每个字节代表一个颜色信息，直接使用uchar
            uchar *pBuffer = (uchar*)(LockedRect.pBits) + i * LockedRect.Pitch;
            for (int j=0; j<colorImage.cols; j++) 
            { 
                ptr[3*j] = pBuffer[4*j];  //内部数据是4个字节，0-1-2是BGR，第4个现在未使用 
                ptr[3*j+1] = pBuffer[4*j+1]; 
                ptr[3*j+2] = pBuffer[4*j+2]; 
            } 
		} 
    } 
    else 
    { 
        cout<<"捕捉色彩图像出现错误"<<endl; 
    }
	pTexture->UnlockRect(0); 
    NuiImageStreamReleaseFrame(colorStreamHandle, colorFrame );
} 
 
void getDepthImage(HANDLE &depthEvent, HANDLE &depthStreamHandle, Mat &depthImage) 
{ 
    const NUI_IMAGE_FRAME *depthFrame = NULL; 
 
    NuiImageStreamGetNextFrame(depthStreamHandle, 0, &depthFrame); 
    INuiFrameTexture *pTexture = depthFrame->pFrameTexture;   
 
    NUI_LOCKED_RECT LockedRect; 
    pTexture->LockRect(0, &LockedRect, NULL, 0);   
 
    RGBQUAD q; 
    if( LockedRect.Pitch != 0 ) 
    { 
        for (int i=0; i<depthImage.rows; i++) 
        { 
            uchar *ptr = depthImage.ptr<uchar>(i); 
            uchar *pBuffer = (uchar*)(LockedRect.pBits) + i * LockedRect.Pitch;
			USHORT *pBufferRun = (USHORT*) pBuffer; 
			
            for (int j=0; j<depthImage.cols; j++) 
            { 
                int player = pBufferRun[j]&7; 
                int data = (pBufferRun[j]&0xfff8) >> 3; 
                 
                uchar imageData = 255-(uchar)(256*data/0x0fff); 
                q.rgbBlue = q.rgbGreen = q.rgbRed = 0; 
 
                switch(player) 
                { 
					case 0:   
						q.rgbRed = imageData / 2;   
						q.rgbBlue = imageData / 2;   
						q.rgbGreen = imageData / 2;   
						break;   
					case 1:    
						q.rgbRed = imageData;   
						break;   
					case 2:   
						q.rgbGreen = imageData;   
						break;   
					case 3:   
						q.rgbRed = imageData / 4;   
						q.rgbGreen = q.rgbRed*4;  //这里利用乘的方法，而不用原来的方法可以避免不整除的情况 
						q.rgbBlue = q.rgbRed*4;  //可以在后面的getTheContour()中配合使用，避免遗漏一些情况 
						break;   
					case 4:   
						q.rgbBlue = imageData / 4;  
						q.rgbRed = q.rgbBlue*4;   
						q.rgbGreen = q.rgbBlue*4;   
						break;   
					case 5:   
						q.rgbGreen = imageData / 4;  
						q.rgbRed = q.rgbGreen*4;   
						q.rgbBlue = q.rgbGreen*4;   
						break;   
					case 6:   
						q.rgbRed = imageData / 2;   
						q.rgbGreen = imageData / 2;    
						q.rgbBlue = q.rgbGreen*2;   
						break;   
					case 7:   
						q.rgbRed = 255 - ( imageData / 2 );   
						q.rgbGreen = 255 - ( imageData / 2 );   
						q.rgbBlue = 255 - ( imageData / 2 ); 
				} 	 
				ptr[3*j] = q.rgbBlue; 
				ptr[3*j+1] = q.rgbGreen; 
				ptr[3*j+2] = q.rgbRed; 
            } 
        } 
    } 
    else 
    { 
        cout << "捕捉深度图像出现错误" << endl; 
    } 
	
	pTexture->UnlockRect(0);
    NuiImageStreamReleaseFrame(depthStreamHandle, depthFrame);  
} 
 
void getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage, Mat &colorImage, Mat &depthImage) 
{ 
    NUI_SKELETON_FRAME skeletonFrame = {0};
    bool bFoundSkeleton = false;  
 
    if(NuiSkeletonGetNextFrame( 0, &skeletonFrame ) == S_OK )   
    {   
        for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )   
        {   
            if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ) 
            {   
                bFoundSkeleton = true;   
                break; 
            }   
        }   
    } 
    else 
    { 
        cout << "没有找到合适的骨骼帧" << endl; 
        return;  
    } 
 
    if( !bFoundSkeleton )   
    {   
        return;  
    }   
 
    NuiTransformSmooth(&skeletonFrame, NULL);//平滑骨骼帧,消除抖动   
    skeletonImage.setTo(0);   
	
    for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )   
    {   
        if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&   
            skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_SHOULDER_CENTER] != NUI_SKELETON_POSITION_NOT_TRACKED)   
        {   
            float fx, fy;   
 
            for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)//所有的坐标转化为深度图的坐标   
            {   
                NuiTransformSkeletonToDepthImage(skeletonFrame.SkeletonData[i].SkeletonPositions[j], &fx, &fy );   
                skeletonPoint[i][j].x = (int)fx;   
                skeletonPoint[i][j].y = (int)fy;   
            }   
 
            for (int j=0; j<NUI_SKELETON_POSITION_COUNT ; j++)   
            {   
                if (skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[j] != NUI_SKELETON_POSITION_NOT_TRACKED)//跟踪点一用有三种状态：1没有被跟踪到，2跟踪到，3根据跟踪到的估计到   
                {   
                    LONG colorx, colory; 
                    NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, 0,  
                        skeletonPoint[i][j].x, skeletonPoint[i][j].y, 0,&colorx, &colory); 
                    colorPoint[i][j].x = int(colorx);
					colorPoint[i][j].y = int(colory); //存储坐标点 
                    circle(colorImage, colorPoint[i][j], 4, cvScalar(0, 255, 255), 1, 8, 0); 
                    circle(skeletonImage, skeletonPoint[i][j], 3, cvScalar(0, 255, 255), 1, 8, 0); 
 
                    tracked[i] = TRUE; 
                } 
            } 
 
            drawSkeleton(colorImage, colorPoint[i], i);  
            drawSkeleton(skeletonImage, skeletonPoint[i], i); 
        } 
    }   
} 
 
void drawSkeleton(Mat &image, CvPoint pointSet[], int whichone) 
{ 
    CvScalar color; 
    switch(whichone) //跟踪不同的人显示不同的颜色 
    { 
    case 0: 
        color = cvScalar(255); 
        break; 
    case 1: 
        color = cvScalar(0,255); 
        break; 
    case 2: 
        color = cvScalar(0, 0, 255); 
        break; 
    case 3: 
        color = cvScalar(255, 255, 0); 
        break; 
    case 4: 
        color = cvScalar(255, 0, 255); 
        break; 
    case 5: 
        color = cvScalar(0, 255, 255); 
        break; 
    } 
 
    if((pointSet[NUI_SKELETON_POSITION_HEAD].x!=0 || pointSet[NUI_SKELETON_POSITION_HEAD].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HEAD], pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SPINE], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SPINE], pointSet[NUI_SKELETON_POSITION_HIP_CENTER], color, 2); 
 
    //左上肢 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HAND_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], pointSet[NUI_SKELETON_POSITION_HAND_LEFT], color, 2); 
 
    //右上肢 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], pointSet[NUI_SKELETON_POSITION_HAND_RIGHT], color, 2); 
 
    //左下肢 
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_LEFT], pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0) &&  
        (pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], pointSet[NUI_SKELETON_POSITION_FOOT_LEFT], color, 2); 
 
    //右下肢 
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT],color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT], pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT], color, 2); 
} 
 
 //根据给定的深度数据的关系（在getDepthImage()中的）确定不同的跟踪目标 
void getTheContour(Mat &image, int whichone, Mat &mask)
{ 
    for (int i=0; i<image.rows; i++) 
    { 
		uchar *ptr = image.ptr<uchar>(i); 
        uchar *ptrmask = mask.ptr<uchar>(i);  
        for (int j=0; j<image.cols; j++) 
        { 
            if (ptr[3*j]==0 && ptr[3*j+1]==0 && ptr[3*j+2]==0) //都为0的时候予以忽略 
            { 
                ptrmask[3*j]=ptrmask[3*j+1]=ptrmask[3*j+2]=0; 
            }else if(ptr[3*j]==0 && ptr[3*j+1]==0 && ptr[3*j+2]!=0)//ID为1的时候，显示绿色 
            { 
                ptrmask[3*j] = 0; 
                ptrmask[3*j+1] = 255; 
                ptrmask[3*j+2] = 0; 
            }else if (ptr[3*j]==0 && ptr[3*j+1]!=0 && ptr[3*j+2]==0)//ID为2的时候，显示红色 
            { 
                ptrmask[3*j] = 0; 
                ptrmask[3*j+1] = 0; 
                ptrmask[3*j+2] = 255; 
            }else if (ptr[3*j]==ptr[3*j+1] && ptr[3*j]==4*ptr[3*j+2])//ID为3的时候 
            { 
                ptrmask[3*j] = 255; 
                ptrmask[3*j+1] = 255; 
                ptrmask[3*j+2] = 0; 
            }else if (4*ptr[3*j]==ptr[3*j+1] && ptr[3*j+1]==ptr[3*j+2])//ID为4的时候 
            { 
                ptrmask[3*j] = 255; 
                ptrmask[3*j+1] = 0; 
                ptrmask[3*j+2] = 255; 
            }else if (ptr[3*j]==4*ptr[3*j+1] && ptr[3*j]==ptr[3*j+2])//ID为5的时候 
            { 
                ptrmask[3*j] = 0; 
                ptrmask[3*j+1] = 255; 
                ptrmask[3*j+2] = 255; 
            }else if (ptr[3*j]==2*ptr[3*j+1] && ptr[3*j+1]==ptr[3*j+2])//ID为6的时候 
            { 
                ptrmask[3*j] = 255; 
                ptrmask[3*j+1] = 255; 
                ptrmask[3*j+2] = 255; 
            }else if (ptr[3*j]==ptr[3*j+1] && ptr[3*j]==ptr[3*j+2])//ID为7的时候或者ID为0的时候，显示蓝色 
            { 
                ptrmask[3*j] = 255; 
                ptrmask[3*j+1] = 0; 
                ptrmask[3*j+2] = 0; 
            }else 
            { 
                cout <<"如果输出这段代码，说明有遗漏的情况，请查询getTheContour函数" << endl; 
            } 
        } 
    } 
}
```
**三、代码解析**
这里，在对前面所说的彩色图像、深度图像和骨骼图像的整合功能的实现上，就是通过整合前面工作的代码而已（请参考之前的博文）。然后唯一有一个新的东西就是抠图，我们需要把用户扣出来，显示与背景不一样的颜色，具体的函数就是：
void getTheContour(Mat &image, int whichone, Mat &mask)；
调用它的时候，传入的是深度图像image，我们在深度图像处理的时候，也就是getDepthImage 函数里面对深度数据已经做了处理，已经将玩家的ID数据转换为RGBQUAD 格式的数据了，所以用户的ID数据已经隐含的在深度图像的每个像素中表示了，所以这里我们只需要分析每个像素的RGB值就可以对应的知道是哪个ID了，那么属于某个ID的像素我们就用某种颜色来表示，其他背景像素我们就用蓝色来表示。这样就完成了抠图。
实际上，我们之前提到的getDepthImage 函数就是处理深度数据的每一个像素，如果属于同一个用户的ID，那么像素就标为同种颜色，不同的用户，其ID不一样，颜色的标示也不一样，如果不属于某个用户的像素，那么就采用原来的深度值，而getTheContour函数实际上就是这里不一样而已。“如果不属于某个用户的像素，那么就采用原来的深度值”，这里改为“如果不属于某个用户的像素，那么就将像素值赋值为蓝色”。
既然你都把用户扣出来了，那么把他弄到你给定的一个图片场景里面应该不难了吧。呵呵，相信大家都有各种办法的。
至此，本文的目标就达到了，下面是结果。其实右边是彩色图像，在彩色图像里面用户的骨架也是被标注出来的，只是本人不宜抛头露面，所以就抛弃这部分的截图了。
![](https://img-my.csdn.net/uploads/201211/08/1352367304_9398.jpg)
另外，感觉到这里，Kinect SDK能提供的基本数据我们会获取了，也就是勉勉强强把地基打好了，那么后面的工作就是在上面创造属于你的世界了。我们后面会学习基于Kinect的姿态、手势识别等等的内容，通过他们去构建良好的人机交互程序。当然，Kinect还包含有语音识别的功能，这里暂时就不涉及了。


