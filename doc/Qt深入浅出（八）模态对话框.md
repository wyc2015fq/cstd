# Qt深入浅出（八）模态对话框

2018年02月24日 00:30:45 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：276



# 模式对话框

​	对话框可以分为模式对话框和非模式对话框两种。模式对话框是指用户只能在当前的窗体中进行操作，在该窗体没有关闭之前不能切换到其它的窗体。非模式对话框是指当前的所操作的窗体可以切换。

​	简单来说，屏蔽同一应用程序中其它窗口事件响应的对话框，就叫做模式对话框。

​	一般情况下，用于确认信息的操作对话框属于模式对话框。如打开文件对话框就是典型的模式对话框，而常见的查找和替换便是非模式对话框。

​	在Qt中只要是继承至QDailog的窗口类都可以以模式对话框的方式来显示。

## 1 QDailog与QWidget对比



​          

 

  QWidget

   1、指定父窗口的时候，不调用QWidget::show()，嵌套在父窗口中显示，与其它窗口共用消息循环。

2、指定父窗口的时候，调用QWidget::show()，仍然嵌套在父窗口中显示，与其它窗口共用消息循环。

   3、不指定父窗口的时候，调用QWidget::show()，以顶级窗口方式显示，与其它窗口共用消息循环。

   4、不能以模态窗口方式显示，没有exe()函数。



  QDailog继承至QWidget

   1、指定父窗口的时候，不调用QWidget::show()，不显示窗口。

   2、指定父窗口的时候，调用QWidget::show()，以顶级窗口方式显示，与其它窗口共用消息循环。

3、不指定父窗口的时候，调用QWidget::show()，以顶级窗口方式显示，与其它窗口共用消息循环。

   4、模态窗口显示,，调用QDailog::exec()，屏蔽其它窗口消息循环。





注意：简单来看，如果想以模式窗口显示，继承QDailog，调用QDailog::exec()即可。



## 2 触发一个模式对话框

​	例如：

```cpp
#include "widget.h"
#include <QPushButton>
#include <QDialog>
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    QPushButton * pb = new QPushButton("dialog", this);
    connect(pb, SIGNAL(clicked()), this, SLOT(showDialog));
}
void Widget::showDialog()
{
    QDialog dialog;
    dialog.exec(); // 消息循环，阻塞等待模式窗口关闭。
}
```



## 3 模式窗口exec返回值

```cpp
[virtual slot] int QDialog::exec()
```

​	这个函数是模式窗口的关键，它会屏蔽原先的QApplication::exec的消息循环接收消息，所有的消息都被它接收走，直到它返回，其返回值为1时代表QDialog::Accepted，为0时代表QDialog::Rejected。

- 如果想返回QDialog::Accepted，那么只需调用一下QDialog::accept槽函数

```cpp
[virtual slot] void QDialog::accept()
```

- 如果想返回QDialog::Rejected，那么只需调用一下QDialog::reject槽函数

```cpp
[virtual slot] void QDialog::reject()
```

- 如果想返回其它自定义的值，那么只需要调用一下QDialog::done槽函数

```cpp
[virtual slot] void QDialog::done(int r) //r参数来指定exec的返回值
```







来看一个复杂一点的例子：

```cpp
#include "widget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
​
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initMainWidget();
    initDialog();
}
void Widget::initMainWidget()
{
    QHBoxLayout * hBox = new QHBoxLayout(this);
    QPushButton * pb  = new QPushButton("pb", this);
    hBox->addWidget(pb);
    this->setLayout(hBox);
    connect(pb, SIGNAL(clicked()), this, SLOT(showDailog())); //按钮触发调用槽函数来显示对话框.
}
​
void Widget::initDialog()
{
    QPushButton * pbEnter  = new QPushButton("enter", this);
    QPushButton * pbCancel  = new QPushButton("cancel", this);
    QHBoxLayout * hBox = new QHBoxLayout(this);
    _dialog = new QDialog(this);
​
    _dialog->setLayout(hBox);
    hBox->addWidget(pb_enter);
    hBox->addWidget(pb_cancel);
​
    connect(pbEnter, SIGNAL(clicked()),  _dialog, SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked()), _dialog, SLOT(reject()));
}
​
void Widget::showDailog()
{
    int ret = _dialog->exec(); //使用模式窗口显示
    qDebug() << "show Dialog  return value " << ret << endl;
}
​
Widget::~Widget()
{
​
}
```





