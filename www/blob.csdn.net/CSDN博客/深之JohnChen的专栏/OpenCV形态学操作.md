# OpenCV形态学操作 - 深之JohnChen的专栏 - CSDN博客

2010年07月30日 10:23:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：20379


OpenCV形态学操作

一、图像腐蚀 膨胀 细化的基本原理

1．图像细化的基本原理
⑴ 图像形态学处理的概念
数字图像处理中的形态学处理是指将数字形态学作为工具从图像中提取对于表达和描绘区域形状有用处的图像分量，比如边界、骨架以及凸壳，还包括用于预处理或后处理的形态学过滤、细化和修剪等。图像形态学处理中我们感兴趣的主要是二值图像。
在二值图像中，所有黑色像素的集合是图像完整的形态学描述，二值图像的各个分量是Z2的元素。假定二值图像A和形态学处理的结构元素B是定义在笛卡儿网格上的集合，网格中值为1的点是集合的元素，当结构元素的原点移到点(x,y)时，记为Sxy，为简单起见，结构元素为3x3，且全都为1，在这种限制下，决定输出结果的是逻辑运算。 

⑵ 二值图像的逻辑运算
逻辑运算尽管本质上很简单，但对于实现以形态学为基础额图像处理算法是一种有力的补充手段。在图像处理中用到的主要逻辑运算是：与、或和非（求补），它们可以互相组合形成其他逻辑运算。

⑶ 膨胀和腐蚀
膨胀和腐蚀这两种操作是形态学处理的基础，许多形态学算法都是以这两种运算为基础的。
① 膨胀
是以得到B的相对与它自身原点的映像并且由z对映像进行移位为基础的。A被B膨胀是所有位移z的集合，这样， 和A至少有一个元素是重叠的。我们可以把上式改写为： 
结构元素B可以看作一个卷积模板，区别在于膨胀是以集合运算为基础的，卷积是以算术运算为基础的，但两者的处理过程是相似的。
⑴ 用结构元素B，扫描图像A的每一个像素
⑵ 用结构元素与其覆盖的二值图像做“与”操作
⑶ 如果都为0，结果图像的该像素为0。否则为1

② 腐蚀
对Z中的集合A和B，B对A进行腐蚀的整个过程如下： 
⑴ 用结构元素B，扫描图像A的每一个像素
⑵ 用结构元素与其覆盖的二值图像做“与”操作
⑶ 如果都为1，结果图像的该像素为1。否则为0
腐蚀处理的结果是使原来的二值图像减小一圈。

⑷ 击中（匹配）或击不中变换
假设集合A是由3个子集X，Y和Z组成的集合，击中（匹配）的目的是要在A中找到X的位置，我们设X被包围在一个小窗口W中，与W有关的X的局部背景定义为集合的差（W－X），则X在A内能得到精确拟合位置集合是由X对A的腐蚀后由（W－X）对A的补集Ac腐蚀的交集，这个交集就是我们要找的位置，我们用集合B来表示由X和X的背景构成的集合，我们可以令B＝（B1，B2），这里B1＝X，B2＝（W－X），则在A中对B进行匹配可以表示为：
A⊙B 
我们称为形态学上的击中或击不中变换。

⑸ 开闭操作
开操作是先腐蚀、后膨胀处理。

闭操作是先膨胀、后腐蚀处理。

(6) 细化
图像细化一般作为一种图像预处理技术出现，目的是提取源图像的骨架，即是将原图像中线条宽度大于1个像素的线条细化成只有一个像素宽，形成“骨架”，形成骨架后能比较容易的分析图像，如提取图像的特征。
细化基本思想是“层层剥夺”，即从线条边缘开始一层一层向里剥夺，直到线条剩下一个像素的为止。图像细化大大地压缩了原始图像地数据量，并保持其形状的基本拓扑结构不变，从而为文字识别中的特征抽取等应用奠定了基础。细化算法应满足以下条件：
① 将条形区域变成一条薄线；
② 薄线应位与原条形区域的中心；
③ 薄线应保持原图像的拓扑特性。
细化分成串行细化和并行细化，串行细化即是一边检测满足细化条件的点，一边删除细化点；并行细化即是检测细化点的时候不进行点的删除只进行标记，而在检测完整幅图像后一次性去除要细化的点。
常用的图像细化算法有hilditch算法，pavlidis算法和rosenfeld算法等。
注：进行细化算法前要先对图像进行二值化,即图像中只包含“黑”和“白”两种颜色。

