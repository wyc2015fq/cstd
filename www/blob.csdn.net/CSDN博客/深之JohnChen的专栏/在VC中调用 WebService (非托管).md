# 在VC中调用 WebService (非托管)  - 深之JohnChen的专栏 - CSDN博客

2008年06月09日 18:02:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1390


**在VC中调用 WebService (非托管) **
作者：[eRay Jiang](http://blog.eray.cn/)
[下载源代码](http://vckbase.com/code/downcode.asp?id=2968)

一、使用Visual Studio.NET调用WebService

　　很多次看到网友讨论VC中调用WebService的问题，其实在Visual Studio.Net（下称VS.NET）及其以后的版本中调用WebService已经变得相当简单。你所要做的只是“找到 WebService的发布地址”，然后将其引用添加到VC项目中即可。接下来VS.NET帮你生成的代理类，会帮你搞定一大堆的麻烦事，包括：网络调用、数据传递等等；什么SOAP啊、网络啊，你都可以不管。

二、托管还是非托管？

　　对于VS.NET生成的C++代理类，很多人都认为必须使用托管代码，其实事实并非这样。VS.NET可以生成托管和非托管两个版本的代理类，至于要不要使用托管，那是你自己的选择。
　　具体地说，对于使用托管的程序，VS.NET会生成基于托管类库的代理类；你可以在生成的代码中找到类的行：public System::Web::Services::Protocols::SoapHttpClientProtocol。对于不使用托管的程序，VS.NET会生成基于ATL的代码；你可以在生成的代码中找到类似的行：template <typename TClient = CSoapSocketClientT<> >。
我想，如果不是确实需要的话，大部份人会选择非托管的方式，因为这起码可以让我们的程序脱离.Net Framework运行。

三、动态设置WebService调用地址

　　这也是网上讨论得比较多的一个问题，因为WebSercie的地址不可能一成不变，特别是在开发调试的时候。幸运的是，在两种版本的代理类中，都可以非常方便地在运行时设置WebService的地址。（具体做法参见实例）

四、程序示例（非托管）

　　由于已经有文章详细讲述了 [VC托管程序中调用 WebService 的过程](http://www.vckbase.com/document/viewdoc/?id=1641)，因此以下示例只描述非托管VC程中调用WebService的方法。

１、新建一个非托管的MFC应用程序，注意是非托管的，如下图（图１）

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService01.JPG)
图1、建立非托管程序

完成后，查看VC项目属性，可以确认不使用托管

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService02.JPG)
图2、查看是否使用托管

2、添加WebService引用
在VC项目名称上，单击右键，选择“添加Web引用”。如下图（图3）：

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService03.JPG)
图3、添加Web引用

在弹出的“添加Web引用”对话框中，填入WebSercie的引用地址， 然后点击“转到按钮”，可以看到WebService的提示页面。

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService04.JPG)
图4、“添加Web引用”对话框

　　点击页面上的“Service Description” 链接可以看到具体的WebMethod的声明。我的这个WebService示例中，只定义了一个Web方法。该方法接受一个字符串作为用户名，并返回一个字符串作为对用户的问候语。如下图所示：

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService05.JPG)
图5、查看WebMethod原型

　　在上图中填入“Web引用名”，然后点击“添加引用按钮”。（在非托管版的代理类中，这里填的“Web引用名”将没有任保实质性的作用，所以随便填入一个名字即可。但是在托管版的代理类中，“Web引用名”将成为代量类的命名空间） 。接下来，VS.NET将生成一个WebService的代理类，生成完后，会自动打开WebService.h头文件：

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService06.JPG)

WebService.h并不是代理类，这个头文件其实是用于包含所有的代理类的头文件，你可以多添加几个“Web引用”试试。

3、浏览代理类

　　我们不妨浏览一下生成的代理类，做到有个基本的了解。切换到“类视图”，可以看到一个“Debug”命名空间，全部展开，可以看到生成的代理类的全部成员：

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService07.JPG)
图6、浏览生成的代码

4、调用示例

首先包含头文件，并打开命名空间 
#include "WebService.h"
using namespace Debug;	// 这个命名空间是自动生成的，与Web服务的实现有关          
以下是调用代码
void CInvokeDemoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	// 因为生成的代码是基于ATL的，所以要初始化COM
	CoInitialize(NULL);

	HRESULT hr = S_OK;
	CComBSTR hiResult;
	CComBSTR username = "vckBase";

	CDebug* debug = new CDebug;	// 代理对象

	// 可以调用SetUrl动态设置Web服务地址
	// debug->SetUrl("http://blog.eray.cn/debug.asmx");

	hr = debug->Hi(username,&hiResult); //注意，返回值是以指针形式反回的

	if(FAILED(hr))
	{
		MessageBox("调用失败");
	}
	else
	{
		CString str(hiResult);
		MessageBox(str,"调用结果");
	}

	delete debug;
	CoUninitialize();
}          
　　由于生成的代理类是基于ATL的，所以在调用前要初始化COM调用。在上述的代码中就使用了CComBSTR而没有直接使用BSTR，因为CComBSTR属于智能类型，可以自己管理内存分配，比较方便。 上述代码中有一行被注释的代码调用了SetUrl来设置WebService的调用地址。在实际的项目中，可以将这个地址写在配置文件中。

5、运行结果

来，看一下吧～

![](http://vckbase.com/document/journal/vckbase49/images/unmanagedWebService08.JPG)

五、结束语

　　通过以上示例可以看出，其实在VS.NET中实现非托管的C++调用WebService是相当简单的。当然，在实际的使用过程中，为了程序的徤壮，还需要更多的代码逻辑，比如错误处理等等。

详细的Web 服务和 Microsoft 平台资料参考:http://msdn.microsoft.com/zh-cn/library/aa480728.aspx

