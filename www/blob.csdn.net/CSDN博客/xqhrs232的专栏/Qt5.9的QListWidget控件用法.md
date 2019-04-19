# Qt5.9的QListWidget控件用法 - xqhrs232的专栏 - CSDN博客
2018年06月14日 14:53:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：180
原文地址::[https://blog.csdn.net/naibozhuan3744/article/details/78988585](https://blog.csdn.net/naibozhuan3744/article/details/78988585)
相关文章
1、[Qt学习笔记 ListWidget的增删改](https://www.cnblogs.com/li-peng/p/3651945.html)----[https://www.cnblogs.com/li-peng/p/3651945.html](https://www.cnblogs.com/li-peng/p/3651945.html)
2、Qt每日小结——QListWidget使用总结----[https://blog.csdn.net/high_cool/article/details/52992877](https://blog.csdn.net/high_cool/article/details/52992877)
本文章主要总结Qt5.9中的listWidget控件的用法。该用法包括在ui界面上添加列表内容和用代码添加列表内容，然后单击第一行内容后，触发单击列表事件，在lineText编辑框中显示单击列表的行号（比如单击列表的第0行，则显示行号为0）。具体编写步骤如下所述。
1.1新建一个Widget工程，基类选择QWidget。在ui界面上拖入一个ListWidget控件和一个lineText控件，如下图所示：
![](https://img-blog.csdn.net/20180106144125286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1.2在ListWidget控件上双击，弹出如下图所示列表编辑框。点击左下角的"+"按钮，再点击右边的"属性"按钮，弹出属性设置框，在属性设置框的text项输入"设备1"，下面还可以设置icon图标，font字体等内容。
![](https://img-blog.csdn.net/20180106144534536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180106144546439?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1.3在listWidget控件鼠标右键，转到槽，选择clicked(QModeIndex)槽函数，如下图所示：
![](https://img-blog.csdn.net/20180106151119237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1.4在槽函数中添加如下代码：
voidWidget::on_listWidget_clicked(constQModelIndex&index)//单击ListWidget的某一行事件触发信号函数{intcurrenRow=ui->listWidget->currentRow();//当前单击选中ListWidget控件的行号（第几行）ui->lineEdit->setText(tr("%1").arg(currenRow));//将这个行号显示在lineText编辑框中}
1.5在Widget构造函数最下面，添加如下代码：
QStringListlist;list<<tr("设备2");//将内容"设备2"赋值给列表第一行。后面还可以赋值给列表的第二行，例如：list<<tr("设备2")<<tr("设备3")ui->listWidget->insertItems(1,list);//在ListWidget控件的第1行插入一个列表list内容
1.6重新构建和运行后，结果如下图所示：
![](https://img-blog.csdn.net/20180106151551415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180106151555992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180106151559286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmFpYm96aHVhbjM3NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
由以上结果可知，单击listWidget列表的设备1，触发单击列表事件信号函数，该函数获取当前选中listWidget的行号，并且在lineText编辑框中显示该行号0。单击设备2，同样在lineText编辑框中心显示行号1。
参考内容：
http://blog.csdn.net/fjb2080/article/details/4922490（重点参考）
http://www.cnblogs.com/venow/archive/2012/10/16/2726576.html
