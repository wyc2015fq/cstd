# Erlang中的OTP简要 - 我相信...... - CSDN博客





2015年03月07日 11:10:26[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1907








OTP包含了一组库和实现方式，可以构建大规模、容错和分布式的应用程序，包含了许多强大的工具，能够实现H248，SNMP等多种协议，核心概念是OTP行为，可以看作一个用回调函数作为参数的应用程序框架，类似一个J2EE容器。行为负责解决问题的非函数部分，回调函数负责解决函数部分。



通过gen_server模块可以实现事物语义和热代码交换，

1)      确定回调模块名

2)      编写接口函数

3)      在回调模块里编写6个必需的回调函数

当服务器崩溃时，需要一种机制来检测并重启它，要用到监测树，即创建一个监控器来管理服务器。监测树有两种：一对一和一对多。

$erl –boot start_sasl 

会创建一个运行生产系统的环境，系统架构支持库（SASL，System Administration Support Libriaries）将负责错误记录和过载保护等工作。



使用gen_server， gen_supervisor，application等行为，可以构建可靠性为99.9999999的系统。

统一化的erlang消息：

1）  抽象了不同线路协议之间的区别

2）  Erlang消息无需解析，接收进程不必先解析消息再处理，而http服务器就必须解析就收到的所有消息

3）  Erlang消息可以包含任意复杂度的数据类型，而http消息必须被序列化成扁平化才能传输

4）  Erlang消息可以在不同处理器之间传送

常见的第三方库有rebar（https://github.com/basho/rebar）和cowboy(https://githun.com/extend/cowboy)。 Rebar是管理erlang项目的事实标准，用户可以通过rebar创建新项目、编译项目、打包它们，以及把它们与其他项目整合在一起，同时集成了github。Cowboy是一个用erlang编写的高性能web服务器，是嵌入式web的热门实现。另外，库mochiweb2（http://github.com/mochi/mochiweb）的编码和解码方法可以实现json字符串和erlang数据类型的相互转换。



Erlang程序在多核CPU上运行

1）  使用大量进程

2）  避免副作用，例如不使用共享式ETS或DETS

3）  避免顺序瓶颈，可以选择pmap代替map

4）  小消息，大计算

5）  用mapreduce使计算并行化

mapreaduce是一个并行高阶函数，定义如下

-specmapreduce(F1,F2,Acc0,L) ->Acc

         F1 = fun(Pid,X) ->void

         F2 = fun(Key,[Value],Acc0) ->Acc

         L = [X]

         Acc = X =term()

Mapreduce 是在并行高阶函数（phofs）模块中定义的。



