# shc对Shell脚本加密方法及问题处理 - xqhrs232的专栏 - CSDN博客
2017年09月12日 17:15:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：311
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/levy_cui/article/details/52935129](http://blog.csdn.net/levy_cui/article/details/52935129)
相关文章
1、**[shell]采用shc工具加密Shell脚本----[http://lxsym.blog.51cto.com/1364623/768286](http://lxsym.blog.51cto.com/1364623/768286)**
2、Shc____百度百科----[https://baike.baidu.com/item/Shc/815446?fr=aladdin](https://baike.baidu.com/item/Shc/815446?fr=aladdin)
3、linux下利用SHC加密shell教程详解----[https://yq.aliyun.com/ziliao/65848](https://yq.aliyun.com/ziliao/65848)
Shc可以用来对shell脚本进行加密，可以将shell脚本转换为一个可执行的二进制文件。经过shc对shell脚本进行加密后，会同时生成两种个新的文件，一个是加密后的可执行的二进制文件（文件名以.x结束），另一个是C语言的原文件（文件名以.x.c结束）。
下面就说明一下shc的安装,参数，以及使用示例：
下载安装：
(官网下载地址: http://www.datsi.fi.upm.es/~frosal/sources/)
wget http://www.datsi.fi.upm.es/~frosal/sources/shc-3.8.7.tgz
tar -zxvf shc-3.8.7.tgz
cd shc-3.8.7
mkdir -p /usr/local/man/man1/
make
make install
(仅用make就可以完成安装，运行shc时要指出绝对路径；用make install，将shc安装到/usr/local/bin下，可以直接运行）
执行命令：
# shc
shc parse(-f): No source file specified
shc Usage: shc [-e date] [-m addr] [-i iopt] [-x cmnd] [-l lopt] [-rvDTCAh] -f script
常用参数：
-e date
Expiration date in dd/mm/yyyy format [none]（指定过期日期）
-m message
message to display upon expiration ["Please contact your provider"]（指定过期提示的信息）
-f script_name
File name of the script to compile（指定要编译的shell的路径及文件名）
-r Relax security.
Make a redistributable binary which executes on different systems running the same operat-ing system.（可以相同操作系统的不同系统中执行）
-v Verbose compilation（编译的详细情况）
使用示例:
[root@centos54 tmp]# /usr/local/src/shc-3.8.7/shc -e 20/10/2010 -m “lianxi aaa@163.com” -v -r -f ./ex.sh
-e:指定过期时间为2010年10月20日
-m:过期后打印出的信息；
-v: verbose
-r: 可在相同操作系统的不同主机上执行
-f: 指定源shell
方法:
shc -r -f script-name
注意:要有-r选项, -f 后跟要加密的脚本名.
运行后会生成两个文件,script-name.x 和 script-name.x.c
script-name.x是加密后的可执行的二进制文件.
./script-name.x 即可运行.
script-name.x.c是生成script-name.x的原文件(c语言)
加密脚本:
# shc -v -f test.sh
-v是verbose模式, 输出更详细编译日志; 
-f 指定脚本的名称.
# ll test*
-rwxr-xr-x  1 oracle oinstall   1178 Aug 18 10:00 test.sh
-rwx--x--x  1 oracle oinstall   8984 Aug 18 18:01 test.sh.x
-rw-r--r--  1 oracle oinstall  14820 Aug 18 18:01 test.sh.x.c
#file test.sh.x
test.sh.x: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), for GNU/Linux 2.2.5, dynamically linked (uses shared libs), stripped
可以看到生成了动态链接可执行二进制文件test.sh.x和C源文件testup.sh.x.c, 注意生成的二进制文件因为是动态链接形式, 所以在其它平台上不能运行.
生成静态链接的二进制可执行文件:
可以通过下面的方法生成一个静态链接的二进制可执行文件:
# CFLAGS=-static shc -r -f test.sh
# file testup.sh.x
问题处理：
#shc -r -f test.sh
# ./test.sh.x 
报错：
[3]+  Stopped                 ./test.sh.x
需要加个 -T参数：
shc -v -r -T -f test.sh
# ./test.sh.x
成功
shc帮助没有-T参数，但可以使用，也许是版本问题，不再深究。

