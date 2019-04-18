# irrlicht引擎的GUI系统(一) - gauss的专栏 - CSDN博客
2011年12月11日 17:31:52[gauss](https://me.csdn.net/mathlmx)阅读数：967标签：[引擎																[图形																[游戏](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
很久以前研究第一款开源引擎irrlicht，至于irrlicht介绍网上英文资料很多，这里就不一一说明了，先来看看irrlicht的GUI系统类图，这样感觉比较简洁知道控件之间的关系.开发东西多了容易忘记，所以最近把很久以前东西写下来算是做个备忘录吧！开发东西多了，记性变差了.刚开始的研究的时候都是在http://irrlicht.sourceforge.net/forum/上面研究学习的，国内这款引擎的中文资料很少，不像ORGE图形引擎那样到处都有中文资料，毕竟有几款商业游戏是使用ORGE，irrlicht国内成功的商业案例很少，基本上都是用于研究.
首先说一下IGUIElement这个接口，它不是抽象接口，但在GUI系统中所有的子控件接口都要继承它，它是所有控件的基类.它本身继承IEventReceiver和IAttributeExchangingObject抽象接口，IEventReceiver这个抽象接口主要作用是GUI事件监听接口,IAttributeExchangingObject主要是序列化与反序列化的抽象接口，IAttributeExchangingObject本身继承了IReferenceCounted.
可以看看下面的类图.
![](http://hi.csdn.net/attachment/201112/11/0_1323595640aN9u.gif)
CGUIEnvironment这个类很重要，一般GUI系统里面只有一份实例，在随着绘制设备创建的时候一起创建,CGUIEnvironment的实例会在每一桢绘制的时候调用draw绘制当前根节点的,当前根节点会调用它的子控件的draw函数，这一过程就是控件的绘制过程，子控件实现从IGUIElement继承的虚拟函数draw，并在里面绘制自己.
