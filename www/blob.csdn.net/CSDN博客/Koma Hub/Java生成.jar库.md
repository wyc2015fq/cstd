# Java生成.jar库 - Koma Hub - CSDN博客
2018年01月19日 15:43:34[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：203
首先准备好编好的java程序，编译生成.class文件（记住说有需要加入.jar的路径和名称）。
编译：
`-bash-4.1$ javac Demo_00_MyWindow.java `
生成对应的一些列.class文件。
建立编辑一个文本文件（名称自拟，我的名称是Mainfest.txt）,内容如下：
Main-class: +（你生成的包含main方法的class名称）
我的内容：
```
-bash-4.1$ cat Mainfest.txt
Main-class: Demo
-bash-4.1$
```
然后就可以用java的jar打包class和库了，这里我的程序包含了应用图片，所以把需要的图片文件也打包进jar里：
```
-bash-4.1$ jar -cvfm Demo.jar Mainfest.txt *.class picture
已添加清单
正在添加: Demo.class(输入 = 556) (输出 = 399)(压缩了 28%)
正在添加: Lagan.class(输入 = 2712) (输出 = 1356)(压缩了 50%)
正在添加: myAuthorAboutDialog$1.class(输入 = 880) (输出 = 529)(压缩了 39%)
正在添加: myAuthorAboutDialog$2.class(输入 = 880) (输出 = 528)(压缩了 40%)
正在添加: myAuthorAboutDialog$3.class(输入 = 540) (输出 = 368)(压缩了 31%)
正在添加: myAuthorAboutDialog.class(输入 = 3768) (输出 = 2190)(压缩了 41%)
正在添加: myJFrame$BackgroundPanel.class(输入 = 744) (输出 = 498)(压缩了 33%)
..............（这里输出了很多，不全部列举）
正在添加: picture/ButtonImg/tcurves32.png(输入 = 1904) (输出 = 1909)(压缩了 0%)
正在添加: picture/ButtonImg/timage32.png(输入 = 377) (输出 = 372)(压缩了 1%)
正在添加: picture/ButtonImg/tmovie32.png(输入 = 1598) (输出 = 1603)(压缩了 0%)
正在添加: picture/ButtonImg/traypath32.png(输入 = 1887) (输出 = 1892)(压缩了 0%)
正在添加: picture/ButtonImg/traytracing32.png(输入 = 2114) (输出 = 2119)(压缩了 0%)
-bash-4.1$
```
然后看看生成了什么：
```
-bash-4.1$  ls
Demo.jar
-bash-4.1$
```
把这个Demo.jar文件搞到安装了JDK的windows环境下，显示如下：
![](https://img-blog.csdn.net/20180119154249570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后双击这个文件就可以运行你的程序了，我的是个SwingGUI的程序，所以直接跳出一个窗口：
![](https://img-blog.csdn.net/20180119154259748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来可能会更新如何将native方法打包进jar文件里并运行（gcc和nvcc），但愿顺利。。。。。
