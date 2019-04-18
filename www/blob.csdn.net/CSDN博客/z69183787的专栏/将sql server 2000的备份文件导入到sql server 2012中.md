# 将sql server 2000的备份文件导入到sql server 2012中 - z69183787的专栏 - CSDN博客
2013年11月11日 13:43:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3045
## 问题
在学习sql server 索引的时候，发现找到的很多资料都用的是一个Credit的数据，但是我不知道Credit数据库是从哪里来的。后来在[聚集索引与非聚集索引详解](http://gocom.primeton.com/blog10698_1221.htm?PHPSESSID=%3C)这篇文章中找到了如下信息：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
USE CREDIT 
　　go 
　　-- These samples use the Credit database. You can download and restore the 
　　-- credit database from here: 
　　-- http://www.sqlskills.com/resources/conferences/CreditBackup80.zip 
　　-- NOTE: This is a SQL Server 2000 backup and MANY examples will work on 
　　-- SQL Server 2000 in addition to SQL Server 2005. 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
顺利下载这个文件并解压缩以后发现这是一个sql server 2000的备份文件，后缀名是.bak。不过我用的是sql server 2012，使用如下方法来恢复这个数据库备份文件：打开SSMS->database->右键选择restore files and filegroups，然后按如下图所示操作：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313273232.png)
但是执行这个操作会报错，错误如下图所示：主要是说明版本差异问题，我的bak文件是8.0版本（sql server 2000），但是要导入到11.0版本（sql server 2010）中，因此报错。
![](http://pic002.cnblogs.com/images/2012/80896/2012070313285364.png)
## 解决方案
因为源文件版本过低，而目标数据库版本过高，这样的问题一个好的解决方案是首先降低目标数据的版本，然后一步一步往上提升版本。所以这里我们用使用sql server 2008作为过渡数据库。我们将上面的CreditBackup80.bak这个数据备份文件还原到sql server 2008中，假设还原在sqlserver2008中的数据库名称为Credit2008，然后在 sqlserver 2008中backup这个Credit2008数据库，默认会备份到C:\Program Files\Microsoft SQL
 Server\MSSQL10.SQLEXPRESS\MSSQL\Backup这个目录下面，备份文件名成为CreditBackup100.bak，最后将此备份文件CreditBackup100.bak还原到sql server 2012中。
## PS：使用SSMS将sqlserver2008的bak文件还原到sqlserver2012中出现错误
错误1：启动sqlserver2012->右键database->restore files and filegroups->在source for restore中选择from device，然后选择CreditBackup100.bak这个文件的路径，但是当我们指定到C:\Program
 Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\Backup这个目录的时候，发现找不到任何文件，如下图所示：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313082071.png)
解决错误1：这是因为C:\Program Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\Backup这个目录有权限设定，我们查看这个目录的属性可以发现这个目录有一个特殊的用户叫做：T-WEIXU-W7\SQLServerMSSQLUser$t-weixu-w7$SQLEXPRESS。也就是说只有这个用户才有访问这个目录下的文件的权限。所以我门可以在windows下将C:\Program
 Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\Backup目录下的CreditBackup100.bak文件拷贝到其他目录，比如我现在拷贝的目录是：D:\CreditBackup80。
错误2：在解决上述错误以后，我们在source for restore中选择from device，并且选定了CreditBackup100.bak这个文件，最后点击OK，但是会提示错误：
The operating system returned the error '5(Access is denied.)' while attempting 'RestoreContainer::ValidateTargetForCreation' on ....。具体如下图所示：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313310762.png)
从上述错误我们发现是因为我们想将sqlserver2008的bak文件还原到sqlserver2012的时候，却默认的被还原到原先的sqlserver2008的目录下，从路径“C:\Program Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\DATA\Credit2012.mdf”我们可以知道这个就是sqlserver2008实例的目录。
解决错误2：造成这样错误的主要原因是因为我们在机器上安装了sqlserver2008和sqlserver2010两个版本，但是SSMS默认情况下只是让数据还原到sqlserver2008的目录下，我们必须指定sqlserver2012的目录，具体操作是：在restore file and
 filegroups界面中选中options，然后选定sqlserver2012实例的存放目录，如下图所示：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313335061.png)
点击箭头所示的按钮（注意：必须通过点击按钮来指定目标位置的地址，如果手动修改路径的字符串，依然会报错。），会弹出如下错误：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313341416.png)
指定Credit2012的数据文件和日志文件的存放目录，如下图所示：
![](http://pic002.cnblogs.com/images/2012/80896/2012070313350466.png)
完成上述操作以后点击确定，就能够正确完成数据库还原操作，最终将sqlserver2000的bak文件还原到了sqlserver2012中来。
解决错误2-方法2：
在sqlserver configure manager中选中sqlserver2012的properties，然后修改log on as中的属性，使用build-in account中的local system，具体参考：[http://social.msdn.microsoft.com/Forums/en-SG/sqldatabaseengine/thread/7b259f06-e1e2-47f5-8303-77775ee80400](http://social.msdn.microsoft.com/Forums/en-SG/sqldatabaseengine/thread/7b259f06-e1e2-47f5-8303-77775ee80400)。
