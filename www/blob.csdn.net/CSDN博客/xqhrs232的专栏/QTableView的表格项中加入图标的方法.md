# QTableView的表格项中加入图标的方法 - xqhrs232的专栏 - CSDN博客
2018年05月16日 17:34:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：389
原文地址::[https://blog.csdn.net/rabinsong/article/details/13158281](https://blog.csdn.net/rabinsong/article/details/13158281)
相关文章
1、QT QTableView用法小结（设置单元格字体颜色，文本字体...）----[https://blog.csdn.net/Devil_box/article/details/47803523](https://blog.csdn.net/Devil_box/article/details/47803523)
2、QTableView 一个单元格中添加按钮----[https://blog.csdn.net/ljt350740378/article/details/50721451](https://blog.csdn.net/ljt350740378/article/details/50721451)
3、qt QTableView 的使用(嵌入QCheckobox,为某一单元格设置颜色，单击，双击，右键菜单QMenu)----[https://blog.csdn.net/w383117613/article/details/51360426](https://blog.csdn.net/w383117613/article/details/51360426)
当在使用表格视图的时候，需要在表格每一行前面加入图标，应该怎么做呢？Qt中通过使用MVC的处理方式，很容易做到这一点，具体实现如下：
先贴出图，让大家一睹为快
![](https://img-blog.csdn.net/20131026223240187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFiaW5zb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面我就来介绍一下，上图的灯泡是怎么实现的，通过重载QAbstractTableModel中的data方法，如下：（CTblModel 派生自QAbstractTableModel）
[cpp][view plain](https://blog.csdn.net/rabinsong/article/details/13158281#)[copy](https://blog.csdn.net/rabinsong/article/details/13158281#)
- QVariant CTblModel::data(const QModelIndex &index, int role) const
- {  
- if (!index.isValid())  
- return QVariant();  
- 
- int col = index.column();  
- if (col == ledColIndex && role == Qt::DecorationRole)  
-     {  
- return QIcon(":/images/light.png");  
-     }  
- else
-     {  
-         ...  
-     }  
- return QVariant();  
- }  
对于mvc的使用方法，请参考http://blog.csdn.net/rabinsong/article/details/8452946
让图片动起来，能够响应用户事件，当用户单击灯泡时，灯泡会点亮，这个怎么实现呢？
要实现这个功能，首先要能接受到鼠标事件，其次，要知道鼠标点击了灯泡部分，不能鼠标不在灯泡上点击，灯泡也亮。有了这两点，下面我们来看看实现：
我们可以通过重载QTableView的 mousePressEvent()，从而获得对鼠标单击的控制权，通过indexAt方找到当前单击的index，再根据索引找到灯泡所在的列，具体实现如下：
[cpp][view plain](https://blog.csdn.net/rabinsong/article/details/13158281#)[copy](https://blog.csdn.net/rabinsong/article/details/13158281#)
- void CTblView::mouseMoveEvent(QMouseEvent *event)  
- {  
- const QPoint &p = event->pos();  
-     QModelIndex modelIndex = indexAt(p);  
- if (modelIndex.isValid())  
-     {  
- int col = modelIndex.column();  
- int row = modelIndex.row();  
- if (col == ledColIndex)  
-         {  
-             pTblModel->setLight(row);  
-         }  
-     }  
- }  
pTblModel就是上面定义的CTblModel，到了这里大家应该知道了基本实现思路了吧。
让图标变大一些，按照上面的做法，灯泡图标的大小很小，不管你的light.png图片多大，在表格中显示时的图标大小默认都很小，那么怎么改变图标的大小呢？方法也很简单，就是在CTblView构造函数中加入setIconSize(QSize(25,25));我设置的是25*25大小，显示效果如上图的灯泡效果，大家可以根据自己的应用，调整其大小。
