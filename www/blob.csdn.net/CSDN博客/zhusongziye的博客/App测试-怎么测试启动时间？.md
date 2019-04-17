# App测试-怎么测试启动时间？ - zhusongziye的博客 - CSDN博客





2017年09月21日 21:55:21[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：563








#### 背景介绍


Android用户也许会经常碰到以下的问题： 1）应用后台开着，手机很快没电了——应用耗电大； 2）首次/非首次启动应用，进入应用特别慢——应用启动慢； 3）应用使用过程中，越来越卡——CPU能力不足/内存泄露； 4）应用页面卡顿——帧率较低、页面卡顿。 因此，对开发的Android应用，必须对其进行性能测试，不然将会直接影响用户体验。


Android应用性能测试通常包括：启动时间、内存、CPU、耗电量、流量、流畅度等。本次先介绍启动时间的测试方法。


启动时间对于App的性能测试，启动时间是个重要指标，启动时间分为两种情况，一种是冷启动时间（通常是系统重启，即在启动前没有该App进程的情况），另一种是热启动，即App从被切换到前台（点back退出后再点击图标启动）。QA测试时，一般关注冷启动的启动时间。以下介绍三种测试启动时间的方法，供大家参考，可以有针对性的使用。

#### 1.1 使用adb命令

1.1.1 测试方法 输入
```
adbshell
 am start -W packagename/MainActivity
```
命令，计算启动时间。如下图： 

![](https://img-blog.csdn.net/20170921215841709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图1应用第一次启动也就是我们常说的冷启动,这时候你的应用程序的进程是没有创建的. 这也是大部分应用的使用场景.用户在桌面上点击你应用的 icon 之后,首先要创建进程,然后才启动 MainActivity.这时候adbshell am start -w packagename/MainActivity 返回的结果,就是标准的应用程序的启动时间。注意Android 5.0 之前的手机是没有WaitTime这个值的。关于ThisTime/TotalTime/WaitTime的区别,下面是其解释。`WaitTime=endTime-startTime`
- 

startTime记录的刚准备调用startActivityAndWait()的时间点

- 

endTime记录的是startActivityAndWait()函数调用返回的时间点

- 

WaitTime = startActivityAndWait()调用耗时。



WaitTime 就是总的耗时，包括前一个应用Activity pause 的时间和新应用启动的时间；ThisTime 表示一连串启动Activity 的最后一个 Activity 的启动耗时；TotalTime表示新应用启动的耗时，包括新进程的启动和 Activity 的启动，但不包括前一个应用Activity pause 的耗时。也就是说，开发者一般只要关心 TotalTime 即可，这个时间才是自己应用真正启动的耗时。


总结一下，如果只关心某个应用自身启动耗时，参考TotalTime；如果关心系统启动应用耗时，参考WaitTime；如果关心应用有界面Activity启动耗时，参考ThisTime。

1.1.2 总结


该方法算出的时间是系统从开始处理启动activity的时间到完成运行layout和draw函数的时间，简单的理解就是启动这个Activity的时间，并不包括点击icon到系统接收到消息的时间。显然，这个时间并不能完整的模拟用户操作场景的启动时间。其次该方法只计算一个Activity的整体启动时间，没有分别统计其中每个函数的时间，不便于定位问题。针对这两个问题，我们接下来看一下下面两个方法是怎样解决的。


我们在测试中关注的其实是用户体验的启动时间，那么上面的时间就不能满足我们的需求了。既然是用户体验我们可以用更直观的方式，使用screenrecord进行屏幕录制然后分析视频。

#### 1.2 使用screenrecord进行屏幕录制

1.2.1 测试方法 （1）输入命令adb shellscreenrecord --bugreport /sdcard/lanch.mp4--bugreport 参数会使视频输出一些时间信息和帧信息便于我们分析启动时间。 （2）点击收集图标，app完全启动后，使用ctrl+c结束视频录制。
 （3）使用命令adb pullsdcard/lanch.mp4 ./，导出视频 （4）导出视频到电脑，使用可以按帧播放的视频软件打开（mac上quicktime就可以，win下可以用kmplayer），并按帧播放。 如下图所示：

![](https://img-blog.csdn.net/20170921220502744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图2按帧播放视频，视频左上角会显示每一帧的时间（精确到ms）和帧数。如图，11:09:38.031表示时分秒，f=333是帧数。在视频中会看到icon会变暗然后高亮，高亮时就是系统开始处理本次icon点击事件了。可以把这里作为点击时间，然后根据体验要求，看到app启动页完全绘制完作为终止时间，这个时间减去点击时间就是app的启动时间。

1.2.2 总结 该方法虽然可以模拟用户的操作场景，但是操作成本较高且无法准确清晰明了的知道哪些函数调用时间过长。 以上两种方法，单从启动时间看，是无法定位出具体哪个函数耗时多一些，遇到启动时间大于预定的启动时间阀值时，需一步步的打log，分析查明原因。下面的方法是贴吧目前计算启动时间的办法，可以很清晰的看到每个函数的调用时间。

#### 1.3 代码埋点，查看输出日志

1.3.1 测试方法 在代码中打点，输出日志查看。拿贴吧举例，下图是整个启动要经历的操作。 

![](https://img-blog.csdn.net/20170921220822332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图3具体每步做了哪些，可以参照下表。

![](https://img-blog.csdn.net/20170921220916246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图4这样通过打点输出日志来测试启动时间，QA就可以很方便的查看到具体每个模块的耗时时间了，如下图。

![](https://img-blog.csdn.net/20170921220958212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1.3.2 总结


这样打点，可以清晰明了的看出Activity的总耗时及各个函数的耗时情况，这样在测试过程中，如果遇到问题可以很容易的定位到具体的函数。在测试过程中也有针对点，比如贴吧直播后续会以插件的形式整合到贴吧里，测试时，可以多关注plugin初始化的时间。


针对启动时间这一性能指标，个人觉得打点输出日志的方式较为理想，QA在测试过程中发现有疑似问题后，可以给出具体的函数耗时时间。



