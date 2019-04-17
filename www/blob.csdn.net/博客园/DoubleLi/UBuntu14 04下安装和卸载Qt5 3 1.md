# UBuntu14.04下安装和卸载Qt5.3.1 - DoubleLi - 博客园






**安装：**

1. Qt5.3.1下载地址为：[http://qt-project.org/](http://qt-project.org/)，选择”Qt 5.3.1 for Linux 32-bit”版本，文件名是”qt-opensource-linux-x86-5.3.1.run”；

2. 进入qt-opensource-linux-x86-5.3.1.run目录下，修改文件权限：chmod u+x qt-opensource-linux-x86-5.3.1.run；

3. 打开安装界面： ./qt-opensource-linux-x86-5.3.1.run；

4.图形化界面安装；

5.select all（选择全部）；

6.选择LGPL协议，安装即可；

7.启动Qt Creater：进入Qt5.3.1/Tools/QtCreater/bin/，可以鼠标双击qtcreater启动；

8.如果你的linux中已经安装了opengl库，则不需要以下命令，否则运行以下命令安装opengl库：sudo apt-get install freeglut3-dev；

9.建立Qt5.3.1桌面快捷方式（主要注意解释的三行代码即可）：

  （1）在桌面上建立Qt5.3.1.desktop文件，将以下代码复制进去：
[Desktop Entry]
Categories=Development;
Comment[zh_CN]=
Comment=
Exec=/home/warsllon/SoftWare/Qt5.3.1/Tools/QtCreater/bin/qtcreator  解释：这里放的是qtcreator的具体文件路径
GenericName[zh_CN]=IDE
GenericName=IDE
Icon=/home/warsllon/Pictures/Qt.png  解释：这里放的是桌面快捷方式的图标路径
MimeType=
Name[zh_CN]=Qt5.3.1   解释：这里放的是桌面快捷方式的名称
Name=Qt5.3.1 
Path=
StartupNotify=true
Terminal=false
Type=Application
X-DBUS-ServiceName=
X-DBUS-StartupType=
X-KDE-SubstituteUID=false
X-KDE-Username=warsllon

（2）进入Qt5.3.1.desktop文件的目录中：cd /home/warsllon/Desktop
（3）赋予该文件权限：chmod +x Qt5.3.1.desktop
（4）桌面快捷方式创建完毕。
**卸载：**

1.进入Qt5.3.1的安装目录：例如我的目录：cd /home/warsllon/SoftWare/Qt5.3.1；

2.运行命令：./MaintenanceTool；

3.进入图形化界面，按照操作卸载即可。

------------------------------------------------------------------------------------------

在ubuntu下使用Qt 编译时候遇上了cannot find -lGL错误,使用命令


sudo apt-get install libqt4-dev或者sudo apt-get install libgl1-mesa-dev或者libgl1-mesa-dev或者libglu1-mesa-dev




可以解决问题 



之前可以先apt-get install update一下









