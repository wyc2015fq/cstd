# Windows Mobile下实现图片的3D效果 - xqhrs232的专栏 - CSDN博客
2010年08月04日 09:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：797标签：[windows																[mobile](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址：：[http://blog.csdn.net/pknife/archive/2009/07/20/4363059.aspx](http://blog.csdn.net/pknife/archive/2009/07/20/4363059.aspx)
有些时候需要酷酷的效果，来装扮一下我们的程序。
以下是类似S1从右至左滑动的效果示意图。
看起来还有点那个意思吧？
其实原理并不复杂，用到了一个画图函数StretchBlt。
函数功能：函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩。
　　函数原型：BOOL StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeighDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)；
　　参数：
　　hdcDest：指向目标设备环境的句柄。
　　nXOriginDest：指定目标矩形左上角的X轴坐标，按逻辑单位表示坐标。
　　nYOriginDest：指定目标矩形左上角的Y轴坐标，按逻辑单位表示坐标。
　　nWidthDest：指定目标矩形的宽度，按逻辑单位表示宽度。
　　nHeightDest：指定目标矩形的高度，按逻辑单位表示高茺。
　　hdcSrc：指向源设备环境的句柄。
　　nXOriginSrc：指向源矩形区域左上角的X轴坐标，按逻辑单位表示坐标。
　　nYOriginSrc：指向源矩形区域左上角的Y轴坐标，按逻辑单位表示坐标。
　　nWidthSrc：指定源矩形的宽度，按逻辑单位表示宽度。
　　nHeightSrc：指定源矩形的高度，按逻辑单位表示高度。
　　dwRop：指定要进行的光栅操作。光栅操作码定义了系统如何在输出操作中组合颜色，这些操作包括刷子、源位图和目标位图等对象。参考BitBlt可了解常用的光栅操作码列表。
　　返回值：如果函数执行成功，那么返回值为非零，如果函数执行失败，那么返回值为零。
我们把一个矩形分成若干个小的矩形，针对每个小矩形，把里面的内容通过StretchBlt函数缩小或放大到特定的目标区域。通过肉眼看上去，就有了3D的效果。
只要想象力足够丰富，可以用该函数弄出各种花哨的效果。比如旋转门，比如放大镜，比如......很多很多。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/pknife/archive/2009/07/20/4363059.aspx](http://blog.csdn.net/pknife/archive/2009/07/20/4363059.aspx)
