# gsoap使用方法，解析wsdl文档 - 建建的博客 - CSDN博客
2017年02月17日 15:37:28[纪建](https://me.csdn.net/u013898698)阅读数：330
下载gSoap
[http://sourceforge.net/projects/gsoap2](http://sourceforge.net/projects/gsoap2)
1.下载完成后直接解压缩，在目录gsoap-2.8\gsoap\bin下面有编译工具，支持多个平台，我们使用的是win32平台的工具
    wsdl2h.exe  解析WebService的wsdl文件
    soapcpp2.exe 编译WebService代码
2.接下来需要先获得WebService的wsdl文件，访问WebService时，[http://XXXX/XXXX.asmx?wsdl](http://xxxx/XXXX.asmx?wsdl)这个地址就是需要的wsdl文件，选择查看源文件，然后另存为的方式，获取到wsdl文件，例如：另存为webs.wsdl文件。
3.解析wsdl文件
    执行wsdl2h.exe -o webs.wsdl命令，就会生成一个webs.h的头文件
![](https://img-my.csdn.net/uploads/201203/30/1333075007_4061.jpg)
这里简单说一下参数，主要常用的是-o和-c，-o后面跟的文件名是需要生成的文件名。-c表示生成纯C的头文件，否则默认是C++。具体参数可以运行wsdl2h.exe /?查看帮助。
4.编译
    执行soapcpp2.exe webs.h，会生成很多个文件，这里同样可以加一些参数，具体不解释了，可以通过soapcpp2.exe /?命令来查看。
    我们找出需要的以下几个文件：
    soapC.cpp
    soapClient.cpp
    soapH.h
    soapStub.h
    WebsSoap.nsmap
    再加上另外两个文件（不在bin目录下，在gsoap-2.8\gsoap目录下）：
    stdsoap2.h
    stdsoap2.cpp
    一共7个文件，就是我们最终访问WebService所需要的soap访问调用文件。这里是C++的代码，根据需要，也可以生成C的代码，stdsoap2.cpp更换为stdsoap2.h文件。
    把上述7个文件，加入到自己的工程中编译即可，接下来就是如何调用的问题了。
5.调用
**[cpp]**[view
 plain](http://blog.csdn.net/threewall/article/details/7410242#)[copy](http://blog.csdn.net/threewall/article/details/7410242#)
- struct soap mysoap;  
- _ns1__Test test_req;  
- _ns1__TestResponse test_res;  
- 
- //初始化
- soap_init(&mysoap);  
- //调用WebService接口函数
- soap_call___ns3__Test(&mysoap, NULL, NULL, &test_req, &test_res);  
- //销毁、退出
- soap_destroy(&mysoap);  
- soap_end(&mysoap);  
- soap_done(&mysoap);  
这里可以看到，WebService的接口都会被转换为soap_call___ns开头的函数，如果接口是多个分词，例如接口函数是Make_Test()，这里会变成soap_call___ns1__Make_USCORETest()，需要注意一下。如果找不到该接口函数，可以去soapClient.cpp文件中搜索，就可以找到需要调用的接口了。
一般情况下，转换后的接口，会多出几个参数，一个soap_endpoint表示访问的WebService地址，一个soap_action表示访问的接口函数名称，可以都给NULL即可。后两个参数，一个是请求时所需传递的XML数据，一个是返回数据时的XML数据，都需要自己解析和组织数据。
这样基本上就可以调用成功了，如果你访问的WebService只需要匿名认证，基本到这里就可以完成了。
6.认证
如果WebService需要认证，这里需要设置一下，在调用函数之前，设置用户名和密码
**[cpp]**[view
 plain](http://blog.csdn.net/threewall/article/details/7410242#)[copy](http://blog.csdn.net/threewall/article/details/7410242#)
- mysoap.userid = "WebService";  
- mysoap.passwd = "InterASP.NET200812";  
- mysoap.authrealm = "";  
这里需要说明一下，大多数WebService的认证都是匿名认证，还有一些是基于Http的Base认证，匿名认证不需要用户名和密码，base认证需要设置用户名和密码，这两种都很容易通过认证。但有些WebService是采用NTLM认证，这个认证方式就比较麻烦了，需要gsoap支持才可以。
简单说一下NTLM认证集成的方式，但我并没有搞定，只能是把查到的信息先搜集起来，以后还需要继续学习。
首先，下载libntlm库，下载地址[http://www.nongnu.org/libntlm](http://www/)
解压缩后，获取DLL文件和头文件，加入到自己的工程中
其次，在项目工程的编译参数中加入-DWITH_NTLM编译参数，重新编译整个工程。
再次，调用程序需要修改。NTLM认证需要六次握手过程：
    1: C --> S POST ...
        Content-Type: text/xml; charset=utf-8
    2: C <-- S 401 Unauthorized
        WWW-Authenticate: NTLM
    3: C --> S GET ...
        Authorization: NTLM <base64-encoded type-1-message>
    4: C <-- S 401 Unauthorized
        WWW-Authenticate: NTLM <base64-encoded type-2-message>
    5: C --> S POST ...
        Content-Type: text/xml; charset=utf-8
        Authorization: NTLM <base64-encoded type-3-message>
    6: C <-- S 200 OK
调用函数的代码修改为：
**[cpp]**[view
 plain](http://blog.csdn.net/threewall/article/details/7410242#)[copy](http://blog.csdn.net/threewall/article/details/7410242#)
- soap_call___ns3__Test(&mysoap, NULL, NULL, &test_req, &test_res);  
- if(mysoap.error==401)  
- {  
-     mysoap.userid = "WebService";  
-     mysoap.passwd = "InterASP.NET200812";  
-     mysoap.authrealm = "";  
-     soap_call___ns3__Test(&mysoap, NULL, NULL, &test_req, &test_res);  
-     ...  
- }  
需要两次调用函数才可以完成handshake的过程，然后就可以获取server端的认证了，完成soap调用。
这里说明一下：我按照文档所述方法，这样调用，并没有通过认证，暂时还没有解决该问题，所以这里暂时先记下这种方法，还需要进一步解决认证问题。
