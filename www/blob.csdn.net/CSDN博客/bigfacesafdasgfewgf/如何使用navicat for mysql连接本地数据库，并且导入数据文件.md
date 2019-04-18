# 如何使用navicat for mysql连接本地数据库，并且导入数据文件 - bigfacesafdasgfewgf - CSDN博客





2014年11月30日 08:58:16[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：10376








**如何使用navicat for mysql连接本地数据库，并且导入数据文件**

# 1. 简介

    navicat for mysql是一种数据库管理工具, 专门用于简化, 开发和管理MySQL。我们可以使用navicat for mysql来连接远程数据库，也可以连接本地数据库。使用这个工具，查看表的数据非常方便，也可以输入SQL命令来实现查询。但是输入创建表等命令就不是很方便了。好了，我们一起来看看怎么使用navicat for mysql 创建并查询数据库中的数据。

# 2. 连接数据库

    首先，你得确认你已经安装了MySQL Server 5.1和navicat for mysql。这个安装是很简单的，网上很多教程，和安装一般软件差不多。只有在安装MySQL Server 5.1时，要注意选择字符编码为gb2312（中文）那个选项。好了，安装这里就不说了。

    安装好了之后，打开MySql Commend Line Client，输入密码，如下图：

![](https://img-blog.csdn.net/20141130091044968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    现在我们就进入到了MySql的命令窗口了。我们可以在这里创建一个数据库和一个表：



```
mysql> create database mydata;
mysql> use mydata;
mysql> create table StudentScore
       -> (name varchar(10) primary key,
       -> number int(10),
       -> department varchar(10),
       -> gender varchar(10),
       -> birth int(10),
       -> score int(10)
       -> );
```


结果如下图：

![](https://img-blog.csdn.net/20141130092048640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





上面的几行命令就已经创建好了一个mydata的数据库，里面有一个StudentScore的表，表里面的有如下几列：name, number, department, gender, birth, score. 也定义好了每个类型以及主键。




# 3. 使用navicat for mysql导入数据文件

    打开navicat for mysql，和localhost本地数据库连接，我们就可以看到刚才建立的数据库和表，如下图：

![](https://img-blog.csdn.net/20141130092821546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    我们可以导入本地的txt数据文件，如下：（这里我钻研的还不是很深，之前都是一直使用远程数据库，导入本地的txt文件试了一下，是可以的，注意保持格式正确）

![](https://img-blog.csdn.net/20141130093108900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





下面一步要注意一下，如果你的数据库中有中文数据，编码格式一定要选择是中文的GB2312.如下图：

![](https://img-blog.csdn.net/20141130095134141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





然后间隔符为空格（根据txt中的具体情况来定）。并选择目标表，将每一列一一对应，即可导入，如下图：

![](https://img-blog.csdn.net/20141130100419479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
















