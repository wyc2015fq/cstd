# LINUX下用C连接MYSQL - gauss的专栏 - CSDN博客
2015年04月17日 01:23:16[gauss](https://me.csdn.net/mathlmx)阅读数：228
以下变量类型在MySQL的库当中定义。我们需要这些变量是为了使用MySQL的函数。这些变量有详细的解释，但是这些解释对于写代码来说并不重要。 
**MYSQL**
以下代码块是用来连接数据库的通讯过程
typedef struct st_mysql {
  NET           net;            /* Communication parameters */
  gptr          connector_fd;   /* ConnectorFd for SSL */
  char          *host,*user,*passwd,*unix_socket,
                *server_version,*host_info,*info,*db;
  unsigned int  port,client_flag,server_capabilities;
  unsigned int  protocol_version;
  unsigned int  field_count;
  unsigned int  server_status;
  unsigned long thread_id;      /* Id for connection in server */
  my_ulonglong affected_rows;
  my_ulonglong insert_id;       /* id if insert on table with NEXTNR */
  my_ulonglong extra_info;              /* Used by mysqlshow */
  unsigned long packet_length;
  enum mysql_status status;
  MYSQL_FIELD   *fields;
  MEM_ROOT      field_alloc;
  my_bool       free_me;        /* If free in mysql_close */
  my_bool       reconnect;      /* set to 1 if automatic reconnect */
  struct st_mysql_options options;
  char          scramble_buff[9];
  struct charset_info_st *charset;
  unsigned int  server_language;
} MYSQL;
**MYSQL_RES**
这段代码返回查询结果的行。返回的数据称为“数据集”
typedef struct st_mysql_res {
  my_ulonglong row_count;
  unsigned int  field_count, current_field;
  MYSQL_FIELD   *fields;
  MYSQL_DATA    *data;
  MYSQL_ROWS    *data_cursor;
  MEM_ROOT      field_alloc;
  MYSQL_ROW     row;            /* If unbuffered read */
  MYSQL_ROW     current_row;    /* buffer to current row */
  unsigned long *lengths;       /* column lengths of current row */
  MYSQL         *handle;        /* for unbuffered reads */
  my_bool       eof;            /* Used my mysql_fetch_row */
} MYSQL_RES;
**MYSQL_ROW**
这个结构是数据行的一个安全表示法。你无法使用以空字符结束的串，因为数据在这个串可以是二进制, 也许没有包括任何字符。
typedef struct st_mysql_field {
  char *name;                   /* Name of column */
  char *table;                  /* Table of column if column was a field */
  char *def;                    /* Default value (set by mysql_list_fields) */
  enum enum_field_types type;   /* Type of field. Se mysql_com.h for types */
  unsigned int length;          /* Width of column */
  unsigned int max_length;      /* Max width of selected set */
  unsigned int flags;           /* Div flags */
  unsigned int decimals;        /* Number of decimals in field */
} MYSQL_FIELD;
**my_ulonglong**
该类型用于行数，mysql_affected_rows() 、mysql_num_rows()和mysql_insert_id() 。该类型提供范围0 到1.84.e19 的支持。在一些系统, 试图打印出my_ulonglong类型的值是不行的.要显示这样的值, 使用%lu printf 格式，把它转换成unsigned long类型就行了。例如:
printf(Number of rows: %lu/n", (unsigned long) mysql_num_rows(result));
typedef unsigned long my_ulonglong;
## 连接MySQL，查询数据
现在假设MySQL已安装, 用户和数据表在数据库被创造。以防有什么不明问题的情况, 请参考[www.mysql.com](http://www.mysql.com/) 网站。
前面已经说过，MySQL的库文件在mysqlclient。因此在编译MySQL程序的时候有必要加上*-lmysqlclient*编译选项。MySQL的头文件在/usr/include/mysql目录下(根据Linux的发行版本的不同，这个目录也有所不同)，因此你的程序头部看起来有点这个样子：
＃i nclude <mysql/mysql.h>
MySQL的变量类型和函数都包含在这个头文件当中
然后，我们需要创建连接数据库的变量，可以简单地这么做：
MYSQL *mysql;
在连接数据库之前，我们要调用以下函数初始化这个变量：
mysql_init(MYSQL *mysql)
然后
MYSQL * STDCALL mysql_real_connect(MYSQL *mysql,
                                   const char *host,
                                   const char *user,
                                   const char *passwd,
                                   const char *db,
                                   unsigned int port,
                                   const char *unix_socket,
                                   unsigned int clientflag);
该函数被调用连接到数据库。host是MySQL服务器的主机名，user是登录的用户名，passwd是登录密码，db是要连接的数据库，port是MySQL服务器的TCP/IP端口，unix_socket是连接类型，clientflag是MySQL运行成ODBC数据库的标记。在这篇文章当中该标记设成0，连接寻建立后，这个函数返回0。
现在可以连接数据库，进行查询了：
char *query;
使用这个字符串我们可以创立任何SQL查询语句进行查询。执行这个查询的函数是：
int STDCALL mysql_real_query(MYSQL *mysql,
                             const char *q,
                             unsigned int length);
mysql是我们前面用过的变量，q是SQL查询语句，length是这个查询语句的长度。如果查询成功，函数返回0。 
查询之后，我们要到一个MYSQL_RES变量来使用查询的结果。以下这行创立这个变量：
MYSQL_RES *res;
然后 
mysql_use_result(MYSQL *query)
该函数读出查询结果。 
尽管可以很容易地查询了，要用这个查询的结果还要用到其它的函数。第一个是：
MYSQL_ROW STDCALL mysql_fetch_row(MYSQL_RES *result);
该函数把结果转换成“数组”。你可能注意到了，该函数返回的是MYSQL_ROW变量类型。以下语句创立那样的变量： 
MYSQL_ROW row;
如前所解释的，变量row是一个字符串数组。也就是说，row[0]是数组的第一个值，row[1]是数组的第二个值...当我们用mysql_fetch_row的时候，接着变量row会取得结果的下一组的数据。当到了结果的尾部，该函数返回一负值。最后我们要关闭这个连接：
mysql_close(MYSQL *mysql)
## 一些有用的函数
看看如何取得一个表格的字段，以下这个函数可能实现这个功能： 
unsigned int STDCALL mysql_num_fields(MYSQL *mysql);
这个函数返回表格里有多少个字段。 
取得“数据集”的数目，用：
my_ulonglong STDCALL mysql_num_rows(MYSQL_RES *res);
my_ulonglong STDCALL mysql_affected_rows(MYSQL *mysql);
这个函数是用来得到受INSERT, DELETE, UPDATE查询语句影响的“数据集”数目。注意该函数返回的数据类型是my_ulonglong 
一些示例代码：
＃i nclude <mysql/mysql.h>
＃i nclude <stdio.h>
void main(){
   MYSQL *mysql;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *query;
   int t,r;
   mysql_init(mysql);
   if (!mysql_real_connect(mysql,"localhost","mysql",
        "mysql","deneme",0,NULL,0))
   {
       printf( "Error connecting to database: %s/n",mysql_error(mysql));
   }
   else printf("Connected.../n");
   query="select * from Deneme";
   t=mysql_real_query(mysql,query,(unsigned int) strlen(query));
   if (t)
   {
      printf("Error making query: %s/n",
              mysql_error(mysql));
   }
   else printf("Query made.../n");
   res=mysql_use_result(mysql);
   for(r=0;r<=mysql_field_count(mysql);r++){
           row=mysql_fetch_row(res);
           if(row<0) break;
           for(t=0;t<mysql_num_fields(res);t++){
                   printf("%s ",row[t]);
           }
           printf("/n");
   }
   mysql_close(mysql);
}++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++==ChinaUnix odin_free 
今天给新来的同事做mysql培训，用到的一个例子： 
连接数据库test 用户名root密码abc 
在里面有一个username的表，describe username结果如下 
mysql> describe username; 
+----------+-------------+------+-----+---------+-------+ 
| Field | Type | Null | Key | Default | Extra | 
+----------+-------------+------+-----+---------+-------+ 
| id | int(10) | YES | | NULL | | 
| username | varchar(32) | | PRI | | | 
+----------+-------------+------+-----+---------+-------+ 
mysql> select * from username; 
+------+----------+ 
| id | username | 
+------+----------+ 
| 1 | sfs | 
+------+----------+ 
1 row in set (0.00 sec) 
本文目的查找执行select id from username这个语句并打印结果 
代码: 
＃i nclude <unistd.h> 
＃i nclude <arpa/inet.h> 
＃i nclude <stdio.h> 
＃i nclude <stdlib.h> 
＃i nclude <string.h> 
＃i nclude <sys/types.h> 
＃i nclude <sys/socket.h> 
＃i nclude <netinet/in.h> 
＃i nclude <mysql/mysql.h> 
＃i nclude <signal.h> 
＃i nclude <errno.h> 
＃i nclude <syslog.h> 
MYSQL mysql; 
main() 
{ 
   char host[32]="localhost"; 
   char user[32]="root"; 
   char passwd[32]="abc"; 
   char dbname[32]="test";     
     
   if( mysql_init(&mysql) == NULL ) 
   { 
      syslog(LOG_USER|LOG_INFO,"inital mysql handle error/n"); 
      return 1; 
   } 
   if (mysql_real_connect(&mysql,host,user,passwd,dbname,0,NULL,0) == NULL) 
   { 
      syslog(LOG_USER|LOG_INFO, "Failed to connect to database: Error: %s/n",mysql_error(&mysql)); 
      return 1; 
   } 
   else syslog(LOG_USER|LOG_INFO, "connect to database: /n"); 
   find_ps();     
   db_close(); 
   return 0; 
} 
int db_close() 
{ 
        mysql_close(&mysql); 
         return 0; 
} 
int  find_ps () 
{ 
   MYSQL_ROW m_row; 
   MYSQL_RES *m_res; 
   char sql[1024],username[32]; 
   int res=1; 
   int *id; 
   sprintf(sql,"select id from username"); 
   if(mysql_query(&mysql,sql) != 0) 
   { 
      syslog(LOG_USER|LOG_INFO, "select ps_info Error: %s/n",mysql_error(&mysql)); 
      return res; 
   } 
   m_res = mysql_store_result(&mysql); 
   if(m_res==NULL) 
   { 
      syslog(LOG_USER|LOG_INFO, "select username Error: %s/n",mysql_error(&mysql)); 
      res = 3; 
      return res; 
   } 
   if(m_row = mysql_fetch_row(m_res)) 
   { 
      printf("m_row=%d/n",atoi(m_row[0])); 
      res = 0;       
   } 
   mysql_free_result(m_res); 
   return res; 
} 
编译命令 
gcc -g connect_db.c -L/usr/lib/mysql -lmysqlclient -lz 
注意： 
如果/tmp/ccTGmMS21.o: In function `main': 
/tmp/ccTGmMS21.o(.text+0x11): undefined reference to `mysql_init' 
那么参数增加-L/usr/lib/mysql -lmysqlclient 
如果 
usr/lib/mysql/libmysqlclient.a(my_compress.o): In function `my_uncompress': 
my_compress.o(.text+0xaa): undefined reference to `uncompress' 
那么增加-lz参数 <span style="font-size:10px;">编译和连接程序</span><p><span style="font-size:10px;">　　MySQL中有一个特殊的脚本,叫做mysql_config. 它会为你编译MySQL客户端,并连接到MySQL服务器提供有用的信息.你需要使用下面两个选项.</span></p><p><span style="font-size:10px;">　　1. --libs 选项 - 连接MySQL客户端函数库所需要的库和选项.</span></p><p><span style="font-size:10px;">　　$ mysql_config --libs</span></p><p><span style="font-size:10px;">　　输出:</span></p><p><span style="font-size:10px;">　　-L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -L/usr/lib64 -lssl -lcrypto</span></p><p><span style="font-size:10px;">　　2. --cflags 选项 - 使用必要的include文件的选项等等.</span></p><p><span style="font-size:10px;">　　$ mysql_config --cflags</span></p><p><span style="font-size:10px;">　　输出:</span></p><p><span style="font-size:10px;">　　-I/usr/include/mysql -g -pipe -m64 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing</span></p><p><span style="font-size:10px;">　　你需要将上面两个选项加入到对源文件的编译命令中. 所以,要编译上面的程序,要使用下面的命令:</span></p><p><span style="font-size:10px;">　　$ gcc -o output-file $(mysql_config --cflags) mysql-c-api.c $(mysql_config --libs)</span></p><p><span style="font-size:10px;">　　执行编译后的程序:</span></p><p><span style="font-size:10px;">　　$ ./output.file</span></p>
