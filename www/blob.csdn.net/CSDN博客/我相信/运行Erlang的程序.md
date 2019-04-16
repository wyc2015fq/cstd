# 运行Erlang的程序 - 我相信...... - CSDN博客





2015年03月06日 12:44:16[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：5697








运行Erlang程序的方式：

1)      在Erlang shell 中编译执行

2)      Shell 脚本执行，例

Hello.sh

#!/bin/sh

Erl –noshell –pa /home/abel/practice/erlang/code –s hllstart –s init stop

3)      作为Escript 运行，例

#!/usr/bin/env escript

Main(args)->

Io:format(“Hello world ~n”)



内置函数apply能调用某个模块的某个函数并传参。每个erlang进程都有一个被称为进程字典的私有数据存储区。为了增强类型的表达能力，可以用描述性变量给它们加上注解，类型规范为spec，类型说明type。通过dialyzer可以检查程序中的类型错误，最好写模块时先考虑类型并声明它们，然后编写代码。两个载入路径的函数：

-spec code:add_patha(Dir)=>true|{error:bad_directory} 载入路径头加入

-spec code:add_pathz(Dir)=>true|{error:bad_directory} 载入路径尾加入

通过os:cmd(command)可以在erlang中调用shell的脚本,查找标准库源码的命令code:which(file).




Make 是erlang的任务自动化工具，可以通过它来运行程序。下面是一个简单的makefile：

.SUFFIXES: .erl .beam

.erl .beam:

        erlc  -W $<

ERL = erl –boot start_clean

MODS = module1 module2 module3

all: compile

        $(ERL)  –pa    ‘home/abel/…/dir’–s module1 start



compile: ${MODS:%=%.beam}



clean:

        rm  -rf *.beam erl_crash.dump



如果Erlang程序崩溃了，会留下一个erl_crash.dump文件，可以通过web故障分析器来分析，命令如下：

1>    crashdump_viewer:start().



