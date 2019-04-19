# SQLite3以及FMDB的简单实用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月12日 15:59:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：631
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/897733b577f3](http://www.jianshu.com/p/897733b577f3)
#### SQLite
SQLite是一款轻型的嵌入式数据库，它占用资源非常的低，在嵌入式设备中，可能只需要几百K的内存就够了。同时它的处理速度也是非常快速的。
- 因为在iOS中SQLite3依赖于系统提供的`libsqlite.tbd`这个类库，所以在使用之前首先我们需要导入这个类库；
- 
打开数据库，打开数据库成功之后进行创表操作；
```
// 数据库在沙盒中的存放路径；
 NSString *filename = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:@"shops.db"];
 //1.0 打开数据库；
 /* 参数一：数据库文件存放路径；如果数据库文件不存在, 系统会自动创建文件并且自动初始化数据库
    参数二：数据库实例对象；
  */
 if (sqlite3_open(filename.UTF8String, &_db) == SQLITE_OK) {
     const char *sql = "CREATE TABLE IF NOT EXISTS t_shop (id integer PRIMARY KEY, name text NOT NULL, price real);";
     //2.0 执行操作；
     char *errmsg = NULL;
     /* 参数一：数据库实例对象；
        参数二：执行的SQL语句；
      */
     if (sqlite3_exec(self.db, sql, NULL, NULL, &errmsg)!= SQLITE_OK) {
          NSLog(@"创表失败--%s", errmsg);
     }
 } else {
     NSLog(@"打开数据库失败");
 }
```
在上面的代码中，我们做了两个操作，首先打开数据库我们使用了`sqlite3_open`这个方法，该方法根据文件路径打开数据库，如果不存在，则会创建一个新的数据库。如果result等于常量SQLITE_OK，则表示成功打开数据库。该方法有两个参数，如下：
```cpp
```cpp
int sqlite3_open(
 const char *filename,   // 数据库的文件路径
 sqlite3 **ppDb          // 数据库实例
);
```
```
其次，我们使用`sqlite3_exec`方法来执行建表操作。该方法适合于查询结果集之外的任何操作。
```
```
2.执行任何SQL语句
int sqlite3_exec(
 sqlite3*,                                  // 一个打开的数据库实例
 const char *sql,                           // 需要执行的SQL语句
 int (*callback)(void*,int,char**,char**),  // SQL语句执行完毕后的回调
 void *,                                    // 回调函数的第1个参数
 char **errmsg                              // 错误信息
);
```
```
- 
执行查询操作；
```
NSString * sql=[NSString stringWithFormat:@"SELECT name,price FROM %@;",tableName];
 // sqlite3_stmt实例，用来获得数据库数据
 sqlite3_stmt *stmt = NULL;
 // 预准备，其实就是分析SQL语句的合法性
 if (sqlite3_prepare_v2(self.db, sql.UTF8String, -1, &stmt, NULL) == SQLITE_OK) {
     // 返回SQLITE_ROW代表成功取出一条数据
     while (sqlite3_step(stmt) == SQLITE_ROW) {
         const char *name = (const char *)sqlite3_column_text(stmt, 0);
         const char *price = (const char *)sqlite3_column_text(stmt, 1);
     }
 }
```
上面用到了两个方法：
```cpp
```cpp
3.检查SQL语句的合法性（查询前的准备）
int sqlite3_prepare_v2(
 sqlite3 *db,            // 数据库实例
 const char *zSql,       // 需要检查的SQL语句
 int nByte,              // SQL语句的最大字节长度,-1代表系统会帮我们自动计算
 sqlite3_stmt **ppStmt,  // sqlite3_stmt实例，用来获得数据库数据
 const char **pzTail
);
4.查询一行数据
int sqlite3_step(sqlite3_stmt*); // 如果查询到一行数据，就会返回SQLITE_ROW
```
```
查询分三个阶段：
准备阶段：`sqlite3_prepare_v2()`
执行阶段：`sqlte3_step()`
终止阶段：`sqlite3_finalize()`
- 使用约束变量
`sqlite3_bind`是用来给sqlite3_stmt *pStmt语句增加值的，对于不同类型的参数要选用不同的函数。
例如，要执行带两个约束变量的插入操作，第一个变量是int类型，第二个是C字符串：
```
char *sql = "insert into oneTable values (?, ?);";
sqlite3_stmt *stmt;
if (sqlite3_prepare_v2(database, sql, -1, &stmt, nil) == SQLITE_OK) {
 sqlite3_bind_int(stmt, 1, 235);
 sqlite3_bind_text(stmt, 2, "valueString", -1, NULL);
}
if (sqlite3_step(stmt) != SQLITE_DONE)
NSLog(@"Something is Wrong!");
sqlite3_finalize(stmt);
```
这里，`sqlite3_bind_int(stmt, 1, 235);`有三个参数：
第一个是`sqlite3_stmt`类型的变量
第二个是所约束变量的标签index,`（从1开始）`
第三个参数是要加的值
有一些函数多出两个变量，例如
`sqlite3_bind_text(stmt, 2, "valueString", -1, NULL);`
第四个参数代表第三个参数中需要传递的长度。对于C字符串来说，-1表示传递全部字符串。
第五个参数是一个回调函数，比如执行后做内存清除工作。
- 
利用stmt获得某一字段的值（字段的下标从0开始）
```cpp
```cpp
double sqlite3_column_double(sqlite3_stmt*, int iCol);  // 浮点数据
int sqlite3_column_int(sqlite3_stmt*, int iCol); // 整型数据
sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol); // 长整型数据
const void *sqlite3_column_blob(sqlite3_stmt*, int iCol); // 二进制文本数据
const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);  // 字符串数据
```
```
因为获取某一列的值我们不知道具体的类型，所以我们可以遍历字段，然后根据类型自动匹配：
```
//存放最终的返回结果；
 NSMutableArray * muArray=[NSMutableArray array];
 //2.0 执行预处理 获取结果，如果下一行有记录，就会自动返回SQLITE_ROW，同时会自动将指针移动到下一行；
 while (sqlite3_step(ppStmt) == SQLITE_ROW) {
     //存放当前行的数据；
     NSMutableDictionary * mnDic=[NSMutableDictionary dictionary];
     [muArray addObject:mnDic];
     //获取当前行 中 列的个数；
     int columnCount= sqlite3_column_count(ppStmt);
     for(int i=0;i<columnCount;i++ )
     {
      //获取列名称；
      NSString * columnName=[NSString stringWithUTF8String:sqlite3_column_name(ppStmt, i)];
         //获取列的类型
       int columnType= sqlite3_column_type(ppStmt, i);
         //匹配类型；
         id value;
         //#define SQLITE_INTEGER  1
         //#define SQLITE_FLOAT    2
         //#define SQLITE_BLOB     4
         //#define SQLITE_NULL     5
         //#define SQLITE3_TEXT    3
         switch (columnType) {
             case SQLITE_INTEGER:
             {
                 value= @(sqlite3_column_int(ppStmt, i));
                 break;
             }
             case SQLITE_FLOAT:
             {
                 value= @(sqlite3_column_double(ppStmt, i));
                 break;
             }
             case SQLITE_BLOB:
             {
                 value= CFBridgingRelease(sqlite3_column_blob(ppStmt, i));
                 break;
             }
             case SQLITE_NULL:
             {
                 value= @"";
                 break;
             }
              case SQLITE3_TEXT:
             {
                 const char * columnValue=(const char *)sqlite3_column_text(ppStmt, i);
                 value=[NSString stringWithUTF8String:columnValue];
                 break;
             }
             default:
                 break;
         }
         [mnDic setValue:value forKeyPath:columnName];
     }
 }
 //3.0 关闭数据库
 sqlite3_finalize(ppStmt);//销毁准备语句，防止内存泄漏；
 [self closeDBWithUID:uid];
 return  muArray;
```
#### FMDB
FMDB是iOS平台的SQLite数据库框架，它以OC的方式封装了SQLite的C语言API，使用起来更加面向对象，省去了很多麻烦的C语言代码，对比苹果自带的Core Data框架，更加轻量级和灵活，提供了多线程安全的数据库操作方法，有效地防止数据混乱。
FMDB有三个主要的类：
`FMDatabase:`一个FMDatabase对象就代表一个单独的SQLite数据库
`FMResultSet:`使用FMDatabase执行查询后的结果集
`FMDatabaseQueue:`用于在多线程中执行多个查询或更新，它是线程安全的
在FMDB中，除查询以外的所有操作，都称为“更新”
`create、drop、insert、update、delete`等
更新操作
使用executeUpdate:方法执行更新:
```
- (BOOL)executeUpdate:(NSString*)sql, ...
- (BOOL)executeUpdateWithFormat:(NSString*)format, ...
- (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments
```
查询操作
```
- (FMResultSet *)executeQuery:(NSString*)sql, ...
- (FMResultSet *)executeQueryWithFormat:(NSString*)format, ...
- (FMResultSet *)executeQuery:(NSString *)sql withArgumentsInArray:(NSArray *)arguments
```
例如：
```
// 查询数据
FMResultSet *result = [_db executeQuery:@"SELECT * FROM t_student"];
// 遍历结果集
while ([result next]) {
    NSString *name = [rs stringForColumn:@"name"];
    int age = [rs intForColumn:@"age"];
    double score = [rs doubleForColumn:@"score"];
}
```
FMDatabaseQueue
FMDatabase这个类是线程不安全的，如果在多个线程中同时使用一个FMDatabase实例，会造成数据混乱等问题,为了保证线程安全，FMDB提供了非常方便快捷的FMDatabaseQueue类：
```
FMDatabaseQueue *queue = [FMDatabaseQueue databaseQueueWithPath:path];
[queue inDatabase:^(FMDatabase *db) {
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Jack"];
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Rose"];
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Jim"];
    FMResultSet *rs = [db executeQuery:@"select * from t_student"];
    while ([rs next]) {
        // …
    }
}];
```
使用事物：
```
[queue inTransaction:^(FMDatabase *db, BOOL *rollback) {
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Jack"];
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Rose"];
    [db executeUpdate:@"INSERT INTO t_student(name) VALUES (?)", @"Jim"];
    FMResultSet *result = [db executeQuery:@"select * from t_student"];
    while ([result next]) {
        // …
    }
}];
```
```
#import <Foundation/Foundation.h>
@interface StatusTool : NSObject
/**
 *  根据请求参数去沙盒中加载缓存的微博数据
 *
 *  @param params 请求参数
 */
+ (NSArray *)statusesWithParams:(NSDictionary *)params;
/**
 *  存储微博数据到沙盒中
 *
 *  @param statuses 需要存储的微博数据
 */
+ (void)saveStatuses:(NSArray *)statuses;
@end
```
具体实现：
```
#import "StatusTool.h"
#import "FMDB.h"
@implementation StatusTool
static FMDatabase *_db;
+ (void)initialize
{
    // 1.通过指定SQLite数据库文件路径来创建FMDatabase对象
    NSString *path = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:@"Statuses.db"];
    _db = [FMDatabase databaseWithPath:path];
    //2.0 打开数据库
    [_db open];
    // 3.0创表
    [_db executeUpdate:@"CREATE TABLE IF NOT EXISTS t_status (id integer PRIMARY KEY, status blob NOT NULL, idstr text NOT NULL);"];
}
+ (NSArray *)statusesWithParams:(NSDictionary *)params
{
    // 根据请求参数生成对应的查询SQL语句
    NSString *sql = nil;
    if (params[@"since_id"]) {
        sql = [NSString stringWithFormat:@"SELECT * FROM t_status WHERE idstr > %@ ORDER BY idstr DESC LIMIT 20;", params[@"since_id"]];
    } else if (params[@"max_id"]) {
        sql = [NSString stringWithFormat:@"SELECT * FROM t_status WHERE idstr <= %@ ORDER BY idstr DESC LIMIT 20;", params[@"max_id"]];
    } else {
        sql = @"SELECT * FROM t_status ORDER BY idstr DESC LIMIT 20;";
    }
    // 执行SQL
    FMResultSet *set = [_db executeQuery:sql];
    NSMutableArray *statuses = [NSMutableArray array];
    while (set.next) {
        //可以根据columnName来获取数据，跟不用根据列号；
        NSData *statusData = [set objectForColumnName:@"status"];
        NSDictionary *status = [NSKeyedUnarchiver unarchiveObjectWithData:statusData];
        [statuses addObject:status];
    }
    return statuses;
}
+ (void)saveStatuses:(NSArray *)statuses
{
    // 要将一个对象存进数据库的blob字段,最好先转为NSData,一个对象要遵守NSCoding协议,实现协议中相应的方法,才能转成NSData
    for (NSDictionary *status in statuses) {
        // NSDictionary --> NSData
        NSData *statusData = [NSKeyedArchiver archivedDataWithRootObject:status];
        [_db executeUpdateWithFormat:@"INSERT INTO t_status(status, idstr) VALUES (%@, %@);", statusData, status[@"idstr"]];
    }
}
@end
```
[OC笔记](http://www.jianshu.com/nb/2349377)
