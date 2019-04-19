# QT（2）Widget的小例子 - xqhrs232的专栏 - CSDN博客
2016年10月13日 23:01:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：351
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/flowingflying/article/details/6117404](http://blog.csdn.net/flowingflying/article/details/6117404)

在上一次我们处理好了QT学习的环境，现在可以安安静静地去学习QT，可以参考[http://doc.qt.nokia.com/latest/tutorials.html](http://doc.qt.nokia.com/latest/tutorials.html)。下面是step by step，创建我们的第一个QT小程序。
**一、搭建一个QT程序**
> 
**#include <QtGui>**
/* In this example, we will test QApplication usage. */
int main(int argc, char *argv[])
{
      /* 对于一个使用QT的GUI应用，无论是0个或者N个窗口，必须有一个QApplication的对象，而一个非GUI的QT应用，则使用QCoreApplication。他们之间的继承关系：QApplication -> QCoreApplication -> QObject */
**QApplication app**(argc, argv);
      // ... 这里是程序的主体，我们在后面慢慢填充之...    
      return **app.exec();**
}
　　创建一个QAppcliation对象，可以将命令行的参数传递过去。QApplication::exec()将唤起QT事件loop，这也是我们在main最后调用的函数。
**二、创建一个窗口Windows**
> 
#include <QtGui>
/* In this example, we will test QApplication usage. */
int main(int argc, char *argv[])
{
QApplication app(argc, argv);
//如果一个widget创建是没有parent，这样将作为一个window或者一个top-level widget
**      QWidget window;**
      // Test 1: 显示Window，对于设置resize的情况，在MeeGo Handset IA32模拟器中是不生效的，Handset中认为所有的windows都是满屏，后面我将视这个window为panel，在上面加上其他组件。
**     window.resize**(240,120);
**     window**.**setWindowTitle**("Test 1: Show Windows");
**     window.show();**
      return app.exec();
}
**三、在Windows上加载Widget**
> 
#include <QtGui>
/* In this example, we will test QApplication usage. */
int main(int argc, char *argv[])
{
       QApplication app(argc, argv);
       QWidget window;
      window.setWindowTitle("Test 2: Child Widgets");
      window.show();
      //我们采用QPushButton为例子：和不带parent创建windows不一样，button是一个windows的child，当windows destroy时，也将会被删除。注意隐藏和关闭windows是不会导致windows destroy的，但是程序结束可以。
      QPushButton * button = new **QPushButton**("Hello,world!",**&window**);
      button -> **move**(100,100);  //指定button的精确位置
      button -> **show**();
      return app.exec();
}
**四、采用layout加载Widget**
　　一般而言，在程序中，很少直接采用指定精确位置，一般layout的方式。
> 
#include <QtGui>
/* In this example, we will test QApplication usage. */
int main(int argc, char *argv[])
{
QApplication app(argc, argv);
QWidget window;
       window.setWindowTitle("Test 3: Using Layouts");
window.show();
     //通常，子widget通过一个layout的对象进行排版，而不是直接精确地指定位置或者大小。这里我们采用QLable和输入框为例子，在创建的过程中，没有作为windows的子widget。
QLabel * label = new QLabel("Name:");
QLineEdit * lineEdit = new QLineEdit ();
**QHBoxLayout** * layout = new QHBoxLayout();
//Layout对象将管理在其上面的widget的位置和大小，通过addWidget()将widget加上。QHBBoxLayout的H表示水平方向。.
      layout->**addWidget**(label);
      layout->**addWidget**(lineEdit);
//而layout本身加入window通过调用setLayout()。windows加入layout后，则layout上的widget成为window的子widget.
**window.setLayout(layout);**
return app.exec();
}
**五、多层次layout（同时演示QDebug和QList的使用方法）**
![](http://hi.csdn.net/attachment/201109/15/161800_1316088242XANY.png)![](http://hi.csdn.net/attachment/201109/15/161800_1316088284skzr.png)
> 
#include <QtGui>
**#include <QDebug>**//QDebug是很重要的，可以打印QString（不能使用printf）
/* In this example, we will test QApplication usage. */
int main(int argc, char *argv[])
{
      QApplication app(argc, argv);
      QWidget window;
      window.setWindowTitle("Test 4: Nested Layouts");
      window.show();
  //layout有QHBoxLayout和QVBoxLayout，分别是横向或者纵向。此外Qt还提供比较复杂的QGridLayout和QFormLayout。
      QLabel * label = new QLabel("Name:");
      QLineEdit * lineEdit = new QLineEdit ();
**QTableView** * resultView = new QTableView();
      //上面的layout放置一个输入框，如同上一个例子。
      QHBoxLayout * layout = new QHBoxLayout();
      layout->addWidget(label);
      layout->addWidget(lineEdit);
      //创建一个纵向的layout，上面放置一个layout（一个输入框），通过**addLayout**()，下面放一个widget（表格）
** QVBoxLayout** * mainLayout = new QVBoxLayout();
      mainLayout->**addLayout**(layout);
      mainLayout->addWidget(resultView);
      window.setLayout(mainLayout);
   //下面这一部分，在表格（ QStandardItemModel）中加入内容，并且学习一下QT的一些语句用法，学习QList的使用。QList在内存方对象，这这个例子中，存放QStringList数组。在QT中在一个数组中加入一个entry采用“<<"的方式，如果继续加入，则继续“<<"。在表格中，每行有两个元素，放置在QStringList数组中，采用QStringList
 << "1" << "2" 。
      QStandardItemModel * model = new QStandardItemModel();
      model->**setHorizontalHeaderLabels**(QStringList() << "Name" << "Office");//设置title
      QList *rows* = QList()
** <<**(QStringList() << "Verne Nilsen" << "123")
**<<** (QStringList() << "Carlos Tang" << "77")
**<<**(QStringList() << "David Bradley" << "42")
** <<** (QStringList() << "Knut Walters" << "25")
**<<**(QStringList() << "Andrea Jones" << "34")**;**
//从Qlist中取出数据，加入table中。每行使用append()，通过foreach从QList中依此读入元素。由于每个元素是一个QStringList，因此两层嵌套的foreach来获取。
**foreach (QStringList row, ***rows***) **{  //将rows的每个元素读入row
 QList items;
            printf("---------/n"); //用于对比QDebug的使用方式。
 foreach(QString text, row){
**qDebug()** << "debug : " << text ; //显示qDebug的用法
                  items.**append**(new QStandardItem(text)); //在List中加入entry
            }
           model->**appendRow**(items);
      }
      resultView->setModel(model);
      resultView->verticalHeader()->hide(); //如果不设置行列的名称，缺省为1，2，3，...，这个可以隐藏这些行列的名称
      resultView->horizontalHeader()->setStretchLastSection(true); //将每行最后一个entry，填满空间
      return app.exec();
}
相关链接：[我的MeeGo/Moblin相关文章](http://blog.csdn.net/flowingflying/archive/2009/09/12/4547085.aspx)
