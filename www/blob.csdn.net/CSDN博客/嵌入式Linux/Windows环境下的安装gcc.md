
# Windows环境下的安装gcc - 嵌入式Linux - CSDN博客

2018年11月03日 23:05:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：92


Windows具有良好的界面和丰富的工具，所以目前linux开发的流程是，windows下完成编码工作，linux上实现编译工作。
为了提高工作效率，有必要在windows环境下搭建一套gcc,gdb,make环境。
MinGW就是windows下gcc的版本。
下载地址[http://sourceforge.net/projects/mingw/files/MinGW/](http://link.zhihu.com/?target=http%3A//sourceforge.net/projects/mingw/files/MinGW/)
进入网址后点击下载mingw-get-setup.exe安装包。
**MinGW-Installation-Manager运行界面如下：**
![](https://pic3.zhimg.com/80/v2-7b3c922cd952cee96a9c7e53ac994326_hd.jpg)

在MinGW-Installation-Manager中选择gcc，gdb，make相关软件包即可。

要正常使用MinGW，还需要设置环境变量。
桌面－》鼠标右键单击我的电脑－》属性－》高级－》环境变量
–将C:\MinGW\bin加入PATH
–将C:\MinGW\include加入INCLUDE
–将C:\MinGW\lib加入LIB

**打开CMD在命令提示符下输入gcc –v,看到gcc版本信息，gcc安装OK**
**打开CMD在命令提示符下输入gdb –v,看到gdb版本信息，gdb安装OK**
**打开CMD在命令提示符下输入make –v,看到make版本信息，make安装OK**
觉得有帮助，请关注微信公众号**【嵌入式Linux】**

