# QSlider 拖动滑动条使用valueChanged(int value)信号重复调用槽函数问题解决 - xqhrs232的专栏 - CSDN博客
2018年08月29日 15:52:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：132
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/u011857683/article/details/77865581?locationNum=5&fps=1](https://blog.csdn.net/u011857683/article/details/77865581?locationNum=5&fps=1)
使用三个槽，防止滑动的时候重复调用，只有滑动停止才会去改变这个值
- 
`//mydialog.h `
- 
`#ifndef MYDIALOG_H `
- 
`#define MYDIALOG_H `
- 
`#include <QDialog> `
- 
`class QLineEdit; `
- 
`class QSlider; `
- 
`class MyDialog : public QDialog `
- 
`{ `
- 
`Q_OBJECT `
- 
`public: `
- 
`explicit MyDialog(QWidget *parent = 0); `
- 
`signals: `
- 
`public slots: `
- 
`void setLineEditValue(int value); `
- 
`private: `
- 
`QLineEdit *lineEdit; `
- 
`QSlider *slider; `
- 
`}; `
- 
`#endif // MYDIALOG_H `
- 
`//mydialog.cpp `
- 
`#include "mydialog.h" `
- 
`#include <QtGui/QLineEdit> `
- 
`#include <QtGui/QSlider> `
- 
`#include <QtGui/QHBoxLayout> `
- 
`MyDialog::MyDialog(QWidget *parent) : `
- 
`QDialog(parent) `
- 
`{ `
- 
`lineEdit = new QLineEdit("50"); `
- 
- 
`//新建一个水平方向的滑动条QSlider控件 `
- 
`slider = new QSlider(Qt::Horizontal); `
- 
`//设置滑动条控件的最小值 `
- 
`slider->setMinimum(0); `
- 
`//设置滑动条控件的最大值 `
- 
`slider->setMaximum(100); `
- 
`//设置滑动条控件的值 `
- 
`slider->setValue(50); `
- 
- 
`//信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本 `
- 
`connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int))); `
- 
- 
`QHBoxLayout *layout = new QHBoxLayout(); `
- 
`layout->addWidget(lineEdit); `
- 
`layout->addWidget(slider); `
- 
- 
`this->setLayout(layout); `
- 
`this->resize(200, 50); `
- 
`this->setWindowTitle("QSliderDemo"); `
- 
`} `
- 
- 
`void MyDialog::setLineEditValue(int value) `
- 
`{ `
- 
`int pos = slider->value(); `
- 
`QString str = QString("%1").arg(pos); `
- 
`lineEdit->setText(str); `
- 
`} `

- 
`//main.cpp `
- 
`#include <QtGui/QApplication> `
- 
`#include "mydialog.h" `
- 
`int main(int argc, char *argv[]) `
- 
`{ `
- 
`QApplication a(argc, argv); `
- 
`MyDialog dialog; `
- 
`dialog.show(); `
- 
`return a.exec(); `
- 
`} `
程序运行截图：
![](http://hi.csdn.net/attachment/201110/23/0_13193437197E1K.gif)
**本文转自：**
**http://blog.csdn.net/qiurisuixiang/article/details/6897914**