## 4 常见Dialog应用

​	常见的继承至QDialog的派生类，QFileDialog，QFontDialog，QColorDialog，QMessageBox，它们有一个共同特点，都提供了快速创建模式窗口的静态函数。









### 4.1 QFileDialog文件浏览框

- 打开文件浏览框，用户选择完成后，返回对应的文件名

```cpp
[static] QString QFileDialog::getOpenFileName(QWidget *parent = Q_NULLPTR, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = Q_NULLPTR, Options options = Options()) 
 /*
 *QWidget *parent 指定父窗口
 *const QString &caption 指定窗口标题
 *const QString &dir 指定打开的路径，模式情况下是当前路径
 *const QString &filter 指定文件的过滤器
 */
```

- 保存文件浏览框，用户选择完成后，返回对应的文件名

```cpp
[static] QString QFileDialog::getSaveFileName(QWidget *parent = Q_NULLPTR, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = Q_NULLPTR, Options options = Options())
  /*
 *QWidget *parent 指定父窗口
 *const QString &caption 指定窗口标题
 *const QString &dir 指定打开的路径，模式情况下是当前路径
 *const QString &filter 指定文件的过滤器
 */
```







例如：

widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void showFileDailog();
    void showSaveFileDialog();
private:
    QString _filename;
};

#endif // WIDGET_H

```

widget.cpp

```cpp
#include "widget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout * hBox = new QHBoxLayout(this);
    QPushButton * pb  = new QPushButton("showFileDailog", this);
    QPushButton * pb1  = new QPushButton("showSaveFileDailog", this);
    hBox->addWidget(pb);
    hBox->addWidget(pb1);
    this->setLayout(hBox);
    connect(pb, SIGNAL(clicked()), this, SLOT(showFileDailog())); //按钮触发调用槽函数来显示对话框.
    connect(pb1, SIGNAL(clicked()), this, SLOT(showSaveFileDialog())); //按钮触发调用槽函数来显示对话框.
​
}
void Widget::showSaveFileDialog()
{
    _filename = QFileDialog::getSaveFileName(this, "save", _filename, "Image(*.png *.jpg)");
    qDebug() << "get save filename :" << _filename << endl;
}
​
void Widget::showFileDailog()
{
   _filename = QFileDialog::getOpenFileName(this, "open", _filename, "Image(*.png *.jpg)");
   qDebug() << "filename :" << _filename << endl;
}
```





### 4.2 QFontDialog字体对话框

​	QFontDialog字体对话框用来获取一个字体格式，那么在这之前，我们需要了解一下什么是字体。

#### 4.2.1 QFont类

​	Qt中的字体类为QFont，没有继承任何Qt基类，我们知道字是有大小、字体格式，那这些字体格式就保存在QFont对象中。

常用函数有:

```cpp
QString QFont::family() const; //获取字体格式
int QFont::pointSize() const;  ///获取字体大小
void QFont::setFamily(const QString &family); //设置字体格式
void QFont::setPointSize(int pointSize); //设置字体大小
```

#### 4.2.2 获取系统支持的所有字体

```cpp
QStringList QFontDatabase::families(WritingSystem writingSystem = Any) const
```

例如：

```cpp
QFontDatabase fontDb;
qDebug() << fontDb.families() << endl;
```



#### 4.2.3 QWidget的QFont对象

​	在QWidget窗口上显示字也需要一个QFont成员对象，用来记录该窗口上显示文本信息时所有使用的字体格式。

- QWidget获取字体对象

```cpp
const QFont & QWidget::font() const
```

- QWidget设置窗口对应字体

```cpp
void QWidget::setFont(const QFont &)
```

例如, 改变Widget对象的字体格式

```cpp
#include "widget.h"
#include <QLabel>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QLabel* label = new QLabel("label", this);
    QFont font = this->font();
    font.setFamily("Verdana"); //将Widget对象的字体格式改为Verdana
    font.set
    this->setFont(font);
}
```











#### 4.2.4 使用QFontDialog获取字体

- 字体对话框可以用来选择一种字体，返回对应的QFont对象，常用静态函数如下：

```cpp
[static] QFont QFontDialog::getFont(bool *ok, const QFont &initial, QWidget *parent = Q_NULLPTR, const QString &title = QString(), FontDialogOptions options = FontDialogOptions());
/*
*bool *ok 通过这个值来判断获取字体是否成功
*QWidget *parent 指定父窗口
*const QString &title 指定标题
*/
```



例如：

```cpp
#include "widget.h"
#include <QFontDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout * hBox = new QHBoxLayout(this);
    QPushButton * pb  = new QPushButton("showFontDailog", this);
    QLabel * label = new QLabel("label", this);
