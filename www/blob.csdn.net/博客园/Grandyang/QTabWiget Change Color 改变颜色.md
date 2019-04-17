# QTabWiget Change Color 改变颜色 - Grandyang - 博客园







# [QTabWiget Change Color 改变颜色](https://www.cnblogs.com/grandyang/p/4304624.html)







Qt中的QTabWiget 类提供了一个标签控件，但是这个控件默认初始化的颜色是白色，和原窗口的颜色不同，看起来非常的违和，所以我们希望将其的背景颜色设为当前窗口的背景颜色。我们所要做的就是先将应用程序窗口的背景颜色取出来，然后再赋给QTabWiget 类的每个标签，比如说我们有tab1和tab2两个标签，要改变其背景颜色可用如下代码：



```
ui.tab1->setPalette(QWidget::palette().color(this->backgroundRole()));
ui.tab1->setAutoFillBackground(true);
ui.tab2->setPalette(QWidget::palette().color(this->backgroundRole()));
ui.tab2->setAutoFillBackground(true);
```



如果要改成某一特定的颜色，比如绿色， 可用如下代码：

```
ui.tab1->setPalette(*(new QPalette(Qt::green)));
ui.tab1->setAutoFillBackground(true);
```














