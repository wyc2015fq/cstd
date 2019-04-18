# 如何使用BHO定制你的Internet Explorer浏览器 - gauss的专栏 - CSDN博客
2013年01月04日 21:54:07[gauss](https://me.csdn.net/mathlmx)阅读数：301
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
                
原文出处：[Browser Helper Objects: The Browser the Way You Want It](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwebgen/html/bho.asp)
**一、简介**
　　有时，你可能需要一个定制版本的浏览器。在这种情况下，你可以自由地把一些新颖但又不标准的特征增加到一个浏览器上。结果，你最终有的只是一个新但不标准的浏览器。Web浏览器控件只是浏览器的分析引擎。这意味着仍然存在若干的与用户接口相关的工作等待你做――增加一个地址栏，工具栏，历史记录，状态栏，频道栏和收藏夹等。如此，要产生一个定制的浏览器，你可以进行两种类型的编程――一种象微软把Web浏览器控件转变成一个功能齐全的浏览器如Internet Explorer；一种是在现有的基础上加一些新的功能。如果有一个直接的方法定制现有的Internet
 Explorer该多好？BHO（Browser Helper Objects，我译为"浏览器帮助者对象"，以下皆简称BHO）正是用来实现此目的的。
**二、关于软件定制**
　　以前，定制一个软件的行为主要是通过子类化方法实现的。 通过这种办法，你可以改变一个窗口的外表与行为。子类化虽然被认为是一种有点暴力方式――受害者根本不知道发生的事情――但它还是长时间以来的唯一的选择。
　　随着微软Win32 API的到来，进程间子类化不再被鼓励使用并愈发变得困难起来。当然，如果你是勇敢的--指针从未吓倒你，而最重要的是，如果你已经游刃于系统钩子之间，你可能觉得这一问题太简单了。 但是情形并不总是这样。暂放下这点不管，问题在于每一个进程运行在自己的地址空间中，而且打破进程边界略微有些不正确性。 另一方面， 你可能需要对定制进行更好的管理。更经常情况下，定制可能是程序本身强烈要求实现的。
　　在后者情况下，已安装的软件只需在既定的磁盘位置查询另外的组件模块，然后装载、设定初值，最后让它们自由地按照既定的设计工作。这正是Internet Explorer浏览器和它的BHO所要实现的。
**三、什么是BHO?**
　　从某种观点看，Internet Explorer同普通的Win32程序没有什么两样。借助于BHO，你可以写一个进程内COM对象，这个对象在每次启动时都要加载。这样的对象会在与浏览器相同的上下文中运行，并能对可用的窗口和模块执行任何行动。例如，一个BHO能够探测到典型的事件，如GoBack、GoForward、DocumentComplete等；另外BHO能够存取浏览器的菜单与工具栏并能做出修改，还能够产生新窗口来显示当前网页的一些额外信息，还能够安装钩子以监控一些消息和动作。简而言之， BHO的工作如我们打入浏览器领地的一位间谍（注意这是微软允许的合法工作）。
　　在进一步了解BHO细节之前，有几点我需要进一步阐述。首先，BHO对象依托于浏览器主窗口。实际上，这意味着一旦一个浏览器窗口产生，一个新的BHO对象实例就要生成。任何 BHO对象与浏览器实例的生命周期是一致的。其次， BHO仅存在于Internet Explorer 4.0及以后版本中。
如果你在使用Microsoft Windows? 98， Windows 2000， Windows 95， or Windows NT版本4.0 操作系统的话，也就一块运行了活动桌面外壳4.71，BHO也被 Windows资源管理器所支持。 BHO是一个COM进程内服务，注册于注册表中某一键下。在启动时，Internet Explorer查询那个键并把该键下的所有对象预以加载。
　　Internet Explorer浏览器初始化这一对象并要求某一接口功能。如果发现这一接口， Internet Explorer使用其提供的方法传递 IUnknown 指针到BHO对象。见图一：
![](http://www.vckbase.com/document/journal/vckbase41/images/bhoieimg1.gif)
图一 ie浏览器如何装入和初始化BHO对象，BHO场所（site）是用于实现通信的COM接口
　　浏览器可能在注册表中发现一系列的CLSID，并由此为每个CLSID建立一个进程中实例。结果是，这些对象被装载至浏览器上下文中并运行起来，好象它们是本地组件一样。但是，由于Internet Explorer的COM特性，即使被装入到它的进程空间中于事（你的野心实现）也不一定会有多大帮助。用另一说法， BHO的确能够做许多潜在的有用的事情，如子类化组成窗口或者安装线程局部钩子，但是它确实远离浏览器的核心活动。为了钩住浏览器的事件或者自动化浏览器，BHO需要建立一个私有的基于COM的通讯通道。为此，该BHO应该实现一个称为IObjectWithSite的接口。事实上，通过接口IobjectWithSite，
 Internet Explorer 可以传递它的IUnknown 接口。BHO反过来能够存储该接口并进一步查询更专门的接口，如IWebBrowser2、IDispatch和IConnectionPointContainer。
　　另外一种分析BHO对象的途径与Internet Explorer外壳扩展有关。我们知道，一个WINDOWS外壳扩展即是一个进程内的COM服务器，它在Windows资源管理器执行某种动作时装入内存――如显示上下文菜单。通过建立一个实现几个COM接口的COM模块，你就给上下文菜单加上一些项并能预以正确处理。一个外壳扩展必须以Windows资源管理器能够发现的方法注册。一个BHO对象遵循同样的模式――唯一的改变在于要实现的接口。然而，尽管实现方式有所不同，外壳扩展与 BHO 仍有许多共同的特点。如下表一：
表一 外壳扩展与 BHO相近特性比较
|**特性**|**外壳扩展**|**BHO对象**|
|----|----|----|
|加载者|Windows资源管理器|Internet Explorer（和外壳4.17及以上版本的Windows资源管理器）|
|击活动作|在某类文档上的用户动作（即单击右键）|打开浏览器窗口|
|何时卸载|参考计数达到0的几秒之后|导致它加载的窗口关闭时|
|实现形式|COM进程中DLL|COM 进程中 DLL|
|注册需求|常常是为一个COM服务器设置的入口处，另加的入口依赖于外壳类型及它要应用至的文档类型|常常是为一个COM服务器设置的入口处，另加一个把它申请为BHO的注册入口|
|接口需求|依赖于外壳扩展的类型|IObjectWithSite|
如果你对SHELL扩展编程有兴趣的话，可以参考MSDN有关资料。
**四、BHO的生存周期**
　　前面已经说过，BHO不仅仅为Internet Explorer所支持。如果你在使用外壳 4.71或者更高版本，你的BHO对象也会被Windows资源管理器所加载。下表二展示了我们可以使用的不同版本的外壳产品情况，Windows外壳版本号存于库文件shell32.dll中。
表二 不同版本的Windows外壳对于BHO的支持情况
|**外壳版本**|**安装的产品**|**BHO的支持情况**|
|----|----|----|
|4.00|Windows 95，Windows  NT 4.0 带或不带 Internet Explorer 4.0 或更老版本。 注意没有安装外壳更新|Internet Explorer 4.0|
|4.71|Windows 95，Windows NT 4.0 带Internet Explorer 4.0 和活动桌面外壳更新|Internet Explorer 与Windows 资源管理器|
|4.72|Windows 98|Internet Explorer与Windows 资源管理器|
|5.00|Windows 2000|Internet Explorer与Windows 资源管理器|
　　BHO对象随着浏览器主窗口的显示而装入，随着浏览器主窗口的销毁而缷载。如果你打开多个浏览器窗口，多个BHO实例也一同产生。
　　无论浏览器以什么样的命令行启动，BHO对象都被加载。举例来说，即使你只是想要见到特定的 HTML 页或一个给定的文件夹，BHO对象也被加载。一般地，当 explorer.exe 或 iexplore.exe 运行的时候，BHO都要被考虑在内。如果你设置了"Open each folder in its own window"（对每一个文件夹以一个独立窗口打开）文件夹选项，那么你每次打开一个文件夹，BHO对象都要被加载。见图二。
![](http://www.vckbase.com/document/journal/vckbase41/images/bhoieimg2.gif)
图二 经过这样设置，你每次打开一个文件夹时，执行一个独立的explorer.exe实例，并装入已注册的BHO对象。
　　但是注意，这种情形仅适于当你从桌面上的"我的电脑"图标中打开文件夹的情况。在这种情况下，每次你移到另外一个文件夹时外壳都要调用explorer.exe。这种情况在你同时用两个窗格进行浏览时是不会发生的。事实上，当你改变文件夹时，外壳是不会启动浏览器的新的实例的而仅是简单创建嵌入视图对象的另外一个实例。奇怪的是，如果你在地址栏中输入一个新的名字来改变文件夹时，在同一个窗口中同样可以达到浏览之目的，无论Windows资源管理器视图是单个的还是双视图形式。
　　对于Internet Explorer的情形，事情要更简单一些。只有你显式地多次运行iexplore.exe浏览器时，你才有多个Internet Explorer的拷贝。当你从Internet Explorer中打开新的窗口时，每一个窗口在一个新的线程中被复制而不是创建一个新的进程，因此也就不需要重新载入BHO对象。
　　首先，BHO最有趣的地方是，它是极度动态的。每次Windows资源管理器或者Internet Explorer打开，装载器从注册表中读取已安装的BHO对象的CLSID然后处理它们。如果你在打开的浏览器多个实例中间编辑注册表的话，你可以随着多个浏览器拷贝的载入而装入多个不同的BHO。 这就是说，如果你选择从头创建一个新的属于自己的浏览器，那么你可以把它内嵌在一个Visual Basic或者MFC框架窗口中。同时你有相当的机会来灵活安排浏览程序。如果它们能满足你的需要的话，你可以依赖于Internet Explorer的强大的功能并且加上你想要的尽可能多的插件。
**五、关于IObjectWithSite接口**
　　从一个高起点来看，BHO即是一个DLL，它能够依附于Internet Explorer浏览器的一个新建的实例，在某些情况下也适用于Windows资源管理器。
　　一般地，一个场所（site）是一个中间对象，它位于容器对象与被包容对象之间。通过它，容器对象管理被包容对象的内容，也因此使得对象的内部功能可用。为此，容器方要实现接口IoleClientSite，被包容对象要实现接口IOleObject 。通过调用IOleObject提供的方法，容器对象使得被包容对象清楚地了解其HOST的环境。
　　一旦容器对象成为Internet Explorer（或是具有WEB能力的Windows资源管理器），被包容对象只需实现一个轻型的IObjectWithSite接口。该接口提供了以下方法：
表三 IObjectWithSite定义
|**方法**|**描述**|
|----|----|
|HRESULT SetSite(IUnknown* pUnkSite)|接收ie浏览器的IUnknown指针。典型实现是保存该指针以备将来使用。.|
|HRESULT GetSite(REFIID riid， void** ppvSite)|从通过SetSite()方法设置的场所中接收并返回指定的接口，典型实现是查询前面保存的接口指针以进一步取得指定的接口。|
　　对BHO 的唯一严格的要求正在于必须实现这一个接口。 注意你应该避免在调用以上任何一个函数时返回E_NOTIMPL 。 要么你不实现这一接口，要么应保证在调用这些方法时进行正确地编码。
**六、构造自己的BHO对象**
　　一个BHO对象就是一个进程中服务器DLL，选用ATL创建它是再恰当不过的了。我们选择ATL的另外一个原因是因为它已经提供了缺省的而且提供了IObjectWithSite接口的足够好的实现。另外，在ATL COM 向导本地支持的已定义好的对象类型当中，有一个，就是Internet Explorer对象，这正是一个BHO应该具有的类型。一个 ATL Internet Explorer 对象，事实上是一个简单对象――也就是说，是一个支持IUnknown和自注册，还有接口IObjectWithSite的COM 服务器。如果你在ATL工程中添加一个这样的对象，并调用相应的类CViewSource，你将从向导中得到下列代码：
class ATL_NO_VTABLE CViewSource : 
   public CComObjectRootEx<CComSingleThreadModel>，
   public CComCoClass<CViewSource， &CLSID_ViewSource>，
   public IObjectWithSiteImpl<CViewSource>，
   public IDispatchImpl<IViewSource， &IID_IViewSource， 
                        &LIBID_HTMLEDITLib>      
　　正如你所见，向导已经使类从接口IObjectWithSiteImpl继承，这是一个ATL模板类，它提供了接口IObjectWithSite的基本实现。一般情况下，没有必要重载成员函数GetSite()。取而代之的是， SetSite() 实现代码经常需要加以定制。ATL实际上仅仅把一个IUnknown接口指针存储在成员变量m_spUnkSite中。
　　在文章的剩余部分，我将讨论一个 BHO 的相当复杂而丰富的例子。该BHO对象将依附于Internet Explorer，并显示一个文本框来显示当前正浏览的网页源码。 该代码窗口将　随着你改变网页而自动更新，如果浏览器显示的不是一个HTML网页时，它将变灰。你对于原始HTML代码的任何改动立即反映在浏览器中。HTML (DHTML)使得这一看似魔术般的实现成为可能。该代码窗口可被隐藏和通过按动热键重现。 在可见情况下，它与Internet Explorer共享整个桌面空间，见图三。
![](http://www.vckbase.com/document/journal/vckbase41/images/bhoieimg3.gif)
图三　BHO对象在使用中。它依附于Internet Explorer，并显示一个窗口来显示当前正浏览的网页源码。还允许你源码进行修改。
　　本例子的关键点在于存取Internet Explorer的浏览机制，其实它只不过是WebBrowser控件的一个实例而已。这个例子可以分解为以下五步来实现：
- 探测谁在装入这个对象，是Internet Explorer还是Windows资源管理器；
- 获取接口IWebBrowser2以实现Web浏览器对象；
- 捕捉Web浏览器的特定事件；
- 存取当前文档对象，确定它是一份HTML类型的文件；
- 管理对话框窗口以实现HTML源码的显示； 
　　第一个步骤是在DllMain()中完成的。SetSite()是取得指向WebBrowser对象指针的适当位置。请详细分析以下步骤。
**七、探测谁在调用这个对象**
　　如前所述，一个BHO对象会被Internet Explorer或者Windows资源管理器（前提：外壳版本4.71或者更高）所加载。所以我专门设计了一个BHO来处理HTML网页，因此这个BHO与资源管理器毫无关系。如果一个Dll不想被调用者一起加载，只需在DllMain()中实现了探明谁在调用该对象后返回FALSE即可。参看下面代码：
if (dwReason == DLL_PROCESS_ATTACH)
{
TCHAR pszLoader[MAX_PATH];
//返回调用者模块的名称，第一个参数应为NULL，详见msdn。
GetModuleFileName(NULL， pszLoader， MAX_PATH);
_tcslwr(pszLoader);
if (_tcsstr(pszLoader， _T("explorer.exe"))) 
   return FALSE;
}
一旦知道了当前进程是Windows资源管理器，可立即退出。
　　注意，再多加一些条件语句是危险的！事实上，另外一些进程试图装入该DLL时将被放弃。如果你做另外一个试验，比方说针对Internet Explorer的执行文件iexplorer.exe，这时第一个受害者就是regsvr32.exe（该程序用于自动注册对象）。
if (!_tcsstr(pszLoader， _T("iexplore.exe"))) 
　　你不能够再次注册该DLL库了。 事实上，当 regsvr32.exe 试图装入DLL以激活函数DllRegisterServer()时，该调用将被放弃。
**八、与Web浏览器取得联系**
　　SetSite()方法正是BHO对象被初始化的地方，此外，在这个方法中你可以执行所有的仅仅允许发生一次的任务。当你用Internet Explorer打开一个URL时，你应该等待一系列的事件以确保要求的文档已完全下载并被初始化。唯有在此时，你才可以通过对象模型暴露的接口（如果存在的话）存取文档内容。这就是说你要取得一系列的指针。第一个就是指向IWebBrowser2(该接口用来生成WebBrowser对象)的指针。第二个指针与事件有关。该模块必须作为一个浏览器的事件侦听器来实现，目的是为接收下载以及与文档相关的事件。下面用ATL灵敏指针加以封装：
CComQIPtr< IWebBrowser2， &IID_IWebBrowser2> m_spWebBrowser2;
CComQIPtr<IConnectionPointContainer， 
      &IID_IConnectionPointContainer> m_spCPC;
源代码部分如下所示：
HRESULT CViewSource::SetSite(IUnknown *pUnkSite)
{
  // 检索并存储 IWebBrowser2 指针 
  m_spWebBrowser2 = pUnkSite; 
  if (m_spWebBrowser2 == NULL)
   return E_INVALIDARG;
  //检索并存储 IConnectionPointerContainer指针
  m_spCPC = m_spWebBrowser2;
  if (m_spCPC == NULL) 
   return E_POINTER;
  //检索并存储浏览器的句柄HWND. 并且安装一个键盘钩子备后用
  RetrieveBrowserWindow();
  // 为接受事件通知连接到容器
  return Connect();
}
　　为了取得IWebBrowser2接口指针，你可以进行查询。当然也可以在事件刚刚发生时查询IConnectionPointContainer。这里，SetSite()检索了浏览器的句柄HWND，并且在当前线程中安装了一个键盘钩子。HWND用于后面Internet Explorer窗口的移动或尺寸调整。这里的钩子用来实现热键功能，用户可以按动热键来显示/隐藏代码窗口。
**九、从Internet Explorer浏览器取得事件**
　　当你导向一个新的URL时，浏览器最需要完成的是两种事件：下载文档并为之准备HOST环境。也就是说，它必须初始化某对象并使该对象从外部可以利用。针对不同的文档类型，或者装入一个已注册的Microsoft ActiveX? 服务器来处理该文档（如Word对于.doc文件的处理）或者初始化一些内部组件来分析文档内容并生成和显示该文档。对于HTML网页就是这样，其内容由于DHTML对象作用而变得可用。当文档全部下载结束，DownloadComplete事件被激活。这并不是说，这样利用对象模型就可以安全地管理文档的内容了。事实上，DocumentComplete
 事件仅指明一切已经结束，文档已准备好了 (注意DocumentComplete事件仅在你第一次存取URL时到达，如果你执行了刷新动作，你仅仅收到一个DocumentComplete事件)。
　　为了截获浏览器发出的事件， BHO需要通过IConnectionPoint 接口连接到浏览器上 并且实现传递接口IDispatch指针以处理各种事件。现在利用前面取得的IConnectionPointContainer指针来调用FindConnectionPoint方法――它返回一个指针指向连接点对象（正是通过这个连接点对象来取得要求的外向接口，此时是DIID_DWebBrowserEvent2）。 下列代码显示了连接点的发生情况：
HRESULT CViewSource::Connect(void)
{
  HRESULT hr;
  CComPtr<IConnectionPoint> spCP;
  //为Web浏览器事件而接收（receive）连接点 
  hr = m_spCPC->FindConnectionPoint(DIID_DWebBrowserEvent2， &spCP);
  if (FAILED(hr))
   return hr;
  // 把事件处理器传递到容器。每次事件发生容器都将激活我们实现的IDispatch接口上的相应的函数。
  hr = spCP->Advise( reinterpret_cast<IDispatch*>(this)， &m_dwCookie);
  return hr; 
}
　　通过调用接口IConnectionPoint的Advise() 方法， BHO告诉浏览器它对它产生的事件很感兴趣。 由于COM事件处理机制，所有这些意味着BHO把IDispatch接口指针提供给浏览器。浏览器将回调IDispatch接口的Invoke() 方法，以事件的ID值作为第一参数：
HRESULT CViewSource::Invoke(DISPID dispidMember， REFIID riid， 
   LCID lcid， WORD wFlags， DISPPARAMS* pDispParams， 
   VARIANT* pvarResult， EXCEPINFO* pExcepInfo， UINT* puArgErr)
{
  if (dispidMember == DISPID_DOCUMENTCOMPLETE) {
      OnDocumentComplete();
      m_bDocumentCompleted = true;
  }
  :
}
　　切记，当事件不再需要时，应该使之与浏览器分离。如果你忘记了做这件事情，BHO对象将被锁定，即使在你关闭浏览器窗口之后。很明显，实现分离的最佳时机是收到事件OnQuit时。
**十、存取文档对象**
　　此时，该BHO已经有一个参照指向Internet Explorer的Web浏览器控件并被连接到浏览器控件以接收所有它产生的事件。当网页被全部下载并正确初始化后，我们就可以通过DHTML文档模型存取它。Web浏览器的文档属性返回一个指向文档对象的IDispatch接口的指针：
CComPtr<IDispatch> pDisp;
HRESULT hr = m_spWebBrowser2->get_Document(&pDisp);
　　get_Document() 方法取得的仅仅是一个接口指针。我们要进一步确定在IDispatch 指针背后存在一个HTML文档对象。用VB实现的话，可以用下面代码：
Dim doc As Object
Set doc = WebBrowser1.Document
If TypeName(doc)="HTMLDocument" Then
   '' 获取文档内容并予以显示
Else
   '' Disable the display dialog
End If
　　现在要了解一下get_Document()返回的IDispatch指针 。Internet Explorer不仅仅是一个HTML浏览器，而且还是一个ActiveX文档容器。 这样一来，难以保证当前浏览对象就是一个HTML文档。不过办法还是有的――你想，如果IDispatch指针真正指向一个HTML文档，查询IHTMLDocument2 接口一定成功。
IHTMLDocument2接口包装了DHTML对象模型用来展现HTML页面的所有功能。下面代码实现这些功能：
CComPtr<IDispatch> pDisp;
HRESULT hr = m_spWebBrowser2->get_Document(&pDisp);
CComQIPtr<IHTMLDocument2， &IID_IHTMLDocument2> spHTML;
spHTML = pDisp;
if (spHTML) {
   // 获取文档内容并予以显示
} 
else {
   // disable the Code Window controls
}
如果IHTMLDocument2接口查询失败，spHTML指针将是NULL。 
　　现在考虑如何获得当前显示窗口的源代码。正如一个HTML页把它所有的内容封装在标签<BODY>中，DHTML对象模型要求你取得一个指向Body对象的指针： 
CComPtr<IHTMLElement> m_pBody;
hr = spHTML->get_body(&m_pBody);
　　奇怪的是，DHTML对象模型不让你取得标签<BODY>之前的原始内容，如<HEAD>。其内容被处理并存于一些属性中，但你还是不能从HTML原始文件中提取这部分的RAW文本。这过，仅从BODY部分取得的内容足够了。为了取得包含在<BODY>…</BODY>间的HTML代码部分，可以把outerHTML属性内容读取到一个BSTR变量中：
BSTR bstrHTMLText;
hr = m_pBody->get_outerHTML(&bstrHTMLText);
　　在此基础上，在代码窗口中显示源码就是一种简单的事情了：生成一个窗口，进行字符的UNICODE至ANSI转化和设置编辑框控件的问题。下面代码实现这些功能：
HRESULT CViewSource::GetDocumentContent()
{
  USES_CONVERSION;
  
  // 获取 WebBrowser的文档对象
  CComPtr<IDispatch> pDisp;
  HRESULT hr = m_spWebBrowser2->get_Document(&pDisp);
  if (FAILED(hr))
   return hr;
  // 确保我们取得的是一个IHTMLDocument2接口指针
  //让我们查询一下 IHTMLDocument2 接口 (使用灵敏指针)
  CComQIPtr<IHTMLDocument2， &IID_IHTMLDocument2> spHTML;
  spHTML = pDisp;
  // 抽取文档源代码
  if (spHTML)
  {
    // 取得BODY 对象
    hr = spHTML->get_body(&m_pBody); 
    if (FAILED(hr))
        return hr;
    // 取得HTML 文本
    BSTR bstrHTMLText;
    hr = m_pBody->get_outerHTML(&bstrHTMLText); 
    if (FAILED(hr))
     return hr;
    // 进行文本的Unicode到 ANSI的转换
    LPTSTR psz = new TCHAR[SysStringLen(bstrHTMLText)];
    lstrcpy(psz， OLE2T(bstrHTMLText));
     // 文本进行相应的调整
    HWND hwnd = m_dlgCode.GetDlgItem(IDC_TEXT);
    EnableWindow(hwnd， true);
    hwnd = m_dlgCode.GetDlgItem(IDC_APPLY);
    EnableWindow(hwnd， true);
    // 设置代码窗口中的文本
    m_dlgCode.SetDlgItemText(IDC_TEXT， psz); 
    delete [] psz;
  }
  else   // 文档不是一个 HTML 页
  {
    m_dlgCode.SetDlgItemText(IDC_TEXT， ""); 
    HWND hwnd = m_dlgCode.GetDlgItem(IDC_TEXT);
    EnableWindow(hwnd， false);
    hwnd = m_dlgCode.GetDlgItem(IDC_APPLY);
    EnableWindow(hwnd， false);
  }
  return S_OK;  
}
　　因为我要运行这段代码来响应DocumentComplete事件通知，每个新的页自动地而且敏捷地被处理。DHTML对象模型使你能够随意修改网页的结构，但这一变化在按F5刷新后全部复原。你还要处理一下DownloadComplete事件以刷新代码窗口 (注意， DownloadComplete 事件发生在 DocumentComplete事件之前)。你应该忽略网页的首次DownloadComplete事件，而是在执行刷新动作时才关注这一事件。布尔成员变量m_bDocumentCompleted正是用来区别这两种情形的。
**十一、管理代码窗口**
　　用来显示当前HTML页原始码的代码窗口涉及另外一个ATL 基本编程问题-对话框窗口，它位于ATL对象向导的"Miscellaneous"选项卡下。
　　我调整了代码窗口的大小来响应WM_INITDIALOG消息，使它占居桌面空间的下部区域，正好是在任务栏的上面。在浏览器启动时你可以选择显示或不显示这个窗口。缺省情况下是显示的，但这可以通过清除"Show window at startup"复选框项来实现。当然喜欢的话，你可以随时关闭。按键F12即可重新显示代码窗口。F12是通过在SetSite()中安装的键盘钩子实现的。启动环境存于WINDOWS注册表中，我选择外壳库文件shlwapi.dll中函数SHGetValue来实现注册表的读写操作。这同使用Reg开头的Win32函数操作相比，简单极了。请看：
DWORD dwType， dwVal;
DWORD dwSize = sizeof(DWORD);
SHGetValue(HKEY_CURRENT_USER， _T("Software\\MSDN\\BHO")， _T("ShowWindowAtStartup")， &dwType， &dwVal， &dwSize);
这个DLL文件是同Internet Explorer 4.0 和活动桌面的诞生一起产生的，是WIN98及以后版本的标准组成，你可以放心使用。 
**十二、注册BHO对象**
　　因为BHO 是一个COM 服务器，所以既应该作为COM 服务器注册又应该作为BHO对象注册。ATL向导自动生成.rgs文件，第一种情况的注册就免除了。下面的文件代码段是用来实现作为BHO对象注册的(CLSID为例中生成)。
HKLM {
 SOFTWARE {
  Microsoft {   
   Windows {
    CurrentVersion {
     Explorer {
      ''BHO'' {
       ForceRemove {1E1B2879-88FF-11D2-8D96-D7ACAC95951F}        
}}}}}}}
　　注意ForceRemove一词能够实现在卸载对象时删除这一行相应的键值。BHO键下聚集了所有的BHO对象。对于这么多的一串家伙是从来不作缓冲调用的。这样以来，安装与测试BHO就是不费时的事情了。
**十三、总结**
　　本文描述了BHO对象，通过它你可以把自己的代码注入浏览器的地址空间中。你必须做的事情是写一个支持IObjectWithSite 接口的COM 服务器。在这一点上，你的BHO对象可以实现浏览器机制范围内的各种合法目的。本文所及示例涉及了COM事件，DHTML对象模型以及WEB浏览器编程接口。虽然内容稍宽一些，但它正显示了现实世界中的BHO对象的应用。如，你想知道浏览器在显示什么，那么您就需要了解接收事件并要熟悉WEB浏览器才行。
　　另外：Windows资源管理器也是与BHO对象交互的，这一点在编程时要特别注意。本文所附源程序为MSDN所带，在Windows2000/VC6下调试通过（编译通过后，重新启动IE即得到结果）。
            
