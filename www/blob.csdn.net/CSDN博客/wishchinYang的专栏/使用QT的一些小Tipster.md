# 使用QT的一些小Tipster - wishchinYang的专栏 - CSDN博客
2014年07月11日 11:30:09[wishchin](https://me.csdn.net/wishchin)阅读数：601
个人分类：[QT./Linux](https://blog.csdn.net/wishchin/article/category/1721683)
1.在使用Qt Creator编程时，难免会用到将**float类型转换为QString类型**的方法：[原文](http://blog.csdn.net/leo115/article/details/7757118)
1.1. 将QString类型转化为float类型，很简单
    QString data;            
    float num = data.toFloat();
    即可很轻松的实现。
1.2. 但是如何将float类型转化为QString类型呢？
    查看API很难发现封装好的转化函数
    可以尝试使用下面的代码转化：
    float num = 1.222;
    QString data = QString("float is %1").arg(num);
    输出结果是：float is 1.222
    如果只要float转化成的数值，则使用如下：
    QString data = QString("%1").arg(num);
至此完毕。。。。。。。。。。。。。。。。
2.QLineEdit输入**类型限制，[原文](http://blog.sina.com.cn/s/blog_6e198d4f0100mxbg.html)**
2.1输入int类型
QValidator *validator=new QIntValidator（100,999，this）；
QLineEdit *edit=new QLineEdit（this）；
edit->setValidator(validator);
也可以表示成
 QLineEdit *edit=new QLineEdit（this）；
 edit->setValidator(newQIntValidator(100,999,this));
2.2.输入double类型
QLineEdit *edit=new QLineEdit（this）；
edit->setValidator(new QDoubleValidator(doublebottom,double top,，int decimal，this));
 bottom为最小范围，top为最大范围，decimal为精确度，精确到第几位小数。
2.3输入任意匹配类型
QRegExpregExp（“[A-Za-z][1-9][0-9]{0,2}”）;//正则表达式，第一位数字为a-z的数字，第二位为1-9的数字，后边是0-2位0-9的数字
lineEdit->setValidator(newQRegExpValidator(regxp,this));
**3.　LineEdit控件成员：[http://book.51cto.com/art/201203/323504.htm](http://book.51cto.com/art/201203/323504.htm)**
1.控件位置
Input Widgets→LineEdit
2.控件介绍
LineEdit控件（行编辑）是一种常用且比较容易掌握的控件。应用程序主要使用它来接收输入文字信息，样式如图3-65所示。
图3-65　LineEdit控件
3.控件设置选项
在LineEdit控件的properties选项中，一般常对以下选项进行设置。
name：该控件对应源代码中的名称；
text：该控件对应图形界面中显示的名称；
font：设置text的字体；
ReadOnly：用来获取或设置一个值，该值指示文本框中的文本是否为只读，值为true时为只读，值为false时为可读写。
4.常用成员函数
- 1) QLineEdit::QLineEdit ( QWidget *parent, const char *name = 0 ) 
构造一个名称为name、父对象为parent的LineEdit。
- 2) QLineEdit::QLineEdit ( const QString & contents, QWidget *parent, const char *name = 0 ) 
构造一个名称为name、父对象为parent和内容为contents的LineEdit。
- 3) void QLineEdit::clear () ［virtual slot］ 
清除行编辑的内容。
- 4) bool QLineEdit::isReadOnly () const 
如果行编辑是只读则返回true，否则返回false。
- 5) void QLineEdit::setReadOnly ( bool )［virtual slot］ 
设置行编辑的读写属性。
- 6) void QLineEdit::setText ( const QString & ) ［virtual slot］ 
设置行编辑的文本。
- 7) QString QLineEdit::text () const 
返回行编辑的文本。
