# Qt Quick核心编程从入门到精通----系列教程 - xqhrs232的专栏 - CSDN博客
2017年07月26日 17:08:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：805
原文地址::[http://blog.csdn.net/gubenpeiyuan/article/details/42977579](http://blog.csdn.net/gubenpeiyuan/article/details/42977579)

本文是个推荐文章，推荐foruok博主的Qt quick 核心编程的系列经典编程！
foruok 博主 的Qt Quick系列文章：
- [Qt Quick 简介](http://blog.csdn.net/foruok/article/details/28620525)
- [QML 语言基础](http://blog.csdn.net/foruok/article/details/28634543)
- [Qt Quick 之 Hello World 图文详解](http://blog.csdn.net/foruok/article/details/28850879)
- [Qt Quick 简单教程](http://blog.csdn.net/foruok/article/details/28859415)
- [Qt Quick 事件处理之信号与槽](http://blog.csdn.net/foruok/article/details/30028711)
- [Qt Quick事件处理之鼠标、键盘、定时器](http://blog.csdn.net/foruok/article/details/30474299)
- [Qt Quick 事件处理之捏拉缩放与旋转](http://blog.csdn.net/foruok/article/details/32078761)
- [Qt Quick 组件与对象动态创建详解](http://blog.csdn.net/foruok/article/details/32730911)
- [Qt Quick 布局介绍](http://blog.csdn.net/foruok/article/details/33738227)
- [Qt Quick 之 QML 与 C++ 混合编程详解](http://blog.csdn.net/foruok/article/details/32698603)
- [Qt Quick 图像处理实例之美图秀秀（附源码下载）](http://blog.csdn.net/foruok/article/details/37740583)
- [Qt Quick 之 PathView 详解](http://blog.csdn.net/foruok/article/details/38060495)
- [Qt Quick实例之挖头像](http://blog.csdn.net/foruok/article/details/38778233)
- [Qt Quick综合实例之文件查看器](http://blog.csdn.net/foruok/article/details/39005703)
- [Qt Quick调试之显示代码行号](http://blog.csdn.net/foruok/article/details/40656285)
- [Qt Quick实现的涂鸦程序](http://blog.csdn.net/foruok/article/details/41152297)
- [Qt Quick播放GIF动画](http://blog.csdn.net/foruok/article/details/41677211)
- [Qt Quick 中的 drag and drop（拖放）](http://blog.csdn.net/foruok/article/details/41747085)
- [Qt Quick里的AnimatedSprite的用法](http://blog.csdn.net/foruok/article/details/42233365)
- [Qt Quick里的粒子系统](http://blog.csdn.net/foruok/article/details/42289909)
- [Qt Quick实现的疯狂算数游戏](http://blog.csdn.net/foruok/article/details/42554321)
- [Qt Quick里的图形效果——混合（Blend）](http://blog.csdn.net/foruok/article/details/42637483)
Qt 的 相应 包下载位置如下：
http://mirrors.hust.edu.cn/qtproject/development_releases/qt/5.4/5.4.0-beta/
windows推荐使用minGW ，他不需要配置，一键安装后可以使用，也可以使用[Android](http://lib.csdn.net/base/android) ，也是一键使用，无需配置。作者因为需要做流媒体及视觉方面应用，工程使用的 vs+cmake
 + qt 开发环境方案！
工程构建管理 pri 的使用：
包括：
**[plain]**[view
 plain](http://blog.csdn.net/gubenpeiyuan/article/details/42977579#)[copy](http://blog.csdn.net/gubenpeiyuan/article/details/42977579#)
- include(src/src.pri)  
pri 的写法 （一木了然吧，很简单）
**[plain]**[view
 plain](http://blog.csdn.net/gubenpeiyuan/article/details/42977579#)[copy](http://blog.csdn.net/gubenpeiyuan/article/details/42977579#)
- SOURCES += \  
-     $$PWD/event.cpp \  
-     $$PWD/main.cpp \  
-     $$PWD/sqleventmodel.cpp  
- 
- 
- HEADERS += \  
-     $$PWD/event.h \  
-     $$PWD/sqleventmodel.h  

最后用Qt5.4 做了一个这样的界面，还凑合吧，使用了网上360的那个哥们的 图片素材，谢谢啦！
         源码是示例，也是开源的。实现了自定义标题栏，状态栏，窗口任意缩放，布局及信号槽等。
![](https://img-blog.csdn.net/20150209090203811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3ViZW5wZWl5dWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
QML标题：[qml界面之自定义TitleBar](http://blog.csdn.net/z3plus/article/details/20741029)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/z3plus/article/details/20741029?utm_source=tuicool
[qt4 qml Flipable、Flickable和状态与动画](http://blog.csdn.net/luck_good/article/details/6992795)
[http://blog.csdn.net/luck_good/article/details/6992795](http://blog.csdn.net/luck_good/article/details/6992795)
