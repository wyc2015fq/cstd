# 用Visual C++开发数据库应用程序(二) - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2165


用Visual C++开发数据库应用程序(二)

2．2 应用ODBC API建立应用程序

虽然直接应用ODBC API编制应用程序相对来说较为繁琐，但是，由于直接使用ODBC API编写的程序相对要简洁、高效。所以，我们有必要学习直接使用ODBC API编程。

一般地，编写ODBC程序主要有以下几个步骤：

分配ODBC环境 

分配连接句柄 

连接数据源 

构造和执行SQL语句 

取得执行结果 

断开同数据源的连接 

释放ODBC环境 

2．21 分配ODBC环境

对于任何ODBC应用程序来说，第一步的工作是装载驱动程序管理器，然后初始化ODBC环境，分配环境句柄。

首先，程序中声明一个SQLHENV类型的变量，然后调用函数SQLAllocHandle，向其中传递分配的上述SQLHENV类型的变量地址和SQL_HANDLE_ENV选项。如下代码所示：

SQLHENV henv;

SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&henv);

执行该调用语句后，驱动程序分配一个结构，该结构中存放环境信息，然后返回对应于该环境的环境句柄。

2．22分配连接句柄

分配环境句柄后，在建立至数据源的连接之前，我们必须分配一个连接句柄，每一个到数据源的连接对应于一个连接句柄。

首先，程序定义了一个SQLHDBC类型的变量，用于存放连接句柄，然后调用SQLAllocHandle函数分配句柄。如下代码所示：

SQLHDBC hdbc;

SQLAllocHandle(SQL_HANDLE_DBC,henv,&hdbc);

henv为环境句柄。

2．23 连接数据源

当连接句柄分配完成后，我们可以设置连接属性，所有的连接属性都有缺省值，但是我们可以通过调用函数SQLSetConnectAttr()来设置连接属性。用函数SQLGetConnectAttr()获取这些连接属性。

函数格式如下：

SQLRETURN SQLSetConnectAttr(SQLHDBC ConnectionHandle,SQLINTEGER Attribute,SQLPOINTER ValuePtr,SQLINTEGER StringLength);

SQLRETURN SQLGetConnectAttr(SQLHDBC ConnectionHandle,SQLINTEGER Attribute,SQLPOINTER ValuePtr,SQLINTEGER StringLength);

应用程序可以根据自己的需要设置不同的连接属性。

完成对连接属性的设置之后，就可以建立到数据源的连接了。对于不同的程序和用户接口，可以用不同的函数建立连接：SQLConnect、SQLDriverConnect、SQLBrowseConnect。

SQLConnect

该函数提供了最为直接的程序控制方式，我们只要提供数据源名称、用户ID和口令，就可以进行连接了。

函数格式:

SQLRETURN SQLConnect(SQLHDBC ConnectionHandle,SQLCHAR ServerName,SQLSMALLINT NameLength1,SQLCHAR UserName,SQLSMALLINT NameLength2,SQLCHAR *Authentication,SQLSMALLINT NameLength3); 

参数:

ConnectionHandle 连接句柄

ServerName 数据源名称

NameLength1 数据源名称长度

UserName 用户ID

NameLength2 用户ID长度

Authentication 用户口令

NameLength3 用户口令长度

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE. 

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

下面的代码演示了如何使用ODBC API的SQLConnect函数建立同数据源SQLServer的连接。　

#include “sqlext.h”

SQLHENV henv;;

SQLHDBC hdbc;

SQLHSTMT hstmt;

SQLRETURN retcode;

/*Allocate environment handle */

retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv); 

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Set the ODBC version environment attribute */

retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Allocate connection handle */

retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Set login timeout to 5 seconds. */

SQLSetConnectAttr(hdbc, (void*)SQL_LOGIN_TIMEOUT, 5, 0);

/* Connect to data source */

retcode = SQLConnect(hdbc, (SQLCHAR*) "Sales", SQL_NTS,

(SQLCHAR*) "JohnS", SQL_NTS,

(SQLCHAR*) "Sesame", SQL_NTS);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){

/* Allocate statement handle */

retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Process data */;

SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

SQLDisconnect(hdbc);

}

