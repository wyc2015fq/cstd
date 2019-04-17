# 安装nghttp2 报错error: Libtool library used but 'LIBTOOL' is undefined - DoubleLi - 博客园






nghttp2 报错error: Libtool library used but ‘LIBTOOL‘ is undefined

如果重新安装libtool和autoconf升级到2.69后，还是报错，

则进行下面的操作：

1,查看aclocal的路径 aclocal --print-ac-dir

显示/usr/local/share/libtool/m4

ls看看里面没有m4文件。

则copy /usr/share/libtool/m4里面的m4文件到此目录下。



2，cd到nghtt2目录

 执行

libtoolize 

aclocal

autoheader



3，重新执行

$ autoreconf -i

$ automake

$ autoconf

$ ./configure xxxx

$ make











