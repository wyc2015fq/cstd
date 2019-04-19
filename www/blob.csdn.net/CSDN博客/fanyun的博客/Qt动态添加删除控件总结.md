# Qt动态添加删除控件总结 - fanyun的博客 - CSDN博客
2017年10月28日 19:04:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3089
Qt动态添加控件的方法，具体而言就是根据用户操作生成新的控件到界面，同时，相应的操作可以使得控件动态删除。
使用Qtcreate创建一个UI生成文件，在生成的对象构造函数中，选择要显示的区域，布局，并添加响应的槽函数
槽函数实现：
```cpp
Btumenu*btumenu=newBtumenu(this);
 ui->gridLayout->addWidget(btumenu);
 btumenu->show();
```
每点击一次在相应组件创建需要创建的对象。
remove函数移除组件，注意，只是不显示，但是对象还是存在。
使用delete才能完全删除组件。
