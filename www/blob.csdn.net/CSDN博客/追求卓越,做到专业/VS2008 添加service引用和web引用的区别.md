
# VS2008 添加service引用和web引用的区别 - 追求卓越,做到专业 - CSDN博客


2012年07月13日 11:17:29[Waldenz](https://me.csdn.net/enter89)阅读数：6531


**前言:**
（1）VS2005里提供的Add Web Reference（添加Web服务引用）的功能主要是添加Web Service引用。
（2）VS2008保留了Add Web Reference也是为了版本向前兼容。目前很多项目还是基于.NET Framework 2.0。
（3）VS2008在升级以后为了对.NET Framework 3.0 或 3.5版本上WCF Service Library的支持。增加了Add Service Reference（添加服务引用）功能。Framework3.0 或 3.5 可用。
(4)同时存在Add Service Reference、Add Web Reference 两者情况的项目类型是Web服务程序,包括Web Service项目。普通的控制台等类型默认是没有Add Web Reference 。
**主要区别:**
1. Add Web Reference是由wsdl.exe生成客户端代理的。
Add Service Reference是由svcutil.exe生成客户端代理的。
2. Add Web Reference生成的代理可以被.net1.1或.net2.0的客户端调用.
Add Service Reference生成的代理只能被.net3.0+的客户端调用，而且Add Service Reference后不仅生成代理类，在web.config中还会生成相应的Tag。(WCF需要.net3.0的支持).
**总结:**
这样说来，说是向上兼容，其实并不是真的向上兼容，“服务引用”的作用范围应该更广泛,(如果装上了.net3.0客户端),应该可以以“服务引用”的方式去调用WebService。
如何在vs2008中使用“服务引用”去调用Web Service呢？
假如你要调用一个java编写的WebService类Test.java）
（1）如果你添加的是web引用，vs会自动生成一个“类名+Service”的类去对照Java的类，本例应该是TestService类。调用这个类就相当于调用Java中的Test类。
（2）查如果你添加的是服务引用，vs会自动生成一个“类名+Client”(本人测试是类名+SoapClient)的类去对照Java的类，本例应该是TestCilent类。调用这个类就相当于调用Java中的Test类。


