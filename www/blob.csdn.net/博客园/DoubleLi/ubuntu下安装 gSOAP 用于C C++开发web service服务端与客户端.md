# ubuntu下安装 gSOAP 用于C/C++开发web service服务端与客户端 - DoubleLi - 博客园







## 昨天在ubuntu下进行安装gSOAP，费了很多时间，没成功，今天又来找了大量教程资料，终于一次成功，这里写下自己的安装步骤和方法，供大家参考。



首先下载gsoap，我下载的是gsoap-2.8.1.zip

用unzip gsoap-2.8.1.zip命令解压缩，会解压生成gsoap-2.8文件夹。

cd gsoap-2.8



在安装之前需要先安装一些编译工具。





安装编译工具：

　　$sudo apt-get install build-essential

　　为了成功编译gSOAP，您需要安装GTK+的开发文件和GLib库(libraries)。

　　$sudo apt-get install libgtk2.0-dev libglib2.0-dev

　　安装Checkinstall以便管理您系统中直接由源代码编译安装的软件。

　　$sudo apt-get install checkinstall

       安装YACC,YACC是Unix/Linux上一个用来生成编译器的编译器（编译器代码生成器）。

       $sudo apt-get install flex bison

       安装OpenSSL

       $sudo apt-get install openssl
       安装OpenSSL通常的库文件，首先使用以下命令来确定在Ubuntu系统中可获得的库文件的应用版本：
       $sudo apt-cache search libssl | grep SSL 
       $sudo apt-get install libssl-dev[](http://www.cs.fsu.edu/~engelen/soap.html)



简单安装：(安装到/usr/local/gSOAP目录下)

        configure --prefix=/usr/local/gSOAP

        make

        make install

安装完成。



为了后面的C和C++版本的服务和客户端的开发能够脱离源代码目录，最好还要如下处理：

（1）将源代码目录下gsoap子目录中的import目录拷贝到gSOAP目录下来；

（2）gSOAP目录下建一个env目录，将gsoap/samples/link下的所有文件拷贝过来，并且生成envC.o（C版本要用到），方法是：

/usr/local/gSOAP/bin/soapcpp2 -penv -c env.h

g++ -c -I /usr/local/gSOAP/include envC.c

（3）gSOAP目录下建一个src目录，将将源代码目录下gsoap子目录中的stdsoap*.*拷贝过来。



安装完成。

下一篇将介绍如何使用gSOAP开发web service 服务端。










