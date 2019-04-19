# Xcode的NSLog/Concole控制台/调试窗口隐藏/显示的设置？ - xqhrs232的专栏 - CSDN博客
2018年12月07日 15:54:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：104
原文地址::[https://www.jianshu.com/p/455bb0d060a8](https://www.jianshu.com/p/455bb0d060a8)
![](https://upload-images.jianshu.io/upload_images/2707674-b6fd76c34756a4cf.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
『导言』
Xcode 编译及运行时 不自动隐藏`dubug/Console窗口`的方法
Xcode在编译运行程序时会自动隐藏到下方的Console窗口，看不到程序的输出，很不方便，那怎么办？
- 
**方法一：点击图标 **（仅隐藏）
![](https://upload-images.jianshu.io/upload_images/2707674-8a168627ab6af040.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图.log/concole控制台.png
- 方法二： **快捷键 **（显示/隐藏切换）
	每次都要按`Cmd+Shift+Y`重新打开。
	
![](https://upload-images.jianshu.io/upload_images/2707674-f7ad48ba73fdf3d6.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/665/format/webp)
控制台隐藏与显示.gif
- 方法三： **设置 **(始终显示)
	- 点击`xcode`界面
- 选择`Xcode`->`Preference`->`Behaviors`,看到`Run Starts`,`Run Pauses`等等，把右边的debugger with那行全都设成 `Show debugger with Console View`,并勾上.其他的Build Testing之类的根据自己需要一样的设置。
		
![](https://upload-images.jianshu.io/upload_images/2707674-40371f6c2c2ff217.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图.设置图.png
- 
动态教程
![](https://upload-images.jianshu.io/upload_images/2707674-d361923b9cc1e7fe.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
控制台输出设置.gif
作者：iOS开发学习与分享zwj
链接：https://www.jianshu.com/p/455bb0d060a8
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
