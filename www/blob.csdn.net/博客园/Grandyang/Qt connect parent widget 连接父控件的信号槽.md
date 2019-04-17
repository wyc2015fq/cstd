# Qt connect parent widget 连接父控件的信号槽 - Grandyang - 博客园







# [Qt connect parent widget 连接父控件的信号槽](https://www.cnblogs.com/grandyang/p/4300495.html)







Qt中的信号槽系统是不同类中间传递数据的神器，如果连接父子空间之间的信号槽很重要，在父类中实例化子类的时候一定要注意将父类连上，不然信号槽无法使用，比如若子类是个对话框Dialog类，一定不要忘了加this

```
QtClass *qc = new QtClass(this);
qc->exec();
```



然后如果需要连接到父控件的槽函数，只需在子类的构造函数中加一句：

```
connect(this, SIGNAL(signal()), parentWidget(), SLOT(slot()));
```














