# QComboBox 和 QSpinBox 使用方法 - Grandyang - 博客园







# [QComboBox 和 QSpinBox 使用方法](https://www.cnblogs.com/grandyang/p/4310080.html)







Qt中QComboBox 和 QSpinBox 是两个很常用的空间，QComboBox 是下拉菜单，而 QSpinBox 是调参数的神器，所以它们的用法十分必要熟练掌握。

首先来看 QComboBox，这个下拉菜单中的值可以在UI Designer中都设定好，位于第一个位置的就是默认值，也就是程序启动时显示的值，那么在用户选择其他的值，怎么处理呢，只需要添加以下面两种槽函数之一即可：



```
void YourClass::on_combobox_currentIndexChanged(QString type) {
    // Do something here
}
void YourClass::on_combobox_currentIndexChanged(int index) {
    // Do something here
}
```



对于QSpinBox来说，我们可以首先设定它的取值范围，在赋上初始值，然后添加槽函数即可：



```
ui.spinbox->setRange(1, 50);
ui.spinbox->setValue(24);

/**
 * SpinBox valueChanged event for spinbox 
 */
void QtVideoCapture::on_spinbox_valueChanged(int val) {
    // Do something here
}
```














