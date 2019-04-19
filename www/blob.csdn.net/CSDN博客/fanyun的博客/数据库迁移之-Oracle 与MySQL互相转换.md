# 数据库迁移之-Oracle 与MySQL互相转换 - fanyun的博客 - CSDN博客
2017年05月31日 21:22:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：13308
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)

       理论上来说， MySQL 已经被Oracle 收购， 这两者之间的Migrate 应该比较容易， 但实际的迁移还是有一些问题， 以下就说一说一些实现的方式和问题。
**方式一：手动方式导入导出**
手动的方式导入， 就是操作步骤会比较繁琐一些。 
对Table 的结构和数据：
1. 使用 SQL Developer 把 oracle 的 table 的schema 和 Data（.sql 和 .xls） 导出
2. 使用 MySQL 的 WorkBench 创建 Table 和导入数据。
这里语法上会稍微有一些不同，所以需要略微做一些调整。
对于View 来说， 特别是复杂的有子查询的OracleView 说，要导入到MySQL 看起来就不是那么容易了。
**方式二： 使用工具Navicat 进行导入**
Navicat ，这是MySQL 官方网站上有人建议使用的工具。这是一个收费的软件。目前的收费是 1000 到1600 人民币。但是可以免费试用一个月。 
下载安装地址：
 http://download.csdn.net/detail/fanyun_01/9632657
破解软件下载地址：
 http://download.csdn.net/detail/fanyun_01/9632659
启动的页面如下：
![](https://img-blog.csdn.net/20160918123013940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
迁移的流程如下：
1.      新建数据库的连接
![](https://img-blog.csdn.net/20160918123035519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
建立需要迁移的Oracle 和 MySQL 的数据库连接。
另外，建立Oracle 连接的时候还需要下载一个oci.dll 的文件。
下载地址：
[http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html](http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html)
下载之后，解压到某个目录：
在 Navicat  的工具 --> 选项下做类似如下设置：
![](https://img-blog.csdn.net/20160918123133739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
细部可以参见：
[http://wiki.navicat.com/wiki/index.php/Instant_client_required](http://wiki.navicat.com/wiki/index.php/Instant_client_required)
完成设置后，重启 navicat
2.  设置过连接之后， 接下来就可以进行表和数据的migrate 了
点击：工具 --》数据传输
![](https://img-blog.csdn.net/20160918123156765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在 "常规" 的标签页中设置需要 migrate 的连接
![](https://img-blog.csdn.net/20160918123222146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在 “高级” 的标签页中， 设置需要 migrate 哪些具体的内容：
![](https://img-blog.csdn.net/20160918123251756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
配置完成之后，点 "开始" 就可以了。
基本上：对于 Table 的结构和数据的迁移的话，基本上没什么问题。
但是对于 View 的导入，因为MySQL 的View 的语法不能有子查询语句。
在Navite 上，可以看到从 Oracle 导入到 MySQL 的时候， View 的Checkbox 不能选取。
