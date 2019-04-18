# 《Linux程序设计》——MsSQL - 一世豁然的专栏 - CSDN博客





2014年09月30日 16:11:12[一世豁然](https://me.csdn.net/Explorer_day)阅读数：961标签：[数据库																[c语言																[linux																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)





一、MsSQL的安装和配置（网络查询）

[MySQL安装和配置](http://blog.csdn.net/xiamizy/article/details/2072525)








二、MySQL管理

 1、命令

   注、除mysqlshow命令外，所有的MySQL命令都接受（P270  表8-1）三个标准参数。

  1）、myisamchk命令

         A、myisamchk工具是设计用来检查和修复使用默认MYISAM表格式额任何数据表，MYISAN表格式由MySQL自身支持。

         B、myisamchk命令参数（P271  表8-2）

   2）、mysql命令

          A、mysql命令集（P271  表8-3）

   3）、mysqladmin命令

          A、快速进行MySQL数据库管理的主要工具。

          B、mysqladmin参数（P272  表8-4）

   4）、mysqlbug命令

          A、生成一个用于发送给MySQL维护者的错误报告。

   5）、mysqldump命令

          A、以SQL命令集的形式将部分或整个数据库导出到一个单独文件中，该文件能被倒入MySQL或其他的SQL RBMS。默认情况下，mysqldump将数据发送到标准输出。

          B、mysqldump命令参数。

   6）、mysqlimport命令

          A、用于批量将数据导入到一个表中。惟一的参数要求是一个文件名和一个数据名。

   7）、mysqlshow命令

          A、不提供参数，它列出所有可用的数据库。

          B、以一个数据库为参数，它列出该数据库中的表。

          C、以数据库和表名为参数，它列出表中的列。

          D、以数据库、表和列为参数，它列出指定列的详细信息。




 2、创建用户并赋予权限

   1）、grant命令

          A、常规格式：**grant  <privilege>   on  <object>  to  <user>  [identified  by  user-password]  [with  grant  option]**

          B、特权值表（P274  表8-6）

   2）、revoke命令

          A、格式：**revoke  <a_privilege>  on  <an_odject>  from  <a_user>**

          B、revoke剥夺用户权限，但不能用来删除用户。




 3、密码（书中例程）




 4、创建数据库（书中例程）




 5、数据类型

   1）、布尔类型

   2）、字符类型（P277  表8-7）

   3）、数值类型（P278  表8-8）

   4）、时间类型（P278  表8-9）




 6、创建表

   1）、语法：**CREATE  TABLE  <table_name>(**

**                          column  type  [NULL  |  NOT  NULL]  [AUTO_INCREMENT]  [PRIMARY  KEY]**

**                          [，.  .  .]**

**                          [，PRIMARY  KEY (  column  [，.  .  .]  )  ]**

**                          )**

    2）、关键字（P279  表8-10）




 7、图形化工具

   1）、MySQL查询游览器

    2）、MySQL管理器







三、使用C语言访问MySQL数据

 1、链接例程

   1）、mysql_init函数

**  #include<mysql.h>**

**         MYSQL  *mysql_init(MYSQL  *);**

         A、初始化进程。

         B、通常传递 给NULL给这个例程，它会返回一个指向新分配的链接句柄的指针。如果传递一个已有的结构，它会被重新初始化。

   2）、mysql_real_connect函数

**#include<mysql.h>**

**          MYSQL  *mysql_real_connect(**

**          MYSQL  *connection,**

**         const  char  *server_host, **

**         const  char  *sql_user_name,**

**         const  char  *sql_password, **

**         const  char  *db_name, **

**         unsigned  int  port_number,**

**        const  char  *unix_socket_name,**

**        unsigned  int  flags);**

        A、用于连接。

        B、指针connection必须指向已经被mysql_init初始化过的结构。

        C、server_host既可以是主机名，也可以是IP地址。

        D、sql_user_name和sql_password：如果登录名为NULL，则假设登录名为但钱Linux用户的登录ID。如果密码为NULL，只能访问服务器上无需密码就可访问的数

           据。

       E、port_number和unix_socket_name应该分别为0和NULL，除非你改变了MYSQL安装的默认设置。

       F、flags参数用来对一些定义的位模式进行OR操作，使得改变使用协议的某些特性。

       G、使用链接完之后，它将返回NULL。mysql_error函数可以提供有帮助的信息。

  3）、mysql_close函数

** #include<mysql.h>**

**          void  mysql_close(MYSQL  *connection);**

          A、关闭连接

   4）、mysql_options函数

**include<mysql.h>**

**          mysql_options(MYSQL  *connection, enum  option_to_set,  const  char  *argument);**

          A、此函数只能在mysql_init和mysql_real_connect之间使用。

          B、此函数可以设置由mysql_close函数关闭连接释放出mysql_real_connect函数的选项。

          C、常用选项（P284  表8-11）




 2、错误处理

** #include<mysql.h>**

**     unsigned  mysql_errno(MYSQL  *connection);**

**     char  *mysql_error(MYSQL  *connection);**

     A、可以通过调用mysql_errno并传递链接结构来获得错误码，它通常都是非0值。如果未设定错误码，它将返回0。

     B、*mysql_error提供有意义的文本信息。




 3、执行SQL语句

  1）、mysql_query函数

**# include<mysql.h>**

**         int  mysql__query(MYSQL  *connection,  const  char  *query);**

        A、此函数接受链接结构指针和文本字符串形式的有效SQL语句。

        B、如果成功。它返回0.


   2）、不返回数据的SQL语句（**UPDATE，DELETE，INSERT**）——查看书中例程

**#include<mysql.h>**

**          my_ulonglong  mysql_affected_rows(MYSQL  *connection);**

          A、检查受查询影响额函数。

   3）、发现插入内容——查看书中例程

   4）、返回数据的语句

       A、一次提取所有数据的函数

          I、**MYSQL_RES  *mysql_store_result(MYSQL  *connection);**

             提取数据，这个函数将立刻保存在客户端中返回的所有数据。

             需要在成功使用mysql_query之后使用此函数。

             它返回一个指向结果集结构的指针，如果失败则返回NULL。

         II、**my_ulonglong  mysql_num_rows(MYSQL_RES  *result);**

            此函数接受由mysql_store_result返回的结果结构，并返回结果集中的行数。

            如果mysql_store_result调用成功，mysql_num_rows始终是成功的。

        III、**MYSQL_ROW  mysql_fetch_row(MYSQL_RES  *result);**

            此函数从使用mysql_store_result得到的结果结构中提取一行，并把它放到一个行结构。

            当数据用完或发生错误时返回NULL。

       IV、**void  mysql_data_seek(MYSQL_RES  *result,my_ulonglong  offsef);**

            此函数用来在结果集中进行跳转，设置将会被下一个mysql_fetch_row操作返回的行。

           参数offsef的值是一个行号，它必须在0到结果集总行数减1的范围内。传递0将会导致下一个mysql_fetch_row调用返回结果集中的第一行。

       V、**MYSQL_ROW_OFFSEF  mysql_row_tell(MYSQL_RES  *result);**

          此函数返回一个偏移值，它用来表示结果集中的当前位置。它不是行号，不能把它用于mysql_data_seek。

       VI、**MYSQL_ROW_OFFSEF  mysql_row_tell(MYSQL_RES  *result，MYSQL_ROW_OFFSEF  offsef);**

           可以使用mysql_row_tell的返回值。这将在结果集中移动当前位置，并返回之前的位置。

      VII、**void  mysql_free_result(MYSQL_RES  *result);**

           完成对结果集的操作后，必须调用此函数来让MySQL库清理它分配的对象。

   B、依次提取一行数据

       I、 **MYSQL_RES  *mysql_use_result(MYSQL  *connection);**

            此函数用于逐行提取数据。

            遇到错误时返回NULL，如果成功，它将返回指向结果集对象的指针。与mysql_store_result不同之处在于它未将提取数据放到它初始化的结果中。

  5）、处理返回的数据

       I、**unsigned  int  mysql_field_count(MYSQL  *connection);**

          提供了一些查询结果的基本信息，它接受连接对象，并返回结果集中的字段（列）数目。

          对于通用查询处理模块或任何构造查询的情况，这个函数是非常有用的。

      II、**MYSQL_FIELD  *mysql_fetch_field(MYSQL_RES  *result);**

         此函数可以同时将元数据和数据提取到一个新的的结构中。

         需要重复调用此函数，直到返回表示数据结束的NULL值为至。

         MYSQL_FIELD结构成员（P298  表8-12）

    III、**MYSQL_FIELD_OFFSEF  mysql_field_seek(MYSQL_RES  *result, MYSQL_FIELD_OFFSEF  offsef);**

        此函数来覆盖当前额字段编号，该编号会随每次mysql_field_seek调用而自动增加。

       如果给参数offsef传递值为0，将跳回第一列。

  6）、更多函数（P301  表8-13）](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)




