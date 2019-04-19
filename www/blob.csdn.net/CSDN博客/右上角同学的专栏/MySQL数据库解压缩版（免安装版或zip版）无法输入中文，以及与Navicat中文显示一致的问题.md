# MySQL数据库解压缩版（免安装版或zip版）无法输入中文，以及与Navicat中文显示一致的问题 - 右上角同学的专栏 - CSDN博客
2016年05月21日 19:42:54[右上角徐](https://me.csdn.net/u011032983)阅读数：541
首先明确此问题属于编码方式设置问题。 
MySQL的默认编码是Latin1，不支持中文，要支持中文需要把编码修改为gbk或者utf8。
     安装版本为 
mysql 5.6.24解压缩版（免安装版或zip版）。
character_set_client为客户端编码方式；
character_set_connection为建立连接使用的编码；        
character_set_database数据库的编码；
character_set_results结果集的编码；
character_set_server数据库服务器的编码；
只要保证以上采用的编码方式一样，就不会出现乱码问题（事实证明character_set_database的编码格式不影响输入 ）
当我们把zip文件格式解压到指定目录后，并且完成基本环境配置后，打开mysql 5.6.24会发现名为【my-default.ini】的文件。我们用记事本打开该文件会发现并没有【default-character-set
 = xxx】或者【character_set_server=xxx】，怎么回事呢？网上给出的许多方法都是直接修改这句话的xxx为utf8。所以执行下面的步骤：
1、
复制该文件，并且命名为【my.ini】，这样根目录下就有两个.ini文件了，一个是my-default.ini，一个是my.ini 。接下来我们只需修改my.ini里面的内容就行了。请往下看：
2、
修改【my.ini】,
在[mysqld]下面添加 character_set_server=utf8，注意是【utf8】不是【utf-8】而且后面没有任何标点符号；
在【sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES】这句话后面再添加三行：
[client]
port=3306
default-character-set=utf8
3、
经过以上三步就ok了。保存后重启服务器，登陆后在命令行输入：【show variables like 'char%';】可查看默认字符集如下图所示：
![](https://img-blog.csdn.net/20160627214254653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
数据库编码方式修改成功。
     但是执行添加语句，当包含中文字段时，还会出现错误提示。
执行show create table student；发现表格的编码方式依旧是Latin1.，如下图。
![](https://img-blog.csdn.net/20160627214143734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所以需要修改表格编码方式，用的语句是alter table student default character set utf8，执行后：
![](https://img-blog.csdn.net/20160627214121666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     但是执行添加语句还是有错误提醒，原来数据库可以指定表级别的编码，行级别编码，甚至可以指定字段级别的编码！！！所以需要继续修改指定字段的编码方式，执行alter table student change sname sname varchar(50) character set
 utf8，然后再次插入数据，如下图：
![](https://img-blog.csdn.net/20160627205940797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
MySQL命令行端插入成功！
另一方面：
Navicat要实现和MySQL命令行端一致，需设置该连接的连接属性为“自动”，如下图所示：![](https://img-blog.csdn.net/20160627210042122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
表格显示：
![](https://img-blog.csdn.net/20160627210116669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
至此，全部流程设置完毕！
