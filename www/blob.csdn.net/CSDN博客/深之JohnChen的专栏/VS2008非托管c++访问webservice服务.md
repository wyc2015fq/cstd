# VS2008非托管c++访问webservice服务 - 深之JohnChen的专栏 - CSDN博客

2011年11月19日 11:26:16[byxdaz](https://me.csdn.net/byxdaz)阅读数：6767


Visual Studio 2008以及以后版本中，微软停止了非托管C++的直接WebService引用。不过ATL Server代码已经托管到开源网站上，我们可以找到ATL Server的源代码，编译出Sproxy.exe，这个工具可以根据wsdl文件来生成非托管的代理类。这个代理类还需要配合一些头文件才能一起使用，这个相关的头文件都包含在ATL Server 的源代码内。

1. 第一步需要使用sproxy.exe工具来生成代理类。在vs2008以前的版本，比如vs2005，本身就带有这个命令，但在vs2008版，已经把它给去除了。需要去[http://atlserver.codeplex.com/](http://atlserver.codeplex.com/)下载ATL_Server源代码（最新的ATL_Server_Source_and_Headers_9_0_70425_Alpha）并编译产生sproxy.exe工具。

注意：在编译时出现atl*.h文件未找到时，需要将ATL_Server_Source_and_Headers_9_0_70425_Alpha目录下的include文件找到并复制入 C:\Program Files\MicrosoftVisual Studio 10.0\VC\atlmfc\include（即VC的安装目录）中。

2. 为了测试webservice服务，我在网上找了一个提供webservice的网站：[http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx](http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx)它提供天气预报服务。它的WSDL文件在这个：[http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?WSDL](http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?WSDL)把这个文件保存为本地文件，名字叫WeatherWS.wsdl。为了方便，我假定这个文件和sproxy.exe放在同一个目录下，执行命令

sproxy.exe /wsdl WeatherWS.wsdl

就会在同目录下生成WeatherWS.h文件。

3. 打开vs2008，建立一个MFC项目WebService，添加WeatherWS.h文件到项目的头文件中。项目中设置ATL_Server的include目录路径。在WebServiceDlg.cpp文件中添加

#include"WeatherWS.h" 

using namespace WeatherWS;

4. 这些做完以后，就可以调用webservice服务了，具体请参考附件的代码。

```cpp
CoInitialize(NULL);

    HRESULT hr = S_OK; 

    CWeatherWST<CSoapSocketClientT<>>* m_srv = newCWeatherWST<CSoapSocketClientT<>>;

 

    CComBSTR cityCode= ""; //参数为空默认返回上海的天气情况

    CComBSTR userId= ""; 

    CComBSTR * bstrOut= NULL; 

    int size; 

    CString strUrl= _T("http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx");

    m_srv->SetUrl(strUrl);//设置url地址

    m_srv->SetTimeout(5000);//设置超时(参数毫秒)

    hr = m_srv->getWeather(cityCode,userId, (BSTR* *)&bstrOut, &size); 

    int nErrorCode = m_srv->GetStatusCode();

    if(FAILED(hr)) 

    {

       MessageBox(L"调用失败"); 

    }

    else 

    {

       for (int i = 0; i < size; i++) 

       {

           bstrOut[i].Append("\r\n");

       }

 

       if(bstrOut!=NULL)

       {

           for(int i=size-1;i>=0;i--)

           {

              ::SysFreeString(bstrOut[i]);

           }

           bstrOut = NULL;

       }

    }

    if (m_srv != NULL) 

       delete m_srv; 

    CoUninitialize();
```


SPROXY.EXE 是一个命令行工具，它生成基于 WSDL 说明访问 XML Web services 的本机 C++ 客户端代码。

命令行语法如下所示：

[复制]()

sproxy [ options ] [/out:outputfile ] input_location

#### 参数

*options*

（可选）一个或多个下列选项：
|选项|说明|
|----|----|
|/? 或 /help|显示使用信息。|
|/nologo|取消显示版权消息。|
|/nowarn|禁用所有警告。|
|/nopragma|不将“#pragma once”插入输出文件。|
|/noclobber|如果输出文件已存在，则不改写它。|
|/nonamespace|不将 C++ 命名空间插入生成的代理文件。默认情况下，SPROXY 插入一个基于 WSDL 服务名的命名空间。|
|/namespace:*<name>*|将名为 *<name>* 的命名空间插入生成的代理文件。默认情况下，SPROXY 插入一个基于 WSDL 服务名的命名空间。 命名空间必须是有效的 C++ 标识符。当 *<name>* 为空时，/namespace:*<name>*  选项的效果与 /nonamespace 相同。|
|/wsdl <*input_location*>|处理位于指定路径 <*input_location*>  的 .wsdl 文件，而不是默认的 .discomap 文件。|
*outputfile*
（可选）生成的代码将写入的文件的名称。如果该文件存在，除非指定 `/noclobber`，否则将改写它。如果未指定 *outputfile*，SPROXY 将在当前目录中基于WSDL 服务名创建一个文件。

*input_location*

对生成其代理代码的XML Web services 进行描述的文件的位置。*input_location*可以是.discomap 或.wsdl 文件的URL 或文件系统路径。使用.wsdl 文件时请指定
`/wsdl`选项。

SPROXY.EXE 也可以处理 results.discomap 文件。请注意，.discomap 文件包含指向 .wsdl 文件本地副本的链接，并且使用架构文件的本地副本。

#### 备注

SPROXY.EXE 将生成一个从 CSoapRootHandler 派生的代理类模板以及它的模板参数。模板参数需要符合 SOAP 客户端原型且默认为 CSoapSocketClientT<>。

服务公开的每个 SOAP 方法由代理类中的一个方法表示。若要访问 XML Webservices，请创建代理类的实例并调用适当的方法。

在 Visual C++ 安装程序的 \vc7\bin 目录中可以找到 SPROXY.EXE。

有关 SPROXY 支持的类型列表，请参见[支持的类型](http://msdn.microsoft.com/zh-cn/library/cc468078)和[用 ATL Server 创建的 XML Web services 中受支持的类型](http://msdn.microsoft.com/zh-cn/library/cc468112)。

**注意   **SPROXY生成的客户端需要MSXML3。在运行该客户端之前，将需要在安装所生成的客户端的计算机上安装MSXML3。

Xmlinst.exe 在替换模式下安装 MSXML3。请从MSDN 的 [http://msdn.microsoft.com/downloads/default.asp?URL=/code/sample.asp?url=/msdn-files/027/001/469/msdncompositedoc.xml](http://msdn.microsoft.com/downloads/default.asp?URL=/code/sample.asp?url=/msdn-files/027/001/469/msdncompositedoc.xml) 下载 Xmlinst.exe 替换模式工具。

有关使用Xmlinst.exe 的信息，请参见知识库的文章：“PRB：在生产服务器上运行 Xmlinst.exe 之后出现的应用程序错误”(Q278636)。可以在MSDN Library CD-ROM 中或
[http://support.microsoft.com/support/](http://support.microsoft.com/support/) 上找到知识库文章。

有关运行MSXML3 的信息，请参见在替换模式下运行 MSXML 3.0。

**注意**   SPROXY生成的头文件包括atlsoap.h。此文件声明 `[emitidl("restricted")];`。如果在遇到
`[module]`属性时限制IDL 发出，将发生错误。在包括atlsoap.h 后，可以在代码中使用 `[emitidl("true")];`以启用IDL 发出。（atlextmgmt.h包括与atlsoap.h 相同的
[emitidl](http://msdn.microsoft.com/zh-cn/library/khyxay72) 声明，因此适应同样的问题。）

**注意**   在取消引用前，SPROXY 生成的方法不检查指针参数是否为 `NULL`。在将指针传递给 XML Web services 代理类中的方法前，请检查指针是否为
`NULL`。

#### 示例

以下命令行将生成一个文件 myservice.h，该文件包含用于访问从http://myserver/myservice.dll?Handler=GenMyServiceWSDL 获取的 WSDL 所描述的 XML Webservices 的 C++ 代理代码：

[复制]()

sproxy /wsdlhttp://myserver/myservice.dll?Handler=GenMyServiceWSDL /out:myservice.h

以下命令行将生成一个输出文件，该文件包含用于访问从results.discomap（它包含指向 .wsdl 文件本地副本的链接）中获取的 WSDL 所描述的 XML Webservices 的 C++ 代理代码：

[复制]()

sproxyresults.discomap

CSoapSocketClientT 类说明

，参考[http://msdn.microsoft.com/zh-cn/subscriptions/0yw680wb(VS.80).aspx](http://msdn.microsoft.com/zh-cn/subscriptions/0yw680wb(VS.80).aspx)

