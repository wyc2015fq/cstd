# gSOAP使用 - 建建的博客 - CSDN博客
2017年02月17日 11:29:20[纪建](https://me.csdn.net/u013898698)阅读数：393
个人分类：[Onvif学习](https://blog.csdn.net/u013898698/article/category/6708886)
# gSOAP使用
Kagula
## 环境
 [1]VS2008+SP1
 [2]WinSP+SP3
 [3] gsoap_2.8.8.zip
## 正文
以实例形式描述，如何用gSOAP Toolkit实现Web Service功能，本文还捎带了如何用C#访问用gSoap with c++写的Web
 Service服务。
我习惯上把所有的SDK放在自己计算机的E:\SDK路径下，你可以指定其它路径。
**准备工作**
[1]从参考资料中下载gsoap_2.8.8.zip并解压到“**E:\SDK\gsoap-2.8**”
[2]“E:\SDK\gsoap-2.8\gsoap\bin\win32”下有**wsdl2.h.exe、soapcpp2.exe**，你也可以自己编译出这两个工具。下面的步骤[3]、[4]可选。
[3]打开“**E:\SDK\gsoap-2.8**\gsoap\VisualStudio2005\wsdl2h\wsdl2h.sln”编译出
“**E:\SDK\gsoap-2.8**\gsoap\VisualStudio2005\wsdl2h\release\wsdl2h.exe”
[4]打开“**E:\SDK\gsoap-2.8**\gsoap\VisualStudio2005\soapcpp2\soapcpp2.sln”文件无法编译通过，参考资料[2]编译通过。
[5] 在VS2008中加入头文件搜索路径“E:\SDK\gsoap-2.8\gsoap”
**第一个Web Service服务端(C++)**
[1]新建Win32控制台项目“MyFirstWS”。定义头文件“MyInterface.h”，输入你要暴露给WS客户端的方法声明，内容如下
int   ns__sqrt(double a, double &result);
[2]转到源文件所在的目录，运行命令“**soapcpp2  -S  MyInterface.h**”生成存根文件
将新生成的C++的源码文件加入到你的工程项目中，soapServerLib.cpp文件不用加。你需要修改自动生成的**ns.wsdl**文件，因为里面的Web
 Service侦听端口默认为80，但是我们一般不使用默认侦听端口。
[3]加入“**E:\SDK\gsoap-2.8**\gsoap”下的“stdsoap2.h、stdsoap2.cpp、dom.cpp”三个源文件到当前项目中。
[4]修改“MyFirstWS.cpp”，源文件内容如下
**[cpp]**[view
 plain](http://blog.csdn.net/lee353086/article/details/7498018#)[copy](http://blog.csdn.net/lee353086/article/details/7498018#)
- // MyFirstWS.cpp : Defines the entry point for the console application.
- //
- 
- #include "stdafx.h"
- 
- #include "MyInterface.h"
- #include "ns.nsmap"
- 
- //我的接口实现
- int ns__sqrt(struct soap *soap, double a, double &result)    
- {    
- if (a >= 0)    
-    {    
-       result = sqrt(a);    
- return SOAP_OK;    
-    }    
- 
- return soap_sender_fault(soap,   
- "Square root of negative value",   
- "I can only compute the square root of a non-negative value");   
- }   
- 
- //返回接口描述，否则在为C#代码添加服务引用时，找不到这个接口
- int http_get(struct soap   *soap)   
- {   
-     std::string fileName = "ns.wsdl";  
- 
- if(strstr(soap->msgbuf,"clientaccesspolicy.xml")!=NULL){  
-         fileName = "clientaccesspolicy.xml";  
- 
-     }//红色乃添加的代码
- FILE*fd = NULL;  
-     fd = fopen(fileName.c_str(), "rb"); //open WSDL file to copy
- if (!fd)  
-     {  
- return 404; //return HTTP not found error
-     }  
-     soap->http_content = "text/xml";  //HTTP header with text /xml content
-     soap_response(soap,SOAP_FILE);  
- for(;;)  
-     {  
- size_t r = fread(soap->tmpbuf,1, sizeof(soap->tmpbuf), fd);  
- if (!r)  
-         {  
- break;  
-         }  
- if (soap_send_raw(soap, soap->tmpbuf, r))  
-         {  
- break; //cannot send, but little we can do about that
-         }  
-     }  
-     fclose(fd);  
-     soap_end_send(soap);  
- return SOAP_OK;   
- }  
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
- int m, s; /* master and slave sockets */
- struct soap sqrt_soap;  
-     soap_init(&sqrt_soap);  
- 
- //添加，返回接口描述(WSDL)的功能，
- //否则其它语言在建立Web Service客户端的时候，无法自动生成代码
-     sqrt_soap.fget = http_get;      
- 
- //服务端侦听端口设为8080，注意ns.wsdl文件内容要做相应修改
-     m = soap_bind(&sqrt_soap, NULL, 8080, 100);  
- if (m < 0)  
-     {  
-         soap_print_fault(&sqrt_soap, stderr);  
-         exit(-1);  
-     }  
- 
-     fprintf(stderr, "Socket connection successful: master socket = %d\n", m);  
- for ( ; ; )  
-     {   
-         s = soap_accept(&sqrt_soap);   
- if (s < 0)  
-         {   
-             soap_print_fault(&sqrt_soap, stderr);  
-             exit(-1);  
-         }  
-         fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);  
- 
-         soap_serve(&sqrt_soap);  
-         soap_end(&sqrt_soap);  
-     }  
- 
- return 0;  
- }  
[5]如果链接的时候找不到soap的析构函数，你需要手动编译stdsoap2.cpp文件（即右键单击文件名，在弹出菜单上选择编译即可）
现在服务端程序可以跑起来了！
**第一个WebService客户端(C++语言)**
[1]新建C++的Console工程
[2]在命令行控制台中转到当前C++项目目录下
根据[http://localhost:8080/](http://localhost:8080/)网址打印出的WSDL信息产生MyInterface.h头文件
“**wsdl2h  -o  MyInterface.h  http://localhost:8080/**”
[3]根据“MyInterface.h”头文件生成存根文件
“**soapcpp2  -C  -IE:\SDK\gsoap-2.8\gsoap\import  MyInterface.h**”，“-C”参数指定只生成客户端存根文件，默认服务端和客户端存根文件都生成。“-I”参数指定stlvector.h文件的搜索路径。“**E:\SDK\gsoap-2.8**”是我gsoap
  Toolkit的存放路径。在项目中添加新生成的源文件，soapClientLib.cpp文件不用添加。
[4]在项目中添加**E:\SDK\gsoap-2.8**\gsoap目录下的**stdsoap2.h、stdsoap2.cpp、dom.cpp**等三个文件。
[5]修改项目默认的cpp文件，源码清单如下
**[cpp]**[view
 plain](http://blog.csdn.net/lee353086/article/details/7498018#)[copy](http://blog.csdn.net/lee353086/article/details/7498018#)
- // MyFirstWSClient.cpp : Defines the entry point for the console application.
- //
- 
- #include "stdafx.h"
- 
- //服务端提供的功能可以查看下面的头文件内容
- //如果服务端的URL地址或侦听端口改变，需要修改下面这个头文件
- #include "soapServiceProxy.h"
- 
- //下面这个头文件是必须的，否则stdsoap2.obj连接的时候会出错
- #include "Service.nsmap"
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
-     Service srv;  
- 
- double dR;  
-     {  
-         _ns2__sqrt         call;      //形参表
-         _ns2__sqrtResponse response;  //返回调用结果
-         call.a = 4.0;  
- int nR = srv.__ns1__sqrt(&call,&response);//调用sqrt函数
- if( nR == SOAP_OK)  
-         {  
-             dR = response.result;  
-             printf("成功返回，sqrt(4)=%f\n",response.result);  
-         } else
-         {  
-             printf("请检查网络连接！");  
-         }  
-     }  
- 
- return 0;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
现在你的程序可以跑起来了
**第一个Web Service客户端(C#语言)**
[1]新建C#的Console工程
[2]在新的工程里添加”Service Reference”，在出现的窗口里把你Web Service服务端的URL地址复制过去，Visual
  Studio 2008会自动发现Web Service接口，并为这个服务的接口建立相应的代码
[3]你可能需要修改**app.config**文件，修改Web Service服务的地址及侦听端口。
[4]修改Program.cs源文件，源码如下
**[csharp]**[view
 plain](http://blog.csdn.net/lee353086/article/details/7498018#)[copy](http://blog.csdn.net/lee353086/article/details/7498018#)
- using System;  
- using System.Collections.Generic;  
- using System.Linq;  
- using System.Text;  
- 
- using WSClient.ServiceReference1;  
- 
- namespace WSClient  
- {  
- class Program  
-     {  
- staticvoid Main(string[] args)  
-         {  
- try
-             {  
-                 ServicePortTypeClient proxy = new ServicePortTypeClient();  
- double dR = proxy.sqrt(4);  
-                 Console.WriteLine("sqrt(..)=" + dR);  
-             }  
- catch (Exception e)  
-             {  
-                 Console.WriteLine(e.Message);  
-             }  
-         }  
-     }  
- }  
现在可以跑了！
## 参考资料（网站）
[1]《gSOAP Toolkit》官网
[http://www.cs.fsu.edu/~engelen/soap.html](http://www.cs.fsu.edu/~engelen/soap.html)
[2]《gSoap入门之一__下载及编译gsoap两大法宝》
[http://blog.csdn.net/hslinux/article/details/5888352](http://blog.csdn.net/hslinux/article/details/5888352)
[3]《Silverlight跨域调用gSoap/[Java](http://lib.csdn.net/base/java)web
 service以及wsdl文件的修改》[http://www.cnblogs.com/MonkChen/archive/2012/01/11/2318672.html](http://www.cnblogs.com/MonkChen/archive/2012/01/11/2318672.html)
[转载地址](http://blog.csdn.net/lee353086/article/details/7498018)
