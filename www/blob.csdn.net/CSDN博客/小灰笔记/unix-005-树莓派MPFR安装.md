# unix-005-树莓派MPFR安装 - 小灰笔记 - CSDN博客





2018年01月25日 22:47:05[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：104








unix-005-树莓派MPFR安装

       这个是我在树莓派上安装gcc遇到问题后才尝试的一次安装，首先也是尝试了apt-get install，没有找到。

       看起来又得通过源代码自己编译，我从下面的网站找到了代码：

[http://www.mpfr.org/mpfr-current/#download](http://www.mpfr.org/mpfr-current/#download)

       下载后，在树莓派中如下：

![](https://img-blog.csdn.net/20180125224510945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       解压：

![](https://img-blog.csdn.net/20180125224530476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       从解压出来的文件中获得信息，编译安装之前需要先完成GMP，好在我这部分已经完成。至于安装，首先需要配置然后进行make。配置如下：

![](https://img-blog.csdn.net/20180125224543042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       看到一个小崩溃的信息，GMP版本不对！

![](https://img-blog.csdn.net/20180125224604527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       重新编译安装了一个新的GMP版本后，再次配置：

![](https://img-blog.csdn.net/20180125224618117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       这一次配置成功，接下来执行make：

![](https://img-blog.csdn.net/20180125224631427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       make比较顺利，接下来make install安装：

![](https://img-blog.csdn.net/20180125224647226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       也比较顺利。



