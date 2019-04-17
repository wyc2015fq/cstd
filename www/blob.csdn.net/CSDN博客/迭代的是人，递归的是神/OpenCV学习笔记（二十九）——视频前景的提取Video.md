# OpenCV学习笔记（二十九）——视频前景的提取Video - 迭代的是人，递归的是神 - CSDN博客





2011年11月19日 16:54:31[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：36359
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









视频捕捉的对象中，背景通常保持不变。一般分析中关注移动的前景物体，威力提取出前景物体，需要建立背景的模型，将模型和当前帧进行比对检测前景物体。前景提取应用非常广泛，特别是在智能监控领域中。

如果有不含前景物体的背景图片，提取前景的工作相对容易，只需要比对当前帧和背景图片的不同，调用函数absdiff实现。但是大多数情况，获得背景图片是不可能的，比如在复杂的场景下，或者有光线条件的变化。因此，就需要动态的变换背景。一种简单的办法是对所观察到的图片取平均，但这样做也有很多弊端，首先，这种办法在计算背景图片的前需要输入大量的图片，其次我们进行取平均的过程中不能有前景物体进入。所以一种相对好的办法是动态建立背景图片并实时更新。

具体的实现过程主要分为两部分：一部分是调用absdiff函数找出当前图片和背景图片的区别，这之中使用了threshold函数去除为前景，当前图片像素与背景图片像素变化超过一定阈值的时候才认定其为前景；另一个工作是更新背景图片，调用函数accumulateWeighted，根据权重参数可以调整背景更新的速度，将当前图片更新到背景中，这里巧妙利用得到的前景提取结果作为mask，在更新背景图片的过程中避免了前景的干扰。

程序效果如图，代码下载地址为[http://download.csdn.net/detail/yang_xian521/3814878](http://download.csdn.net/detail/yang_xian521/3814878)

![](http://hi.csdn.net/attachment/201111/19/0_13216897411292.gif)


虽然可以调整阈值参数和权重更新速度调节前景提取的结果，但从测试视频可以发现，树叶的运动对结果的干扰还是不小的，特别对于第一帧出现前景的情况，由于后续更新背景都是对前景mask后对背景进行更新的，所以第一帧的前景部分对背景的影响因子很难被更新掉。这里提出一种改进的办法——混合高斯模型。可以使一个像素具有更多的信息，这样可以有效的减少类似树叶的不停飘动，水波的不停荡漾这种对前景的干扰。这个精密的算法比之前我所介绍的简单方法要复杂很多，不易实现。还好，OpenCV已经为我们做好了相关工作，将其封装在类BackgroundSubtractorMOG，使用起来非常方便。实现代码如下：



```cpp
Mat frame; 
	Mat foreground;	// 前景图片
	namedWindow("Extracted Foreground");
	// 混合高斯物体
	BackgroundSubtractorMOG mog;
	bool stop(false);
	while (!stop)
	{
		if (!capture.read(frame))
		{
			break;
		}
		// 更新背景图片并且输出前景
		mog(frame, foreground, 0.01);
		// 输出的前景图片并不是2值图片，要处理一下显示  
		threshold(foreground, foreground, 128, 255, THRESH_BINARY_INV);
```


新程序的效果图如下，下载地址为[http://download.csdn.net/detail/yang_xian521/3815366](http://download.csdn.net/detail/yang_xian521/3815366)


![](http://hi.csdn.net/attachment/201111/19/0_1321700460COUH.gif)




