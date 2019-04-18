# wiringpi安装编译问题解决 - 小灰笔记 - CSDN博客





2017年11月13日 21:51:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：520








       想用一下树莓派的硬件输出功能，发现我用的系统版本并没有安装PRi。找了半天也没有找到可以用的安装包，倒是又找到了一个WiringPi的一个驱动包。

       Pip安装失败了，使用git又怕出现断线不能够续传的问题。于是Github下载下来自己安装了一下。在安装WiringPi的时候比较顺利，但是安装WiringPi-Python的时候编译失败，出了一点问题。

       具体的问题如下：

![](https://img-blog.csdn.net/20171113215052836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       感觉就是少了一个文件而已，找了一下在WiringPi中找到了相应的文件，于是在python驱动安装编译环境中创建了相应的目录并添加了缺失的文件。再次尝试，又报错！

![](https://img-blog.csdn.net/20171113215103055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       跟上面的错误很相似，于是直接把WiringPi主目录中的几个从模块连目录带文件添加到了python驱动编译环境中。再次编译，十分顺利！

![](https://img-blog.csdn.net/20171113215115168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       没有看别人使用Git安装的方式过程中是否有脚本取代了我手工操作的这个过程，我想八成会是如此吧！



