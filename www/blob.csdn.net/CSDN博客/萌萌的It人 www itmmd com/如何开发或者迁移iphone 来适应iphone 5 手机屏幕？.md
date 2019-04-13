
# 如何开发或者迁移iphone 来适应iphone 5 手机屏幕？ - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月19日 18:21:36[Jlins](https://me.csdn.net/dyllove98)阅读数：950标签：[andorid																](https://so.csdn.net/so/search/s.do?q=andorid&t=blog)个人分类：[android																](https://blog.csdn.net/dyllove98/article/category/1130032)


新的iPhone6采用了一个新的长宽比和新的分辨率（640×1136像素）。
如何让现有的app适应这个屏幕尺寸
如何才能然搞一个app适应不同的屏幕分辨率呢？不用每次都调整？
iPhone IOS屏幕分辨率与iPhone6

### 处理方法

1、下载并安装Xcode中的最新版本。
2、为您的应用程序中设置了一块4.7 和5,5英寸启动图像。主要是用来适应现有的屏幕高度（以前的图片不兼容）。
3、设置自动调整大小，如果你的图片正确，应该不会有太大的问题。
4、如果还没有达到效果，你需要调整视图布局自动适应
5、如果你有其他特殊的要求，可以按照如下的方式处理
例如：
Example:
CGRect screenBounds = [[UIScreen mainScreen] bounds];
if (screenBounds.size.height == 568) {
// code for 4-inch screen
} else {
// code for 3.5-inch screen
}
Also note: The auto-rotation API has changed completely, take a look at that as well if your application supports any rotation other than default.
另外还要注意：自动旋转API已经完全改变了

原文地址：[http://www.itmmd.com/201410/84.html ](http://www.itmmd.com/201410/84.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

