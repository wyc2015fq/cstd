# 如何使用eclipse进行嵌入式Linux的开发 - DoubleLi - 博客园






**如何使用eclipse进行嵌入式Linux的开发**

**作者：**曾宏安，[华清远见嵌入式学院](http://www.embedu.org/)高级讲师。

如何使用eclipse进行嵌入式Linux的开发 习惯了在windows环境下开发的程序员在转到Linux平台时经常会抱怨没有一个好用的集成开发环境。和windows下常用的一些开发软件相比，Linux自带的一些开发环境使用起来要么界面和操作不够方便、友好，要么功能不全。其实我们完全可以在开源社区上找到操作方便，功能强大的开发环境。下面就向大家介绍一个能在Linux上运行，界面和功能与windows上同类软件相似的开发平台eclipse。

eclipse是著名的跨平台的集成开发环境(IDE)，最初是由IBM公司开发的替代商业软件Visual Age For Jave的下一代开发环境。2001年，IBM将eclipse贡献给开源社区，现在由eclipse基金会管理。2005年7月，eclipse稳定版3.1.0发布，目前最新的稳定版本是3.4。

eclipse本身只是一个框架平台，最初主要用于java语言的开发。但是众多插件的支持使得eclipse同样可以用来开发其他的语言，如c/c++、c#、perl、cobol的等等。cdt(c/c++ development toolkit)就是支持c/c++开发的插件。许多软件开发商也以eclipse为框架推出了自己的集成开发环境。由于eclipse平台用java实现，所以运行时需要jre(java runtime environment)的支持。下面我们以ubuntu(linux发行版的一种)为例，介绍如何在linux上使用 eclipse+cdt来进行嵌入式Linux应用程序的开发。

**一、安装软件**

在ubuntu下安装软件有两种方法:使用apt-get来自动安装或手动安装。用户使用自动安装非常方便，适合初学者。但是安装过程过于依赖资源网站的连接状况，可能需要耗费过多的时间。这里我们推荐手动安装的方式，花的时间比较短。

**1、安装jdk**

jdk(java development kit)即java开发包，其中包含了运行eclipse所需要的jre. ubuntu缺省安装的gnu版本的jre运行效率不高，所以我们需要安装sun公司的jre. 首先从sun的网站上(http://java.sun.com/javase/downloads/index.jsp)下载安装包jdk-6u7-linux-i586.bin，然后如下进行安装：

# mkdir -p /opt/java 
        # ./jdk-6u7-linux-i586.bin // 执行自解压文件，生成目录
        # mv jdk1.6.0_07 /opt/java // 把目录移到/opt/java下
        # cd /opt/java
        # ln -s jdk1.6.0_07 jdk 
        # mv /etc/alternatives/java /etc/alternatives/java.gnu
        # ln -s /opt/java/jdk/bin/java /etc/alternatives/java
        # export JAVA_HOME=/opt/java/jdk // 设置环境变量

**2、安装eclipse和cdt**

从eclipse网站上(http://www.eclipse.org/)下载安装包eclipse-SDK-3.4-linux-gtk.tar.gz 和cdt-master-5.0.0.zip

# cd /opt
        # tar zxvf eclipse-SDK-3.4-linux-gtk.tar.gz // 解压生成eclipse目录
        # export PATH=/opt/eclipse:$PATH
        # mkdir -p /opt/cdt
        # cd /opt/cdt
        # unzip cdt-master-5.0.0.zip
        # cp -r plugins/* /opt/eclipse/plugins/
        # cp -r features/* /opt/eclipse/features/

**二、创建c/c++工程**

假设我们已经在linux上安装好了arm交叉工具链，下面介绍如何创建、交叉编译和调试c/c++工程。为了更直观的说明，我们截取了相关的图片：

进入eclipse安装路径后，运行eclipse

![eclipse运行界面](http://www.embedu.org/Column/images/Column26-1.jpg)

eclipse运行界面

![](http://www.embedu.org/Column/images/Column26-2.jpg)

创建c 工程

![](http://www.embedu.org/Column/images/Column26-3.jpg)

使用缺省的Linux GCC工具链

![](http://www.embedu.org/Column/images/Column26-4.jpg)

进入c/c++ 界面

![](http://www.embedu.org/Column/images/Column26-5.jpg)

添加源代码文件1

![](http://www.embedu.org/Column/images/Column26-6.jpg)

添加源代码文件2

![](http://www.embedu.org/Column/images/Column26-7.jpg)

编辑代码

![](http://www.embedu.org/Column/images/Column26-8.jpg)

设置工程属性，指定交叉工具链1

![](http://www.embedu.org/Column/images/Column26-9.jpg)

设置工程属性，指定交叉工具链2

![](http://www.embedu.org/Column/images/Column26-10.jpg)

设置工程属性，指定交叉工具链3

![](http://www.embedu.org/Column/images/Column26-11.jpg)

编译工程

![](http://www.embedu.org/Column/images/Column26-12.jpg)

已生成arm平台可执行代码

![](http://www.embedu.org/Column/images/Column26-13.jpg)

配置调试选项

![](http://www.embedu.org/Column/images/Column26-14.jpg)

设置Debugger和 GDB debugger选项

![](http://www.embedu.org/Column/images/Column26-15.jpg)

设置连接类型为TCP，目标板的ip地址和调试端口号。端口号任意指定，通常大于1024(避免和系统所用端口号冲突)

![](http://www.embedu.org/Column/images/Column26-16.jpg)

在开发板上运行程序，格式如上图所示：gdbserver是交叉调试辅助程序；192.168.0.150是主机的ip地址；1234是调试端口号，和eclipse调试选项里设定的端口号要一致

![](http://www.embedu.org/Column/images/Column26-17.jpg)

点击Debug，进入调试界面

![](http://www.embedu.org/Column/images/Column26-18.jpg)

执行到断点

eclipse的调试界面和操作和在windows下开发类似，相信到了这一步，大家已经可以很方便来进行应用程序的开发了。

















所需开发工具及环境
- 虚拟机Linux：Fedora 9
- 交叉编译工具：arm-linux-gcc-3.4.1.tar.bz2
- 集成开发工具：Eclipse（Linux安装包中自带的，安装Linux时选中安装）

安装ARM交叉编译器

    1. 安装。其实很简单，就是将下载的arm-linux-gcc-3.4.1.tar.bz2解压到Linux根目录即可（因为解压后已经包含了/usr/local目录，所以要解压到根目录），如下命令：
|`tar -jxvf arm-linux-gcc-3.4.1.tar.bz2 -C /`|
|----|

解压后的目录如下图：

![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018320.png)

解压后的目录/usr/local/arm/3.4.1/

    2. 设置环境变量。就是把交叉编译器的路径设置到系统环境变量里面去。
|`echo $PATH`|
|----|

查看现有的环境变量，显示如下图：

![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018321.png)

如果你只想临时添加交叉编译器的环境变量（注：临时添加的系统重启或注销后又没有了），只需在命令行输入如下命令：
|`export PATH=$PATH:/usr/local/arm/3.4.1/bin`|
|----|

添加后的环境变量如下图：（多了一个/usr/local/arm/3.4.1/bin）

![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018322.png)

如果你想这个环境变量永久的添加到系统的环境变量中，则你只需把这个命令添加到/root/.bashrc文件的最后一行即可（这个文件是隐藏文件，你要在文件浏览器中单击查看菜单，勾选显示隐藏文件才能看到）

    3. 测试建立的交叉编译环境。新建一个c程序，文件名:test.c， 如下：


|```#include <stdio.h>int main(void){    printf("Hellow world!\n");    return 0;}```|
|----|

进行交叉编译，如下：
|`arm-linux-gcc -o test test.c`|
|----|

将交叉编译好的目标文件test下载到ARM开发板中并运行，如下：
|`./test`|
|----|



输出结果为：

Hellow world!

用集成开发工具Eclipse来开发嵌入式应用程序

![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018323.jpg)

双击Eclipse图标启动Eclipse



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018324.jpg)

设置Eclipse的工作空间，就是选个目录，单击OK



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018325.jpg)

启动后的主界面



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018326.jpg)

开始新建一个应用程序工程



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018327.jpg)

选择新建一个C工程，单击Next



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018328.jpg)

给工程取个名字，这里我就叫test_app；然后选择工程类型，我选择了个Hellow world C工程模版，单击Next



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/1018329.jpg)

填写作者、版权等信息，单击Next



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183210.jpg)

没什么选的，点击Finish



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183211.jpg)

单击Yes



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183212.jpg)

新建好的项目工程



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183213.jpg)

展开左边的源代码目录，双击打开主函数。这里只是输出一句"Hello World!!!"，你可以做一些其他复杂的应用



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183214.jpg)

单击Project菜单，去掉Build Automatically的勾选，因为勾选的话，每次修改一个地方工程都会自动进行编译



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183215.jpg)

打开工程的属性对话框进行编译选项的设置



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183216.jpg)

我们的应用程序最终要到嵌入式arm上运行，这里就要把gcc改成arm-linux-gcc编译器，当然前提是你要在你的开发主机上先安装arm-linux-gcc



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183217.jpg)

C连接器也要改成arm-linux-gcc



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183218.jpg)

这里的Libraries是设置你的应用程序中用到的库文件，比如应用程序中用到了线程类，则这里在上面添加pthread这个库，下面就添加这个库的路径。如果应用程序没用到任何库文件就不用管了



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183219.jpg)

选中左侧的工程名，右键，在弹出的菜单中选择Build Project编译工程



![](http://www.embeddedlinux.org.cn/uploads/allimg/130316/10183220.jpg)

编译好后的工程，目标文件在Debug目录下

结束语

    建立交叉编译环境是开发嵌入式程序的第一步，编译器版本也有很多，这里就介绍用的较多的一种。而Eclipse则是本人认为开发嵌入式应用程序最为方便的集成开发工具，他不仅让整个工程清晰可见，更重要的是他不需要你去为整个工程编写复杂的Makefile文件，他会为你自动生成Makefile。















## ubuntu上安装arm交叉编译环境arm-linux-gcc


   首先下载 arm-linux-gcc-4.3.2.tgz，这个就不多说了，重点介绍一下怎么安装。将 arm-linux-gcc-4.3.2.tgz 文件拷贝到 Ubuntu 的主文件夹下,在 Ubuntu 中新建一个终端,以root用户运行，输入下面的命令安装交叉编译器:

#tar zxvf arm-linux-gcc-4.3.2.tgz
解压后会产usr文件夹，将这个目录下的usr/local/arm的arm文件夹复制到文件系统的usr/local目录下，命令为
#mv usr/local/arm  usr/local
然后把交叉编译器路径添加到系统环境变量中,

在终端中执行:
gedit /etc/profile
添加以下四行到该文件中:
export PATH=/usr/local/arm/4.3.2/bin:$PATH
export TOOLCHAIN=/usr/local/arm/4.3.2
export TB_CC_PREFIX=arm-linux-
export PKG_CONFIG_PREFIX=$TOOLCHAIN/arm-none-linux-gnueabi













