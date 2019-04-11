# 目标跟踪之Lukas-Kanade光流法

 





光流是图像亮度的运动信息描述。光流法计算最初是由Horn和Schunck于1981年提出的，创造性地将二维速度场与灰度相联系，引入光流约束方程，得到光流计算的基本算法．光流计算基于物体移动的光学特性提出了2个假设：

①运动物体的灰度在很短的间隔时间内保持不变；
 ②给定邻域内的速度向量场变化是缓慢的。

## 算法原理

假设图像上一个像素点(x,y)，在t时刻的亮度为E(x+Δx,y+Δy,t+Δt)，同时用u(x,y0和v(x,y)来表示该点光流在水平和垂直方向上的移动分量：

u=dx/dt

v=dy/dt

在经过一段时间间隔Δt后该点对应点亮度为E(x+Δx,y+Δy,t+Δt),当Δt很小趋近于0时，我们可以认为该点亮度不变，所以可以有：
 E(x,y,t)=E(x+Δx,y+Δy,t+Δt)
 当该点的亮度有变化时，将移动后点的亮度由Taylor公式展幵，可得：
![img](https://img-blog.csdn.net/20150625154944724)
 忽略其二阶无穷小，由于Δt趋近于0时，有：

![img](https://img-blog.csdn.net/20150625155058212)
 式中w=(u,v)，所以上式就是基本的光流约束方程。
 其中令![img](https://img-blog.csdn.net/20150625155240977)表示图像中像素点灰度沿x，y，t方向的梯度，可将上式改写成：
![img](https://img-blog.csdn.net/20150625155338065)



Lucas-Kanade是一种广泛使用的光流估计的差分方法，这个方法是由Bruce D. Lucas和Takeo Kanade发明的。它假设光流在像素点的邻域是一个常数，然后使用最小二乘法对邻域中的所有像素点求解基本的光流方程。
 通过结合几个邻近像素点的信息，卢卡斯-金出方法(简称为L-K方法)通常能够消除光流方程里的多义性。而且，与逐点计算的方法相比，L-K方法对图像噪声不敏感。不过，由于这是一种局部方法，所以在图像的均匀区域内部，L-K方法无法提供光流信息。

## Lucas-Kanade改进算法

Jean-Yves Bouguet提出一种基于金字塔分层，针对仿射变换的改进Lucas-Kanade算法。



为什么要用金字塔？因为lk算法的约束条件即：小速度，亮度不变以及区域一致性都是较强的假设，并不很容易得到满足。如当物体运动速度较快时，假设不成立，那么后续的假设就会有较大的偏差，使得最终求出的光流值有较大的误差。

考虑物体的运动速度较大时，算法会出现较大的误差。那么就希望能减少图像中物体的运动速度。一个直观的方法就是，缩小图像的尺寸。假设当图像为400×400时，物体速度为[16   16],那么图像缩小为200×200时，速度变为[8,8]。缩小为100*100时，速度减少到[4,4]。所以在源图像缩放了很多以后，原算法又变得适用了。所以光流可以通过生成  原图像的金字塔图像，逐层求解，不断精确来求得。简单来说上层金字塔（低分辨率）中的一个像素可以代表下层的两个。



假设I和J是两幅2D的灰度图像，对于图像上每个像素点的灰度值定义为：

I(x)=I(x,y)   和  J(x)=j(x,y)

其中x=(x,y)是图像上像素点的图像坐标。
 在实际场景中图像I和图像J可以代表前后两帧图像。对于图像特征点金字塔跟踪来说的目的是：对于前一帧的图像I上一点u(ux,uy)，要在后一帧图像J上找到一点v(ux+dx,uy+dy)与之相匹配，即灰度值最接近。那么向量d=[dx,dy]就是图像在点u处的运动速度，也就是所说像素点u的光流。为了进一步说明向量d的含义。我们假设前一帧图像经历了仿射变换到后一帧图像，定义变换矩阵为

![img](https://img-blog.csdn.net/20150625161307819)

其中四个参数dxx，dyy，dxy，dyx表征着图像中的仿射变形。所以光流计算的目的转变成找到向量d和变换矩阵A使得图像上一块区域内灰度差最小。
 定义误差

![img](https://img-blog.csdn.net/20150625161545518)

其中两个整数wx和wy设定了图像上矩形窗口的大小(2*wx+1)和(2*wy+1)。
 典型的wx和wy取值为1，2，3，4，5，6，7个像素，相似度的函数被在(2ωx+1, 2ωy+1)的区域内定义。注意在金字塔各层窗口的大小是保持恒定的尺寸

对于Lucas-Kanade改进算法来说，主要的步骤有三步：建立金字塔，基于金字塔跟踪，迭代过程。





### 金字塔的建立

令I0 = I 是第 0  层的图像，它是金字塔图像中分辨率最高的图像，图像的宽度和高度分别定义为nx0 = nx 和 ny0 =  ny 。以一种递归的方式建立金字塔：从I0中计算I1，从I1中计算I2 ，···。令L =1,  2,...代表金字塔的层数，L通常取2,3,4。IL−1 是第L−1层的图像，nxL−1 和 nyL−1分别是图像IL−1  的宽度和高度。图像IL可按如下方式由IL−1 求得：
![img](https://img-blog.csdn.net/20150625105715094)

即用一个[0.25 0.5 0.25]的低通滤波器对IL-1进行卷积。



### 金字塔跟踪

总体来讲，金字塔特征跟踪算法描述如下：首先，光流和仿射变换矩阵在最高一层的图像上计算出；将上一层的计算结果作为初始值传递给下一层图像，这一层的图像在这个初始值的基础上，计算这一层的光流和仿射变化矩阵；再将这一层的光流和仿射矩阵作为初始值传递给下一层图像，直到传递给最后一层，即原始图像层，这一层计算出来的光流和仿射变换矩阵作为最后的光流和仿射变换矩阵的结果。

![img](https://img-blog.csdn.net/20151013233103806)
 对于L=0,1,2,…L，定义![img](https://img-blog.csdn.net/20150625164033922)是图像中像素点u在第L层对应点的坐标。根据上一步中图像金字塔的定义，可以计算出

![img](https://img-blog.csdn.net/20150625164201769)

我们用数学的思想重新描述在L层和L+1层迭代运算，假定在第L层有对被跟踪目标的位置有个大致估计，而从第L+1层传递到L层的运动矢量，即光流计算初值为![img](https://img-blog.csdn.net/20150625164304434)（后面会对gL做一个解释）并且对于最上层的变换矩阵猜测
![img](https://img-blog.csdn.net/20150625164611496)

为了在L层上计算光流和仿射变换矩阵，需要重新定义在L层上的匹配误差ξL：
![img](https://img-blog.csdn.net/20150625164921988)
 其中图像![img](https://img-blog.csdn.net/20150625165041147)和![img](https://img-blog.csdn.net/20150625165108097)是原始图像在L层上采样出来的图像，基于这层中的光流和仿射矩阵初值gL和GL可以计算出两个对应图像![img](https://img-blog.csdn.net/20150625165041147)和![img](https://img-blog.csdn.net/20150625165108097)：
![img](https://img-blog.csdn.net/20150625165245492)
 这里用L+1层得到的最初估计gL对L层作预平移，L层在gL的基础上求该层的光流dL，这样求得的残余光流向量dL= [dLx,  dLy]T就足够小，因此能够通过标准的光流法来求出这个运动矢量。然后得到的dL结合gL又可以对L-1层的gL-1做估计。最终的光流和就是在所有层的分段光流d的叠加。使用金字塔图像计算光流的一个明显的好处是，对于一个有着较大的像素偏移的矢量d，可以通过计算几个比较小的残余光流来得到。这里就是金字塔跟踪算法的核心。



接下来就是计算该层上的光流dL和变换矩阵AL，我们将在下一步中谈论。现在，假设在这一层上的光流和变换矩阵己经计算出来。接着将结果传递给下一层，计算出下一层的假设初值：

![img](https://img-blog.csdn.net/20150625165422047)
 将gL-1和GL-1作为初值，重新循环上面的步骤，直到最上一层，计算出光流d和仿射变换矩阵A。

由于金字塔的缩放减小了光流值，最高层的光流估计值可以设为0，设顶层时的初始为：

![img](https://img-blog.csdn.net/20150625165551119)

这种算法最明显的优势在于对于每一层的光流都会保持很小，但是最终计算来的光流可以进行累积，便于有效地跟踪特征点。



### 迭代过程

这一步是算法的核心步骤。在金字塔的每一层，目标是计算出光流dL和仿射变换矩阵AL从而使误差ξL最小。由于每一层的迭代过程是相同的，所以我们就描述从一层到下一层的迭代过程。首先将上一层的光流u和A传给这一层，计算这一帧图像中像素点的光照![img](https://img-blog.csdn.net/20150625162518004)，同时计算出图像在该点x方向和y方向上的偏导

Ix=[I(x+1,y)-I(x-1,y)]/2

Iy=[I(x,y+1)-I(x,y-1)]/2

在此基础上，计算出空间梯度矩阵：

![img](https://img-blog.csdn.net/20150625162950764)

更新光流v=2*v

迭代过程：计算后一帧图像中对应像素点的灰度![img](https://img-blog.csdn.net/20150625163218897)，计算两
 帧图像间相同位置点的灰度值之差![img](https://img-blog.csdn.net/20150625163309499)，在计算图像之间的误差
 向量：
![img](https://img-blog.csdn.net/20150625163401745)
 最后计算针对仿射光流

![img](https://img-blog.csdn.net/20150625163456969)，
 更新跟踪结果

![img](https://img-blog.csdn.net/20150625163612758)
 直到![img](https://img-blog.csdn.net/20150625163701502)某个阈值，结束在这一层的迭代过程。





## 特征点选择

因此，可按照以下的步骤选择特征点：
 1、计算图像 I 中每一个像素的矩阵G和最小特征值λm。
 2、寻找整副图像中最小特征值 λm 中的最大特征值λmax。
 3、保留最小特征值 λm 大于给定阈值的像素点。阈值通常取5% λmax ~10% λmax 。
 4、保留 λm 局部最大值的像素：像素特征值 λm 大于其3*3 邻域中其他像素的特征值 λm 。
 5、剔除像素密集区域中的一些像素，确保图像中相邻像素的距离都大于给定的阈值（常取5~10 pixels）。
 上述操作完成后，图像 I 中剩下的像素即为选择的特征点，并作为跟踪特征点。特征点选择算法的步骤5 确保了特征点间的最小距离。

没有必要取一个大的综合窗口选择特征点（或计算矩阵G）。大量实验证明，wx = wy =1的 3*3 大小的综合窗口能够取得满意的效果。





## 金字塔高度的选择

在大多数的情况下，超过4的金字塔图像层次没有太大的意义。



有时为了简化可以将仿射变换矩阵G简化为单位矩阵。



## 算法流程

![img](https://img-blog.csdn.net/20150625060606188)

![img](https://img-blog.csdn.net/20150625060617657)

![img](https://img-blog.csdn.net/20150625060633893)





下面是我的实现。



```cpp
#ifndef _LKOF_
#define _LKOF_
 
/***********Lucas-Kanade optical flow track**********/
/*****************author Marshall********************/
/**********************2015.10.14********************/
/******************version 1.0***********************/
 
class LucasKanadeTracker
{
	struct DBPoint
	{
		double x;
		double y;
		DBPoint(const double X, const double Y):x(X),y(Y){
		}
		DBPoint(){}
	};
	int*height;
	int*width;
 
private:
	unsigned int max_pyramid_layer;
	unsigned int original_imgH;
	unsigned int original_imgW;
	unsigned int window_radius;
	BYTE**pre_pyr;//the pyramid of previous frame image,img1_pyr[0] is of max size
	BYTE**next_pyr;//the frame after img1_pyr
	bool isusepyramid;
	DBPoint*target,*endin;
	int numofpoint;
 
 
private:
	void build_pyramid(BYTE**&original_gray);
	void lucaskanade(BYTE**&frame_pre, BYTE**&frame_cur, DBPoint*& start,
		DBPoint*& finish, unsigned int point_nums, char*state);
	void get_max_pyramid_layer();
	void pyramid_down(BYTE*&src_gray_data, const int src_h, 
		const int src_w, BYTE*& dst, int&dst_h, int&dst_w);
	void lowpass_filter(BYTE*&src, const int H, const int W, BYTE*&smoothed);
	double get_subpixel(BYTE*&src, int h, int w, const DBPoint& point);
	// caculate the inverse mareix of pMatrix，the result is put into _pMatrix   
	void ContraryMatrix(double *pMatrix, double * _pMatrix, int dim);
	bool matrixMul(double *src1, int height1, int width1, double *src2, int height2, int width2, double *dst);
public:
	LucasKanadeTracker(const int windowRadius, bool usePyr);
	~LucasKanadeTracker();
	void get_pre_frame(BYTE*&gray);//use only at the beginning
	void discard_pre_frame();
	//set the next frame as pre_frame,must dicard pre_pyr in advance
	void get_pre_frame();
	//use every time,must after using get_pre_frame(BYTE**pyr)
	void get_next_frame(BYTE*&gray);
	void get_info(const int nh, const int nw);
	void get_target(POINT*target, int n);
	void run_single_frame();
	POINT get_result();
	BYTE*&get_pyramid(int th);
	int get_pyrH(int th){ return height[th]; }
	int get_pyrW(int th){ return width[th]; }
};
 
#endif



#include "stdafx.h"
#include "LucasKanadeTracker.h"
 
 
using namespace std;
LucasKanadeTracker::LucasKanadeTracker(const int windowRadius, bool usePyr)
	:window_radius(windowRadius), isusepyramid(usePyr)
{
 
}
 
 
LucasKanadeTracker::~LucasKanadeTracker()
{
	for (int i = 0; i < max_pyramid_layer; i++)
	{
		if (pre_pyr[i])
			delete[]pre_pyr[i];
		if (next_pyr[i])
			delete[]next_pyr[i];
	}
	delete[]pre_pyr;
	delete[]next_pyr;
	if (height)
		delete[]height;
	if (width)
		delete[]width;
}
 
 
void LucasKanadeTracker::lowpass_filter(BYTE*&src, const int H, const int W, BYTE*&smoothed)
{
	//tackle with border
	for (int i = 0; i < H; i++)
	{
		smoothed[i*W] = src[i*W];
		smoothed[i*W + W - 1] = src[i*W + W - 1];
	}
	for (int i = 0; i < W; i++)
	{
		smoothed[i] = src[i];
		smoothed[(H - 1)*W + i] = src[(H - 1)*W + i];
	}
 
	for (int i = 1; i < H - 1; i++)
		for (int j = 1; j < W - 1; j++)
		{
			double re = 0;
			re += src[i*W + j] * 0.25;
			re += src[(i - 1)*W + j] * 0.125;
			re += src[i*W + j + 1] * 0.125;
			re += src[i*W + j - 1] * 0.125;
			re += src[(i + 1)*W + j] * 0.125;
			re += src[(i - 1)*W + j - 1] * 0.0625;
			re += src[(i + 1)*W + j - 1] * 0.0625;
			re += src[(i - 1)*W + j + 1] * 0.0625;
			re += src[(i + 1)*W + j + 1] * 0.0625;
			smoothed[i*W + j] = BYTE(re);
		}
	delete[]src;
	src = smoothed;
}
 
 
void LucasKanadeTracker::get_info(const int nh, const int nw)
{
	original_imgH = nh;
	original_imgW = nw;
	if (isusepyramid)
		get_max_pyramid_layer();
	else
		max_pyramid_layer = 1;
	pre_pyr = new BYTE*[max_pyramid_layer];
	next_pyr = new BYTE*[max_pyramid_layer];
	height = new int[max_pyramid_layer];
	width = new int[max_pyramid_layer];
	height[0] = nh;
	width[0] = nw;
}
void LucasKanadeTracker::get_target(POINT*target, int n)
{
	this->target = new DBPoint[n];
	endin = new DBPoint[n];
	for (int i = 0; i < n; i++)
	{
		this->target[i].x = target[i].x;
		this->target[i].y = target[i].y;
	}
	numofpoint = n;
}
 
BYTE*&LucasKanadeTracker::get_pyramid(int th)
{
	return pre_pyr[th];
}
POINT LucasKanadeTracker::get_result()
{
	POINT pp;
	pp.x = target[0].x;
	pp.y = target[0].y;
	return pp;
}
void LucasKanadeTracker::get_pre_frame(BYTE*&gray)//use only at the beginning
{
	pre_pyr[0] = gray;
	build_pyramid(pre_pyr);
	//save_gray("1.bmp", pre_pyr[1], height[1], width[1]);
}
 
void  LucasKanadeTracker::discard_pre_frame()
{
	//we don't new memory for original data,so we don't delete it here
	for (int i = 0; i < max_pyramid_layer; i++)
		delete[]pre_pyr[i];
}
//set the next frame as pre_frame,must dicard pre_pyr in advance
void  LucasKanadeTracker::get_pre_frame()
{
	for (int i = 0; i < max_pyramid_layer; i++)
		pre_pyr[i] = next_pyr[i];
}
//use every time,must after using get_pre_frame(BYTE**pyr)
void  LucasKanadeTracker::get_next_frame(BYTE*&gray)
{
	next_pyr[0] = gray;
	build_pyramid(next_pyr);
	//save_gray("1.bmp", next_pyr[1], height[1], width[1]);
}
 
void LucasKanadeTracker::pyramid_down(BYTE*&src_gray_data,
	const int src_h, const int src_w, BYTE*& dst, int&dst_h, int&dst_w)
{
	dst_h = src_h / 2;
	dst_w = src_w / 2;
	int ii = height[1];
	int hh = width[1];
	assert(dst_w > 3 && dst_h > 3);
	//BYTE*smoothed = new BYTE[src_h*src_w];
	dst = new BYTE[dst_h*dst_w];
	//lowpass_filter(src_gray_data, src_h, src_w,smoothed);
	for (int i = 0; i < dst_h - 1; i++)
		for (int j = 0; j < dst_w - 1; j++)
		{
			int srcY = 2 * i + 1;
			int srcX = 2 * j + 1;
			double re = src_gray_data[srcY*src_w + srcX] * 0.25;
			re += src_gray_data[(srcY - 1)*src_w + srcX] * 0.125;
			re += src_gray_data[(srcY + 1)*src_w + srcX] * 0.125;
			re += src_gray_data[srcY*src_w + srcX - 1] * 0.125;
			re += src_gray_data[srcY*src_w + srcX + 1] * 0.125;
			re += src_gray_data[(srcY - 1)*src_w + srcX + 1] * 0.0625;
			re += src_gray_data[(srcY - 1)*src_w + srcX - 1] * 0.0625;
			re += src_gray_data[(srcY + 1)*src_w + srcX - 1] * 0.0625;
			re += src_gray_data[(srcY + 1)*src_w + srcX + 1] * 0.0625;
			dst[i*dst_w + j] = re;
		}
	for (int i = 0; i < dst_h; i++)
		dst[i*dst_w + dst_w - 1] = dst[i*dst_w + dst_w - 2];
	for (int i = 0; i < dst_w; i++)
		dst[(dst_h - 1)*dst_w + i] = dst[(dst_h - 2)*dst_w + i];
}
 
//bilinear interplotation
double LucasKanadeTracker::get_subpixel(BYTE*&src, int h, int w, const DBPoint& point)
{
	int floorX = floor(point.x);
	int floorY = floor(point.y);
 
	double fractX = point.x - floorX;
	double fractY = point.y - floorY;
 
	return ((1.0 - fractX) * (1.0 - fractY) * src[floorX + w* floorY])
		+ (fractX * (1.0 - fractY) * src[floorX + 1 + floorY*w])
		+ ((1.0 - fractX) * fractY * src[floorX + (floorY + 1)*w])
		+ (fractX * fractY * src[floorX + 1 + (floorY + 1)*w]);
}
 
 
void LucasKanadeTracker::get_max_pyramid_layer()
{
	int layer = 0;
	int windowsize = 2 * window_radius + 1;
	int temp = original_imgH > original_imgW ?
	original_imgW : original_imgH;
	if (temp > ((1 << 4) * 2 * windowsize))
	{
		max_pyramid_layer = 5;
		return;
	}
	temp = double(temp) / 2;
	while (temp > 2 * windowsize)
	{
		layer++;
		temp = double(temp) / 2;
	}
	max_pyramid_layer = layer;
}
 
void LucasKanadeTracker::build_pyramid(BYTE**&pyramid)
{
	for (int i = 1; i < max_pyramid_layer; i++)
	{
		pyramid_down(pyramid[i - 1], height[i - 1],
			width[i - 1], pyramid[i], height[i], width[i]);
	}
}
 
void LucasKanadeTracker::run_single_frame()
{
	char*state = NULL;
	lucaskanade(pre_pyr, next_pyr, target, endin, numofpoint, state);
	for (int i = 0; i < numofpoint; i++)
	{
		target[i].x = endin[i].x;
		target[i].y = endin[i].y;
	}
 
}
 
void LucasKanadeTracker::lucaskanade(BYTE**&frame_pre, BYTE**&frame_cur,
	DBPoint*& start, DBPoint*& finish, unsigned int point_nums, char*state)
{
 
	double*derivativeXs = new double
		[(2 * window_radius + 1)*(2 * window_radius + 1)];
	double*derivativeYs = new double
		[(2 * window_radius + 1)*(2 * window_radius + 1)];
	for (int i = 0; i < point_nums; i++)
	{
		double g[2] = { 0 };
		double finalopticalflow[2] = { 0 };
 
		memset(derivativeXs, 0, sizeof(double)*
			(2 * window_radius + 1)*(2 * window_radius + 1));
 
		memset(derivativeYs, 0, sizeof(double)*
			(2 * window_radius + 1)*(2 * window_radius + 1));
 
		for (int j = max_pyramid_layer - 1; j >= 0; j--)
		{
			DBPoint curpoint;
			curpoint.x = start[i].x / pow(2.0, j);
			curpoint.y = start[i].y / pow(2.0, j);
			double Xleft = curpoint.x - window_radius;
			double Xright = curpoint.x + window_radius;
			double Yleft = curpoint.y - window_radius;
			double Yright = curpoint.y + window_radius;
 
			double gradient[4] = { 0 };
			int cnt = 0;
			for (double xx = Xleft; xx < Xright + 0.01; xx += 1.0)
				for (double yy = Yleft; yy < Yright + 0.01; yy += 1.0)
				{
					assert(xx < 1000 && yy < 1000 && xx >= 0 && yy >= 0);
					double derivativeX = get_subpixel(frame_pre[j],
						height[j], width[j], DBPoint(xx + 1.0, yy)) -
						get_subpixel(frame_pre[j], height[j],
						width[j], DBPoint(xx - 1.0, yy));
					derivativeX /= 2.0;
 
					double t1 = get_subpixel
						(frame_pre[j], height[j], width[j], DBPoint(xx, yy + 1.0));
					double t2 = get_subpixel(frame_pre[j], height[j],
						width[j], DBPoint(xx, yy - 1.0));
					double derivativeY = (t1 - t2) / 2.0;
 
					derivativeXs[cnt] = derivativeX;
					derivativeYs[cnt++] = derivativeY;
					gradient[0] += derivativeX * derivativeX;
					gradient[1] += derivativeX * derivativeY;
					gradient[2] += derivativeX * derivativeY;
					gradient[3] += derivativeY * derivativeY;
				}
			double gradient_inv[4] = { 0 };
			ContraryMatrix(gradient, gradient_inv, 2);
 
			double opticalflow[2] = { 0 };
			int max_iter = 50;
			double opticalflow_residual = 1;
			int iteration = 0;
			while (iteration<max_iter&&opticalflow_residual>0.00001)
			{
				iteration++;
				double mismatch[2] = { 0 };
				cnt = 0;
				for (double xx = Xleft; xx < Xright + 0.001; xx += 1.0)
					for (double yy = Yleft; yy < Yright + 0.001; yy += 1.0)
					{
						assert(xx < 1000 && yy < 1000 && xx >= 0 && yy >= 0);
						double nextX = xx + g[0] + opticalflow[0];
						double nextY = yy + g[1] + opticalflow[1];
						assert(nextX < 1000 && nextY < 1000 && nextX >= 0 && nextY >= 0);
						double pixelDifference = (get_subpixel(frame_pre[j],
							height[j], width[j], DBPoint(xx, yy))
							- get_subpixel(frame_cur[j], height[j],
							width[j], DBPoint(nextX, nextY)));
						mismatch[0] += pixelDifference*derivativeXs[cnt];
						mismatch[1] += pixelDifference*derivativeYs[cnt++];
					}
				double temp_of[2];
				matrixMul(gradient_inv, 2, 2, mismatch, 2, 1, temp_of);
				opticalflow[0] += temp_of[0];
				opticalflow[1] += temp_of[1];
				opticalflow_residual = abs(temp_of[0]) + abs(temp_of[1]);
			}
			if (j == 0)
			{
				finalopticalflow[0] = opticalflow[0];
				finalopticalflow[1] = opticalflow[1];
			}
			else
			{
				g[0] = 2 * (g[0] + opticalflow[0]);
				g[1] = 2 * (g[1] + opticalflow[1]);
			}
		}
		finalopticalflow[0] += g[0];
		finalopticalflow[1] += g[1];
		finish[i].x = start[i].x + finalopticalflow[0];
		finish[i].y = start[i].y + finalopticalflow[1];
	}
	delete[]derivativeXs, derivativeYs;
}
 
 
 
//matrix inverse
void LucasKanadeTracker::ContraryMatrix(double *pMatrix, double * _pMatrix, int dim)
{
	double *tMatrix = new double[2 * dim*dim];
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++)
			tMatrix[i*dim * 2 + j] = pMatrix[i*dim + j];
	}
	for (int i = 0; i < dim; i++){
		for (int j = dim; j < dim * 2; j++)
			tMatrix[i*dim * 2 + j] = 0.0;
		tMatrix[i*dim * 2 + dim + i] = 1.0;
	}
	//Initialization over!   
	for (int i = 0; i < dim; i++)//Process Cols   
	{
		double base = tMatrix[i*dim * 2 + i];
		if (fabs(base) < 1E-300){
			// AfxMessageBox("求逆矩阵过程中被零除，无法求解!" );
			_ASSERTE(-1);//throw exception
			exit(0);
		}
		for (int j = 0; j < dim; j++)//row   
		{
			if (j == i) continue;
			double times = tMatrix[j*dim * 2 + i] / base;
			for (int k = 0; k < dim * 2; k++)//col   
			{
				tMatrix[j*dim * 2 + k] = tMatrix[j*dim * 2 + k] - times*tMatrix[i*dim * 2 + k];
			}
		}
		for (int k = 0; k < dim * 2; k++){
			tMatrix[i*dim * 2 + k] /= base;
		}
	}
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
			_pMatrix[i*dim + j] = tMatrix[i*dim * 2 + j + dim];
	}
	delete[] tMatrix;
}
 
bool LucasKanadeTracker::matrixMul(double *src1, int height1, int width1, double *src2, int height2, int width2, double *dst)
{
	int i, j, k;
	double sum = 0;
	double *first = src1;
	double *second = src2;
	double *dest = dst;
	int Step1 = width1;
	int Step2 = width2;
 
	if (src1 == NULL || src2 == NULL || dest == NULL || height2 != width1)
		return false;
 
	for (j = 0; j < height1; j++)
	{
		for (i = 0; i < width2; i++)
		{
			sum = 0;
			second = src2 + i;
			for (k = 0; k < width1; k++)
			{
				sum += first[k] * (*second);
				second += Step2;
			}
			dest[i] = sum;
		}
		first += Step1;
		dest += Step2;
	}
	return true;
}
```







下面是两帧图像间特征点的跟踪



![img](https://img-blog.csdn.net/20151021224025167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)      ![img](https://img-blog.csdn.net/20151021224041517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



参考文献：Pyramidal Implementation of the AÆne Lucas Kanade Feature Tracker Description of the algorithm

An Iterative Image Registration Technique with an Application to Stereo Vision

 Detection and Tracking of Point Features

Good Features to Track

 Derivation of Kanade-Lucas-Tomasi Tracking Equation



[Lucas–Kanade光流算法](http://www.cnblogs.com/gnuhpc/archive/2012/12/04/2802124.html)



[Kanade-Lucas-Tomasi(KLT)目标跟踪](http://blog.csdn.net/App_12062011/article/details/51880866)

[光流法与KLT](http://blog.csdn.net/lanbing510/article/details/40451317)

[跟踪算法合集](http://blog.csdn.net/App_12062011/article/category/6269524)





[Opencv学习笔记（九）光流法](http://blog.csdn.net/crzy_sparrow/article/details/7407604)



[OpenCV光流跟踪程序学习](http://blog.csdn.net/yzhang6_10/article/details/51221916)

