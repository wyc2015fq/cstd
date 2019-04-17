# 图像局部特征（十六）--SimpleBlobDetector - 工作笔记 - CSDN博客





2016年07月19日 09:53:47[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10196
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

http://blog.csdn.net/zhaocj/article/details/44886475

倒是觉得此文中对于圆识别的各种约束条件，挺有用。



Opencv中提供了SimpleBlobDetector的特征点检测方法，正如它的名称，该算法使用最简单的方式来检测斑点类的特征点。下面我们就来分析一下该算法。

首先通过一系列连续的阈值把输入的灰度图像转换为一个二值图像的集合，阈值范围为[T1,T2]，步长为t，则所有阈值为：

T1，T1+t，T1+2t，T1+3t，……，T2              （1）

第二步是利用Suzuki提出的算法通过检测每一幅二值图像的边界的方式提取出每一幅二值图像的连通区域，我们可以认为由边界所围成的不同的连通区域就是该二值图像的斑点；

第三步是根据所有二值图像斑点的中心坐标对二值图像斑点进行分类，从而形成灰度图像的斑点，属于一类的那些二值图像斑点最终形成灰度图像的斑点，具体来说就是，灰度图像的斑点是由中心坐标间的距离小于阈值Tb的那些二值图像斑点所组成的，即这些二值图像斑点属于该灰度图像斑点；

最后就是确定灰度图像斑点的信息——位置和尺寸。位置是属于该灰度图像斑点的所有二值图像斑点中心坐标的加权和，即公式2，权值q等于该二值图像斑点的惯性率的平方，它的含义是二值图像的斑点的形状越接近圆形，越是我们所希望的斑点，因此对灰度图像斑点位置的贡献就越大。尺寸则是属于该灰度图像斑点的所有二值图像斑点中面积大小居中的半径长度。

![](https://img-blog.csdn.net/20160329100324312)         （2）

在第二步中，并不是所有的二值图像的连通区域都可以认为是二值图像的斑点，我们往往通过一些限定条件来得到更准确的斑点。这些限定条件包括颜色，面积和形状，斑点的形状又可以用圆度，偏心率，或凸度来表示。

对于二值图像来说，只有两种斑点颜色——白色斑点和黑色斑点，我们只需要一种颜色的斑点，通过确定斑点的灰度值就可以区分出斑点的颜色。

连通区域的面积太大和太小都不是斑点，所以我们需要计算连通区域的面积，只有当该面积在我们所设定的最大面积和最小面积之间时，该连通区域才作为斑点被保留下来。

圆形的斑点是最理想的，任意形状的圆度C定义为：

![](https://img-blog.csdn.net/20160329100611004)（3）

其中，S和p分别表示该形状的面积和周长，当C为1时，表示该形状是一个完美的圆形，而当C为0时，表示该形状是一个逐渐拉长的多边形。

偏心率是指某一椭圆轨道与理想圆形的偏离程度，长椭圆轨道的偏心率高，而近于圆形的轨道的偏心率低。圆形的偏心率等于0，椭圆的偏心率介于0和1之间，而偏心率等于1表示的是抛物线。直接计算斑点的偏心率较为复杂，但利用图像矩的概念计算图形的惯性率，再由惯性率计算偏心率较为方便。偏心率E和惯性率I之间的关系为：

![](https://img-blog.csdn.net/20160329100728786)（4）

因此圆形的惯性率等于1，惯性率越接近1，圆形的程度越高。

最后一个表示斑点形状的量是凸度。在平面中，凸形图指的是图形的所有部分都在由该图形切线所围成的区域的内部。我们可以用凸度来表示斑点凹凸的程度，凸度V的定义为：

![](https://img-blog.csdn.net/20160329100853960)（5）

其中，H表示该斑点的凸壳面积

在计算斑点的面积，中心处的坐标，尤其是惯性率时，都可以应用图像矩的方法。

下面我们就介绍该方法。

矩在统计学中被用来反映随机变量的分布情况，推广到力学中，它被用来描述空间物体的质量分布。同样的道理，如果我们将图像的灰度值看作是一个二维的密度分布函数，那么矩方法即可用于图像处理领域。设*f*(*x*,*y*)是一幅数字图像，则它的矩*Mij*为：



![](https://img-blog.csdn.net/20160329101053337)  （6）

对于二值图像的来说，零阶矩*M*00等于它的面积。图形的质心为：

![](https://img-blog.csdn.net/20160329101205260) （7）

图像的中心矩*μpq*定义为：

![](https://img-blog.csdn.net/20160329101252840)  （8）

一阶中心矩称为静矩，二阶中心矩称为惯性矩。如果仅考虑二阶中心矩的话，则图像完全等同于一个具有确定的大小、方向和离心率，以图像质心为中心且具有恒定辐射度的椭圆。图像的协方差矩阵为：

![](https://img-blog.csdn.net/20160329101400311)      （9）

该矩阵的两个特征值*λ*1和*λ*2对应于图像强度（即椭圆）的主轴和次轴：

![](https://img-blog.csdn.net/20160329101446233)    （10）

![](https://img-blog.csdn.net/20160329101531264)    （11）

而图像的方向角度*θ*为：

![](https://img-blog.csdn.net/20160329101608796)       （12）

图像的惯性率*I*为：

![](https://img-blog.csdn.net/20160329101653688)  （13）



下面给出SimpleBlobDetector的源码分析。我们先来看看SimpleBlobDetector类的默认参数的设置：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/44886475#)[copy](http://blog.csdn.net/zhaocj/article/details/44886475#)[print](http://blog.csdn.net/zhaocj/article/details/44886475#)[?](http://blog.csdn.net/zhaocj/article/details/44886475#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- SimpleBlobDetector::Params::Params()  
- {  
-     thresholdStep = 10;    //二值化的阈值步长，即公式1的t  
-     minThreshold = 50;   //二值化的起始阈值，即公式1的T1  
-     maxThreshold = 220;    //二值化的终止阈值，即公式1的T2  
-     //重复的最小次数，只有属于灰度图像斑点的那些二值图像斑点数量大于该值时，该灰度图像斑点才被认为是特征点  
-     minRepeatability = 2;     
-     //最小的斑点距离，不同二值图像的斑点间距离小于该值时，被认为是同一个位置的斑点，否则是不同位置上的斑点  
-     minDistBetweenBlobs = 10;  
- 
-     filterByColor = true;    //斑点颜色的限制变量  
-     blobColor = 0;    //表示只提取黑色斑点；如果该变量为255，表示只提取白色斑点  
- 
-     filterByArea = true;    //斑点面积的限制变量  
-     minArea = 25;    //斑点的最小面积  
-     maxArea = 5000;    //斑点的最大面积  
- 
-     filterByCircularity = false;    //斑点圆度的限制变量，默认是不限制  
-     minCircularity = 0.8f;    //斑点的最小圆度  
-     //斑点的最大圆度，所能表示的float类型的最大值  
-     maxCircularity = std::numeric_limits<float>::max();  
- 
-     filterByInertia = true;    //斑点惯性率的限制变量  
-     //minInertiaRatio = 0.6;  
-     minInertiaRatio = 0.1f;    //斑点的最小惯性率  
-     maxInertiaRatio = std::numeric_limits<float>::max();    //斑点的最大惯性率  
- 
-     filterByConvexity = true;    //斑点凸度的限制变量  
-     //minConvexity = 0.8;  
-     minConvexity = 0.95f;    //斑点的最小凸度  
-     maxConvexity = std::numeric_limits<float>::max();    //斑点的最大凸度  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
SimpleBlobDetector::Params::Params()
{
    thresholdStep = 10;    //二值化的阈值步长，即公式1的t
    minThreshold = 50;   //二值化的起始阈值，即公式1的T1
    maxThreshold = 220;    //二值化的终止阈值，即公式1的T2
    //重复的最小次数，只有属于灰度图像斑点的那些二值图像斑点数量大于该值时，该灰度图像斑点才被认为是特征点
    minRepeatability = 2;   
    //最小的斑点距离，不同二值图像的斑点间距离小于该值时，被认为是同一个位置的斑点，否则是不同位置上的斑点
    minDistBetweenBlobs = 10;

    filterByColor = true;    //斑点颜色的限制变量
    blobColor = 0;    //表示只提取黑色斑点；如果该变量为255，表示只提取白色斑点

    filterByArea = true;    //斑点面积的限制变量
    minArea = 25;    //斑点的最小面积
    maxArea = 5000;    //斑点的最大面积

    filterByCircularity = false;    //斑点圆度的限制变量，默认是不限制
    minCircularity = 0.8f;    //斑点的最小圆度
    //斑点的最大圆度，所能表示的float类型的最大值
    maxCircularity = std::numeric_limits<float>::max();

    filterByInertia = true;    //斑点惯性率的限制变量
    //minInertiaRatio = 0.6;
    minInertiaRatio = 0.1f;    //斑点的最小惯性率
    maxInertiaRatio = std::numeric_limits<float>::max();    //斑点的最大惯性率

    filterByConvexity = true;    //斑点凸度的限制变量
    //minConvexity = 0.8;
    minConvexity = 0.95f;    //斑点的最小凸度
    maxConvexity = std::numeric_limits<float>::max();    //斑点的最大凸度
}
```

我们再来介绍检测二值图像斑点的函数findBlobs。

**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/44886475#)[copy](http://blog.csdn.net/zhaocj/article/details/44886475#)[print](http://blog.csdn.net/zhaocj/article/details/44886475#)[?](http://blog.csdn.net/zhaocj/article/details/44886475#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- //image为输入的灰度图像  
- //binaryImage为二值图像  
- //centers表示该二值图像的斑点  
- void SimpleBlobDetector::findBlobs(const cv::Mat &image, const cv::Mat &binaryImage, vector<Center> ¢ers) const  
- {  
-     (void)image;  
-     centers.clear();    //斑点变量清零  
- 
-     vector < vector<Point> > contours;    //定义二值图像的斑点的边界像素变量  
-     Mat tmpBinaryImage = binaryImage.clone();    //复制二值图像  
-     //调用findContours函数，找到当前二值图像的所有斑点的边界  
-     findContours(tmpBinaryImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);  
- 
- #ifdef DEBUG_BLOB_DETECTOR  
-     //  Mat keypointsImage;  
-     //  cvtColor( binaryImage, keypointsImage, CV_GRAY2RGB );  
-     //  
-     //  Mat contoursImage;  
-     //  cvtColor( binaryImage, contoursImage, CV_GRAY2RGB );  
-     //  drawContours( contoursImage, contours, -1, Scalar(0,255,0) );  
-     //  imshow("contours", contoursImage );  
- #endif  
-     //遍历当前二值图像的所有斑点  
-     for (size_t contourIdx = 0; contourIdx < contours.size(); contourIdx++)  
-     {  
-         //结构类型Center代表着斑点，它包括斑点的中心位置，半径和权值  
-         Center center;    //斑点变量  
-         //初始化斑点中心的置信度，也就是该斑点的权值  
-         center.confidence = 1;  
-         //调用moments函数，得到当前斑点的矩  
-         Moments moms = moments(Mat(contours[contourIdx]));  
-         if (params.filterByArea)    //斑点面积的限制  
-         {  
-             double area = moms.m00;    //零阶矩即为二值图像的面积  
-             //如果面积超出了设定的范围，则不再考虑该斑点  
-             if (area < params.minArea || area >= params.maxArea)  
-                 continue;  
-         }  
- 
-         if (params.filterByCircularity)    //斑点圆度的限制  
-         {  
-             double area = moms.m00;    //得到斑点的面积  
-             //计算斑点的周长  
-             double perimeter = arcLength(Mat(contours[contourIdx]), true);  
-             //由公式3得到斑点的圆度  
-             double ratio = 4 * CV_PI * area / (perimeter * perimeter);  
-             //如果圆度超出了设定的范围，则不再考虑该斑点  
-             if (ratio < params.minCircularity || ratio >= params.maxCircularity)  
-                 continue;  
-         }  
- 
-         if (params.filterByInertia)    //斑点惯性率的限制  
-         {  
-             //计算公式13中最右侧等式中的开根号的值  
-             double denominator = sqrt(pow(2 * moms.mu11, 2) + pow(moms.mu20 - moms.mu02, 2));  
-             const double eps = 1e-2;    //定义一个极小值  
-             double ratio;  
-             if (denominator > eps)  
-             {  
-                 //cosmin和sinmin用于计算图像协方差矩阵中较小的那个特征值λ2  
-                 double cosmin = (moms.mu20 - moms.mu02) / denominator;  
-                 double sinmin = 2 * moms.mu11 / denominator;  
-                 //cosmin和sinmin用于计算图像协方差矩阵中较大的那个特征值λ1  
-                 double cosmax = -cosmin;  
-                 double sinmax = -sinmin;  
-                 //imin为λ2乘以零阶中心矩μ00  
-                 double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;  
-                 //imax为λ1乘以零阶中心矩μ00  
-                 double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;  
-                 ratio = imin / imax;    //得到斑点的惯性率  
-             }  
-             else  
-             {  
-                 ratio = 1;    //直接设置为1，即为圆  
-             }  
-             //如果惯性率超出了设定的范围，则不再考虑该斑点  
-             if (ratio < params.minInertiaRatio || ratio >= params.maxInertiaRatio)  
-                 continue;  
-             //斑点中心的权值定义为惯性率的平方  
-             center.confidence = ratio * ratio;  
-         }  
- 
-         if (params.filterByConvexity)    //斑点凸度的限制  
-         {  
-             vector < Point > hull;    //定义凸壳变量  
-             //调用convexHull函数，得到该斑点的凸壳  
-             convexHull(Mat(contours[contourIdx]), hull);  
-             //分别得到斑点和凸壳的面积，contourArea函数本质上也是求图像的零阶矩  
-             double area = contourArea(Mat(contours[contourIdx]));  
-             double hullArea = contourArea(Mat(hull));  
-             double ratio = area / hullArea;    //公式5，计算斑点的凸度  
-             //如果凸度超出了设定的范围，则不再考虑该斑点  
-             if (ratio < params.minConvexity || ratio >= params.maxConvexity)  
-                 continue;  
-         }  
- 
-         //根据公式7，计算斑点的质心  
-         center.location = Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);  
- 
-         if (params.filterByColor)    //斑点颜色的限制  
-         {  
-             //判断一下斑点的颜色是否正确  
-             if (binaryImage.at<uchar> (cvRound(center.location.y), cvRound(center.location.x)) != params.blobColor)  
-                 continue;  
-         }  
- 
-         //compute blob radius  
-         {  
-             vector<double> dists;    //定义距离队列  
-             //遍历该斑点边界上的所有像素  
-             for (size_t pointIdx = 0; pointIdx < contours[contourIdx].size(); pointIdx++)  
-             {  
-                 Point2d pt = contours[contourIdx][pointIdx];    //得到边界像素坐标  
-                 //计算该点坐标与斑点中心的距离，并放入距离队列中  
-                 dists.push_back(norm(center.location - pt));  
-             }  
-             std::sort(dists.begin(), dists.end());    //距离队列排序  
-             //计算斑点的半径，它等于距离队列中中间两个距离的平均值  
-             center.radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;  
-         }  
- 
-         centers.push_back(center);    //把center变量压入centers队列中  
- 
- #ifdef DEBUG_BLOB_DETECTOR  
-         //    circle( keypointsImage, center.location, 1, Scalar(0,0,255), 1 );  
- #endif  
-     }  
- #ifdef DEBUG_BLOB_DETECTOR  
-     //  imshow("bk", keypointsImage );  
-     //  waitKey();  
- #endif  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
//image为输入的灰度图像
//binaryImage为二值图像
//centers表示该二值图像的斑点
void SimpleBlobDetector::findBlobs(const cv::Mat &image, const cv::Mat &binaryImage, vector<Center> ¢ers) const
{
    (void)image;
    centers.clear();    //斑点变量清零

    vector < vector<Point> > contours;    //定义二值图像的斑点的边界像素变量
    Mat tmpBinaryImage = binaryImage.clone();    //复制二值图像
    //调用findContours函数，找到当前二值图像的所有斑点的边界
    findContours(tmpBinaryImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

#ifdef DEBUG_BLOB_DETECTOR
    //  Mat keypointsImage;
    //  cvtColor( binaryImage, keypointsImage, CV_GRAY2RGB );
    //
    //  Mat contoursImage;
    //  cvtColor( binaryImage, contoursImage, CV_GRAY2RGB );
    //  drawContours( contoursImage, contours, -1, Scalar(0,255,0) );
    //  imshow("contours", contoursImage );
#endif
    //遍历当前二值图像的所有斑点
    for (size_t contourIdx = 0; contourIdx < contours.size(); contourIdx++)
    {
        //结构类型Center代表着斑点，它包括斑点的中心位置，半径和权值
        Center center;    //斑点变量
        //初始化斑点中心的置信度，也就是该斑点的权值
        center.confidence = 1;
        //调用moments函数，得到当前斑点的矩
        Moments moms = moments(Mat(contours[contourIdx]));
        if (params.filterByArea)    //斑点面积的限制
        {
            double area = moms.m00;    //零阶矩即为二值图像的面积
            //如果面积超出了设定的范围，则不再考虑该斑点
            if (area < params.minArea || area >= params.maxArea)
                continue;
        }

        if (params.filterByCircularity)    //斑点圆度的限制
        {
            double area = moms.m00;    //得到斑点的面积
            //计算斑点的周长
            double perimeter = arcLength(Mat(contours[contourIdx]), true);
            //由公式3得到斑点的圆度
            double ratio = 4 * CV_PI * area / (perimeter * perimeter);
            //如果圆度超出了设定的范围，则不再考虑该斑点
            if (ratio < params.minCircularity || ratio >= params.maxCircularity)
                continue;
        }

        if (params.filterByInertia)    //斑点惯性率的限制
        {
            //计算公式13中最右侧等式中的开根号的值
            double denominator = sqrt(pow(2 * moms.mu11, 2) + pow(moms.mu20 - moms.mu02, 2));
            const double eps = 1e-2;    //定义一个极小值
            double ratio;
            if (denominator > eps)
            {
                //cosmin和sinmin用于计算图像协方差矩阵中较小的那个特征值λ2
                double cosmin = (moms.mu20 - moms.mu02) / denominator;
                double sinmin = 2 * moms.mu11 / denominator;
                //cosmin和sinmin用于计算图像协方差矩阵中较大的那个特征值λ1
                double cosmax = -cosmin;
                double sinmax = -sinmin;
                //imin为λ2乘以零阶中心矩μ00
                double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
                //imax为λ1乘以零阶中心矩μ00
                double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
                ratio = imin / imax;    //得到斑点的惯性率
            }
            else
            {
                ratio = 1;    //直接设置为1，即为圆
            }
            //如果惯性率超出了设定的范围，则不再考虑该斑点
            if (ratio < params.minInertiaRatio || ratio >= params.maxInertiaRatio)
                continue;
            //斑点中心的权值定义为惯性率的平方
            center.confidence = ratio * ratio;
        }

        if (params.filterByConvexity)    //斑点凸度的限制
        {
            vector < Point > hull;    //定义凸壳变量
            //调用convexHull函数，得到该斑点的凸壳
            convexHull(Mat(contours[contourIdx]), hull);
            //分别得到斑点和凸壳的面积，contourArea函数本质上也是求图像的零阶矩
            double area = contourArea(Mat(contours[contourIdx]));
            double hullArea = contourArea(Mat(hull));
            double ratio = area / hullArea;    //公式5，计算斑点的凸度
            //如果凸度超出了设定的范围，则不再考虑该斑点
            if (ratio < params.minConvexity || ratio >= params.maxConvexity)
                continue;
        }

        //根据公式7，计算斑点的质心
        center.location = Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);

        if (params.filterByColor)    //斑点颜色的限制
        {
            //判断一下斑点的颜色是否正确
            if (binaryImage.at<uchar> (cvRound(center.location.y), cvRound(center.location.x)) != params.blobColor)
                continue;
        }

        //compute blob radius
        {
            vector<double> dists;    //定义距离队列
            //遍历该斑点边界上的所有像素
            for (size_t pointIdx = 0; pointIdx < contours[contourIdx].size(); pointIdx++)
            {
                Point2d pt = contours[contourIdx][pointIdx];    //得到边界像素坐标
                //计算该点坐标与斑点中心的距离，并放入距离队列中
                dists.push_back(norm(center.location - pt));
            }
            std::sort(dists.begin(), dists.end());    //距离队列排序
            //计算斑点的半径，它等于距离队列中中间两个距离的平均值
            center.radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;
        }

        centers.push_back(center);    //把center变量压入centers队列中

#ifdef DEBUG_BLOB_DETECTOR
        //    circle( keypointsImage, center.location, 1, Scalar(0,0,255), 1 );
#endif
    }
#ifdef DEBUG_BLOB_DETECTOR
    //  imshow("bk", keypointsImage );
    //  waitKey();
#endif
}
```

最后介绍检测特征点的函数detectImpl。

**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/44886475#)[copy](http://blog.csdn.net/zhaocj/article/details/44886475#)[print](http://blog.csdn.net/zhaocj/article/details/44886475#)[?](http://blog.csdn.net/zhaocj/article/details/44886475#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- void SimpleBlobDetector::detectImpl(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, const cv::Mat&) const  
- {  
-     //TODO: support mask  
-     keypoints.clear();    //特征点变量清零  
-     Mat grayscaleImage;  
-     //把彩色图像转换为二值图像  
-     if (image.channels() == 3)  
-         cvtColor(image, grayscaleImage, CV_BGR2GRAY);  
-     else  
-         grayscaleImage = image;  
-     //二维数组centers表示所有得到的斑点，第一维数据表示的是灰度图像斑点，第二维数据表示的是属于该灰度图像斑点的所有二值图像斑点   
-     //结构类型Center代表着斑点，它包括斑点的中心位置，半径和权值  
-     vector < vector<Center> > centers;  
-     //遍历所有阈值，进行二值化处理  
-     for (double thresh = params.minThreshold; thresh < params.maxThreshold; thresh += params.thresholdStep)  
-     {  
-         Mat binarizedImage;  
-         //调用threshold函数，把灰度图像grayscaleImage转换为二值图像binarizedImage  
-         threshold(grayscaleImage, binarizedImage, thresh, 255, THRESH_BINARY);  
- 
- #ifdef DEBUG_BLOB_DETECTOR  
-         //    Mat keypointsImage;  
-         //    cvtColor( binarizedImage, keypointsImage, CV_GRAY2RGB );  
- #endif  
-         //变量curCenters表示该二值图像内的所有斑点  
-         vector < Center > curCenters;  
-         //调用findBlobs函数，对二值图像binarizedImage检测斑点，得到curCenters  
-         findBlobs(grayscaleImage, binarizedImage, curCenters);  
-         //newCenters表示在当前二值图像内检测到的不属于已有灰度图像斑点的那些二值图像斑点  
-         vector < vector<Center> > newCenters;  
-         //遍历该二值图像内的所有斑点  
-         for (size_t i = 0; i < curCenters.size(); i++)  
-         {  
- #ifdef DEBUG_BLOB_DETECTOR  
-             //      circle(keypointsImage, curCenters[i].location, curCenters[i].radius, Scalar(0,0,255),-1);  
- #endif  
-             // isNew表示的是当前二值图像斑点是否为新出现的斑点  
-             bool isNew = true;  
-             //遍历已有的所有灰度图像斑点，判断该二值图像斑点是否为新的灰度图像斑点  
-             for (size_t j = 0; j < centers.size(); j++)  
-             {  
-                 //属于该灰度图像斑点的中间位置的那个二值图像斑点代表该灰度图像斑点，并把它的中心坐标与当前二值图像斑点的中心坐标比较，计算它们的距离dist  
-                 double dist = norm(centers[j][ centers[j].size() / 2 ].location - curCenters[i].location);  
-                 //如果距离大于所设的阈值，并且距离都大于上述那两个二值图像斑点的半径，则表示该二值图像的斑点是新的斑点  
-                 isNew = dist >= params.minDistBetweenBlobs && dist >= centers[j][ centers[j].size() / 2 ].radius && dist >= curCenters[i].radius;  
-                 //如果不是新的斑点，则需要把它添加到属于它的当前（即第j个）灰度图像的斑点中，因为通过上面的距离比较可知，当前二值图像斑点属于当前灰度图像斑点  
-                 if (!isNew)  
-                 {  
-                     //把当前二值图像斑点存入当前（即第j个）灰度图像斑点数组的最后位置  
-                     centers[j].push_back(curCenters[i]);  
-                     //得到构成该灰度图像斑点的所有二值图像斑点的数量  
-                     size_t k = centers[j].size() - 1;  
-                     //按照半径由小至大的顺序，把新得到的当前二值图像斑点放入当前灰度图像斑点数组的适当位置处，由于二值化阈值是按照从小到大的顺序设置，所以二值图像斑点本身就是按照面积的大小顺序被检测到的，因此此处的排序处理要相对简单一些  
-                     while( k > 0 && centers[j][k].radius < centers[j][k-1].radius )  
-                     {  
-                         centers[j][k] = centers[j][k-1];  
-                         k--;  
-                     }  
-                     centers[j][k] = curCenters[i];  
-                     //由于当前二值图像斑点已经找到了属于它的灰度图像斑点，因此退出for循环，无需再遍历已有的灰度图像斑点  
-                     break;  
-                 }  
-             }  
-             if (isNew)    //当前二值图像斑点是新的斑点  
-             {  
-                 //把当前斑点存入newCenters数组内  
-                 newCenters.push_back(vector<Center> (1, curCenters[i]));  
-                 //centers.push_back(vector<Center> (1, curCenters[i]));  
-             }  
-         }  
-         //把当前二值图像内的所有newCenters复制到centers内  
-         std::copy(newCenters.begin(), newCenters.end(), std::back_inserter(centers));  
- 
- #ifdef DEBUG_BLOB_DETECTOR  
-         //    imshow("binarized", keypointsImage );  
-         //waitKey();  
- #endif  
-     }    //所有二值图像斑点检测完毕  
-     //遍历所有灰度图像斑点，得到特征点信息  
-     for (size_t i = 0; i < centers.size(); i++)  
-     {  
-         //如果属于当前灰度图像斑点的二值图像斑点的数量小于所设阈值，则该灰度图像斑点不是特征点  
-         if (centers[i].size() < params.minRepeatability)  
-             continue;  
-         Point2d sumPoint(0, 0);  
-         double normalizer = 0;  
-         //遍历属于当前灰度图像斑点的所有二值图像斑点  
-         for (size_t j = 0; j < centers[i].size(); j++)  
-         {  
-             sumPoint += centers[i][j].confidence * centers[i][j].location;    //公式2的分子  
-             normalizer += centers[i][j].confidence;    //公式2的分母  
-         }  
-         sumPoint *= (1. / normalizer);    //公式2，得到特征点的坐标位置  
-         //保存该特征点的坐标位置和尺寸大小  
-         KeyPoint kpt(sumPoint, (float)(centers[i][centers[i].size() / 2].radius));  
-         keypoints.push_back(kpt);    //保存该特征点  
-     }  
- 
- #ifdef DEBUG_BLOB_DETECTOR  
-     namedWindow("keypoints", CV_WINDOW_NORMAL);  
-     Mat outImg = image.clone();  
-     for(size_t i=0; i<keypoints.size(); i++)  
-     {  
-         circle(outImg, keypoints[i].pt, keypoints[i].size, Scalar(255, 0, 255), -1);  
-     }  
-     //drawKeypoints(image, keypoints, outImg);  
-     imshow("keypoints", outImg);  
-     waitKey();  
- #endif  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void SimpleBlobDetector::detectImpl(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, const cv::Mat&) const
{
    //TODO: support mask
    keypoints.clear();    //特征点变量清零
    Mat grayscaleImage;
    //把彩色图像转换为二值图像
    if (image.channels() == 3)
        cvtColor(image, grayscaleImage, CV_BGR2GRAY);
    else
        grayscaleImage = image;
    //二维数组centers表示所有得到的斑点，第一维数据表示的是灰度图像斑点，第二维数据表示的是属于该灰度图像斑点的所有二值图像斑点 
    //结构类型Center代表着斑点，它包括斑点的中心位置，半径和权值
    vector < vector<Center> > centers;
    //遍历所有阈值，进行二值化处理
    for (double thresh = params.minThreshold; thresh < params.maxThreshold; thresh += params.thresholdStep)
    {
        Mat binarizedImage;
        //调用threshold函数，把灰度图像grayscaleImage转换为二值图像binarizedImage
        threshold(grayscaleImage, binarizedImage, thresh, 255, THRESH_BINARY);

#ifdef DEBUG_BLOB_DETECTOR
        //    Mat keypointsImage;
        //    cvtColor( binarizedImage, keypointsImage, CV_GRAY2RGB );
#endif
        //变量curCenters表示该二值图像内的所有斑点
        vector < Center > curCenters;
        //调用findBlobs函数，对二值图像binarizedImage检测斑点，得到curCenters
        findBlobs(grayscaleImage, binarizedImage, curCenters);
        //newCenters表示在当前二值图像内检测到的不属于已有灰度图像斑点的那些二值图像斑点
        vector < vector<Center> > newCenters;
        //遍历该二值图像内的所有斑点
        for (size_t i = 0; i < curCenters.size(); i++)
        {
#ifdef DEBUG_BLOB_DETECTOR
            //      circle(keypointsImage, curCenters[i].location, curCenters[i].radius, Scalar(0,0,255),-1);
#endif
            // isNew表示的是当前二值图像斑点是否为新出现的斑点
            bool isNew = true;
            //遍历已有的所有灰度图像斑点，判断该二值图像斑点是否为新的灰度图像斑点
            for (size_t j = 0; j < centers.size(); j++)
            {
                //属于该灰度图像斑点的中间位置的那个二值图像斑点代表该灰度图像斑点，并把它的中心坐标与当前二值图像斑点的中心坐标比较，计算它们的距离dist
                double dist = norm(centers[j][ centers[j].size() / 2 ].location - curCenters[i].location);
                //如果距离大于所设的阈值，并且距离都大于上述那两个二值图像斑点的半径，则表示该二值图像的斑点是新的斑点
                isNew = dist >= params.minDistBetweenBlobs && dist >= centers[j][ centers[j].size() / 2 ].radius && dist >= curCenters[i].radius;
                //如果不是新的斑点，则需要把它添加到属于它的当前（即第j个）灰度图像的斑点中，因为通过上面的距离比较可知，当前二值图像斑点属于当前灰度图像斑点
                if (!isNew)
                {
                    //把当前二值图像斑点存入当前（即第j个）灰度图像斑点数组的最后位置
                    centers[j].push_back(curCenters[i]);
                    //得到构成该灰度图像斑点的所有二值图像斑点的数量
                    size_t k = centers[j].size() - 1;
                    //按照半径由小至大的顺序，把新得到的当前二值图像斑点放入当前灰度图像斑点数组的适当位置处，由于二值化阈值是按照从小到大的顺序设置，所以二值图像斑点本身就是按照面积的大小顺序被检测到的，因此此处的排序处理要相对简单一些
                    while( k > 0 && centers[j][k].radius < centers[j][k-1].radius )
                    {
                        centers[j][k] = centers[j][k-1];
                        k--;
                    }
                    centers[j][k] = curCenters[i];
                    //由于当前二值图像斑点已经找到了属于它的灰度图像斑点，因此退出for循环，无需再遍历已有的灰度图像斑点
                    break;
                }
            }
            if (isNew)    //当前二值图像斑点是新的斑点
            {
                //把当前斑点存入newCenters数组内
                newCenters.push_back(vector<Center> (1, curCenters[i]));
                //centers.push_back(vector<Center> (1, curCenters[i]));
            }
        }
        //把当前二值图像内的所有newCenters复制到centers内
        std::copy(newCenters.begin(), newCenters.end(), std::back_inserter(centers));

#ifdef DEBUG_BLOB_DETECTOR
        //    imshow("binarized", keypointsImage );
        //waitKey();
#endif
    }    //所有二值图像斑点检测完毕
    //遍历所有灰度图像斑点，得到特征点信息
    for (size_t i = 0; i < centers.size(); i++)
    {
        //如果属于当前灰度图像斑点的二值图像斑点的数量小于所设阈值，则该灰度图像斑点不是特征点
        if (centers[i].size() < params.minRepeatability)
            continue;
        Point2d sumPoint(0, 0);
        double normalizer = 0;
        //遍历属于当前灰度图像斑点的所有二值图像斑点
        for (size_t j = 0; j < centers[i].size(); j++)
        {
            sumPoint += centers[i][j].confidence * centers[i][j].location;    //公式2的分子
            normalizer += centers[i][j].confidence;    //公式2的分母
        }
        sumPoint *= (1. / normalizer);    //公式2，得到特征点的坐标位置
        //保存该特征点的坐标位置和尺寸大小
        KeyPoint kpt(sumPoint, (float)(centers[i][centers[i].size() / 2].radius));
        keypoints.push_back(kpt);    //保存该特征点
    }

#ifdef DEBUG_BLOB_DETECTOR
    namedWindow("keypoints", CV_WINDOW_NORMAL);
    Mat outImg = image.clone();
    for(size_t i=0; i<keypoints.size(); i++)
    {
        circle(outImg, keypoints[i].pt, keypoints[i].size, Scalar(255, 0, 255), -1);
    }
    //drawKeypoints(image, keypoints, outImg);
    imshow("keypoints", outImg);
    waitKey();
#endif
}
```

下面我们给出一个具体的应用实例。









**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/44886475#)[copy](http://blog.csdn.net/zhaocj/article/details/44886475#)[print](http://blog.csdn.net/zhaocj/article/details/44886475#)[?](http://blog.csdn.net/zhaocj/article/details/44886475#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- #include "opencv2/core/core.hpp"  
- #include "highgui.h"  
- #include "opencv2/imgproc/imgproc.hpp"  
- #include "opencv2/features2d/features2d.hpp"  
- #include "opencv2/nonfree/nonfree.hpp"  
- 
- using namespace cv;  
- //using namespace std;  
- 
- int main(int argc, char** argv)  
- {  
-    Mat img = imread("box_in_scene.png");  
- 
-    SimpleBlobDetector::Params params;    
-    params.minThreshold = 40;    
-    params.maxThreshold = 160;    
-    params.thresholdStep = 5;    
-    params.minArea = 100;    
-    params.minConvexity = .05f;    
-    params.minInertiaRatio = .05f;    
-    params.maxArea = 8000;    
- 
-    SimpleBlobDetector detector(params);  
- 
-    vector<KeyPoint> key_points;  
- 
-    detector.detect(img,key_points);  
- 
-    Mat output_img;  
- 
-    drawKeypoints( img, key_points, output_img, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );  
- 
-    namedWindow("SimpleBlobDetector");  
-    imshow("SimpleBlobDetector", output_img);  
-    waitKey(0);  
- 
-    return 0;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
#include "opencv2/core/core.hpp"
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
//using namespace std;

int main(int argc, char** argv)
{
   Mat img = imread("box_in_scene.png");

   SimpleBlobDetector::Params params;  
   params.minThreshold = 40;  
   params.maxThreshold = 160;  
   params.thresholdStep = 5;  
   params.minArea = 100;  
   params.minConvexity = .05f;  
   params.minInertiaRatio = .05f;  
   params.maxArea = 8000;  

   SimpleBlobDetector detector(params);

   vector<KeyPoint> key_points;

   detector.detect(img,key_points);
   
   Mat output_img;

   drawKeypoints( img, key_points, output_img, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

   namedWindow("SimpleBlobDetector");
   imshow("SimpleBlobDetector", output_img);
   waitKey(0);

   return 0;
}
```



在上述程序中，为了更好的检测到图像的特征点，我们修改了一些默认参数，如果仅仅使用系统的默认参数，则在实例化SimpleBlobDetector时，只需下列代码即可：

SimpleBlobDetector detector;



