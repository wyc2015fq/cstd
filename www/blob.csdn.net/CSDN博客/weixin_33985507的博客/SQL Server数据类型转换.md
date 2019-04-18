# SQL Server数据类型转换 - weixin_33985507的博客 - CSDN博客
2016年03月25日 11:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：18
　　在SQL Server日常的函数、存储过程和SQL语句中，经常会用到不同数据类型的转换。在SQL Server有两种数据转换类型：一种是显性数据转换；另一种是隐性数据转换。下面分别对这两种数据类型转换进行简要的说明：
### **1 显式转换**
　　显示转换是将某种数据类型的表达式显式转换为另一种数据类型。常用的是CAST 和 CONVERT 函数。
-  CAST： CAST ( expression AS data_type ) 
-  CONVERT： CONVERT (data_type[(length)], expression [, style])
　　参数 expression 是任何有效的 Microsoft SQL Server表达式。data_type 目标系统所提供的数据类型，不能使用用户定义的数据类型。
### 2 隐性转换
　　隐性转换对于用户是不可见的，由SQL Server 引擎自动处理。 隐性转换自动将数据从一种数据类型转换成另一种数据类型。例如，如果一个 smallint 变量和一个 int 变量相比较，这个 smallint 变量在比较前即被隐性转换成 int 变量。 当从一个 SQL Server 对象的数据类型向另一个转换时，一些隐性和显式数据类型转换是不支持的。例如，nchar 数值根本就不能被转换成 image 数值。nchar 只能显式地转换成 binary，隐性地转换到 binary 是不支持的。nchar 可以显式地或者隐性地转换成 nvarchar。 
### 3 隐性转换的风险
　　隐性转换有的时候非常方便，可以简化SQL 脚本，但是这里面也孕育着潜在的风险，可能会出现在脚本一开始运行的时候都是正常的，但却某一个时间点之后，程序莫名出现错误。下面举一个现实项目中的例子来说明。在SQL Server 2008中有一个表，需要从两个不同的数据表中拉取数据，由于这两个数据表属于不同的系统，其主键类型是不同的，一个是int类型，一个是GUID，一开始想着这两个都可以转换成字符类型进行存储。所以就在表中建立一个nvarchar(50)的混合ID列作为主键。如下图所示：
![](https://images2015.cnblogs.com/blog/76497/201603/76497-20160322114718761-964906029.jpg)
　　一开始拉取的数据并未有GUID的值，都是INT类型转换过来的数据，所以SQL脚本运行的正常，但是突然某一次运行时，出现了“在将 nvarchar 值 '4C185367-F004-41FE-8A0A-DB4E819B1FF2' 转换成数据类型 int 时失败。”的错误。如下图所示：
![](https://images2015.cnblogs.com/blog/76497/201603/76497-20160322114740526-752817851.jpg)
 　　定位到脚本，执行的SQL如下：
```
1 select * from dbo.Demo where 混合ID=305
```
　　其中主键中的数据有GUID转换的字符型，也有INT转换的字符串，示例数据如下：
![](https://images2015.cnblogs.com/blog/76497/201603/76497-20160322114730261-242409213.jpg)
　　但是如果执行下面的SQL，则都是正常执行：
```
1 select * from dbo.Demo where 混合ID=305 and 名称='INT'
2 
3 select * from dbo.Demo where 混合ID=305 and 序号='2'
4 
5 select * from dbo.Demo where 混合ID=305 and 序号=2
6 
7 select * from dbo.Demo where 混合ID='305' and 名称='INT'
8 
9 select * from dbo.Demo where 混合ID='305'
```
 　　结果如下：
![](https://images2015.cnblogs.com/blog/76497/201603/76497-20160322114752604-281988261.jpg)
　　出现上述错误的结果应该是这样的：
　　select * from dbo.Demo where 混合ID=305在执行时，SQL Server会将nvarchar类型的隐性转换成int类型，如果数据中没有GUID类型的字符，则转换正常，如果有，当进行GUID字符到INT的隐性转换时，则转换失败。
![](https://images2015.cnblogs.com/blog/76497/201603/76497-20160325081925558-39742334.png)
