# 使用OTL进行数据库编程  - 深之JohnChen的专栏 - CSDN博客

2010年03月11日 16:19:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1563标签：[数据库																[oracle																[microsoft																[insert																[exception																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)


操作环境：
1． 操作系统：Windows XP Professional with SP2。
2． 编程环境：Visual C++ 6.0 with SP6。
3． 数据库环境：Access 2003。
OTL简介：
OTL 是 Oracle, Odbc and DB2-CLI Template Library 的缩写，是一个C++编译中操控关系数据库的模板库，它目前几乎支持所有的当前各种主流数据库，例如Oracle, MS SQL Server, Sybase, Informix, MySQL, DB2, Interbase / Firebird, PostgreSQL, SQLite, SAP/DB, TimesTen, MS ACCESS等等。OTL中直接操作Oracle主要是通过Oracle提供的OCI接口进行，进行操作DB2数据库则是通过CLI接口来进行，至于MS的数据库和其它一些数据库，则OTL只提供了ODBC来操作的方式。当然Oracle和DB2也可以由OTL间接使用ODBC的方式来进行操纵。
在MS Windows and Unix 平台下，OTL目前支持的数据库版本主要有：Oracle 7 (直接使用 OCI7), Oracle 8 (直接使用 OCI8), Oracle 8i (直接使用OCI8i), Oracle 9i (直接使用OCI9i), Oracle 10g (直接使用OCI10g), DB2 (直接使用DB2 CLI), ODBC 3.x ,ODBC 2.5。OTL最新版本为4.0，参见[http://otl.sourceforge.net/](http://otl.sourceforge.net/)，下载地址[http://otl.sourceforge.net/otlv4_h.zip](http://otl.sourceforge.net/otlv4_h.zip)。
优点：
      a. 跨平台
      b. 运行效率高，与C语言直接调用API相当
      c. 开发效率高，起码比ADO.net使用起来更简单，更简洁
      d. 部署容易，不需要ADO组件，不需要.net framework 等
缺点：
      a. 说明文档以及范例不足够丰富（暂时性的）
     其实现在它提供有377个使用范例可参考，下载地址：[http://otl.sourceforge.net/otl4_examples.zip](http://otl.sourceforge.net/otl4_examples.zip)。
建立数据源
1．依次点击“开始->控制面板”，打开“控制面板”界面，双击“管理工具”，然后再双击“数据源(ODBC)”，就打开了“ODBC数据源管理器”，选择“系统DSN”。
2．单击“添加”，弹出“创建新数据源”对话框，选择“Microsoft Access Driver(*.mdb)”。
3．点击“完成”，弹出“ODBC Microsoft Access安装”对话框，单击“创建”，开始创建数据库，弹出“新建数据库”对话框，添加数据库名称my_db和选择数据库存放目录，单击“确定”，创建完成，然后添加数据源名：my_db。点击“确定”。
4．然后在系统数据源中就有我们刚才添加的数据源。
5．单击“确定”，完成数据源的创建。
OTL编程
下面我们用一个实例来说明：
1． 创建数据表：TestTable ( ColumA int , ColumB varchar(50),ColumC varchar(50) )
2． 插入100条数据，ColumA 为数据的 id 范围：0-99 ， ColumB=”Test Data %d” , 其中 %d=id 。
3． 删除表中ColumA 中小于10和大于90的数据。
4． 将ColumA为3的倍数的记录中ColumC更新为ColumB的内容。
具体代码为：

#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OTL_ODBC // 编译 OTL 4.0/ODBC
// #define OTL_ODBC_UNIX // 如果在Unix下使用UnixODBC，则需要这个宏
#include "otlv4.h" // 包含 OTL 4.0 头文件
otl_connect db; // 连接对象

//此函数完成插入100条数据，ComulA为数据的id，范围为0-99，
//ColumB="Test Data %d",其中%d=id
void insert()  
// 向表中插入行
{ 
// 打开一个通用的流，以模板的方式向表中插入多项数据
otl_stream
  o(1, // 流的缓冲值必须设置为1
  "insert into TestTable values(:f1<int>,:f2<char[50]>,:f3<char[50]>)", 
  // SQL 语句
  db  // 连接对象
  );
char tmp1[32];
char tmp2[30];

for(int i=0;i<100;++i){
  sprintf(tmp1,"Test Data %d",i);
  sprintf(tmp2,"");
  o<<i<<tmp1<<tmp2;
}

}
//此函数完成删除表中ColumA中小于10和大于90的数据
void delete_rows()
{ 
long rpc=otl_cursor::direct_exec(db,"delete from TestTable where ColumA<10 or ColumA>90");
// rpc是作用效果的返回值，otl_cursor::direct_exec为直接执行sql语句
cout<<"Rows deleted: "<<rpc<<endl;
}

//此函数完成将ColumA为3的倍数的记录中ColumC更新为ColumB的内容
void update()
// 更新表
{
otl_stream 
  o(1, // 缓冲值
  "UPDATE TestTable "
  "   SET ColumC=:f2<char[50]> "
  " WHERE ColumA=:f1<int>", 
        // UPDATE 语句
  db // 连接对象
  );
otl_stream c(1,"select ColumB from TestTable where ColumA=:f3<int>",db);
char temp[10];
for(int i=10;i<91;i++)
{
  if(i%3==0)
  {
   c << i;
   c >> temp;
   o << temp << i;
  }
}

}

int main()
{
otl_connect::otl_initialize(); // 初始化 ODBC 环境
try{

  db.rlogon("UID=scott;PWD=tiger;DSN=my_db"); // 连接到 ODBC
  //或者使用下面的连接语句方式。 
  //  db.rlogon("[scott/tiger@firebird](mailto:scott/tiger@firebird)"); // connect to ODBC, alternative format
  // of connect string 

  otl_cursor::direct_exec
   (
   db,
   "drop table TestTable",
   otl_exception::disabled // disable OTL exceptions
   ); // drop table

  //这里完成表的创建
  otl_cursor::direct_exec
   (
   db,
   "create table TestTable(ColumA int, ColumB varchar(50),ColumC varchar(50))"
   );  // create table

  insert(); // insert records into the table
  //  update(10); // update records in the table
  delete_rows();
  update();

}

catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.sqlstate<<endl; // print out SQLSTATE message
  cerr<<p.var_info<<endl; // print out the variable that caused the error
}

db.logoff(); // disconnect from the database

return 0;

}

[](http://blog.csdn.net/freezezdj/archive/2009/10/26/4729301.aspx)


