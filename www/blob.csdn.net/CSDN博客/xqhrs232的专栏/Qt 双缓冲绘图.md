# Qt 双缓冲绘图 - xqhrs232的专栏 - CSDN博客
2017年10月11日 11:21:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：181
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/yuyin86/article/details/7302060](http://blog.csdn.net/yuyin86/article/details/7302060)
相关文章
1、qt 双缓冲贴图步骤详细 ----[http://bbs.csdn.net/topics/390647081](http://bbs.csdn.net/topics/390647081)
双缓冲技术是GUI 编程中常用的技术。所谓的双缓冲就是把需要绘制的控件保存到一个图像中，然后在把图像拷贝到需要绘制的控件上。在Qt 的早期版本中，为了用户界面更加清爽，经常用这个技术来消除闪烁。
在Qt4 中，QWidget 能够自动处理闪烁，因此我们不用再担心这个问题。尽管如此，如果控件绘制复杂且需要经常刷新，双缓冲技术还是很有用的。我们可以把控件永久保存在一个图像中，随时准备下一次绘制事件的到来，一旦接到一个控件的绘制事件，就把图片拷贝到控件上。如果我们要做的只是小范围的修改，这个技术更是尤为有用，如要绘制一条橡皮筋线，就不必刷新整个控件了。
      下面再介绍两个其他博客上面介绍的例子：
/////////////////////////////////////////////////////////////////////////////////////
闪烁首先,要想把闪烁减弱,请设置组件的背景模式为NoBackground. 
setBackgroundMode(NoBackground);
其次,重载组件的paintEvent()函数,如下改写: 
void MyWidget::paintEvent(QPaintEvent *e) 
{ 
QRect ur=e->rect();//得到组件尺寸 
QPixmap pix(ur.size());//以此为参数创建一个位图变量 
pix.fill(this,ur.topLeft());//填充位图 
QPainter p(&pic);//以位图为参数创建一个QPainter 对象
p.translate(-ur.x(),-ur.y());//在QPainter 上绘画 
//......//Drawing
p.End();//绘画完毕
bitBlt(this,ur.topLeft().&pix);//把位图贴到组件上
//注从qt4开始，bitBlt函数不在使用，取而代之的是drawPixmap。
}
///////////////////////////////////////////////////////////////////////////////
再分享一个国外论坛上的例子，该作者提供了两种方式，一种方式明显比另一种方式速度快 。Share 供大家分析 ^_^
'm migrating an application to Qt from MFC.
The MFC app would use GDI calls to construct the window (a graph plot, basically). It would draw to a memory bitmap back buffer, and then BitBlt that to the screen. Qt, however, already does
 double buffering.
When the user clicks and drags in the graph, I'd like that section of the window to be inverted.
I'd like to find the best way to do this. Is there a way to do something like grabWindow() that will grab from the widget's back buffer, not the screen? ... maybe a BitBlt(..., DST_INVERT) equivalent?
I saw setCompositionMode() in QPainter, but the docs say that only works on painters operating on QImage. (Otherwise I could composite a solid rectangle image onto my widget with a fancy composition
 mode to get something like the invert effect)
I could do the same thing as MFC, painting to a QImage back buffer... but I read that hardware acceleration may not work this way. It seems like it'd be a waste to reimplement the double buffering
 already provided to you in Qt. I'm also not so sure what the side effects of turning off the widget's double-buffering may be (to avoid triple-buffering).
At one point, I had a convoluted QPixmap::grabWidget() call with recursion-preventing flags protecting it, but that rendered everything twice and is obviously worse than just drawing to a QImage.
 (and it's specifically warned against in the docs)
Should I give up and draw everything to a QImage doing it basically like I did in MFC?
**EDIT:**
Okay, a QPixmap painter runs at approximately the same speed as direct now. So, using a QPixmap back-buffer seems to be the best way to do this.
The solution was not obvious to me, but possibly if I looked at more examples (like Ariya's Monster demo) I would have just coded it the way it was expected to be done and it would have worked
 just fine.
Here's the difference. I saw help system demos using this:
```
QPainter painter(this)
```
in the start of paintEvent(). So, it seemed to naturally follow to me that to double buffer to a QPixmap then paint on the screen, you needed to do this:
```
QPainter painter(&pixmap);
    QPainter painterWidget(this);
    ...  draw using 'painter' ...
    painterWidget.drawPixmap(QPoint(0,0), pixmap);
```
when in fact you are apparently supposed to do this:
```
QPainter painter;
    painter.begin(&pixmap);
    ... draw using 'painter' ...
    painter.end();
    painter.begin(this);
    painter.drawPixmap(QPoint(0,0), pixmap);
    painter.end();
```
I can see that my way had two active painters at the same time. I'm not entirely sure *why* it's faster, but intuitively I like the latter one better. It's a single QPainter object, and
 it's only doing one thing at a time. Maybe someone can explain why the first method is bad? (In terms of broken assumptions in the Qt rendering engine)
[http://hi.baidu.com/kdwyc/blog/item/4d4c5273892125098601b0c8.html](http://hi.baidu.com/kdwyc/blog/item/4d4c5273892125098601b0c8.html)
[http://www.cnblogs.com/zzxap/archive/2011/04/02/2175709.html](http://www.cnblogs.com/zzxap/archive/2011/04/02/2175709.html)
[http://blog.sina.com.cn/s/blog_640531380100ui8b.html](http://blog.sina.com.cn/s/blog_640531380100ui8b.html)

