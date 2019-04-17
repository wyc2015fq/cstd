# ODB学习笔记之基础环境搭建 - DoubleLi - 博客园






一，简介

ODB是应用于C++的一个开源、跨平台、跨[数据库](http://lib.csdn.net/base/mysql)的对象关系映射（ORM）系统。

它可以让你持久化C++对象到关系数据库，而不必处理表、列或者SQL，无需手动编写任何映射代码。

ODB支持[MySQL](http://lib.csdn.net/base/mysql)，SQLite，PostgreSQL，[Oracle](http://lib.csdn.net/base/oracle)和微软SQL Server关系数据库以及C ++98/03和C ++11语言标准。

它还配备了用于Boost和Qt可选的配置文件，让你可以无缝地使用这些库持久化C++类的值类型、容器和智能指针。

它有易用性，简洁的代码，安全，数据库可移植性，优良的性能，可维护性等优点。

总之它具有完善的学习文档的非常棒的C++的ORM框架。

可以去官网查看更多：http://www.codesynthesis.com/products/odb/



二，准备工作

以sqlite为例，在windows平台，用VS2013来搭建开发环境。

1，下载安装包

2，下载ODB核心库

3，下载数据库运行库

4，下载例子程序

5，下载sqlite的源文件(用sqlite所以需要)

地址：http://www.codesynthesis.com/products/odb/download.xhtml

![](http://img.blog.csdn.net/20140622223824953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRmVuZ19fX19fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由于是windows平台，sqlite数据库，用的以下文件。（别的平台，别的数据库下载相应该的库文件）

odb-2.3.0-i686-windows（ODB的编译器，就是所对象成对sql和头文件，用于持久化相关）

libodb-2.3.0（ODB的核心库，编译出来的odb_d.lib(Debug版本)在examples中需要）

libodb-sqlite-2.3.0（odb操作sqlite的相关功能，编译出来的odb-sqlite-d.lib(Debug版本)在examples中需要）

odb-examples-2.3.0 （odb的例子和演示）



三，安装ODB

安装很简单。

1，解压odb-2.3.0-i686-windows，

2，把bin目录加到环境变量中

3，命令行输入odb --version看是否安装成功

详细的可以看README里面写的。

![](http://img.blog.csdn.net/20140622225603281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRmVuZ19fX19fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如现如下提示，可以就说安装成功了

![](http://img.blog.csdn.net/20140622225841687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRmVuZ19fX19fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



四，编译ODB核心库，ODB的sqlite库，sqlite库

以下都是说的debug版本。

1，编译ODB核心库，生成odb-d.lib和odb-d-2.3-vc11.dll

2，编译sqlite库，生成sqlite.lib

3，编译ODB的sqlite库，生成odb-sqlite-d.lib和odb-sqlite-d-2.3-vc11.dll





解压libodb-2.3.0，后会看见vs2008-2011的工程文件，真接用VS打开就OK，然后把下面路径加到VC的目录中。

  Include:    ...\libodb
  Library:    ...\libodb\lib
  Executable: ...\libodb\bin

然后编译，会在lib目录下生成odb-d.lib，在bin目录下生成odb-d-2.3-vc11.dll。



用sqlite编译出，生成sqlite.lib，直接可以参考官网。

需要注意的是，在编译的时候需要把SQLITE_ENABLE_UNLOCK_NOTIFY宏加上，如果不加上，那就要在libodb-sqlite-2.3.0工程中去掉LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY宏。



解压libodb-sqlite-2.3.0，同样打开vs的工程文件。把vc目录添加到到工程中。

  Include:    ...\libodb-sqlite
  Library:    ...\libodb-sqlite\lib
  Executable: ...\libodb-sqlite\bin

要把odb-d.lib所在目录加到Library中，把odb-d-2.3-vc11.dll加到Executable中。

还要把sqlite的头文件所以在的目录放到  Include中，把sqlite.lib所在目录加到Library中。

然后编译，会在lib目录下生成odb-sqlite-d.lib，在bin目录下生成odb-sqlite-d-2.3-vc11.dll。



五，运行例子程序

前期的工作已经做好了，直接解压，用vs的打开。

我先进的是Hello程序。

1，用ODB编译器生成，persion.hxx，和person-odb.ixx文件。

方法是： 在该目录下输入odb -d sqlite --generate-query --generate-schema person.hxx

详见：http://www.codesynthesis.com/products/odb/doc/odb.xhtml

2，把核心库头文件，libodb-sqlite的头文件及sqlite的头文件目录加到VC的Include中。

3，把sqlite.lib，odb-sqlite-d.lib，odb-d.lib的目录均加到VC的Libary中。

然后编译运行。出现

![](http://img.blog.csdn.net/20140622232819890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRmVuZ19fX19fXw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明搭建成功。



总结：

1，ODB也是才接触，当时走了不了弯路，原因就是求成心切，不看文档，下了程序包就开搞了，结果可想而知。勿在浮沙筑高台。

2，编译时当时为一个错误盲目google好久，最后发现自己output都没看，直接看error面板去了。

插曲：当时odb才安装好，在自己打开的cmd命令中odb是OK的，但在批处理中写的odb系统却找不到odb。导致vs中讲运例子出现错误。（vs中要调用odb修改相关文件），但vs报的是连接错误，cmd.exe 的9009错误。没看output，直接google去了。后来发现是找不到odb，半天也没解决。后来重起之后解决了。环境变量有一定的缓存？

3，多看看文档，官网上的操作写得很详细，而且每个包下，都有readme和insall文档，都对如何运行有详细的描述，按上面的来一般是没什么问题。这个项目考虑也很周全，2008到2011的工程都给出了。









