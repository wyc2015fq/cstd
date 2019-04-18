# unix-003-树莓派安装GMP - 小灰笔记 - CSDN博客





2018年01月25日 22:41:39[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：80








unix003-树莓派安装GMP

       这个是我升级gcc的时候遇到的一个依赖，直接apt-get install没有找到，直接从下面的网站找到了代码：

[https://gmplib.org/list-archives/gmp-announce/2008-September/000020.html](https://gmplib.org/list-archives/gmp-announce/2008-September/000020.html)

       在树莓派上如下：

![](https://img-blog.csdn.net/20180125223949235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       解压：

![](https://img-blog.csdn.net/20180125223959770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       看上去这不是个很大的软件，不知道会不会比较顺利完成build。看了一下里面的介绍信息，安装应该也是先配置后make。接下来尝试配置：

![](https://img-blog.csdn.net/20180125224012771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       配置中出现错误，还缺少依赖：

![](https://img-blog.csdn.net/20180125224024692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       处理后继续配置：

![](https://img-blog.csdn.net/20180125224037412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       完成配置后make：

![](https://img-blog.csdn.net/20180125224053430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       耗时有点长，但是顺利通过。接下来执行安装：

![](https://img-blog.csdn.net/20180125224106413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       安装过程比较顺利，但是这个需要一定的权限才能够成功：

![](https://img-blog.csdn.net/20180125224127358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



