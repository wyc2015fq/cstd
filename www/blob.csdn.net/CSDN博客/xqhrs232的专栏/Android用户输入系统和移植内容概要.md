# Android用户输入系统和移植内容概要 - xqhrs232的专栏 - CSDN博客
2011年09月07日 09:58:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：716
原文地址::[http://www.cnblogs.com/mengheyun/archive/2011/03/14/1983148.html](http://www.cnblogs.com/mengheyun/archive/2011/03/14/1983148.html)
Android中，用户输入系统的结构相对简单，主要的输入硬件设备是[键盘](http://product.it168.com/list/b/0201_1.shtml)、[触摸屏](http://product.it168.com/list/b/0106_1.shtml)、轨迹球等。
　　在Android的上层中，可以通过获得这些设备产生的事件，并对设备的事件做出响应。在Java框架和应用程序层，通常使用运动事件获得[触摸屏](http://product.pcpop.com/touch_screen/00000_1.html)、轨迹球等设备的信息，用按键事件获得各种[键盘](http://product.pcpop.com/Keyboard/00000_1.html)的信息。
　　Android用户输入系统的基本层次结构如图8-1所示。
![Android用户输入系统和移植内容概要](http://image20.it168.com/201103_500x375/510/ce5b20077d3dc344.jpg)
　　▲图8-1 Android用户输入系统的基本层次结构
**　　用户输入系统的结构**
　　Android用户输入系统的结构比较简单，自下而上包含了驱动程序、本地库处理部分、Java类对输入事件的处理、对Java程序的接口。Android用户输入系统的结构如图8-2所示。
![Android用户输入系统和移植内容概要](http://image20.it168.com/201103_500x375/510/f6412f3ffff0c9f3.jpg)
　　▲图8-2 用户输入系统的结构
　　如图8-2所示，自下而上，Android的用户输入系统分成几个部分：
　　驱动程序：在/dev/input目录中，通常是Event类型的驱动程序
　　EventHub：本地框架层的EventHub是libui中的一部分，它实现了对驱动程序的控制，并从中获得信息
　　KeyLayout(按键布局)和KeyCharacterMap(按键字符映射)文件。同时，libui中有相应的代码对其操作。定义按键布局和按键字符映射需要运行时配置文件的支持，它们的后缀名分别为kl和kcm
　　Java框架层的处理：在Java框架层具有KeyInputDevice等类用于处理由EventHub传送上来的信息，通常信息由数据结构RawInputEvent和KeyEvent来表示。通常情况下，对于按键事件，则直接使用KeyEvent来传送给应用程序层，对于触摸屏和轨迹球等事件，则由RawInputEvent经过转换后，形成MotionEvent时间传送给应用程序层
　　在Android的应用程序层中，通过重新实现onTouchEvent和onTrackballEvent等函数来接收运动事件(MotionEvent)，通过重新实现onKeyDown和onKeyUp等函数来接收按键事件(KeyEvent)。这些类包含在android.view包中
**　移植的内容**
　　移植Android的用户输入系统，主要的工作分成以下两个部分：
　　输入(input)驱动程序
　　用户空间中动态配置的kl和kcm文件
　　由于Android用户输入部分的“硬件抽象层”就是libui库中的EventHub，这部分是系统标准的部分。因此，在实现特定硬件平台的Android系统的时候，用户输入的硬件抽象层通常情况下不做改变。
　　EventHub使用Linux标准的input设备作为输入设备，其中又以实用Event设备居多。
　　在这种情况下，为了实现Android系统的输入，也必须使用Linux标准input驱动程序作为标准的输入。
　　由于标准化程度比较高，实现用户输入系统，在用户空间一般不需要更改代码。唯一的情况是使用不同的kl和kcm文件，使用按键的布局和按键字符映射关系。
