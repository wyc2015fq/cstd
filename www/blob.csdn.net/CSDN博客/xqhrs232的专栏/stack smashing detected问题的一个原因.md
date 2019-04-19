# stack smashing detected问题的一个原因 - xqhrs232的专栏 - CSDN博客
2016年06月29日 17:42:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1898
原文地址::[http://blog.sina.com.cn/s/blog_40ac9d4301016c4m.html](http://blog.sina.com.cn/s/blog_40ac9d4301016c4m.html)
相关文章
1、[stack smashing detected 错误的处理](http://blog.csdn.net/qustdjx/article/details/8022474)----[http://blog.csdn.net/qustdjx/article/details/8022474](http://blog.csdn.net/qustdjx/article/details/8022474)
2、[分配空间不足引起的错误：‘Stack smashing detected’](http://blog.csdn.net/xiaobai1593/article/details/7376573)----[http://blog.csdn.net/xiaobai1593/article/details/7376573](http://blog.csdn.net/xiaobai1593/article/details/7376573)

最近在写一个linux程序，需要附带一个命令行参数（含义是某个配置文件的路径）。程序编译为二进制文件后在linux环境下执行，随着命令行参数的不同，有时候执行完全正常，有时候会提示错误“***stack smashing detected***”。
经过一番搜索，终于找到了问题的原因所在：linux程序在某些环境下编译为二进制文件之后，如果运行时提供的命令行参数长度超过了10个字符，则会产生错误提示“***stack smashing detected***”。想办法保证命令行参数不超过10个字符虽然可以避免错误，但终究治标不治本。
其实解决方法很简单，在程序编译的时候，加上一个选项即可："-fno-stack-protector"
举个简单的例子，比如，原来编译时使用的命令为：
gcc -o xxxx source.c
则需要改为：
gcc -o xxxx "-fno-stack-protector" source.c
如果是通过makefile文件进行编译，除了那个生成最终二进制文件的编译命令之外，记得将其中每个“.o”文件的编译命令中也加上"-fno-stack-protector"，最好直接将这个选项加到环境变量中。
附上参考的两个链接：
[http://www.holeeo.com/archives/558](http://www.holeeo.com/archives/558)
[http://disyulei.blog.163.com/blog/static/926142712009424103355392/](http://disyulei.blog.163.com/blog/static/926142712009424103355392/)
