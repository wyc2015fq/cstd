
# linux下安装boost - 高科的专栏 - CSDN博客

2013年03月28日 14:13:21[高科](https://me.csdn.net/pbymw8iwm)阅读数：730个人分类：[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)[
							](https://blog.csdn.net/pbymw8iwm/article/category/910215)


以下是在ubuntu 7.10 (内核 2.6.22-14)下安装的例子：
一、下载最新的 boost 库，下载地址： http://www.boost.org/users/download/
二、在适当的位置解压 boost 库，推荐把 boost 库解压到 /usr/local/ 下：
$ cd dowload/ \# 这个地方替换成你的boost库下载目录
$ tar xvjf boost_1_35_0.tar.bz2 \# 如果是.tar.gz 的话，用 tar xvzf boost_1_35_0.tar.gz
$ sudo mv boost_1_35_0 /usr/local/
三、安装 boost，boost 库的安装应该分为两部分，第一部分是安装无需编译（HEAD ONLY）的boost库，第二部分是安装需要单独编译（separately-compiled）的boost库。
这里先写如何安装HEAD ONLY库：
其实就是给boost的头文件的目录加一个名字连接到系统目录里：
$ cd /usr/include $ sudo ln -s /usr/local/boost_1_35_0/boost boost
HEAD ONLY 库安装完成。
\#sudo ./bootstrap.sh
编译成功
**4.编译boost**
\#sudo ./bjam
开始编译
大约要个十几分钟
编译完成后：
The Boost C++ Libraries were successfully built!
The following directory should be added to compiler include paths:
/home/terry/Local/boost_1_47_0
The following directory should be added to linker library paths:
/home/terry/Local/boost_1_47_0/stage/lib
因为我是解压到/home/terry/Local/boost_1_46_1下了
所以编译完了是这样的
5.update动态链接库：
sudo ldconfig
安装完毕

## 三,测试使用:
**1.测试代码**
\#cat test.cpp
\#include <boost/lexical_cast.hpp>
\#include <iostream>
int main()
{
using boost::lexical_cast;
int a= lexical_cast<int>("123456");
double b = lexical_cast<double>("123.456");
std::cout << a << std::endl;
std::cout << b << std::endl;
return 0;
}
**2.编译，运行**
--g++ -o test test.cpp
\#ls
test  test.cpp
\# ./test
123456
123.456

