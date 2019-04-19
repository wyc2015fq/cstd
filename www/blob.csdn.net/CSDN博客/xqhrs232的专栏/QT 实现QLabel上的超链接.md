# QT 实现QLabel上的超链接 - xqhrs232的专栏 - CSDN博客
2018年08月28日 11:15:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：64
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/robertkun/article/details/20562341](https://blog.csdn.net/robertkun/article/details/20562341)
## QT 实现QLabel上的超链接
方法一:
一个简单的例子：
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QLabel* label = new QLabel("<a href = 要跳转到的链接地址>显示的内容</a>",this);
    label->setFixedSize(200,100);
    label->move(10,10);
    label->show();
    connect(label,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));  //在.h里面定义一个槽private slots: void openUrl(QString url);
}
void Widget::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
设置链接颜色:
ui.labView->setText(QString::fromLocal8Bit("<a style='color: green;' href = www.doshow.com> 查看</a>"));
原文链接：http://hi.baidu.com/tibelf/item/2f27a733ae3ab9f2a984289f
感谢原作者的分享
方法二:
试了一下在QT中真是非常方便地就实现了，只要几条语句
QLabel*openfilelabel=newQLabel(this);
//这句很关键啊，否则就只能通过linkActivated信号，连接到自定义槽函数中打开了
openfilelabel->setOpenExternalLinks(true);
openfilelabel->setText(tr("<a href=\"[http://www.csdn.net/](http://www.csdn.net/)\">打开CSDN"));
openfilelabel.show();
就搞定了![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
原文链接：http://blog.csdn.net/menglongbor/article/details/7734308
补充:
去掉 链接下面的下划线:
setText(QString::fromLocal8Bit("<style> a {text-decoration: none} </style>  <a href =[www.csdn.net](http://www.csdn.net/)> 更换头像</a>"));
