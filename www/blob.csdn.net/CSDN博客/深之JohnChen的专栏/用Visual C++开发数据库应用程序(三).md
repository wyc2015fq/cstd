# 用Visual C++开发数据库应用程序(三) - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:30:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2378


用Visual C++开发数据库应用程序(三)

2．24 SQL操作

构造和执行SQL语句

构造SQL语句

可以通过三种方式构造SQL语句：在程序开发阶段确定、在运行时确定或由用户输入SQL语句。

在程序开发时确定的SQL语句，具有易于实现、且可在程序编码时进行测试的优势。

在程序运行时确定SQL语句提供了极大灵活性，但给程序高度带来了困难，且需更多的处理时间。由用户输入的SQL语句，极大的增强了程序的功能，但是，程序必须提供友好的人机界面，且对用户输入的语句执行一定程序的语法检查，能够报告用户错误。

执行SQL语句

应用程序的绝大部分数据库访问工作都是通过执行SQL语句完成的，在执行SQL语句之前，必须要先分配一个语句句柄，然后设置相应语句的语句属性，再执行SQL语句。当一个语句句柄使用完成后，调用函数SQLFreeHandle()释放该句柄。

SQLExecute() 

SQLExecute用于执行一个准备好的语然，当语句中有参数时，用当前绑定的参数变量的值。

函数格式：

SQLRETURN SQLExecute(SQLHSTMT StatementHandle);

参数:

StatementHandle 标识执行SQL语句的句柄，可以用SQLAllocHandle()来获得。

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

SQLExecDiret() 

SQLExecDirect直接执行SQL语句，对于只执行一次的操作来说，该函数是速度最快的方法。

函数格式：

SQLRETURN SQLExecDirect(SQLHSTMT StatementHandle,SQLCHAR *StatementText,SQLINTEGER TextLength);

参数:

StatementHandle 语句句柄

StatementText 要执行的SQL语然

StatementText SQL语句的长度。

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, or SQL_INVALID_HANDLE 

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

SQLPripare() 

对于需多次执行的SQL语句来说，除了使用SQLExecDirect函数之外，我们也可以在执行SQL语句之前，先准备SQL语句的执行。对于使用参数的语句，这可大提高程序执行速度。

函数格式:

SQLRETURN SQLPrepare(SQLHSTMT StatementHandle,SQLCHAR* StatementText,SQLINTEGER TextLength); 

参数:

StatementHandle 语句句柄

StatementText 要执行的SQL语然

StatementText SQL语句的长度。

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

使用参数

使用参数可以使一条SQL语句多次执行，得到不同结果

SQLBindParameter

函数SQLBindParameter负责为参数定义变量，实现参数值的传递。

函数格式如下：

SQLRETURNSQLBindParameter(SQLHSTMT StatementHandle,SQLUSMALLINT ParameterNumber,SQLSMALLINT InputOutputType,SQLSMALLINT ValueType,SQLSMALLINT ParameterType,SQLUINTEGER ColumnSize,SQLSMALLINT DecimalDigits,SQLPOINTER ParameterValuePtr,SQLINTEGER BufferLength,SQLINTEGER *StrLen_or_IndPtr); 

参数：

StatementHandle 语句句柄

ParameterNumber 绑定的参数在SQL语句中的序号，在SQL中，所有参数从左到右顺序编号，从1开始。SQL语句执行之前，应该为每个参数调用函数SQLBindParameter绑定到某个程序变量。

InputOutputType 参数类型，可为SQL_PARA_INPUT, SQL_PARAM_INPUT_OUTPUT, SQL_PARAM_OUTPUT。

ParameterType 参数数据类型

ColumnSIze 参数大小

DecimalDigits 参数精度

ParameterValutePtr 指向程序中存放参数值的缓冲区的指针

BufferLength 程序中存放参数值的缓冲区的字节数

StrLen_or_IndPtr 指向存放参数ParameterValuePtr的缓冲区指针

返回值：

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。

执行时传递参数

对于某些文本文档或位图文件，要占用大量的存储空间。因此，在数据源传递这些数据时，可以分开传递。有两个函数可完成这个工作。

函数格式：

SQLRETURN SQLPutData(SQLHSTMT StatementHandle,

SQLPOINTER DataPtr,SQLINTEGER StrLen_or_Ind);

参数：

StatementHandle 参数句柄

DataPtr 指向包含实际数据的缓冲区指针。

StrLen_or_Lnd 缓冲区长度

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。

函数格式：

SQLRETURNSQLParamData(SQLHSTMT StatementHandle,SQLPOINTER *ValuePtrPtr);

