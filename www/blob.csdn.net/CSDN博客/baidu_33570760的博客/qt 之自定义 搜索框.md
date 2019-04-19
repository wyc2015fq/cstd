# qt 之自定义 搜索框 - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:16:53[carman_风](https://me.csdn.net/baidu_33570760)阅读数：646
转载自：http://blog.csdn.net/liang19890820/article/details/50357523
# 简述
关于搜索框，大家都经常接触。例如：浏览器搜索、Windows资源管理器搜索等。
![这里写图片描述](https://img-blog.csdn.net/20151219102309493)
![这里写图片描述](https://img-blog.csdn.net/20151219102323323)
当然，这些对于Qt实现来说毫无压力，只要思路清晰，分分钟搞定。
- 
方案一：调用QLineEdit现有接口
- 
void addAction(QAction * action, ActionPosition position) 
在QLineEdit的前/后添加部件，ActionPosition表示部件所在方位。
- 
QAction * addAction(const QIcon & icon, ActionPosition position) 
重载函数。
枚举：QLineEdit::ActionPosition
|常量|值|描述|
|----|----|----|
|QLineEdit::LeadingPosition|0|当使用布局方向Qt::LeftToRight时，部件显示在文本左侧，使用Qt::RightToLeft则显示在右侧。|
|QLineEdit::TrailingPosition|1|当使用布局方向Qt::LeftToRight时，部件显示在文本右侧，使用Qt::RightToLeft则显示在左侧。|
- 方案二：自定义（可以实现任何组合）
下面，我们来针对自定义进行讲解。
- [简述](http://blog.csdn.net/liang19890820/article/details/50357523#%E7%AE%80%E8%BF%B0)
- [效果](http://blog.csdn.net/liang19890820/article/details/50357523#%E6%95%88%E6%9E%9C)
- [细节分析](http://blog.csdn.net/liang19890820/article/details/50357523#%E7%BB%86%E8%8A%82%E5%88%86%E6%9E%90)
- [Coding](http://blog.csdn.net/liang19890820/article/details/50357523#coding)
- [源码下载](http://blog.csdn.net/liang19890820/article/details/50357523#%E6%BA%90%E7%A0%81%E4%B8%8B%E8%BD%BD)
# 效果
![这里写图片描述](https://img-blog.csdn.net/20151219102454821)
# 细节分析
实现细节需要如下步骤：
- 组合实现，输入框+按钮
- 事件关联
- 获取输入文本，进行文本搜索
为了更人性、易用，这里有一些细节需要注意：
- 输入框的文本不能处于按钮之下
- 输入框无文本时必须给与友好性提示
- 按钮无文本描述，一般需要给予ToolTip提示
- 按钮样式-正常、滑过、按下，以及鼠标滑过鼠标样式手型，
这些都想清楚了，我们就能快速实现一个搜索框了。
# Coding
搜索框实现
```
m_pSearchLineEdit = new QLineEdit();
QPushButton *pSearchButton = new QPushButton(this);
pSearchButton->setCursor(Qt::PointingHandCursor);
pSearchButton->setFixedSize(22, 22);
pSearchButton->setToolTip(QStringLiteral("搜索"));
pSearchButton->setStyleSheet("QPushButton{border-image:url(:/images/icon_search_normal); background:transparent;} \
                                     QPushButton:hover{border-image:url(:/images/icon_search_hover)} \
                                     QPushButton:pressed{border-image:url(:/images/icon_search_press)}");
//防止文本框输入内容位于按钮之下
QMargins margins = m_pSearchLineEdit->textMargins();
m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
m_pSearchLineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));
QHBoxLayout *pSearchLayout = new QHBoxLayout();
pSearchLayout->addStretch();
pSearchLayout->addWidget(pSearchButton);
pSearchLayout->setSpacing(0);
pSearchLayout->setContentsMargins(0, 0, 0, 0);
m_pSearchLineEdit->setLayout(pSearchLayout);
connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));
```
槽函数实现
```
void Widget::search()
{
    QString strText = m_pSearchLineEdit->text();
    if (!strText.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("搜索"), QStringLiteral("搜索内容为%1").arg(strText));
    }
}
```
