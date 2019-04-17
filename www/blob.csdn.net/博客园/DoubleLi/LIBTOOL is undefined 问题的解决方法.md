# LIBTOOL is undefined 问题的解决方法 - DoubleLi - 博客园







**[plain]**[view plain](http://blog.csdn.net/sky_qing/article/details/9707647#)[copy](http://blog.csdn.net/sky_qing/article/details/9707647#)

[print](http://blog.csdn.net/sky_qing/article/details/9707647#)[?](http://blog.csdn.net/sky_qing/article/details/9707647#)

- configure.ac:10: error: possibly undefined macro: AC_PROG_LIBTOOL  
- If this token and others are legitimate, please use m4_pattern_allow.  
- See the Autoconf documentation.  
- + automake --foreign --copy --add-missing -Woverride  
- src/common/compress/Makefile.am:9: Libtool library used but `LIBTOOL' is undefined  
- src/common/compress/Makefile.am:9: The usual way to define `LIBTOOL' is to add `AC_PROG_LIBTOOL'  
- src/common/compress/Makefile.am:9: to `configure.ac' and run `aclocal' and `autoconf' again.  
- src/common/compress/Makefile.am:9: If `AC_PROG_LIBTOOL' is in `configure.ac', make sure  
- src/common/compress/Makefile.am:9: its definition is in aclocal's search path.  



这个问题我也找了好久，最后看到两篇文章，记录下来：

观点一：



原因分析
aclocal是个扫描程序, 负责扫描configure.ac中所有的宏定义并展开,
上面产生的原因就是找不到LIBTOOL宏的定义之处造成的.


原因就是aclocal与libtool没有安装在一个相同目录下面

aclocal是去默认 安装目录 /usr/share/aclocal下面搜索所有的.m4文件找所定义的宏,

但是由于安装了多个aclocal,可能aclocal目录不存在,实际目录为/usr/share/aclocal1.10等,
这就造成了aclocal找不到m4文件的情况, 解决办法就是将文件夹aclocal1.10重命名为aclocal.

或者显示指定路径 aclocal -I /usr/share/aclocal1.10 -I /usr/share/libtool/m4 --install

或者把/usr/share/libtool/m4下面的文件都copy至/usr/share/aclocal1.10中.

[http://welcomemht.blog.sohu.com/143343960.html](http://welcomemht.blog.sohu.com/143343960.html)



观点二：

出现这个问题的原因主要有两个：
1.没有安装aclocal
解决方法：安装libtool
2.没有配置正确aclocal的库LIBTOOL.m4的路径
解决方法：
—-查看aclocal的路径aclocal --print-ac-dir
—-如果相应的*.m4文件不在这个目录下，则重新配置路径，或是将相应的*.m4文件复制到这个路径下 （aclocal -I DIR)

[http://www.trueeyu.com/?p=805](http://www.trueeyu.com/?p=805http://www.trueeyu.com/?p=805)









