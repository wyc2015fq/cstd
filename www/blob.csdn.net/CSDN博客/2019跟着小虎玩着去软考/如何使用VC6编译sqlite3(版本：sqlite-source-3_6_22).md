# 如何使用VC6编译sqlite3(版本：sqlite-source-3_6_22) - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月23日 15:30:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3565
**如何使用VC6编译sqlite-source-3_6_22**
**引言：**
**[小老虎多多](http://blog.csdn.net/littletigerat)使用VC6编译sqlite-source-3_6_22的编译方法。**
# 一.创建Win32动态链接库工程
1．打开VC新建一个“Win32 Dynamic-Link Library”工程，命名为:sqlite3；
2．在接下来的对话框中选择"An empty DLL project",点 FINISH->OK；
# 二.将文件添加进工程
将sqlite-source-3_6_22里的文件添加到sqlite3工程中：
1.将sqlite-source-3_6_22下的所有*.C文件添加在工程的Source File中
2.将sqlite-source-3_6_22下的所有*.h文件添加在工程的Source File中
# 三.按F7进行编译
**出现如下错误：**
d:/learn/sqlite3/tclsqlite.c(28) : fatal error C1083: Cannot open include file: 'tcl.h': No such file or directory
**说明：**tclsqlite.c用于生成基于TCL的API，如果要编译，这需要另外下载tcl.h；shell.c用于生成命令行模式的sqlite.exe；
# 四.删除掉文件tclsqlite.c
再此处去掉文件tclsqlite.c，不让此文件参加编译，按F7重新编译
**出现如下错误**
fts3_tokenizer.obj : error LNK2005: _sqlite3_api already defined in fts3.obj
rtree.obj : error LNK2005: _sqlite3_extension_init already defined in fts3.obj
rtree.obj : error LNK2005: _sqlite3_api already defined in fts3.obj
# 五.编译通过方案
## 1.编译通过的方案一：
添加编译选项SQLITE_CORE 和SQLITE_ENABLE_FTS3
以及SQLITE_ENABLE_RTREE，再次按F7进行编译
## 2.编译通过的方法二：
### 2.1添加编译选项
如果仅仅添加SQLITE_ENABLE_FTS3和以及SQLITE_ENABLE_RTREE两个编译选项，不添加SQLITE_CORE编译选项；
### 2.2修改文件sqlite3ext.h
在文件sqlite3ext.h的
#define SQLITE_EXTENSION_INIT1const sqlite3_api_routines *sqlite3_api = 0;
行下面添加，添加如下内容：
#define EXTERN_SQLITE_EXTENSION_INIT1extern const sqlite3_api_routines *sqlite3_api = 0;
### 2.3修改fts3.c和fts3_tokenizer.c两个文件
在fts3.c和fts3_tokenizer.c此2个文件里，**将****SQLITE_EXTENSION_INIT1****修改为EXTERN_SQLITE_EXTENSION_INIT1**，具体如下：
#ifndef SQLITE_CORE
#include "sqlite3ext.h"
EXTERN_SQLITE_EXTENSION_INIT1
#else
#include "sqlite3.h"
#endif
### 2.4修改文件rtree.c
将rtee.c文件的里，**将****SQLITE_EXTENSION_INIT1****修改为**extern const sqlite3_api_routines *sqlite3_api即可，具体如下：
#ifndef SQLITE_CORE
#include "sqlite3ext.h"
extern const sqlite3_api_routines *sqlite3_api;
#else
#include "sqlite3.h"
#endif
# 附注说明：
1. rTree.c是建立数据库R树索引的模块；
2. fts3*.c是全文索引模块
