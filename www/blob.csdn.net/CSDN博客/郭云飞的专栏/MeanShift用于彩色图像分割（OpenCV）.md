
# MeanShift用于彩色图像分割（OpenCV） - 郭云飞的专栏 - CSDN博客


2017年12月06日 13:55:21[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1756


**一、均值偏移（Mean Shift）算法**
Mean Shift算法，一般是指一个迭代的步骤，即先算出当前点的偏移均值，移动该点到其偏移均值，然后以此为新的起始点。继续移动，直到满足一定的条件结束。
![](https://img-blog.csdn.net/20171206131425249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Mean Shift向量定义：
![](https://img-blog.csdn.net/20171206131312154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上述定义的理解：在d维空间中，任选一个点，然后以这个点为圆心，h为半径做一个高维球，因为有d维，d可能大于2，所以是高维球。落在这个球内的所有点和圆心都会产生一个向量（以圆心为起点落在球内的点为终点）。然后把这些向量都相加，其结果还是一个向量，即Meanshift向量。
如下左图所示，黄色箭头就是MeanShift向量。再以meanshift向量的终点为圆心，再做一个高维的球。如下中图所示，重复以上步骤，又得到一个meanshift向量。如此重复下去，meanshift算法可以收敛到概率密度最大得地方。也就是最稠密的地方，如下右图所示。
![](https://img-blog.csdn.net/20171206132504334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**二、MeanShift用于彩色图像分割**
利用均值偏移算法实现彩色图像分割，OpenCV中对应的函数是pyrMeanShiftFiltering。这个函数严格来说并不是图像的分割，而是图像在色彩层面的平滑滤波，它可以中和色彩分布相近的颜色，平滑色彩细节，侵蚀掉面积较小的颜色区域，所以在Opencv中它的后缀是滤波“Filter”，而不是分割“segment”。

```python
/********************************
src      输入图像，8位，三通道的彩色图像，并不要求必须是RGB格式，HSV、YUV等OpenCV中的彩色图像格式均可；
dst      输出图像，跟输入src有同样的大小和数据格式；
sp       定义的漂移物理空间半径大小；
sr       定义的漂移色彩空间半径大小；
maxLevel 定义金字塔的最大层数；
termcrit 定义的漂移迭代终止条件，可以设置为迭代次数满足终止，迭代目标与中心点偏差满足终止，或者两者的结合；
*/
void cv::pyrMeanShiftFiltering( InputArray  src,
                                OutputArray dst,
                                double  sp,
                                double  sr,
                                int maxLevel = 1,
                                TermCriteria termcrit = TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 5, 1) 
)
```
pyrMeanShiftFiltering函数的执行过程是这样的：
迭代空间构建：以输入图像src上任一点P0为圆心，建立物理空间上半径为sp，色彩空间上半径为sr的球形空间，物理空间上坐标2个x、y，色彩空间上坐标3个R、G、B（或HSV），构成一个5维的空间球体。其中物理空间的范围x和y是图像的长和宽，色彩空间的范围R、G、B分别是0~255。
求取迭代空间的向量并移动迭代空间球体后重新计算向量，直至收敛。在1中构建的球形空间中，求得所有点相对于中心点的色彩向量之和后，移动迭代空间的中心点到该向量的终点，并再次计算该球形空间中所有点的向量之和，如此迭代，直到在最后一个空间球体中所求得的向量和的终点就是该空间球体的中心点Pn，迭代结束。
更新输出图像dst上对应的初始原点P0的色彩值为本轮迭代的终点Pn的色彩值，如此完成一个点的色彩均值漂移。
对输入图像src上其他点，依次执行步骤1,、2、3，遍历完所有点位后，整个均值偏移色彩滤波完成，这里忽略对金字塔的讨论。
在这个过程中，关键参数是sp和sr的设置，二者设置的值越大，对图像色彩的平滑效果越明显，同时函数耗时也越多。

到这里，meanShift均值偏移算法对彩色图像的平滑操作就完成了，为了达到分割的目的，需要借助另外一个漫水填充函数的进一步处理来实现，那就是floodFill：

```python
/********************************
image       输入三通道8bit彩色图像，同时作为输出。
mask        是掩模图像，它的大小是输入图像的长宽左右各加1个像素，
            mask一方面作为输入的掩模图像，另一方面也会在填充的过程中不断被更新。
            floodFill漫水填充的过程并不会填充mask上灰度值不为0的像素点，
            所以可以使用一个图像边缘检测的输出作为mask，
            这样填充就不会填充或越过边缘轮廓。
            mask在填充的过程中被更新的过程是这样的：
            每当一个原始图上一个点位（x，y）被填充之后，
            该点位置对应的mask上的点（x+1，y+1）的灰度值随机被设置为1（原本该点的灰度值为0），
            代表该点已经被填充处理过。
seedPoint   是漫水填充的起始种子点。
newVal      被充填的色彩值。
rect        可选的参数，用于设置floodFill函数将要重绘区域的最小矩形区域；
loDiff
upDiff      用于定义跟种子点相比色彩的下限值和上限值，
            介于种子点减去loDiff和种子点加上upDiff的
            值会被填充为跟种子点同样的颜色。
第八个参数，定义漫水填充的模式，用于连通性、扩展方向等的定义。
*/
int cv::floodFill(  InputOutputArray    image,
                    InputOutputArray    mask,
                    Point   seedPoint,
                    Scalar  newVal,
                    Rect *  rect = 0,
                    Scalar  loDiff = Scalar(),
                    Scalar  upDiff = Scalar(),
                    int     flags = 4 
)
```

OpenCV例程：
```python
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/core/core.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
  
using namespace cv;  
  
int main(int argc, char** argv)  
{  
    Mat img = imread( argv[1] ); //读入图像，RGB三通道    
    imshow("原图像",img);  
    Mat res;                    //分割后图像  
    int spatialRad = 50;        //空间窗口大小  
    int colorRad = 50;          //色彩窗口大小  
    int maxPyrLevel = 2;        //金字塔层数  
    pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel); //色彩聚类平滑滤波  
    imshow("res",res);        
    RNG rng = theRNG();  
    Mat mask( res.rows+2, res.cols+2, CV_8UC1, Scalar::all(0) );  //掩模  
    for( int y = 0; y < res.rows; y++ )  
    {  
        for( int x = 0; x < res.cols; x++ )  
        {  
            if( mask.at<uchar>(y+1, x+1) == 0 )  //非0处即为1，表示已经经过填充，不再处理  
            {  
                Scalar newVal( rng(256), rng(256), rng(256) );  
                floodFill( res, mask, Point(x,y), newVal, 0, Scalar::all(5), Scalar::all(5) ); //执行漫水填充  
            }  
        }  
    }  
    imshow("meanShift图像分割", res );  
    waitKey();  
    return 0;  
}
```







