# 最全iOS数据存储方法介绍：FMDB，SQLite3 ，Core Data，Plist，Preference偏好设置，NSKeyedArchiver归档，Realm - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月26日 09:57:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：73标签：[FMDB，SQLite3 ，Core Data，Plist，																[FMDB																[SQLite3																[Core Data](https://so.csdn.net/so/search/s.do?q=Core Data&t=blog)](https://so.csdn.net/so/search/s.do?q=SQLite3&t=blog)](https://so.csdn.net/so/search/s.do?q=FMDB&t=blog)](https://so.csdn.net/so/search/s.do?q=FMDB，SQLite3 ，Core Data，Plist，&t=blog)
项目准备运用的Core Data进行本地数据存储，本来打算只写一下Core Data的，不过既然说到了数据存储，干脆来个数据存储基础大总结！
本文将对以下几个模块进行叙述。
> - 沙盒
- Plist
- Preference偏好设置
- NSKeyedArchiver归档 / NSKeyedUnarchiver解档
- SQLite3的使用
- FMDB
- Core Data
Realm的文档很详细，这篇文章就不写了： **[Realm的使用方法](https://link.jianshu.com?t=https%3A%2F%2Frealm.io%2Fcn%2Fdocs%2Fswift%2Flatest%2F)**
下图是Core Data堆栈的图示，在这里是为了做文章的封面图片，后文会介绍Core Data的使用方法。
![](https://upload-images.jianshu.io/upload_images/3873004-53bc0752d2ab526e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Core Data
# 一、沙盒
iOS本地化存储的数据保存在沙盒中， 并且每个应用的沙盒是相对独立的。每个应用的沙盒文件结构都是相同的，如下图所示：
![](https://upload-images.jianshu.io/upload_images/3873004-835e0ae90a24b3cb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/292/format/webp)
沙盒目录
> 
**Documents**：iTunes会备份该目录。一般用来存储需要持久化的数据。
> 
**Library/Caches**：缓存，iTunes不会备份该目录。内存不足时会被清除，应用没有运行时，可能会被清除，。一般存储体积大、不需要备份的非重要数据。
> 
**Library/Preference**：iTunes同会备份该目录，可以用来存储一些偏好设置。
> 
**tmp**： iTunes不会备份这个目录，用来保存临时数据，应用退出时会清除该目录下的数据。
如何拿到每个文件夹的路径呢？我这里就只说最好的一种方法。
```
// 这个方法返回的是一个数组，iOS中这个数组其实只有一个元素，所以我们可以用lastObject或lastObject来拿到Documents目录的路径
NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
// 得到Document目录下的test.plist文件的路径
NSString *filePath = [documentPath stringByAppendingPathComponent:@"test.plist"];
```
上面的方法有三个参数，这里分别说一下。
> 
NSDocumentDirectory: 第一个参数代表要查找哪个文件，是一个枚举，点进去看一下发现有很多选择，为了直接找到沙盒中的Documents目录，我们一般用NSDocumentDirectory。
> 
NSUserDomainMask: 也是一个枚举，表示搜索的范围限制于当前应用的沙盒目录。我们一般就选择NSUserDomainMask。
> 
YES (expandTilde): 第三个参数是一个BOOL值。iOS中主目录的全写形式是/User/userName，这个参数填YES就表示全写，填NO就是写成‘‘~’’，我们一般填YES。
根据上面的文字，你应用可以知道如何拿到Library/Caches目录下的文件路径了吧？没错，就是这样：
```
//获取Library/Caches目录路径
NSString *path = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES).firstObject; 
//获取Library/Caches目录下的test.data文件路径
NSString *filePath = [path stringByAppendingPathComponent:@"test.data"];
```
```
//获取temp路径
NSString *tmp= NSTemporaryDirectory();
//获取temp下test.data文件的路径
NSString *filePath = [tmp stringByAppendingPathComponent:@"test.data"];
```
所以，如果程序中有需要长时间持久化的数据，就选择Documents，如果有体积大但是并不重要的数据，就可以选择交给Library，而临时没用的数据当然是放到temp。至于Preference则可以用来保存一些设置类信息，后面会讲到偏好设置的使用方法。
> 
推荐一个好用的分类工具集合**[WHKit](https://www.jianshu.com/p/c935314b078e)**，可以直接使用分类方法拿到文件路径。
```
//一行代码搞定
NSString *filePath = [@"test.plist" appendDocumentPath];
//使用WHKit，上面这一行代码等同于下面这两行。
NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
NSString *filePath = [documentPath stringByAppendingPathComponent:@"test.plist"];
```
# 二、Plist存储
Plist文件的Type可以是字典NSDictionary或数组NSArray，也就是说可以把字典或数组直接写入到文件中。
NSString、NSData、NSNumber等类型，也可以使用writeToFile:atomically:方法直接将对象写入文件中，只是Type为空。
下面就举个例子来看一下如何使用Plist来存储数据。
```
//准备要保存的数据
NSDictionary *dict = [NSDictionary dictionaryWithObject:@"first" forKey:@"1"];
//获取路径，你也可以利用上面说WHKit更优雅的拿到路径
NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
NSString *filePath = [documentPath stringByAppendingPathComponent:@"test.plist"];
//写入数据
[dict writeToFile:filePath atomically:YES];
```
上面的代码运行之后，在应用沙盒的Documents中就创建了一个plist文件，并且已经写入数据保存。
![](https://upload-images.jianshu.io/upload_images/3873004-ae96d131ad931538.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/816/format/webp)
写入plist
数据存储了，那么如何读取呢？
```
//拿到plist文件路径，你也可以利用上面说WHKit更优雅的拿到路径
NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
NSString *filePath = [documentPath stringByAppendingPathComponent:@"test.plist"];
//解析数据，log出的结果为first，读取成功
NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:filePath];
NSString *result = dict[@"1"];
NSLog(@"%@",result);
```
上面这段代码就读出了plist种的数据。
# 三、Preference偏好设置
偏好设置的使用非常方便快捷，我们一般使用它来进行一些设置的记录，比如用户名，开关是否打开等设置。
Preference是通过NSUserDefaults来使用的，是通过键值对的方式记录设置。下面举个例子。
利用NSUserDefaults判断APP是不是首次启动。
```
// 启动的时候判断key有没有value，如果有，说明已经启动过了，如果没有，说明是第一次启动
if (![[NSUserDefaults standardUserDefaults] valueForKey:@"first"]) {
        
    //如果是第一次启动，就运用偏好设置给key设置一个value
    [[NSUserDefaults standardUserDefaults] setValue:@"start" forKey:@"first"];
    NSLog(@"是第一次启动");
        
} else {
    NSLog(@"不是第一次启动");
}
```
通过键值对的方式非常easy的保存了数据。
> 
**注意:**NSUserDefaults可以存储的数据类型包括：NSData、NSString、NSNumber、NSDate、NSArray、NSDictionary。如果要存储其他类型，则需要转换为前面的类型，才能用NSUserDefaults存储。
下面的例子是用NSUserDefaults存储图片，需要先把图片转换成NSData类型。
```
UIImage *image=[UIImage imageNamed:@"photo"];
//UIImage对象转换成NSData
NSData *imageData = UIImageJPEGRepresentation(image, 100);
//偏好设置可以保存NSData，但是不能保存UIimage
[[NSUserDefaults standardUserDefaults] setObject:imageData forKey:@"image"];
    
// 读出data
NSData *getImageData = [[NSUserDefaults standardUserDefaults] dataForKey:@"image"];
//NSData转换为UIImage
UIImage *Image = [UIImage imageWithData:imageData];
```
NSUserDefaults是不是很好用！不过有没有觉得每次都写[NSUserDefaults standardUserDefaults]有点烦，那么又到了推荐环节，好用的分类工具**[WHKit](https://www.jianshu.com/p/c935314b078e)**，这个工具中有许多好用的宏定义，其中一个就是KUSERDEFAULT，等同于[NSUserDefaults standardUserDefaults]。
**[WHKit](https://www.jianshu.com/p/c935314b078e)**中还有更多好用的宏定义等着你！
# 四、NSKeyedArchiver归档 / NSKeyedUnarchiver解档
归档和解档会在写入、读出数据之前进行序列化、反序列化，数据的安全性相对高一些。
NSKeyedArchiver可以有三个使用情景。
1.对单个简单对象进行归档/解档
与plist差不多，对于简单的数据进行归档，直接写入文件路径。
```
//获取归档文件路径
NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
NSString *filePath = [documentPath stringByAppendingPathComponent:@"test"];
//对字符串@”test”进行归档，写入到filePath中
[NSKeyedArchiver archiveRootObject:@"test" toFile:filePath];
//根据保存数据的路径filePath解档数据
NSString *result = [NSKeyedUnarchiver unarchiveObjectWithFile:filePath];
//log结构为@”test”，就是上面归档的数据
NSLog(@"%@",result);
```
当然，也可以存储NSArray，NSDictionary等对象。
2.对多个对象进行归档/解档
这种情况可以一次保存多种不同类型的数据，最终使用的是与plist相同的writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile来写入数据。
```
//获取归档路径
    NSString *documentPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
    NSString *filePath = [documentPath stringByAppendingPathComponent:@"test"];
    
    //用来承载数据的NSMutableData
    NSMutableData *data = [[NSMutableData alloc] init];
    //归档对象
    NSKeyedArchiver *archiver = [[NSKeyedArchiver alloc] initForWritingWithMutableData:data];
    
    //将要被保存的三个数据
    NSString *name = @"jack";
    int age = 17;
    double height = 1.78;
    
    //运用encodeObject:方法归档数据
    [archiver encodeObject:name forKey:@"name"];
    [archiver encodeInt:age forKey:@"age"];
    [archiver encodeDouble:height forKey:@"height"];
    //结束归档
    [archiver finishEncoding];
    //写入数据（存储数据）
    [data writeToFile:filePath atomically:YES];
    
    
    //NSMutableData用来承载解档出来的数据
    NSMutableData *resultData = [[NSMutableData alloc] initWithContentsOfFile:filePath];
    //解档对象
    NSKeyedUnarchiver *unArchiver = [[NSKeyedUnarchiver alloc] initForReadingWithData:resultData];
    
    //分别解档出三个数据
    NSString *resultName = [unArchiver decodeObjectForKey:@"name"];
    int resultAge = [unArchiver decodeIntForKey:@"age"];
    double resultHeight = [unArchiver decodeDoubleForKey:@"height"];
    //结束解档
    [unArchiver finishDecoding];
    //成功打印出结果，说明成功归档解档
    NSLog(@"name = %@, age = %d, height = %.2f",resultName,resultAge,resultHeight);
```
3.归档保存自定义对象
我认为这是归档最常使用的情景。
定义一个Person类，如果想对person进行归档解档，首先要让Person遵守<NSCoding>协议。
```
/********Person.h*********/
#import <Foundation/Foundation.h>
//遵守NSCoding协议
@interface Person : NSObject<NSCoding>
@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) NSInteger age;
//自定义的归档保存数据的方法
+(void)savePerson:(Person *)person;
//自定义的读取沙盒中解档出的数据
+(Person *)getPerson;
@end
```
NSCoding协议有2个方法：
> - (void)encodeWithCoder:(NSCoder *)aCoder
	归档时调用这个方法，在方法中使用encodeObject:forKey:归档变量。
- (instancetype)initWithCoder:(NSCoder *)aDecoder
	解档时调用这个方法，在方法中石油decodeObject:forKey读出变量。
```
/******Person.m*******/
#import "Person.h"
@implementation Person
//归档，Key建议使用宏代替，这里就不使用了
- (void)encodeWithCoder:(NSCoder *)aCoder {
    [aCoder encodeObject:self.name forKey:@"name"];
    [aCoder encodeInteger:self.age forKey:@"age"];
}
//解档
-(instancetype)initWithCoder:(NSCoder *)aDecoder {
    if (self=[super init]) {
        self.name = [aDecoder decodeObjectForKey:@"name"];
        self.age = [aDecoder decodeIntegerForKey:@"age"];
    }
    return self;
}
//类方法，运用NSKeyedArchiver归档数据
+(void)savePerson:(Person *)person {
    NSString *docPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
    NSString *path=[docPath stringByAppendingPathComponent:@"Person.plist"];
    [NSKeyedArchiver archiveRootObject:person toFile:path];
}
//类方法，使用NSKeyedUnarchiver解档数据
+(Person *)getPerson {
    NSString *docPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject;
    NSString *path=[docPath stringByAppendingPathComponent:@"Person.plist"];
    Person *person = [NSKeyedUnarchiver unarchiveObjectWithFile:path];
    return person;
}
@end
```
下面就可以在需要的地方归档或解档Person对象。
```
/*******ViewController.m*******/
    //创建Person对象
    Person *person = [Person new];
    person.name = @"jack";
    person.age = 17;
    //归档保存数据
    [Person savePerson:person];
    //解档拿到数据
    Person *resultPerson = [Person getPerson];
    //打印出结果，证明归档解档成功
    NSLog(@"name = %@, age = %ld",resultPerson.name,resultPerson.age);
```
# 五、SQLite3的使用
> 
1、首先需要添加库文件libsqlite3.0.tbd
2、导入头文件#import <sqlite3.h>
3、打开数据库
4、创建表
5、对数据表进行增删改查操作
6、关闭数据库
上代码之前，有些问题你需要了解。
- 
SQLite 不区分大小写，但也有需要注意的地方，例如GLOB 和 glob 具有不同作用。
- 
SQLite3有5种基本数据类型 text、integer、float、boolean、blob
- 
SQLite3是无类型的，在创建的时候你可以不声明字段的类型，不过还是建议加上数据类型
```
create table t_student(name, age);
```
```
create table t_student(name text, age integer);
```
下面的代码就是SQLite3的基本使用方法，带有详细注释。
代码中用到了一个Student类，这个类有两个属性name和age。
```
/****************sqliteTest.m****************/
#import "sqliteTest.h"
//1.首先导入头文件
#import <sqlite3.h>
//2.数据库
static sqlite3 *db;
@implementation sqliteTest
/** 3.打开数据库 */
+ (void)openSqlite {
    
    //数据库已经打开
    if (db != nil) {
        NSLog(@"数据库已经打开");
        return;
    }
    
    //创建数据文件路径
    NSString *string = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
    NSString *path = [string stringByAppendingPathComponent:@"Student.sqlite"];
    NSLog(@"%@",path);
    
    //打开数据库
    int result = sqlite3_open(path.UTF8String, &db);
    
    if (result == SQLITE_OK) {
        NSLog(@"数据库打开成功");
    } else {
        NSLog(@"数据库打开失败");
    }
}
/** 4.创建表 */
+ (void)createTable {
    
    //创建表的SQLite语句，其中id是主键，not null 表示在表中创建纪录时这些字段不能为NULL
    NSString *sqlite = [NSString stringWithFormat:@"create table if not exists t_student (id integer primary key autoincrement, name text not null, age integer)"];
    
    //用来记录错误信息
    char *error = NULL;
    
    //执行SQLite语句
    int result = sqlite3_exec(db, sqlite.UTF8String, nil, nil, &error);
    
    if (result == SQLITE_OK) {
        NSLog(@"创建表成功");
    }else {
        NSLog(@"创建表失败");
    }
}
/** 5.添加数据 */
+ (void)addStudent:(Student *)stu {
    
    //增添数据的SQLite语句
    NSString *sqlite = [NSString stringWithFormat:@"insert into t_student (name,age) values ('%@','%ld')",stu.name,stu.age];
    char *error = NULL;
    
    int result = sqlite3_exec(db, [sqlite UTF8String], nil, nil, &error);
    
    if (result == SQLITE_OK) {
        NSLog(@"添加数据成功");
    } else {
        NSLog(@"添加数据失败");
    }
}
/** 6.删除数据 */
+ (void)deleteStuWithName:(NSString *)name {
    
    //删除特定数据的SQLite语句
    NSString *sqlite = [NSString stringWithFormat:@"delete from t_student where name = '%@'",name];
    char *error = NULL;
    
    int result = sqlite3_exec(db, sqlite.UTF8String, nil, nil, &error);
    
    if (result == SQLITE_OK) {
        NSLog(@"删除数据成功");
    } else {
        NSLog(@"删除数据失败");
    }
}
/** 7.更改数据 */
+ (void)upDateWithStudent:(Student *)stu WhereName:(NSString *)name {
    
    //更新特定字段的SQLite语句
    NSString *sqlite = [NSString stringWithFormat:@"update t_student set name = '%@', age = '%ld' where name = '%@'",stu.name,stu.age,name];
    char *error = NULL;
    
    int result = sqlite3_exec(db, sqlite.UTF8String, nil, nil, &error);
    
    if (result == SQLITE_OK) {
        NSLog(@"修改数据成功");
    } else {
        NSLog(@"修改数据失败");
    }
}
/** 8.根据条件查询 */
+ (NSMutableArray *)selectWithAge:(NSInteger)age {
    
    //可变数组，用来保存查询到的数据
    NSMutableArray *array = [NSMutableArray array];
    
    //查询所有数据的SQLite语句
    NSString *sqlite = [NSString stringWithFormat:@"select * from t_student where age = '%ld'",age];
    
    //定义一个stmt存放结果集
    sqlite3_stmt *stmt = NULL;
    
    //执行
    int result = sqlite3_prepare(db, sqlite.UTF8String, -1, &stmt, NULL);
    
    if (result == SQLITE_OK) {
        NSLog(@"查询成功");
        
        //遍历查询到的所有数据，并添加到上面的数组中
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student *stu = [[Student alloc] init];
            //获得第1列的姓名，第0列是id
            stu.name = [NSString stringWithUTF8String:(const char *)sqlite3_column_text(stmt, 1)];
            //获得第2列的年龄
            stu.age = sqlite3_column_int(stmt, 2);
            [array addObject:stu];
        }
    } else {
        NSLog(@"查询失败");
    }
    
    //销毁stmt，防止内存泄漏
    sqlite3_finalize(stmt);
    return array;
}
/** 9.查询所有数据 */
+ (NSMutableArray *)selectStudent {
    
    //可变数组，用来保存查询到的数据
    NSMutableArray *array = [NSMutableArray array];
    
    //查询所有数据的SQLite语句
    NSString *sqlite = [NSString stringWithFormat:@"select * from t_student"];
    
    //定义一个stmt存放结果集
    sqlite3_stmt *stmt = NULL;
    
    //执行
    int result = sqlite3_prepare(db, sqlite.UTF8String, -1, &stmt, NULL);
    
    if (result == SQLITE_OK) {
        NSLog(@"查询成功");
        
        //遍历查询到的所有数据，并添加到上面的数组中
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student *stu = [[Student alloc] init];
            //获得第1列的姓名，第0列是id
            stu.name = [NSString stringWithUTF8String:(const char *)sqlite3_column_text(stmt, 1)];
            //获得第2列的年龄
            stu.age = sqlite3_column_int(stmt, 2);
            [array addObject:stu];
        }
    } else {
        NSLog(@"查询失败");
    }
    
    //销毁stmt，防止内存泄漏
    sqlite3_finalize(stmt);
    return array;
}
/** 10.删除表中的所有数据 */
+ (void)deleteAllData {
    NSString *sqlite = [NSString stringWithFormat:@"delete from t_student"];
    char *error = NULL;
    int result = sqlite3_exec(db, sqlite.UTF8String, nil, nil, &error);
    if (result == SQLITE_OK) {
        NSLog(@"清除数据库成功");
    } else {
        NSLog(@"清除数据库失败");
    }
}
/** 11.删除表 */
+ (void)dropTable {
    NSString *sqlite = [NSString stringWithFormat:@"drop table if exists t_student"];
    char *error = NULL;
    int result = sqlite3_exec(db, sqlite.UTF8String, nil, nil, &error);
    if (result == SQLITE_OK) {
        NSLog(@"删除表成功");
    } else {
        NSLog(@"删除表失败");
    }
}
/** 12.关闭数据库 */
+ (void)closeSqlite {
    int result = sqlite3_close(db);
    if (result == SQLITE_OK) {
        NSLog(@"数据库关闭成功");
    } else {
        NSLog(@"数据库关闭失败");
    }
}
```
附上SQLite的基本语句
- 创建表:
	create table if not exists 表名 (字段名1, 字段名2...);
> 
create table if not exists t_student (id integer primary key autoincrement, name text not null, age integer)
- 增加数据:
	insert into 表名 (字段名1, 字段名2, ...) values(字段1的值, 字段2的值, ...);
> 
insert into t_student (name,age) values (@"Jack",@17);
- 根据条件删除数据：
	delete from 表名 where 条件;
> 
delete from t_student where name = @"Jack";
- 删除表中所有的数据：
	delete from 表名
> 
delete from t_student
- 根据条件更改某个数据：
	update 表名 set 字段1 = '值1', 字段2 = '值2' where 字段1 = '字段1的当前值'
> 
update t_student set name = 'lily', age = '16' where name = 'Jack'
- 根据条件查找：
	select * from 表名 where 字段1 = '字段1的值'
> 
select * from t_student where age = '16'
- 查找所有数据：
	select * from 表名
> 
select * from t_student
- 删除表：
	drop table 表名
> 
drop table t_student
- 排序查找：
	select * from 表名 order by 字段
> 
select * from t_student order by age asc （升序，默认）
select * from t_student order by age desc （降序）
- 限制:
	select * from 表名 limit 值1, 值2
> 
select * from t_student limit 5, 10 （跳过5个，一共取10个数据）
SQLite3还有事务方面的使用，这里就不做说明了，下面的FMDB中会有事务的使用。
# 六、FMDB
FMDB封装了SQLite的C语言API，更加面向对象。
首先需要明确的是FMDB中的三个类。
> 
FMDatabase：可以理解成一个数据库。
> 
FMResultSet：查询的结果集合。
> 
FMDatabaseQueue：运用多线程，可执行多个查询、更新。线程安全。
FMDB基本语法
查询：executeQuery: SQLite语句命令。
```
[db executeQuery:@"select id, name, age from t_person"]
```
其余的操作都是“更新”：executeUpdate: SQLite语句命令。
```
// CREATE, UPDATE, INSERT, DELETE, DROP，都使用executeUpdte
[db executeUpdate:@"create table if not exists t_person (id integer primary key autoincrement, name text, age integer)"]
```
FMDB的基本使用
在项目中导入FMDB框架和sqlite3.0.tbd，导入头文件。
**1. 打开数据库，并创建表**
> 
初始化FMDatabase：FMDatabase *db = [FMDatabase databaseWithPath:filePath];
其中的filePath是提前准备好要存放数据的路径。
> 
打开数据库：[db open]
> 
创建数据表：[db executeUpdate:@"create table if not exists t_person (id integer primary key autoincrement, name text, age integer)"];
```
#import "ViewController.h"
#import <FMDB.h>
@interface ViewController ()
@end
@implementation ViewController{
    FMDatabase *db;
}
- (void)openCreateDB {
    //存放数据的路径
    NSString *path = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES).firstObject;
    NSString *filePath = [path stringByAppendingPathComponent:@"person.sqlite"];
    
    //初始化FMDatabase
    db = [FMDatabase databaseWithPath:filePath];
    
    //打开数据库并创建person表，person中有主键id，姓名name，年龄age
    if ([db open]) {
        BOOL success = [db executeUpdate:@"create table if not exists t_person (id integer primary key autoincrement, name text, age integer)"];
        if (success) {
            NSLog(@"创表成功");
        }else {
            NSLog(@"创建表失败");
        }
    } else {
        NSLog(@"打开失败");
    }
}
```
**2. 插入数据**
> 
运用executeUpdate方法执行插入数据命令： [db executeUpdate:@"insert into t_person(name,age) values(?,?)",@"jack",@17]
```
-(void)insertData {
    BOOL success = [db executeUpdate:@"insert into t_person(name,age) values(?,?)",@"jack",@17];
    if (success) {
        NSLog(@"添加数据成功");
    } else {
        NSLog(@"添加数据失败");
    }
}
```
**3. 删除数据**
> 
删除姓名为lily的数据：[db executeUpdate:@"delete from t_person where name = 'lily'"]
```
-(void)deleteData {
    BOOL success = [db executeUpdate:@"delete from t_person where name = 'lily'"];
    if (success) {
        NSLog(@"删除数据成功");
    } else {
        NSLog(@"删除数据失败");
    }
}
```
**4. 修改数据**
> 
把年龄为17岁的数据，姓名改为lily：[db executeUpdate:@"update t_person set name = 'lily' where age = 17"]
```
-(void)updateData {
    BOOL success = [db executeUpdate:@"update t_person set name = 'lily' where age = 17"];
    if (success) {
        NSLog(@"更新数据成功");
    } else {
        NSLog(@"更新数据失败");
    }
}
```
**5. 查询数据**
> 
执行查询语句，用FMResultSet接收查询结果：FMResultSet *set = [db executeQuery:@"select id, name, age from t_person"]
> 
遍历查询结果：[set next]
> 
拿到每条数的姓名：NSString *name = [set stringForColumnIndex:1];
> 
也可以这样拿到每条数据的姓名：NSString *name = [result stringForColumn:@"name"];
```
FMResultSet *set = [db executeQuery:@"select id, name, age from t_person"];
    while ([set next]) {
        int ID = [set intForColumnIndex:0];
        NSString *name = [set stringForColumnIndex:1];
        int age = [set intForColumnIndex:2];
        NSLog(@"%d,%@,%d",ID,name,age);
    }
}
```
**6. 删除表**
> 
删除指定表：[db executeUpdate:@"drop table if exists t_person"]
```
-(void)dropTable {
    BOOL success = [db executeUpdate:@"drop table if exists t_person"];
    if (success) {
        NSLog(@"删除表成功");
    } else {
        NSLog(@"删除表失败");
    }
}
```
FMDatabaseQueue基本使用
FMDatabase是线程不安全的，当FMDB数据存储想要使用多线程的时候，FMDatabaseQueue就派上用场了。
> 
初始化FMDatabaseQueue的方法与FMDatabase类似
```
//数据文件路径
NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
NSString *filePath = [path stringByAppendingPathComponent:@"student.sqlite"];
    
//初始化FMDatabaseQueue
FMDatabaseQueue *dbQueue = [FMDatabaseQueue databaseQueueWithPath:filePath];
```
> 
在FMDatabaseQueue中执行命令的时候也是非常方便，直接在一个block中进行操作
```
-(void)FMDdatabaseQueueFunction {
    
    NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
    //数据文件路径
    NSString *filePath = [path stringByAppendingPathComponent:@"student.sqlite"];
    //初始化FMDatabaseQueue
    FMDatabaseQueue *dbQueue = [FMDatabaseQueue databaseQueueWithPath:filePath];
    
    //在block中执行SQLite语句命令
    [dbQueue inDatabase:^(FMDatabase * _Nonnull db) {
        //创建表
        [db executeUpdate:@"create table if not exists t_student (id integer primary key autoincrement, name text, age integer)"];
        //添加数据
        [db executeUpdate:@"insert into t_student(name,age) values(?,?)",@"jack",@17];
        [db executeUpdate:@"insert into t_student(name,age) values(?,?)",@"lily",@16];
        //查询数据
        FMResultSet *set = [db executeQuery:@"select id, name, age from t_student"];
        //遍历查询到的数据
        while ([set next]) {
            int ID = [set intForColumn:@"id"];
            NSString *name = [set stringForColumn:@"name"];
            int age = [set intForColumn:@"age"];
            NSLog(@"%d,%@,%d",ID,name,age);
        }
        
    }];
}
```
FMDB中的事务
什么是事务？
> 
事务（Transaction）是不可分割的一个整体操作，要么都执行，要么都不执行。
举个例子，幼儿园有20位小朋友由老师组织出去春游，返校的时候，所有人依次登上校车，这时候如果有一位小朋友没有上车，车也是不能出发的。所以哪怕19人都上了车，也等于0人上车。20人是一个整体。
当然这个例子可能不是很精准。
FMDB中有事务的回滚操作，也就是说，当一个整体事务在执行的时候出了一点小问题，则执行回滚，之后这套事务中的所有操作将整体无效。
> 
下面代码中，利用事务循环向数据库中添加2000条数据，假如在添加的过程中出现了一些问题，由于执行了*rollback = YES的回滚操作，数据库中一个数据都不会出现。
如果第2000条数据的添加出了问题，哪怕之前已经添加了1999条数据，由于执行了回滚，数据库中依然一个数据都没有。
```
//数据库路径
NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
NSString *filePath = [path stringByAppendingPathComponent:@"student.sqlite"];
//初始化FMDatabaseQueue
FMDatabaseQueue *dbQueue = [FMDatabaseQueue databaseQueueWithPath:filePath];
//FMDatabaseQueue的事务inTransaction
[dbQueue inTransaction:^(FMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        //创建表
        [db executeUpdate:@"create table if not exists t_student (id integer primary key autoincrement, name text, age integer)"];
        //循环添加2000条数据
        for (int i = 0; i < 2000; i++) {
            BOOL success = [db executeUpdate:@"insert into t_student(name,age) values(?,?)",@"jack",@(i)];
            //如果添加数据出现问题，则回滚
            if (!success) {
                //数据回滚
                *rollback = YES;
                return;
            }
        }
    }];
```
# 七、Core Data
Core Data有着图形化的操作界面，并且是操作模型数据的，更加面向对象。当你了解并熟悉它的时候，相信你会喜欢上这个数据库存储框架。
## 利用Core Data快速实现数据存储
### 1. 图形化创建模型
> 
创建项目的时候，勾选下图中的Use Core Data选项，工程中会自动创建一个数据模型文件。当然，你也可以在开发中自己手动创建。
![](https://upload-images.jianshu.io/upload_images/3873004-30e030fb329b2643.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/730/format/webp)
自动创建模型文件
> 
下图就是自动创建出来的文件
![](https://upload-images.jianshu.io/upload_images/3873004-99d5db789d967547.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/207/format/webp)
创建出来的文件
> 
如果没有勾选，也可以在这里手动创建。
![](https://upload-images.jianshu.io/upload_images/3873004-251764adcf0b0974.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/730/format/webp)
手动创建
> 
点击Add Entity之后，相当一张数据表。表的名称自己在上方定义，注意首字母要大写。
在界面中还可以为数据实体添加属性和关联属性。
![](https://upload-images.jianshu.io/upload_images/3873004-c55d052fbfa927da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
创建一个数据表
> 
Core Data属性支持的数据类型如下
![](https://upload-images.jianshu.io/upload_images/3873004-79805e0cf409be6d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/164/format/webp)
数据类型
> 
编译之后，Xcode会自动生成Person的实体代码文件，并且文件不会显示在工程中，如果下图中右侧Codegen选择Manual/None,则Xcode就不会自动生成代码，我们可以自己手动生成。
![](https://upload-images.jianshu.io/upload_images/3873004-f173f833442ef047.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
6.png
> 
手动生成实体类代码，选中CoreDataTest.xcdatamodeld文件，然后在Mac菜单栏中选择Editor，如下图所示。一路Next就可以了。
如果没有选择Manual/None，依然进行手动创建的话，则会与系统自动创建的文件发生冲突，这点需要注意。
你也可以不要选择Manual/None，直接使用系统创建好的NSManagedObject，同样会有4个文件，只是在工程中是看不到的，使用的时候直接导入#import "Person+CoreDataClass.h"头文件就可以了。
![](https://upload-images.jianshu.io/upload_images/3873004-2b736bc1c5123364.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
手动创建NSManagedObject
> 
手动创建出来的是这样4个文件
![](https://upload-images.jianshu.io/upload_images/3873004-bb4b87cba7ead220.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/196/format/webp)
10.png
> 
还要注意编程语言的选择，Swift或OC
![](https://upload-images.jianshu.io/upload_images/3873004-03e56a90691514a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/257/format/webp)
编程语言
### 2.Core Data堆栈的介绍与使用
下面要做的就是对Core Data进行初始化，实现本地数据的保存。
需要用到的类有三个：
> 
NSManagedObjectModel 数据模型的结构信息
NSPersistentStoreCoordinator 数据持久层和对象模型协调器
NSManagedObjectContext 对象的上下文managedObject 模型
如下图所示，一个context内可以有多个模型对象，不过在大多数的操作中只存在一个context，并且所有的对象存在于那个context中。
对象和他们的context是相关联的，每个被管理的对象都知道自己属于哪个context，每个context都知道自己管理着哪些对象。
Core Data从系统读或写的时候，有一个持久化存储协调器(persistent store coordinator)，并且这个协调器在文件系统中与SQLite数据库交互，也连接着存放模型的上下文Context。
![](https://upload-images.jianshu.io/upload_images/3873004-29ba0050746dea8c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Core Data
下图是比较常用的方式：
![](https://upload-images.jianshu.io/upload_images/3873004-1bf62d77778e9bb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Core Data堆栈
下面我们来一步步实现Core Data堆栈的创建。
> - 首先在AppDelegate中定义一个NSManagedObjectModel属性。然后利用懒加载来创建NSManagedObjectModel对象。并且要注意创建时候的后缀用momd，代码如下：
```
//创建属性
@property (nonatomic, readwrite, strong) NSManagedObjectModel *managedObjectModel;
//懒加载
- (NSManagedObjectModel *)managedObjectModel {
    if (!_managedObjectModel) {
        //注意扩展名为 momd
        NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"CoreDataTest" withExtension:@"momd"];
        _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    }
    return _managedObjectModel;
}
```
> - 创建协调器NSPersistentStoreCoordinator，同样的先在AppDelegate中来一个属性，然后懒加载。
```
//属性
@property (nonatomic, readwrite, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
//懒加载
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
    
    if (!_persistentStoreCoordinator) {
        
        //传入之前创建好了的Model
        _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:self.managedObjectModel];
        
        //指定sqlite数据库文件
        NSURL *sqliteURL = [[self documentDirectoryURL] URLByAppendingPathComponent:@"CoreDataTest.sqlite"];
        
        //这个options是为了进行数据迁移用的，有兴趣的可以去研究一下。
        NSDictionary *options=@{NSMigratePersistentStoresAutomaticallyOption:@(YES),NSInferMappingModelAutomaticallyOption:@(YES)};
        NSError *error;
        
        [_persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType
                                                  configuration:nil
                                                            URL:sqliteURL
                                                        options:options
                                                          error:&error];
        if (error) {
            NSLog(@"创建协调器失败： %@", error.localizedDescription);
        }
    }
    return _persistentStoreCoordinator;
}
//获取document目录
- (nullable NSURL *)documentDirectoryURL {
    return [[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask].firstObject;
}
```
> - 创建NSManagedObjectContext，同样的是属性+懒加载。
```
//属性
@property (nonatomic, readwrite, strong) NSManagedObjectContext *context;
//懒加载
- (NSManagedObjectContext *)context {
    if (!_context) {
        
        _context = [[NSManagedObjectContext alloc ] initWithConcurrencyType:NSMainQueueConcurrencyType];
        
        // 指定协调器
        _context.persistentStoreCoordinator = self.persistentStoreCoordinator;
    }
    return _context;
}
```
### 3.运用Core Data对数据进行增删改查
> - 添加数据
	使用NSEntityDesctiption类的一个方法创建NSManagedObject对象。参数一是实体类的名字，参数二是之前创建的Context。
	为对象赋值，然后存储。
```
@implementation ViewController {
    NSManagedObjectContext *context;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    
    //拿到managedObjectContext
    context = [AppDelegate new].context;
    
    //运用NSEntityDescription创建NSManagedObject对象
    Person *person = [NSEntityDescription insertNewObjectForEntityForName:@"Person" inManagedObjectContext:context];
    //为对象赋值
    person.name = @"Jack";
    person.age = 17;
    NSError *error;
    //保存到数据库
    [context save:&error];
}
```
> - 查询数据
	Core Data从数据库中查询数据，会用到三个类：
**NSFetchRequest：**一条查询请求，相当于 SQL 中的select语句
**NSPredicate：**谓词，指定一些查询条件，相当于 SQL 中的where
**NSSortDescriptor：**指定排序规则，相当于 SQL 中的 order by
NSFetchRequest中有两个属性：
> 
**predicate：**是NSPredicate对象
**sortDescriptors：**它是一个NSSortDescriptor数组，数组中前面的优先级比后面高。可以有多个排列规则。
```
//更多NSFetchRequest属性
    fetchLimit：结果集最大数，相当于 SQL 中的limit
    fetchOffset：查询的偏移量，默认为0
    fetchBatchSize：分批处理查询的大小，查询分批返回结果集
    entityName/entity：数据表名，相当于 SQL中的from
    propertiesToGroupBy：分组规则，相当于 SQL 中的group by
    propertiesToFetch：定义要查询的字段，默认查询全部字段
```
设置好NSFetchRequest之后，调用NSManagedObjectContext的executeFetchRequest方法，就会返回结果集了。
```
//Xcode自动创建的NSManagedObject会生成fetchRequest方法，可以直接得到NSFetchRequest
    NSFetchRequest *fetchRequest = [Person fetchRequest];
    //也可以这样获得：[NSFetchRequest fetchRequestWithEntityName:@"Student"];
    
    //谓词
    fetchRequest.predicate = [NSPredicate predicateWithFormat:@"age == %@", @(16)];
    
    //排序
    NSArray<NSSortDescriptor *> *sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"age" ascending:YES]];
    
    fetchRequest.sortDescriptors = sortDescriptors;
    //运用executeFetchRequest方法得到结果集
    NSArray<Person *> *personResult = [context executeFetchRequest:fetchRequest error:nil];
```
Xcode自己有一个NSFetchRequest的code snippet，“fetch”，出现的结果如下图。
![](https://upload-images.jianshu.io/upload_images/3873004-daa3cc2b0bf01cc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/502/format/webp)
snippet
![](https://upload-images.jianshu.io/upload_images/3873004-93cce81e7f89c890.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/777/format/webp)
NSFetchRequest的code snippet: “fetch”
> - 更新数据
	更新数据比较简单，查询出来需要修改的数据之后，直接修改值，然后用context save就可以了。
```
for (Person *person in personResult) {
        //直接修改
        person.age = 26;
    }
//别忘了save一下
[context save:&error];
```
> - 删除数据
	查询出来需要删除的数据之后，调用 NSManagedObjectContext 的deleteObject方法就可以了。
```
for (Person *person in personResult) {
        //删除数据
        [context deleteObject:person];
    }
//别忘了save
[context save:&error];
```
至此Core Data的基础内容就讲完了。
# 后记：
iOS中有多种数据持久化的方法，要根据具体情景选用最合适的技术。
> 
推荐简单又好用的分类集合：**[WHKit](https://www.jianshu.com/p/c935314b078e)**
github地址：[https://github.com/remember17](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fremember17)
作者：remember17
链接：https://www.jianshu.com/p/e88880be794f
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
