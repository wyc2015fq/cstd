# qt之 透明提示框 - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:13:31[carman_风](https://me.csdn.net/baidu_33570760)阅读数：325
转载自： http://blog.csdn.net/liang19890820/article/details/51029825
# 简述
经常使用企鹅的小伙伴一定对登录失败的提示框很熟悉，主要涉及窗口透明并添加图标、提示信息、关闭按钮的显示等。
我们可以利用QWidget创建一个提示框，然后通过样式设置我们想要的效果。
- [简述](http://blog.csdn.net/liang19890820/article/details/51029825#%E7%AE%80%E8%BF%B0)
- [效果](http://blog.csdn.net/liang19890820/article/details/51029825#%E6%95%88%E6%9E%9C)
- [源码](http://blog.csdn.net/liang19890820/article/details/51029825#%E6%BA%90%E7%A0%81)
- [样式](http://blog.csdn.net/liang19890820/article/details/51029825#%E6%A0%B7%E5%BC%8F)
# 效果
![这里写图片描述](https://img-blog.csdn.net/20160331202116812)
# 源码
QMessageWidget.h
```
#ifndef MESSAGE_WIDGET
#define MESSAGE_WIDGET
#include <QWidget>
class QLabel;
class QPushButton;
class QMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMessageWidget(QWidget *parent = 0);
    ~QMessageWidget();
    // 设置显示文本
    void setText(const QString &text);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QLabel *m_pMessageLabel;
};
#endif // MESSAGE_WIDGET
```
QMessageWidget.cpp
```
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include "QMessageWidget.h"
QMessageWidget::QMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(25);
    setAutoFillBackground(true);
    setObjectName("messageWidget");
    // 提示图标
    QLabel *pIconLabel = new QLabel(this);
    m_pMessageLabel = new QLabel(this);
    QPushButton *pCloseButton = new QPushButton(this);
    pCloseButton->setFixedSize(8, 8);
    pIconLabel->setFixedSize(16, 16);
    pIconLabel->setScaledContents(true);
    pIconLabel->setObjectName("informationLabel");
    m_pMessageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pMessageLabel->setObjectName("highlightLabel");
    m_pMessageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pCloseButton->setObjectName("closeTipButton");
    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->addWidget(pIconLabel);
    pLayout->addWidget(m_pMessageLabel);
    pLayout->addWidget(pCloseButton);
    pLayout->setSpacing(5);
    pLayout->setContentsMargins(3, 3, 5, 3);
    setLayout(pLayout);
    connect(pCloseButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}
QMessageWidget::~QMessageWidget()
{
}
// 设置显示文本
void QMessageWidget::setText(const QString &text)
{
    m_pMessageLabel->setText(text);
}
// 设置样式需要重写
void QMessageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
```
# 样式
```
// 界面样式
QWidget#messageWidget {
    background: rgba(255, 255, 255, 20%);
}
// 提示信息样式
QLabel#highlightLabel {
        color: rgb(0, 160, 230);
}
// 图标样式
QPushButton#closeTipButton {
        border-radius: none;
        border-image: url(:/Images/tipClose);
        background: transparent;
}
QPushButton#closeTipButton:hover {
        border-image: url(:/Images/tipCloseHover);
}
QPushButton#closeTipButton:pressed {
        border-image: url(:/Images/tipClosePressed);
}
```
这里实现了设置信息，隐藏等效果，可以在此基础上进行扩展。背景色、字体颜色、图标等样式都可以自行设置，主要是实现思路，愿大家共勉
