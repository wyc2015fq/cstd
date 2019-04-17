# unix-006-树莓派编译安装MPC - 小灰笔记 - CSDN博客





2018年01月25日 22:51:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：84
个人分类：[unix](https://blog.csdn.net/grey_csdn/article/category/7424831)









unix-006-树莓派编译安装MPC

       MPC是编译安装gcc的一个依赖，为了安装gcc，从下面网站找到了源代码：

[https://ftp.gnu.org/gnu/mpc/](https://ftp.gnu.org/gnu/mpc/)

       在树莓派上如下：

![](https://img-blog.csdn.net/20180125224927624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       解压：

![](https://img-blog.csdn.net/20180125224938107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       看了上面的文件，根据经验应该依然是先配置，然后make，最后安装。尝试配置如下：

![](https://img-blog.csdn.net/20180125224948740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       配置比较顺利，接下来make，遇到如下错误：

![](https://img-blog.csdn.net/20180125225000229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       看起来不应该有问题，可能不同的标准规定不一样。我自己做了一点小处理，先编译过去后面再说。后面还出现了类似的问题，一并处理。之后，make没有问题：

![](https://img-blog.csdn.net/20180125225013779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       接下来make install安装：

![](https://img-blog.csdn.net/20180125225058640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


       比较顺利：

![](https://img-blog.csdn.net/20180125225108429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       之前欺骗方式完成了编译，修改的头文件再次复原。