​
    hBox->addWidget(pb);
    hBox->addWidget(label);
​
    this->setLayout(hBox);
    connect(pb, SIGNAL(clicked()), this, SLOT(showFontDailog())); //按钮触发调用槽函数来显示对话框.
​
}
void Widget::showFontDailog()
{
    bool ok;
    QFont initFont = this->font();
    QFont font = QFontDialog::getFont(&ok, initFont, this);
    if(ok)
    {
        qDebug() << font << endl;
        this->setFont(font);
    }
}
​
```



### 4.3 QColorDialog颜色对话框

- QColor类

  QColor用来表示颜色，没有继承任何Qt基类，我们知道界面必然涉及到颜色相关的属性，那这些属性值都是一个个QColor对象来保存。

- QPalette类

  QPalette调色板，由于QWidget需要多个QColor对象来保存窗口丰富的色彩，Qt又把所有不同用处的颜色封装到QPalette类中。可以通过QWidget::palette函数来获取对应窗口的调色板。

  ```cpp
  const QPalette & QWidget::palette() const;
  ```

  然后再通过QPalette::setColor函数来设置不同用途的颜色。

  ```cpp
  void QPalette::setColor(ColorRole role, const QColor &color);
  /*
  * ColorRole role枚举类型，其值可以是：
  * QPalette::Window 背景色
  * QPalette::WindowText 字体颜色
  * 等等
  */
  ```

  例如:

  ```cpp
  #include "widget.h"
  #include <QLabel>
  #include <QDebug>
  Widget::Widget(QWidget *parent)
      : QWidget(parent)
  {
      new QLabel("label", this);
      QPalette palette = this->palette();
      palette.setColor(QPalette::WindowText, QColor(Qt::red));
      this->setPalette(palette);
  }
  ```





- QColorDialog常用静态函数getColor，用来获取一个QColor对象

  ```cpp
  #include "widget.h"
  #include <QHBoxLayout>
  #include <QPushButton>
  #include <QDebug>
  #include <QLabel>
  #include <QColorDialog>
  ​
  Widget::Widget(QWidget *parent)
      : QWidget(parent)
  {
  ​
      QHBoxLayout * hBox = new QHBoxLayout(this);
      QPushButton * pb  = new QPushButton("showColorDailog", this);
      QLabel * label = new QLabel("label", this);
  ​
      hBox->addWidget(pb);
      hBox->addWidget(label);
  ​
      this->setLayout(hBox);
      connect(pb, SIGNAL(clicked()), this, SLOT(showColorDailog())); //按钮触发调用槽函数来显示对话框.
  ​
  }
  void Widget::showColorDailog()
  {
      QPalette palette = this->palette();
      QColor initColor = palette.color(QPalette::Background);
  ​
      QColor color = QColorDialog::getColor(initColor, this, "getcolor");
      if(color.isValid())
      {
          palette.setColor(QPalette::Background, color);
          qDebug() << color << endl;
          this->setPalette(palette);
      }
  }
  ```

  ### 4.4 QMessageBox消息对话框

  常用静态函数：

- 警告对话框

  ```cpp
  [static] StandardButton QMessageBox::warning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
  /*
  * QWidget *parent 指定父窗口
  * const QString &title 窗口标题
  * const QString &text  警告内容 
  */
  ```

- 信息对话框

  ```cpp
  [static] StandardButton QMessageBox::information(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
  /*
  * QWidget *parent 指定父窗口
  * const QString &title 窗口标题
  * const QString &text  信息内容 
  */
    
  ```

  

  

- 疑问对话框

  ```cpp
  [static] StandardButton QMessageBox::question(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = StandardButtons( Yes | No ), StandardButton defaultButton = NoButton);
  /*
  * QWidget *parent 指定父窗口
  * const QString &title 窗口标题
  * const QString &text  疑问内容 
  */
  ```