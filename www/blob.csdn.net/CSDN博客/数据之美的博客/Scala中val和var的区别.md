# Scala中val和var的区别 - 数据之美的博客 - CSDN博客
2018年02月07日 13:09:24[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：118
Scala有两种变量，val和var。val就不能再赋值了。与之对应的，var可以在它生命周期中被多次赋值。
val的演示： 
scala> val msg=”Hello,world!”; 
msg: java.lang.String = Hello,world!
scala> msg=”Hello again,world!”; 
:5: error: reassignment to val 
msg=”Hello again,world!”; 
^
可以看出如果再次给val的变量赋值就会报错！
var的演示： 
scala> var msg2=”Hello,world!”; 
msg2: java.lang.String = Hello,world!
scala> msg2=”Hello again,world!”; 
msg2: java.lang.String = Hello again,world!
