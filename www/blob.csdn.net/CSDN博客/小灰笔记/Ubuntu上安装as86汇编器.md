# Ubuntu上安装as86汇编器 - 小灰笔记 - CSDN博客





2017年08月20日 18:43:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：596








       我使用的Linux版本是Win10中的Ubuntu子系统，为了学习Linux内核现在需要as86汇编器。

       测试了一下命令行，发现没有响应的命令。于是又试了链接了命令，依然没有找到。于是借助搜索引擎查了几份英文网页，发现这几个命令其实是在同一个软件包，bin86。于是尝试了sudo apt-get install bin86，安装很顺利！

       安装后查看其文档也已经有了：

as86(1)                            General CommandsManual                           as86(1)



NAME

       as86 - Assembler for 8086..80386processors



SYNOPSIS

       as86 [-0123agjuw] [-lm[list]] [-n name][-o obj] [-b[bin]] [-s sym] [-t textseg] src



       as86_encap prog.s prog.v [prefix_][as86_options]



DESCRIPTION

       as86 is  an  assembler for the 8086..80386 processors, it's syntax is closer to the

       intel/microsoft form rather than themore normal generic  form  of  the  unix system

       assembler.



       The src file can be '-' to assemble thestandard input.



       This assembler can be compiled tosupport the 6809 cpu and may even work.



       as86_encap  is a  shell  script to call as86 and convert the createdbinary into a C

       file prog.v to be included in or linkedwith programs  like  boot block  installers.

       The prefix_ argument is a prefix to be added to all variables defined by thesource,

       ……

       尝试进行代码的编译，也已经通过。具体如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls

boot.s

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$as86 -0 -a -o boot.o boot.s

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls

boot.o  boot.s



