# QLineEdit 使用方法 - Grandyang - 博客园







# [QLineEdit 使用方法](https://www.cnblogs.com/grandyang/p/4587537.html)







在Qt中，QLineEdit是文本编辑框控件，是比较基础且常用的控件的之一，下面是其的一些基本操作。

比如：

禁用或启用该控件

```
// lineedit is the object name of QLineEdit
ui->lineedit.setDisabled(false);
ui->lineedit.setDisabled(true);
```



设置内容或者返回内容：

```
ui->lineedit.setText("hello");
QString s = ui->lineedit.text();
```



将控件设为只读模式：

```
ui->lineedit.setReadOnly(true);
```



控件只接受整型或者浮点型：

```
ui->lineedit->setValidator( new QIntValidator(0, 100, this) );
ui->lineedit->setValidator( new QDoubleValidator(0, 100, 2, this) );
```



文本改变响应函数：

```
void MainWindow::on_lineedit_textChanged() {
    qDebug() << "on_lineedit_textChanged()..." << endl;
}
```














