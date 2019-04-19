# ln: creating symbolic link XXXXXX : Operation not supported - xqhrs232的专栏 - CSDN博客
2016年06月08日 22:24:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：240
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/f22jay/article/details/7402315](http://blog.csdn.net/f22jay/article/details/7402315)
相关文章
1、ln-s 提示 Operation not supported 咋回事呢?----[http://bbs.csdn.net/topics/220034969](http://bbs.csdn.net/topics/220034969)

VmWare的Linux + Windows XP + vmhgfs文件共享
【问题】
在编译VMware下的Linux系统对从Windows中共享过来的文件，进行编译的时候，遇到：
ln: creating symbolic link XXXXXX : Operation not supported
【解决办法】
出现这类问题，主要是由于在编译的时候，要用ln去建立一些软链接，
而这些文件是从Windows中，通过VMWare虚拟机共享进Linux的，
而虽然此种操作在Linux系统中很常见，但Windows不支持，所以，
编译会报错。
有个解决办法就是，在VMWare下的Linux中，建立Samba服务，
然后新创建新samba用户和文件夹，然后在windows中就可以访问到该文件夹了。
然后把在Linux中，从共享目录拷贝到你所要共享的samba目录中，
这样，也可以实现我们所要的文件共享。
此时在去编译这些代码的时候，由于是在Linux系统中的，所以就OK了。
//================================================================================================================================
备注::
1>不把文件放windows与linux共享的文件夹下编译就不会产生这样的错误！！！
ation not sup回事 Operation
 not supported 咋回事呢?
