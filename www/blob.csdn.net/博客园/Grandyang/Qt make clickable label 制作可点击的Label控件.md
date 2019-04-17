# Qt make clickable label 制作可点击的Label控件 - Grandyang - 博客园







# [Qt make clickable label 制作可点击的Label控件](https://www.cnblogs.com/grandyang/p/4299100.html)







Qt中的Label标签控件的作用绝不仅仅限于显示静态文本，其实它的功能很强大，由于其有setPixmap（）成员函数，故而可以当显示图片窗口使用，而且还可以实时显示摄像头捕获的图片，然后它对鼠标事件的支持却没有QWidget那样强大，很多时候我们想要QLabel捕获鼠标单击或双击的事件，它本身无法实现，我们只好写一个新的class来实现我们需要的功能，这也是C++的强大之处所在，继承特性可以让我们在原有的基础上无限增加新的特性。那么这里我们想要QLabel捕获鼠标的双击事件，我们需要新建一个类名为QtClickableLabel，继承于QLabel：

qtclickablelabel.h

```
#ifndef QTCLICKABLELABEL_H
#define QTCLICKABLELABEL_H

#include <QLabel>

class QtClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit QtClickableLabel(QWidget *parent = 0);
    explicit QtClickableLabel(const QString &text = "", QWidget *parent = 0);
    ~QtClickableLabel();

signals:
    void clicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // QTCLICKABLELABEL_H
```



qtclickablelabel.cpp

```
#include "qtclickablelabel.h"

QtClickableLabel::QtClickableLabel(const QString &text, QWidget *parent)
    : QLabel(parent)
{
    this->setText(text);
}

QtClickableLabel::QtClickableLabel(QWidget *parent)
    : QLabel(parent)
{
}

QtClickableLabel::~QtClickableLabel()
{

}

void QtClickableLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    emit clicked();
}
```



如果想实现捕获鼠标单击事件，只需要添加如下代码即可：

```
void QtClickableLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}
```



最后不要忘了在UI Designer把原来的QtLabel的控件promote 到这个QtClickableLabel，尽情享用吧~












