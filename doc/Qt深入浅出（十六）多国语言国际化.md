# Qt深入浅出（十六）多国语言国际化 - qq769651718的专栏 - CSDN博客












2018年02月24日 00:40:14[吓人的猿](https://me.csdn.net/qq769651718)阅读数：142








# 多国语言国际化

​	Qt自己提供了一个种国际化方案, 生成字典文件的方法来翻译Qt应用中的tr()、translate()字符串，字典文件以”.qm”命名。

​	

## 1 生成一个qm文件


- 
新建一个GUI工程”TestHello.pro”，在UI界面上添加两个按钮,并分别将文本修改为“hello”, “china”.

- 
修改”TestHello.pro”文件,添加如下代码:

TRANSLATIONS += TestHello.ts- 
编译

- 
编译完成后,选择`开始(或者win键)`->  `所有应用` -> `Qt5.7.0`-> `Qt5.7.0 for Desktop (MinGW 5.3.0 32 bit)`菜单项，打开DOS命令行窗口，进入`TestHello工程目录, 执行命令:


```cpp
lupdate  TestHello.pro
```
- 
选择`开始(或者win键)`->  `所有应用` -> `Qt5.7.0`-> `Linguist` 菜单项

- 
在主界面中选择`文件`-> `打开`, 选择`TestHello.ts`文件, 单击`打开`按钮,根据需要设置源语言和目标语言

- 
选择要翻译的字符串，输入对应的翻译文字，单击上面`Mark as done item`按钮

- 
选择`文件`-> `发布`菜单项. 或者在命令行输入`lrelease TestHello.pro`, 在工程目录里面就生成了一个`TestHello.qm`文件.

- 
修改源代码如下:

main.cpp


```cpp
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
​
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator * translator =  new QTranslator;        \\生成一个字典对象
    translator->load("D:\\workspace\\TestHello.qm");    \\加载一个字典文件
    a.installTranslator(translator);                    \\安装字典文件
    MainWindow w;
    w.show();
    return a.exec();
}
```





## 2 时实切换qm文件

​	事先我们编好三本字典分别是lang_zh.qm中文字典、lang_en.qm英文字典、lang_yy.qm粤语字典。

​	Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include <QLabel>
#include <QComboBox>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void reflashLabel();
public slots:
    void changeLangSlot(int);
private:
    QLabel* _label;
    QComboBox * _combobox;
};
​
#endif // WIDGET_H
```



​	Widget.cpp

```cpp
#include "widget.h"
#include <QLabel>
#include <QTranslator>
#include <QComboBox>
#include <QVBoxLayout>
#include <QApplication>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout * vBox = new QVBoxLayout;
    _combobox = new QComboBox; //用到一个下拉选择控件
    _label = new QLabel;
    this->setLayout(vBox);
    vBox->addWidget(_label, 5);
    vBox->addWidget(_combobox, 1);
    _combobox->addItem("English", "en");
    _combobox->addItem("Chinese", "zh");
    _combobox->addItem("YueYu", "yy");
​
    /*下拉选择控件被用户选择某一项后，会发射出一个带某一项索引的信号*/
    connect(_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLangSlot(int)));
    reflashLabel();
}
void Widget::reflashLabel()
{
    _label->setText(tr("LABEL_HELLO", "hello"));
}
​
void Widget::changeLangSlot(int index)
{
    QString name = _combobox->itemData(index).toString();
    static QTranslator* tran = NULL;
​
    if(!tran)
    {
        QApplication::instance()->removeTranslator(tran); //移除已经安装的字典文件
        delete tran;
        tran = NULL;
    }
    /*重新安装字典*/
    tran = new QTranslator;
    tran->load("D:/workspace/untitled11/lang_" + name + ".qm");
    QApplication::instance()->installTranslator(tran);
    reflashLabel();
}
​
Widget::~Widget()
{
​
}
​
```






