# Opencv学习笔记（四）霍夫变换

2012年03月15日 18:00:39

crzy_sparrow

阅读数：14115

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

 最简单的霍夫变换是在图像中识别直线。在平面直角坐标系(x-y)中，一条直线可以用下式表示。

​      y=kx+b

​      对于直线上一个确定的点(x_0,y_0)，有：y_0=kx_0+b

​       这表示参数平面(k-b)中的一条直线。因此，图像中的一个点对应参数平面中的一条直线，图像中的一条直线对应参数平面中的一个点。对图像上所有的点作霍夫变换，最终所要检测的直线对应的一定是参数平面中直线相交最多的那个点。这样就在图像中检测出了直线。在实际应用中，直线通常采用参数方程



​        ![p=x\cos\theta+y\sin\theta](http://upload.wikimedia.org/wikipedia/zh/math/8/c/5/8c5f742b64fba2ba48e675d38e572c1e.png)

​    Opencv里有以下函数检测直线（最基本的霍夫变换）：

void HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold, double srn=0, double stn=0 )

​    具体用法看代码就知道了：

  

```cpp
#include "opencv2/opencv.hpp"
#define PI 3.1415926
 
int main(int argc, char *argv[])
{
    cv::Mat image = cv::imread ("road.jpg");
    cv::Mat result;
    cv::cvtColor (image,result,CV_BGRA2GRAY);
    cv::Mat contours;
    //边缘检测
    cv::Canny (result,contours,125,350);
    std::vector<cv::Vec2f> lines;
    //霍夫变换,获得一组极坐标参数（rho，theta）,每一对对应一条直线，保存到lines
    //第3,4个参数表示在（rho，theta)坐标系里横纵坐标的最小单位，即步长
    cv::HoughLines (contours,lines,1,PI/180,80);
    std::vector<cv::Vec2f>::const_iterator it = lines.begin ();
    std::cout<<lines.size ()<<std::endl;
    while(it != lines.end()){
        float rho = (*it)[0];
        float theta = (*it)[1];
        if(theta<PI/4.||theta>3.*PI/4){
            //画交点在上下两边的直线
            cv::Point pt1(rho/cos(theta),0);
            cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
            cv::line(image,pt1,pt2,cv::Scalar(255),1);
        }
       else {
            //画交点在左右两边的直线
            cv::Point pt1(0,rho/sin(theta));
            cv::Point pt2(result.cols,(rho-result.cols*cos(theta)/sin(theta)));
            cv::line(image,pt1,pt2,cv::Scalar(255),1);
        }
        ++it;
    }
    cv::namedWindow ("hough");
    cv::imshow("hough",image);
    cv::waitKey (0);
}
```

检测结果示例：



![img](http://hi.csdn.net/attachment/201203/15/0_1331805815ylDF.gif)



​    另外，可以看出，上面的直线检测存在以下问题：

​    1）只能检测出线段所在的直线，而不知道具体线段位置，也不知道线段长度；

​    2）同一直线可能检测出多条直线；

​    3）偶然地也可能误判段直线。

​    针对这些问题，opencv有那么一个函数：

void HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold, dou-
 ble minLineLength=0, double maxLineGap=0 )
 这个方法是通过概率霍夫变换实现的：



1）随机获取边缘图片上的前景点，映射到级坐标系画曲线；

2）当极坐标系里面有交点达到最小投票数，将该点对应x-y坐标系的直线L找出来；

3）搜索边缘图片上前景点，在直线L上的点（且点与点之间距离小于maxLineGap的）连成线段，然后这些点全部删除，并且记录该线段的参数，就是起始点和终止点啦～～～～～～～～～～～～～～～～～～～（当然这里线段长度要满足最小长度的，否则就不用记录了）

4）重复1），2），3）

​    其使用方法见代码:

   

```cpp
#include "opencv2/opencv.hpp"
#define PI 3.1415926
 
class LineFinder{
private:
        // 直线对应的点参数向量
        std::vector<cv::Vec4i> lines;
        //步长
        double deltaRho;
        double deltaTheta;
        // 判断是直线的最小投票数
        int minVote;
        // 判断是直线的最小长度
        double minLength;
        // 同一条直线上点之间的距离容忍度
        double maxGap;
public:
        //初始化
        LineFinder() : deltaRho(1), deltaTheta(PI/180),
        minVote(10), minLength(0.), maxGap(0.) {}
        // 设置步长
        void setAccResolution(double dRho, double dTheta) {
                deltaRho= dRho;
                deltaTheta= dTheta;
        }
        // 设置最小投票数
        void setMinVote(int minv) {
                minVote= minv;
        }
        // 设置最小线段长度和线段间距容忍度
        void setLineLengthAndGap(double length, double gap) {
                minLength= length;
                maxGap= gap;
        }
 
        //寻找线段
        std::vector<cv::Vec4i> findLines(cv::Mat& binary) {
                lines.clear();
                cv::HoughLinesP(binary,lines, deltaRho, deltaTheta, minVote,minLength, maxGap);
//                return lines;
        }
 
        // 画线段
        void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,255,255)) {
        std::vector<cv::Vec4i>::const_iterator it2=lines.begin();
        while (it2!=lines.end()) {
                cv::Point pt1((*it2)[0],(*it2)[1]);
                cv::Point pt2((*it2)[2],(*it2)[3]);
                cv::line( image, pt1, pt2, color);
                ++it2;
              }
        }
};
 
 
int main(int argc, char *argv[])
{
    cv::Mat image = cv::imread ("road.jpg");
    cv::Mat result;
    cv::cvtColor (image,result,CV_BGRA2GRAY);
    cv::Mat contours;
    //边缘检测
    cv::Canny (result,contours,125,350);
    LineFinder finder;
    finder.setMinVote (80);
    finder.setLineLengthAndGap (100,20);
    finder.findLines (contours);
    finder.drawDetectedLines (image);
    /*
    std::vector<cv::Vec2f> lines;
    //霍夫变换,获得一组极坐标参数（rho，theta）,每一对对应一条直线，保存到lines
    //第3,4个参数表示在（rho，theta)坐标系里横纵坐标的最小单位，即步长
    cv::HoughLines (contours,lines,1,PI/180,80);
    std::vector<cv::Vec2f>::const_iterator it = lines.begin ();
    std::cout<<lines.size ()<<std::endl;
    while(it != lines.end()){
        float rho = (*it)[0];
        float theta = (*it)[1];
        if(theta<PI/4.||theta>3.*PI/4){
            //画交点在上下两边的直线
            cv::Point pt1(rho/cos(theta),0);
            cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
            cv::line(image,pt1,pt2,cv::Scalar(255),1);
        }
       else {
            //画交点在左右两边的直线
            cv::Point pt1(0,rho/sin(theta));
            cv::Point pt2(result.cols,(rho-result.cols*cos(theta)/sin(theta)));
            cv::line(image,pt1,pt2,cv::Scalar(255),1);
        }
        ++it;
    }
    */
    cv::namedWindow ("hough");
    cv::imshow("hough",image);
    cv::waitKey (0);
}
```

测试结果如下

： 



![img](http://hi.csdn.net/attachment/201203/15/0_13318079100Kr0.gif)