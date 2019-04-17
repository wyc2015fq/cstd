# [zt] ROI (Region of Interest) 感兴趣区域 OpenCV - Grandyang - 博客园







# [[zt] ROI (Region of Interest) 感兴趣区域 OpenCV](https://www.cnblogs.com/grandyang/p/4014895.html)





在以前介绍IplImage结构的时候，有一个重要的参数——ROI。ROI全称是”Region Of Interest”,即感兴趣的区域。实际上，它是IPL/IPP（这两个是Inter的库）结构IplROI的实例。IplROI包含xOffset、yOffset、height、width和coi成员变量。其中COI代表channel of interest（感兴趣的通道）。ROI的思想是：一旦设定ROI，通常组用于整幅图像的函数便只会对ROI所表示的子图像进行操作。如果COI被设置非0值，则对该图像的操作就只作用于被指定的通道上了。这个COI变量可取的值是1、2、3、4通道，并且为了使COI无效而保留了0取值（有点像忽略）。但是，许多OpenCV函数都忽略参数COI。

ROI在实际工作中有很重要的作用，在许多情况下，使用它会提高计算机视觉代码的执行速度。这是因为他们允许对图像的某一部分进行操作，而不是对整个图像进行操作。在OpenCV中，普遍支持ROI，函数的操作被限于感兴趣的区域。要设置或取消ROI，就要使用 cvSetImageROI( )和cvResetImageROI( )函数。如果设置ROI，可以使用函数 cvSetImageROI( )，并为其传递一个图像指针和矩形。要取消ROI，只需要为函数 cvResetImageROI( )传递一个图像指针。







|12|voidcvSetImageROI(IplImage*image,CvRect rect);voidcvResetImageROI(IplImage*image);|
|----|----|




下面用一个例子来来说明ROI的用法。例子中：我们加载一幅图像并修改一些区域——设置ROI的x，y，width和height的值，最后将ROI区域中像素都加上一个整数。代码如下：







|1234567891011121314151617181920212223242526272829|#include <opencv2/highgui/highgui.hpp>#include <opencv2/imgproc/imgproc.hpp>#include <stdio.h>using namespacecv;using namespacestd;intmain(intargc,char**argv){IplImage *src;src=cvLoadImage("E:\OpenCV\images\cat.jpg");intx=90;inty=110;intwidth=180;intheight=180;intadd=150;cvSetImageROI(src,cvRect(x,y,width,height));cvAddS(src,cvScalar(add),src);cvResetImageROI(src);cvNamedWindow("Roi_Add",1);cvShowImage("Roi_Add",src);cvWaitKey();return0;}|
|----|----|




上述代码把ROI集中于猫的脸部，并将其蓝色通道增加150。最终效果如图：

![OpenCV学习笔记（5）——ROI - 第1张  | 时间轨迹 ]( http://time-track.qiniudn.com/wp-content/uploads/2014/04/5-1.png)

**最后有两点需要说明：**

1. cvAddS( )函数，该函数用于实现一个数组和一个标量的元素级的相加运算。例子中通过cvScalar( )构造函数将CvScalar的第一个分量设置为add代表的值，其他值为默认值0，然后和src里面的元素相加。那为什么是蓝色通道增加150，而不是红色通道呢？因为RGB图在内存中的排列顺序是：BGRA BGRA BGRA …。所以,实际上第一个通道是B(blue)。该函数其实还有第四个参数（默认为NULL），其代表的含义可参考OpenCV文档。

2. 显示图像之前的” cvResetImageROI(src); ”这一句代码千万不能少。如果没有这行代码，那接下去显示图像的时候，只会显示ROI区域。因为前面已经说过，设置了ROI区域后，OpenCV的函数只会对ROI区域有效。注释掉那一句后的程序运行结果如下所示：

![OpenCV学习笔记（5）——ROI - 第2张  | 时间轨迹 ]( http://time-track.qiniudn.com/wp-content/uploads/2014/04/5-2.png)












