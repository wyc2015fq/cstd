# OpenCV学习笔记（十八）——图像的各种变换（cvtColor*+）imgproc

2011年11月25日 09:40:41

yang_xian521

阅读数：58666

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

imgproc是OpenCV一个比较复杂的lib，我是分开介绍的，之前介绍过了[滤波器](http://blog.csdn.net/yang_xian521/article/details/6906960)、[直方图](http://blog.csdn.net/yang_xian521/article/details/6927773)、[结构分析和形状描述](http://blog.csdn.net/yang_xian521/article/details/6922798)三节，这次介绍一下图像的变换，OpenCV对于图像的变换又可分为几何变换和其他的变换，我这节先介绍一下其他的变换。

这部分的函数包括adaptiveThreshold，对图像进行自适应的阈值操作。

相应的也有更一般常用的阈值操作函数threshold。

这里还是详细介绍一下cvtColor，这个函数是用来进行颜色空间的转换，随着OpenCV版本的升级，对于颜色空间种类的支持也是越来越多。这里汇总一下，把我知道的全部空间列举出来，也许还不完整，希望大家补充。需要先告诉大家的是OpenCV默认的图片通道是BGR。

> RGB <--> BGR：CV_BGR2BGRA、CV_RGB2BGRA、CV_BGRA2RGBA、CV_BGR2BGRA、CV_BGRA2BGR
>
> RGB <--> 5X5：CV_BGR5652RGBA、CV_BGR2RGB555、（以此类推，不一一列举）

> RGB <---> Gray：CV_RGB2GRAY、CV_GRAY2RGB、CV_RGBA2GRAY、CV_GRAY2RGBA
>
> RGB <--> CIE XYZ：CV_BGR2XYZ、CV_RGB2XYZ、CV_XYZ2BGR、CV_XYZ2RGB
>
> RGB <--> YCrCb（YUV） JPEG：CV_RGB2YCrCb、CV_RGB2YCrCb、CV_YCrCb2BGR、CV_YCrCb2RGB、CV_RGB2YUV（将YCrCb用YUV替代都可以）
>
> RGB <--> HSV：CV_BGR2HSV、CV_RGB2HSV、CV_HSV2BGR、CV_HSV2RGB
>
> RGB <--> HLS：CV_BGR2HLS、CV_RGB2HLS、CV_HLS2BGR、CV_HLS2RGB
>
> RGB <--> CIE L*a*b*：CV_BGR2Lab、CV_RGB2Lab、CV_Lab2BGR、CV_Lab2RGB
>
> RGB <--> CIE L*u*v：CV_BGR2Luv、CV_RGB2Luv、CV_Luv2BGR、CV_Luv2RGB

> RGB <-->  Bayer：CV_BayerBG2BGR、CV_BayerGB2BGR、CV_BayerRG2BGR、CV_BayerGR2BGR、CV_BayerBG2RGB、CV_BayerGB2RGB、  CV_BayerRG2RGB、CV_BayerGR2RGB（在CCD和CMOS上常用的Bayer模式）
>
> YUV420 <--> RGB：CV_YUV420sp2BGR、CV_YUV420sp2RGB、CV_YUV420i2BGR、CV_YUV420i2RGB

还有函数distanceTransform，是用来计算各像素距离最近的零像素距离的。

floodFill函数用来用指定颜色填充一个连通部件。

inpaint函数用来用附近区域信息重建选中区域，可以对图像里由于传输噪声丢失的块进行重建。

integral函数用来获得图像的积分值。

寻找边界使用分水岭分割办法的函数watershed。

对图像进行GrabCut算法的grabCut函数（有待研究，不熟悉）。

总之，这些变换千奇百怪，不是很系统，常用的还是我先介绍的几个，比如threshold、cvtColor。就这样吧，以后有收获再陆续补充。