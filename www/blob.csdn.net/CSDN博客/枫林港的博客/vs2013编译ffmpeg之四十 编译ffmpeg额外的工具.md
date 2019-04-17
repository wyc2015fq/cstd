# vs2013编译ffmpeg之四十 编译ffmpeg额外的工具 - 枫林港的博客 - CSDN博客





2018年07月10日 23:35:18[枫林港](https://me.csdn.net/u013601132)阅读数：34








# 编译ffmpeg所依赖的额外工具

## texi2html

texi2html可以将.texi文件转换成html。没有找到现成的二进制文件，从[http://download.savannah.gnu.org/releases/texi2html/](http://download.savannah.gnu.org/releases/texi2html/)下载。

解压后在msys下执行configure，成功后要修改makefile，将makefile里面的：

```bash
msgexec -i "$$file" "$(srcdir)/gettext_to_separated.pl"
```

改成：

```bash
msgexec -i "$$file" "perl" "$(srcdir)/gettext_to_separated.pl"
```

不改编译的时候会报错，原因是不能在shell下直接执行pl文件，要用perl来执行。

然后make; make install；configure时没有指定参数–prefix的话，默认是安装在\MinGW\msys\1.0\local下面。

## makeinfo

[http://ftp.gnu.org/gnu/texinfo/](http://ftp.gnu.org/gnu/texinfo/)

makeinfo最开始是texinfo的一部分，msys用的版本是4.13。从texinfo-5.0开始，makeinfo变成了个perl脚本，其实就是texi2any的软连接。

ffmpeg里面有下面的规则：

```
[ 0$(makeinfo --version | grep "texinfo" | sed 's/.texinfo0-9([0-9])../\1/') -ge 5 ] \

    && enable makeinfo_html || disable makeinfo_html
```

makeinfo的版本号首位要大于5才会打开makeinfo_html标志。但从texinfo-5.0开始，makeinfo变成了perl脚本，根本就没有–version参数，不知道ffmpeg是怎么考虑的。

texinfo的编译很简单，在msys下执行：configre; make；中间没有遇到错误。

## netpbm

这里主要是描述如何将TS流中的subtitle字幕存成png格式的图片。

首先需要安装netpbm，下载地址：[http://gnuwin32.sourceforge.net/packages/netpbm.htm](http://gnuwin32.sourceforge.net/packages/netpbm.htm)，下载完整的安装版本：netpbm-10.27.exe，安装的时候会提示要选择mingw所在的目录，要安装在mingw下面的。这个功能需要用pnmtopng将数据转换成pnm格式的图片。

其次是需要将相关的代码打开，在dvbsubdec.c里面，这个功能的代码包含在DEBUG宏定义里面，可以ffmpeg在configure的时候加上–extra-cflags=-DDEBUG （加了之后对应会很多），也可以直接在dvbsubdec.c里面定义上DEBUG。



