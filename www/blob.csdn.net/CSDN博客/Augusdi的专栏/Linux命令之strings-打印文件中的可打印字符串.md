
# Linux命令之strings - 打印文件中的可打印字符串 - Augusdi的专栏 - CSDN博客


2015年08月22日 10:42:40[Augusdi](https://me.csdn.net/Augusdi)阅读数：2235


﻿﻿
## 用途说明
打印文件中的可打印字符串（print the strings of printable characters in files）。常用来在二进制文件中查找字符串，与grep配合使用。strings命令输出的字符串是至少包含连续4个可打印字符的，这个可以通过参数来修改。

## 常用方式

## 使用示例
### 示例一
[root@jfht ~]\#strings /usr/bin/uptime
/lib/ld-linux.so.2
FK9{
PTRhp
[^_]
usage: uptime [-V]
-V    display version
libproc-3.2.7.so
__gmon_start__
_Jv_RegisterClasses
display_version
print_uptime
libc.so.6
_IO_stdin_used
stderr
fwrite
__libc_start_main
memset
_3_2_5
GLIBC_2.0
/lib/ld-linux.so.2
[root@jfht ~]\#

### 示例二
[root@jfht ~]\#strings /usr/bin/uptime | grep GLIB
GLIBC_2.0
[root@jfht ~]\#

### 示例三
[root@jfht ~]\#strings -n 3 /usr/bin/uptime
/lib/ld-linux.so.2
GNU
FK9{
%()
PTRhp
QVh
WVS
t+1
[^_]
tZ1
usage: uptime [-V]
-V    display version
libproc-3.2.7.so
__gmon_start__
_Jv_RegisterClasses
display_version
print_uptime
libc.so.6
_IO_stdin_used
stderr
fwrite
__libc_start_main
memset
_3_2_5
GLIBC_2.0
/lib/ld-linux.so.2
[root@jfht ~]\#

## 问题思考
## 相关资料
【1】xiaobei的博客 linux strings命令的用法
http://laiqingtao1984.blog.163.com/blog/static/11363446820101331952670/
【2】飘渺的风 linux strings命令详解
http://www.huanxiangwu.com/234/linux-strings%E5%91%BD%E4%BB%A4%E8%AF%A6%E8%A7%A3
【3】Prayer linux命令strings
http://www.cppblog.com/prayer/archive/2009/07/22/90836.html


