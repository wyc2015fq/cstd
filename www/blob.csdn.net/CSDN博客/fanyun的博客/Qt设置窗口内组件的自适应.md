# Qt设置窗口内组件的自适应 - fanyun的博客 - CSDN博客
2017年11月30日 20:55:12[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1153
**[Qt设置窗口内组件位置随窗口变化](http://www.cnblogs.com/sfy5848/p/4178605.html)**
当我们用Qt Designer设计界面时，有时会面临这样一个问题：需要在窗口指定位置放置组件，并且当窗口位置大小改变时，该组件相对其父对象的位置是不变的。
我们刚开始做的可能是在designer中将组件直接拖到我们期望的位置，编译运行后正是我们想要的结果，但是当我们拖动窗口，使窗口尺寸变化后，发现组件位置改变了，如下面图2所示。发生这种情况的原因很简单，直接拖放组件到界面中，不加任何布局情况下，组件的位置是相对于整个桌面不变的，而当窗口背景图变化时，两者自然而然的就不匹配了。
　　解决这种问题的方法是将组件加入到布局当中，我们知道布局共有3种方式：水平布局、垂直布局、栅格布局；这三种布局方式都能够使组件在一定程度上保持其相对于父窗口的位置不变，但对于水平和垂直相对位置都要固定的问题来说，用棋盘布局无疑是相对方便的，棋盘布局即为QGridLayout，添加组件的代码大致如下：
   QGridLayout *layout=new QGridLayout;
   layout->addWidget(组件);
   其中addwidget函数为void QGridLayout::addWidget(QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan,Qt::Alignment alignment = 0 )
addwidget函数的参数，fromRow为组件起始行，fromColumn为组件起始列，rowSpan为组件的所占行数，columnSpan为组件所占列数，alignment则为组件的对齐方式。
可通过调整addwidget中组件起始行列、所占行列来控制组件在整个窗口中所占的相对大小以及位置，并且能够实现组件的相互重叠。但是这种方式和我们想要的还不是完全相同，因为这样也不能
完全保证组件和背景图的某个位置完全对应，并且窗口布局中某个只存在一个组件时，该组件会占满窗口，这时就要用到按比例设定布局的方法，在designer中，通过改变布局对象中的layoutRowStretch和LayoutColumnStretch属性来设置，其中layoutRowStretch中“0”的个数对应共有几行组件，layoutColumnStretch中“0”的个数对应共有几列组件,“0”为最初的初始值，通过更改该值（百分比），控制不同位置的组件在整个窗口中的大小与位置。
　　在本文中，为了将组件控制在指定位置，我们需要在组件周边放置spacer，水平、垂直方向各两个，这样就可以将窗口分为2行2列，然后通过之前所说的layoutRowStretch和
layoutCColumnStretch中对应值的大小，改变组件的位置和大小。
　　这种方法如果用代码实现，示例：
       gridLayout->setRowStretch(0, 29);
       gridLayout->setRowStretch(1, 3);
       gridLayout->setColumnStretch(0, 28);
       gridLayout->setColumnStretch(1, 11);
       gridLayout->setColumnStretch(2, 6);
窗口组件初始位置是我们期望的组件位置,如图1所示：
![](https://img-blog.csdn.net/20171027105228776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当放大时，组件位置还是原来位置，并没有跟随窗口比例改变，如图2所示：
![](https://img-blog.csdn.net/20171027105252778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
增加布局以后，把组件放到我们的相对位置，如图3所示：
![](https://img-blog.csdn.net/20171027105440264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
增加布局设置以后，我们的组件位置就会随着窗口的放大缩小等比例放大缩小。且相对位置也会等比例改变，如图4所示：
![](https://img-blog.csdn.net/20171027105500342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
