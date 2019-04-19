# QSizePolicy类使用总结 - fanyun的博客 - CSDN博客
2017年12月31日 14:32:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：294标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
       QSizePolicy类是设置组件跟布局之间的伸缩拉伸策略的。大小策略会影响布局引擎处理部件的方式，部件加入布局以后，会返回一个QSizePolicy，描述了其水平和垂直方向的大小策略。可以通过QWidget::sizePolicy属性为特定部件设置大小策略。
      QSizePolicy包含了两个独立的QSizePolicy::Policy值和两个缩放因子，一个描述了部件水平方向上的大小策略，另一个描述了垂直方向上的大小策略。它还包含一个标志表明高度和宽度是否与首选大小有关。水平和垂直方向的大小策略可以在构造函数中设置，也可以通过setHorizontalPolicy()和setVerticalPolicy()函数改变。缩放因子可以使用setHorizontal。Stretch()和setVerticalStretch()函数设置。setHeightForWidth()函数的标志表示部件的缺省大小sizeHint()。horizontalPolicy()、verticalPolicy()、horizontalStretch()和verticalStretch()函数来返回当前的大小策略和缩放因子。另外，使用transpose()函数可以互换水平和垂直的大小策略和缩放因子。hasHeightForWidth()函数返回了当前状态表示的大小依赖性。
      可以使用QSizePolicy类进行大小策略之间的比较，并且可以用QVariant来存储QSizePolicy。
      通过expandingDirections()函数，可以设置组件的展开方向。