参数：

StatementHandle 参数句柄

ValuePtrPtr 指向缓冲区地址的指针

返回值：

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。 　

下面的代码展示如何使用这两个函数

#define MAX_DATA_LEN 1024

SQLINTEGER cbPartID = 0, cbPhotoParam, cbData;

SQLUINTEGER sPartID; szPhotoFile;

SQLPOINTER pToken, InitValue;

SQLCHAR Data[MAX_DATA_LEN];

SQLRETURN retcode;

SQLHSTMT hstmt;

retcode = SQLPrepare(hstmt, "INSERT INTO PICTURES (PARTID, PICTURE) VALUES 

(?, ?)", SQL_NTS);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Bind the parameters. For parameter 2, pass */

/* the parameter number in ParameterValuePtr instead of a buffer */ 

/* address. */ SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, 

SQL_INTEGER, 0, 0, &sPartID, 0, &cbPartID);

SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT,

SQL_C_BINARY, SQL_LONGVARBINARY,

0, 0, (SQLPOINTER) 2, 0, &cbPhotoParam);

/* Set values so data for parameter 2 will be */

/* passed at execution. Note that the length parameter in */

/* the macro SQL_LEN_DATA_AT_EXEC is 0. This assumes that */

/* the driver returns "N" for the SQL_NEED_LONG_DATA_LEN */

/* information type in SQLGetInfo. */

cbPhotoParam = SQL_LEN_DATA_AT_EXEC(0);

sPartID = GetNextID(); /* Get next available employee ID */

/* number. */ retcode = SQLExecute(hstmt);

/* For data-at-execution parameters, call SQLParamData to */

/* get the parameter number set by SQLBindParameter. */

/* Call InitUserData. Call GetUserData and SQLPutData */

/* repeatedly to get and put all data for the parameter. */

/* Call SQLParamData to finish processing this parameter */

while (retcode == SQL_NEED_DATA) {

retcode = SQLParamData(hstmt, &pToken);

if (retcode == SQL_NEED_DATA) {

InitUserData((SQLSMALLINT)pToken, InitValue);

while (GetUserData(InitValue, (SQLSMALLINT)pToken, Data, 

&cbData))

SQLPutData(hstmt, Data, cbData); } }}

VOID InitUserData(sParam, InitValue)SQLPOINTER InitValue;{

SQLCHAR szPhotoFile[MAX_FILE_NAME_LEN];

/* Prompt user for bitmap file containing employee */

/* photo. OpenPhotoFile opens the file and returns the */ /* file handle. */

PromptPhotoFileName(szPhotoFile);

OpenPhotoFile(szPhotoFile, (FILE *)InitValue); break; }

BOOL GetUserData(InitValue, sParam, Data, cbData)SQLPOINTER InitValue;

SQLCHAR *Data;SQLINTEGER *cbData;BOOL Done;{

/* GetNextPhotoData returns the next piece of photo */

/* data and the number of bytes of data returned */

/* (up to MAX_DATA_LEN). */ Done = GetNextPhotoData((FILE *)InitValue, Data,

MAX_DATA_LEN, &cbData); if (Done) {

ClosePhotoFile((FILE *)InitValue); 

return (TRUE); } 

return (FALSE); }

记录的添加、删除和更新

应用程序对数据源的数据更新可以通过三种方式实现：使用相应的SQL语句在数据源上执行；调用函数SQLSetPos实现记录集的定义更新；调用函数SQLBulkOperations实现数据的更新。

直接在数据源上执行SQL语句的方式，可以适用于任何的ODBC数据源，但是，对于后两种更新方式，有的数据源并不支持，应用程序可以调用函数SQLGetInfo确定数据源是否支持这两种方式。

定位更新和删除 

要使用定位更新和删除功能，要按如下顺序：

1)取回记录集：

2)定位到要进行更新或删除操作的行

3)执行更新或删除操作　

参考如下的代码：

#define ROWS 20#define STATUS_LEN 6

SQLCHAR szStatus[ROWS][STATUS_LEN], szReply[3];

SQLINTEGER cbStatus[ROWS], cbOrderID;

SQLUSMALLINT rgfRowStatus[ROWS];

SQLUINTEGER sOrderID, crow = ROWS, irow;

SQLHSTMT hstmtS, hstmtU;

SQLSetStmtAttr(hstmtS, SQL_ATTR_CONCURRENCY, (SQLPOINTER) SQL_CONCUR_ROWVER, 0);

SQLSetStmtAttr(hstmtS, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);

SQLSetStmtAttr(hstmtS, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) ROWS, 0);

