# Qt的5种常用布局搭建 - fanyun的博客 - CSDN博客
置顶2016年09月30日 09:06:16[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：894
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Qt布局详解：**
界面开发首先要对整个界面进行布局，使窗体上的所有的控件必须有一个合适的尺寸和位置。那么做出来的界面才看起来美观。
那么如何对界面进行布局呢？Qt提供了一些类负责排列窗体上的控件，主要有：QHBoxLayout，QVBoxLayout，QGridLayout，QFormLayout，QStackLayout。（布局管理类）这些类简单易用，无论在代码中还是用Qt Designer开发程序都能用到。
      常用的布局方法：
（1）使用水平布局类QHBoxLayout；
（2）使用垂直布局类QVBoxLayout；
（3）使用网格布局类QGridLayout；
（4）使用表格布局类QFormLayout;
（5）使用分组布局类QStackLayout(  QStackedLayout类把子控件进行分组或者分页，一次只显示一组或者一页，隐藏其他组或者页上的控件)。
这些方法可以嵌套使用。使用这些Qt布局管理类的另一个原因是，在程序、系统改变字体，语言或者在不同的平台上运行时，布局管理器能够自动调整窗体里所有控件的大小和尺寸。
布局管理的三种方式：
Qt中有三种方式对窗体上的控件进行布局管理：
**1.绝对位置定位（absolute positioning）：**控件布局是固定位置，没有自适应功能。
例子如下：
QWidget *pWidget = new QWidget;
QLabel label(pWidget);
label.setText(QObject::tr("姓名："));
label.setGeometry(10,10,20,20);
 QLineEdit namedLineEdit("小王",pWidget);
namedLineEdit.setGeometry(35,10,50,20);
 QPushButton *btn = new QPushButton(QObject::tr("关闭"),pWidget);
     btn->setGeometry(90,10,40,20);
![](https://img-blog.csdn.net/20160810155313765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.手工布局（manual layout）：**给出控件的绝对位置，但是他们的尺寸根据窗口的大小确定，可以通过重写窗体控件的resizeEvent()实现对子控件的大小设置。
**3.布局管理器（layout managers）：**运用QHBoxLayout、QVBoxLayout、QGridLayout 、QFormLayout、QStackLayout布局。
**(1).Horizontal Layout布局设置（水平布局）:**他包含的对象都横向排列开,示例如图3-1所示：
![](https://img-blog.csdn.net/20160810155416234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  图 3-1  水平布局
**(2).Vertical Layout布局设置（垂直布局）:**他包含的对象都纵向排列开，示例如图3-2所示：
![](https://img-blog.csdn.net/20160810155504445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                 图3-2  竖直布局
**(3).Grid Layout布局设置（网格布局）：**将控件放置到网格中布局，它本身会从父窗口或父布局中占据尽可能多的界面空间，然后把自己的空间划分为行和列，再把每个控件塞到设置好的一个或多个单元格中。通常情况下 QGridLayout不需要自己添加空白条 QSpacerItem，因为其他功能控件把各自的单元格占据之后，剩下没控件占据的单元格自然就是空的，空的格子默认里面什么都没有，也没有空白条。示例如图3-3所示：
![](https://img-blog.csdn.net/20160810155601868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                  图3-3 网格布局
**(4).Form Layout布局设置（表格布局）**：专门用于管理输入控件和与之相关的标签等表单布局，QFormLayout固定为两列布局，并针对表单做了建模，配套了一堆方便使用的函数。网格布局器的基本单元是单元格，而表单布局器的基本单元是行。表单布局器是高度建模并封装的，它没有 addWidget()和 addLayout()之类的函数，它只有addRow()函数。表单布局器中一行的空间可以由多个控件占据，也可以由一个控件占据。示例如图3-4所示：
![](https://img-blog.csdn.net/20160810155725824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                    图3-4 表格布局
**(5).布局管理器在指定的位置留出一块空间：**（Qt Designer中，可以加入一个spacer实现这一功能）示例如图3-5所示:
![](https://img-blog.csdn.net/20160810155806043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                 图3-5  布局留出指定空间
**(6).QSpliter分割器的作用：**分裂器 QSplitter 是一个实体功能控件，它的基类是 QFrame，QFrame 基类正是 QWidget。QSplitter 可以独立存在，可以作为父窗口容纳多个子控件，分裂器会完全拥有内部的子控件。在 Qt 设计师或 QtCreator 设计模式左边 Widget Box 里面没有分裂器可以拖动，使用分裂器的方式是：选中已有的控件，然后点击上面工具栏的水平分裂器或垂直分裂器按钮。比如上图是将三个丰富文本编辑器作为一个水平分裂器排布
 的。分裂器内每个控件都有一个手柄 Handle，水平分裂器内控件的手柄在左边，垂直分裂器内控件的手柄在控件上方。第 0 个控件的手柄是永久隐藏的，分裂器自身占据的大矩形四个边界线通常不能拖动拉大，只能拖动控件之间的手柄，比如上图的手柄 1 和 手柄 2 。分裂器整体的尺寸不是用户控制的，而在分裂器内部的控件尺寸可以让用户手工拖动手柄来控制。在程序运行时，水平分裂器内各个控件的宽度、垂直分裂器内部各个控件的高度， 一般都是用户拖动手柄控制，这是分裂器和布局器最大的不同。
具体操作如下图3-6所示：
![](https://img-blog.csdn.net/20160810155848810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                               图  3-6
实际效果如下图3-7所示：
![](https://img-blog.csdn.net/20160810155933936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                     图 3-7分割实际效果图
