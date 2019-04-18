# 打包压缩命令tar - bigfacesafdasgfewgf - CSDN博客





2015年05月03日 20:47:30[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：505标签：[tar																[gzip																[bzip2																[压缩																[解压缩](https://so.csdn.net/so/search/s.do?q=解压缩&t=blog)
个人分类：[Linux](https://blog.csdn.net/puqutogether/article/category/3147399)








    之前介绍的gzip和bzip2都是压缩命令，但是它们只可以压缩单个文件。如果我们需要压缩一个目录中的所有文件的话，就需要使用tar。




    tar的使用简单，关键是做好参数的设置。几个常用的参数配置如下：



**1）tar -jcv -f 压缩文件名.tar.bz2 被压缩文件名**

    第一个文件名是压缩文件名，这个需要用户自己设置，tar命令是不会默认文件名的。-jcv参数表示使用bzip2压缩方式，而且压缩的时候显示处理的文件名。-f必须和被压缩的文件名放在一起。




**2）tar -jtv -f 压缩文件名 **

    其功能主要是查看打包文件的内容有哪些文件名，重点在查看文件名。




**3）tar -jxv -f 压缩文件名 .tar.bz2 -C 解压之后存放的目录**

    -x是解压的参数，-C用在解压时，在特定目录中解压




以上的参数中都是-j，这个是用bzip2方式来压缩解压缩的，如果换成-z，那么就是gzip的方式压缩解压缩。




    tar的参数很多，下面列出一些常见的：

![](https://img-blog.csdn.net/20150503205834593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=压缩&t=blog)](https://so.csdn.net/so/search/s.do?q=bzip2&t=blog)](https://so.csdn.net/so/search/s.do?q=gzip&t=blog)](https://so.csdn.net/so/search/s.do?q=tar&t=blog)




