# 好记性不如烂笔头——WebService与Remoting - 左直拳的马桶_日用桶 - CSDN博客
2014年02月28日 22:44:15[左直拳](https://me.csdn.net/leftfist)阅读数：1609
个人分类：[.NET																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/94497)
一、WebService总体上分为5个层次：
1）HTTP传输信道
2）XML的数据格式
3）SOAP的封装协议，用于传输
4）WSDL的描述方式，用于引用
5）UDDI，通用描述、发现与集成服务，用于定义如何查找 Web 服务（及其 WSDL 文件）
二、Webservice的运行机理
1）首先客户端添加位于服务器的WebService的WSDL
2）在客户端声明一个代理类（Proxy Class），这个代理类负责与WebService服务器的Request和Response。
3）当一个数据（XML格式）被封装成SOAP格式的数据流发送到服务器端，服务器就会生成一个进程对象把这个Request的SOAP包进行解析，然后处理。处理完毕，再对计算结果进行SOAP包装，然后把这个包作为一个Response发送会客户端的代理。同样，这个代理也对发回的SOAP包进行解析、处理。
![](https://img-blog.csdn.net/20140305211158515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三、Remoting
1）既支持TCP信道又支持HTTP信道，传输速度快
2）既可传输XML的SOAP包，又可传输二进制流，效率高
3）不必依赖WEB服务器
四、Remoting开发步骤
1）创建远程对象，必须继承自MarshalByRefObject；
2）创建一个应用程序作为宿主HOST，以接受客户请求，包括注册信道，注册服务器激活或者客户端激活远程对象，运行宿主程序（服务器端开发）；
3）创建一个客户端调用远程对象，包括注册通道，根据URL获取远程对象的代理，使用代理调用远程对象（客户端开发）。
