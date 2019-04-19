# mysql数据的备份恢复和还原 - fanyun的博客 - CSDN博客
2016年03月18日 14:04:08[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：524
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**1.数据的备份，恢复和还原准备：**
（1）将要导入的.sql文件移至bin文件下，这样的路径比较方便
（2）同上面导出的第1步
（3） 进入MySQL：mysql
 -u 用户名 -p
　　 如我输入的命令行:mysql -u root-p (输入同样后会让你输入MySQL的密码)
（4）在MySQL-Front中新建你要建的数据库，这时是空数据库，如新建一个名为news的目标数据库
（5）输入：mysql>use目标数据库名
　　如我输入的命令行:mysql>usenews;
（6）导入文件：mysql>source导入的文件名;
　　如我输入的命令行：mysql>sourcenews.sql;
MySQL备份和还原,都是利用mysqldump、mysql和source命令来完成的。
**1.Win32下MySQL的备份与还原**
（1）备份
　　开始菜单 |运行 | cmd |利用“cd\\Program
 Files\\MySQL\\MySQL Server 5.0\\bin”命令进入bin文件夹 |利用“mysqldump
-u用户名 -p databasename >exportfilename”导出数据库到文件，如mysqldump
 -u root -p voice>voice.sql，然后输入密码即可开始导出。
（2）还原
　　进入MySQL Command Line Client，输入密码，进入到“mysql>”，输入命令"showdatabases;"，回车，看看有些什么数据库;建立你要还原的数据库，输入"create
 database voice;"，回车;切换到刚建立的数据库，输入"usevoice;"，回车;导入数据，输入"source
 voice.sql;"，回车，开始导入，再次出现"mysql>"并且没有提示错误即还原成功。
**2.Linux下MySQL的备份与还原**
**（1）备份**
[root@localhost ~]# cd/var/lib/mysql (进入到MySQL库目录，根据自己的MySQL的安装情况调整目录)
[root@localhost mysql]#mysqldump -u root -p Test>Test0809.sql，输入密码即可。
**（2）还原**
　　方法一：
[root@localhost ~]# mysql-u root -p回车，输入密码，进入MySQL的控制台"mysql>"，同1.2还原。
　　方法二：
[root@localhost ~]# cd/var/lib/mysql (进入到MySQL库目录，根据自己的MySQL的安装情况调整目录)
[root@localhost mysql]#mysql -u root -pTest
moodle_bak.sql是需要恢复的文件名
