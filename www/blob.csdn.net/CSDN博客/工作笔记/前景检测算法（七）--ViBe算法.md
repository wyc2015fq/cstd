# 前景检测算法（七）--ViBe算法 - 工作笔记 - CSDN博客





2016年07月09日 13:17:31[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6876
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

[http://blog.csdn.net/zouxy09/article/details/9622285](http://blog.csdn.net/zouxy09/article/details/9622285)




因为监控发展的需求，目前前景检测的研究还是很多的，也出现了很多新的方法和思路。个人了解的大概概括为以下一些：

       帧差、背景减除（GMM、CodeBook、 SOBS、 SACON、 VIBE、 W4、多帧平均……）、光流（稀疏光流、稠密光流）、运动竞争（Motion Competition）、运动模版（运动历史图像）、时间熵……等等。如果加上他们的改进版，那就是很大的一个家族了。

      对于上一些方法的一点简单的对比分析可以参考下：

[http://www.cnblogs.com/ronny/archive/2012/04/12/2444053.html](http://www.cnblogs.com/ronny/archive/2012/04/12/2444053.html)

       至于哪个最好，看使用环境吧，各有千秋，有一些适用的情况更多，有一些在某些情况下表现更好。这些都需要针对自己的使用情况作测试确定的。呵呵。

       推荐一个牛逼的库：[http://code.google.com/p/bgslibrary/](http://code.google.com/p/bgslibrary/)里面包含了各种背景减除的方法，可以让自己少做很多力气活。

       还有王先荣博客上存在不少的分析：

[http://www.cnblogs.com/xrwang/archive/2010/02/21/ForegroundDetection.html](http://www.cnblogs.com/xrwang/archive/2010/02/21/ForegroundDetection.html)

       下面的博客上转载王先荣的上面几篇，然后加上自己分析了两篇：

[http://blog.csdn.net/stellar0](http://blog.csdn.net/stellar0)



       本文主要关注其中的一种背景减除方法：ViBe。stellar0的博客上对ViBe进行了分析，我这里就不再啰嗦了，具体的理论可以参考：

[http://www2.ulg.ac.be/telecom/research/vibe/](http://www2.ulg.ac.be/telecom/research/vibe/)

[http://blog.csdn.net/stellar0/article/details/8777283](http://blog.csdn.net/stellar0/article/details/8777283)

[http://blog.csdn.net/yongshengsilingsa/article/details/6659859](http://blog.csdn.net/yongshengsilingsa/article/details/6659859)

[http://www2.ulg.ac.be/telecom/research/vibe/download.html](http://www2.ulg.ac.be/telecom/research/vibe/download.html)

[http://www.cvchina.info/2011/12/25/vibe/](http://www.cvchina.info/2011/12/25/vibe/)







**ViBe**是一种像素级的背景建模、前景检测算法，该算法主要不同之处是**背景模型的更新策略，随机选择需要替换的像素的样本，随机选择邻域像素进行更新**。在无法确定像素变化的模型时，随机的更新策略，在一定程度上可以模拟像素变化的不确定性。

**背景模型的初始化**

　　初始化是建立背景模型的过程，一般的检测算法需要一定长度的视频序列学习完成，影响了检测的实时性，而且当视频画面突然变化时，重新学习背景模型需要较长时间。

　　ViBe算法主要是**利用单帧视频序列初始化背景模型，对于一个像素点，结合相邻像素点拥有相近像素值的空间分布特性，随机的选择它的邻域点的像素值作为它的模型样本值**。

**优点：**不仅减少了背景模型建立的过程，还可以处理背景突然变化的情况，当检测到背景突然变化明显时，只需要舍弃原始的模型，重新利用变化后的首帧图像建立背景模型。

**缺点：**由于可能采用了运动物体的像素初始化样本集，容易引入拖影（Ghost）区域。

**前景检测过程**

　　背景模型为每个背景点存储一个样本集，然后每个新的像素值和样本集比较判断是否属于背景。

　　计算新像素值和样本集中每个样本值的距离，若距离小于阈值，则近似样本点数目增加。

　　如果近似样本点数目大于阈值，则认为新的像素点为背景。

　　检测过程主要由三个参数决定：**样本集数目N，阈值#min和距离相近判定的阈值R**，一般具体实现，参数设置为N=20，#min=2，R=20。

![](http://images.cnitblog.com/blog/414008/201401/210950440475.png)

**背景模型的更新策略**

1).无记忆更新策略

　　每次确定需要更新像素点的背景模型时，以新的像素值随机取代该像素点样本集的一个样本值。

2).时间取样更新策略

　　并不是每处理一帧数据，都需要更新处理，而是按一定的更新率更新背景模型。当一个像素点被判定为背景时，它有1/rate的概率更新背景模型。rate是时间采样因子，一般取值为16。

3).空间邻域更新策略

　　针对需要更新像素点，随机的选择一个该像素点邻域的背景模型，以新的像素点更新被选中的背景模型。

**ViBe的改进**

![](http://images.cnitblog.com/blog/414008/201401/211010138138.png)

1).距离计算方法

　　以**圆椎模型**代替原来的几何距离计算方法



![](http://images.cnitblog.com/blog/414008/201401/211012299068.png)

　　以自适应阈值代替原来固定的距离判定阈值，**阈值大小与样本集的方差成正比**，样本集方差越大，说明背景越复杂，判定阈值应该越大。

![](http://images.cnitblog.com/blog/414008/201401/211014547505.png)

2).分离updating mask和segmentation mask

　　引入目标整体的概念，弥补基于像素级前景检测的不足。针对updating mask和segmentation mask采用不同尺寸的形态学处理方法，提高检测准确率。

3).抑制邻域更新

　　在updating mask里，计算像素点的梯度，根据梯度大小，确定是否需要更新邻域。梯度值越大，说明像素值变化越大，说明该像素值可能为前景，不应该更新。

4).检测闪烁像素点

　　引入闪烁程度的概念，当一个像素点的updating label与前一帧的updating label不一样时，blinking level增加15，否则，减少1，然后根据blinking level的大小判断该像素点是否为闪烁点。闪烁像素主要出现在背景复杂的场景，如树叶、水纹等，这些场景会出现像素背景和前景的频繁变化，因而针对这些闪烁应该单独处理，可以作为全部作为背景。

5).增加更新因子

　　ViBe算法中，默认的更新因子是16，当背景变化很快时，背景模型无法快速的更新，将会导致前景检测的较多的错误。因而，需要根据背景变化快慢程度，调整更新因子的大小，可将更新因子分多个等级，如rate = 16,rate = 5,rate = 1。




1）VIBE-A powerful random technique to estimatie the background in video sequences.

2) VIBE-A universal background subtraction algorithms for video sequences

   VIBE的头文件Vibe.hpp如下：





**[cpp]**[view plain](http://blog.csdn.net/kezunhai/article/details/9190923#)[copy](http://blog.csdn.net/kezunhai/article/details/9190923#)[print](http://blog.csdn.net/kezunhai/article/details/9190923#)[?](http://blog.csdn.net/kezunhai/article/details/9190923#)

- #pragma once
- #include "stdafx.h"
- #define  WINSIZE 3
- 
- class Vibe  
- {  
- public:  
-     Vibe(void);  
-     Vibe(IplImage *img);  
- void SetMinMatch(int nthreshold){g_MinMatch=nthreshold;}  
- void SetRadius(int radius){g_Radius=radius;}  
- void SetSampleNum(int num){g_SampleNum=num;}  
- void SetThreshold(double t){g_threshold=t;}  
-     IplImage* GetForeground(){return g_ForeImg;}  
-     IplImage* GetSegMask(){return g_SegementMask;}  
- void Detect(IplImage *img);   
- void ForegroundCombineEdge(); // 结合边缘信息
- void DeleteSmallAreaInForeground(double minArea=20);//删除小面积区域
- // 实现背景更新机制
- void Update();  
- // 实现后处理，主要用形态学算子
- void PostProcess();  
- 
- public:  
-     ~Vibe(void);  
- 
- private:      
- void ClearLongLifeForeground(int i_lifeLength=200); // 清除场景中存在时间较长的像素，i_lifeLength用于控制允许存在的最长时间
- double AreaDense(IplImage *pFr,int AI,int AJ,int W,int H); //计算(i,j)处邻域大小为W×H的密度
- int GetRandom(int istart,int iend); // 默认istart=0,iend=15
- int GetRandom(int random);  
- int GetRandom();// 产生一个随机数
- // 计算两个像素之间的欧式距离
- double CalcPixelDist(CvScalar bkCs,CvScalar curCs);  
- // 按照Kim的方法来计算颜色畸变
- double CalcuColorDist(CvScalar bkCs,CvScalar curCs);  
- int g_SampleNum;// Sample number for the models，默认为20
- int g_MinMatch; // 当前像素与背景模型匹配的最少个数，默认为2
- int g_Height;  
- int g_Width;  
- int g_Radius;// 球体的半径，默认为20
- int g_offset; //边界的宽和高
- double g_threshold; // 距离度量的阈值
-     unsigned char ***g_Model;// 保存背景模型  
-     IplImage *g_ForeImg;// 保存前景图
-     IplImage *g_Edge;  
- 
-     IplConvKernel* element;  
- 
-     IplImage *g_SegementMask; //分割掩膜
-     IplImage *g_UpdateMask; // 更新掩膜
-     IplImage *g_Gray;  
- int ** LifeLength; // 记录前景点的生命长度,如果前景点的生命长度到达一定的阈值，则将其融入背景中去,且要随机两次。    
- };  

![](http://static.blog.csdn.net/images/save_snippets.png)


```cpp
#pragma once
#include "stdafx.h"
#define  WINSIZE 3

class Vibe
{
public:
	Vibe(void);
	Vibe(IplImage *img);
	void SetMinMatch(int nthreshold){g_MinMatch=nthreshold;}
	void SetRadius(int radius){g_Radius=radius;}
	void SetSampleNum(int num){g_SampleNum=num;}
	void SetThreshold(double t){g_threshold=t;}
	IplImage* GetForeground(){return g_ForeImg;}
	IplImage* GetSegMask(){return g_SegementMask;}
	void Detect(IplImage *img);	
	void ForegroundCombineEdge(); // 结合边缘信息
	void DeleteSmallAreaInForeground(double minArea=20);//删除小面积区域
	// 实现背景更新机制
	void Update();
	// 实现后处理，主要用形态学算子
	void PostProcess();

public:
	~Vibe(void);

private:	
	void ClearLongLifeForeground(int i_lifeLength=200); // 清除场景中存在时间较长的像素，i_lifeLength用于控制允许存在的最长时间
	double AreaDense(IplImage *pFr,int AI,int AJ,int W,int H); //计算(i,j)处邻域大小为W×H的密度
	int GetRandom(int istart,int iend); // 默认istart=0,iend=15
	int GetRandom(int random);
	int GetRandom();// 产生一个随机数
	// 计算两个像素之间的欧式距离
	double CalcPixelDist(CvScalar bkCs,CvScalar curCs);
	// 按照Kim的方法来计算颜色畸变
	double CalcuColorDist(CvScalar bkCs,CvScalar curCs);
	int g_SampleNum;// Sample number for the models，默认为20
	int g_MinMatch; // 当前像素与背景模型匹配的最少个数，默认为2
	int g_Height;
	int g_Width;
	int g_Radius;// 球体的半径，默认为20
	int g_offset; //边界的宽和高
	double g_threshold; // 距离度量的阈值
	unsigned char ***g_Model;// 保存背景模型	
	IplImage *g_ForeImg;// 保存前景图
	IplImage *g_Edge;

	IplConvKernel* element;

	IplImage *g_SegementMask; //分割掩膜
	IplImage *g_UpdateMask; // 更新掩膜
	IplImage *g_Gray;
	int ** LifeLength; // 记录前景点的生命长度,如果前景点的生命长度到达一定的阈值，则将其融入背景中去,且要随机两次。	
};
```




对应的实现文件如下Vibe.cpp所示：











**[cpp]**[view plain](http://blog.csdn.net/kezunhai/article/details/9190923#)[copy](http://blog.csdn.net/kezunhai/article/details/9190923#)[print](http://blog.csdn.net/kezunhai/article/details/9190923#)[?](http://blog.csdn.net/kezunhai/article/details/9190923#)

- #include "StdAfx.h"
- #include "Vibe.h"
- 
- Vibe::Vibe(void)  
- {  
-     g_Radius=20;  
-     g_MinMatch=2;     
-     g_SampleNum=20;  
-     g_offset=(WINSIZE-1)/2;  
- 
- }  
- 
- Vibe::Vibe(IplImage *img)  
- {  
- if (!img)  
-     {  
-         cout<<" The parameter referenced to NUll Pointer!"<<endl;  
- return;  
-     }  
- this->g_Height=img->height;  
- this->g_Width=img->width;  
- 
-     g_Radius=20;  
-     g_MinMatch=2;     
-     g_SampleNum=20;  
-     g_threshold=50;  
-     g_offset=(WINSIZE-1)/2;  
- 
-     g_ForeImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
-     g_Gray=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
-     g_Edge=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
-     g_SegementMask=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
-     g_UpdateMask=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
- 
-     element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_CROSS,NULL);  
- 
-     cvCvtColor(img,g_Gray,CV_BGR2GRAY);  
- 
- // 以上完成相关的初始化操作
- /**********************  以下实现第一帧在每个像素的8邻域内的采样功能，建立对应的背景模型*****************************/
- 
- int i=0,j=0,k=0;  
-     g_Model=new unsigned char**[g_SampleNum];  
- for (k=0;k<g_SampleNum;k++)  
-     {  
-         g_Model[k]=new unsigned char *[g_Height];  
- for(i=0;i<g_Height;i++)  
-         {  
-             g_Model[k][i]=new unsigned char [g_Width];  
- for (j=0;j<g_Width;j++)  
-             {  
-                 g_Model[k][i][j]=0;  
-             }  
-         }  
-     }  
- 
- // 采样进行背景建模 
- double dVal;  
- int ri=0,rj=0; //随机采样的值
- for (i=g_offset;i<g_Height-g_offset;i++)  
-     {  
- for (j=g_offset;j<g_Width-g_offset;j++)  
-         {  
- // 周围3*3的邻域内进行采样
- for(k=0;k<g_SampleNum;k++)  
-             {  
-                 ri=GetRandom(i);  
-                 rj=GetRandom(j);  
-                 dVal=cvGetReal2D(g_Gray,ri,rj);       
-                 g_Model[k][i][j]=dVal;                            
-             }  
-         }  
-     }  
- 
- // 初始化前景点掩膜的生命长度
-     LifeLength=newint *[g_Height];  
- for (i=0;i<g_Height;i++)  
-     {  
-         LifeLength[i]=newint [g_Width];  
- for(j=0;j<g_Width;j++)  
-         {  
-             LifeLength[i][j]=0;  
-         }  
-     }  
- }  
- 
- 
- void Vibe::Detect(IplImage *img)  
- {  
-     cvZero(g_ForeImg);    
-     cvCvtColor(img,g_Gray,CV_BGR2GRAY);  
- int i=0,j=0,k=0;  
- double dModVal,dCurrVal;  
- int tmpCount=0;// 距离比较在阈值内的次数
- double tmpDist=0;     
- int iR1,iR2;//产生随机数
- int Ri,Rj; // 产生邻域内X和Y的随机数
- 
- for (i=0;i<g_Height;i++)  
-     {         
- for (j=0;j<g_Width;j++)  
-         {             
- if( i < g_offset || j < g_offset || i> g_Height - g_offset || j> g_Width - g_offset )  
-             {  
-                 cvSetReal2D(g_ForeImg,i,j,0);  
- continue;  
-             }  
- else
-             {  
-                 tmpCount=0;  
-                 dCurrVal=cvGetReal2D(g_Gray,i,j);                 
- for (k=0;k<g_SampleNum && tmpCount<g_MinMatch  ;k++)  
-                 {                     
-                     dModVal=g_Model[k][i][j];  
- //tmpDist=CalcPixelDist(dCurrVal,dModVal);
- //tmpDist=CalcuColorDist(dCurrVal,dModVal); 
-                     tmpDist=fabs(dModVal-dCurrVal);                                   
- if (tmpDist<g_Radius)  
-                     {  
-                         tmpCount++;  
-                     }                     
-                 }  
- 
- //判断是否匹配上
- if (tmpCount>=g_MinMatch)  
-                 {  
-                     cvSetReal2D(g_ForeImg,i,j,0);  
- // 背景模型的更新                  
-                     iR1=GetRandom(0,15);  
- if (iR1==0)  
-                     {  
-                         iR2=GetRandom();  
-                         g_Model[iR2][i][j]=dCurrVal;                          
-                     }  
- 
- //进一步更新邻域模型
- 
-                     iR1=GetRandom(0,15);  
- if (iR1==0)  
-                     {  
-                         Ri=GetRandom(i);  
-                         Rj=GetRandom(j);  
-                         iR2=GetRandom();  
-                         g_Model[iR2][Ri][Rj]=dCurrVal;                        
-                     }                         
-                 }  
- else
-                 {  
-                     cvSetReal2D(g_ForeImg,i,j,255);  
-                 }  
-             }  
-         }         
-     }         
- 
- //ForegroundCombineEdge();
-     DeleteSmallAreaInForeground(80);  
-     ClearLongLifeForeground();  
- //PostProcess();
- }  
- 
- double Vibe::AreaDense(IplImage *pFr,int AI,int AJ,int W,int H)  
- {  
- if (AI<=2 || AJ<=2 || AJ>=(g_Width-2) || AI>=(g_Height-2))  
-     {  
- return 0;  
-     }  
- int Num=0,i=0,j=0;  
- double dVal=0,dense=0;  
- int Total=(2*H+1)*(2*W+1);  
- for (i=AI-H;i<=AI+H;i++)  
-     {  
- for (j=AJ-W;j<=AJ+W;j++)  
-         {  
-             dVal=cvGetReal2D(pFr,i,j);  
- if (dVal>200)  
-             {  
-                 Num++;  
-             }  
-         }  
-     }  
-     dense=(double)Num/(double)Total;  
- return dense;  
- }  
- 
- void Vibe::ForegroundCombineEdge()  
- {     
-     cvZero(g_Edge);  
- //cvZero(g_SegementMask);
- //cvCopy(g_ForeImg,g_SegementMask);
-     cvCanny(g_Gray,g_Edge,30,200,3);  
- int i=0,j=0;  
- double dense;  
- double dVal;  
- for (i=g_offset;i<g_Height-g_offset;i++)  
-     {  
- for (j=g_offset;j<g_Width-g_offset;j++)  
-         {  
-             dense=AreaDense(g_ForeImg,i,j,2,2);  
-             dVal=cvGetReal2D(g_Edge,i,j);  
- if (dense>0.2 && dVal>200)  
-             {  
-                 cvSetReal2D(g_ForeImg,i,j,255);  
-             }  
-         }  
-     }  
- 
- }  
- 
- 
- void Vibe::DeleteSmallAreaInForeground(double minArea/* =20 */)  
- {  
- //cvZero(g_SegementMask);
- //cvCopy(g_ForeImg,g_SegementMask);
- int region_count = 0;  
-     CvSeq *first_seq = NULL, *prev_seq = NULL, *seq = NULL;  
-     CvMemStorage*  storage = cvCreateMemStorage();  
-     cvClearMemStorage(storage);  
-     cvFindContours( g_ForeImg, storage, &first_seq, sizeof(CvContour), CV_RETR_LIST );  
- for( seq = first_seq; seq; seq = seq->h_next )  
-     {  
-         CvContour* cnt = (CvContour*)seq;  
- if( cnt->rect.width * cnt->rect.height < minArea )  
-         {  
-             prev_seq = seq->h_prev;  
- if( prev_seq )  
-             {  
-                 prev_seq->h_next = seq->h_next;  
- if( seq->h_next ) seq->h_next->h_prev = prev_seq;  
-             }  
- else
-             {  
-                 first_seq = seq->h_next;  
- if( seq->h_next ) seq->h_next->h_prev = NULL;  
-             }  
-         }  
- else
-         {             
-             region_count++;  
-         }  
-     }              
-     cvZero(g_ForeImg);  
-     cvDrawContours(g_ForeImg, first_seq, CV_RGB(0, 0, 255), CV_RGB(0, 0, 255), 10, -1);   
- 
- /*
-     CvContourScanner scanner = cvStartFindContours( g_ForeImg, storage,sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
-     CvSeq *contours=NULL,*c=NULL;
-     int poly1Hull0=0;
-     int nContours=0;
-     double perimScale=100;
-     while( (c = cvFindNextContour( scanner )) != 0 ) 
-     {
-         double len = cvContourPerimeter( c );
-         double q = (g_ForeImg->height + g_ForeImg->width)/perimScale; // calculate perimeter len threshold
-         if( len < q ) //Get rid of blob if it's perimeter is too small
-             cvSubstituteContour( scanner, 0 );
-         else //Smooth it's edges if it's large enough
-         {
-             CvSeq* newC;
-             if( poly1Hull0 ) //Polygonal approximation of the segmentation 
-                 newC = cvApproxPoly( c, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 2, 0 ); 
-             else //Convex Hull of the segmentation
-                 newC = cvConvexHull2( c, storage, CV_CLOCKWISE, 1 );
-             cvSubstituteContour( scanner, newC );
-             nContours++;
-         }
-     }
-     contours = cvEndFindContours( &scanner );
-     // paint the found regions back into the image
-     cvZero( g_ForeImg );
-     for( c=contours; c != 0; c = c->h_next ) 
-         cvDrawContours( g_ForeImg, c, cvScalarAll(255), cvScalarAll(0), -1, CV_FILLED, 8,cvPoint(0,0));
-     */
- 
-     cvReleaseMemStorage(&storage);    
- }  
- 
- void Vibe::ClearLongLifeForeground(int i_lifeLength/* =200 */)  
- {  
- int i=0,j=0;  
- double dVal=0;  
- double dLife=0;  
- int iR1,iR2=0;  
- double dCurrVal=0;  
- for (i=g_offset;i<g_Height-g_offset;i++)  
-     {  
- for (j=g_offset;j<g_Width-g_offset;j++)  
-         {  
-             dVal=cvGetReal2D(g_ForeImg,i,j);  
-             dLife=LifeLength[i][j];  
- if (dLife>i_lifeLength)  
-             {  
-                 LifeLength[i][j]=0;  
-                 dCurrVal=cvGetReal2D(g_Gray,i,j);  
- // 更新背景模型
-                 iR1=GetRandom();  
-                 iR2=GetRandom();  
-                 g_Model[iR1][i][j]=dCurrVal;  
-                 g_Model[iR2][i][j]=dCurrVal;  
-             }  
- else
-             {  
-                 LifeLength[i][j]=dLife+1;  
-             }  
- 
-         }  
-     }  
- }  
- 
- void Vibe::Update()  
- {  
-     cvZero(g_UpdateMask);     
- 
- }  
- 
- void Vibe::PostProcess()  
- {  
-     cvZero(g_SegementMask);  
-     cvMorphologyEx(g_ForeImg,g_SegementMask,NULL,element,CV_MOP_OPEN,1);  
- 
- }  
- 
- //算颜色畸变
- double Vibe::CalcuColorDist(CvScalar bkCs,CvScalar curCs)  
- {  
- double r,g,b,br,bg,bb;  
-     r=curCs.val[0];  
-     g=curCs.val[1];  
-     b=curCs.val[2];  
- 
-     br=bkCs.val[0];  
-     bg=bkCs.val[1];  
-     bb=bkCs.val[2];  
- 
- double curDist=r*r+g*g*b*b;   
- double bkDist=br*br+bg*bg+bb*bb;  
- 
- double curBK=r*br+g*bg+b*bb;  
- double curbkDist=curBK*curBK;  
- double SquareP;  
- if (bkDist==0.0)  
-     {  
-         SquareP=0;  
-     }  
- else
-     {  
-         SquareP=curbkDist/bkDist;  
-     }  
- double dist=sqrtf(curDist-SquareP);  
- return dist;      
- }  
- 
- double Vibe::CalcPixelDist(CvScalar bkCs,CvScalar curCs)  
- {  
- double tmpDist=pow(bkCs.val[0]-curCs.val[0],2)+pow(bkCs.val[1]-curCs.val[1],2)+pow(bkCs.val[2]-curCs.val[2],2);  
- return sqrtf(tmpDist);  
- }  
- 
- int Vibe::GetRandom()  
- {  
- int val = g_SampleNum * 1.0 * rand() / RAND_MAX;      
- if( val == g_SampleNum )  
- return val - 1;  
- else
- return val;  
- }  
- 
- int Vibe::GetRandom(int random)  
- {  
- int val=random-g_offset+rand()%(2*g_offset);  
- if (val<random-g_offset)  
-     {  
-         val=random-g_offset;  
-     }  
- if (val>random+g_offset)  
-     {  
-         val=random+g_offset;  
-     }     
- return val;   
- }  
- 
- int Vibe::GetRandom(int istart,int iend)  
- {  
- int val=istart+rand()%(iend-istart);  
- return val;  
- }  
- 
- 
- Vibe::~Vibe(void)  
- {  
- if (g_ForeImg)  
-     {  
-         cvReleaseImage(&g_ForeImg);  
-     }  
- if (g_SegementMask)  
-     {  
-         cvReleaseImage(&g_SegementMask);  
-     }  
- if (g_UpdateMask)  
-     {  
-         cvReleaseImage(&g_UpdateMask);  
-     }  
- if (g_Gray)  
-     {  
-         cvReleaseImage(&g_Gray);  
-     }  
- 
- if (g_Model!=NULL)  
-     {  
- delete[]g_Model;  
-         g_Model=NULL;  
-     }  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)


```cpp
#include "StdAfx.h"
#include "Vibe.h"

Vibe::Vibe(void)
{
	g_Radius=20;
	g_MinMatch=2;	
	g_SampleNum=20;
	g_offset=(WINSIZE-1)/2;

}

Vibe::Vibe(IplImage *img)
{
	if (!img)
	{
		cout<<" The parameter referenced to NUll Pointer!"<<endl;
		return;
	}
	this->g_Height=img->height;
	this->g_Width=img->width;

	g_Radius=20;
	g_MinMatch=2;	
	g_SampleNum=20;
	g_threshold=50;
	g_offset=(WINSIZE-1)/2;

	g_ForeImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	g_Gray=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	g_Edge=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	g_SegementMask=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	g_UpdateMask=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);

	element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_CROSS,NULL);

	cvCvtColor(img,g_Gray,CV_BGR2GRAY);
	
	// 以上完成相关的初始化操作
	/**********************  以下实现第一帧在每个像素的8邻域内的采样功能，建立对应的背景模型*****************************/
	
	int i=0,j=0,k=0;
	g_Model=new unsigned char**[g_SampleNum];
	for (k=0;k<g_SampleNum;k++)
	{
		g_Model[k]=new unsigned char *[g_Height];
		for(i=0;i<g_Height;i++)
		{
			g_Model[k][i]=new unsigned char [g_Width];
			for (j=0;j<g_Width;j++)
			{
				g_Model[k][i][j]=0;
			}
		}
	}
	
	// 采样进行背景建模	
	double dVal;
	int ri=0,rj=0; //随机采样的值
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
			// 周围3*3的邻域内进行采样
			for(k=0;k<g_SampleNum;k++)
			{
				ri=GetRandom(i);
				rj=GetRandom(j);
				dVal=cvGetReal2D(g_Gray,ri,rj);		
				g_Model[k][i][j]=dVal;							
			}
		}
	}

	// 初始化前景点掩膜的生命长度
	LifeLength=new int *[g_Height];
	for (i=0;i<g_Height;i++)
	{
		LifeLength[i]=new int [g_Width];
		for(j=0;j<g_Width;j++)
		{
			LifeLength[i][j]=0;
		}
	}
}


void Vibe::Detect(IplImage *img)
{
	cvZero(g_ForeImg);	
	cvCvtColor(img,g_Gray,CV_BGR2GRAY);
	int i=0,j=0,k=0;
	double dModVal,dCurrVal;
	int tmpCount=0;// 距离比较在阈值内的次数
	double tmpDist=0;	
	int iR1,iR2;//产生随机数
	int Ri,Rj; // 产生邻域内X和Y的随机数

	for (i=0;i<g_Height;i++)
	{		
		for (j=0;j<g_Width;j++)
		{			
			if( i < g_offset || j < g_offset || i> g_Height - g_offset || j> g_Width - g_offset )
			{
				cvSetReal2D(g_ForeImg,i,j,0);
				continue;
			}
			else
			{
				tmpCount=0;
				dCurrVal=cvGetReal2D(g_Gray,i,j);				
				for (k=0;k<g_SampleNum && tmpCount<g_MinMatch  ;k++)
				{					
					dModVal=g_Model[k][i][j];
					//tmpDist=CalcPixelDist(dCurrVal,dModVal);
					//tmpDist=CalcuColorDist(dCurrVal,dModVal);	
					tmpDist=fabs(dModVal-dCurrVal);									
					if (tmpDist<g_Radius)
					{
						tmpCount++;
					}					
				}

				//判断是否匹配上
				if (tmpCount>=g_MinMatch)
				{
					cvSetReal2D(g_ForeImg,i,j,0);
					// 背景模型的更新					
					iR1=GetRandom(0,15);
					if (iR1==0)
					{
						iR2=GetRandom();
						g_Model[iR2][i][j]=dCurrVal;						
					}

					//进一步更新邻域模型
					
					iR1=GetRandom(0,15);
					if (iR1==0)
					{
						Ri=GetRandom(i);
						Rj=GetRandom(j);
						iR2=GetRandom();
						g_Model[iR2][Ri][Rj]=dCurrVal;						
					}						
				}
				else
				{
					cvSetReal2D(g_ForeImg,i,j,255);
				}
			}
		}		
	}		
	
	//ForegroundCombineEdge();
	DeleteSmallAreaInForeground(80);
	ClearLongLifeForeground();
	//PostProcess();
}

double Vibe::AreaDense(IplImage *pFr,int AI,int AJ,int W,int H)
{
	if (AI<=2 || AJ<=2 || AJ>=(g_Width-2) || AI>=(g_Height-2))
	{
		return 0;
	}
	int Num=0,i=0,j=0;
	double dVal=0,dense=0;
	int Total=(2*H+1)*(2*W+1);
	for (i=AI-H;i<=AI+H;i++)
	{
		for (j=AJ-W;j<=AJ+W;j++)
		{
			dVal=cvGetReal2D(pFr,i,j);
			if (dVal>200)
			{
				Num++;
			}
		}
	}
	dense=(double)Num/(double)Total;
	return dense;
}

void Vibe::ForegroundCombineEdge()
{	
	cvZero(g_Edge);
	//cvZero(g_SegementMask);
	//cvCopy(g_ForeImg,g_SegementMask);
	cvCanny(g_Gray,g_Edge,30,200,3);
	int i=0,j=0;
	double dense;
	double dVal;
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
			dense=AreaDense(g_ForeImg,i,j,2,2);
			dVal=cvGetReal2D(g_Edge,i,j);
			if (dense>0.2 && dVal>200)
			{
				cvSetReal2D(g_ForeImg,i,j,255);
			}
		}
	}

}


void Vibe::DeleteSmallAreaInForeground(double minArea/* =20 */)
{
	//cvZero(g_SegementMask);
	//cvCopy(g_ForeImg,g_SegementMask);
	int region_count = 0;
	CvSeq *first_seq = NULL, *prev_seq = NULL, *seq = NULL;
	CvMemStorage*  storage = cvCreateMemStorage();
	cvClearMemStorage(storage);
	cvFindContours( g_ForeImg, storage, &first_seq, sizeof(CvContour), CV_RETR_LIST );
	for( seq = first_seq; seq; seq = seq->h_next )
	{
		CvContour* cnt = (CvContour*)seq;
		if( cnt->rect.width * cnt->rect.height < minArea )
		{
			prev_seq = seq->h_prev;
			if( prev_seq )
			{
				prev_seq->h_next = seq->h_next;
				if( seq->h_next ) seq->h_next->h_prev = prev_seq;
			}
			else
			{
				first_seq = seq->h_next;
				if( seq->h_next ) seq->h_next->h_prev = NULL;
			}
		}
		else
		{			
			region_count++;
		}
	}			 
	cvZero(g_ForeImg);
	cvDrawContours(g_ForeImg, first_seq, CV_RGB(0, 0, 255), CV_RGB(0, 0, 255), 10, -1);	

	/*
	CvContourScanner scanner = cvStartFindContours( g_ForeImg, storage,sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
	CvSeq *contours=NULL,*c=NULL;
	int poly1Hull0=0;
	int nContours=0;
	double perimScale=100;
	while( (c = cvFindNextContour( scanner )) != 0 ) 
	{
		double len = cvContourPerimeter( c );
		double q = (g_ForeImg->height + g_ForeImg->width)/perimScale; // calculate perimeter len threshold
		if( len < q ) //Get rid of blob if it's perimeter is too small
			cvSubstituteContour( scanner, 0 );
		else //Smooth it's edges if it's large enough
		{
			CvSeq* newC;
			if( poly1Hull0 ) //Polygonal approximation of the segmentation 
				newC = cvApproxPoly( c, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 2, 0 ); 
			else //Convex Hull of the segmentation
				newC = cvConvexHull2( c, storage, CV_CLOCKWISE, 1 );
			cvSubstituteContour( scanner, newC );
			nContours++;
		}
	}
	contours = cvEndFindContours( &scanner );
	// paint the found regions back into the image
	cvZero( g_ForeImg );
	for( c=contours; c != 0; c = c->h_next ) 
		cvDrawContours( g_ForeImg, c, cvScalarAll(255), cvScalarAll(0), -1, CV_FILLED, 8,cvPoint(0,0));
	*/

	cvReleaseMemStorage(&storage);	
}

void Vibe::ClearLongLifeForeground(int i_lifeLength/* =200 */)
{
	int i=0,j=0;
	double dVal=0;
	double dLife=0;
	int iR1,iR2=0;
	double dCurrVal=0;
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
			dVal=cvGetReal2D(g_ForeImg,i,j);
			dLife=LifeLength[i][j];
			if (dLife>i_lifeLength)
			{
				LifeLength[i][j]=0;
				dCurrVal=cvGetReal2D(g_Gray,i,j);
				// 更新背景模型
				iR1=GetRandom();
				iR2=GetRandom();
				g_Model[iR1][i][j]=dCurrVal;
				g_Model[iR2][i][j]=dCurrVal;
			}
			else
			{
				LifeLength[i][j]=dLife+1;
			}

		}
	}
}

void Vibe::Update()
{
	cvZero(g_UpdateMask);	

}

void Vibe::PostProcess()
{
	cvZero(g_SegementMask);
	cvMorphologyEx(g_ForeImg,g_SegementMask,NULL,element,CV_MOP_OPEN,1);

}

//算颜色畸变
double Vibe::CalcuColorDist(CvScalar bkCs,CvScalar curCs)
{
	double r,g,b,br,bg,bb;
	r=curCs.val[0];
	g=curCs.val[1];
	b=curCs.val[2];

	br=bkCs.val[0];
	bg=bkCs.val[1];
	bb=bkCs.val[2];

	double curDist=r*r+g*g*b*b; 
	double bkDist=br*br+bg*bg+bb*bb;

	double curBK=r*br+g*bg+b*bb;
	double curbkDist=curBK*curBK;
	double SquareP;
	if (bkDist==0.0)
	{
		SquareP=0;
	}
	else
	{
		SquareP=curbkDist/bkDist;
	}
	double dist=sqrtf(curDist-SquareP);
	return dist;	
}

double Vibe::CalcPixelDist(CvScalar bkCs,CvScalar curCs)
{
	double tmpDist=pow(bkCs.val[0]-curCs.val[0],2)+pow(bkCs.val[1]-curCs.val[1],2)+pow(bkCs.val[2]-curCs.val[2],2);
	return sqrtf(tmpDist);
}

int Vibe::GetRandom()
{
	int val = g_SampleNum * 1.0 * rand() / RAND_MAX;	
	if( val == g_SampleNum )
		return val - 1;
	else
		return val;
}

int Vibe::GetRandom(int random)
{
	int val=random-g_offset+rand()%(2*g_offset);
	if (val<random-g_offset)
	{
		val=random-g_offset;
	}
	if (val>random+g_offset)
	{
		val=random+g_offset;
	}	
	return val;	
}

int Vibe::GetRandom(int istart,int iend)
{
	int val=istart+rand()%(iend-istart);
	return val;
}


Vibe::~Vibe(void)
{
	if (g_ForeImg)
	{
		cvReleaseImage(&g_ForeImg);
	}
	if (g_SegementMask)
	{
		cvReleaseImage(&g_SegementMask);
	}
	if (g_UpdateMask)
	{
		cvReleaseImage(&g_UpdateMask);
	}
	if (g_Gray)
	{
		cvReleaseImage(&g_Gray);
	}

	if (g_Model!=NULL)
	{
		delete[]g_Model;
		g_Model=NULL;
	}
}
```



最后附上调用的main函数；











**[cpp]**[view plain](http://blog.csdn.net/kezunhai/article/details/9190923#)[copy](http://blog.csdn.net/kezunhai/article/details/9190923#)[print](http://blog.csdn.net/kezunhai/article/details/9190923#)[?](http://blog.csdn.net/kezunhai/article/details/9190923#)

- int _tmain(int argc, _TCHAR* argv[])  
- {     
-     CvCapture *capture=NULL;  
-     IplImage* frame=NULL;  
-     IplImage* pForeImg=NULL;  
-     IplImage* segImg=NULL;    
- 
- char *file_path="E:\\testVideo\\VTS_01_4.avi";  // m1  test2 锦带河  VTS_01_4_2  head rear  VTS_01_6_2  VTS_01_4
- //const char* file_path="E:\\suntektechvideo\\锦带河.avi";  //test2
- 
-     capture=cvCreateFileCapture(file_path);  
- if (!capture)  
-     {  
- //cout<<"Read Video File Error!"<<endl;
- return -1;  
-     }  
-     frame=cvQueryFrame(capture);  
-     frame=cvQueryFrame(capture);  
- 
-     cvNamedWindow("img",1);  
-     cvNamedWindow("foreN",1);  
- //cvNamedWindow("seg",1);
- 
-     Vibe* pV=new Vibe(frame);  
- 
- while(frame=cvQueryFrame(capture))  
-     {  
-         pV->Detect(frame);  
-         pForeImg=pV->GetForeground();  
- //segImg=pV->GetSegMask();
- //frame->origin=1;
- //pForeImg->origin=1;
-         cvShowImage("img",frame);  
-         cvShowImage("foreN",pForeImg);  
- //cvShowImage("seg",segImg);
-         cvWaitKey(1);  
-     }  
- 
-     cvReleaseImage(&frame);  
-     cvReleaseImage(&pForeImg);  
-     cvReleaseCapture(&capture);  
- return 0;     
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)


```cpp
int _tmain(int argc, _TCHAR* argv[])
{	
	CvCapture *capture=NULL;
	IplImage* frame=NULL;
	IplImage* pForeImg=NULL;
	IplImage* segImg=NULL;	

	char *file_path="E:\\testVideo\\VTS_01_4.avi";  // m1  test2 锦带河  VTS_01_4_2  head rear  VTS_01_6_2  VTS_01_4
	//const char* file_path="E:\\suntektechvideo\\锦带河.avi";  //test2

	capture=cvCreateFileCapture(file_path);
	if (!capture)
	{
		//cout<<"Read Video File Error!"<<endl;
		return -1;
	}
	frame=cvQueryFrame(capture);
	frame=cvQueryFrame(capture);

	cvNamedWindow("img",1);
	cvNamedWindow("foreN",1);
	//cvNamedWindow("seg",1);

	Vibe* pV=new Vibe(frame);

	while(frame=cvQueryFrame(capture))
	{
		pV->Detect(frame);
		pForeImg=pV->GetForeground();
		//segImg=pV->GetSegMask();
		//frame->origin=1;
		//pForeImg->origin=1;
		cvShowImage("img",frame);
		cvShowImage("foreN",pForeImg);
		//cvShowImage("seg",segImg);
		cvWaitKey(1);
	}

	cvReleaseImage(&frame);
	cvReleaseImage(&pForeImg);
	cvReleaseCapture(&capture);
	return 0;	
}
```



代码没做过多的注释，但现有的注释应该对于理解代码足够了。另外，对于计算机视觉里的任何一种算法都不是万能的，VIBE也不例外，只能说VIBE相对其他算法有一定的优势，但是还是有相当的不足，其pixel-wise-based的灰度建模方式解决不了pixel-wise建模算法共有的问题，其他必要辅助信息的融合是必要的。
﻿﻿

﻿﻿

﻿﻿



