# Linux上安装使用boost入门指导 - Orisun - 博客园







# [Linux上安装使用boost入门指导](https://www.cnblogs.com/zhangchaoyang/articles/2048952.html)




- 获得boost
- boost分布
- 只需要头文件的库
- 使用boost建立一个简单的程序
- 准备使用boost二进制文件库
- 把你的程序链接到boost库

1.获得boost

[下载boost_1_46_1.tar.bz2](http://sourceforge.net/projects/boost/files/boost/1.46.1/boost_1_46_1.tar.bz2/download)

解压

2.boost分布

boost_1_46_1.........................boost根目录

　　boost/.....................................所有boost头文件

　　libs/........................................Tests,.cpps,docs等的库文件

注意：

　　（1）boost根目录（通常是/usr/local/boost_1_46_1）想到$BOOST_ROOT变量中

　　（2）编译程序时如果用到boost库，需要指定头文件路径-I$BOOST_ROOT

　　（3）因为所有头文件都在boost文件夹下，并且头文件都是hpp后缀，所#include形如：

　　　　#include <boost/whaever.hpp>

3.只需要头文件的库

　　绝大多数的boost库都是header-noly的：它们完全由包含模板和inline函数的头文件组成，不需要单独编译和二进制库文件，也不需要链接时特别对待。

　　只有下面的boost库必需单独built:
- Boost.Filesystem
- Boost.GraphParallel
- Boost.IOStreams
- Boost.MPI
- Boost.ProgramOptions
- Boost.Python (see the Boost.Python build documentation before building and installing it)
- Boost.Regex
- Boost.Serialization
- Boost.Signals
- Boost.System
- Boost.Thread
- Boost.Wave

　　下面这些单独built是可选（optional）的：　

　　Boost.DateTime　　Boost.Graph　　Boost.Math　　Boost.Random　　Boost.Test

4.使用boost建立简单的程序

　　下面的程序(example.cc)只用到header-only库。它是从标准输入中读入一串整数，使用Boost.Lambda每个数乘以3后输出。

#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}


编译：g++  -I$BOOST_ROOT  example.cc  -o  example

运行：echo 1 2 3 | ./example

5.准备使用boost二进制库

　　如果你的程序用到需要单独编译的boost库，你需要首先获得这些二进制库文件。

5.1编译安装所有二进制库文件

　　cd $BOOST_ROOT

　　./bootstrap.sh  --help

　　./bootstrap.sh  --prefix=/usr/local　　　　##其实默认情况下prefix的值就是/usr/local

　　此时生成了bjam可执行文件，这个东西就是用来编译boost库的。

　　./bjam  install

5.2仅安装指定的二进制库文件

　　下面均使用系统默认的编译器，即Linux上的gcc。

　　5.2.1安装Boost.build

　　Boost.Build是一个用于开发、测试、安装软件的基于文本的系统。Boost.Build的生成安装步骤：

　　（1）cd $BOOST_ROOT/tools/build/v2

　　（2）./bootstrap.sh

　　（3）./bjam  install  --prefix=/usr/local/　　　　　　##prefix是Boost.Build安装位置

　　（4）把prefix/bin放到PATH中　　　　　　　　　　##当然/usr/local/bin已经PATH中了

　　5.2.2调用bjam时不指定toolset则使用系统默认的编译器。如果你的Linux上装了不同版本的gcc，则使用toolset选项时可以指定版本号：toolset=gcc-4.4

　　5.2.3指定build路径，通过--build-dir=/path选项，不指定时默认在当前路径下创建bin.v2文件夹，把生成的文件放在其内。

　　5.2.4调用bjam

　　cd  $BOOST_ROOT

　　bjam  --build-dir=./build-boost  toolset=gcc   stage

　　上面的命令将创建static and shared non-debug multi-threaded variants of the libraries.如果要建立所有的variants，请使用"--build-type=complete"选项。

　　所有的boost二进制库文件将放在stage/lib/下，如果你要另外指定路径，请使用“--stagedir=directory"选项。

　　注意为节省build时间，你可能需要少build一些库文件：
- 查看库文件名称　　--show-libraries
- 限制build哪些库　　--with-libraryname或者--without-libraryname
- 选择特定的build variant　　adding release or debug to the command line

　　友情提示：Boost.Build会生成很多报告输出，如果你能保证建立过程不出错误，你可以禁止这些输出以节省时间。方法：在命令后追加”>build.log  2>&1"

6.把你的程序链接到boost库

　　下面的程序是从邮件中抽取“主题”内容，它用到了Boost.Regex库，这个库是需要单独编译的。

#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main()
{
    std::string line;
    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );
　　　　
    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }
}


编译：

方法A：g++  -I$BOOST_ROOT  example.cc -o example  $BOOST_ROOT/stage/lib/ -lboost_regex.a

方法B：g++  -I$BOOST_ROOT  example.cc -o example  -L$BOOST_ROOT/stage/lib/ -lboost_regex

当你要使用多个库源于一个路径时使用方法B就省力了（paid off）。注意到方法B中并没有指定.a（静态库）还中.so（动态库），系统将自动地帮你选择使用静态库还是动态库，当然你可以通过选项“-static"显示地指定。

6.1库文件的命名方式

拿libboost_regex-gcc34-mt-d-1_36来说：

lib....................................通用前缀

boost_regex.....................库名

gcc34...............................编译时使用的toolset是gcc-3.4

mt...................................编译时是支持多线程的

d/s/g/y/p..........................ABI tag

1_36................................Tag version

6.2运行我们的程序

　　首先新建一个文本文件mail.txt

To: George Shmidlap
From: Rita Marlowe
Subject: Will Success Spoil Rock Hunter?
---
See subject.


如果我们的程序链接到了一个共享动态库，我们需要让系统知道到哪儿去加载它。请看我的~/.bashrc文件：

#boost
export BOOST_ROOT="/usr/local/boost_1_46_1"
export LD_LIBRARY_PATH="/usr/local/boost_1_46_1/stage/lib:$LD_LIBRARY_PATH"


运行程序：./example  <  mail.txt

应该输出
Will Success Spoil Rock Hunter?











