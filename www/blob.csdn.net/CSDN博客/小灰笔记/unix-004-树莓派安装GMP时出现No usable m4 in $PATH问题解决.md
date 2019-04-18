# unix-004-树莓派安装GMP时出现No usable m4 in $PATH问题解决 - 小灰笔记 - CSDN博客





2018年01月25日 22:43:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：166








unix004-树莓派安装GMP时出现No usable m4 in $PATH问题解决

       手动编译安装一个软件可真锻炼人，我这是一连串依赖了好几重了！看到了下面这样的提示：

       No usable m4 in $PATH

       不消说，八成又是缺少库或者是程序了。碰了一下运气，apt-get install可以安装：

![](https://img-blog.csdn.net/20180125224253237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       接下来的GMP继续配置，通过。



