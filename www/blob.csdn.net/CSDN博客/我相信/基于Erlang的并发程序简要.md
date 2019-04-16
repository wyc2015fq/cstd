# 基于Erlang的并发程序简要 - 我相信...... - CSDN博客





2015年03月06日 13:39:11[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1381








 Erlang中基本的并发函数

1）  Pid =spwan(Mod,Func,Args) 创建一个新的进程来执行apply（Mod,Func,Args）,与调用进程并列运行，会使用最新的代码定义模块。

2）  Pid！Message 向Pid进程异步发送Message，！为发送操作符

3）  Receive … end 接收消息

receive

           Pattern1[when Guard1]-> Expression1;

           Pattern2[whenGuard2]->Expression2;

…

         aftertime->

                   Expressions

         end.

内置函数erlang:system_info(process_limit)可找出所允许的最大进程数，默认为262144.

进程注册的内置函数有：

register（AnAtom，Pid）用名称注册Pid

uregister（AnAtom） 注销关联注册

whereis(AnAtom)->Pid|undefined 检查Pid是否注册

registered()->[AnAtom::atom()]返回系统里所有注册进程的列表。



并发程序模板：

-module(ctemplate).

-compile(export_all).



start() ->

         Spwan(?MODULE,loop,[]).



rpc(Pid,Request) ->

         Pid! {self(),Request},

         receive

                   {Pid,Respone}->

                            Response

         end.



loop(X) ->

         receive

                   Any->

                            Io:format(“Received:~p ~n”, [Any]),

                            loop(X)

         end.



每当收到消息时会处理它并再次调用loop(),这一过程称为尾递归，无需消耗堆栈空间可以一直循环下去。



Erlang并发程序的错误处理建立在远程监测和处理错误的基础上，重点在补救而不是预防，几乎没有防御性代码，只有在错误后清理系统的代码，即让其他进程修复错误和任其崩溃。



程序在出错时立即崩溃的优点：

1）  不编写防御性代码，直接崩溃简单

2）  别人来修复

3）  不会使错误恶化

4）  第一时间举旗示意

5）  修复时不担心原因重在清理

6）  简化了系统架构

监视和连接类似，但监视是单向的，如果被监视的进程挂了会向监视进程发一“宕机“消息，而不是退出信号。基本错误处理函数有：

-spec spwan_link(Fun) ->Pid

-spec spwan_monitor(Fun)-> {Pid,Ref}

-spec process_flag(trap_exit,true) 

-spec link(Pid) ->true

-spec unlink(Pid) -> true

-spec erlang:monitor(process,Item) ->Ref

-spec exit(Why) -> none()






分布式模型：分布式erlang 和基于socket的分布式模型。分布式erlang运行在可信网络，通常在同一局域网的集群上，并受防火墙保护。基于socket的分布式模型基于TCP/IP不可信网络.

分布式Erlang的主要问题在于客户端可以自行决定在服务器上分裂出多种进程，适合于你拥有全部的机器，并且想在单台机器上控制他们。lib_chan 模块让用户能够显式控制自己的机器分裂出哪些进程。



为了在互联网上执行并发程序：

1）  确保4369端口对TCP和UDP都开发，该端口保留给epmd（Erlang端口映射守护进程）

2）  选择1个或一段连续的端口给分布式erlang使用，确保这些端口开放，例如：

$erl  -name …-setcookie …  -kernelinet_dist_listen_min Min \

           Inet_dist_listen_maxMax

Rpc提供了许多远程调用服务，global里的函数可以用来在分布式系统里注册名称以及维护一个全连接的网络。

Erlang集群就是一组带有相同cookie的互连节点。创建cookie的三种方法：

1）  在文件$HOME/.erlang.cookie存放相同的cookie

2）  在Erlang启动时，可以用 –setcookie，例如

$erl  -setcookieABCDEFG2048

3）  内置函数erlang:set_cookie(node(),C)在程序中指定



Erlang通过名为端口的对象与外部程序通信，如果想端口发送一个消息，这一消息就会被发往与端口相连的外部程序，来自外部程序的消息会变成来自端口的Erlang消息。创建端口的进程成为端口的相连进程，所有发往端口的消息都必须标明相连进程的PID，所有来自外部程序的消息都会发往相连进程。








