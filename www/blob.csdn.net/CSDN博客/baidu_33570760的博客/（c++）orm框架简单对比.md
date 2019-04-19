# （c++）orm框架简单对比 - baidu_33570760的博客 - CSDN博客
2017年05月27日 10:49:24[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2064
- - - [ODB](#odb)- [基本情况](#基本情况)
- [需要安装四个基本软件包](#需要安装四个基本软件包)
- [测试](#测试)
- [QxOrm](#qxorm)- [基本情况](#基本情况-1)
- [安装](#安装)
- [测试](#测试-1)
Java和.net中的ORM方案非常多也比较成熟, 但是c++中却不多见,目前C++中现存的ORM主流库主要为以下几个
对比图： 
![](http://i.imgur.com/K2y6aGj.png)
主要介绍以下两种
### ODB
#### 基本情况
下载地址 ：[http://www.codesynthesis.com/products/odb/download.xhtml](http://www.codesynthesis.com/products/odb/download.xhtml)
最近一次版本更新时间：2015-07-03
开源协议： GPLv2
前提条件：为了构建ODB源码包，需要Microsoft Visual Studio 2008（VC ++ 9.0）或更高版本。需要一个客户端库用于您选择的数据库系统。例如，对于MySQL，您需要libmysqlclient库。每个数据库运行时库附带的INSTALL文件有关数据库特定先决条件的更多信息。
![](http://i.imgur.com/OnIvZEL.png)
#### 需要安装四个基本软件包：
- 安装ODB编译器
要安装ODB编译器，请下载Windows的预编译二进制包，并将其解压缩到您选择的目录中，例如C：\ tools。 ODB编译器二进制文件将在包目录中的bin \目录中，例如C：\ tools \ odb-x.y.z-i686-windows \ bin \。您可以使用绝对路径运行ODB编译器，例如：C：\ tools \ odb-x.y.z-i686-windows \ bin \ odb –version  
或者您可以将bin \目录添加到Path环境变量中。你应该能够使用它的名称来运行ODB编译器，例如： 
odb –version  查看是否安装成功。
- 安装通用运行时库（libodb）
要安装公共运行时库，请下载libodb源码包，并使用与您的Visual Studio版本相对应的提供的解决方案文件。请参阅libodb软件包中的INSTALL文件，以获取有关如何配置Visual Studio以自动定位libodb头文件，DLL和导入库的信息。
- 
特定于数据库的运行时库（libodb- ）。数据库运行时库特定于您计划使用的数据库系统。例如，如果要使用MySQL，那么数据库运行时将是libodb-mysql。如果要使用多个数据库系统，请为每个数据库系统安装相应的运行时库.
- 
安装配置文件库
如果要安装配置文件库，请下载相应的源代码包（libodb- ），并使用与您的Visual Studio版本对应的提供的解决方案文件。请参阅libodb- 包中的INSTALL文件，了解有关配置文件特定的先决条件的信息，以及如何配置Visual Studio自动定位libodb- 头文件，DLL和导入库。
#### 测试：
构建和运行示例
如果要构建并运行ODB示例，请下载odb-examples软件包并使用提供的解决方案文件。虽然您可以单独运行每个示例，但程序包还包括一个自动运行所有示例的批处理文件。有关如何构建和运行示例的更多详细信息，请参阅随附的INSTALL文件。
建设和运行测试 
   如果要构建并运行ODB测试套件，请下载odb-tests软件包并使用提供的解决方案文件。该软件包还提供允许您以非交互模式自动构建和运行测试的批处理文件。有关如何构建和运行测试套件的更多详细信息，请参阅随附的INSTALL文件。
### QxOrm
#### 基本情况
下载地址：[http://www.qxorm.com/qxorm_en/tutorial_3.html](http://www.qxorm.com/qxorm_en/tutorial_3.html)  需要翻墙（vpn工具） 
开源协议：GNU/GPLv3 （或者花钱购买）
最近一次更新：2016-7-26
![](http://i.imgur.com/nAzjwI2.png)
前提条件： 在安装和构建QxOrm之前，您需要以下库：Qt（从版本4.5.0）和boost（从1.38版本，默认情况下仅需要头文件* .hpp）。
#### 安装：
- 下载并安装Qt（Qt4或Qt5）添加Qt«bin»目录到Windows«路径»
- 下载并安装boost库添加boost«lib_shared»目录到Windows“路径”（可选）
- 下载并安装QxOrm库
- 配置«QxOrm.pri»文件
- 将QxOrm目录添加到Windows“路径”
- 测试执行“qxBlog”示例项目的安装
#### 测试：
- 使用MSVC ++ 2012测试QxOrm软件包的所有样品项目
- 用QtCreator测试QxOrm库
