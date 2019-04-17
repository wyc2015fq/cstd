# Linux学习笔记（三）：文件解压缩与tar的用法 - Machine Learning with Peppa - CSDN博客





2018年03月10日 21:23:04[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：126标签：[Linux																[压缩																[文件																[文件系统																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[系统：编译与底层																[***系统与网络***																[系统：Linux与Shell](https://blog.csdn.net/qq_39521554/article/category/7479173)
所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)](https://blog.csdn.net/qq_39521554/article/category/7361345)






### 什么是压缩？

简单的说，你可以理解为，其实原有文件里面有相当多的空余空间存在（在bit里以0表示），并不是完全填满的， 而压缩技术就是将这些浪费的空间填满，以让整个文件占用的容量下降。

不过，这些压缩过的文件并无法直接被操作系统所使用，因此， 若要使用这些被压缩过的文件数据，则必须将他还原回来未压缩前的模样， 那就是所谓的解压缩。而压缩前与压缩后的文件所占用的磁碟空间大小， 就可以被称为是压缩比。




一般来说，我们在Linux中常见的压缩格式文件有以下几种：

*.Z         compress 程序压缩的文件；
*.gz        gzip 程序压缩的文件；
*.bz2       bzip2 程序压缩的文件；
*.tar       tar 程序打包的数据，并没有压缩过；
*.tar.gz    tar 程序打包的文件，其中并且经过 gzip 的压缩
*.tar.bz2   tar 程序打包的文件，其中并且经过 bzip2 的压缩





自从上一代Linux之后，gzip 与 bzip2 命令已经取代了传统的compress 。 后来 GNU 又开发出 bzip2 这个压缩比更好的压缩命令。不过这些命令通常仅能针对一个文件来压缩与解压缩，如此一来， 每次压缩与解压缩都要一大堆文件。因此，tar命令就显的很重要了。

这个 tar 可以将很多文件『打包』成为一个文件！甚至是目录也可以。不过，单纯的 tar 功能仅是打包而已，亦即是将很多文件集结成为一个文件， 事实上，他并没有提供压缩的功能，后来在新的版本中，将整个 tar 与压缩的功能结合在一起，如此一来提供使用者更方便并且更强大的压缩与打包功能。

那么tar是什么，又到底怎么用呢？




### tar的用法



tar是UNIX/Linux系统上的压缩文件格式,tar文件则是一种压缩文件,在Linux系统中可以直接解压使用这种压缩文件。在Windows下也可以使用WinRAR等常见的解压缩软件打开。其使用参数及命令如下：
[root@www ~]# tar [-j|-z] [cv] [-f 创建的文件名] filename... <==打包与压缩
[root@www ~]# tar [-j|-z] [tv] [-f 创建的文件名]             <==察看档名

[root@www ~]# tar [-j|-z] [xv] [-f 文件名] [-C 目录]   <==解压缩
选项与参数：
-c  ：创建打包文件，可搭配 -v 来察看过程中被打包的filename
-t  ：察看打包文件的内容含有哪些文件
-x  ：解打包或解压缩的功能，可以搭配 -C (大写) 在特定目录解开，特别留意的是， -c, -t, -x 不可同时出现在一串命令列中。
-j  ：透过 bzip2 的支持进行压缩/解压
-z  ：透过 gzip  的支持进行压缩/解压
-v  ：在压缩/解压缩的过程中，将正在处理的文件名显示出来

-C 目录    ：这个选项用在解压缩，若要在特定目录解压缩，可以使用这个选项




其中，最常用的只有这三个命令：

压　缩：tar -jcv -f filename.tar.bz2 要被压缩的文件或目录名称
查　询：tar -jtv -f filename.tar.bz2


解压缩：tar -jxv -f filename.tar.bz2 -C 欲解压缩的目录




tips：有时候压缩和解压一起做会方便很多，比如将/dir/file打包后直接解开在/dir2下面，而不用产生文件


>>> tar cvf - /dir/file | tar xvf - /dir2 




参考资料：

[鸟哥的Linux私房菜](http://cn.linux.vbird.org/linux_basic/0240tarcompress_3.php)](https://blog.csdn.net/qq_39521554/article/category/7484528)](https://so.csdn.net/so/search/s.do?q=文件系统&t=blog)](https://so.csdn.net/so/search/s.do?q=文件&t=blog)](https://so.csdn.net/so/search/s.do?q=压缩&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)




