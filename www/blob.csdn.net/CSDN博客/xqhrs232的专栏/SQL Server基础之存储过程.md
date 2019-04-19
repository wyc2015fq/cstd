# SQL Server基础之存储过程 - xqhrs232的专栏 - CSDN博客
2017年10月19日 16:10:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：363
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[http://www.cnblogs.com/selene/p/4483612.html](http://www.cnblogs.com/selene/p/4483612.html)
相关文章
1、[SQL server存储过程创建与使用](http://blog.csdn.net/miniduhua/article/details/52102176)----[http://blog.csdn.net/miniduhua/article/details/52102176](http://blog.csdn.net/miniduhua/article/details/52102176)
2、[SQLSERVER存储过程基本语法](http://www.cnblogs.com/chaoa/articles/3894311.html)----[http://www.cnblogs.com/chaoa/articles/3894311.html](http://www.cnblogs.com/chaoa/articles/3894311.html)
3、SqlServer存储过程详解----[http://www.cnblogs.com/sunniest/p/4386296.html](http://www.cnblogs.com/sunniest/p/4386296.html)
4、sqlserver存储过程学习(通俗易懂)----[http://blog.csdn.net/wanlong360599336/article/details/52415420](http://blog.csdn.net/wanlong360599336/article/details/52415420)
5、sqlserver存储过程详细代码(超赞!)----[https://wenku.baidu.com/view/c44c0149de80d4d8d15a4f62.html](https://wenku.baidu.com/view/c44c0149de80d4d8d15a4f62.html)
6、sqlserver怎么创建存储过程----[https://jingyan.baidu.com/article/380abd0a4122161d91192c60.html](https://jingyan.baidu.com/article/380abd0a4122161d91192c60.html)
阅读目录
- [一:存储过程概述](http://www.cnblogs.com/selene/p/4483612.html#_label0)
- [二：存储过程分类](http://www.cnblogs.com/selene/p/4483612.html#_label1)
- [三：创建存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label2)
- [1.创建无参存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label3)
- [2.修改存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label4)
- [3.删除存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label5)
- [4.重命名存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label6)
- [5.创建带参数的存储过程](http://www.cnblogs.com/selene/p/4483612.html#_label7)
  简单来说，存储过程就是一条或者多条sql语句的集合,可视为批处理文件，但是其作用不仅限于批处理。本篇主要介绍变量的使用，存储过程和存储函数的创建，调用，查看，修改以及删除操作。
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 一:存储过程概述
SQL Server中的存储过程是使用T_SQL编写的代码段。它的目的在于能够方便的从系统表中查询信息，或者完成与更新数据库表相关的管理任务和其他的系统管理任务.T_SQL语句是SQL Server数据库与应用程序之间的编程接口。在很多情况下，一些代码会被开发者重复编写多次，如果每次都编写相同功能的代码，不但繁琐，而且容易出错，而且由于SQL Server逐条的执行语句会降低系统的运行效率。
 简而言之，存储过程就是SQL Server为了实现特定任务，而将一些需要多次调用的固定操作语句编写成程序段，这些程序段存储在服务器上，有数据库服务器通过程序来调用。
存储过程的优点:
- 存储过程加快系统运行速度，存储过程只在创建时编译，以后每次执行时不需要重新编译。
- 存储过程可以封装复杂的数据库操作，简化操作流程，例如对多个表的更新，删除等。
- 可实现模块化的程序设计，存储过程可以多次调用，提供统一的数据库访问接口，改进应用程序的可维护性。
- 存储过程可以增加代码的安全性，对于用户不能直接操作存储过程中引用的对象，SQL  Server可以设定用户对指定存储过程的执行权限。
- 存储过程可以降低网络流量，存储过程代码直接存储于数据库中，在客户端与服务器的通信过程中，不会产生大量的T_SQL代码流量。
存储过程的缺点：
- 数据库移植不方便，存储过程依赖与数据库管理系统， SQL Server 存储过程中封装的操作代码不能直接移植到其他的数据库管理系统中。
- 不支持面向对象的设计，无法采用面向对象的方式将逻辑业务进行封装，甚至形成通用的可支持服务的业务逻辑框架.
- 代码可读性差，不易维护。不支持集群。
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 二：存储过程分类
1.系统存储过程
  系统存储过程是 SQL Server系统自身提供的存储过程，可以作为命令执行各种操作。
  系统存储过程主要用来从系统表中获取信息，使用系统存储过程完成数据库服务器的管理工作，为系统管理员提供帮助，为用户查看数据库对象提供方便，系统存储过程位于数据库服务器中，并且以sp_开头，系统存储过程定义在系统定义和用户定义的数据库中，在调用时不必在存储过程前加数据库限定名。例如：sp_rename系统存储过程可以修改当前数据库中用户创建对象的名称，sp_helptext存储过程可以显示规则，默认值或视图的文本信息，SQL SERVER服务器中许多的管理工作都是通过执行系统存储过程来完成的，许多系统信息也可以通过执行系统存储过程来获得。
  系统存储过程创建并存放在与系统数据库master中，一些系统存储过程只能由系统管理员使用，而有些系统存储过程通过授权可以被其它用户所使用。
2.用户存储过程(自定义存储过程)
  自定义存储过程即用户使用T_SQL语句编写的、为了实现某一特定业务需求，在用户数据库中编写的T_SQL语句集合，自定义存储过程可以接受输入参数、向客户端返回结果和信息，返回输出参数等。创建自定义存储过程时，存储过程名前加上"##"表示创建了一个全局的临时存储过程;存储过程前面加上"#"时，表示创建的局部临时存储过程。局部临时存储过程只能在创建它的回话中使用，会话结束时，将被删除。这两种存储过程都存储在tempdb数据库中。
用户定义的存储过程分为两类：T_SQL 和CLR
T_SQL:存储过程是值保存的T_SQL语句集合，可以接受和返回用户提供的参数，存储过程也可能从数据库向客户端应用程序返回数据。
CLR存储过程是指引用Microsoft.NET Framework公共语言的方法存储过程，可以接受和返回用户提供的参数，它们在.NET Framework程序集是作为类的公共静态方法实现的。
3.扩展存储过程
 扩展存储过程是以在SQL SERVER环境外执行的动态连接(DLL文件)来实现的，可以加载到SQL SERVER实例运行的地址空间中执行，扩展存储过程可以用SQL SERVER扩展存储过程API编程，扩展存储过程以前缀"xp_"来标识，对于用户来说，扩展存储过程和普通话存储过程一样，可以用相同的方法来执行。 
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 三：创建存储过程
工欲善其事，必先利其器，准备数据如下:
```
use sample_db;
--创建测试books表
create table books (
    book_id int identity(**1**,**1**) primary key,
    book_name varchar(**20**),
    book_price float,
    book_auth varchar(**10**)
);
--插入测试数据
insert into books (book_name,book_price,book_auth)
                    values
                        ('论语',**25.6**,'孔子'),
                        ('天龙八部',**25.6**,'金庸'),
                        ('雪山飞狐',**32.7**,'金庸'),
                        ('平凡的世界',**35.8**,'路遥'),
                        ('史记',**54.8**,'司马迁');
```
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 1.创建无参存储过程
```
--1.创建无参存储过程
if (exists (select * from sys.objects where name = 'getAllBooks'))
    drop proc proc_get_student
go
create procedure getAllBooks
as
select * from books;
--调用,执行存储过程
exec getAllBooks;
```
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 2.修改存储过程
```
alter procedure dbo.getAllBooks 
as
select book_auth from books;
```
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 3.删除存储过程
```
drop procedure getAllBooks;
```
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 4.重命名存储过程
```
sp_rename getAllBooks,proc_get_allBooks;
```
[回到顶部](http://www.cnblogs.com/selene/p/4483612.html#_labelTop)
## 5.创建带参数的存储过程
 存储过程的参数分为两种：输入参数和输出参数
输入参数：用于向存储过程传入值，类似java语言或则c中的值传递。
输出参数：用于调用存储过程后，参会结果，类似java语言的按引用传递。
值传递和引用传递区别:
- 基本数据类型赋值属于值传递；引用类型之间赋值属于引用传递。
- 值传递传递的是实实在在的变量值；引用传递传递的是对象的引用地址。
- 值传递后，两个变量改变的是各自的值；引用传递后，两个引用改变的是同一个对象的状态
(1)带一个参数存储过程
```
if (exists (select * from sys.objects where name = 'searchBooks'))
    drop proc searchBooks
go
create proc searchBooks(@bookID int)
as
    --要求book_id列与输入参数相等
    select * from books where book_id=@bookID;
--执行searchBooks
exec searchBooks **1**;
```
(2)带2个参数存储过程
```
if (exists (select * from sys.objects where name = 'searchBooks1'))
    drop proc searchBooks1
go
create proc searchBooks1(
    @bookID int,
    @bookAuth varchar(**20**)
)
as
    --要求book_id和book_Auth列与输入参数相等
    select * from books where book_id=@bookID and book_auth=@bookAuth;
exec searchBooks1 **1**,'金庸';
```
(3)创建有返回值的存储过程
```
if (exists (select * from sys.objects where name = 'getBookId'))
    drop proc getBookId
go
create proc getBookId(
    @bookAuth varchar(**20**),--输入参数,无默认值
    @bookId int output --输入/输出参数 无默认值
)
as
    select @bookId=book_id from books where book_auth=@bookAuth
--执行getBookId这个带返回值的存储过程
declare @id int --声明一个变量用来接收执行存储过程后的返回值
exec getBookId '孔子',@id output
select @id as bookId;--as是给返回的列值起一个名字
```
(4)创建带通配符的存储过程
```
if (exists (select * from sys.objects where name = 'charBooks'))
    drop proc charBooks
go
create proc charBooks(
    @bookAuth varchar(**20**)='金%',
    @bookName varchar(**20**)='%'
)
as 
    select * from books where book_auth like @bookAuth and book_name like @bookName;
--执行存储过程charBooks
exec  charBooks    '孔%','论%';
```
(5)加密存储过程
　　with encryption子句对用户隐藏存储过程的文本.下例创建加密过程,使用 sp_helptext 系统存储过程获取关于加密过程的信息,然后尝试直接从 syscomments 表中获取关于该过程的信息.
```
if (object_id('books_encryption', 'P') is not null)
    drop proc books_encryption
go
create proc books_encryption 
with encryption
as 
    select * from books;
--执行此过程books_encryption
exec books_encryption;
exec sp_helptext 'books_encryption';--控制台会显示"对象 'books_encryption' 的文本已加密。"
```
(6).不缓存存储过程
```
--with  recompile不缓存
if (object_id('book_temp', 'P') is not null)
    drop proc book_temp
go
create proc book_temp
with recompile
as
    select * from books;
go
exec book_temp;
exec sp_helptext 'book_temp';
```
(7).创建带游标参数的存储过程
```
if (object_id('book_cursor', 'P') is not null)
    drop proc book_cursor
go
create proc book_cursor
    @bookCursor cursor varying output
as
    set @bookCursor=cursor forward_only static for
    select book_id,book_name,book_auth from books
    open @bookCursor;
go
--调用book_cursor存储过程
declare @cur cursor,
        @bookID int,
        @bookName varchar(**20**),
        @bookAuth varchar(**20**);
exec book_cursor @bookCursor=@cur output;
fetch next from @cur into @bookID,@bookName,@bookAuth;
while(**@@FETCH_STATUS**=**0**)
begin 
    fetch next from @cur into @bookID,@bookName,@bookAuth;
    print 'bookID:'+convert(varchar,@bookID)+' , bookName: '+ @bookName
            +' ,bookAuth: '+@bookAuth;
end
close @cur    --关闭游标
DEALLOCATE @cur; --释放游标
```
(8).创建分页存储过程
```
if (object_id('book_page', 'P') is not null)
    drop proc book_page
go
create proc book_page(
    @TableName varchar(**50**),            --表名
    @ReFieldsStr varchar(**200**) = '*',   --字段名(全部字段为*)
    @OrderString varchar(**200**),         --排序字段(必须!支持多字段不用加order by)
    @WhereString varchar(**500**) =N'',  --条件语句(不用加where)
    @PageSize int,                     --每页多少条记录
    @PageIndex int = **1** ,               --指定当前为第几页
    @TotalRecord int output            --返回总记录数
)
as
begin
     --处理开始点和结束点
    Declare @StartRecord int;
    Declare @EndRecord int; 
    Declare @TotalCountSql nvarchar(**500**); 
    Declare @SqlString nvarchar(**2000**);    
    set @StartRecord = (@PageIndex-**1**)*@PageSize + **1**
    set @EndRecord = @StartRecord + @PageSize - **1** 
    SET @TotalCountSql= N'select @TotalRecord = count(*) from ' + @TableName;--总记录数语句
    SET @SqlString = N'(select row_number() over (order by '+ @OrderString +') as rowId,'+@ReFieldsStr+' from '+ @TableName;--查询语句
    --
    IF (@WhereString! = '' or @WhereString!=null)
        BEGIN
            SET @TotalCountSql=@TotalCountSql + '  where '+ @WhereString;
            SET @SqlString =@SqlString+ '  where '+ @WhereString;            
        END
    --第一次执行得到
    --IF(@TotalRecord is null)
    --   BEGIN
           EXEC sp_executesql @totalCountSql,N'@TotalRecord int out',@TotalRecord output;--返回总记录数
    --  END
    ----执行主语句
    set @SqlString ='select * from ' + @SqlString + ') as t where rowId between ' + ltrim(str(@StartRecord)) + ' and ' +  ltrim(str(@EndRecord));
    Exec(@SqlString)    
END
--调用分页存储过程book_page
exec book_page 'books','*','book_id','',**3**,**1**,**0**;
--
declare @totalCount int
exec book_page 'books','*','book_id','',**3**,**1**,@totalCount output; 
select @totalCount as totalCount;--总记录数。
```