具体详细的图像形态学资料参考：[http://wenku.baidu.com/view/1923d18fcc22bcd126ff0ccc.html](http://wenku.baidu.com/view/1923d18fcc22bcd126ff0ccc.html)

二、OpenCv形态学操作相关函数

1、MorphologyEx高级形态学变换

void cvMorphologyEx( const CvArr* src, CvArr* dst, CvArr* temp,
IplConvKernel* element, int operation, int iterations=1 );
src 
输入图像. 
dst 
输出图像. 
temp 
临时图像，某些情况下需要
element 
结构元素
operation 
形态操作的类型: 
CV_MOP_OPEN - 开运算
CV_MOP_CLOSE - 闭运算
CV_MOP_GRADIENT - 形态梯度
CV_MOP_TOPHAT - "顶帽" 
CV_MOP_BLACKHAT - "黑帽" 
iterations 
膨胀和腐蚀次数. 
函数 cvMorphologyEx 在膨胀和腐蚀基本操作的基础上，完成一些高级的形态变换：

开运算
dst=open(src,element)=dilate(erode(src,element),element) 
闭运算
dst=close(src,element)=erode(dilate(src,element),element) 
形态梯度
dst=morph_grad(src,element)=dilate(src,element)-erode(src,element) 
"顶帽"
dst=tophat(src,element)=src-open(src,element) 
"黑帽"
dst=blackhat(src,element)=close(src,element)-src 
临时图像 temp 在形态梯度以及对“顶帽”和“黑帽”操作时的 in-place 模式下需要。

2、Dilate使用任意结构元素膨胀图像

void cvDilate( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 );
src 
输入图像. 
dst 
输出图像. 
element 
用于膨胀的结构元素。若为 NULL, 则使用 3×3 长方形的结构元素
iterations 
膨胀的次数
函数 cvDilate 对输入图像使用指定的结构元进行膨胀，该结构决定每个具有最小值象素点的邻域形状：
dst=dilate(src,element): dst(x,y)=max((x',y') in element))src(x+x',y+y')
函数支持（in-place）模式。膨胀可以重复进行 (iterations) 次. 对彩色图像，每个彩色通道单独处理。

3、Erode使用任意结构元素腐蚀图像

void cvErode( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 );
src 
输入图像. 
dst 
输出图像. 
element 
用于腐蚀的结构元素。若为 NULL, 则使用 3×3 长方形的结构元素
iterations 
腐蚀的次数
函数 cvErode 对输入图像使用指定的结构元素进行腐蚀，该结构元素决定每个具有最小值象素点的邻域形状：
dst=erode(src,element): dst(x,y)=min((x',y') in element))src(x+x',y+y')
函数可能是本地操作，不需另外开辟存储空间的意思。腐蚀可以重复进行 (iterations) 次. 对彩色图像，每个彩色通道单独处理。

注：CreateStructuringElementEx创建结构元素；ReleaseStructuringElement 删除结构元素。

三、OpenCv形态学实例代码：

1、腐蚀、膨胀、开运算、闭运算

内容参考：http://blog.csdn.net/gnuhpc/archive/2009/06/21/4286177.aspx

/*******************************

数学形态运算，最常见的基本运算有七种，

分别为：腐蚀、膨胀、开运算、闭运算、击中、细化和粗化，

它们是全部形态学的基础。

********************************/

#include "cv.h"

#include "highgui.h"

#include <stdlib.h>

#include <stdio.h>

IplImage *src=0;

IplImage *dst=0;

IplConvKernel *element=0;//声明一个结构元素

int element_shape=CV_SHAPE_RECT;//长方形形状的元素

int max_iters=10;

int open_close_pos=0;

int erode_dilate_pos=0;

void OpenClose(int pos)

{

int n=open_close_pos-max_iters;

int an=n>0?n:-n;

element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);//创建结构元素

if (n<0)

{

cvErode(src,dst,element,1);//腐蚀图像

cvDilate(dst,dst,element,1);//膨胀图像

}

else

{

cvDilate(dst,dst,element,1);//膨胀图像

cvErode(src,dst,element,1);//腐蚀图像

}

cvReleaseStructuringElement(&element);

cvShowImage("Open/Close",dst);

}

void ErodeDilate(int pos)

{

int n=erode_dilate_pos-max_iters;

int an=n>0?n:-n;

element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);

if (n<0)

{

cvErode(src,dst,element,1);

}

else

{

cvDilate(src,dst,element,1);

}

cvReleaseStructuringElement(&element);

cvShowImage("Erode/Dilate",dst);

}

int main(int argc,char **argv)

{

char *filename =argc ==2?argv[1]:(char *)"lena.jpg"; 

if( (src = cvLoadImage(filename,1)) == 0 )

return -1;

dst=cvCloneImage(src);

cvNamedWindow("Open/Close",1);

cvNamedWindow("Erode/Dilate",1);

open_close_pos = erode_dilate_pos = max_iters;

cvCreateTrackbar("iterations","Open/Close",&open_close_pos,max_iters*2+1,OpenClose);

cvCreateTrackbar("iterations","Erode/Dilate",&erode_dilate_pos,max_iters*2+1,ErodeDilate);

for (;;)

{

int c;

OpenClose(open_close_pos);

ErodeDilate(erode_dilate_pos);

c= cvWaitKey(0);

if (c==27)

{

break;

}

switch(c) {

case 'e':

element_shape=CV_SHAPE_ELLIPSE;

break;

case 'r':

element_shape=CV_SHAPE_RECT;

break;

case '/r':

element_shape=(element_shape+1)%3;

break;

default:

break;

}

}

cvReleaseImage(&src);

cvReleaseImage(&dst);

cvDestroyWindow("Open/Close");

cvDestroyWindow("Erode/Dilate");

return 0;

}

/*****************************

腐蚀和膨胀，看上去好像是一对互逆的操作，实际上，这两种操作不具有互逆的关系。

开运算和闭运算正是依据腐蚀和膨胀的不可逆性，演变而来的。

先腐蚀后膨胀的过程就称为开运算。

闭运算是通过对腐蚀和膨胀的另一种不同次序的执行而得到的，

闭运算是先膨胀后腐蚀的过程，其功能是用来填充物体内细小空洞、连接邻近物体、平滑其边界，

同时不明显改变不明显改变其面积。

******************************/

2、opencv实现二值图像细化

内容参考：[http://blog.csdn.net/byxdaz/archive/2010/06/02/5642669.aspx](http://blog.csdn.net/byxdaz/archive/2010/06/02/5642669.aspx)