SQLFreeHandle(SQL_HANDLE_DBC, hdbc); 

} 

}

SQLFreeHandle(SQL_HANDLE_ENV, henv);

SQLDriveConnect

函数SQLDriveConnect用一个连接字符串建立至数据源的连接。它可以提供比SQLConnect函数的三个参数更多的信息，可以让用户输入必要的连接信息。

如果连接建立，该函数返回完整的字符串，应用程序可使用该连接字符串建立另外的连接。

函数格式:

SQLRETURN SQLDriverConnect(SQLHDBC ConnectionHandle,SQLHWND WindowHandle,SQLCHAR InConnectionString,SQLSMALLINT StringLength1,SQLCHAR OutConnetionString,SQLSMALLINT BufferLength,SQLSMALLINT *StringLength2Ptr,SQLSMALLINT DriverCompletion); 

参数：

ConnectionHandle 连接句柄

WindowHandle 窗口句柄，应用程序可以用父窗口的句柄，或用NULL指针

InConnectionString 连接字符串长度

OutConnectionString 一个指向连接字符中的指针

BufferLength 存放连接字符串的缓冲区的长度

StringLength2Ptr 返回的连接字符串中的字符数

DriverCompletion 额外连接信息,可能取值有：SQL_DRIVER_PROMPT,

SQL_DRIVER_COMPLETE, 

SQL_DRIVER_COMPLETE_REQUIRED, or

SQL_DRIVER_NOPROMPT.

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE.

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

SQLBrowseConnect

函数SQLBrowseConnect支持以一种迭代的方式获取到数据源的连接，直到最后建立连接。它是基于客房机/服务器的体系结构，因此，本地数据库不支持该函数。

一般，我们提供部分连接信息，如果足以建立到数据源的连接，则成功建立连接，否则返回SQL__NEED__DATA，并在OutConnectionString参数中返回所需要的信息。

函数格式:

SQLRETURN SQLBrowseConnect(SQLHDBC ConnectionHandle,SQLCHAR* InConnectionString,SQLSAMLLINT StringLength1,SQLCHAR* OutConnectionString,SQLSMALLINT BufferLength,SQLSMALLINT *StringLength2Ptr); 

参数：

ConnectionHandle 连接句柄

InConnectionString 指向输出字符串的指针

StringLength1 输出字符串的指针长度

OutConnectionString 指向输出字符串的指针

BufferLength 用于存放输出字符串的缓冲区的长度

StringLength2Ptr 实际返回的字符串的长度　

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE. 

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

下面的代码讲述了如何使用ODBC API的SQLBrowseConnect函数建立同数据源的连接。　

#define BRWS_LEN 100SQLHENV 

henv;SQLHDBC hdbc;

SQLHSTMT hstmt;

SQLRETURN retcode;

SQLCHAR szConnStrIn[BRWS_LEN], szConnStrOut[BRWS_LEN];

SQLSMALLINT cbConnStrOut;/* Allocate the environment handle. */

retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv); 

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Set the version environment attribute. */

retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3, 0);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Allocate the connection handle. */

retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Call SQLBrowseConnect until it returns a value other than */

/* SQL_NEED_DATA (pass the data source name the first time). */

/* If SQL_NEED_DATA is returned, call GetUserInput (not */

/* shown) to build a dialog from the values in szConnStrOut. */

/* The user-supplied values are returned in szConnStrIn, */

/* which is passed in the next call to SQLBrowseConnect. */

lstrcpy(szConnStrIn, "DSN=Sales"); do {

retcode = SQLBrowseConnect(hdbc, szConnStrIn, SQL_NTS,

szConnStrOut, BRWS_LEN, &cbConnStrOut);

if (retcode == SQL_NEED_DATA)

GetUserInput(szConnStrOut, szConnStrIn);

} while (retcode == SQL_NEED_DATA);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){

/* Allocate the statement handle. */

retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Process data after successful connection */ ...;

SQLFreeHandle(SQL_HANDLE_STMT, hstmt); }

SQLDisconnect(hdbc); } }

SQLFreeHandle(SQL_HANDLE_DBC, hdbc); }}

SQLFreeHandle(SQL_HANDLE_ENV, henv);

