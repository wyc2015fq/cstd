# Ubuntu10.04下安装Qt4和创建第一个Qt程序 - DoubleLi - 博客园






**1.首先安装Qt4并采用Qt Creator进行开发演示**

（1）在Terminal中输入:



```
sudo apt-get install qt4-dev-tools qt4-doc qt4-qtconfig qt4-demos qt4-designer
```

 其中：

　　 qt4-dev-tools 中包括了Qt Assistant,Qt Linguist,Qt Creator
     qt4-doc 这个是帮助文档
     qt4-qtconfig Qt的配置工具,这个装好默认好
     qt4-demos 官方的一些Demo
     qt4-designer 可视化窗体设置工具

安装完成后,在“应用程序->编程”下面会看到多出Qt Creator ,Qt4助手 ,Qt4设计器,Qt Linguist等

**2.用Qt Creator创建我的第一个Qt程序hello world**

1.新建一个空的Qt4项目，选择其他项目，在右边选择空的Qt项目。

![](https://pic002.cnblogs.com/images/2011/361355/2011121613213145.png)

2.然后就是对项目命名，跟着向导做，就完成了项目的构建。

3.在Qt Creator的菜单栏中选择“窗体->显示边栏”，现在可以看到刚才建立的项目。

![](https://pic002.cnblogs.com/images/2011/361355/2011121613253957.png)

4.右键项目文件夹，点击“添加新文件”，在这里选择C++ ->C++源文件，或者选择  概要->文本文件，只是在下面命名的时候加上后缀.cpp就可以了。文件　　　　名不一定要写成main.cpp，任何名字都可以。　　

![](https://pic002.cnblogs.com/images/2011/361355/2011121613342795.png)



5 编辑源代码，点一下左下角的第一个绿色三角形按钮就可以了。



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 #include <QApplication>
 2 #include <QLabel>
 3 
 4 int main(int argc, char *argv[])
 5 {
 6     QApplication app(argc, argv);
 7     QLabel hello("<h2><i>Hello</i><font color=red> Qt!</font></h2>");
 8    hello.show();
 9     return app.exec();
10 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

6 程序运行结果如下：　　

![](https://pic002.cnblogs.com/images/2011/361355/2011121613504725.png)

代码中第七行中也可以不用UML代码，直接



```
QLabel hello("Hello Qt!");








#### [](http://www.cnblogs.com/aleny-liu/archive/2011/12/16/aleny-Qtnote1.html#2765475)


 


我安装了之后没有Qt creater这个选项啊，只有其他几个


我的也没有Qt creator，
sudo apt-get install qtcreator 就好了，
然后再配置下就好。
```









