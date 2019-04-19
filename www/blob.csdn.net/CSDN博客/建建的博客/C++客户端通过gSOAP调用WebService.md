# C++客户端通过gSOAP调用WebService - 建建的博客 - CSDN博客
2017年02月17日 10:35:45[纪建](https://me.csdn.net/u013898698)阅读数：154
个人分类：[Onvif学习](https://blog.csdn.net/u013898698/article/category/6708886)
webService三要素：
SOAP（Simple Object Access Protocol）、WSDL(WebServicesDescriptionLanguage)、UDDI(UniversalDescriptionDiscovery andIntegration)。 SOAP用来描述传递信息的格式， WSDL 用来描述如何访问具体的接口， UDDI用来管理，分发，查询webService 。SOAP 可以和现存的许多因特网协议和格式结合使用，包括超文本传输协议（HTTP），简单邮件传输协议（SMTP），多用途网际邮件扩充协议（MIME）。它还支持从消息系统到远程过程调用（RPC）等大量的应用程序。SOAP使用基于XML的[数据结构](http://lib.csdn.net/base/datastructure)和超文本传输协议(HTTP)的组合定义了一个标准的方法来使用Internet上各种不同操作环境中的分布式对象。
gSOAP: 
1. wsdl2h.exe: 
We use the gSOAP ‘wsdl2h’ WSDL parser to obtain the gSOAP header file specification of a Web service from a WSDL document. 
完成 wsdl文件（.wsdl）到头文件（.h）的转换，即由写好的wsdl格式的接口定义文件转换为C/C++格式的头文件。 
2. soapcpp2.exe： 
根据接口头文件生成应用程序客户端和服务端的框架文件（soapClient.cpp、soapServer.cpp等）。
计算器示例： 
wsdl文件URL地址：[http://www.cs.fsu.edu/~engelen/calc.wsdl](http://www.cs.fsu.edu/~engelen/calc.wsdl)
a) 程序->运行 输入cmd 启动命令行; 
b) cd到wsdl2h.exe所在文件路径; 
c) 输入:wsdl2h.exe -o cal.h [http://www.cs.fsu.edu/~engelen/calc.wsd](http://www.cs.fsu.edu/~engelen/calc.wsd)
-c代表转化成C代码。 
-s代表不使用STL，取而代之需要在工程中包含stdsoap2.h和stdsoap2.cpp。 
-o file代表指定输出文件名称。具体：wsdl2h.exe -h 
d)soapcpp2.exe -i cal.h 
-C代表生成客户端代码。 
-S代表生成服务端代码。 
-L代表不生成soapClientLib/soapServerLib 
-c代表仅生成c代码 -i代表使用Proxy 
通常情况下使用命令soapcpp2 –S/-C –L -i xxx.h 
生成如下文件： 
![生成文件](https://img-blog.csdn.net/20150323160811216)
![添加到项目的文件](https://img-blog.csdn.net/20150323160959730)
![文件简介](https://img-blog.csdn.net/20150323161949915)
