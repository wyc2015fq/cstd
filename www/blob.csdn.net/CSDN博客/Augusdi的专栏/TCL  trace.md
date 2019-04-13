
# TCL: trace - Augusdi的专栏 - CSDN博客


2015年10月13日 17:48:29[Augusdi](https://me.csdn.net/Augusdi)阅读数：1682


﻿﻿
#### Abstract:
Tcl的trace用法

###### 先看一下info level的用法
info level 返回函数调用的深度，main的level是0，调用一层函数加1
如果level的值为>0，则下面的命令返回函数调用的信息。
假设在main调用test a b
set level [info level]
puts "[info level $level]"
会得到 test a b
###### 被trace安装的过程会在低一层的level调用
例如：
main程序里
trace variable i1 w traceproc
set i1 1
proc traceproc (variableName arrayElement operation} {
puts "[info level]"
}
结果为 1
如果变量在第一层的函数里被调用，返回为2
其它的文档说得很明白了。

