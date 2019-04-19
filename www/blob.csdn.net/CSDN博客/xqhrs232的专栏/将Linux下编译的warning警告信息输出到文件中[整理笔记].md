# 将Linux下编译的warning警告信息输出到文件中[整理笔记] - xqhrs232的专栏 - CSDN博客
2017年06月02日 16:46:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：805
原文地址::[http://blog.csdn.net/feihongwang/article/details/6975670](http://blog.csdn.net/feihongwang/article/details/6975670)
相关文章
1、[【整理】将Linux脚本中的正常输出，警告，错误等信息输出到文件中](http://blog.csdn.net/woshinia/article/details/18040063)----[http://blog.csdn.net/woshinia/article/details/18040063](http://blog.csdn.net/woshinia/article/details/18040063)
2、[将Linux脚本中的正常输出，警告，错误等信息输出到文件中](http://blog.csdn.net/nancygreen/article/details/42294119)----[http://blog.csdn.net/nancygreen/article/details/42294119](http://blog.csdn.net/nancygreen/article/details/42294119)
Linux中，脚本语言环境中，即你用make xxx即其他一些普通linux命令，比如ls，find等，不同的数字，代表不同的含义： 
数字 含义标准叫法
0标准输入 stdin = standard input
1标准输出 stdout = standard output
2标准错误输出 stderr = standard error
而系统默认的stdin，stdout，stderr，都是屏幕，所以，当你执行命令，比如make，后，所输出的信息，都是可以在屏幕上看到的。
所以，想要将对应信息输出到某个文件中，就用对应的数字加上重定向符号'>'，实现将这些信息，重新定向到对应的文件中，即可。
（上面这两句，也是之前太多的人来解释，但是没有说明白的地方。）
下面以make命令为例来说明，如何把对应的信息，输出到对应的文件中：
**【用法】**
**1.想要把make输出的全部信息，输出到某个文件中，最常见的办法就是：**make xxx > build_output.txt**此时默认情况是没有改变2=stderr的输出方式，还是屏幕，所以，如果有错误信息，还是可以在屏幕上看到的。2.只需要把make输出中的错误（及警告）信息输出到文件中ing，可以用：**make xxx 2> build_output.txt**相应地，由于1=stdout没有变，还是屏幕，所以，那些命令执行时候输出的正常信息，还是会输出到屏幕上，你还是可以在屏幕上看到的。3.只需要把make输出中的正常（非错误，非警告）的信息输出到文件中，可以用：**make xxx 1> build_output.txt**相应地，由于2=stderr没有变，还是屏幕，所以，那些命令执行时候输出的错误信息，还是会输出到屏幕上，你还是可以在屏幕上看到的。4.想要把正常输出信息和错误信息输出到分别的文件中，可以用：**make xxx 1> build_output_normal.txt 2>build_output_error.txt**即联合使用了1和2，正常信息和错误信息，都输出到对应文件中了。5. 所有的信息都输出到同一个文件中：**make xxx > build_output_all.txt 2>&1**其中的2>&1表示错误信息输出到&1中，而&1，指的是前面的那个文件：build_output_all.txt 。**注意：上面所有的1,2等数字，后面紧跟着大于号'>' ，中间不能有空格****
**具体的使用方法：**
**在编译修改好的UBOOT时，个人喜欢自己建个小shell，这样每次不用敲很多命令，就能搞定了。**
**uboot的shell如下：**
**#!/bin/sh**
**echo "    uboot creating......"echo "distclean ..."    make distclean CROSS_COMPILE=arm-none-linux-gnueabi-**
**echo "make at91sam9260ek_config ..."    make at91sam9260ek_config CROSS_COMPILE=arm-none-linux-gnueabi- 2>warning.txt**
**echo "make all..."    make all CROSS_COMPILE=arm-none-linux-gnueabi- 2>warning.txt**
**echo "copy uboot.bin to /opt/uboot_bin!"    cp u-boot.bin /mnt/hgfs/share/uboot_bin/echo "make OK!"**
**解释：使用的板子是9260，**
**以下语句就将编译过程的中产生的警告信息打印到文件warning.txt中了**
**make at91sam9260ek_config CROSS_COMPILE=arm-none-linux-gnueabi- 2>warning.txt**
make all CROSS_COMPILE=arm-none-linux-gnueabi- 2>warning.txt
