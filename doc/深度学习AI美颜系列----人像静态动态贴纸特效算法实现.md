# 深度学习AI美颜系列----人像静态/动态贴纸特效算法实现

置顶

 

2019年01月02日 17:00:13

 

Trent1985

 

阅读数：1095

更多

个人分类： [深度学习AI美颜系列](https://blog.csdn.net/Trent1985/article/category/7651523)

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/85625120

人像静态/动态贴纸特效几乎已经是所有图像视频处理类/直播类app的必需品了，这个功能看起来复杂，实际上很简单，本文将给大家做个详细的讲解。

我们先来看一下FaceU的两张效果图：

![img](https://img-blog.csdnimg.cn/20190102163459787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)  

​        这两张效果图中， 我们可以看到一些可爱的圣诞帽贴纸和小鹿形象贴纸，在人脸区域，自动贴上不同的贴纸，便会生成很多有趣的特效，这也是大家爱不释手的原因。

​        我们从静态贴纸讲起，动态贴纸实际上是静态贴纸的循环播放而已。人像静态贴纸的鼻祖是in app，而后FaceU的动态贴纸风靡一时，现在静态/动态贴纸已经随处可见了。它的算法分为两个部分：

1，人脸检测与关键点识别；

​        所谓人像贴纸，第一步必然是人脸检测与关键点识别。在有人脸的情况下获取到必须的关键点，这样我们才能准确的打上贴纸。现有的app中，比如FaceU和轻颜相机，当然他们是一家的，他们的贴纸基本上都是数十个人脸特征点的模板配置，也就是说，完成一个贴纸效果，需要数十个点位信息的支持。不过，个人认为，最简单的贴纸，可以从三个点开始。

​        关于人脸特征点识别算法，相关的论文或者开源的代码工程，已经多如牛毛，这里我不一一例举，本人这里以mtcnn为例，因为本文算法只需要三个点即可。mtcnn在检测到人脸的情况下，会给我们提供5个特征点，如下图所示：

![img](https://img-blog.csdnimg.cn/20190102163828473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

​        mtcnn的算法详解大家也可以在网络资源中自行学习，这里我们仅仅给出网络结构示意图，如图所示。

![img](https://img-blog.csdnimg.cn/20190102163929341.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)   

​        对于上述三个网络，分别为三个阶段：

Stage 1：使用P-Net是一个全卷积网络，用来生成候选窗和边框回归向量(bounding box regression vectors)。使用Bounding box regression的方法来校正这些候选窗，使用非极大值抑制（NMS）合并重叠的候选框；

Stage 2：使用N-Net改善候选窗，将通过P-Net的候选窗输入R-Net中，拒绝掉大部分false的窗口，继续使用Bounding box regression和NMS合并；

Stage 3：使用O-Net输出最终的人脸框和特征点位置。和第二步类似，但是不同的是生成5个特征点位置；

​        本文使用NCNN调用MTCNN的模型，上层采用C#调用，代码如下：

```cpp
/*************************************************
Copyright:  Copyright Trent.
Author:		Trent
Date:		2015-03-09
Description:MTCNN.
**************************************************/
 
 
#include"Trent.h"
 
#include "mtcnn.h"
#include <stdint.h>
static char* model_path = "C:/Users/Administrator/Desktop/mtcnn/001_TestDemo/TestDemo/TestDemo_C/models";
MTCNN* mtcnn;
 
 
int FD_Initialize()
{
	mtcnn = new MTCNN(model_path);
	return 0;
};
int FD_Process(unsigned char* srcData, int width, int height, int stride, int faceInfos[15])
{
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * height * width * 3);
	unsigned char* pSrc = srcData;
	unsigned char* pData = data;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			pData[0] = pSrc[0];
			pData[1] = pSrc[1];
			pData[2] = pSrc[2];
			pData += 3;
			pSrc += 4;
		}
 
	}
	ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(data, ncnn::Mat::PIXEL_RGB, width, height);
	std::vector<Bbox> finalBbox;
	mtcnn->detect(ncnn_img, finalBbox);
	if(finalBbox.size() > 0)
	{
		faceInfos[0] = 1;
		faceInfos[1] = finalBbox[0].x1;
		faceInfos[2] = finalBbox[0].y1;
		faceInfos[3] = finalBbox[0].x2 - finalBbox[0].x1;
		faceInfos[4] = finalBbox[0].y2 - finalBbox[0].y1;
		faceInfos[5] = finalBbox[0].ppoint[0];
		faceInfos[6] = finalBbox[0].ppoint[5];
		faceInfos[7] = finalBbox[0].ppoint[1];
        faceInfos[8] = finalBbox[0].ppoint[6];
        faceInfos[9] = finalBbox[0].ppoint[2];
        faceInfos[10] = finalBbox[0].ppoint[7];
		faceInfos[11] = finalBbox[0].ppoint[3];
		faceInfos[12] = finalBbox[0].ppoint[8];
		faceInfos[13] = finalBbox[0].ppoint[4];
		faceInfos[14] = finalBbox[0].ppoint[9];
	}
	free(data);
	return 0;
};
void FD_Unitialize()
{
	if(model_path != NULL)
		free(model_path);
	delete(mtcnn);
}
```

​        拿到五个人脸特征点，这一步我们就完成了。

2，贴纸融合；

有了特征点，我们如何将贴纸融合到恰当的位置？

①计算基准点

我们从5个特征点中，计算得到三个关键点A,B,C；

A为左眼中心点；

B为右眼中心点；

C为嘴巴水平中心点；

对于一张人脸，它的这三个点变化比较小，同时又可以覆盖整个脸部区域，因此，具有整张脸的代表性。

②构建模特基准点

选取一张模特图，要求五官端正，比例协调，如下图所示；

![img](https://img-blog.csdnimg.cn/20190102164143830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

在图中标定出三个人脸关键点位置A,B,C，如蓝色点所示，并记录位置信息；

③构建贴纸模板

我们使用如下两个贴纸，在PS中将贴纸放置到模特脸上合适的位置，然后保存两个贴纸模板为mask_a,mask_b，这样两个贴纸模板就制作完成了，模板如下图所示：

![img](https://img-blog.csdnimg.cn/20190102164303119.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

③贴图

对于任意一张用户照片，先使用MTCNN得到人脸5个关键点，计算出人脸关键点A,B,C，我们记做A0,B0,C0；

然后，使用仿射变换，公式如下图所示所示，将A0,B0,C0映射到模特图A,B,C三点，得到仿射变换矩阵H；

![img](https://img-blog.csdnimg.cn/20190102164519445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

仿射变换H矩阵求解代码如下：

```cpp
void GetTexTransMatrix(float x1, float y1, float x2, float y2, float x3, float y3,float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float*texMatrix)
{
	float detA;
	detA = tx1*ty2 + tx2*ty3 + tx3*ty1 - tx3*ty2 - tx1*ty3 - tx2*ty1;
	float A11, A12, A13, A21, A22, A23, A31, A32, A33;
	A11 = ty2 - ty3;
	A21 = -(ty1 - ty3);
	A31 = ty1 - ty2;
	A12 = -(tx2 - tx3);
	A22 = tx1 - tx3;
	A32 = -(tx1 - tx2);
	A13 = tx2*ty3 - tx3*ty2;
	A23 = -(tx1*ty3 - tx3*ty1);
	A33 = tx1*ty2 - tx2*ty1;  
	texMatrix[0] = (x1*A11 + x2*A21 + x3*A31) / detA;  
	texMatrix[1] = (x1*A12 + x2*A22 + x3*A32) / detA;  
	texMatrix[2] = (x1*A13 + x2*A23 + x3*A33) / detA;  
	texMatrix[3] = (y1*A11 + y2*A21 + y3*A31) / detA; 
	texMatrix[4] = (y1*A12 + y2*A22 + y3*A32) / detA;       
	texMatrix[5] = (y1*A13 + y2*A23 + y3*A33) / detA;  
}
```

最后，根据H遍历用户照片，将用户照片像素映射到贴纸模板mask_a或者mask_b中，根据贴纸模板像素的透明度进行alpha混合，即可得到最终效果，如下图所示；

![img](https://img-blog.csdnimg.cn/20190102164654147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

最后给出完整的自动人像贴纸代码，仅仅50行左右，即可实现静态图贴纸效果，如果大家需要动态贴纸，可以循环播放不同贴纸即可，代码如下：

```cpp
#include"Trent_Sticker.h"
#include"Trent.h"
void GetTexTransMatrix(float x1, float y1, float x2, float y2, float x3, float y3,float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float*texMatrix)
{
	float detA;
	detA = tx1*ty2 + tx2*ty3 + tx3*ty1 - tx3*ty2 - tx1*ty3 - tx2*ty1;
	float A11, A12, A13, A21, A22, A23, A31, A32, A33;
	A11 = ty2 - ty3;
	A21 = -(ty1 - ty3);
	A31 = ty1 - ty2;
	A12 = -(tx2 - tx3);
	A22 = tx1 - tx3;
	A32 = -(tx1 - tx2);
	A13 = tx2*ty3 - tx3*ty2;
	A23 = -(tx1*ty3 - tx3*ty1);
	A33 = tx1*ty2 - tx2*ty1;  
	texMatrix[0] = (x1*A11 + x2*A21 + x3*A31) / detA;  
	texMatrix[1] = (x1*A12 + x2*A22 + x3*A32) / detA;  
	texMatrix[2] = (x1*A13 + x2*A23 + x3*A33) / detA;  
	texMatrix[3] = (y1*A11 + y2*A21 + y3*A31) / detA; 
	texMatrix[4] = (y1*A12 + y2*A22 + y3*A32) / detA;       
	texMatrix[5] = (y1*A13 + y2*A23 + y3*A33) / detA;  
}
int Trent_Sticker(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int maskWidth, int maskHeight, int maskStride, int srcFacePoints[6], int maskFacePoints[6], int ratio)
{
	int ret = 0;
	float H[6];
	GetTexTransMatrix(maskFacePoints[0], maskFacePoints[1], maskFacePoints[2], maskFacePoints[3], maskFacePoints[4], maskFacePoints[5], srcFacePoints[0], srcFacePoints[1], srcFacePoints[2], srcFacePoints[3], srcFacePoints[4], srcFacePoints[5], H);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			float x = (float)i;
			float y = (float)j;
			float tx = 0;
			float ty = 0;
			tx = (int)((H[0] * (x)+H[1] * (y)+H[2]) + 0.5);
			ty = (int)((H[3] * (x)+H[4] * (y)+H[5]) + 0.5);
			tx = CLIP3(tx, 0, maskWidth - 1);
			ty = CLIP3(ty, 0, maskHeight - 1);			
			int mb = mask[(int)tx * 4 + (int)ty * maskStride];
			int mg = mask[(int)tx * 4 + (int)ty * maskStride + 1];
			int mr = mask[(int)tx * 4 + (int)ty * maskStride + 2];
			int alpha = mask[(int)tx * 4 + (int)ty * maskStride + 3];
			int b = srcData[i * 4 + j * stride];
			int g = srcData[i * 4 + j * stride + 1];
			int r = srcData[i * 4 + j * stride + 2];			
			srcData[(int)i * 4 + (int)j * stride] = CLIP3((b * (255 - alpha) + mb * alpha) / 255, 0, 255);
			srcData[(int)i * 4 + (int)j * stride + 1] = CLIP3((g * (255 - alpha) + mg * alpha) / 255, 0, 255);
			srcData[(int)i * 4 + (int)j * stride + 2] = CLIP3((r * (255 - alpha) + mr * alpha) / 255, 0, 255);
		}
	}
	return ret;
};
```

最后，给出完整工程的源代码，包含MTCNN调用代码和静态贴纸代码：<https://download.csdn.net/download/trent1985/10891121>

注意，代码运行环境为VS2015，运行时需要把model_path修改为自己的本地路径即可。

DEMO源码界面如下：

![img](https://img-blog.csdnimg.cn/20190102165904404.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

本人正在参加2018博客之星，顺便拉个选票，如果觉得本文写的还凑合，还望拉个票，博客之星22号，Trent1985,谢谢！地址：[博客之星](https://bss.csdn.net/m/topic/blog_star2018/index?from=groupmessage#10006-weixin-1-52626-6b3bffd01fdde4900130bc5a2751b6d1)

如有问题，欢迎联系，本人QQ1358009172

 