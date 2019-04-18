# webservice里面wsdl后缀和asmx是干嘛用的，区别是什么？ - z69183787的专栏 - CSDN博客
2015年09月08日 12:46:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：19512
Web Service也叫XML Web Service WebService是一种可以接收从Internet或者Intranet上的其它系统中传递过来的请求，轻量级的独立的通讯技术。是:通过SOAP在Web上提供的软件（服务），使用WSDL文件进行（说明），并通过（UDDI）进行注册。
XML：(Extensible Markup Language)扩展型可标记语言。面向短期的临时数据处理、面向万维网络，是Soap的基础。
Soap：(Simple Object Access Protocol)简单对象存取协议。是XML Web Service 的通信协议。当用户通过UDDI找到你的WSDL描述文档后，他通过可以SOAP调用你建立的Web服务中的一个或多个操作。SOAP是XML文档形式的调用方法的规范，它可以支持不同的底层接口，像HTTP(S)或者SMTP。
WSDL：(Web Services Description Language) WSDL 文件是一个 XML 文档，用于说明一组 SOAP 消息以及如何交换这些消息。大多数情况下由软件自动生成和使用。
.asmx是webservice服务程序的后缀名，ASP.NET 使用.asmx 文件来对Web Services的支持。.asmx 文件和.aspx文件一样都属于文本文件。它包含在.aspx文件之中，成为ASP.NET应用程序的一部分。
