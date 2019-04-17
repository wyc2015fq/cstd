# 【Linux】JDK+Eclipse 搭建C/C++开发环境 - DoubleLi - 博客园






注：本文所提供的参考示例是在CentOS Linux环境下的安装，不保证适用于其他版本的Linux系统。
- **·  ****安装前的注意事项**

编译源代码是需要对应的代码编译工具的，本文中安装的Eclipse只是一个编写代码的外壳，不能对代码进行编译工作，所以你的Linux系统应当安装了编译器，这样Eclipse在编译源代码的时候才可以调用到对应的编译器；小编安装了的GCC，也就是我的Eclipse使用g++/gcc 进行代码的编译工作。如果你还没安装GCC的话请参见：

[【Linux】Linux下GCC的安装与编译C/C++代码示例](http://www.youranshare.com/blog/sid/24.html)
- **·    ****下载需要的文件**

1、  下载你需要的**JDK**，你可以下载rpm安装包或者是gz压缩包，

具体可以到Oracle官网去下载：[JDK Oracle官网下载地址](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)



2、  下载**Linux****版本**的Eclipse IDE For C/C++ ，这个你下载后是一个压缩包：

[Eclipse Oracle官网下载地址](http://www.eclipse.org/downloads/?osType=linux&release=undefined)



对于Eclipse的安装总的来说类似于Windows，首先设置好JDK，配置一下环境变量神马的，Eclipse就可以用了。
- **·    ****第一步安装JDK以及配置环境变量PATH**

1、  将下载的JDK压缩包解内的**JDK****文件夹**解压到你想要的位置，例如小编解压到了

/usr/java/ 这个目录下面，那么我的JDK的位置就是’/usr/java/jdk1.7.0_51’

2、  配置系统环境变量：

**方法一:**

使用带有root权限的账户进行登录，小编使用的是root账户登录的可以修改任意文件，找到文件 ‘/etc/profile’ 右键修改属性，将Permission中的权限修改为read-write,如图：



打开这个Profile 文件，然后在 如图的export位置再添加上你的JDK位置信息:

export JAVA_HOME=/usr/java/jdk1.7.0_51

export JAVA_BIN=/usr/java/jdk1.7.0_51/bin

export PATH=$PATH:$JAVA_HOME/bin

export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar

export JAVA_HOME JAVA_BIN PATH CLASSPATH





你只需要修改前两行就行了，然后关闭并保存这个profile文档，重启一下使得PATH生效；在终端输入 java  -version 可以查看Java的版本信息，如图：



**方法二：**

       在终端使用命令对 ‘/etc/profile’ 文件进行修改:

①   开终端，使用su命令输入密码获取root管理权限

②   使用命令在终端打开 profile这个文件:

# vi /etc/profile

③   按下回车进入文件的修改，这里显示的 代码与 方法一中的完全一样，将光标滚动到修改的位置(见方法一)，按下 键盘上的i按键，开始进行insert操作(终端左下角会显示)，将环境变量的配置输入到这个位置后，按下ESC按键,然后按下 “:”这个按键，输入exit，就可以退出保存了如图:



和方法一相同，最好是重启一下吧，然后使用命令java –version 查看JAVA版本信息。
- **·    ****第二步安装Eclipse IDE For C/C++**

因为JDK环境变量都整好了，这一步就很简单了，打开你下载的Eclipse压缩包，随便解压到你想要的位置就好了，在Eclipse文件夹下面有一个eclipse文件，这就是启动文件，双击即可启动Eclipse了。

创建一个桌面快捷方式:

在桌面右键创建一个 Launcher，将command指定为启动图标的路径就好了。


- **·  ****C++ Hello Word ****测试**











