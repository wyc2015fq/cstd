# 用Qt Creator编写代码+ vs2010编译调试的配置 - L_Andy的专栏 - CSDN博客

2015年10月20日 20:51:45[卡哥](https://me.csdn.net/L_Andy)阅读数：2716



1.我在的系统是win7 64位的操作系统 2G内存

2.已安装了vs2010旗舰版

3.顺序安装 后面的是我的安装路径

                 1. qt-win-opensource-4.8.2-vs2010.exe         E:\work\QT482_Creator250_add1_1_11\qt4.8.2   //注意不是源码。

                 2. qt-vs-addin-1.1.11-opensource.exe            E:\work\QT482_Creator250_add1_1_11\qtvsadd//他是可以自己找到vs2010 在vs2010菜单栏新增一“Qt”

                 3. qt-creator-win-opensource-2.5.0.exe          E:\work\QT482_Creator250_add1_1_11\QtCreator

4.启动QtCreator

                 Tools->Options->Qt Versions->右侧add->qmake location：E:\work\QT482_Creator250_add1_1_11\qt4.8.2\bin\qmake.exe（这个是我的路径）

这样就配好了Qt Creator的IDE

5.file->new project ->左侧选Applications->Qt Gui Application->Name+Create in(名字和路径)->选择Desktop ->next->next->finish这样就创建了一个简单的窗口

6.左下角的构建-> 绿色按钮运行---------------我们就看见了一个mainWindow的窗口

7.在project工程里有个名字Forms的文件夹->扩展名 .ui  打开我们就可以在这上面拖拽我们的控件了，之后点击运行，我们就可以看见控件在我们窗口上显示了

8.我们可以再添加自己的 *.cpp  *.h 文件， 右键黑色字体Add new 选择要添加的是 *.cpp *.h  选择一样之后输入名字，之后工程他会自动按照你选的分到相应的文件夹。

9.用vs2010的菜单栏的  “Qt " Open project file 直接打开你以上QtCreator所建立的.pro工程，这样你就可以在Qt Creator里面写你的代码，拿到vs2010里进行调试设断点。。。over 

