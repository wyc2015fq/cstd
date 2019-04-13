
# ubuntu 12.04 eclipse 安装 - 嵌入式Linux - CSDN博客

2013年10月17日 09:48:55[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1194


**方法二**：(优点是安装内容清爽，缺点是配置麻烦)
1、安装JDK，参考[Ubuntu 12.04 下安装 JDK 7](http://hi.baidu.com/sanwer/blog/item/5e5ee922b44287844623e816.html)
2、下载 Eclipse
从[http://www.eclipse.org/downloads/index-developer.php](http://www.eclipse.org/downloads/index-developer.php)下载合适版本，如：Eclipse IDE for C/C++ Developers
3、解压文件
$ sudo mv eclipse-cpp-juno-M7-linux-gtk.tar.gz /usr/lib/
$ cd /usr/lib/
$ sudo tar -zxvf eclipse-cpp-juno-M7-linux-gtk.tar.gz
$ sudo rm ./eclipse-cpp-juno-M7-linux-gtk.tar.gz
4、创建启动快捷方式
$ sudo gedit /usr/share/applications/eclipse.desktop
内容如下：
[Desktop Entry]
Type=Application
Name=Eclipse
Comment=Eclipse Integrated Development Environment
Icon=/usr/lib/eclipse/icon.xpm
Exec=/usr/lib/eclipse/eclipse
Terminal=false
Categories=Development;IDE;Java;

Ubuntu 12.04使用了Unity界面，在桌面左侧侧多了一个启动器栏。我们可以将自己常用的软件在启动器栏放一个图标，实现快速地启动。启动器栏的每一个图标实际上都对应了/usr/share/applications目录下的一个文件。

为了给eclipse创建启动栏图标，我们可以在/usr/share/applications目录下创建文件eclipse.desktop（文件名可以任意，但是文件名后缀必须是.desktop）,再将下面的内容复制过来：
[Desktop Entry]
Type=Application
Name=Eclipse
Comment=Eclipse Integrated Development Environment
Icon=/usr/local/development/eclipse-indigo-3.7.2/icon.xpm
Exec=eclipse
Terminal=false
Categories=Development;IDE;Java;
![linux](http://www.linuxidc.com/linuxfile/logo.gif)

