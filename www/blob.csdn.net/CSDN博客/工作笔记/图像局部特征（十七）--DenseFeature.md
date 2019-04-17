# 图像局部特征（十七）--DenseFeature - 工作笔记 - CSDN博客





2016年07月19日 09:58:18[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6088
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

http://blog.csdn.net/zhaocj/article/details/45198965

DenseFeatureDetector可以生成具有一定密度和规律分布的图像特征点，它主要用于3D VIZ。

DenseFeatureDetector的原理是，把输入图像分割成大小相等的网格，每一个网格提取一个像素作为特征点。类似于图像尺度金字塔，该方法也可以生成不同层图像的特征点，每一层图像所分割的网格大小是不同的，即表示各层的尺度不同。



下面我们就来分析它的源码。

DenseFeatureDetector类的构造函数：





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/45198965#)[copy](http://blog.csdn.net/zhaocj/article/details/45198965#)[print](http://blog.csdn.net/zhaocj/article/details/45198965#)[?](http://blog.csdn.net/zhaocj/article/details/45198965#)
- DenseFeatureDetector::DenseFeatureDetector( float _initFeatureScale, int _featureScaleLevels,  
-                                       float _featureScaleMul, int _initXyStep,  
-                                       int _initImgBound, bool _varyXyStepWithScale,  
-                                       bool _varyImgBoundWithScale ) :  
-     initFeatureScale(_initFeatureScale), featureScaleLevels(_featureScaleLevels),  
-     featureScaleMul(_featureScaleMul), initXyStep(_initXyStep), initImgBound(_initImgBound),  
-     varyXyStepWithScale(_varyXyStepWithScale), varyImgBoundWithScale(_varyImgBoundWithScale)  
- {}  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
DenseFeatureDetector::DenseFeatureDetector( float _initFeatureScale, int _featureScaleLevels,
                                      float _featureScaleMul, int _initXyStep,
                                      int _initImgBound, bool _varyXyStepWithScale,
                                      bool _varyImgBoundWithScale ) :
    initFeatureScale(_initFeatureScale), featureScaleLevels(_featureScaleLevels),
    featureScaleMul(_featureScaleMul), initXyStep(_initXyStep), initImgBound(_initImgBound),
    varyXyStepWithScale(_varyXyStepWithScale), varyImgBoundWithScale(_varyImgBoundWithScale)
{}
```







initFeatureScale表示初始图像层特征点的尺度，默认为1



featureScaleLevels表示需要构建多少层图像，默认为1



featureScaleMul表示各层图像之间参数的比例系数，该系数等于相邻两层图像之间的网格宽度之比，尺度之比，以及预留边界宽度之比，默认为0.1



initXyStep表示初始图像层的网格宽度，默认为6



initImgBound表示初始图像层的预留边界宽度，默认为0



varyXyStepWithScale表示各层图像是否进行网格宽度的调整，如果为false，则表示各层图像网格宽度都是initXyStep，如果为true，则表示各层图像网格宽度不等，它们之间的比例系数为featureScaleMul，默认为true



varyImgBoundWithScale表示各层图像是否进行预留边界宽度的调整，如果为false，则表示各层图像预留边界宽度都是initImgBound，如果为true，则表示各层图像预留边界宽度不等，它们之间的比例系数为featureScaleMul，默认为false



下面是检测特征点函数detectImpl：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/45198965#)[copy](http://blog.csdn.net/zhaocj/article/details/45198965#)[print](http://blog.csdn.net/zhaocj/article/details/45198965#)[?](http://blog.csdn.net/zhaocj/article/details/45198965#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- void DenseFeatureDetector::detectImpl( const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask ) const  
- {  
-     // curScale表示当前层图像特征点的尺度  
-     float curScale = static_cast<float>(initFeatureScale);  
-     //curStep表示当前层图像的网格宽度  
-     int curStep = initXyStep;  
-     //curBound表示当前层图像的预留边界宽度  
-     int curBound = initImgBound;  
-     //遍历各层图像  
-     for( int curLevel = 0; curLevel < featureScaleLevels; curLevel++ )  
-     {  
-         //遍历当前层图像的所有网格，图像四周的预留边界处是没有网格的，横、纵坐标的步长就是网格的宽度  
-         for( int x = curBound; x < image.cols - curBound; x += curStep )  
-         {  
-             for( int y = curBound; y < image.rows - curBound; y += curStep )  
-             {  
-                 //把网格的左上角坐标处的像素作为该网格的特征点，并保存  
-                 keypoints.push_back( KeyPoint(static_cast<float>(x), static_cast<float>(y), curScale) );  
-             }  
-         }  
-         //调整下一层图像特征点的尺度  
-         curScale = static_cast<float>(curScale * featureScaleMul);  
-         //如果varyXyStepWithScale为true，则调整下一层图像的网格宽度  
-         if( varyXyStepWithScale ) curStep = static_cast<int>( curStep * featureScaleMul + 0.5f );  
-         //如果varyImgBoundWithScale为true，则调整下一层图像的预留边界宽度  
-         if( varyImgBoundWithScale ) curBound = static_cast<int>( curBound * featureScaleMul + 0.5f );  
-     }  
-     //掩码矩阵的特征点处理  
-     KeyPointsFilter::runByPixelsMask( keypoints, mask );  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void DenseFeatureDetector::detectImpl( const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask ) const
{
    // curScale表示当前层图像特征点的尺度
    float curScale = static_cast<float>(initFeatureScale);
    //curStep表示当前层图像的网格宽度
    int curStep = initXyStep;
    //curBound表示当前层图像的预留边界宽度
    int curBound = initImgBound;
    //遍历各层图像
    for( int curLevel = 0; curLevel < featureScaleLevels; curLevel++ )
    {
        //遍历当前层图像的所有网格，图像四周的预留边界处是没有网格的，横、纵坐标的步长就是网格的宽度
        for( int x = curBound; x < image.cols - curBound; x += curStep )
        {
            for( int y = curBound; y < image.rows - curBound; y += curStep )
            {
                //把网格的左上角坐标处的像素作为该网格的特征点，并保存
                keypoints.push_back( KeyPoint(static_cast<float>(x), static_cast<float>(y), curScale) );
            }
        }
        //调整下一层图像特征点的尺度
        curScale = static_cast<float>(curScale * featureScaleMul);
        //如果varyXyStepWithScale为true，则调整下一层图像的网格宽度
        if( varyXyStepWithScale ) curStep = static_cast<int>( curStep * featureScaleMul + 0.5f );
        //如果varyImgBoundWithScale为true，则调整下一层图像的预留边界宽度
        if( varyImgBoundWithScale ) curBound = static_cast<int>( curBound * featureScaleMul + 0.5f );
    }
    //掩码矩阵的特征点处理
    KeyPointsFilter::runByPixelsMask( keypoints, mask );
}
```

下面给出一个具体的应用实例：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/45198965#)[copy](http://blog.csdn.net/zhaocj/article/details/45198965#)[print](http://blog.csdn.net/zhaocj/article/details/45198965#)[?](http://blog.csdn.net/zhaocj/article/details/45198965#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
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
-    Mat img = imread("box_in_scene.png"), img1;;  
- 
-    cvtColor( img, img1, CV_BGR2GRAY );  
- 
-    DenseFeatureDetector dense;  
- 
-    vector<KeyPoint> key_points;  
-    Mat output_img;  
- 
-    dense.detect(img1,key_points,Mat());  
-    drawKeypoints(img, key_points, output_img, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);  
- 
-    namedWindow("DENSE");  
-    imshow("DENSE", output_img);  
-    waitKey(0);  
- 
-    return 0;  
- }  







