# iOS绘图-线条旋转锯齿问题 - 月若无涯的专栏 - CSDN博客
2015年05月28日 19:39:19[月若无涯](https://me.csdn.net/u010124617)阅读数：1802
最近在使用我的绘图控件[YRBordView](https://github.com/YueRuo/YRBorderView)时，发现做旋转和抖动动画时有明显的锯齿出现，经过查资料，解决方案如下：
绘制时不要紧靠控件边缘，距离各边只要留出1像素（1px）即可解决。
找到方案后，顺便也把我的控件再次升级，支持通过设置contentEdgeInsets来调整绘制线条的边距。
有兴趣的可以看看我的加边线控件：[YRBordView](https://github.com/YueRuo/YRBorderView)
