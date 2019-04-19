# 如何对sqlite3数据库进行加密 - 2019跟着小虎玩着去软考 - CSDN博客
2010年04月19日 15:01:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：9206
**如何对****sqlite3****数据库进行加密**
## 一．请先按笔者的文章,先生成sqlite3的动态链接库
[如何使用VC6编译sqlite3源码生成动态链接库(版本：sqlite-source-3_6_23_1)](http://blog.csdn.net/littletigerat/archive/2010/04/19/5502491.aspx)
## 二．解过互联网获取wxsqlite3-1.9.8.zip软件开发包
**三．解压**
解压展开wxsqlite3-1.9.8.zip软件开发包
## 四．拷贝
将目录wxsqlite3-1.9.8/sqlite3/secure/src/codec-c下的所有文件拷贝到sqlite3的工程目录下
## 五．添加头文件：
将codec.h、rijndael.h以及sha2.h该三个头文件添加进工程的Header File中
## 修改codec.h头文件
打开codec.h在#include "rijndael.h"下面添加#include "sqliteInt.h"
## 六．修改文件pager.c
打开sqlite3secure.c源文件，将整个文件进行复制；
然后粘贴到pager.c文件最后，语句“#endif /* SQLITE_OMIT_DISKIO */”语句之前，即可；
注意：也可将粘贴部分里面的多余的“#ifndef SQLITE_OMIT_DISKIO，以及#endif /* SQLITE_OMIT_DISKIO */”删除掉；
## 七．新增2个编译开关：
1．SQLITE_HAS_CODEC；
2．THREADSAFE；
## 八．在sqlite3.def文件最后新增2行：
sqlite3_key以及sqlite3_rekey
也就是多导出sqlite3_key和sqlite3_ rekey两个函数。
## 九．编译（End）。
