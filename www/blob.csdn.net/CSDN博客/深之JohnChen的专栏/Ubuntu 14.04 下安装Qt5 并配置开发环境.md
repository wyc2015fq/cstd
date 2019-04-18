# Ubuntu 14.04 下安装Qt5 并配置开发环境 - 深之JohnChen的专栏 - CSDN博客

2019年03月28日 23:24:39[byxdaz](https://me.csdn.net/byxdaz)阅读数：121


**转载地址：http://blog.csdn.net/cdaima/article/details/40917823**

**Ubuntu14.04 下安装Qt5 并配置开发环境**

         近一周出于实训的需要，我开始在Linux环境下学习使用Qt。一开始Qt环境的搭建就碰了壁，因为所用软件工具都是最新版的，而且网上的一些配置方法都不适合我这个环境。通过两天的琢磨和看相关博客，我完成了预期的目的，能够在Linux下使用Qt，也能在终端里用qmake和make去生成可执行文件。此过程中查看的博客和资料都落下了很多细节问题，我在这就整理下方便后面的同仁学习。

一、我所使用的软件

使用Ubuntu Kylin 14.04.1 长期支持版（2014年四月份发布，并可以免费下载了。下载链接：[http://www.ubuntu.com/download/ubuntu-kylin-zh-CN](http://www.ubuntu.com/download/ubuntu-kylin-zh-CN)，可以根据自己的需要下载相应版本），下载后的文件名：ubuntukylin-14.04.1-desktop-i386。

Qt5.3.2  for Linux32-bit（下载链接：[http://qt-project.org/downloads](http://qt-project.org/downloads)，根据自己的需要下载相应版本），下载后的文件名：qt-opensource-linux-x86-5.3.2.run。

二、相应步骤

1、  我首先在VMwareworkstation中安装好Ubuntu 系统，然后把下载后的qt run文件拖进Linux桌面。（若操作不允许，需要安装VMware tools，可以直接在VMware软件的菜单栏虚拟机的下拉菜单里安装就行），建议将qt 安装文件放到主目录的一个根目录（可自行建立QT文件夹）下。如下图所示：

![](https://img-blog.csdn.net/20141108090351000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2、  打开终端（快捷键ctrl+alt+t）                                              输入命令**chmod 777 >>>.run **(也可不写此命令，命令改变文件权限)                     **./>>>.run**  进行安装。

3、  安装很简单，一直点击next就行。       

![](https://img-blog.csdn.net/20141108090737828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108090735175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108090839656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108090755018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108091156272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108091258984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108091306890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4、  安装完后编一个简单程序测试：提示需要在Ubuntu 下安装一个编译器（终端下输入命令：  sudo apt-get installg++  安装g++编译器）。![](https://img-blog.csdn.net/20141108091242776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5、  安装完后，重新打开Qtcreator ,发现可以编译了，但是仍然有一个错误提示，这是因为机器没有安装OpenGL库，执行下面语句安装：

![](https://img-blog.csdn.net/20141108091256192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

6、  此时QT creator 就安装完成了，能够单独使用了，但在终端下还未配置好。在某些情况下，我们希望在终端（shell）下编写QT 程序，这就需要进行以下的配置操作。

![](https://img-blog.csdn.net/20141108091323008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

7、  在终端里输入 qmake,会提示这样的错误：这就需要用链接来改变 qmake的路径以完成配置。需要在终端输入命令：   cd >>>(qt安装的路径下的gcc文件夹下的bin文件中的qmake  如：cd  /home/username/QT5/5.3/gcc/bin)                              sudo  ln qmake /usr/lib/i386-linux-gnu/qt4/bin（链接）  

![](https://img-blog.csdn.net/20141108091357796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

8、  再次在终端输入命令qmake ，出现以下信息，表示配置成功。  

![](https://img-blog.csdn.net/20141108091518203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

9、  最后在终端里编一个小程序测试一下：     

![](https://img-blog.csdn.net/20141108091439729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141108091450946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ2RhaW1h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

到这里就完全配置好，恭喜你成功了！！！

# Qt 之 运行Qt Creator出现cannot find -lGL的问题的两种解决

由于 Qt5.0 以上的版本默认将OpenGL加入了工程，但是在机器上没有安装OpenGL，所以只需要在机器系统安装OpenGL即可。

有两种原因，一种是没有按照libGL库，那么就安装：
`sudo apt-get install libgl1-mesa-dev`

一种是装了，但是路径不对，那么就配置一下路径：

```
$ locate libGL
/usr/lib/i386-linux-gnu/mesa/libGL.so.1
/usr/lib/i386-linux-gnu/mesa/libGL.so.1.2.0
/usr/lib/x86_64-linux-gnu/libGLEW.so.1.10
/usr/lib/x86_64-linux-gnu/libGLEW.so.1.10.0
/usr/lib/x86_64-linux-gnu/libGLEWmx.so.1.10
/usr/lib/x86_64-linux-gnu/libGLEWmx.so.1.10.0
/usr/lib/x86_64-linux-gnu/libGLU.so.1
/usr/lib/x86_64-linux-gnu/libGLU.so.1.3.1
/usr/lib/x86_64-linux-gnu/mesa/libGL.so.1
/usr/lib/x86_64-linux-gnu/mesa/libGL.so.1.2.0
/usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2
/usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2.0.0
$ sudo ln -s /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1 /usr/lib/libGL.so
```

