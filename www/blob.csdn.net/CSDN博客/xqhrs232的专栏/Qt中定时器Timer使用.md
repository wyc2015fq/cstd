# Qt中定时器Timer使用 - xqhrs232的专栏 - CSDN博客
2018年05月29日 08:56:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：127
原文地址::[https://blog.csdn.net/sun980984305/article/details/52729465?locationNum=4](https://blog.csdn.net/sun980984305/article/details/52729465?locationNum=4)
相关文章
1、QT定时器使用的实例----[https://blog.csdn.net/feiyangyangfei/article/details/8662365](https://blog.csdn.net/feiyangyangfei/article/details/8662365)
2、Qt中两种定时器用法----[https://blog.csdn.net/steven6977/article/details/9310997](https://blog.csdn.net/steven6977/article/details/9310997)
软件平台：Qt5.3
电脑操作系统：Win7 64位
1.QTimeEvent类用来描述一个定时器事件的。通过函数startTimer来开启一个定时器，这个函数需要一个以毫秒为单位的证书作为参数来表明设定的时间，函数返回一个整型编号来代表这个定时器。当定时器溢出时就在timerEvent（）函数中获取定时器的编号来进行相关操作。
id1=startTimer（1000）；//开启一个1秒定时器，返回其ID
在定时器处理函数中进行定义：
void Widget：：timerEvent（QTimerEvent *event）
{
  if（event->timerId()==id1）        //判断是哪个定时器
   qDebug()<<”timer1”;
}
   使用QTimerEvent里面的timerId（）函数来获取定时器的编号。
2.使用QTimer类来实现一个定时器，它提供了一个更高层次的编程接口。比如可以使用信号和槽，还可以设置只运行一次的定时器。如果使用定时器，一般都是使用QTimer类的。
QTimer *timer = new QTimer(this); //新建定时器
connect(timer,SIGNAL(timeout()),this,SLOT(on_pushButton_clicked()));
 //关联定时器的溢出信号到槽上
timer->start(1000);//定时设置，设置为1秒溢出
然后在定时器溢出信号的槽函数中进行定义
另外一些时间要用到的函数
QTime time =QTime::currentTime(); //获取当前时间
QString b=time.toString("hh:mm");//将时间数据转换成字符串数组
在lineEdit中回车获取其中的内容的槽函数写法
void MainWindow::on_lineEdit_returnPressed()
{
   ui->lineEdit->setFocus(); //获取lineEdit里面的数据
    QString  str =ui->lineEdit->text();//数据赋值给字符串数组
}
