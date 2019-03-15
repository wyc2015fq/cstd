# 深度学习AI美颜系列---AI人像美妆算法初识

2018年07月24日 16:25:16

 

Trent1985

 

阅读数：3496

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/81185763

人像美妆是近几年来深受广大女孩儿群体喜欢的修图功能之一，目前市面中做的比较好的有美妆相机、玩美彩妆、天天P图等APP，当然还有一些PC专用的秀图软件，本文将给大家做个算法初识；

什么是人像美妆？通俗的看个样例图：

![img](https://img-blog.csdn.net/20180724154933173?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这个图中，由左边的原图，到右边的化妆效果图，就叫做人像美妆。

本人对AI美妆的一些看法如下：

1.妆容自然，逼真；

2.鲁棒性高，不受五官遮挡影响；

3.速度越快越好；

4.完全智能化，可以针对不同人像照片智能匹配最优妆容；

目前传统美妆的优缺点：

优点：

1.妆容种类丰富，可自由搭配，用户自主选择；

美妆相机和玩美彩妆两款App均提供了数十种不同的妆容效果，供用户自由选择；

2.上妆速度快，可以实时处理；

玩美彩妆、美妆相机、天天P图、无他相机、FaceU等APP均已支持实时上妆效果；

缺点：

1.妆容鲁棒性不高，被光线，五官遮挡影响较大；

2.逼真度不高；

3.无法完全智能化；

目前市面基于传统算法的美妆类APP均无法达到上述3点；

传统人像美妆算法流程：

1.妆容模版制作(Photoshop等编辑软件制作，由设计完成)

2.人脸检测，特征点识别；

这一步骤主要通过人脸检测+人脸对齐来获得N个特征点，目前开源的有Dlib，OpenCV等，商用版有商汤科技、旷世科技、虹软科技等，以及腾讯、美图等；

这里给出一个开源的人脸检测+对齐(68点位)的资源链接：<https://github.com/XiuSdk/cnn-facial-landmark>

![img](https://img-blog.csdn.net/20180724160540943?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.基于人脸特征点，将模版变形，对齐到人脸五官区域；

变形算法有很多，仿射变换，IDW变换，MLS变换，RMLS变换等；

相关代码连接：

[IDW](https://blog.csdn.net/trent1985/article/details/79565274)

[RMLS](https://blog.csdn.net/trent1985/article/details/79566052)

[MLS](https://blog.csdn.net/Trent1985/article/details/80667611)(MLS代码在博客内容中)

如果大家懒得看博客，这里给出MLS变形代码：

```cpp
static void setSrcPoints(const vector<PointD> &qsrc, vector<PointD> &newDotL, int* nPoint) {
    *nPoint = qsrc.size();
    newDotL.clear();
    newDotL.reserve(*nPoint);
    for (size_t i = 0; i < qsrc.size(); i++) 
        newDotL.push_back(qsrc[i]);
}
 
static void setDstPoints(const vector<PointD> &qdst,vector<PointD> &oldDotL, int* nPoint) {
    *nPoint = qdst.size();
    oldDotL.clear();
    oldDotL.reserve(*nPoint);
 
    for (size_t i = 0; i < qdst.size(); i++) oldDotL.push_back(qdst[i]);
}
static double bilinear_interp(double x, double y, double v11, double v12,
                              double v21, double v22) {
    return (v11 * (1 - y) + v12 * y) * (1 - x) + (v21 * (1 - y) + v22 * y) * x;
}
 
static double calcArea(const vector<PointD> &V) {
    PointD lt, rb;
    lt.x = lt.y = 1e10;
    rb.x = rb.y = -1e10;
    for (vector<PointD >::const_iterator i = V.begin(); i != V.end();
         i++) {
        if (i->x < lt.x) lt.x = i->x;
        if (i->x > rb.x) rb.x = i->x;
        if (i->y < lt.y) lt.y = i->y;
        if (i->y > rb.y) rb.y = i->y;
    }
    return (rb.x - lt.x) * (rb.y - lt.y);
}
static void calcDelta_rigid(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;
    PointD swq, qstar, newP, tmpP;
    double sw;
 
    double ratio;
 
    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];
 
    if (nPoint < 2) {
        //rDx.setTo(0);
        //rDy.setTo(0);
        return;
    }
    PointD swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    double miu_r;
 
    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                if (alpha == 1)
                    w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                (j - oldDotL[k].y) * (j - oldDotL[k].y));
                else
                    w[k] = pow((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                   (j - oldDotL[k].y) * (j - oldDotL[k].y),
                               -alpha);
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
                swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
				pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_r
                double s1 = 0, s2 = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					Qi.x = newDotL[k].x - qstar.x;
					Qi.y = newDotL[k].y - qstar.y;
					s1 += w[k] * (Qi.x*Pi.x+Qi.y*Pi.y);
                    s2 += w[k] * (Qi.x*PiJ.x+Qi.y*PiJ.y);
                }
                miu_r = sqrt(s1 * s1 + s2 * s2);
				curV.x -= pstar.x;
				curV.y -= pstar.y;
 
                curVJ.x = -curV.y, curVJ.y = curV.x;
 
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					 Pi.x = oldDotL[k].x - pstar.x;
					  Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					 tmpP.x = (Pi.x*curV.x+Pi.y*curV.y)* newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y)* newDotL[k].y;
                    tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_r;
					tmpP.y *= w[k] / miu_r;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
				newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }
 
            if (preScale) {
                rDx[j * tarW + i] = newP.x * ratio - i;
                rDy[j * tarW + i] = newP.y * ratio - j;
            } else {
				rDx[j * tarW + i] = newP.x - i;
				rDy[j * tarW + i] = newP.y - j;
            }
        }
    }
    delete[] w;
 
    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static void calcDelta_Similarity(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;
 
    PointD swq, qstar, newP, tmpP;
    double sw;
	
    double ratio;
 
    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];
 
    if (nPoint < 2) {
        return;
    }
 
    PointD swp, pstar, curV, curVJ, Pi, PiJ;
    double miu_s;
 
    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                            (j - oldDotL[k].y) * (j - oldDotL[k].y));
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
				swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
                pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_s
                miu_s = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
 
                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    miu_s += w[k] * (Pi.x*Pi.x+Pi.y*Pi.y);
                }
 
                curV.x -= pstar.x;
				curV.y -= pstar.y;
                curVJ.x = -curV.y, curVJ.y = curV.x;
 
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
 
                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
 
					tmpP.x = (Pi.x*curV.x+Pi.y*curV.y) * newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y) * newDotL[k].y;
					tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_s;
					tmpP.y *= w[k] / miu_s;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
                newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }
 
            rDx[j * tarW + i] = newP.x - i;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
            rDy[j * tarW + i] = newP.y - j;
        }
    }
 
    delete[] w;
	    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static int GetNewImg(unsigned char* oriImg, int width, int height, int stride, unsigned char* tarImg, int tarW, int tarH, int tarStride, int gridSize, double* rDx, double* rDy, double transRatio)
{
    int i, j;
    double di, dj;
    double nx, ny;
    int nxi, nyi, nxi1, nyi1;
    double deltaX, deltaY;
    double w, h;
    int ni, nj;
	int pos, posa, posb, posc, posd;
    for (i = 0; i < tarH; i += gridSize)
        for (j = 0; j < tarW; j += gridSize) {
            ni = i + gridSize, nj = j + gridSize;
            w = h = gridSize;
            if (ni >= tarH) ni = tarH - 1, h = ni - i + 1;
            if (nj >= tarW) nj = tarW - 1, w = nj - j + 1;
            for (di = 0; di < h; di++)
                for (dj = 0; dj < w; dj++) {
                    deltaX =
                        bilinear_interp(di / h, dj / w, rDx[i * tarW + j], rDx[i * tarW + nj],
                                        rDx[ni * tarW + j], rDx[ni * tarW + nj]);
                    deltaY =
                        bilinear_interp(di / h, dj / w, rDy[i * tarW + j], rDy[i * tarW + nj],
                                        rDy[ni * tarW + j], rDy[ni * tarW + nj]);
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    if (nx > width - 1) nx = width - 1;
                    if (ny > height - 1) ny = height - 1;
                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);
					pos = (int)(i + di) * tarStride + ((int)(j + dj) << 2);
					posa = nyi * stride + (nxi << 2);
					posb = nyi * stride + (nxi1 << 2);
					posc = nyi1 * stride + (nxi << 2);
					posd = nyi1 * stride + (nxi1 << 2);
					tarImg[pos]     = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa],    oriImg[posb],   oriImg[posc],   oriImg[posd]);
					tarImg[pos + 1] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 1],oriImg[posb + 1], oriImg[posc + 1], oriImg[posd + 1]);
					tarImg[pos + 2] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 2],oriImg[posb + 2], oriImg[posc + 2], oriImg[posd + 2]);
					tarImg[pos + 3] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 3],oriImg[posb + 3], oriImg[posc + 3], oriImg[posd + 3]);
                }
        }
		return 0;
};
 
static void MLSImageWrapping(unsigned char* oriImg,int width, int height, int stride,const vector<PointD > &qsrc, const vector<PointD > &qdst, unsigned char* tarImg, int outW, int outH, int outStride, double transRatio, int preScale, int gridSize, int method)
{
	int srcW = width;
	int srcH = height;
	int tarW = outW;
    int tarH = outH;
    double alpha = 1;
	int nPoint;
	int len = tarH * tarW;
	vector<PointD> oldDotL, newDotL;
	double *rDx = NULL,*rDy = NULL;
    setSrcPoints(qsrc,newDotL,&nPoint);
    setDstPoints(qdst,oldDotL,&nPoint);
    rDx = (double*)malloc(sizeof(double) * len);
    rDy = (double*)malloc(sizeof(double) * len);
    memset(rDx, 0, sizeof(double) * len);
    memset(rDy, 0, sizeof(double) * len);
	if(method!=0)
		calcDelta_Similarity(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	else
	    calcDelta_rigid(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	GetNewImg(oriImg, srcW, srcH, stride, tarImg, tarW, tarH, outStride, gridSize, rDx, rDy, transRatio);
	if(rDx != NULL)
		free(rDx);
	if(rDy != NULL)
		free(rDy);
};
int f_TMLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, unsigned char* dstData, int outW, int outH, int outStride, int srcPoint[], int dragPoint[], int pointNum, double intensity, int preScale, int gridSize, int method)
{
	int res = 0;
    vector<PointD> qDst;
    vector<PointD> qSrc;
	PointD point = {0};
	int len = 0;
	for(int i = 0; i < pointNum; i++)
	{
		len = (i << 1);
		point.x = srcPoint[len];
		point.y = srcPoint[len + 1];
		qSrc.push_back(point);
		point.x = dragPoint[len];
		point.y = dragPoint[len + 1];
		qDst.push_back(point);
	}
	MLSImageWrapping(srcData, width, height, stride, qSrc, qDst, dstData, outW, outH, outStride, intensity, preScale,gridSize, method);
	return res;
};
```

4.将模版与人脸五官图像进行融合；

融合算法主要有alpha融合，Photoshop图层混合，泊松融合等；

alpha融合： S * alpha + D*(1-alpha)

图层混合公式如下：

![img](https://img-blog.csdn.net/2018072416102622?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

泊松融合：[算法详解](https://blog.csdn.net/baimafujinji/article/details/6485778)

上述过程即传统算法流程，其中对美妆效果起决定性的是人脸特征点识别，如果没有准确的特征点，再好的妆容模版，上妆效果也出不来；

比如下面的例子：

![img](https://img-blog.csdn.net/20180724153659555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/20180724153750963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/201807241538325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图1中，由于眼睛特征点位置不准确，睫毛妆容已经偏离了眼睛区域；

图2中，由于拍照光线较暗，腮红明显，逼真度过低；

图3中，由于人眼和眉毛被部分遮挡，因此，传统算法的睫毛和眉毛效果悬浮在了头发之上；

目前传统算法相关的论文资料如下：

Rule-Based Facial Makeup Recommendation System.

Example-Based Cosmetic Transfer.

Region-based Face Makeup using example face images.

Simulating Makeup through Physics-based Manipulation of Intrinsic Image Layers.

A Visual Representation for editing face images.

Digital Face Makeup By Example.

在传统算法中，有一种妆容迁移算法，该算法可以直接将一张妆容效果图中的妆容特征，迁移到任意一张人像照片中去，实际上也是与人脸特征点密不可分，具体连接可参考：<https://blog.csdn.net/trent1985/article/details/70226779>

目前AI美妆相关的论文资料如下：

Makeup Like a Superstar Deep Localized Makeup Transfer Network.

Examples-Rules Guided Deep Neural Network for Makeup Recommendation.

上述两篇基于深度学习的美妆算法论文主要思想有两个：

1，对于第一篇论文主要是对人像进行五官分析，获取肤色，眉毛颜色，唇色等等信息，然后进行不同妆容的最佳匹配，最后上妆；

框架如下：

![img](https://img-blog.csdn.net/20180724162036668?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2，对五官进行分别提取分类成不同的style，依据样例数据的特征style，进行最优匹配并上妆；

框架如下：

![img](https://img-blog.csdn.net/20180724162118485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上述两篇算法论文，依旧是建立在人脸特征点的基础上研究的。

**本人针对传统美妆算法，结合深度学习，做了如下改进：**

**1.只需要人脸检测框，不依赖于人脸特征点；**

**2.不受五官遮挡和光线影响；**

**3.妆容效果逼真度提高；**

本人算法框架：

1.人脸检测，得到正方形人脸框，包含五官区域；

2.基于全卷积网络，以人脸框图像作为输入，上妆之后的人脸五官效果图作为输出，进行学习训练；

妆容模版使用如下模版：

![img](https://img-blog.csdn.net/2018072415395829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在Fig.4模板妆容中，分别进行了眉毛处理，眼影、睫毛和唇彩的上妆，整体肤色以及其他内容均无调整；

训练中迭代10次，训练集和验证集准确率均达到了94%-95%，本人训练样本选取了500张，数据比较少，这里仅仅探讨可行性与方法分析；

3.使用2中的训练模型，对测试图进行上妆；

效果图如下：

![img](https://img-blog.csdn.net/20180724154149939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/20180724154257162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上述效果图中我们可以看到，基于深度学习的美妆效果，避免了五官遮挡的影响，同时上妆效果更加自然，对环境光的鲁棒性也较高，本文这里未给出具体的网络模型与参数，不过思路大家已经可以借鉴！目前算法处于研究测试阶段，后续本人将公布完整的DEMO！

本人QQ：1358009172