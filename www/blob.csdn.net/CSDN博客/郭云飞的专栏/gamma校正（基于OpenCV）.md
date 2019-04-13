
# gamma校正（基于OpenCV） - 郭云飞的专栏 - CSDN博客


2017年10月25日 18:39:10[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：988


Gamma校正是对输入图像灰度值进行的**非线性操作**，使输出图像灰度值与输入图像灰度值呈**指数关系**。
![](http://pic.opencv66.net/2017/2017-10/0020/13.jpg)
指数**γ**即为**Gamma**
经过**Gamma**校正后的输入和输出图像灰度值关系如下图所示：
![](https://img-blog.csdn.net/20171025184133642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
横坐标是输入灰度值，纵坐标是输出灰度值，蓝色曲线是gamma值小于1时的输入输出关系，红色曲线是gamma值大于1时的输入输出关系。
可以观察到，当gamma值小于1时(蓝色曲线)，图像的整体亮度值得到提升，同时低灰度处的对比度增加，高灰度处的对比度降低，更利于分辩低灰度值时的图像细节；
当gamma值大于1时(红色曲线)，图像的整体亮度值得到减小，同时低灰度处的对比度降低，高灰度处的对比度增加，更利于分辩高灰度值时的图像细节。
**函数实现（亲测通过）：**

```python
/************************************************
* gamma correction
* template<...> _Tpsaturate_cast(_Tp2v): Template function for 
* accurate conversion from one primitive type to another
*/
void GammaCorrection(Mat& src, Mat& dst, float fGamma)  
{  
    // build look up table  
    unsigned char lut[256];  
    for( int i = 0; i < 256; i++ ) {  
        lut[i] = saturate_cast<uchar>(pow((float)(i/255.0), fGamma) * 255.0f);  
    }  
  
    dst = src.clone();  
    const int channels = dst.channels();  
    switch(channels)  
    {  
        case 1: { 
            MatIterator_<uchar> it, end;  
            for( it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++ )  
                *it = lut[(*it)];  
            break;  
        }  
        case 3: {  
            MatIterator_<Vec3b> it, end;  
            for( it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++ ) {   
                (*it)[0] = lut[((*it)[0])];  
                (*it)[1] = lut[((*it)[1])];  
                (*it)[2] = lut[((*it)[2])];  
            }  
            break;  
        }  
    }  
}
```
**Demo：**
```python
void demo()
{
	cv::Mat src = cv::imread("./3.jpg");
	cv::Mat dst;
	GammaCorrection(src, dst, 0.45);
	cv::namedWindow("win1");
	cv::imshow("win1", src);
	cv::namedWindow("win2");
	cv::imshow("win2", dst);
	cv::waitKey(0);
}
```


