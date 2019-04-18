# Linux下目录和文件的三种权限 - bigfacesafdasgfewgf - CSDN博客





2015年04月29日 09:06:40[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3966










    在Linux下，目录和文件会有3种权限，分别是：可读r，可写w，可执行x。那么目录和文件的权限有什么区别呢？这篇博文解释的就是这个问题。

    在解释之前，我们应该有一些基础知识：Linux系统下有3种身份（所有组、用户组、其他人），3种权限（r，w，x），3个修改权限的命令（chown, chgrp, chmod）。




# 1. 文件的3种权限




1）read：可**读取文件的内容**，例如读取文本文件的内容。

2）writer：可以编辑、新增或者修改文件的内容，**但是不可以删除该文件**。这里的修改都是基于文件内容的，文件中记录的数据而言的。

3）execute：该文件**可以被系统执行**。这个需要注意，因为Linux和Windows系统不一样。在Windows系统下，文件是否可以被执行是通过扩展名来区别的，例如.exe, .bat, .com等，这些文件类型都是可被执行的；而在Linux下，文件是否可以被执行时通过权限x来标注的，和文件名没有绝对的关系。




# 2. 目录的3种权限




1）read：具有读取目录结构列表的权限。**说明用户可以使用ls来查询该目录的文件列表**。

2）writer：具有更改该目录结构列表的权限。**可以新建新的目录和文件，删除已经存在的文件和目录（不论被删除文件的权限），重命名已有的文件和目录，转移已有的文件和目录位置**。所以说用户对目录的write权限是很重要的，权利也是很大的。

3）execute：用户可以进入该目录，使其成为成为用户当前的工作目录。也就是**可以cd进入该目录操作**。




鸟哥的书中用了一个例子来说明：

![](https://img-blog.csdn.net/20150429094042299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150429094112111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





由于，目录和文件的权限是744，对于root来说都是rwx，所以可以使用ls来看到目录下的文件列表信息。

但是换成其他用户的话，其只有r权限，所以只能读取ls的文件名，详细信息读取不了，如下：




![](https://img-blog.csdn.net/20150429094335725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





（ps：但是我在vbird用户下，使用ls -al就可以看到testing的详细信息，单独看testing文件确实如上图看不了）




之后，我们可以使用chown来修改vbird用户对testing文件的权限，如下：




![](https://img-blog.csdn.net/20150429094635825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





此时vbird用户对testing文件拥有rwx权限，那么就可以实现cd，ls -l， rm等操作了。

注意，对目录有w权限即可删除目录下的文件了!




**注意其中mkdir, ls, cd, rm, chown, touch（建立一个空档案）, su这些常用命令的使用。**




参考：

《鸟哥的Linux私房菜——基础学习篇》












