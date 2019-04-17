# UGUI内核大探究（三）输入模块 - 凯奥斯的注释 - CSDN博客





2016年09月13日 22:45:22[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4900
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# [UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)我们探究了事件系统，[UGUI内核大探究（二）执行事件](http://blog.csdn.net/ecidevilin/article/details/52510373)中我们介绍了事件是如何执行的。那么事件是如何产生的呢？这就涉及到BaseInputModule、PointerInputModule、StandaloneInputModule、TouchInputModule这些类。今天我们就探究输入模块的原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。

BaseInputModule是一个抽象类，是所有输入模块类的基类。PointerInputModule也是一个抽象类，继承自BaseInputModule，是StandaloneInputModule和TouchInputModule的基类。而StandaloneInputModule是在PC、Mac&Linux上的具体实现，而TouchInputModule是在IOS、Android等移动平台上的具体实现。之所以这种继承结构，我认为是向开发者提供了扩展，方便开发者实现新型设备的输入模块。



[UGUI内核大探究（二）执行事件](http://blog.csdn.net/ecidevilin/article/details/52510373)里我们讲到，EventInterface声明了很多接口，每个接口都对应着一个事件。这些事件是由输入模块产生的，而归根结底大部分是通过Input这个类的各种属性和静态方法获取了数据才生成了事件。




当鼠标或触摸进入、退出当前对象时执行pointerEnterHandler、pointerExitHandler。

在鼠标或者触摸按下、松开时执行pointerDownHandler、pointerUpHandler。

在鼠标或触摸松开并且与按下时是同一个响应物体时执行pointerClickHandler。

在鼠标或触摸位置发生偏移（偏移值大于一个很小的常量）时执行beginDragHandler。

在鼠标或者触摸按下且当前对象可以响应拖拽事件时执行initializePotentialDrag。

对象正在被拖拽且鼠标或触摸移动时执行dragHandler。

对象正在被拖拽且鼠标或触摸松开时执行endDragHandler。

鼠标或触摸松开且对象未响应pointerClickHandler情况下，如果对象正在被拖拽，执行dropHandler。

当鼠标滚动差值大于零执行scrollHandler。

当输入模块切换到StandaloneInputModule时执行updateSelectedHandler。（不需要Input类）

当鼠标移动导致被选中的对象改变时，执行selectHandler和deselectHandler。

导航事件可用情况下，按下上下左右键，执行moveHandler，按下确认键执行submitHandler，按下取消键执行cancelHandler。

关于如何获取当前对象，参考[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)。

本文没有贴出任何源码，一个原因是输入模块本身并不直接参与到UI交互当中去（通过EventSystem），我认为Unity官方也不希望开发者直接使用它们。另一个原因是四个类中并没有大段的结构型代码，都是一些细节的具体实现，理解起来很容易，分析起来很乏味~~（实现起来很痛苦）~~。