SQLSetStmtAttr(hstmtS, SQL_ATTR_ROW_STATUS_PTR, (SQLPOINTER) rgfRowStatus, 0);

SQLSetCursorName(hstmtS, "C1", SQL_NTS);

SQLExecDirect(hstmtS, "SELECT ORDERID, STATUS FROM ORDERS ", SQL_NTS);

SQLBindCol(hstmtS, 1, SQL_C_ULONG, &sOrderID, 0, &cbOrderID);

SQLBindCol(hstmtS, 2, SQL_C_CHAR, szStatus, STATUS_LEN, &cbStatus);

while ((retcode == SQLFetchScroll(hstmtS, SQL_FETCH_NEXT, 0)) != SQL_ERROR) {

if (retcode == SQL_NO_DATA_FOUND) break;

for (irow = 0; irow < crow; irow++) {

if (rgfRowStatus[irow] != SQL_ROW_DELETED)

printf("%2d %5d %*s/n", irow+1, sOrderID, NAME_LEN-1, szStatus[irow]);

} while (TRUE) { printf("/nRow number to update?");

gets(szReply); irow = atoi(szReply);

if (irow > 0 && irow <= crow) { printf("/nNew status?");

gets(szStatus[irow-1]);

SQLSetPos(hstmtS, irow, SQL_POSITION, SQL_LOCK_NO_CHANGE);

SQLPrepare(hstmtU,

"UPDATE ORDERS SET STATUS=? WHERE CURRENT OF C1", SQL_NTS);

SQLBindParameter(hstmtU, 1, SQL_PARAM_INPUT,

SQL_C_CHAR, SQL_CHAR,

STATUS_LEN, 0, szStatus[irow], 0, NULL);

SQLExecute(hstmtU); } else if (irow == 0) { break; }

}

}

用SQLBulkOperations()更新数据 

函数SQLBulkOperations的操作是基于当前行集的，在调用函数SQLBulkOperations之前，必须先调用函数SQLFetch或SQLFetchScroll获取行集，然后，再执行修改或删除操作。　

函数格式：

SQLRETURN SQLBulkOperations(SQLHSTMT StatementHandle,

SQLUSMALLINT Operation);　

参数：

StatementHandle 参数句柄

Operation 标识执行的操作类型，可以是以下几种之一：

SQL_ADD

SQL_UPDATE_BY_BOOKMARK

SQL_DELETE_BY_BOOKMARK

SQL_FETCH_BY_BOOKMARK　

返回值：

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

取回查询结果

绑定列

从数据源取回的数据存放在应用程序定义的变量中，因此，我们必须首先分配与记录集中字段相对应的变量,然后通过函数SQLBindCol将记录字段同程序变量绑定在一起。对于长记录字段，可以通过调用函数SQLGetData()直接取回数据。

绑定字段可以根据自己的需要，全部绑定，也可以绑定其中的某几个字段。

记录集中的字段可以在任何时候绑定，但是，新的绑定只有当下一次从数据源中取数据时执行的操作才有郊，而不会对已经取回的数据产生影响。

函数格式:

SQLRETURN SQLBindCol(SQLHSTMT StatementHandle,

SQLUSMALLINTColumnNumber, SQLSMALLINT TargetType,

SQLPOINTERTargetValuePtr,SQLINTEGERBufferLength,

SQLINTEGER *StrLen_or_IndPtr);

参数：

StatementHandle 语句句柄

ColumnNumber 标识要绑定的列号。数据列号从0开始升序排列，其中第0列用作书签。如果没有使用书签，则列号从1开始。

TargetType 数据类型

TargetValuePtr 绑定到数据字段的缓冲区的地址

BufferLength 缓冲区长度

StrLen_or_IndPtr 指向绑定数据列使用的长度的指针.

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。　

SQLFetch()

函数SQLFetch用于将记录集中的下一行变为当前行，并把所有捆绑过的数据字段的数据拷贝到相应的缓冲区。

函数格式:

SQLRETURN SQLFetch(SQLHSTMT StatementHandle)

参数:

StatementHandle 语句句柄

返回值:

SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NO_DATA, SQL_STILL_EXECUTING, SQL_ERROR, or SQL_INVALID_HANDLE.

成功返回SQL_SUCCESS，如果返回值为SQL_ERROR或SQL_SUCCESS_WITH_INFO，可以用函数SQLGetDiagRec获取相应SQLSTATE的值。

光标

应用程序获取数据是通过光标(Cursor)来实现的，在ODBC中，主要有三种类型的光标:

单向光标： 

