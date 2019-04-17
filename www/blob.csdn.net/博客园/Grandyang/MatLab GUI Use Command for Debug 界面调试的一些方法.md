# MatLab GUI Use Command for Debug 界面调试的一些方法 - Grandyang - 博客园







# [MatLab GUI Use Command for Debug 界面调试的一些方法](https://www.cnblogs.com/grandyang/p/5150947.html)







在MatLab的GUI界面编程，我们在调试的时候需要打印出一些变量，那么介绍下我用到的两种调试方法：



第一种，使用弹出对话框来打印变量，要注意的是打印的东西必须是string类型的，所以其他类型的变量需要转换成string类型的，比如用num2str()函数将数字转为字符串显示出来。我们还可以用class()函数来看一个变量的类型：

```
helpdlg(class(a));
helpdlg(num2str(b));
```



第二种，我们可以借助MatLab的命令行，让变量显示到MatLab的软件界面上，比如说我们要在GUI上加载一个mat文件到MatLab的软件主界面上，我们可以这么做：

```
filename = uigetfile('*.mat');

command = sprintf('load(''%s'')', filename);

evalin('base', command);
```














