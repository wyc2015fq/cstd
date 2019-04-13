
# Tcl中upvar的用法 - Augusdi的专栏 - CSDN博客


2015年05月08日 23:28:23[Augusdi](https://me.csdn.net/Augusdi)阅读数：6108


﻿﻿
### Abstract:
upvar的用法
**upvar 很象c语言的引用传参**，我用一个例子说明
set a 1 ;\#定义变量a, 并且值设为1
proc test {b} {
upvar $b mya
puts $b
puts $mya
}
test a ;\#调用函数 test
a   ;\#参数b的值为a(变量名）
1  ;\#由于upvar 使mya(变量名）指向a(变量名）指向的同一个变量,mya的为a的值
upvar使的在函数内部可以更改函数外部的变量的值
**注意：Tcl和其他的script语言的不同之处之一，Tcl要区分全局变量和局部变量。**
### upvar跟level的概念相关
level就是堆栈的深度，说简单一点，就是函数调用的层次，举例子说明
proc test_2 {b} {
puts [info level]
}
proc test_1 {a} {
puts [info level]
test_2 b
}
test_1 a
输出是
1
2
upvar中的level表示向上level层，据例子说明
set a aa
set b bb
proc test_2 {a} {
upvar 2 b myb  ;\# test_2被调用，它的层是2，要访问0层的b，需要倒退2层
}
proc test_1 {b} {
upvar a mya  ;\#缺省为1
test_2 sdlfld ；
}
test_1 a  ;\#call the test_1 proc

[http://blog.chinaunix.net/uid-401523-id-2407996.html](http://blog.chinaunix.net/uid-401523-id-2407996.html)

