
# ubuntu 14.04中文显示乱码问题 - 嵌入式Linux - CSDN博客

2015年09月22日 16:42:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2317


乱码显示如下：
[<E9><97><AE><E9><A2><98><E6><8F><8F><E8><BF><B0>]:<E5><A2><9E><E5><8A><A0>tm100<E9><A1><B9><E7><9B><AE>
[<E8><AF><84><E5><AE><A1><E4><BA><BA>]:<E9><9F><A6><E5><90><AF><E5><8F><91>
正常显示如下：
[问题描述]:增加tm100项目
[评审人]:韦启发

修改：

vim ~/.bashrc

在最后一行添加
export LESSCHARSET=utf-8

:wq
. ~/.bashrc

生效有用


