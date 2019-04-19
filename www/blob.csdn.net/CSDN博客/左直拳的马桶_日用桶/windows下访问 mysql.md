# windows下访问 mysql - 左直拳的马桶_日用桶 - CSDN博客
2018年07月12日 18:39:45[左直拳](https://me.csdn.net/leftfist)阅读数：128
个人分类：[mySql																[应用](https://blog.csdn.net/leftfist/article/category/2778939)](https://blog.csdn.net/leftfist/article/category/2557425)
同事下午问起mysql。时间太长了，还是2016年的事情，根本想不起来。而且就没有怎么熟悉。现在，我连mysql有没有启动，怎么访问都忘记了。
经过一番努力之后，记录一下：
1、windows下，安装mysql数据库的话，操作系统的服务里会建立一个自动启动的启动项。所以，默认情况下，windows里，mysql是自动运行的，不用像tomcat那样，要么到它bin目录里运行批处理文件，要么手动建一个服务。
2、我不记得要不要配置路径了，如果安装的时候，系统没有自动设置的话，那我们应该要设置path，指向mysql的bin，里面有mysql.exe。我们访问mysql就靠它了
3、访问mysql的命令：
```
mysql -u root -p回车
输入密码回车
```
OK了
4、查看mysql版本
`mysql --version`
