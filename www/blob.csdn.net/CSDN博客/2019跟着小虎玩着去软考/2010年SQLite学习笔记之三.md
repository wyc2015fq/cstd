# 2010年SQLite学习笔记之三 - 2019跟着小虎玩着去软考 - CSDN博客
2010年02月22日 17:45:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3650

**2010年SQLite学习笔记之三**
# 一．如何备份数据库
先打开数据库test**.**db
**E:/sqlite/tool/sqlite-3_6_22>sqlite3 D:/Test/debug/test.db**
**sqlite> .backup D://Test//debug//test.bak**
**注意：一定要用//双斜杠**
# 二．如何恢复数据库
先打开数据库test**.**db
**E:/sqlite/tool/sqlite-3_6_22>sqlite3 D:/Test/debug/test.db**
**sqlite> .restore D://Test//debug//test.bak**
**注意：一定要用//双斜杠**
# 三．先判断表如果不存在，则再创建表
SQL可以如下写，供参考：
**create table if not exists student(id integer primary key autoincrement,/**
**age smallint ,anchor smallint);**
# 四．如何查询SQLite3小工具或者SQLite3.dll动态链接库的版本
**执行 select sqlite_version();命令即可**
# 五．SQLite3的最新版本是（截止日期2010年2月22日）
**3.6.22**
# 六．针对SQLite3,两大优秀C++封装库
**1.CppSQLite3**
[http://www.codeproject.com/KB/database/CppSQLite.aspx](http://www.codeproject.com/KB/database/CppSQLite.aspx)
**2.wxSQLite3**
[http://sourceforge.net/projects/wxcode/files/Components/wxSQLite3/wxsqlite3-1.9.8.zip/download](http://sourceforge.net/projects/wxcode/files/Components/wxSQLite3/wxsqlite3-1.9.8.zip/download)
# 七．如何在Windows下编译SQLite3
1．先从官网获取sqlite-amalgamation-3_6_22.zip；
2．接着从官网获取sqlitedll-3_6_22.zip
下载编译好的DLL，这里有我们需要的sqlite3.def该文件
3．打开VC新建一个“Win32 Dynamic-Link Library”工程，命名为:sqlite3；
4．在接下来的对话框中选择"An empty DLL project",点 FINISH->OK；
5．将sqlite-amalgamation-3_6_22文件夹下的sqlite3.h以及sqlite3.c两个文件复制到工程文件夹下；
将sqlitedll-3_6_22文件夹下的sqlite3.def该文件复制到工程文件夹下；
6．在工程的Source Files中添加sqlite3.c文件；
7．在工程的Include Files中添加sqlite3.h文件；
8．在工程的Resource Files中添加sqlite3.def文件；
9．针对如下问题：
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_database_name
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_database_name16
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_origin_name
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_origin_name16
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_table_name
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_table_name16
sqlite3.def : error LNK2001: unresolved external symbol sqlite3_table_column_metadata
Debug/sqlite3.lib : fatal error LNK1120: 7 unresolved externals
在菜单【Project】-【Settings…】-【C/C++】标签下的Category【General】下
Preprocessor definitions下:
新增2个编译选项，分别是：
THREADSAFE
SQLITE_ENABLE_COLUMN_METADATA
10．往工程中添加sqlite3.def文件就是为生成sqlite3.lib文件；
sqlite3.lib是与sqlite3.dll动态链接库文件一起提供给软件开发者的；
# 八．如何解决Windows下的编译警告
**e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4047: 'function' : 'void ** ' differs in levels of indirection from 'long *'**
**e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 1**
**e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 2**
**e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4047: 'function' : 'void ** ' differs in levels of indirection from 'long *'**
**e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 1**
**e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 3**
**e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4047: '==' : 'void *' differs in levels of indirection from 'const int '**
**e:/sqlite/code/sqlite3/sqlite3.c(65536) : warning C4049: compiler limit : terminating line number emission**
**warning C4761: integral size mismatch in argument; conversion supplied**
**warning C4761: integral size mismatch in argument; conversion supplied**
**warning C4761: integral size mismatch in argument; conversion supplied**
**目前，还不想直接屏蔽警告错误。没有想到更好的方法。待定。**
**有知道的朋友，还请指点。我将不甚感激。**
