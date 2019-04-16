# Java Swing 2D系统属性参数详解 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年10月07日 10:32:56[gloomyfish](https://me.csdn.net/jia20003)阅读数：5329










Java Swing 2D系统属性参数详解




**-Dsun.java2d.opengl=true** // 如果硬件加速已经被enable，可以通过这个选项来提高Swing GUI

速度，默认值为false




**-Dsun.java2d.trace=[log[,timestamp]],[count],[out:<filename>],[help],[verbose]**

//帮助发现哪个Swing组件在绘制时有性能问题。




**-splash:filename.png** // 启动时候出现splash screen. 如果不能找到对应图片则不会加载




**-Dswing.aatext=true** // 启动Java的抗锯齿功能，很多时候Swing绘制的图形对象

边缘总呈现锯齿状的，此时只要在JVM启动参数中加入这个选项就可以解决问题，你会看

到很平滑的边缘和文字。默认值是false




**-Dsun.java2d.d3d=false **// 关闭JAVA 3D功能（Direct3D，有时候Direct3D功

能可能导致JVM Crashed，默认是打开（true）




**-Dawt.nativeDoubleBuffering=true** // Swing将会enable操作系统的本地双缓存，而Swing

本身将不会做任何处理了。




** -Dsun.java2d.noddraw=true** //完全关闭JAVA 2D/3D的DirectDraw或者Direct3D的功能 




**-Dsun.java2d.d3dtexbpp=16** //说明系统和加速图片为16位的（4444）格式，这样会节

省VRAM和系统内存，但是导致图像质量下降。注意如果3D管道在被使用的情况下，此设置无效。



**-Dsun.java2d.ddoffscreen=false**//关闭Java2D图形系统用Direct3D或者DirectDraw来

绘制屏幕以外的表面，默认是true。默认值情况下有时候可能导致严重问题，可以使用该选项关闭。



**-Dsun.java2d.ddscale=true** //打开硬件加速的放缩，默认值是false即关闭的，主要原因

在于避免绘制缺陷在已存的应用程序中，这些绘制缺陷可能是由于不同的绘制方法导致和视频卡

(Video Card)导致



参考引用：

http://download.oracle.com/javase/1.5.0/docs/guide/2d/flags.html

http://java.sun.com/products/java-media/2D/perf_graphics.html





