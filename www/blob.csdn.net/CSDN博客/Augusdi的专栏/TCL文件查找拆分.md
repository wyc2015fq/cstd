
# TCL文件查找拆分 - Augusdi的专栏 - CSDN博客


2015年10月13日 13:17:55[Augusdi](https://me.csdn.net/Augusdi)阅读数：1741个人分类：[Tcl/Tk																](https://blog.csdn.net/Augusdi/article/category/2574303)


﻿﻿
TCL文件查找拆分
我现在有一个 .res 的文件，这个文件中包括以下两行测试结果：
rdly=  3.4405E-10  targ=  2.0084E-07   trig=  2.0050E-07
fdly=  3.1211E-10  targ=  1.0081E-07   trig=  1.0050E-07
现在我想写一个Tcl文件拥有功能如下：
1. 读这个 .res 文件并寻找到上述的那两行信息（这两行所在的行数未知）。
2. 生成一个 ASCII 的 .txt 文件，文件内容应当如下：
Rise Delay is [rdly 的测试结果，在这里应当是3.4405E-10]
Fall Delay is [fdly 的测试结果，3.1211E-10]

```python
set srcfile myres.res
set dstfile dtest.txt
if {[catch {open $srcfile r} res]} {
 puts "Failed to open the source file: $srcfile; $res"
 return 0
}
set hSrc $res
if {[catch {open $dstfile w+} res]} {
 puts "Failed to open the destination file: $dstfile; $res"
 return 0
}
set hDst $res
foreach line [split [read $hSrc] \n] {
 if {[regexp {rdly=(.*)targ=(.*)trig=(.*)} $line match sub1]} {
  puts $hDst "Rise Delay is $sub1"
 }
 if {[regexp {fdly=(.*)targ=(.*)trig=(.*)} $line match sub1]} {
  puts $hDst "Fall Delay is $sub1"
 }
}
close $hSrc
close $hDst
```


