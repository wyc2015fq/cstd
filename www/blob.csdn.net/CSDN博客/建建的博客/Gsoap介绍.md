# Gsoap介绍 - 建建的博客 - CSDN博客
2017年02月17日 09:34:51[纪建](https://me.csdn.net/u013898698)阅读数：149
最近重温gsoap，发现之前的一些理解有误，这里更新一下。
gsoap客户端代码支持两种实现方式：
1>代理类
2>非代理类的方式。
gSOAP主要包括两个exe：
wsdl2h.exe的作用是根据WSDL生成C/C++风格的头文件;
soapcpp2.exe的作用是根据头文件自动生成调用远程 SOAP服务的客户端代码（称为存根:Stub）和提供SOAP服务的框架代码（称为框架：Skeleton），另外它也能从头文件生成WSDL文件。
1 用 wsdl2h.exe 是将wsdl文件翻译成为.h文件。
   wsdl2h.exe calc.wsdl    --这个命令将本地的calc.wsdl 生成 calc.h
   wsdl2h.exe http://services.xmethods[.NET](http://lib.csdn.net/base/dotnet)/soap/urn:xmethods-delayed-quotes.wsdl 
 --也可以是网络wsdl
   wsdl2h.exe -s -o Calc2.h calc.wsdl abcd.wsdl   --指定多个wsdl生成同一个的文件名为Calc2.h，并且不使用STL
   默认的typemap.dat 从gsoap安装包直接拷贝到wsdl2h.exe所在目录就可以了。type map文件用于指定SOAP/XML中的类型与C/C++之间的转换规则，比如在wsmap.dat里写xsd__string = |   std::wstring | wchar_t*；那么SOAP/XML中的string将转换成std::wstring或wchar_t*，这样能更   好地支持中文。
  自己定义的type map文件可用 -t 选项来指定：  -tfile  use type map file instead of the default file typemap.dat
  如果不用-t选项，默认就用gsoap自带的type map文件 typemap.dat
常用选项：
l        -o 文件名，指定输出头文件
l        -n 名空间前缀 代替默认的ns
l        -c 产生纯C代码，否则是C++代码
l        -s 不要使用STL代码
l        -t 文件名，指定type map文件，默认为typemap.dat
l        -e 禁止为enum成员加上名空间前缀
l        -qname 将使用C++的命名空间，为所有函数指定命名空间
2 用soapcpp2.exe 利用.h文件生成客户端的代码
1>  soapcpp2  -C -x -L calc.h 
 这是非代理类的方式。这种方式会生成两个*BindingProxy.h头文件，但是不会生成cpp文件的，没有什么用的。
相关文件：
soapStub.h
从输入 Header 文件生成的经过修改且带标注的 Header 文件
soapH.h
主 Header 文件，所有客户机和服务源代码都要将其包括在内
soapC.cpp
指定数据结构的序列化器和反序列化器
soapClient.cpp
远程操作的客户机存根例程
stdsoap2.h
为stdsoap2.cpp 运行时库的 Header 文件
stdsoap2.cpp
运行时 C++ 库，带 XML 解析器和运行时支持例程
ServiceSoap11Binding.nsmap
名空间定义，客户端需要包含它
注意：stdsoap2.h 和 stdsoap2.cpp 是从gsoap包中的gsoap-2.8\gsoap 目录下直接复制的。
2> soapcpp2  -i -C -x -L calc.h 
加上-i就可以生成代理类(-j跟-i一样也可以生成代理类，细微区别是-i生成的代理类本身是从soap派生，而-j生成的代理类是将soap作为成员变量来处理)，相关文件如下：
soapC.cpp，soapH.h，soapcalcProxy.cpp，soapcalcProxy.h，soapStub.h，stdsoap2.cpp，stdsoap2.h，calc.nsmap
注：
如果看到soapcpp2提示:”Critical error: #import: Cannot open file "stlvector.h" for reading.“， 那是因为我们的头文件使用了STL(wsdl2h 没用-s选项)，这时要使用**-I**选项指定gSOAP的 import文件路径。如：
soapcpp2  -C -x -L -IC:\webservice\gsoap\gsoap-2.8\gsoap\import    calc.h 
其实用-i生成代理类和生成非代理类的代码本质没什么区别的。代理类中封装了如何去调用webservice API的，默认构造中包括了默认soap对象，直接调用代理类中封装的方法就可以了。对于非代理类，soapClient.cpp中则是API的存根，调用时要先初始化soap对象，然后作为参数传入soapClient.cpp中API的存根。
二 http和https
代理类和非代理类方式都支持http和https。
为了支持https，代理类和非代理类相关代码都必须先生成一个支持https的soap对象，代码大致如下：
/* Init OpenSSL */
soap_ssl_init();
struct soap client_soap;
soap_init(&client_soap);
soap_ssl_client_context(&client_soap,SOAP_SSL_NO_AUTHENTICATION,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
根据此soap对象来支持https的情形大致如下：
代理类：
ServiceSoapProxy service_proxy(&client_soap);
非代理类：
soap_call___ns3__accessService(&client_soap,
            dest_url,
            NULL,
            NULL,
            &response);
除了上面的支持https的soap对象，对于代理类和非代理类代码要支持 HTTPS，还需要在编译的时候对 gSOAP 进行配置。在您的平台上安装 OpenSSL 库，以允许安全 SOAP 客户机使用 HTTPS/SSL。安装完成后，在应用程序工程中添加宏WITH_OPENSSL选项来让gSOAP打开对OpenSSL的支持。然后只需要编译应用程序的所有源文件就可以了。
具体步骤可参考：
https://www.ibm.com/developerworks/cn/webservices/ws-soa-gsoap/
三 代理类和非代理类
参见http://www.cs.fsu.edu/~engelen/soap.html 可知非代理类的方式是一个比较老的使用方式，现在官方推荐使用代理类的方式。
without the *-i* option only old-style service proxies and objectsare generated, which are less flexible and no longer recommended.
四  gsoap支持汉字
如果gsoap要支持汉字，则要执行gsoap为utf8编码，然后在设置soap接口参数之前，都要把字符转换为utf8格式。Web service API接口接收的也是utf8字符串，程序中需要按照需要，将收到的utf8字符串转换为所需要的格式。
soap_init(&client_soap);
soap_set_mode(&client_soap,SOAP_C_UTFSTRING);
