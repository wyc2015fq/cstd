# QTabWidget标签页设置及获取 - fanyun的博客 - CSDN博客
2017年12月11日 18:47:56[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：8810标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
                
QTabWidget可用于多标签显示，鼠标选中的标签为当前活动标签页。
QTabWidget有个setCurrentIndex槽，可用于修改当前活动标签页。
(1).设置当前Tab页：
```cpp
QTabWidget tabWg = new QTabWidget(this);
        QWidget *Tab1 = new QWidget();
        QWidget *Tab2 = new QWidget();
        tabWg.addTab(Tab1, "Tab1");
        tabWg.addTab(Tab2, "Tab2");
        tabWg.setCurrentIndex(0);
        tabWg.setCurrentIndex(1);
```
ui->tabWidget->setCurrentIndex(1); //设置当前活动页
(2).获取当前标签页索引
ui->tabWidget->currentIndex(); //获取当前活动页的索引值
