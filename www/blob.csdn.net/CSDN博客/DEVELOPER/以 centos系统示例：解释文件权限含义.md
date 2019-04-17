# 以 centos系统示例：解释文件权限含义 - DEVELOPER - CSDN博客





2017年11月26日 16:12:11[学术袁](https://me.csdn.net/u012827205)阅读数：343
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









![](https://img-blog.csdn.net/20171126153621548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





我们在使用linux系统操作时候，肯定见过这样的诸多目录；有时可能会模棱两可，或者记得不太劳。好，就来解释一下：

对于上图中通过命令：ll ，截取其内容的一部分示意。

对上面使用红色线框圈起来的一次表示：文件/目录权限-文件/目录链接数-文件/目录所有者-文件/目录所在群组所有者-文件/目录大小-创建时间-文件/目录

![](https://img-blog.csdn.net/20171126154236951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后放大部分继续进行解释：

可以看到上图中 NO.1-NO.2-NO.3-NO.4，分为四组，一共10个字符/字母，第一个字母单独一组，其余每三个字母/字符一组。

NO.1（第一组）表示：d：目录、l：链接、-：文件

NO.2-NO.3-NO.4（第二、三、四组）表示：r：可读、w：可写、x：可执行、-：无权限

那么数字权限怎么与这些字母/字符权限进行匹配呢？看下面

![](https://img-blog.csdn.net/20171126155952975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对于 " -rw-r--r-- " 对应的数字权限是 " 644 " ，怎么计算的呢？抛去第一位，从后九位开始计算：r=4、w=2、x=1、-=0；因此每三位一组各个字母代表的数字相加，rw- = 4+2+0、r-- = 4+0+0、r-- = 4+0+0 ,最后的结果就是 "644"；

所以上图的修改权限的结果就是从 " -rw-r--r-- " 到 " -rwxrwxrwx "

![](https://img-blog.csdn.net/20171126161546563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Linux 文件的基本权限有9个，分别是 owner/group/other 这三种身份，他们各自有自己的 read/write/execute  读写执行权限。比如：" -rw-r--r-- " 而对于后三组就分别表示 owner/group/other 这三种身份所分别对应权限。









