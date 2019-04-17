# QRadioButton 使用方法 - Grandyang - 博客园







# [QRadioButton 使用方法](https://www.cnblogs.com/grandyang/p/4594463.html)







QRadioButton 控件是Qt中实现多选一功能的控件，它的使用方法如下：

声明控件：

```
QRadioButton *rbutton;
```



然后实现它的响应函数：

```
void YourClass::on_rbutton_toggled(bool state) {
  // Implement here
  if (state) {
    // TO DO
  }
  else {
    // TO DO
  }
}
```



别忘了在头文件中加入私有槽的声明：

```
private slots:
  void on_rbutton_toggled(bool state);
```














