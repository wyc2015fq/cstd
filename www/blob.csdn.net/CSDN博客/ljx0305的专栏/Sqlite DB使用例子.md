# Sqlite DB使用例子 - ljx0305的专栏 - CSDN博客
2009年12月31日 14:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2305标签：[sqlite																[数据库																[sql																[insert																[integer																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)
个人分类：[Sqlite DB](https://blog.csdn.net/ljx0305/article/category/638539)
前一阵字做项目(嵌入式linux)，由于要保存大量的数据，而且最长要保存30天的时间。本来打算保存到文件中，每次启动应用程序的时候重新解析一遍，可是当数据量很大的时候，就出现效率的问题了。所以最后还是放弃了使用文件的打算，决定使用数据库存取数据。
linux下的数据库也很多，有开源的，也有收费的。对于我们来说，肯定要使用开源的数据库。以前用过Berkely DB，但是需要licience。所以结合效率性能以及大小限制，最终选定了Sqlite DB。我们对它的评价是“sqlite是一个优秀的完全free的开源数据项目”。
下面是一个简单的使用实例：
int main( int argc, char **argv ) 
{ 
        sqlite3 *db; 
        sqlite3_stmt * stmt; 
        const char *zTail; 
        //打开数据库 
        int r = sqlite3_open("mysqlite.db",&db) 
        if(r){ 
                printf("%s",sqlite3_errmsg(db)); 
        } 
        //创建Table 
        sqlite3_prepare(db, 
                "CREATE TABLE players ( ID INTEGER PRIMARY KEY, name TEXT, age INTERER );", 
                -1,&stmt,&zTail); 
        sqlite3_step(stmt); 
        sqlite3_finalize(stmt); 
        //插入数据 
        sqlite3_prepare(db, 
                "INSERT INTO players (name,num) VALUES(?,?);", 
                -1,&stmt,&zTail); 
        char str[] = "Kevin"; 
        int n = 23; 
        sqlite3_bind_text(stmt,1,str,-1,SQLITE_STATIC); 
        sqlite3_bind_int(stmt,2,n); 
        r = sqlite3_step(stmt); 
        if( r!=SQLITE_DONE){ 
                printf("%s",sqlite3_errmsg(db)); 
        } 
        sqlite3_reset(stmt); 
        //插入第二个数据 
        char str2[] = "Jack"; 
        int n2 = 16; 
        sqlite3_bind_text(stmt,1,str2,-1,SQLITE_STATIC); 
        sqlite3_bind_int(stmt,2,n2); 
        r = sqltie3_step(stmt); 
        if( r!=SQLITE_DONE){ 
                printf("%s",sqlite3_errmsg(db)); 
        } 
        sqltie3_finalize(stmt); 
        //查询所有数据 
        sqlite3_prepare(db, 
                "SELECT ID, name, num FROM players ORDER BY num;", 
                -1,&stmt,&zTail); 
        r = sqlite3_step(stmt); 
        int number; 
        int id; 
        const unsigned char * name; 
        while( r == SQLITE_ROW ){ 
                id = sqlite3_column_int( stmt, 0 ); 
                name = sqlite3_column_text( stmt,1 ); 
                number = sqlite3_column_int( stmt, 2 ); 
                printf("ID: %d  Name: %s  Age: %d /n",id,name,number); 
                sqlite3_step(stmt); 
        } 
        sqlite3_finalize(stmt); 
        //关闭数据库 
        sqlite3_close(db); 
        return 0; 
} 
同时加上头文件#include <stdio.h> #icnlude <sqlite3.h> 编译运行gcc -o sample sample.c ./sample
结果如下：
ID:1 Name:Kevin Age:23
ID:2 Name:Jack Age:16 
简要说明一下SQLite数据库执行SQL语句的过程 
** 调用sqlite3_prepare()将SQL语句编译为sqlite内部一个结构体(sqlite3_stmt).该结构体中包含了将要执行的的SQL语句的信息. 
** 如果需要传入参数,在SQL语句中用'?'作为占位符,再调用sqlite3_bind_XXX()函数将对应的参数传入. 
** 调用sqlite3_step(),这时候SQL语句才真正执行.注意该函数的返回值,SQLITE_DONE和SQLITE_ROW都是表示执行成功, 不同的是SQLITE_DONE表示没有查询结果,象UPDATE,INSERT这些SQL语句都是返回SQLITE_DONE,SELECT查询语句在 查询结果不为空的时候返回SQLITE_ROW,在查询结果为空的时候返回SQLITE_DONE. 
** 每次调用sqlite3_step()的时候,只返回一行数据,使用sqlite3_column_XXX()函数来取出这些数据.要取出全部的数据需要 反复调用sqlite3_step(). (注意, 在bind参数的时候,参数列表的index从1开始,而取出数据的时候,列的index是从0开始). 
** 在SQL语句使用完了之后要调用sqlite3_finalize()来释放stmt占用的内存.该内存是在sqlite3_prepare()时分配的. 
** 如果SQL语句要重复使用,可以调用sqlite3_reset()来清楚已经绑定的参数. 
当然了，你也可以根据自己的需要对现有的libsqlite进行一层封装，使得操作更方便一些。高级的功能还在学习中。。。另外想使用好sqlite db，最好先看一遍SQL的语法操作。 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/mawl2002/archive/2007/11/23/1899678.aspx](http://blog.csdn.net/mawl2002/archive/2007/11/23/1899678.aspx)
