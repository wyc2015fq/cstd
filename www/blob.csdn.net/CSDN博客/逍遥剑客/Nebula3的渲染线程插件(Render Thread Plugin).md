# Nebula3的渲染线程插件(Render Thread Plugin) - 逍遥剑客 - CSDN博客
2009年06月22日 23:14:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2385标签：[thread																[wizard																[application																[vb																[图形																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=vb&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=wizard&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
上个版本的SDK把渲染放入一个独立的线程, 这是一个很先进的改动
但是它也造成一个问题: 不好操作底层的渲染API了
对于想自己控制VB, IB来绘制三角形级的demo程序来说, 一点也不方便
之前我写过一个[SyncRenderApplication](http://blog.csdn.net/xoyojank/archive/2009/02/26/3937796.aspx), 但是很不完美, 比如不能使用InputServer
April2009加入了RTPlugin, 为写demo指明了方向, 嘿嘿(虽然作者原意肯定不是这样的)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090622/N3_RTPlugin.JPG)
从上图可以看出, RTPlugin完全存在于Render Thread, 这样可以在它的派生类中直接进行底层图形的操作, 主线程只管启动程序和注册这个plugin
这里又用到一个比较好玩的特性: [根据Rtti创建对象](http://blog.csdn.net/xoyojank/archive/2009/06/17/4274947.aspx). 因为主线程创建的对象是不能直接传给渲染线程的(要不然就违背了Fat Thread的概念), 所以创建只能在渲染线程进行. 而什么时候创建, 只能由主线程来控制, 所以消息RegisterRTPlugin只包含了这个类的Rtti, 在渲染线程由这个Rtti来创建指定对象的实例, 嗯嗯, 这招很很很(什么形容词好呢?)....
代码待续........(打算集成进Nebula3 Render Application Wizard)
