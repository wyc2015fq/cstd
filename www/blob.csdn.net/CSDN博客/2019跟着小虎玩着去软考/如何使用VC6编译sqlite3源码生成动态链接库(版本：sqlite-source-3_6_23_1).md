# 如何使用VC6编译sqlite3源码生成动态链接库(版本：sqlite-source-3_6_23_1)  - 2019跟着小虎玩着去软考 - CSDN博客
2010年04月19日 14:17:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：6937
**如何使用****VC6****编译****sqlite-source-3_6_23_1**
## 一．版本：
2010-Mar-30Version3.6.23.1
## 二．源代码获取：
在官网的[http://www.sqlite.org/download.html的Download](http://www.sqlite.org/download.html%E7%9A%84Download)页面下载：
sqlite-source-3_6_23_1.zip
## 三．创建Win32动态链接库工程
1．打开VC新建一个“Win32 Dynamic-Link Library”工程，命名为:sqlite3；
2．在接下来的对话框中选择"An empty DLL project",点 FINISH->OK；
# 四．将文件添加进工程
将sqlite-source-3_6_22里的文件添加到sqlite3工程中：
1.将sqlite-source-3_6_22下的所有*.C文件添加在工程的Source File中
2.将sqlite-source-3_6_22下的所有*.h文件添加在工程的Header File中
**注意：不要将****tclsqlite.c****源文件添加进工程中；**
## 五．文件说明：
1. tclsqlite.c用于生成基于TCL的API，如果需要编译，则需要另外下载tcl.h头文件；
2. shell.c用于生成命令行模式的sqlite.exe；
3. fts3*.c是全文索引的模块；
4. rTree.c是建立数据库R树索引的模块；
## 六．编译参数：
点击VC6的菜单【Project】-【Settings…】-【C/C++】书签--【Preprocessor definitions：】
添加如下4个编译选项，每个编译选项之间用逗号分隔开来。
1. SQLITE_CORE
2. SQLITE_ENABLE_RTREE
3. SQLITE_ENABLE_COLUMN_METADATA
4. SQLITE_ENABLE_FTS3
## 七．下载sqlite-amalgamation-3_6_23_1.zip：
在官网的[http://www.sqlite.org/download.html的Download](http://www.sqlite.org/download.html%E7%9A%84Download)页面下载
sqlite-amalgamation-3_6_23_1.zip
## 八．将sqlite3.def文件添加在工程的Header File目录中：
1.解压sqlite-amalgamation-3_6_23_1.zip；
2.将sqlite-amalgamation-3_6_23_1目录下的sqlite3.def文件拷贝至sqlite3工程中；
3.在工程的Header File目录添加sqlite3.def文件；
## 九．按F7进行编译：
## 附注：更多编译信息请参考如下：
官方的编译信息：
[http://www.sqlite.org/compile.html](http://www.sqlite.org/compile.html)
如何用Visual Studio.NET开发环境进行编译
[http://www.sqlite.org/cvstrac/wiki?p=HowToCompileWithVsNet](http://www.sqlite.org/cvstrac/wiki?p=HowToCompileWithVsNet)
## 更多参考：
**[小老虎多多](http://blog.csdn.net/littletigerat)****的****[如何使用VC6编译sqlite3(版本：sqlite-source-3_6_22)](http://blog.csdn.net/littletigerat/archive/2010/03/23/5408424.aspx)**
