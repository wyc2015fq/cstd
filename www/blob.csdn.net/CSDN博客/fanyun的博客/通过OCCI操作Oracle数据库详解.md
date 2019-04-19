# 通过OCCI操作Oracle数据库详解 - fanyun的博客 - CSDN博客
2017年03月31日 12:34:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2205
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**1.简介**
 OCCI 即 [Oracle](http://lib.csdn.net/base/oracle) C++ Call Interface     
 在Windows下访问Oracle[数据库](http://lib.csdn.net/base/mysql)可以使用ADO,ADO[.NET](http://lib.csdn.net/base/dotnet),OLEDB,ODBC,跨平台的OCCI等方法,其中速度最快，对Oracle支持最完整的是Oracle提供的OCCI.
**   2.安装OCCI SDK**
  OCCI SDK包含在Oracle客户端中，在如下位置可以找到：
  头文件：%OARCLE_HOME%\oci\include
  库文件：%OARCLE_HOME%\oci\lib\msvc
  推荐安装Oracle即时客户端（Instant Client），它也包含有OCCI SDK。
  头文件：instantclient\sdk\include
  库文件：instantclient\sdk\lib\msvc
** 3.OCCI编程步骤**
**3.1连接到数据库**
//创建OCCI上下文环境
Environment*env=Environment::createEnvironment();
assert(env!=NULL);
//创建数据库连接
Connection*conn=env->createConnection("uid","pwd","oracle_svr_name");
//...
// todo:插入自己的代码
//关闭连接
env->terminateConnection(conn);
//释放
Environment::terminateEnvironment(env);
**3.2执行基本的SQL语句**
//创建SQL语句控制句柄
Statement*stmt=conn->createStatement();
**a.执行一般的SQL语句**
stmt->executeUpdate("Create TABLE basket_tab (fruit VARCHAR2(30), quantity NUMBER)");
stmt->executeUpdate("delete basket_tab");
**b.重复利用SQL语句,参数化的SQL语句**
//:1,:2是参数占位符
stmt->setSQL("Insert INTO basket_tab VALUES(:1,:2)");
//第一个参数
stmt->setString(1,"Bananas");
//第二个参数
stmt->setInt(2,5);
stmt->executeUpdate();
**c.一次修改多行数据**
//最多允许的迭代次数，注意这个参数需要在setXXX系列函数之前执行
stmt->setMaxIterations(intmaxIterations);
//指定某个参数的大小，string和byte需要
stmt->setMaxParamSize(intparameterIndex,intmaxParamSize);
for(...)
{
// 第一个参数
stmt->setString(1,"Bananas");
// 第二个参数
stmt->setInt(2,5);
//増加一行记录，类似于ADO的AddNew
pStmt->addIteration();
}
stmt->executeUpdate();
**d.执行查询：获取结果集(记录集)**
ResultSet*rs=stmt->executeQuery("Select
 * FROM basket_tab");
while(rs->next())
{
stringfruit=rs->getString(1);//
 get the first column as string
intquantity=rs->getInt(2);//
 get the second column as int
}
//关闭结果集
stmt->closeResultSet(rs);
**e.执行存储过程**
//指定存储过程countFruit
stmt->setSQL("BEGIN countFruit(:1, :2); END:");
//设置第一个参数
stmt->setString(1,"Apples");
intquantity;
//注册输出参数
stmt->registerOutParam(2,Type::OCCIINT,sizeof(quantity));
//执行此存储过程
stmt->executeUpdate();
//释放SQL语句控制句柄
conn->terminateStatement(Statement*stmt);
**3.3事务**
所有的DDL默认开始并自动提交一个事务
所有的DML默认开始一个事务，且不会自动提交
//可以指定DML是否自动提交
stmt->setAutoCommit(TRUE/FALSE);
//也可以手动提交或回滚。
conn->commit();
conn->rollback();
**3.4异常处理**
try
{
//OCCI程序
}
catch(SQLException&sqlExcp)
{
cerr<<sqlExcp.getErrorCode<<":
 "<<sqlExcp.getErrorMessage()<<endl;
}
catch(exception&excp)
{
cerr<<excp.what()<<endl;
}
# 1.OCCI编程模式
## 1.1.总体流程图
编写OCCI程序的总体流程图，如下所示：
![](https://img-blog.csdn.net/20130910091949750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcnpuaWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1.2.执行SQL语句的流程图
在OCCI中处理SELECT语句跟处理INSERT、UPDATE、CREATE等语句的方法是不一样的，其中SELECT语句是有返回结果，其它语句没有返回结果，所以必须区分SQL语句是否为查询语句，然后按照各自的流程处理。流程图如下所示：
![](https://img-blog.csdn.net/20130910092201093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcnpuaWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.   流程图具体说明
2.1. 创建与终止OCCI环境变量
创建OCCI环境变量
Environment类是OCCI程序的基础类，所有的OCCI对象的建立都是依计Environment对象来创建的，所以Environment对象的建立必须放在第一位，而且也必须是最后一个被终止的。例如：首先创建一个Environment对象env，然后通过env创建一个Connection对象conn，conn是连接了数据库的对象；终止时，却反过来先终止对象conn，再终止对象env。具体Environment对象的创建方法如下所示：
 Environment *env = Environment::createEnvironment();
注意：createEnvironment()方法有以下几个参数模式：
1．在线程环境中运行分为互斥线程(THREADEC_MUTEXED)、并行线程(THREADEC_UMMUTEXED)。
2．使用对象（OBJECT）。
3．利用共享[数据结构](http://lib.csdn.net/base/datastructure)（SHARED）。
终止OCCI环境变量
Environment对象的终止必须是放在最后一位，即等所有其它OCCI对象被终止完后再终止Environment对象。终止Environment对象的方法如下所示：
Environment::terminateEnvironment(env);
2.2. 连接与断开数据库
连接数据库
创建完一个Environment对象env后，可以调用Environment类下的方法createConnection()来创建一个Connection对象conn；通过createConnection()方法我们可以连接到任意一个数据库中。连接数据库的具体方法如下所示：
Connection *conn =env->createConnection(usename,password,[connectionstring]);
参数说明：usename是登录数据库的用户名；
         password是登录数据库用户的密码；
         connectstingr是数据库的连接串(可选)。
断开数据库
当对数据库操作完成之后，我们必须断开程序与数据库的连接，断开数据库的连接方法与创建的方法一样来自Environment类下的terminateConnection()函数。方法如下所示：
env->terminateConnection(conn);
2.3. 创建与断开连接池
创建连接池
对于很多共享服务器或中间层应用程序而言，数据库的连接必须要能被许多线程所使用，如果为每个线程都开一个数据库连接的话，当数量一大效率会明显的降低，所以我们通过创建连接池来处理这样的情况。创建完一个Environment对象env后，可以调用Environment类下的方法createConnectionPool()来创建一个Connection对象connPool；通过createConnectionPool()方法我们可以连接到任意一个数据库中。连接池创建的具体方法如下所示：
ConnectionPool  *connPool= env->createConnectionPool(
const string &poolUseName,
const string &poolPassWord,
const string &connectstring,
unsigned int   minConn,
unsigned int   maxConn,
unsigned int   incrConn);
参数说明：poolUseName是连接池的用户名；
                poolPassWord是连接数据库的密码；
                connectstring是数据库连接串；
                minConn是最小连接数；
                maxConn是最大连接数；
                incrConn是指所有已连接数处于繁忙中且小于最大连接数时，每次增加的连接数。
断开连接池
当对数据库操作完成之后，我们必须断开程序与数据库的连接，断开数据库的连接方法与创建的方法一样来自Environment类下的terminateConnectPool()函数。方法如下所示：
env->terminateConnectPool(connPoll);
2.4. 创建与终止Statement对象
2.4.1. 创建Statement对象
Statement类包含了执行SQL语句的所有方法，是对数据库操作的具体实现。Statement对象stmt的创建是通过调用Connection类下的createStatement()函数实现，所以Statement对象的创建必须是在Connection对象创建之后。createStatement()函数可以带参数或不带参数，如果带参数的话，参数必须是一个SQL语句；如果没有带参数，则在后面必须用Statement类下setSQL()函数为Statement对象赋一个SQL语句。Statement对象的创建方法如下所示：
Statement *stmt =conn->createStatement();
终止Statement对象
当对数据库的操作完成时，我们必须先终止Statement对象，再断开Connection对象。终止Statement对象的方法如下所示：
Conn->terminateStatement(stmt);
3.   执行SQL语句（Statement类）
3.1. 执行SQL语句的方法
执行SQL的所有方法都包含在Statement类下。SQL语句按返回结果分，可以分为两种：一种是有返回值的（查询语句），另外一种是没有返回值的（非查询语句）。一般情况下，在执行查询SQL语句我们使用executeQurey()函数，在执行非查询SQL语句时我们使用executeUpdate()函数。
1． stmt->execute();执行所有非特殊声明的SQL语句。
2．stmt->executeUpdate();执行所有DDL和DML（一条记录）的SQL语句。
3．stmt->executeQurey();执行所有查询SQL语句。
4．stmt->executeArrayUpdate();执行多记录的DML的SQL语句。
例如：
      stmt->executeUpdate(“createtable student (sno number(10),sname varchar2)”)；
      stmt->executeUpdate(“insertinto student (sno,sname)  values(1,’ZHANGSAN’)”);
      stmt->executeQurey(“select  snofrom student ”);
3.2. 执行非查询语句
当我们创建了一个Statement对象stmt后，可以通过Statement类下的方法setSQL()为stmt设置不同的SQL语句。
3.2.1.准备SQL语句
stmt->setSQL(string &sql)；
例如：
stmt->setSQL(“insertinto student(sno,sname)  values(:1,:2)”)
其中：:1、:2是参数，通过给参数赋不同的值来实现。
与之对应的函数getSQL()是取出当前对象对应的SQL语句。
3.2.2. 绑定输入参数值
在Statement类中包含了所有参数的赋值方法，赋值方法set×××(unsignedint paramindex,type values),其中paramindex参数是代表所赋值参数在SQL语句中的第几个参数，×××与type代表中所赋值的类型，values是所赋的值。×××可以是Int、String、Date等等。
例如：
stmt->setString(1,’zhangsan’)；指把’zhangsan’赋值给第一个参数，参数类型为字符串型。
stmt->setInt(2,1009);指把1009赋值给第二个参数，参数类型为整型。
3.2.3.   执行非查询语句
当SQL语句准备好后，就可以执行SQL语句了，方法如下所示：
stmt->executeUpdate();
3.3. 执行查询语句（SELECT）
在执行查询语句时有返回查询结果（可能是一条，也可能是多条），我们必须创建一个ResultSet对象来处理，通过使用ResultSet对象的next()方法我们可以一条一条（或批量）地处理结果集。具体使用如下所示：
3.3.1.   准备SQL语句
详见3.2.1。
3.3.2.   绑定输入参数值
详见3.2.2。
3.3.3.   执行查询语句
ResultSet *rs = stmt->executeQuery();
3.3.4. 定义输出变量
根据SELECT语句查询的列的类型定义对应的变量，此变量用于存储SELECT查询的结果集。
3.3.5. 推进或处理数据
如果结果集为多条记录，我们可以批量或者一条条处理数据，使用ResultSet类下的方法next(unsigned int numRows)。
在Statement类中包含了所有参数的取值方法。取值方法get×××（unsignedint paramindex），其中paramindex参数是代表所取值列在SQL语句中的第几列，×××代表该列对应的类型。×××可以是Int、String、Date等等。
例如：
rs->next(unsignedint numRows);//其中numRows为批量处理的记录行数。
stmt->getInt(paramindex);获取整型参数值,其中paramindex为参数所在的位置；
stmt->getString(paramindex);获取字符型参数值其中paramindex为参数所在的位置；
3.4. 输出参数声明
在调用存储过程时，有时候有返回参数，这些返回参数都必须在调用之前先用Statement类下的registerOutParam()方法声明，声明方法的使用如下所示：
stmt->registerOutParam(paramName,paramType,paramSize);
其中：paramName是参数名或者参数所在位置；
        paramType是参数类型（比如：TYPE::OCCIINT）；
        paramSize是参数的大小。
3.5. 事务提交
3.5.1. 手动提交
当事务的提交设置为手动时，每完成一个事务我们都必须调用Connection类下的commit()或rollback()方法来提交一个事务。使用方法如下所示：
手动提交必须调用以下函数：
Connection::commit();
Connection::rollback();
3.5.2. 自动提交
在对数据库进行操作的过程中，一个事务的提交是用手动还是自动，我们可以调用Statement类下的setAutoCommit()方法来设置。使用方法如下所示：
设置自动提交：
Statement:: setAutoCommit (TRUE);
设置手动提交：
Statement::setAutoCommit(FALSE);
4.  OCCI常用类简要介绍
4.1. Environment类
Environment类提供了管理OCCI对象的内存及其它资源的一个环境,包含以下方法：
   createConnection(const stringusename,const string password,const stringconnstring);指与数据库建立一个连接。
   createConnectionPoll(
                                                                                                                         const  string &poolUseName,
                                                                                                                         const  string &poolPassWord,
                                                                                                                         const  string &connectstring,
                                                                                                                         unsigned  int  minConn,
                                                                                                                         unsigned  int maxConn,
                                                                                                                         unsigned  int incrConn); 创建连接池。
参数说明：poolUseName是连接池的用户名；
                                    poolPassWord是连接数据库的密码；
                                                connectstring是数据库连接串；
                                                minConn是最小连接数；
                                                maxConn是最大连接数；
                   incrConn是指所有已连接数处于繁忙中且小于最大连接数时，每次增加的连接数。
   createEnironment(Mode  mode );创建一个环境对象，其中mode是模式（可选DEFAULT、THREADED_MUTEXED、THREADED_UN-MUTEXED、OBJECT、SHARED值）。
   getCurrentHeapSize();指在同一环境下已经分配的内存数。
   getOCIEnironment();返回一个OCI环境指针。
   terminateConnection(Connection*conn);断开与数据库的连接。
   terminateConnection(ConnectionPool*poolconn);断开与数据库的连接池。
   terminateEnironment(Einironment *env);终止OCCI环境变量。
4.2. Connection类
Connection类提供了C++程序与指定数据库的连接，包含以下方法：
   changePassword(const string&user,const string &oldPassword,conststring &newPassword);改变当前用户的密码。
   Commit();提交事务。
   createStatement(const string&sql=””);创建一个Statement对象。
   flushCache();
   rollback();事务的回滚。
   terminateStatement(Statement*stmt).终止一个Statement对象，并释放它占有的所有资源。
4.3. ConnectionPool类
ConnectionPoll类提供了C++程序与指定数据库间的连接池，包含以下方法：
   createConnection(const string&username,const string &password);创建一个连接。
   getBusyConnections();返回连接池中被占用的连接数量。
   getMaxConnections();返回连接池的最大连接数。
   getMinConnections();返回连接池的最小连接数。
   getOpenConnections();返回连接池中已经被打开的连接数。
   getPollName();返回连接池的名称。
   getTimeOut();返回连接池的超时时间。
   setErrorOnBusy();设置异常，当连接池处于繁忙或达到最大连接数后。
   serPoolSize(unsigned intminConn,unsigned int maxConn,unsigned intincrConn);设置连接池的大小，其中参数minConn是最小连接数，maxConn是最大连接数，incrConn是递增连接数。
                 setTimeOut();设置超时时间。
                 terminateConnection(Connection  *conn);断开连接。
4.4. Statement类
                   Statement对象主要被用来执行SQL语句，它可以运行一个查询语句返回一个结果集也可以运行UPDATE、DELETE等SQL语句。
   addIteration();增加一个循环。
   closeResultSet(ResultSet *rs);关闭一个结果集。
   closeStream(Stream  *stream);关闭一个已定义的流。
   execute(const string &sql =“”);执行一个SQL语句，如果前面使用的setSQL()方法，则参数sql可以为空。
   executeArrayUpdate(unsigned intarrayLength);同时修改多条记录，前面必须使用了setDataBuffer()方法设置一个数量或设置了一个流（stream）。
   executeQuery();执行一个查询语句。
   executeUpdate();执行DDL、DML语句，如：INSERT、UPDATE、DELETE、CREATE等等。
   getAutoCommit();返回是否事务自动提交。
   getDate(unsigned int paramindex);返回DATE型数据。
                 getDouble(unsignedintparamindex);返回DOUBLE型数据。
                 getFloat(unsigned intparamindex);返回FLOAT型数据。
                 getInt(unsignedintparamindex);返回INT型数据。
                 getNumber(unsigned intparamindex);返回NUMBER型数据。
                 getRef(unsignedintparamindex);返回REF参数值。
                 getResultSet();得到一个结果集（ResultSet）。
                 getSQL();返回当前的SQL语句。
                 getString(unsignedintparamindex);返回STRING型数据。
                 getUpdateCount();返回被更新的数量。
                 isNull(unsignedintparamindex);判断第X个值是否为空。
                 isTruncated(unsignedintparamindex);判断第X个值是否被截取。
                 setAutoCommit(boolautocommit);设置事务是否自动提交。
                 setDouble(unsignedintparamindex,double x);赋一个double型的值。
                 setFloat(unsignedintparamindex,float x);赋一个float型的值。
                 setInt(unsignedintparamindex,int x);赋一个INT型的值。
                 setNull(unsigned intparamindex,TYPEtype);某参数设置为空。
                 setNumber(unsignedintparamindex,const number &x);赋一个Number型的值。
                 setObject(unsignedintparamindex,PObject *x);赋一个PObject型的值。
                 setString(unsignedintparamindex,const string &x);赋一个string型的值。
                 setSQL(const string &sql);设置一个SQL语句。
                 Status();返回当前的状态。
4.5. ResultSet类
ResultSet类是查询数据表的结果集，它包含以下方法：
   cancel();取消结果集。
   closeStream(Stream  *stream);关闭一个流。
   getDouble(unsigned intcolIndex);返回double型的值。
   getFloat(unsigned intcolIndex);返回float型的值。
   getInt(unsigned int colIndex);返回int型的值。
   getMaxColumnSize(unsigned intcolIndex);返回某列包含的数据量。
   getNumber(unsigned intcolIndex);返回number型的数据。
   getPObject(unsigned intcolIndex);返回PObject型的数据。
   getRowid(unsigned intcolIndex);返回rowid值。
                 getString(unsignedintcolIndex);返回string型的数据。
                 isNull(unsigned int colIndex);判断值是否为空。
                 isTruncated(unsignedintcolIndex);判断值是否被删。
                 next(unsigned int numRows);取下numRows条记录。
                 status();返回当前状态。
4.6. PObject类
PObject类包含以下方法：
   fush();
   Connection *getConnection();从一个实例化的永久对象中返回连接。
   isLocked();判断永久对象是否被锁（TRUE或FALSE）。
   isNull();判断永久对象是否为空（TRUE或FALSE）。
   Lock(PObject::LockOption lock_option);在数据库中锁住一个对象，其中lock_option取值为OCCI_LOCK_WAIT、OCCI_LOCK_NOWAIT。
   markDelete();删除数据库中的永久对象。
   markModified();修改永久对象。
   PObject &operator(constPObject &obj);赋值（对象）。
   operator delete(void*obj,size_t size);删除对象，但不能删除数据库中的对象。
                 *operator new(size_t size);创建临时对象。
                 *operator new(size_tsize,constConnection *x,const string &tablename,const char *type_name);创建永久对象。
                 pin();
                 setNull();设置对象为空。
                unmark();不标记永久对象为删除或修改。
                 unpin(UnpinOption mode);其中，mode的取值为OCCI_PINCOUNT_RESET、OCCI_PINCOUNT_DECR。
4.7. SQLException类
SQLException类提供错误代码及其错误信息，包含以下方法：
1． getErrorCode();返回数据库中的错误编码。
2． getMessage();返回错误信息，没有则为空。
4.8. Date类
Date类包含以下方法：
   addDays(int i);返回增加i天后的日期。
   addMonths(int i);返回增加i个月后的日期。
   intervalDS daysBetween(constDate &d);返回与d相差的天数。
   getDate(int &year,unsignedint &month,unsigned int&day,unsigned int &hour,unsigned int&minute,unsigned int&second );返回分年月日小时分秒格式。
    getSystemDate(const Environment*env);返回系统日期。
   isNull();判断是否为空；
   lastDay();返回当月的最后一天日期。
   setDate(int&year=1,unsigned int &month=1,unsigned int&day=1,unsigned int&hour=0,unsigned int &minute=0,unsigned int&second=0);设置日期值。
   setNull();设置为空。
4.9. Number类
Number类包含以下常用方法：
         abs();返回绝对值。
         floor();向下取整。
         isNull();判断是否为空。
         operator+(constNumber&a,const Number &b);两数相加。
         operator-(const Number&a,const Number &b);两数相减（a-b）。
         operator*(constNumber&a,const Number &b);两数相乘。
         operator/(constNumber&a,const Number &b);两数相除。
         operator%(constNumber&a,const Number &b);求余。
         operator<(constNumber&a,const Number &b);a是否小于b。
                      operator<=(constNumber&a,const Number &b);a是否小于等于b。
                      operator>(constNumber&a,const Number &b);a是否大于b。
                      operator>=(constNumber&a,const Number &b);a是否大于等于b。
                      operator==(constNumber&a,const Number &b);a与b是否相等。
                      operator!=(constNumber&a,const Number &b);判断a是否不等于b。
                      operator char();将number型转化为char型。
                      operator signed char();将number型转化为有符号char型。
                      operator double();将number型转化为double型。
                      operator float();将number型转化为float型。
                      operator int();将number型转化为int型。
                      operator long();将number型转化为long型。
                      operator short();将number型转化为short int型。
                      setNull();设置为空。
                      trunk();取整。
5.   附录：代码示例
5.1. 查询流程处理示例
下面给出了一个查询的例子qrystudent.cpp，通过使用OCCI接口函数查询数据库表student的信息（学号，学生姓名）。
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main ()
{
Environment *env;
Connection *conn;
Statement *stmt;
ResultSet *rs;
string username = “ipd”;
string password = “ipd”;
string connstring = “ora9i”;
string sql,strname;
int isno;
env = Environment::createEnvironment(); //创建一个环境变量
conn =env->createConnection(username,password,connstring);//创建一个数据库连接对象
stmt = conn->createStatement(); //创建一个Statement对象
sql = “ SELECT sno,snameFROM student “;  //拼接一个SQL语句
stmt->setSQL(sql); //设置SQL语句到Statement对象中
try {
      rs=  stmt->executeQuery();//执行SQL语句
      while(rs->next()) { //用循环，一条一条地取得查询的结果记录
              isno= rs->getInt(1);//取出第一列的值（对应为int型）
              strname=rs->getString(2);//取出第二列的值（对应为string型）
              cout<< isno << “  AND  “<< strname << endl;
      }
      cout<< “SELECT―― SUCCESS”<< endl;
} catch (SQLException ex) {
      cout<< “ Error Number : “<< ex.getErrorCode() << endl; //取出异常代码
      cout<< ex.getMessage() << endl; //取出异常信息
}
conn->terminateStatement(stmt); //终止Statement对象
env->terminateConnection(conn); //断开数据库连接
Environment::terminateEnvironment(env); //终止环境变量
return 1;
}
5.2. 非查询流程处理示例
下面给出了一个insert例子inserttab.cpp，通过使用OCCI接口函数往数据库表student中插入信息（学号，学生姓名）。
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main ()
{
Environment *env;
Connection *conn;
Statement *stmt;
string username = “ipd”;
string password = “ipd”;
string connstring = “ora9i”;
string sql;
env = Environment::createEnvironment(); //创建一个环境变量
conn=env->createConnection(username,password,connstring); //创建一个数据库连接对象
stmt = conn->createStatement(); //创建一个Statement对象
sql = “ INSERT INTOstudent(sno,sname)  VALUES ( :x,:y) “; //拼接SQL语句
stmt->setSQL(sql); //设置SQL语句到Statement对象中
try {
      stmt->setInt(1,10001);//给第一个参数x赋值
      stmt->setString(2,”zhangsan”);//给第二个参数y赋值
      stmt->executeUpdate();//执行SQL语句
      cout<< “INSERT―― SUCCESS”<< endl;
} catch (SQLException ex) {
      cout<< “ Error Number : “<< ex.getErrorCode() << endl; //获得异常代码
      cout<< ex.getMessage() << endl; //获得异常信息
}
conn->terminateStatement(stmt); //终止Statement对象
env->terminateConnection(conn); //断开数据库连接
Environment::terminateEnvironment(env); //终止环境变量
return 1;
}
5.3. 批量处理非查询语句
在批量处理非查询语句时，首先必须使用m_stmt->setMaxIterations(unsigned int maxnum)方法设置最大的批处理数；然后使用setMaxParamSize(unsignedint paramindex,int paramsize)方法设置对应参数的大小，对于有固定长度的参数可以不必重新设置，如int,double；最后循环绑定参数使用set×××()方法，每循环一次必须使用addIteration()方法再进行下一次循环，直到最后一次。例子如下所示：
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main ()
{
  Environment *env;
  Connection *conn;
  Statement *stmt;  
  string username = "bill";
  string password = "bill";
  string connstring = "jftest";
  string sql;
  intisno[2]={21,22},i,iage[2]={18,19};
  stringcsname[2]={"namea","nameb"};
  try {
      env = Environment::createEnvironment();
      conn = env->createConnection(username,password,connstring);
      stmt = conn->createStatement();
      stmt->setAutoCommit(false);      
      sql = " INSERT INTO student (sno,sname,age)  VALUES (:x,:y,:z) ";     
      m_stmt->setSQL(sql);
      m_stmt->setMaxIterations(2);
      m_stmt->setMaxParamSize(1,sizeof(int));
      m_stmt->setMaxParamSize(2,sizeof(string));
      m_stmt->setMaxParamSize(3,sizeof(int));
      cout<<"setMaxIterations success !"<<endl;
      //cout<<"getMaxIterations ="<<m_stmt->getMaxIterations()<<endl;
      for (i=0;i<2;i++)
      {
          m_stmt->setInt(1,isno[i]);
          m_stmt->setString(2,csname[i]);
          m_stmt->setInt(3,iage[i]);     
           //cout <<isno[i]<< " and "<<csname[i]<< " and " << iage[i] <<endl;
          if (i!=1) m_stmt->addIteration();
      }
      cout<<"getMaxIterations="<<m_stmt->getMaxIterations()<<endl;
          cout<<"setsuccess"<<endl;
          inum = m_stmt->executeUpdate();
          m_conn->commit();
          cout << "INSERT ¨D¨DSUCCESS" << endl;
   }catch (SQLException ex) {
          inum = m_stmt->getUpdateCount();
          cout << "ON" <<inum+1 << " shi fail!" << endl;
          cout << " INSERT Error Number: "<<ex.getErrorCode() << endl;
          cout << ex.getMessage() <<endl;
          m_conn->rollback();
          return 0;
    }
  conn->terminateStatement(stmt);
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
  cout << "PROGRAM IS END ." << endl;
  return 1;
}
5.4. 查询语句中批量取数
在批量取数时，使用setDataBuffer（）方法来设置接收数据缓冲区大小，例子如下所示：
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main ()
{
  Environment *env;
  Connection *conn;
  Statement *stmt;
  ResultSet *rs;
  string username = "bill";
  string password = "bill";
  string connstring = "jftest";
  string sql;
  int i,numrows=5,isnum,ilnum,ipos=0;
  int isno[5],vsno[20],ii=0,x[20];
  ub2 isnolen[5],snamelen[5];
  char sname[5][20],vsname[20][20];
   for (i=0;i<20;i++)
    {
      isnolen[i]=sizeof(isno[i]);
                             snamelen[i]=strlen(sname[i]+1);
    }
  cout << "PROGRAM START ..." << endl;
  try {
      env = Environment::createEnvironment();
      conn = env->createConnection(username,password,connstring);
      stmt = conn->createStatement();
      sql = " SELECT sno,sname FROM student ";
      stmt->setSQL(sql);
          rs = stmt->executeQuery();
          rs->setDataBuffer(1,isno,OCCIINT,sizeof(int));
          rs->setDataBuffer(2,sname,OCCI_SQLT_STR,sizeof(sname[0]),snamelen);
          ilnum = rs->getNumArrayRows();
          cout << " ilnum = "<< ilnum <<endl;
          while (rs->next(numrows)) {
                  isnum =rs->getNumArrayRows();
               cout << " isnum ="<< isnum <<endl;
                 for(i=0;i<numrows;i++)
              {
                  vsno[ipos] = isno[i];
                  vsname[ipos] = sname[i];
                  ipos = ipos+1;
              }
          }
      ilnum = rs->getNumArrayRows();
          cout << " ilnum = "<< ilnum <<endl;
          for(i=0;i<ilnum-isnum;i++)
      {
          vsno[ipos] = isno[i];
          vsname[ipos] = sname[i];
          ipos = ipos+1;
      }
      cout << "SELECT RESULT IS ..." << endl;
      for(i=0;i<ilnum;i++)
      {
          cout<<vsno[i]<<"AND"<<vsname[i]<<endl;
      }
      cout << "SELECT RESULT END !" << endl;
          cout << "SELECT ¨D¨DSUCCESS" << endl;
   }catch (SQLException ex) {
          cout << " Error Number :"<< ex.getErrorCode()<< endl;
          cout << ex.getMessage() <<endl;
    }
  conn->terminateStatement(stmt);
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
  cout << "PROGRAM END !" << endl;
  return 1;
}
5.5. MERGE语句的使用
在使用MEGER语句时，USING部分的取值应该使用表或者SELECT param1,param2FROM DUAL,其中param1,param2是常量值，可以通过数组赋值，并且每列值都应该取别名，在后面的使用中就只用别名。例子如下所示：
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main ()
{
  Environment *env;
  Connection *conn;
  Statement *stmt;
  string username = "bill";
  string password = "bill";
  string connstring = "jftest";
  string sql;
  int isno[2]={21,22},i,iage[2]={18,19};
  try {
      env = Environment::createEnvironment();
      conn = env->createConnection(username,password,connstring);
      stmt = conn->createStatement();
      stmt->setAutoCommit(false);
      sql = " merge into student a ";
      sql += "    using (select:xbid,:y bage from dual) b ";
      sql += "    on (a.sno =b.bid)";
      sql += "    when matchedthen";
      sql += "    update seta.age=a.age+b.bage ";
      sql += "    when notmatchedthen ";
      sql += "   insert(a.sno,a.age) ";
      sql += "   values(b.bid,b.bage) ";
      stmt->setSQL(sql);
      stmt->setMaxIterations(2);
      stmt->setMaxParamSize(1,sizeof(int));
      //stmt->setMaxParamSize(2,sizeof(string));
      cout<<"setMaxIterations success !"<<endl;
      //cout<<"getMaxIterations ="<<stmt->getMaxIterations()<<endl;
      for (i=0;i<2;i++)
      {
          stmt->setInt(1,isno[i]);
          stmt->setInt(2,iage[i]);
          if (i!=1) stmt->addIteration();
      }
      cout<<"getMaxParamSize ="<<stmt->getMaxParamSize(2)<<endl; 
      cout<<"getMaxIterations="<<stmt->getMaxIterations()<<endl;
          cout<<"setsuccess"<<endl;
          stmt->executeUpdate(); //?′DDSQLó???
          conn->commit();
          cout << "INSERT ¨D¨DSUCCESS" << endl;
   }catch (SQLException ex) {
          cout << " Error Number :"<< ex.getErrorCode()<< endl;
          cout << ex.getMessage() <<endl;
          conn->rollback();
    }
  conn->terminateStatement(stmt);
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env)
  cout << "PROGRAM IS END ." << endl;
  return 1;
}
