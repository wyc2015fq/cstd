# Sqlite - gauss的专栏 - CSDN博客
2013年01月12日 17:36:34[gauss](https://me.csdn.net/mathlmx)阅读数：272
个人分类：[开源库](https://blog.csdn.net/mathlmx/article/category/944352)
转自:  http://blog.csdn.net/jgood/article/details/4640577
Sqlite是我最喜欢使用的文件数据库，它小巧、方便、速度快、支持大部分标准sql语句、开源、免费 .. .. .. 优点太多了。自从认识了她之后，我就将Access打入了冷宫，发誓以后再也不用Access了。我们可以到Sqlite的[官方网站](http://www.sqlite.org/)下载Sqlite的源代码，编译后的结果只有200多K，太小巧，太卡哇依了。如果嫌自己手动编译麻烦，也可以直接从官网下载二进制的可执行文件或者动态链接库。可执行文件可以用命令行的方式来创建、操作数据库；动态链接库则可以用程序来操作Sqlite数据库。今天我就来介绍一下如何使用c/c++来操作Sqlite吧~~~
#### 准备工作
    我这里使用的sqlite版本是3.x，所以首先从官网下载动态链接库[sqlite3.dll](http://www.sqlite.org/sqlitedll-3_6_18.zip)；然后再下载api头文件[sqlite3.h](http://cpansearch.perl.org/src/MSERGEANT/DBD-SQLite-1.14/sqlite3.h)。在Code::Block中新建一个C++的控制台项目，添加对sqlite3.dll的引用，同时将sqlite3.h添加到项目中。我这里还使用了一个工具：[SqliteSpy](http://www.yunqa.de/delphi/doku.php/products/sqlitespy/index)，用于对数据库进行可视化的操作。
#### 开始使用API
    下面就开始使用API吧~。Sqlite的所有api，可以在[**这里**](http://www.sqlite.org/c3ref/funclist.html)查询到。
##### sqlite3_open
##### sqlite3_exec
##### sqlite3_close
    看到这些函数，我相信你大概能猜出这些函数的功能。sqlite3_open用于打开一个数据库，并初始化一个数据库的连接；sqlite3_close用于关闭连接；sqlte3_exec用于执行sql语句。用一个简单的例子来演示这些函数的使用：下面的代码创建一个数据库test.db，并创建数据表：test_for_cpp。
// ----------------------------------------------// [http://blog.csdn.net/JGood](http://blog.csdn.net/JGood)// 创建数据库// sqlite3_open, sqlite3_exec, sqlite3_close// ----------------------------------------------
sqlite3 *conn = NULL;   //数据库连接。char *err_msg = NULL;   //操作失败的原因。char sql[200];
// 打开数据库, 创建连接if (SQLITE_OK != sqlite3_open("test.db", &conn))
{
    printf("can't open the database.”);
    exit(-1);
}
// 执行SQL
sprintf(sql, "CREATE TABLE [test_for_cpp] (/
    [id] int, [name] varchar(20), [age] int)");if (SQLITE_OK != sqlite3_exec(conn, sql, 0, 0, &err_msg))
{
    printf("operate failed: %s", err_msg);
    exit(-1);
}
// 关闭连接。if (SQLITE_OK != sqlite3_close(conn))
{
    printf("can't close the database: %s/n", sqlite3_errmsg(conn));
    exit(-1);
}
printf("operate successfully. /n");
上面的例子代码都非常简单。主要的函数sqlite3_exec比较复杂，它的原形为：
int sqlite3_exec(
  sqlite3*,                                  /* An open database */constchar *sql,                           /* SQL to be evaluated */int (*callback)(void*,int,char**,char**),  /* Callback function */void *,                                    /* 1st argument to callback */char **errmsg                              /* Error msg written here */
);
第三个参数是一个函数指针，表示函数执行完毕后的回调。第四个参数是传递给回调函数的参数，第五个参数表示执行失败时的原因。下面用一个简单的例子来演示该函数的详细使用：
// ----------------------------------------------// INSERT, SELECT// sqlite3_exec的详细使用// ----------------------------------------------
sqlite3 *conn = NULL;   //数据库连接。char *err_msg = NULL;   //操作失败的原因。char sql[200];
// 回调函数声明。externint sqlite3_exec_callback(void *data, int n_columns,
    char **col_values, char **col_names);
// 打开数据库, 创建连接
sqlite3_open("test.db", &conn);
//添加10条记录for (int i = 0; i < 10; i++)
{
    // 执行SQL
    sprintf(sql, "INSERT INTO [test_for_cpp] ([id], [name], [age]) /
        VALUES (%d, '%s', %d)", i, "JGood", i);
    sqlite3_exec(conn, sql, 0, 0, &err_msg);
}
// 查询
sprintf(sql, "SELECT * FROM [test_for_cpp]");
sqlite3_exec(conn, sql, &sqlite3_exec_callback, 0, &err_msg);
// 关闭连接。
sqlite3_close(conn);
//… …/** sqlite3_exec的回调。
 *
 *  向控制台打印查询的结果。
 *
 *  @param in data 传递给回调函数的数据。
 *  @param in n_columns sqlite3_exec执行结果集中列的数量。
 *  @param in col_values sqlite3_exec执行结果集中每一列的数据。
 *  @param in col_names sqlite3_exec执行结果集中每一列的名称。
 *  @return 状态码。
 */int sqlite3_exec_callback(void *data, int n_columns, char **col_values, char **col_names)
{
    for (int i = 0; i < n_columns; i++)
    {
    	printf("%s/t", col_values[i]);
    }
    printf("/n");
    return 0;
}
下面是执行的结果：![img](https://p-blog.csdn.net/images/p_blog_csdn_net/JGood/601714/o_img_thumb.jpg)
#### 使用事务
    在上面的例子中，我们向数据库里插入了10条数据，然后再从数据库里读出来。细心的你不知道有没有发现，在执行Insert的时候，并没有使用同一个事务。在很多情况下，我们需要使用事务来保证对数据库操作的原子性。Sqlite是支持事务的，而且对事务的使用非常简单：使用sql语句”begin;”表示事务开始，”rollback;”表示事务的回滚，”commit;”表示事务的提交。下面我们对上面例子中的代码作一下修改，给Insert操作添加事务支持：
//... ...bool is_succed = true;
sqlite3_exec(conn, "begin;", 0, 0, 0);  // 开启事务//添加10条记录for (int i = 0; i < 10; i++)
{
    // 执行SQL
    sprintf(sql, "INSERT INTO [test_for_cpp] ([id], [name], [age]) /
        VALUES (%d, '%s', %d)", i, "JGood", i);if (SQLITE_OK != sqlite3_exec(conn, sql, 0, 0, &err_msg))
    {
        is_succed = false;
        break;
    }
}
if (is_succed)
    sqlite3_exec(conn, "commit;", 0, 0, 0);  // 提交事务else
    sqlite3_exec(conn, "rollback;", 0, 0, 0);  // 回滚事务//... ...
#### 使用sql参数
    基本上，使用sqlite3_open, sqlite3_close, sqlite3_exec这三个函数，可以完成大大部分的工作。但还不完善。上面的例子中，都是直接以sql语句的形式来操作数据库，这样很容易被注入。所以有必要使用sql参数。
##### sqlite3_prepare
##### sqlite3_bind_*
##### sqlite3_step
##### sqlite3_column_*
##### struct sqlite3_stmt
##### sqlite3_finalize
    sqlite3_prepare用来编译sql语句。sql语句被执行之前，必须先编译成字节码。sqlite3_stmt是一个结构体，表示sql语句编译后的字节码。sqlite3_step用来执行编译后的sql语句。sqlite3_bind_*用于将sql参数绑定到sql语句。sqlite3_column_*用于从查询的结果中获取数据。sqlite3_finalize用来释放sqlite3_stmt对象。代码最能说明函数的功能，下面就用一个例子来演示吧~~
// ----------------------------------------------// [http://blog.csdn.net/JGood](http://blog.csdn.net/JGood)// sqlite3_prepare, sqlite3_bind_*, sqlite3_step, sqlite3_column_*, sqlite3_column_type// sqlite3_stmt, sqlite3_finalize, sqlite3_reset// 查询// ----------------------------------------------
sqlite3 *conn = NULL;
sqlite3_stmt *stmt = NULL;
constchar *err_msg = NULL;
// 列数据类型char col_types[][10] = { "", "Integer", "Float", "Text", "Blob", "NULL" };
sqlite3_open("test.db", &conn);
sqlite3_prepare(conn, "SELECT * FROM [test_for_cpp] WHERE [id]>?", -1, &stmt, &err_msg);
sqlite3_bind_int(stmt, 1, 5);
while (SQLITE_ROW == sqlite3_step(stmt))
{
    int col_count = sqlite3_column_count(stmt); // 结果集中列的数量constchar *col_0_name = sqlite3_column_name(stmt, 0); // 获取列名int id = sqlite3_column_int(stmt, 0);
    int id_type = sqlite3_column_type(stmt, 0); // 获取列数据类型constchar *col_2_name = sqlite3_column_name(stmt, 2);
    int age = sqlite3_column_int(stmt, 2);
    int age_type = sqlite3_column_type(stmt, 2);
    constchar *col_1_name = sqlite3_column_name(stmt, 1);
    char name[80];
    strncpy(name, (constchar *)sqlite3_column_text(stmt, 1), 80);
    int name_type = sqlite3_column_type(stmt, 1);
    // 打印结果
    printf("col_count: %d, %s = %d(%s), %s = %s(%s), %s = %d(%s)/n",
        col_count, col_0_name, id, col_types[id_type], col_2_name, name,
        col_types[name_type], col_1_name, age, col_types[age_type]);
}
sqlite3_finalize(stmt); // 释放sqlite3_stmt 
sqlite3_close(conn);
这段代码查询id号大于5的所有记录，并显示到控制台，最后效果为![img](https://p-blog.csdn.net/images/p_blog_csdn_net/JGood/601714/o_img_thumb_1.jpg)
#### 其他函数
    在上面的例子中，还使用了其他的一些函数，如：sqlite3_column_count用于获取结果集中列的数量；sqlite3_column_name用于获取列的名称；sqlite3_column_type用于获取列的数据类型；sqlite3_errcode用于获取最近一次操作出错的错误代码；sqlite3_errmsg用于获取最近一次操作出错的错误说明。sqlite的api中还有很多的函数，有了上面的基础，相信你通过查询官方的文档，能迅速掌握本文未介绍的api。
#### 字符串编码
    在官网上查看Sqlite的api的时候，发现有很同函数的名称都非常相似，只是最后添加了”_16”，如：sqlite3_open和sqlite3_open16,  sqlite3_errmsg和sqlite3_errmsg16，等等。其实添加了”16”后缀的函数，主要用于支持utf-16编码的字符串。如sqlite3_open16可以接收utf-16编码的数据库路径。
    在sourceforge上，有一个开源的项目sqlitex，它封装了这些api，使对sqlite数据库的操作更加方便。sqlitex的源代码非常的简单，感兴趣的同学可以下载下来自己研究。
- **参考文档：**
- Sqlite官网: [http://www.sqlite.org/](http://www.sqlite.org/)
- Sqlite中文社区：[http://www.sqlite.com.cn/](http://www.sqlite.com.cn/)
