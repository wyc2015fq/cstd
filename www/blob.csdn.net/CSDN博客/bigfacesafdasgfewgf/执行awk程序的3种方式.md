# 执行awk程序的3种方式 - bigfacesafdasgfewgf - CSDN博客





2015年05月19日 21:28:21[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3960











    awk是Linux系统中一种强大的数据处理工具，执行awk程序有3种方式。介绍如下：




# 1. 命令行方式

    语法形式：**awk ' awk程序语句****' 被处理文件名**

**![](https://img-blog.csdn.net/20150519213352905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**







# **2. 执行awk脚本方式**

    语法形式：**awk -f awk脚本文件名 被处理文件名**

    注意：必须要先vi新建一个awk脚本文件，以.awk结尾，里面编辑awk程序。

![](https://img-blog.csdn.net/20150519213655051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    ex11-2.awk脚本文件中只有一个程序{ print }

# 3. 执行shell脚本文件，不过指定编辑器为awk

    语法形式：先vi编辑shell脚本文件，文件名后缀为.sh，不过在开头需要指定#! /bin/awk，这点和shell脚本不同，shell脚本开头指定的是#! /bin/bash。然后在终端输入：./shell脚本文件 被处理文件.

![](https://img-blog.csdn.net/20150519214122311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    注意：通常我们需要先给予shell脚本文件可执行权限。









