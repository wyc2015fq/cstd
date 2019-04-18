# Linux操作系统Ubuntu部署GCC篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月10日 11:28:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2091








### GCC编译器部署

在线联网部署方式：

1．启动虚拟机，进入Ubuntu操作系统；

2．打开终端，“应用程序—附件—终端”，进入命令窗口；

3．执行下面命令完成GCC编译和基本库的部署：

1）刷新软件包列表：sudo apt-get update

2）安装基本开发环境：sudo apt-get install build-essential gcc make

3）安装编辑器：sudo apt-get install vim emacs

4）安装基本开发库：sudo apt-get install libc6 libc6-dev

5）安装manpage（可用printf）：sudo apt-get install manpages-dev

4．编译执行简单C文件通过； 

说明：安装GCC就包含G++，即可以编译C++的CPP文件。



