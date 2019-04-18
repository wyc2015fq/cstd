# 在VC中动态加载ODBC时的一些小细节 - ljx0305的专栏 - CSDN博客
2008年05月28日 10:01:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：713标签：[microsoft																[access																[数据库																[文档																[null																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
Windows系统子目录中的动态链接库Odbcinst.dll提供了一个可以动态地增加、修改和删除数据源的函数SQLConfigDataSource()。该函数的原型如下：
BOOL SQLConfigDataSource(HWND hwndParent, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes);
hwndParent参数是父窗口句柄。如果该值为NULL，将不会显示与父窗口有关的对话框。
fRequest参数可以设置为下面的数值之一：
ODBC_ADD_DSN：增加一个新的用户数据源；
ODBC_CONFIG_DSN：修改(配置)一个已经存在的用户数据源；
ODBC_REMOVE_DSN：删除一个已经存在的用户数据源；
ODBC_ADD_SYS_DSN：增加一个新的系统数据源；
ODBC_CONFIG_SYS_DSN：修改 (配置)一个已经存在的系统数据源；
ODBC_REMOVE_SYS_DSN：删除一个已经存在的系统数据源。
lpszDriver参数用于传递数据库引擎的名字，等同于方法一中strDbType变量。
lpszAttirbutes参数是关键字的值，即一连串的"keyname=value"字符串，每两个字符串之间用 "/"隔开，如"DSN=Demo/0DSN=demo/0Description=示例数据库"。关于该参数的详细设置请参阅MSDN中SQLConfigDataSource()函数的帮助文档和各种ODBC驱动程序文档。
由于VC的缺省库文件中不包含SQLConfigDataSource()函数，因此使用该函数之前需要将odbcinst.h文件包含在工程的头文件中，在工程的Settings属性对话框Link属性页的Object/library modules编辑框中增加odbccp32.lib，同时保证系统目录system32下有文件odbccp32.dll。
否则**在link时会出现:error LNK2001: unresolved external symbol **[**_SQLConfigDataSource@16**](mailto:_SQLConfigDataSource@16)**的错误.**
仍以Microsoft Access为例，设置数据源名为demo，数据源描述为"示例数据源"，那么在需要动态加载数据源的地方加入下列代码即可：
::SQLConfigDataSource(NULL, ODBC_ADD_SYS_DSN, "Microsoft Access Driver (*.mdb)", "DSN=demo/0Description=示例数据库");
需要注意的是**在Microsoft Access Driver (*.mdb)中,"Driver" 和 "("之间一定要有一格空格**,不然的话会导致函数调用失败,笔者就是在这个上面调试花了大半个小时的时间.
