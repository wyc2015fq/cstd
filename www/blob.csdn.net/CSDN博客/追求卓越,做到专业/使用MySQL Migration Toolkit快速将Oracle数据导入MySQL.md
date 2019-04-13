
# 使用MySQL Migration Toolkit快速将Oracle数据导入MySQL - 追求卓越,做到专业 - CSDN博客


2016年01月07日 11:26:39[Waldenz](https://me.csdn.net/enter89)阅读数：3833


MySQL Migration Toolkit是一款很不错的将其他数据源转换成MySQL的工具。
一、安装MySQL Migration Toolkit
先[下载](http://www.2cto.com/soft)MySQL
 GUI Tools（其实就是一个MySQL管理工具），其中就包括MySQL Migration Toolkit工具，一路next安装完毕。
MySQL
 Migration Toolkit下载[下载](http://download.csdn.net/detail/enter89/9393909)
二、打开时，会检查启动必备条件。如果没有JRE，需要安装
Java Runtime Environment JRE6.0下载[下载](http://download.csdn.net/detail/enter89/9393917)

![](https://img-blog.csdn.net/20160107093708071)
安装JRE后，
![](https://img-blog.csdn.net/20160107104407704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
三、第一次运行需要加载ojdbc14.jar包
运行MySQL
 Migration Toolkit，一路“Next”到“Source Database”，在Database System中选择Oracle Database Server，如果第一次使用会告之要求加载驱动程序ojdbc14.jar，然后重新启动MySQL Migration Toolkit。
ojdbc14.jar下载[下载](http://download.csdn.net/detail/enter89/9393923)
![](https://img-blog.csdn.net/20160107104614312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
四、加载驱动程序之后，来到Source
 Database界面将变成如下的形式，在其中填写Oracle[数据库](http://www.2cto.com/database/)的连接信息，按“Next”继续。
我尝试使用using
 Service没有成功，后来使用using SID成功了。
![](https://img-blog.csdn.net/20160107104809456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
五、在Target
 Database中默认Database System为MySQL Server，在Connection Parameters中填写相应的MySQL数据库的连接信息，按“Next”继续。
![](https://img-blog.csdn.net/20160107105002842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
六、经过Connecting
 to Server测试通过后按“Next”，到Source Schemata Selection，点选准备进行数据迁移的数据库后按“Next”继续。
![](https://img-blog.csdn.net/20160107105058003?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160107105127059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
七.
 经过Reverse Engineering测试通过后按“Next”，在Object Type Selection，点Detailed selection按钮，在下方左侧列表中选择不进行迁移的表，将其放入右侧列表后，即左侧列表剩余的表都将进行数据迁移。选择好之后按“Next”继续。
![](https://img-blog.csdn.net/20160107105226928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
八、在Object
 Mapping的Migration of type Oracle Schema，如果要设置参数，点Set Parameter按钮。如果默认数据库表为UTF8的话，则选择Multilanguage；如果默认数据库表为GBK的话，则需要选择User defined，并在下方填写charset=gbk, collation=gbk_general_ci。
Migration of type Oracle Table中要设置参数点Set Parameter按钮。如果默认数据库表为UTF8的话，则选择Data consistency/multilanguage；如果默认数据库表为GBK的话，则需要选择User defined，并在下方填写addAutoincrement=yes, charset=gbk, collation=gbk_general_ci, engine=INNODB。选择好之后按“Next”继续。
![](https://img-blog.csdn.net/20160107111740427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160107111800308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
九、经过Migration测试通过后，再到Manual Editing，在这里可以修改建表脚本。由于Oracle与MySQL之间语法规则的差异，通常需要对脚本的数据类型以及默认值进行调整，比如Oracle中通常会对Timestamp类型的数据设置默认值sysdate，但在MySQL中是不能识别的。在Filter中选择Show All Objects，然后在Migrated Objects中选择要修改脚本的表，再点击左下方的Advanced就可以进行脚本编辑了。修改完之后点击右侧Apply Changes按钮保存，按“Next”继续。
![](https://img-blog.csdn.net/20160107111847380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
十、在Object
 Creation Options中，选择本地磁盘储存数据表结构，按“Next”继续。
经过Creating Objects创建所有表的结构完毕，表中并没有数据，按“Next”继续。
![](https://img-blog.csdn.net/20160107111956618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
十一、一路“next”来到Data
 Mapping Options，选择本地磁盘储存数据表中的数据，按“Next”继续。
十二、经过Bulk
 Data Transfer创建所有表中的数据完毕，按“Next”继续。
![](https://img-blog.csdn.net/20160107112233045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
十三、来到summary显示此次数据转换的信息，可以保存成文件，按“Finish”完成。
![](https://img-blog.csdn.net/20160107112259566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
需要补充一点，在导大容量数据特别是CLOB数据时，可能会出现异常：“Packets
 larger than max_allowed_packet are not allowed”。这是由于MySQL数据库有一个系统参数max_allowed_packet，其默认值为1048576(1M)，可以通过如下语句在数据库中查询其值：show VARIABLES like '%max_allowed_packet%';修改此参数的方法是在[mysql](http://www.2cto.com/database/MySQL/)文件夹找到my.ini文件，在my.ini文件[mysqld]中添加一行：max_allowed_packet=16777216
重启MySQL，这样将可以导入不大于16M的数据了，当然这数值可以根据需要作调整。
十四、使用MySQL Workbench导入数据
如果前面创建表结构和表数据，都没有选择
 create online,而只是保存到本地文件，则通过Workbench导入数据。
点击Data Import/Restore先导入一次表结构，再导入一次表数据，完成数据库迁移。




