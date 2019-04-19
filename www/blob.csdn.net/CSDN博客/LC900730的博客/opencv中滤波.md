# opencv中滤波 - LC900730的博客 - CSDN博客
2017年09月28日 20:50:43[lc900730](https://me.csdn.net/LC900730)阅读数：204标签：[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[opencv](https://blog.csdn.net/LC900730/article/category/7199508)
邻域滤波(卷积)：左边图像与中间图像的卷积产生右边图像。线性滤波处理的输出像素值g(i,j)是输入像素值f(i+k,j+I)的加权和，如下： 
g(i,j)=∑f(i+k,j+I)h(k,I) 
其中的h(k,l),我们称之为“核”，是滤波器的加权系数，即滤波器的滤波系数。 
g=f卷积h；
## 方框滤波
方框滤波(box Filter)被封装在一个名为boxblur的函数中，即boxblur函数作用是使用方框滤波器(box filter)来模糊一张图片，从src输入，从dst输出。
```
void boxFilter(
    //源图像
    //该函数对通道是独立处理的，且可以处理人员通道的图片
    //CV_8U、CV_16U、CV_16S、CV_32F、CV_64F
    InputArray src,
    //OutputArray类型的dst，需要和源图片一样的尺寸和类型
    OutputArray dst,
    //输出图像的深度
    //-1代表原图深度，src.depth()
    int ddepth,
    //Size类型的ksize，内核的大小
    //  一般用Size(w,h),w为像素宽度，h为像素高度
    Size ksize,
    //表示锚点，默认(-1,-1)
    Point anchor=Point(-1,-1),
    //表示内核是否被归一化
    bool normalize=true,
    //int类型的borderType，用于推断图像外部像素的某种边界模式
    //默认值为BORDER_DEFAULT 
    int borderType=BORDER_DEFAULT
)
```
## 高斯滤波
高斯滤波是一种线性平滑滤波，可以消除高斯噪声，广泛应用于图像处理的减躁过程。通俗来讲就是：对整幅图像进行加权平均的过程，每个像素点的值，都由其本身和邻域内的其他像素值经过加权平均后得到。 
作用：用一个模板(或称卷积、掩模)扫描图像中的每一个像素，用模板确定的邻域内像素的加权平均灰度值去替代模板中心像素点的值。
卷积和协相关差别是：卷积需要先对滤波矩阵进行180°翻转，但是如果矩阵是对称的，那么两者没有什么差别。
