# Qt resizeEvent 控件居中设置 - Grandyang - 博客园







# [Qt resizeEvent 控件居中设置](https://www.cnblogs.com/grandyang/p/4523632.html)







在Qt中我们有时需要让一个控件在窗口居中或是在父控件中居中，而且在窗口拉伸时仍然保持正中央的位置。这里介绍一种方法，用到了主窗口的响应函数resizeEvent(QResizeEvent*event)，这个响应函数在窗口的大小改变时会自动调用，该函数如下：



```
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here
}
```



需要注意的是，在主程序窗口生成时，会首先调用下这个函数，这个函数可以返回主窗口的大小，但是不能准确的返回上面的控件的大小，这可能是个bug，所以我们如果想要找到上面的一个控件的正中间的位置，需要根据主窗口的大小的值来定位中间控件的中心。



```
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here
   int width = ui->centralWidget->width(), height = ui->centralWidget->height();
   ui->item->move(width * 0.25 - 80, (height - 320) / 2 - 8);
   ui->item->move(width * 0.25 - 80, (height - 320) / 2 - 8);
}
```














