# mysql备份及还原 - 左直拳的马桶_日用桶 - CSDN博客
2016年10月11日 19:22:23[左直拳](https://me.csdn.net/leftfist)阅读数：519标签：[mysql																[备份																[数据库																[还原](https://so.csdn.net/so/search/s.do?q=还原&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=备份&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Web																[mySql](https://blog.csdn.net/leftfist/article/category/2557425)](https://blog.csdn.net/leftfist/article/category/674421)
mysql怎样复制数据库？或者说，将数据库拷贝到另外一台机？
按照我的理解，复制数据库，如果是：
1、直接拷贝数据库文件，应该先停数据服务，否则拷不出来。但是，生产机器，哪能说停就停呢？
2、在线拷贝，则循备份-还原的路子。
“不是的，mysql可以直接拷贝数据库文件，无须停服务，俺以前用过”，有个同事说。
哦，有这等好事！mySql果然就是牛。不过我上网搜了下，mysql有innoDB和myISAM两种模式。如果innoDB的话，在线直接拷贝，有可能会对数据库文件造成损坏！我叉。
所以，谨慎起见，还是老老实实的备份、还原的好。
1、备份
DOS窗口，直接敲入
`mysqldump -u root -p test1>c:\temp\dump.txt`
提示：要设置path，才能让系统识别mysql命令。例如：
1） 设置系统变量MYSQL_HOME=C:\Program Files\MySQL\MySQL Server 5.7 
2）PATH += %MYSQL_HOME%\bin;
然后，将dump.txt拷贝到目标服务器
2、还原 
现在我们要将名为test1的还原回来。
打开mysql command line client（牛逼，命令行，昨天装好mysql，我到处找图形界面，还以为自己装错了。其实要图形UI的话，要装一个叫mysql banch的东东）
先创建test1，当然，别的名字也可以的。然后，
```php
mysql -u root -p
mysql>create database test1;
mysql>use test1;
mysql>source c:\temp\dump.txt #注意后面别带;号
```
