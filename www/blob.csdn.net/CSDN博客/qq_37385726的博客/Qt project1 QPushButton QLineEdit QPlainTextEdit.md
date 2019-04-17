# Qt/project1/QPushButton/QLineEdit/QPlainTextEdit - qq_37385726的博客 - CSDN博客





2017年05月11日 16:06:47[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：187








下面介绍Qt/project1/QPushButton/QLineEdit/QPlainTextEdit

1.QPushBtton类

2.QLineEdit类

3.QPlainTextEdit类

**建立每一个子窗口的时候都要去指定它的父窗口。**






```cpp
#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include //按钮类
#include   //单行编辑类
#include  //无格式文本编辑类
class Mywindow :public QWidget
{
	Q_OBJECT;
public:
	Mywindow(QWidget *parent);
private:
	QPushButton* m_button;
	QLineEdit* m_lineedit;
	QPlainTextEdit* m_plaintextedit;
};
#endif
```



```cpp
#include"Mywindow.h"

Mywindow::Mywindow(QWidget *parent) :QWidget(parent)
{
	m_lineedit = new QLineEdit(this);
	m_lineedit->setGeometry(50, 50, 200, 40);

	m_button = new QPushButton(this);
	m_button->setText("hello,qt");
	m_button->setGeometry(260, 50, 220, 40);

	m_plaintextedit = new QPlainTextEdit(this);
	m_plaintextedit->setGeometry(50, 100, 430, 200);
}
```



```cpp
#include 
#include"Mywindow.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Mywindow window(NULL);
	window.show();
	return a.exec();
}
```







**运行后效果：**

**![](https://img-blog.csdn.net/20170511160617018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



