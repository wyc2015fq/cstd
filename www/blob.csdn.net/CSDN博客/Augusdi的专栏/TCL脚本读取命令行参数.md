
# TCL脚本读取命令行参数 - Augusdi的专栏 - CSDN博客


2015年05月08日 23:25:47[Augusdi](https://me.csdn.net/Augusdi)阅读数：9343


﻿﻿
1：Tcl脚本的执行
A：获取tclsh（linux）、tclsh.exe（windows）的安装路径
B：脚本第一行书写 “\#!C:\Tcl\bin\tclsh” 或者 “\#!/usr/bin/tclsh” 所有解释性语言(shell、perl)开头都是这玩意。
C：Linux 下直接./test.tcl 就可以了。windows下要使用 "tclsh test.tcl"否则会弹出一个页面，看不到输出结果（我使用的是Active Tcl）。
2：获取命令行参数
和C语言一样，Tcl中有两个默认变量，$argc 存储命令行参数的个数。list $argv中包含了参数信息。
举例如下：
\#!C:\Tcl\bin\tclsh
set i 0
while {$i < $argc} {
\#lindex 命令用于取出list中指定索引的参数
set arg [lindex $argv $i]
puts "$arg"
\#incr 命令用于对变量进行加操作
incr i 1
}

