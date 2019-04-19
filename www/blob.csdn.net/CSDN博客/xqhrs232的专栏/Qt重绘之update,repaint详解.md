# Qt重绘之update,repaint详解 - xqhrs232的专栏 - CSDN博客
2017年10月11日 11:25:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：618
原文地址::[http://blog.csdn.net/zb872676223/article/details/38084225](http://blog.csdn.net/zb872676223/article/details/38084225)
相关文章
1、[paintEvent()重绘事件](http://blog.csdn.net/moli1949/article/details/50113841)----[http://blog.csdn.net/moli1949/article/details/50113841](http://blog.csdn.net/moli1949/article/details/50113841)
2、[Qt 重绘事件的调用](http://www.cnblogs.com/weiweiqiao99/archive/2011/01/23/1942634.html)----[http://www.cnblogs.com/weiweiqiao99/archive/2011/01/23/1942634.html](http://www.cnblogs.com/weiweiqiao99/archive/2011/01/23/1942634.html)
3、Qt-PaintEvent频繁重绘Linux死机----[http://bbs.csdn.net/topics/392188165?page=1](http://bbs.csdn.net/topics/392188165?page=1)
Qt里面的重绘和Windows编程里面的重绘差不多。但是Qt的重绘更有特色，更加智能。
在讲之前，先说说paintEvent()
paintEvent()是一个虚函数槽(slot)，子类可以对父类的paintEvent进行重写。当调用update()，repaint()的时候，paintEvent()会被调用，另外，当界面有任何改变的时候，paintEvent()也会被调用，这种界面的改变包括界面从隐藏到显示，界面尺寸改变，当然还包括界面内容改变的时候会被调用。paintEvent()是已经被高度优化过的函数，它本身已经自动开启并实现了双缓冲(X11系统需要手动去开启双缓冲)，因此Qt中重绘不会引起任何闪烁。
X11系列系统手动 开启双缓冲的方法如下：
**[cpp]**[view
 plain](http://blog.csdn.net/feiyinzilgd/article/details/6116634#)[copy](http://blog.csdn.net/feiyinzilgd/article/details/6116634#)
- ...  
- externvoid qt_x11_set_global_double_buffer(bool);  
-  qt_x11_set_global_double_buffer(false);  
-  ...  
有了paintEvent的知识，现在再来看看update()和repaint()
update和repaint是一类的，需要重绘的对象主动去调用，然后重绘。update和repaint调用之后，都回去调用paintEvent().
repaint()，被调用之后，立即执行重绘，因此repaint是最快的，紧急情况下需要立刻重绘的可以使用repaint()。但是调用repaint的函数不能放到paintEvent中调用。举个例子：有一个继承于QWidget的子类MyWidget，并在子类中对paintEvent进行了重写。我们在MyWidget::myrepaint()中调用repaint()。但是，myrepaint()又被重写的paintEvent()调用。 这样调用repaint()的函数又被paintEvent()调用，由于repaint()是立即重绘，而且repaint()在调用paintEvent之前几乎不做任何优化操作，而是会造成死循环，即：先调用repaint()，继而调用paintEvent()，paintEvent()反过来有调用repaint()...如此死循环。
update()跟repaint()比较，update则更加有优越性。update()调用之后并不是立即重绘，而是将重绘事件放入主消息循环中，由main的event loop来统一调度的(其实也是比较快的)。update在调用paintEvent之前，还做了很多优化，如果update被调用了很多次，最后这些update会合并到一个大的重绘事件加入到消息队列，最后只有这个大的update被执行一次。同时也避免了repaint()中所提到的死循环。因此，一般情况下，我们调用update就够了，跟repaint()比起来，update是推荐使用的。

