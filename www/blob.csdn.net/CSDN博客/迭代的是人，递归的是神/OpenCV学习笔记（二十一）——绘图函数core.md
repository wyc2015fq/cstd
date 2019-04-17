# OpenCV学习笔记（二十一）——绘图函数core - 迭代的是人，递归的是神 - CSDN博客





2011年11月28日 15:49:11[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：7964
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









在图像中，我们经常想要在图像中做一些标识记号，这就需要绘图函数。OpenCV虽然没有太优秀的GUI，但在绘图方面还是做得很完整的。这里就介绍一下相关的函数。

在绘图函数中都会用到颜色，这里用Scalar结构表示颜色参数。

作图相关的函数有circle、clipLine、ellipse、ellipse2Poly、line、rectangle、polylines、类LineIterator。

填充相关的函数有fillConvexPoly、fillPoly。

显示文字相关的函数有getTextSize、cvInitFont、putText。

以上这些函数使用都非常简单方便，我就不过多介绍了。



