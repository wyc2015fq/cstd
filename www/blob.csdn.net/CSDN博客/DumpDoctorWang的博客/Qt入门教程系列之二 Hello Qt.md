# Qt入门教程系列之二 Hello Qt - DumpDoctorWang的博客 - CSDN博客





2018年08月11日 14:21:59[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：351








# 一、Qt Creator

Qt Creator是一个IDE，类似于VS，理论上，也可以使用其他IDE来开发Qt程序。打开QtCreator之后，出现如下页面

![](https://img-blog.csdn.net/20180810215930618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中对“选项栏”中有三个按钮，分别为“Projects”、“示例”、“教程”；“Projects”对应着项目的管理，包含创建项目和打开项目。”示例“里面有非常的多的例子，可以研究研究。”教程“里面有一些简单的教程。

# **二、创建自己的第一个Qt项目**
- **点击“New Project”**

依次点击“Application”→“Qt Widgets Application”→“Choose...”。“Qt Widgets Application”是有UI的程序，“Widgets”暂且可以理解为UI。“Qt Console Application”是控制台应用，就是一个黑框框那种。“Qt Quick Application”是*Qt* 4.7 中推出的一项新技术。*Qt Quick* 是一种高级用户界面技术，使用它可轻松地创建供移动和嵌入式设备使用的动态触摸式界面和轻量级应用程序。

![](https://img-blog.csdn.net/20180811114334877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-  设置项目名字和路径

![](https://img-blog.csdn.net/20180811115627346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-  选择“Kit”

“Kit”翻译过来是构建套件，这里有三个可以选，选第三个就行。

Qt中的Compiler、Kit、和Qt Version的区别与联系，可参见[Qt creator的Options中Kits，Qt versions，Compilers之间的关系是什么？](https://bbs.csdn.net/topics/392261056)。

![](https://img-blog.csdn.net/20180811120333975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-  类信息

设置主窗口类的类名和对应的.ui文件名。基类除了QMainWindow以外，还有QWidget、QDialog可以选，三者的差别可参考[Qt窗口之QMainWindow、QDialog、QWidget](https://blog.csdn.net/rl529014/article/details/51419126)。

![](https://img-blog.csdn.net/20180811125950678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
-  版本控制

我安装了Git，所以有Git的选项，要是没安装，就不会有选项，建议安装一个Git。点击“完成”之后，可能需要稍等片刻，等待Qt的预处理完成。

![](https://img-blog.csdn.net/20180811131950229?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 三、运行项目

创建好项目之后，进入如下界面。绿色框为项目的目录结构；黄色框里面选的代码的编译模式(分别为Debug、Profile、Release)；红色框内的三个按钮从上到下依次分别为运行、调试和构建。

![](https://img-blog.csdn.net/20180811133119212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击“运行”，也就是红色框里的第一个绿色三角 ，就会开始编译和运行项目，稍等片刻，就会出现如下界面

![](https://img-blog.csdn.net/20180811133625471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对，就是一个空白的窗口，貌似有点单调，再加点东西吧。

# 四、添加第一个控件

项目目录截图如下

![](https://img-blog.csdn.net/20180811133913807?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Hello-Qt.pro为项目文件，类似于VS中的.sin文件；main.cpp是入口文件；mainwindow.h、mainwindow.cpp和mainwindow.ui是属于一个UI的文件，mainwindow.h包含了一个UI类的定义，mainwindow.cpp是实现，mainwindow.ui的本质是xml文件，里面包含了UI的布局和控件(按钮、编辑框之类的)。 

双击mainwindow.ui，进如下UI编辑界面，红色框是UI控件的搜索框；绿色框为控件框；黄色为UI界面；蓝色框里列出了UI中包含了哪些控件及层次关系；粉色框里是一个控件的属性表，每点击蓝色框里一个控件，粉色框的属性表会随之变化，可以自己试下。

![](https://img-blog.csdn.net/20180811140516202?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在搜索框中输入label ，出现如下界面

![](https://img-blog.csdn.net/201808111410418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

把Label拖动到黄色框里面的UI中去，然后双击刚才拖进去的label，会进入编辑模式，输入Hello Qt，然后按回车，完成编辑。

点击label时，粉色框的属性表会随之变成label的属性表。

![](https://img-blog.csdn.net/20180811141720860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后点击运行，就得到如下界面

![](https://img-blog.csdn.net/20180811141855977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 第一个项目到此结束。



