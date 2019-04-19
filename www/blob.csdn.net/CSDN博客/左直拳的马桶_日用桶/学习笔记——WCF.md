# 学习笔记——WCF - 左直拳的马桶_日用桶 - CSDN博客
2014年03月06日 17:05:57[左直拳](https://me.csdn.net/leftfist)阅读数：1456
学了一下WCF，发现怎么跟Web Service这么像！
这个WCF究竟干嘛的？
一查，原来：
“Windows Communication Foundation (WCF) 是由微软发展的一组数据通信的应用程序开发接口，它是.NET框架的一部分，由 .NET Framework 3.0 开始引入，与 Windows Presentation Foundation（WPF） 及 Windows Workflow Foundation（工作流） 并行为新一代 Windows 操作系统以及 WinFX 的三个重大应用程序开发类库。在 .NET Framework 2.0 以及前版本中，微软发展了 Web Service (SOAP with HTTP communication)，.NET Remoting (TCP/HTTP/Pipeline communication) 以及基础的 Winsock 等通信支持，由于各个通信方法的设计方法不同，而且彼此之间也有相互的重叠性（例如 .NET Remoting 可以开发 SOAP, HTTP 通信），对于开发人员来说，不同的选择会有不同的程序设计模型，而且必须要重新学习，让开发人员在用户有许多不便。同时，服务导向架构 (Service-Oriented Architecture) 也开始盛行于软件工业中，因此微软重新查看了这些通信方法，并设计了一个统一的程序开发模型，对于数据通信提供了最基本最有弹性的支持，这就是 Windows Communication Foundation”
也就是说，WCF 集成了原先的asp.net web service，remoting（远程调用）、MSMQ（消息队列），从此我们使用再也不必这么分散，妈妈再也不用担心我们的学习。
