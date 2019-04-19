# WCF学习笔记——不支持内容类型 text/xml; charset=utf-8 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月22日 15:47:21[左直拳](https://me.csdn.net/leftfist)阅读数：8030
我在使用WCF的时候，客户端运行报错：
**不支持内容类型 text/xml; charset=utf-8**
原因是WCF服务做了修改。刷新客户端的服务引用，问题消失
============================================================
使用WCF，就要在运行的时候保证WCF服务已经有宿主承载并激活。
如何才能保证WCF服务已经激活？网上有许多教程，什么自承载，WINFORM应用程序承载，控制台应用程序承载，IIS承载，很多，并且看起来有点复杂。
可能是现在版本比较新吧，使用VS2012，新建WCF服务应用程序，会自动生成契约文件IService1.cs 和 寄宿于IIS的文件Service1.svc。
然后在客户端添加服务引用，点“发现”，处于同一解决方案的WCF服务可以被发现，添加，那么客户端的配置文件会自动被修改。
现在，配置就已经弄好了，根本不需要修改什么。
然后在客户端代码里使用：
```
//ServiceReference1 是服务引用名称
            using (ServiceReference1.Service1Client client = new ServiceReference1.Service1Client())
            {
                string re = client.GetData(0);
            }
```
运行一切OK，就这么简单。
==============================================================
这种寄宿于IIS的WCF服务，要能被发现和引用，似乎要在类开头加一句
[AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