单向光标只能向前移动，要返回记录集的开始位置，程序必须先关闭光标，再打开光标，它对于只需要浏览一次的应用非常有用，而且效率很高。对于需要光标前后移动的，单向光村不适用。

可滚动光标 

可滚动光标通常基于图形用户界面的程序，用户通过屏幕向前或向后滚动，浏览记录集中的数据。

块光标 

所谓块光标，可以理解为执行多行的光标，它所指向的行称为行集。对于网络环境下的应用，使用块光标可以在一定程序上减轻网络负载。

要使用块光标，应完成以下工作:

1）设定行集大小

2）绑定行集缓冲区

3）设定语句句柄属性

4）取行行集结果

由于块光标返回多行记录，应用程序必须把这多行的数据绑定到某些数据组中，这些数据称为行集缓冲区。绑定方式有两种:列方式和行方式。

ODBC光标库 

有些应用程序不支持可滚动光标和块光标，ODBC SDK 提供了一个光标库(ODBCCR32.DLL),在应用程序中可通过设置连接属性(SQL_STTR_ODBC_CURSORS)激活光标库。　

参考如下代码：

#define NAME_LEN 50

#define PHONE_LEN 10

SQLCHAR szName[NAME_LEN], szPhone[PHONE_LEN];

SQLINTEGER sCustID, cbName, cbCustID, cbPhone;

SQLHSTMT hstmt;

SQLRETURN retcode;retcode = SQLExecDirect(hstmt,

"SELECT CUSTID, NAME, PHONE FROM CUSTOMERS ORDER BY 2, 1, 3", 

SQL_NTS);

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

/* Bind columns 1, 2, and 3 */

SQLBindCol(hstmt, 1, SQL_C_ULONG, &sCustID, 0, &cbCustID);

SQLBindCol(hstmt, 2, SQL_C_CHAR, szName, NAME_LEN, &cbName);

SQLBindCol(hstmt, 3, SQL_C_CHAR, szPhone, PHONE_LEN, &cbPhone);

/* Fetch and print each row of data. On */

/* an error, display a message and exit. */ while (TRUE) {

retcode = SQLFetch(hstmt);

if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {

show_error(); }

if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO){

fprintf(out, "%-*s %-5d %*s", NAME_LEN-1, szName,

sCustID, PHONE_LEN-1, szPhone); } else { break; }

}

}

2．25 断开同数据源的连接

当完成对数据库操作后，就可以调用SQLDisconnect函数关闭同数据源连接。

当该句柄的事务操作还未完成时，应用程序调用SQLDisconnect,这时，驱动器返回SQLSTATE 25000，表明此次事务没有变化且连接还打开。

在应用程序调用SQLDisconnect之前没有释放与之相连的描述时，当成功的与数据源断开连接后，驱动器自动释放与之相连的参数、描述器等。但当与之相连的某一个参数还在执行异步操作时，SQLDisConnect返回SQL_ERROR，且SQLSTATE的值置为HY010.

2．26 释放ODBC环境

最后一步就是释放ODBC环境参数了。

2．3 ODBC API编程总结

关系数据库的研究与应用是当今计算机界最活跃的领域之一，各种数据库产品行行色色，各有千秋；这种情况一方面给用户带来了好处，另一方面又给应用程序的移植带来了困难。尤其是在客户机／服务器体系结构中，当用户要从客户机端访问不同的服务器，而这些服务器的数据库系统又各不相同，数据库之间的互连访问就成为一个难题，因此，微软公司提出了ODBC。由于ODBC思想上的先进性及其微软公司的开放策略，ODBC现在已经成为事实上的工业标准，它是目前数据库应用方面很多问题强有力的解决方案，正逐步成为Windows平台上的标准接口。

ODBC是一种用来在相关或不相关的数据库管理系统（DBMS）中存取数据的标准应用程序设计接口（API）。它的基本思想是为用户提供简单、标准、透明、统一的数据库联接的公共编程接口，在各个厂家的支持下能为用户提供一致的应用开发界面，使应用程序独立于数据库产品，实现各种数据库之间的通信。开发厂商根据ODBC的标准去实现底层的驱动程序，它对用户是透明的。

作为一种数据库联接的标准技术，ODBC有以下几个主要特点：

1·ODBC是一种使用SQL的程序设计接口；

2·ODBC的设计是建立在客户机／服务器体系结构基础之上的；

3·ODBC使应用程序开发者避免了与数据源联接的复杂性；

4·ODBC的结构允许多个应用程序访问多个数据源，即应用程序与数据源的关系是多对多的关系。　

