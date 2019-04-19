# 【Android工具】被忽略的UI检视利器：Hierarchy Viewer - xqhrs232的专栏 - CSDN博客
2012年03月23日 14:50:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：629标签：[hierarchy																[ui																[android																[工具																[layout																[tools](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=hierarchy&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/ddna/article/details/5527072](http://blog.csdn.net/ddna/article/details/5527072)
相关网帖::
1.[使用Hierarchy Viewer](http://marshal.easymorse.com/archives/3905)----[http://marshal.easymorse.com/archives/3905](http://marshal.easymorse.com/archives/3905)
2.Android 实用工具Hierarchy Viewer实战----[http://tech.it168.com/a2011/0802/1226/000001226690.shtml](http://tech.it168.com/a2011/0802/1226/000001226690.shtml)
 Hierarchy Viewer是随AndroidSDK发布的工具，位置在tools文件夹下，名为hierarchyviewer.bat。它是Android自带的非常有用而且使用简单的工具，可以帮助我们更好地检视和设计用户界面(UI)，绝对是UI检视的利器，但是好像很少有人提它，难道是因为太简单？
具体来说主要功能有2个：
1.从可视化的角度直观地获得UI布局设计结构和各种属性的信息，帮助我们优化布局设计；
2.结合debug帮助观察特定的UI对象进行invalidate和requestLayout操作的过程。
1.基本使用方法
（1）hierarchyviewer的使用非常简单，启动模拟器或者连接上真机后，启动hierarchyviewer.bat，会看到下面的界面，Devices里列出了可以观察的设备，Windows里列出的是当前选中的设备的可以用来显示View结构的Window：
![](http://hi.csdn.net/attachment/201004/25/521376_1272187402e5N5.png)
    选中某个想要观察的Window，比如上面列出的com.android.launcher/com.android.launcher.Launcher项，然后点击菜单栏的Load
 View Hierarchy，就进入Layout View，由于要解析相关Window，所以这个过程要几秒钟，左边列出的是当前窗口的树型布局结构图，右边列出的是当前选中的某个子View的属性信息和在窗口中的位置：
![](http://hi.csdn.net/attachment/201004/25/521376_12721874021SJp.png)
    需要注意的是：Layout View列出的View结构是从视图的根节点开始的，比如针对Launcher使用的layout，它的底层基础布局DragLayer实际上是放在一个FrameLayout里的，该FrameLayout又是被PhoneWindow的DecorView管理的。
（2）点击界面左下角类似九宫格的按钮，就进入了Android称之为Pixel
 Perfect View的界面，这个界面里主要是从细节上观察UI效果：
![](http://hi.csdn.net/attachment/201004/25/521376_12721873981ia8.png)
    左边是浏览视图，中间是全局的视图，右边是当前关注的地方的细节放大，是像素级别的，对于观察细节非常有用。
Refresh Rate用来控制View多久从模拟器或者真机上更新一次视图数据。
Zoom就是放大局部细节用的，细节显示在最右边的视图上。
Overlay比较有意思，主要用来测试在当前视图上加载新的图片后的效果，点击Load…选择图片后，可以控制在当前界面上显示的透明读，滑动0%~100%的控件即可。如果选择了Show
 in Loupe，右侧的放大视图也会将加载的图片的细节结合着透明度显示出来。不过目前这个Overlay做的比较简单，合成的图只能从界面的左下角为原点画出来，不能移动。
（3）在Layout
 View中，选中一个view的图示，点击工具栏的Display View，就可以看到这个view的实际显示效果，可以点选Show
 Extras，这个功能也比较实用，可以显示出该View中不同元素显示的边界，帮助我们检查是否正确。
![](http://hi.csdn.net/attachment/201004/25/521376_1272187400Byb8.png)
2.Hierarchyviewer的invalidate和requestLayout功能
对于Android的UI来说，invalidate和requestLayout是最重要的过程，Hierarchyviewer提供了帮助我们Debug特定的UI执行invalidate和requestLayout过程的途径，方法很简单，只要选择希望执行这两种操作的View点击按钮就可以。当然，我们需要在例如onMeasure()这样的方法中打上断点。这个功能对于UI组件是自定义的非常有用，可以帮助单独观察相关界面显示逻辑是否正确。

