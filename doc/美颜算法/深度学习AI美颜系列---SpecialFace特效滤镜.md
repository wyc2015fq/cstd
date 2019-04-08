# 深度学习AI美颜系列---SpecialFace特效滤镜

2018年09月19日 18:00:15 [Trent1985](https://me.csdn.net/Trent1985) 阅读数：932



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/82775829

SpecialFace滤镜这个名字实际上是本人自己起的，因为这个滤镜是一种比较另类的，人脸美化特效，所以给了这个名字。先看一下效果：

![img](https://img-blog.csdn.net/2018091917461830?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

![img](https://img-blog.csdn.net/20180919174638373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面我们来分析一下这个滤镜效果的算法逻辑：

1，设计一张逼真的模版A，如下图所示：

![img](https://img-blog.csdn.net/20180919174941854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这张模版中有透明区域和白色区域以及花草区域三个区域组成；

同时，在A中标定出两个人眼眼睛的位置E1和E2。

2，对人物照片原图S进行人脸检测与特征点识别；

这里我们只需要两个眼睛的点位SE1和SE2即可，可以直接使用腾讯或者Face++开源人脸特征点检测。

这里给出一个开源的人脸检测+对齐(68点位)的资源链接：<https://github.com/XiuSdk/cnn-facial-landmark>

![img](https://img-blog.csdn.net/20180919175320534?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个开源代码是68个点的，没有眼睛中心点，但是可以根据眼睛轮廓点计算出眼睛中心点。

3，根据人眼特征点位置，将A放置到S的人脸区域，A和S中的人眼位置对齐；

这一步算法是仿射变换，将E1和E2对齐到SE1和SE2上，这样A即对齐到了S中。

仿射变换公式如下：

![img](https://img-blog.csdn.net/20180919175123106?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在OPENCV中有API接口调用：

```cpp
void cv::warpAffine ( InputArray src, OutputArray dst, InputArray M, Size dsize, int flags = INTER_LINEAR, int borderMode = BORDER_CONSTANT, const Scalar & borderValue = Scalar() )
```

4，对A的三个区域分别处理：

①A中透明区域显示S中的内容；

②A中花草区域填充到S中对应位置；

③A中的白色区域显示对应S中的人脸区域，并对该区域像素进行右向的平移，平移距离OFFSET的大小，反映了图像中人脸裂开的程度；

以①-③的步骤将A和S进行融合，即得到该滤镜的效果图。

用类似的方法可以做出很多另类的滤镜风格，滤镜的真实度与模版的逼真度相关，所以模版的设计要求比较高。

本算法核心代码如下：

```cpp
int TriangleProcess(Triangle srcTriangle, unsigned char* srcData, int width ,int height, int stride, Triangle dstTriangle, unsigned char* dstData, int dWidth, int dHeight, int dStride, unsigned char* pFaceLabel, int ratio)
{
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	int i, j, pos, mw, mh, nx, ny, r, g, b, a, k, nk, temp;
	float px = 0, py = 0, cx, cy, AR, AG, AB, AA;
	int x0, y0, index_x0y0, index_x0y1, index_x1y0, index_x1y1;
	float disX, disY, tmp1, tmp2;
	TPoint curPoint;
	TPoint A =srcTriangle.A;
	TPoint B = srcTriangle.B;
	TPoint C = srcTriangle.C;
	px = MIN2(A.x, MIN2(B.x, C.x));
	py = MIN2(A.y, MIN2(B.y, C.y));
	mw = (int)(MAX2(A.x,MAX2(B.x, C.x)) - px);
	mh = (int)(MAX2(A.y,MAX2(B.y, C.y)) - py);
	k = ratio * 255 / 100;
	nk = 255 - k;
 
		for(j = 0; j < mh; j++)
	    {
	    	for(i = 0; i < mw; i++)
	    	{			
	    		curPoint.x = CLIP3(i + px, 0, width - 1);
	    		curPoint.y = CLIP3(j + py, 0, height - 1);			
	    		if (WARP_JudgePointInTriangleOrNot(curPoint,srcTriangle))
	    		{				
					if(pFaceLabel[(int)curPoint.x + (int)curPoint.y * width] == 255)
						continue;
	    			TriangleTransform(curPoint, srcTriangle,dstTriangle, &cx, &cy);
	    			x0 = (int)CLIP3(floor(cx), 0, dWidth - 2);
	    			y0 = (int)CLIP3(floor(cy), 0, dHeight - 2);
	    			index_x0y0 = (x0 << 2) + y0 * dStride;
	    			index_x1y0 = index_x0y0 + 4;
	    			index_x0y1 = index_x0y0 + dStride;
	    			index_x1y1 = index_x0y1 + 4;
	    			disX = cx - x0; 
	    			disY = cy - y0;
	    
	    			tmp1 = dstData[index_x0y0] + disX *(dstData[index_x1y0] - dstData[index_x0y0]);
	    			tmp2 = dstData[index_x0y1] + disX *(dstData[index_x1y1] - dstData[index_x0y1]);
	    			AB = tmp1 + disY * (tmp2 - tmp1);
	    
	    			tmp1 = dstData[index_x0y0 + 1] + disX *(dstData[index_x1y0 + 1] - dstData[index_x0y0 + 1]);
	    			tmp2 = dstData[index_x0y1 + 1] + disX *(dstData[index_x1y1 + 1] - dstData[index_x0y1 + 1]);
	    			AG = tmp1 + disY * (tmp2 - tmp1);
	    
	    			tmp1 = dstData[index_x0y0 + 2] + disX *(dstData[index_x1y0 + 2] - dstData[index_x0y0 + 2]);
	    			tmp2 = dstData[index_x0y1 + 2] + disX *(dstData[index_x1y1 + 2] - dstData[index_x0y1 + 2]);
	    			AR = tmp1 + disY * (tmp2 - tmp1);
	    
	    			tmp1 = dstData[index_x0y0 + 3] + disX *(dstData[index_x1y0 + 3] - dstData[index_x0y0 + 3]);
	    			tmp2 = dstData[index_x0y1 + 3] + disX *(dstData[index_x1y1 + 3] - dstData[index_x0y1 + 3]);
	    			AA = tmp1 + disY * (tmp2 - tmp1);
	    
	    			nx = (int)CLIP3(i + px, 0, width - 1);
	    			ny = (int)CLIP3(j + py, 0, height - 1);
	    			pos = (nx << 2) + ny * stride;
	    
					r = (int)AR;
					g = (int)AG;
					b = (int)AB;
					a = (int)AA;
					temp = 255 - a;
					if(b > 220 && g > 220 && r > 220 && a != 0)
					{
						srcData[pos + 3] = 0;
					}
 
					b = CLIP3((b * a + temp * srcData[pos + 0]) / 255, 0, 255);
	    			g = CLIP3((g * a + temp * srcData[pos + 1]) / 255, 0, 255);
	    			r = CLIP3((r * a + temp * srcData[pos + 2]) / 255, 0, 255);
 
					
 
					srcData[pos + 0] = CLIP3((b * k + nk * srcData[pos + 0]) >> 8, 0, 255);
	    			srcData[pos + 1] = CLIP3((g * k + nk * srcData[pos + 1]) >> 8, 0, 255);
	    			srcData[pos + 2] = CLIP3((r * k + nk * srcData[pos + 2]) >> 8, 0, 255);
					
	    			//srcData[pos + 3] = 255;/////////////////////////////////////////////////////////////
					pFaceLabel[(int)curPoint.x + (int)curPoint.y * width] = 255;
 
	    		}
	    	}
	    }
	free(tempData);
	return 0;
}
int f_FaceMesh(unsigned char* srcData, int width ,int height, int stride, unsigned char* maskData, int mWidth, int mHeight, int mStride, int srcFacePointsAll[101 * 2], int mskFacePointsAll[101 * 2 + 8 * 2], int ratio, int method)
 {
	 int ret = 0;
	 Triangle mskTriangle[191], srcTriangle[191];
	 GetTriangle(mskFacePointsAll,mskTriangle);
	 int maxx = 0, maxy = 0, minx = 10000, miny = 10000;
	 for(int i = 0; i < 101; i++)
	 {
	 	maxx = maxx > srcFacePointsAll[(i << 1)] ? maxx : srcFacePointsAll[(i << 1)];
	 	maxy = maxy > srcFacePointsAll[(i << 1) + 1] ? maxy : srcFacePointsAll[(i << 1) + 1];
	 	minx = minx < srcFacePointsAll[(i << 1)] ? minx : srcFacePointsAll[(i << 1)];
	 	miny = miny < srcFacePointsAll[(i << 1) + 1] ? miny : srcFacePointsAll[(i << 1) + 1];
	 }
	 int tx, ty, roiWidth;
	 tx = minx;
	 ty = miny;
	 roiWidth = MAX2(abs(maxx - minx),abs(maxy - miny));
	 int roiHeight = roiWidth;
	 int k = 2;
	 int px = MAX2(tx - roiWidth / k, 0);
	 int py = MAX2(ty - roiWidth / k, 0);
	 roiWidth = CLIP3(tx - px + roiWidth + roiWidth / k, 0, width - 1);
	 roiHeight = CLIP3(ty - py + roiHeight + roiWidth / k, 0, height - 1);
	 int rw = MIN2(roiWidth, roiHeight);
	 roiWidth = roiHeight = rw;
	 tx = px;
	 ty = py;
	 int facePointsAll[202 + 16];
	 for(int i = 0; i < 202; i++)
	 {
		 facePointsAll[i] = srcFacePointsAll[i];
	 }
	 facePointsAll[202] = tx;
	 facePointsAll[203] = ty;
 
	 facePointsAll[204] = tx;
	 facePointsAll[205] = ty + (roiHeight >> 1);
 
	 facePointsAll[206] = tx;
	 facePointsAll[207] = ty + roiHeight;
 
	 facePointsAll[208] = tx + (roiWidth >> 1);
	 facePointsAll[209] = ty + roiHeight;
 
	 facePointsAll[210] = tx + roiWidth;
	 facePointsAll[211] = ty + roiHeight;
 
	 facePointsAll[212] = tx + roiWidth;
	 facePointsAll[213] = ty + (roiHeight >> 1);
 
	 facePointsAll[214] = tx + roiWidth;
	 facePointsAll[215] = ty;
 
	 facePointsAll[216] = tx + (roiWidth >> 1);
	 facePointsAll[217] = ty;
	 GetTriangle(facePointsAll, srcTriangle);
	 unsigned char* pFaceLabel = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	 memset(pFaceLabel, 0, sizeof(unsigned char) * width * height);
     for(int i = 0; i < 191-15; i++)
	 {
	   TriangleProcess(srcTriangle[i], srcData, width ,height, stride, mskTriangle[i], maskData, mWidth, mHeight, mStride, pFaceLabel, ratio);
	 }
	 free(pFaceLabel);
	 return ret;
 };
//SpecialFace filter api
int f_Test(unsigned char* srcData, int width ,int height, int stride, unsigned char* maskData, int mWidth, int mHeight, int mStride, int srcFacePointsAll[101 * 2], int mskFacePointsAll[101 * 2 + 8 * 2], int ratio)
{
	int ret = 0;
	unsigned char* pMask = maskData;
	for(int j = 0; j < mHeight; j++)
	{
		for(int i = 0; i < mWidth; i++)
		{
			if(pMask[3] == 0)
				pMask[0] = pMask[1] = pMask[2] = 0;
			pMask += 4;
		}
	}
	//Process
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	ret = f_FaceMesh(srcData, width, height, stride, maskData, mWidth, mHeight, mStride, srcFacePointsAll, mskFacePointsAll, ratio, 0);
	int nx = 0, ny = 0, pos = 0, dis;
	unsigned char* pSrc = srcData;
	dis = srcFacePointsAll[2 * 51] - srcFacePointsAll[2 * 45];
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			if(pSrc[3] == 0)
			{
				nx = (int)CLIP3(i - dis, 0, width - 1);
				ny = (int)CLIP3(j, 0, height - 1);
				pos = (nx << 2) + ny * stride;
				pSrc[0] = (int)tempData[pos + 0];
				pSrc[1] = (int)tempData[pos + 1];
				pSrc[2] = (int)tempData[pos + 2];
				pSrc[3] = 255;
			}
			pSrc += 4;
	    
		}
	}
	free(tempData);
	return ret;
};
```

本文算法的DEMO连接：[SpecialFace Filter](https://download.csdn.net/download/trent1985/10677114)

最后，本人QQ1358009172，微信公众号：SF图像算法