
# SQL Developer 工具小试 - David Dai -- Focus on Oracle - CSDN博客


2015年03月18日 17:15:27[Dave](https://me.csdn.net/tianlesoftware)阅读数：12242




# 1  安装配置
一直都是用Toad 来管理数据库，今天小研究了一下SQL Developer工具，安装好oracle 之后，默认就安装了SQL Developer，安装目录是$ORACLE_HOME/sqldeveloper,第一次启动时会提示需要输入Java的目录，注意这里要求是JavaSE，JRE不行。

![](https://img-blog.csdn.net/20150318171026592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果是JRE,就会报如下错误。
![](https://img-blog.csdn.net/20150318171201180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
JavaSE 下载地址：
[http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
当然也可能会出现另一种错误：
![](https://img-blog.csdn.net/20150318171038198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果报上面的错误，因为SQL Developer版本和JavaSE 版本不一致，比如javaSE 是64位的，SQL Developer 是32位的。这个问题可以修改java版本，或者从官网重新下载SQL Developer版本，这个工具是可以单独下载的，下载完成后替换掉$ORACLE_HOME/sqldeveloper就可以了。
下载地址：
[http://www.oracle.com/technetwork/developer-tools/sql-developer/downloads/index.html](http://www.oracle.com/technetwork/developer-tools/sql-developer/downloads/index.html)
安装完之后如下：
![](https://img-blog.csdn.net/20150318171212146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 2  测试

## 2.1            新功能
![](https://img-blog.csdn.net/20150318171050538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在启动界面，讲了SQL Developer中的新功能，第一眼看上去，还是有一点的管理功能，这点比PL/SQL dev 强点。

## 2.2            连接数据库
![](https://img-blog.csdn.net/20150318171226826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
标准连接界面，没啥好说的。

## 2.3            编写SQL
![](https://img-blog.csdn.net/20150318171234735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里除了可以执行SQL，还可以对SQL 进行性能分析，查看执行计划，通过这个去优化SQL，就可以省点力气了。

## 2.4            查看报告
报告功能是重点，至少在做性能分析的时候，很有用，免去了自己敲代码的苦恼。直接看图，非常直观，不多说。
![](https://img-blog.csdn.net/20150318171112300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150318171249072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150318171126340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




功能这么强大，有没有弊端？  看下图：
![](https://img-blog.csdn.net/20150318171301006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对内存消耗比较严重啊，不过8G 以上的机器，跑这个，应该还是无压力。
总结：
（1）  SQL Developer 可以执行SQL 语句，并提供对SQL 的优化。
（2）  具备查看报告的功能，注意这里是查看，就是我们可以看到这些性能相关的指标，但是不能修改，所以更多的是提供一些建议值。
（3）  Java的工具，有点耗内存。


--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

