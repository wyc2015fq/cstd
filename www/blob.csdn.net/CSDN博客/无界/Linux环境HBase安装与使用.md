
# Linux环境HBase安装与使用 - 无界 - CSDN博客

置顶2014年01月25日 21:54:51[21aspnet](https://me.csdn.net/21aspnet)阅读数：29181


原文出自本人另一个Linux博客[http://blog.csdn.net/unix21/article/details/18776073](http://blog.csdn.net/unix21/article/details/18776073)
Linux环境：CentOS6.5
HBase版本：hbase-0.94.16
HBase官网：[http://hbase.apache.org](http://hbase.apache.org)
**1.下载解压安装文件**
\#wget http://mirror.bit.edu.cn/apache/hbase/stable/hbase-0.94.16.tar.gz
\#tar xvfz hbase-0.94.16.tar.gz

**2.启动HBase**
\#/usr/hbase-0.94.16/bin/start-hbase.sh
如果报错
+======================================================================+
|      Error: JAVA_HOME is not set and Java could not be found         |
+----------------------------------------------------------------------+
| Please download the latest Sun JDK from the Sun Java web site        |
|       > http://java.sun.com/javase/downloads/ <                      |
|                                                                      |
| Hadoop requires Java 1.6 or later.                                   |
| NOTE: This script will find Sun Java whether you install using the   |
|       binary or the RPM based installer.                             |
+======================================================================+
如图所示
![](https://img-blog.csdn.net/20140125213843562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
需要修改HBase的配置文件，配置文件在conf文件夹下
\#vi conf/hbase-env.sh
增加JDK的位置，以本机为例JDK被放在/usr/lib/jdk下

```python
export JAVA_HOME=/usr/lib/jdk/jdk1.7.0_51/
```
需要说明的是，JDK是要自己安装的，如果系统已经自带JDK是需要删除重新自己安装比较好。
关于JDK的卸载与安装可以参考这里：
### Linux环境安装卸载JDK
不然修改配置文件启动还是没用的！

重新启动HBase
\#/usr/hbase-0.94.16/bin/start-hbase.sh
如果成功启动会有如下提示信息：

```python
starting master, logging to /usr/hbase-0.94.16/bin/../logs/hbase-root-master-localhost.localdomain.out
```
![](https://img-blog.csdn.net/20140125214759531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3.查看管理界面**
运行在端口http://localhost:60010
![](https://img-blog.csdn.net/20140125214931578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样就可以使用HBase了。

**4.使用HBase Shell命令行**
可以通过HBase Shell和HBase交互
\#/usr/hbase-0.94.16/bin/hbase
![](https://img-blog.csdn.net/20140125225029734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
\#/usr/hbase-0.94.16/bin/hbase shell
这说明Java和HBase函数库已经安装成功，列出HBase中的所有表可以使用list命令。
![](https://img-blog.csdn.net/20140125225216546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**建表**
![](https://img-blog.csdn.net/20140125225446500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**输入数据**
![](https://img-blog.csdn.net/20140125225510828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
scan可以扫描全表

**5.HBase常用命令以及建表**
![](https://img-blog.csdn.net/20140125225748531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**复杂的表**
![](https://img-blog.csdn.net/20140125235050828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
建表命令
create 'GDP','city','code','count'
put 'GDP','SH','code:','021'
put 'GDP','BJ','code:','010'
put 'GDP','GZ','code:','020'
put 'GDP','SH','count:pop','2500'
put 'GDP','SH','count:area','6340'
put 'GDP','SH','count:gdp','20100'
put 'GDP','BJ','count:pop','2100'
put 'GDP','BJ','count:area','16410'
put 'GDP','BJ','count:gdp','19500'
put 'GDP','GZ','count:pop','1275'
put 'GDP','GZ','count:area','7434'
put 'GDP','GZ','count:gdp','13500'

![](https://img-blog.csdn.net/20140125235123421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140125235137093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
分别查出所有信息和SH的信息。

**6.使用REST访问HBase**

启动REST服务
\#/usr/hbase-0.94.16/bin/hbase rest
![](https://img-blog.csdn.net/20140126130404328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

启动REST服务，监听9999端口
\#/usr/hbase-0.94.16/bin/hbase rest start -p 9999
![](https://img-blog.csdn.net/20140126130416796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在浏览器以及可以访问HBase的REST了
http://localhost:9999/
![](https://img-blog.csdn.net/20140126130841390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
http://localhost:9999/version
![](https://img-blog.csdn.net/20140126130851500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用curl访问HBase的REST
\#curl -H "Accept: application/json" http://192.168.2.111:9999/GDP/SH/
![](https://img-blog.csdn.net/20140126130859843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

访问“SH”的数据
\# curl -H "Accept: application/json" http://192.168.2.111:9999/GDP/SH/
\# curl -H "Accept: application/json" http://192.168.2.111:9999/GDP/SH/count:area
![](https://img-blog.csdn.net/20140126131311750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意输出是JSON格式数据，以Base64编码字符串形式返回的，我们可以验证其中的：
MDIx解码就是021
MDEw解码就是010
就是之前录入的上海和北京的区号。

﻿﻿

