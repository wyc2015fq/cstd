# QPushButton 与 QListWidget 的按键响应 - Grandyang - 博客园







# [QPushButton 与 QListWidget 的按键响应](https://www.cnblogs.com/grandyang/p/4287249.html)







在Qt中添加按钮或表格控件时需要添加其按键响应，一般来说有两种方法，一种是直接实现其响应函数，第二种是自己写一个响应函数，然后用Qt的信号槽将它们连接起来。愚以为第一种方法更为简单一些。

声明这些控件：

```
QPushButton *pbutton;
QListWidget *lwidget;
```



然后实现它们的响应函数：

```
void YourClass::on_pbutton_clicked() {
  // Implement here
}
void YourClass::on_lwidget_itemClicked(QListWidgetItem* item) {
  // Implement here
}
void YourClass::on_lwidget_itemEntered(QListWidgetItem* item) {
  // Implement here
}
```



别忘了在头文件中加入私有槽的声明：

```
private slots:
  void on_pbutton_clicked();
  void on_lwidget_itemClicked(QListWidgetItem* item);
  void on_lwidget_itemEntered(QListWidgetItem* item);
```



itemClicked() 和  itemEntered() 的区别：

一般来说这两个可以一起使用，前者是当表格中的项目被点击后触发事件，后者是当鼠标按下时滑动到别的项目时触发事件。












