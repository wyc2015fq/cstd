# 最新论文阅读（21）--Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convoluti - wydbyxr的博客 - CSDN博客
2018年06月09日 18:51:44[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：197
# Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convolutional Neural Network
```
- 2016年9月   
- 超分辨率
- Twitter
```
　　sub-pixel convolution layer，它可以学习一组而不是一个上采样滤波器，从低分辨率的特征图得到高分辨率图像。
### 框架
　　提出了一种新型的CNN框架，用于实现在低分辨率的图像上提取特征并进行超分辨处理。而之前的超分辨方法需将低分辨率图像上采样至高分辨率图像的尺寸，再使用滤波器进行双线性插值，这种方式容易陷入局部最优且计算量较大。 
　　本篇文章提出了一种叫做 sub-pixel convolution layer，它可以学习一组而不是一个上采样滤波器，从低分辨率的特征图得到高分辨率图像。 
![这里写图片描述](https://img-blog.csdn.net/20180609185055586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 实验
　　这种方法既取得比之前方法更好的效果，处理速度也比之前基于CNN的超分辨率方法更快。 
![这里写图片描述](https://img-blog.csdn.net/20180609185132740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
