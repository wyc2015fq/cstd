# MinGW安装和使用基础教程 - DoubleLi - 博客园






MinGW全称Minimalist GNU For Windows，是个精简的Windows平台C/C++、ADA及Fortran编译器，相比Cygwin而言，体积要小很多，[使用](http://www.zhixing123.cn/computer/)较为方便。MinGW提供了一套完整的开源编译工具集，以适合Windows平台应用开发，且不依赖任何第三方C运行时库。

MinGW包括：
- 一套集成编译器，包括C、C++、ADA语言和Fortran语言编译器
- 用于生成Windows二进制文件的GNU工具的（编译器、链接器和档案管理器）
- 用于Windows平台安装和部署MinGW和MSYS的命令行安装器（mingw-get）
- 用于命令行安装器的GUI打包器（mingw-get-inst）

注:本文测试环境为Windows 7 SP1+MinGW 20120426。

## 一、MinGW安装

MinGW官方网址：http://www.mingw.org/

下载地址：http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/

下载后只是个安装器，需要在线下载真正的安装内容，所以速度不会快。具体步骤如下：

### 1、选择安装版本（Repository Catalogues）

有两个选择，一个是当前打包版本，另一个是下载最新版本。只要你的安装器是从官方下载的，选择前者即可，结果一般就是最新版本。

### 2、设置安装目录

默认安装到C:\MinGW

### 3、选择安装组件

这个根据大家需要选择安装组件（C编译器、C++编译器、Fortran编译器、ObjC编译器、Ada编译器等），一般选择C/C++编译器即可，看各位用途确定，组件列表中还有MSYS小系统等可供选择。

### 4、等待下载并安装完成

### 5、设置MinGW环境变量

依次鼠标点击桌面“我的电脑”->选择左侧的“高级系统设置”，选择“高级”->“环境变量”，然后再Path里增加;C:\MinGW\bin声明。点击确定。

小提示：设置完成后，Windows似乎不会自动更新环境变量，除非重启机子，那么我们可以通过在下面的命令行中，胡乱设置一下PATH（比如SET PATH=C:\），然后退出命令行，那么系统环境变量就会被强制刷新。

### 6、查看GCC版本信息

开始运行，输入CMD，回车并运行，输入gcc -v查看编译器版本。如下图：

![](http://www.zhixing123.cn/uploads/allimg/140827/2144202263-0.jpg)

## 二、MinGW使用

### 1、测试编译C++

测试文件test.cpp，代码如下（下面为C代码，只是使用G++来编译而已）
#include <stdio.h>

int main() {
	printf("Hello World!");
        return 0;
}
### 2、编译test

编译命令如下：

g++ test.cpp -o test

输出结果为：test.exe

### 3、执行

输入test回车，结果如下：

![](http://www.zhixing123.cn/uploads/allimg/140827/2144205631-1.jpg)









