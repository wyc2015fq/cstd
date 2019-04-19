# Android2.2、2.3定制修改状态栏布局、背景、去除状态栏按键、添加状态栏按键 - xqhrs232的专栏 - CSDN博客
2012年04月20日 14:26:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：713标签：[android																[frameworks																[java																[xml																[框架																[产品](https://so.csdn.net/so/search/s.do?q=产品&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://zhu-apollo-163-com.iteye.com/blog/1242045](http://zhu-apollo-163-com.iteye.com/blog/1242045)
Android2.2、2.3定制修改状态栏，包括修改布局、背景、去除状态栏按键、添加状态栏按键等，涉及到xml文件、java文件和底层库c文件以及图片等。
注意：状态栏不属于Launcher，状态栏关系到系统层即Android框架的第三层，而Launcher只是相当于一个应用程序，只涉及到最上层。
当修改好状态栏时需要全编译，但Launcher只需要小编译即可。
修改前先明白源代码架构，网上很多关于源代码目录结构的介绍，但都是很基础，我想可能是由于目录结构庞大导致无法详细说明每一个包，不过随着研究的深入，肯定会有详细介绍目录的文章，这里先引用网上的一篇介绍源代码目录结构的文章，主
要是先了解下：http://dev.10086.cn/cmdn/wiki/index.php?doc-view-5675.html
其实我很想把每个包详细的含义都作一个完整的说明，但需要一个漫长的学习过程。现在首先来学习状态栏的相关修改，这个涉及到frameworks包。这里先用2.2的布局文件来介绍，2.3的目录和这个基本上一样。
1. 状态栏的布局文件：/frameworks/base/packages/SystemUI/res/layout/status_bar.xml
在这个布局文件里修改状态栏的背景，可以把背景设置成图片或者颜色，可以把添加或者删除某个按键等；
2. 修改布局后，需要在java文件里添加这个布局，并且创建布局中每个视图的对象，涉及到的java文件是：
/frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/StatusBarView.java
在此文件中，需要声明视图对象，添加、删除监听事件、以及触发事件后的处理方法等。
3. 要修改状态栏的高度，需要在/frameworks/base/core/res/res/values/dimens.xml文件中修改。
4. 如果添加或者删除了某个按键，系统需要获取这个按键的按键值，涉及到按键值的四个文件是：
XML文件： /frameworks/base/api/current.xml
java文件： /frameworks/base/core/java/android/view/KeyEvent.java
xml文件：  /frameworks/base/core/res/res/values/attrs.xml
c文件：      /frameworks/base/include/ui/KeycodeLabels.h
注意技巧，里面很多的键值都是固定的，所以可以参看他们的方法增加新键的键值。
在上面几个文件中只要添加或者删除某个按键对应的按键值就可以了。
5. 状态栏涉及到资源图片文件主要在：/frameworks/base/core/res/res/
6. 状态栏可以下拉，那么这个下拉界面主要在：frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/StatusBarService.java
举例1：假如我们想把状态栏背景变成透明的，那需要在两个地方修改：
a) 打开：/frameworks/base/packages/SystemUI/res/layout/status_bar.xml
将第24行的背景改成：
android:background="#7f000000"
即表示背景透明了。
b) 上面XML布局里修改也只能在默认情况下显示透明，但这只是静态修改，还需要在java代码里修改使得动态情况下也变成透明的。
打开：frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/StatusBarService.java
找到：
protected void addStatusBarView()
将该方法中得代码：
PixelFormat.RGBX_8888 改成 PixelFormat.TRANSLUCENT
PixelFormat.TRANSLUCENT即表示透明。
举例2：我们也可以把下拉界面全屏显示
找到方法：void updateExpandedViewPos(int expandedPosition)
有一段代码：
        mTrackingPosition = mTrackingParams.y = pos; //原来是的坐标
        mTrackingParams.height = disph-h; //原来的高度
h表示状态栏的高度，默认情况下不是全屏的，是占据了大半个屏幕。如果要全屏，至少要吧下拉界面的宽度加上一个h，同时把下拉界面向上移动一个h，所以可以改成：
        mTrackingPosition =pos;
        mTrackingParams.y = pos-h; //原来的坐标是pos，现在要把下了界面向上移动，那就必须要减去h（注意坐标的方向，详细请参看下面的平面坐标图，请注意和数学里面的几何坐标不一样）
        mTrackingParams.height = disph; 
// 表明高度等于屏幕的高度
![](http://zhu-apollo-163-com.iteye.com/upload/picture/pic/103624/ca859943-521b-3fde-94d9-34541795a8ae.bmp)
7. 状态栏里面有一个时钟，它的主要代码在：
/frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/Clock.java
上面只是简单介绍了要修改状态栏所涉及到的主要文件所在的位置，具体修改可以根据客户需求或者产品需求来作。
