# Android UI设计 --DroidDraw之AnDroidDraw - xqhrs232的专栏 - CSDN博客
2012年03月29日 21:07:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：718标签：[android																[ui																[cmd																[tcp																[layout																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/dreamList/article/details/5998908](http://blog.csdn.net/dreamList/article/details/5998908)
相关网帖
1.DroidDraw+Android+UI可视化设计器 ----[http://wenku.baidu.com/view/e94484aad1f34693daef3eaf.html](http://wenku.baidu.com/view/e94484aad1f34693daef3eaf.html)
DroidDraw老外写的一个android UI 编辑工具，用起来挺不错，可以节约不少时间。
如果想查看更多细节，可以访问 [http://www.droiddraw.org](http://www.droiddraw.org/)
最新版本可以到 [http://code.google.com/p/droiddraw/](http://code.google.com/p/droiddraw/) 这里下载。
下载后解压的目录如下图
![](http://hi.csdn.net/attachment/201011/9/0_12893277650656.gif)
我们运行 droiddraw.exe 如下图所示。用起来挺方便。
![](http://hi.csdn.net/attachment/201011/9/0_1289327851X1xm.gif)
[](http://code.google.com/p/droiddraw/issues/list)
下面我们安装下AnDroidDraw
AnDroidDraw是一个与DroidDraw集成的Android应用程序，
它允许你从DroidDraw应用程序下载你的GUIs,
也允许你在一个Android设备上预览你的GUIs.
官方的介绍可以到 [http://www.droiddraw.org/androiddraw.html](http://www.droiddraw.org/androiddraw.html) 这查看。
下面我们把 AnDroidDraw.apk  安装到模拟器上 
AnDroidDraw.apk 的下载地址  [http://www.droiddraw.org/AnDroidDraw.apk](http://www.droiddraw.org/AnDroidDraw.apk)
首先在cmd 下启动模拟器，看下图命令，emulator.exe @firstAndroid  其中 @firstAndroid  为模拟器的名字
![](http://hi.csdn.net/attachment/201011/9/0_1289328663KncY.gif)
模拟器启动成功的界面
![](http://hi.csdn.net/attachment/201011/9/0_1289328762Qq90.gif)
下面我们安装把 AndDroidDraw.apk 安装到模拟器上，另外打开一个cmd，执行命令 adb install AnDroidDraw.apk 
返回success 就说明安装成功了，看下面安装成功的截图。
![](http://hi.csdn.net/attachment/201011/9/0_1289329327KGkQ.gif)
另外我们还有设置下 端口转发规则：adb forward tcp:6100 tcp:7100
设置好后，我们去模拟器中执行 AnDroidDrow ,执行的界面如下图。
![](http://hi.csdn.net/attachment/201011/9/0_1289329503K1vt.gif)
现在我们到之前执行的DroidDraw 菜单中选择" Project"->"Send GUI to Deviec" 
执行成功看下面的图
![](http://hi.csdn.net/attachment/201011/9/0_1289330034mSRr.gif)
点击Preview Layout 按钮就可以看到DroidDraw 中设计的结果了
![](http://hi.csdn.net/attachment/201011/9/0_12893301705E8F.gif)
