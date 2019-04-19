# Qt动画机制详解 - fanyun的博客 - CSDN博客
2016年09月30日 09:03:18[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：929
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

Qt动画框架主要基于QAbstractAnimation以及它的两个子类QVariantAnimation和QAnimationGroup组成。QAbstractAnimation类是所有动画类的祖先。它包含了一些在框架中被普遍使用的基本功能；尤其是启动、停止和暂停动画功能。它也接收定时触发通知。
      Qt动画框架更是提供了QPropertyAnimation类，该类继承于QVariantAnimation类，用于对Qt属性的动画操作(Qt属性系统是Qt元对象系统的一部分)。QPropertyAnimation类使用缓和曲线算法对属性进行插值演化操作。因此当你想动画改变一个值时，你就声明该值为一个属性值并且使该类为成为一个QObject对象。这给我们提供了很大的方便性去动画操作现有的部件和其他的QObject对象。
      复杂动画可以通过构建QAbstractAnimation树形结构来构造。该树主要使用QAnimationGroup，QAnimationGroup类是一个包含其他动画类的容器类；同时QAnimationGroup类也是QAbstractAnimation类的子类，因此一个容器可以包含其他容器。
      Qt动画框架既是独立的一部分，也是Qt状态机框架的一部分。Qt状态机框架提供一个状态用来行使动画。当QState进入或者退出时可以改变属性，当这个动画状态提供了一个QPropertyAnimatio时，则动画状态即在这些值之间进行插值衍化操作。后续我们将了解的更加仔细。
Qt动画框架类：
![](https://img-blog.csdn.net/20170226210451292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
