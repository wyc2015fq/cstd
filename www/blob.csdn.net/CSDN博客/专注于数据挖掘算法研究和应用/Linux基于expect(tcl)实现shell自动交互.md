# Linux基于expect(tcl)实现shell自动交互 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月11日 08:44:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：887








                
1、需求：在shell中执行scp命令时，可以自动输入密码，而不用手工交互输入。



2、方案：采用expect来实现。

      Expect是一个基于TCL开发出的语言包。

      而TCL（Tool Command Language）工具脚本语言，是Linux内的一种语言包。

      执行shell脚本，要先安装tcl和expect，安装次序先tcl后expect，expect依赖tcl。

      1)tcl官网：http://www.tcl.tk/

      2)expect官网：http://expect.sourceforge.net/

      下载相应包后解压make install即可。



3、脚本：shell



```
#!bin/sh  
  
expect<<EOF  
set timeout 200  
spawn scp -R /tmp/test.txt username@ip:/tmp
expect "password:"   
send "123456\r"  
expect eof  
EOF
```


其中要复制的目的端主机，用户名是username，密码是123456。


有兴趣可以继续研究expect。



