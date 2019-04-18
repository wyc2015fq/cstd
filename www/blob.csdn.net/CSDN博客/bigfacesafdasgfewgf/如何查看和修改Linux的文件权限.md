# 如何查看和修改Linux的文件权限 - bigfacesafdasgfewgf - CSDN博客





2015年04月29日 09:57:04[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：864标签：[Linux																[文件权限																[chgrp																[chown																[chmod](https://so.csdn.net/so/search/s.do?q=chmod&t=blog)
个人分类：[Linux](https://blog.csdn.net/puqutogether/article/category/3147399)








    Linux中的文件权限很重要，如何查看和修改文件的权限呢？




# 1. 查看权限




    主要使用ls命令，即list查看文件的详细信息。具体ls命令的详细使用方法可以man到，我们经常用到的就是：




ls -al    查看当前目录下所有文件的详细信息；（-a表示所有文件，-l表示详细信息）




那么输出结果是什么呢？如下：




drwxr--r-- 3 root root 4096 Jun 25 08:35 .ssh





上面这条结果就包含了ls查询所有文件的详细信息，那么每个字段都代表什么意思呢？




![](https://img-blog.csdn.net/20150429161304299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





所有的信息包含7列：




**1）文件权限。**文件权限有10个字符，每个字符的解释如下：

![](https://img-blog.csdn.net/20150429162332325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第一个字符代表这个档案是『 目彔、档案戒链接文件等等』：

o 当为[ d ]则是目彔，例如上表档名为『 .gconf』的那一行；

o 当为[ - ]则是档案，例如上表档名为『 install.log』那一行；

o 若是[ l ]则表示为连结档(link file)；

o 若是[ b ]则表示为装置文件里面的可供储存的接口讴备(可随机存取装置)；

o 若是[ c ]则表示为装置文件里面的串行端口讴备，例如键盘、鼠标(一次性读取装置)。





**2）连接数。**

有多少文件名连接到此节点。这个数是和文件系统相关的。




**3）文件所有者。**




**4）文件所属用户组。**




**5）文件大小。**这里的大小单位默认是B。




**6）文件最后被修改时间。**




**7）文件名。**如果文件名之前多一个“ . ”，则代表这个文件为隐藏文件。







# 2. 修改权限

    修改权限主要依靠3个指令，如下：




**1） chgrp：改变文件所属用户组。**




chgrp [-R] 用户组 文件名或目录名

如下：

![](https://img-blog.csdn.net/20150429163328386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**2）chown：改变文件所有者。**




chown 用户名 文件名或目录名

如下：

![](https://img-blog.csdn.net/20150429163308826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





注意：我们在复制文件（cp 源文件 目标文件）的时候会经常遇到问题，因为cp会复制源文件的权限，那么我们就要用chgrp和chown来改变文件拥有者的权限。




**3）chmod：改变文件的完整权限。**




chmod是直接更改文件的那9个字符的权限，我们可以用数字或者是符号来修改。

数字：r——4，w——2，x——1，那么744就是文件拥有者是rwx的权限，文件组成员是r，其他成员也是r的权限。例如：chmod 644 .bashrc



符号：

![](https://img-blog.csdn.net/20150429164004939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





例如：

chmod u=rwx,go=rx .bashrc         user是rwx，group和others是rx；  （注意r和go的设置中间的逗号没有空格）


chmod a+w .bashrc                  在原有权限的基础上，均增加user，group，others的w权限；


chmod a-x .bashrc                     在原有权限的基础上，均去除user，group，others的x权限；







参考：

《鸟哥的Linux私房菜》](https://so.csdn.net/so/search/s.do?q=chown&t=blog)](https://so.csdn.net/so/search/s.do?q=chgrp&t=blog)](https://so.csdn.net/so/search/s.do?q=文件权限&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)




