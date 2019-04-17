# QMessageBox 使用方法 - Grandyang - 博客园







# [QMessageBox 使用方法](https://www.cnblogs.com/grandyang/p/4296395.html)







在Qt中经常需要弹出窗口，QMessageBox可以实现此功能，一共有三种窗口，information, question, 和 warning，critical, about分别对应感叹号，问号和叉号等等,使用方法很简单，一共有三个参数，第一个是父窗口句柄，剩下两个分别为窗口名称和显示内容，显示内容为QString类型。如下所示：



```
QMessageBox::information(this, tr("Title"), tr("Content"));

QMessageBox::question(this, tr("Title"), tr("Content"));

QMessageBox::warning(this, tr("Title"), tr("Content"));

QMessageBox::critical(this, tr("Title"), tr("Content"));

QMessageBox::about(this, tr("Title"), tr("Content"));
```



如果需要对话框下面有Ok, Yes, No 等按钮，需要添加后面两个参数，一个是出现哪两个按钮，一个是设置缺省按钮：



```
QMessageBox::StandardButton r = QMessageBox::question(this, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
if (r == QMessageBox::Yes) {
    // Do something here
}
```














