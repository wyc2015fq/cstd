# Linux下如何压缩与解压文件 - bigfacesafdasgfewgf - CSDN博客





2015年05月03日 17:24:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：519标签：[Linux																[压缩																[gzip																[bzip2																[compass](https://so.csdn.net/so/search/s.do?q=compass&t=blog)
个人分类：[Linux](https://blog.csdn.net/puqutogether/article/category/3147399)








    Linux的压缩和解压是可以通过吗，命令来完成的。压缩文件的扩展名大部分是：*.tar, *.tar.gz, *.tar.bz2, *.tgz, *.gz, *.Z（虽然在Linux下文件的扩展名没有特殊的意义，但是为了更好地方便记忆，这里的压缩文件扩展名的不同可以对应不用的指令，就是我们看到哪个扩展名，就知道使用哪种压缩和解压指令）。

![](https://img-blog.csdn.net/20150503174540619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    常见的压缩和解压有如下几种命令：




# 1. Compass




    非常老旧的一种压缩命令，现在几乎都不再支持。




    压缩：compass 文件或目录

    解压：uncompass 文件名.Z    （注意，compass命令只能解压*.Z扩展名的文件）



    在默认的情况下，被compass压缩的源文件会不见了，而压缩文件会被创建起来，而且扩展名会是*.Z。





compass已经很少有人使用了，因为这个程序无法解开*.gz的文件，而gzip则可以解开.Z的文件。




# 2.  gzip




    gzip可以说是应用最为广泛的压缩命令了，目前gzip可以解开compass, zip, gzip等软件所压缩的文件。

    gzip新建的压缩文件是*.gz的文件名。




    压缩：gzip [-cdtv#] 文件名;




![](https://img-blog.csdn.net/20150503175340757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    解压：gzip -d 文件名




下面可以通过实际的命令来操作压缩功能：

![](https://img-blog.csdn.net/20150503175526449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在默认的情况下，原本的文件会被压缩成为.gz的文件名，源文件就不再存在了。

那么如果保留源文件呢？

![](https://img-blog.csdn.net/20150503182209336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


解压也是使用gzip和-d参数：




![](https://img-blog.csdn.net/20150503181251183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# 3. bzip2




    比gzip具有更好的压缩比，使用方法类似。

![](https://img-blog.csdn.net/20150503182609046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=bzip2&t=blog)](https://so.csdn.net/so/search/s.do?q=gzip&t=blog)](https://so.csdn.net/so/search/s.do?q=压缩&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)




