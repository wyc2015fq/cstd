# DirectShow简介 - 建建的博客 - CSDN博客
2017年12月11日 18:07:25[纪建](https://me.csdn.net/u013898698)阅读数：404
3.1 DirectShow 的功用
(1) 保证数据量庞大的多媒体数据处理的高效性； (2) 时刻保持音频和视频的同步； (3) 用简单的方法处理复杂的媒体源问题，包括本地文件，计算机网络，广播电 视以及其他数码产品； (4) 处理各种各样的媒体格式问题，包括 AVI,ASF,MPEG,DV,MOV 等； (5) 支持目标系统中不可预知的硬件； DirectShow
 的设计初衷就是尽量要让应用程序开发人员从复杂的数据传输， 硬件差异， 同步性等工作中解放出来， 总体应用框架和底层工作由 DirectShow 来完成。
图 3.1 DirectShow 系统 Ring0 特权级别的硬件设备，Ring3 特权级别的应用层。 按照功能 Filter 分为三类：Source Filters-主要负责获取数据，数据源可以是 文件，因特网计算机里的采集卡 (WDM 驱动或 VFM 驱动 ) 数字摄像机等； Transform Filters-主要负责数据的格式转换，
 例如数据流分离/合成， 解码/编码等； Rendering Filters-主要负责数据的最终去向将数据传给显卡，声卡进行多媒体的 演示，或者输出到文件进行存储。
3.2 COM 编程基础
如何创建 COM 组件 如何得到组件对象上的接口以及调用接口方法 如何管理组件对象（COM 的引用记数机制）
—DirectShow 应用程序开发人员
COM 本身是一种规范，而不是实现 COM 组件是一个 C++类，其接口都是纯虚类
—Filter 开发人员
Class Ifuction //接口 { public: virtual Method1()=0; virtual Method2()=0; }; Class MyObject:public Ifunction //COM 组件 { public: virtual Method1(); virtual Method2();
 }； CoInitialize(NULL); //COM 库初始化 //Do something IUnkown *pUnk=NULL;//COM 规范规定，任何组件或接口都必须从 IUnkown 接 口中继承 IObject *pObject=NULL; //创建组件对象 HRESULThr=CoCreateInstance(CLISD_Object, CLSCTX_INPROC_SERVER,NULL,IID_IUnkown,(void**)&pUnk); if(SUCCEEDED(hr)) { //查询得到组件对象上的接口
 Hr=pUnk->QueryInterface(IID_IObject,(void**)&pObject; //IUnkown 中负责组件对象上的接口查询的虚函数 If(SUCCEEDED(hr)) { //调用接口方法 pObject->SomeMethod(); //IUnkown 中 AddRef 用于增加引用计数 pObject->Release(); //IUnkown 中 Release 用于减少引用计数 } pUnk->Release; } CoUninitialize();//释放 COM
 库使用资源 引用计数是 COM 中一个非常重要的概念，它很好的解决了组件对象的生命 周期问题，即 COM 组件到底在什么时候被销毁，以及由谁来销毁的问题。 COM 组件有三种类型：进程内组件，本地进程组件和远程组件。Filter 一般 是一种进程内组件，以 DLL(动态链接库)的形式提供服务。 COM 规 范 规 定 ， 每 个 组 件 都 必 须 实 现 一 个 与 之 相 对 应 的 类 工 厂
(ClassFactory). 类工厂也是一个 COM 组件，用 IClassFactory 接口来实现，在 IClassFactory 的接口函数 CreateInstance 中， 才能使用 new 操作生成一个 COM 组件类
对象实例。
CoCreateInstance() { IClassFactory *pClassFactory=NULL; CoGetClassObject(CLSID_Object,CLSCTX_INPROC_SERVER,NULL,IID_ICl assFactory,(void**)&pClassFactory); //通过每个
 COM 组件唯一标识调用 CoGetClassObject 来获得创建这个组件对 象的类工厂 pClassFactory->CreateInstance(NULL,IID_IUnkown,(void**)&pUnk); //调用类工厂接口方法创建 CLSID_Object 标识的真正组件 pClassFactory->Release(); } CoGetClassObject() { //通过查询注册表 CLSID_Object 得知组件 DLL 文件路径 //装入 DLL 库(调用 LoadLibrary)
 //使用函数 GetProcAddress()得到 DLL 中函数 DllGetClassObject 的函数指针 //调用 DllGetClassObject 得到类工厂对象指针 } DllGetClassObject() //必须实现一个（导出）函数，根据指定的组件 GUID 创建相应的类工厂对象 //返回类工厂 IClassFactory 接口 {
//创建类工厂对象 CFactory* pFactory= new CFactory; //查询得到 IClassFactory 指针 pFactory->QueryInterface(IID_IClassFactory,(void**)&pClassFactory); pFactory->Release(); }
 CFactory::CreateInstance()//IClassFactory 接口的方法，负责最终创建组件对象实 例 { //创建 CLSID_Object 对应的组件对象 CObject *pObject= new CObject; //我们的 COM 组件类，实现 COM 框架以外的真正组件功能 pObject->QueryInterface(IID_IUnkown,(void**)&pUnk); pObject->Release(); } 典型自注册 COM 组件 DLL 所必需的 5 个（导出）函数：
 DllMain:DLL 的入口函数（DirectShow 实现的是 DllEntryPoint）; DllGetClassObject:在创建 Filter 对象时被调用， 根据 CLSID 返回对应类工厂指针； DllCanUnloadNow:系统空闲时会调用这个函数，以确定是否可以卸载 DLL; DllRegisterServer:将 COM 组件注册到注册表中； STDAPI DllRegisterServer() { return AMovieDllRegisterServer2(TRUE); }
 DllUnregisterServer:删除注册表中 COM 组件的注册信息； STDAPI DllUnregisterSever() { return AMovieDllREgisterServer2(FALSE); }
第四章 Filter 原理
4.1 Filter 概述
Filter 一般由一个或多个 Pin 组成，Filter 之间通过 Pin 相互连接，构成一条顺序 链路。Filter 是一种 COM 组件，为实现在 FilterGraph 中的统一操作，每个 Filter 上都至少实现了 IBaseFilter 接口，实现 Filter 的文件一般是一个 DLL，扩展名可 以是
 dll,但更多的时候是 ax.。 跟普通的 COM 组件一样， Filter 的创建是通过 API 函数 CoCreateInstance 来完成的： STDAPI CoCreateInstance{ REFCLSID rclsid,//指定要创建的 Filter 的 CLSID LPUNKOWN pUnkOutter, //绝大多数情况下创建的 Filter 不是被“聚合“的，所以 pUnkOuter 指定为 NULL DWORD dwClsContext,//可指定为 CLSCTX_INPROC_SERVER，
 以创建进程内组 件对象
REFIID riid, //riid 在创建 Filter 成功后获得的接口的 ID,一般为 IID_IBaseFilter,也可以是其他 特殊接口 LPVOID *ppv//用于获得接口对象的指针 }
4.2 Filter 的注册
Filter 的注册程序为 regsvr32.exe （位于操作系统目录的 system32 子目录下） 。 假设现在有一个 Filter 文件，它的完整路径为 C:\DSFilter\myFilter.ax. 注册这个 Filter 的方法为：在命令行的状态下，执行 regsvr32 C:\DSFilter\myFilter.ax.随后
 会弹出一个对话框，告诉 Filter 是否注册成功。注销 Fliter 也使用 regsvr32.exe 程序，方法为加命令行参数/u，即执行 regsvr32/u C:\DSFilters\myFilter.ax. 判断 Filter 是否注册 BOOL IsFilterRegistered(CLSID inFilterId) { IBaseFilter *pFilter =NULL; If(SUCCEEDED(CoCreateInstance(inFilterId,NULL,CLSCTX_INPRO_SERVER,
 IID_IBaseFilter,(void)**&pFilter))) {pFilter->Release(); return TURE;} Return FALSE; } 在应用程序中注册（或注销）某个 Filter 文件 只要在应用程序中使用 LoadLibrary 装载这个 Fliter 文件，并得到它的导出 函数 DllRegisterServer(DllUnregisterSever)的入口地址，然后执行。 BOOL RegisterFilter(const char *inFilterAx) {
 typedef (WINAPI *REGISTER_FUNC) (void); REGISTER_FUNC MyFunc= NULL; HMODULE hModule =::LoadLibrary(inFilterAx); If(hModule) { MyFunc=(REGISTER_FUNC)GetProAddress(hModule,”DllRegisterServer”); BOOL pass=(MyFunc!=NULL); If(pass) { MyFunc(); } ::FreeLibrary(hModule);
 return pass; } return FLASE;
} Filter 的注册信息一般包括两部分：基本的 COM 信息和 Filter 特有信息。前 者是基本的 COM 组件所必需的信息；后者是描述 Filter 的信息（包括 Filter 注 册的类型目录，Filter 上的 Pin 数量，支持的媒体类型等） ，这部分内容会被系 统枚举器或者 Filter 影射器访问到，但不是必须的。
4.3 Filter 的媒体类型
类型媒体用来描述格式化的数据流， DirectShow 中定义一个数据结构 AM_MEDIA_TYPE： typedef struct _MediaType{ GUID majortype;//主类型 GUID subtype;//辅助说明类型 BOOL bFixedSizeSample;// BOOL bTemporalcompression;
 ULONG ISampleSize; GUID formattype;//格式细节类型 IUnkown *pUnk; ULONG cbFormat; /*[size_is]*/ BYTE *pbFormat; }AM_MEIDA_TYPE; 当 使 用 AM_MEDIA_TYPE 数 据 结 构 描 述 媒 体 类 型 时 ， 如 果 majortype,subtype 和 formatype 都指定一个特定的 GUID 值，称为“完全指定媒 体类型” ；这三个部分只要有一个指定为 GUID_NULL，则称之为“不完全指定
 的媒体类型” 。GUID_NULL 具有“通配符”的作用。
4.4 Filter 的连接
Filter 的 连 接 世 上 是 Fliter 上 Pin 的 连 接 ， 连 接 的 方 向 总 是 由 上 一 级 Filter(Upstream Filter)的输出 Pin 指向下一级 Filter(Downstream Filter)的输入 Pin。 Pin 的连接实际上是连接双方使用的媒体类型的一个“协商”过程。
4.4.1 连接过程
Pin 也是一种 COM 组件， 而且每个 Pin 上都实现了 IPin 接口。 首先连接 Filter 的 是 应 用 程 序 ， 一 般 通 过 调 用 接 口 方 法 IFilterGraph::ConnectDirect, IGraphBuilder::Connect, IGraphBuilder::Render
 或者 IGraphBuilder::RenderFile 来 实现。 试图连接的两个 Filter 必须处在同一 Filter Graph 中，调用接口方法 IFilterGraph::AddFilter 将指定的 Filter 加入到 FilterGraph 中。对于 Filter 的加入 或 者 移 走 ， Filter Graph Manager 会 调 用 该 Filter 上 的 接 口 方 法
IBase-Filter::JoinFilterGraph 通 知 。 对 于 Filter 开 发 人 员 来 说 ， 可 以 重 写 CBaseFilter::Join-FilterGraph 函数来捕捉此事件。
整个连接过程的大致步骤： （1） Filter Graph Manager 在输出 Pin 上调用 IPin::Connect（带输入 Pin 的指针 作为参数） ； （2） 如果输出 Pin 接受连接，则调用输入 Pin 上的 IPin::ReceiveConnection； （3） 如果输入 Pin 也接受这次连接，则双方连接成功；
 STDMETHODIMP CBasePin::Connect(IPin* pReceivePin, Const AM_MEDIA_TYPE *pmt //optional media type) { CheckPointer(pReceivePin,E_POINTER);//指针是否有效 ValidateReadPtr(pReceivePin,sizeof(IPin));//分配内存是否成功 CAutoLock cObjectLock(m_pLock);//CCirSec Class 临界 DisplayPinInfo(pReceivePin);
 //检查该 Pin 是否早已连接 if(m_Connected){ DbgLog((LOG_TRACE,CONNECT_TRACE_LEVEL,TEXT(“Already connected”))); return VFW_E_ALREDY_CONNECTED; } //一般 Filter 只能在停止状态下进行连接 If(!IsStopped()&&!m_bCanReconnectWhenActive){ return VFW_E_NOT_STOPPED;} //开始媒体类型的检查过程，找出一种连接双方都支持的媒体类型
 const CMediaType *pType=(CMediaType*)pmt; HRESUSLT hr=AgreeMeidaType(pReceivePin,pType);//pType 必 须 是 const
if(FAILED(hr)){ DbgLog((LOG_TRACE,CONNECT_TRACE_LEVEL,TEXT(“Failed to agree type”))); EXECUTE_ASSERT(SUCCEEDED(BreakConnect())); return hr;} DbgLog((LOG_TRACE,CONNECT_TRACE_LEVEL,TEXT(“Connection
 succeeded”))); return NOERROR; } 上述函数没有真正实现 Pin 连接过程， 只是进行了参数的检查以及状态的检 查，这个函数是从输出 Pin 进入的；对于真正的连接过程，还要进一步查看 AgreeMediaType 函数： HRESULT CBasePin::AgreeMediaType(IPin *pReceivePin,const CMediaType *pmt) { ASSERT(pReceivPin);//诊断函数，如返回失败，输出宏信息 IEnumMediaTypes
 *pEnumMediaTypes=NULL; //判断 pmt 是不是一个完全指定的媒体类型 if((pmt!=NULL)&&(!pmt->IsPartiallySpecified())){ //用这个完全指定的媒体类型进行测试连接， 如果连接失败， 不再作其他 尝试 return AttemptConnection(pReceivePin,pmt); } HRESULT hrFailure = VFW_E_NO_ACCEPTABLE_TYPES; //进行 Pin 上支持的媒体类型枚举，开始媒体类型的“协商”过程
 for(int i=0;i<2;i++) { HRESULT hr; if(i= =(int)m_bTryMyTypesFirst){//输出 Pin 上的成员变量， 初始值为 false hr=pReceivePin->EnumMediaTypes(&pEnumMediaTypes); }eslse{ hr=EnumMediaTypes(&pEnumMediaTypes);}//输出 Pin 上的枚举器 if(SUCCEEDED(hr)){ ASSERT(pEumMediaTypes); hr=TryMediaTypes(pReceivePin,pmt,pEnumMediaTypes);
 pEnumMediaTypes->Release(); if(SUCCEEDED(hr)){ return NOEEROR; }else{ If((hr!=E_FAIL)&& (hr!=E_INVALIDARG) && (hr!=E_VAW_E_TYPE_NOT_ACCEPTED) ){ hrFailure=hr;} }
} } return hrFailure; } 如果 pmt 是一个空指针，或者 pmt 包含的是一个不完全指定的媒体类型， 那么真正的协商过程开始了。注意，for 循环的循环次数为 2：输出 Pin 上的成 员变量 m_bTryMyTypesFires 初始值为 FLASE。 也就是说， 连接进程进行到这里， 会首先得到输入
 Pin 上的媒体类型枚举器的试连接；如果不成功，再得到输出 Pin 上的媒体类型枚举器的试连接。 HRESULT CBasePin::TryMediaTypes(IPin *pReceivePin,const CMediaType *pmt, IEnumMediaTypes *pEnum) { //复位枚举器内部状态 HRESULT hr=pEnum->Reset(); if(FAILED(hr)){ return hr;} CMediaType *pMediaType=NULL; ULONG ulMediaCount=0;
 HRESULT hrFaillure =S_OK; for(;;){ hr=pEnum->Next(1,(AM_MEDIA_TYPE**)&pMediaType,&ulMediaCount); //枚举 Pin 上所提供的所有媒体类型 if(hr!=S_OK){ (S_OK==hrFailure){ hrFailure=VFW_E_NO_ACCEPTABLE_TYPE;} return hrFailure;} ASSERT(ulMediaCount= =1); ASSERT(pMediaType); //检查当前枚举得到的媒体类型是否与不完全指定的媒体类型参数匹配
 if((pmt==NULL)||pMediaType->MatchesPartial(pmt)){// 是否跟不完全类型 匹配 //进行试连接 hr=AttempConnection(pReceivePin, pMediaType);//是否跟输入 Pin 媒体匹 配 if(FAILED(hr)&& SUCCEEDED(hrFailure)&& (hr!=E_FAIL) && (hr!=E_INVALIDARG) && (hr!=E_VAW_E_TYPE_NOT_ACCEPTED)){ hrFailure=hr;}
 else{ hr=VFW_E_TYPE_NO_ACCEPTABLE_TYPES;} if(S_OK==hr){
return hr;} } } 当连接进程进入 TryMediaTypes 函数后，会使用媒体类型枚举器枚举 Pin 上 提供的所有媒体类型，然后一种一种的进行试连接（调用 AttemptConnection 函 数） 。在枚举过程中，如果有一种媒体类型试连接成功，那么整个 Pin 连接过程 也就宣告成功。如果媒体类型都未获成功，TryMediaTypes
 函数会返回一个错误 的值。如果使用输入 Pin 和输出 Pin 的媒体类型枚举器进行连接都失败了，那么 本次 Pin 连接宣告彻底失败。 最后需要看看 AttempConnection 函数中所做的工作。对于 Filter 开发人员， 这部分内容显得尤为重要。因为可以从 AttemptConnection 函数的实现中，清楚 的看到一些基类跟 Pin 连接相关的虚函数的调用顺序， 这对以后自己写 Filter 时， 选择需要重写的基类虚函数具有指导意义。 HRESULT CBasePin::AttemptConnection{IPin
 *pReceivePin,const CMediaType* pmt} { //获取 Filter 对象上的操作权 ASSERT(CritCheckIn(m_pLock)); HRESULT hr=CheckConnect(pReceivePin); if(FAILED(hr)){ DbgLog((LOG_TRACE,CONNECT_TRACE_LEVEL,TEXT(“CheckConnect faliled”))); EXECUTE_ASSERT(SUCCEEDED(BreakConnect())); return
 hr;} //一个很有用的调试函数，可以显示媒体类型 DisplayTypeInfo(pReceivePin,pmt); //Pin 上的媒体类型检查 hr=CheckMediaType(pmt); if(hr==NOERROR){ m_Connected=pReceivePin;//输出 Pin 上的成员变量 m_Connected->AddRef(); hr=SetMediaType(pmt);//在输出 Pin 上保存媒体类型 if(SUCCEEDED(hr)){ //询问连接对方 Pin 是否也能接受当前的媒体类型
 hr=pReceivePin->ReceiveConnection((IPin)this,pmt); if(SUCCEEDED(hr){ //连接完成 Hr=CompleteConnect(pReceivePin); if(SUCCEEDED(hr)){ return hr;} else{ DbgLog((LOG_TRACE,CONNECT_TRACE_LEVEL,TEXT(“Failed to complete connection”))) ;}}}}
else{ if(SUCCEEDED(hr)||(hr==E_FAIL)||(hr==E_INVALIDAGR)){ hr=VFW_E_TYPE_NOT_ACCEPTED;}} EXECUTE_ASSERT(SUCCEEDED(BreakConnect())); /*If failed then undo our
 state*/ if(m_Connected){ m_Connected->Release(); m_Connected=NULL;} return hr; } 在 AttemptConnection 函数实现中，首先调用输出 Pin 上的 CheckConnect 函 数进行连接检查，例如查看接受连接的 Pin 是否支持某个特殊的接口等。如果 CheckConnect 失败，会调用 BreakConnect 。 如果成功，会继续调用输出 Pin 上 的 CheckMediaType，进行连接用的媒体类型检查。如果
 CheckMediaType 也成 功，则会将输入 Pin 对象指针保存在输出 Pin 对象中，然后调用输入 Pin 上的 ReceiveConnection 函数。该函数在输入 Pin 上也进行了一系列检查，检查内容以 及次序与上述输出 Pin 上的类似。如果 ReceiveConnection 调用成功，最后就调 用输出 Pin 上的 CompleteConnect 函数。 输出 Pin 上的 CompleteConnect 函数中完成收尾工作如：用于数据传送的内 存怎么分配，谁来管理这些内存，然后才能在
 Pin 之间开始传输数据： HRESULT CBaseOutputBin::CompleteConnect(IPin *pReceivePin) { UNREFERENCED_PARAMETER(pReceivePin); return DecideAllocator(m_pInputBin,&m_pAllocator);//完成内存分配器协商} 在 DirectShow 中，数据传送单元叫做 Sample（COM 组件，管理一块数据内 存） ；而 Sample 是由分配器 Allocator（COM 组件）来管理。连接双方的
 Pin 必 须使用同一个分配器但是这个分配器到底由哪个 Pin 来建立也需要协商。 HRESULT CBaseOutputPin::DecideAllocator(IMemInputPin* pPin,IMemAllocator **ppAlloc) { HRESULT hr=NERROR; *ppAlloc=NULL; ALLCATOR_PROPERTIES prop; ZeroMemory(&prop,sizeof(prop)); //询问输入 Pin 对分配器的要求 pPin->GetAllocatorRequirements(&prop);
 if(prop.cbAlign= =0){ prop.cbAlign=1;} // 询问输入 Pin 是否提供一个分配器 hr=pPin->GetAllocator(ppAlloc); if(SUCCEEDED(hr)) { //决定 Sample 使用的内存大小，以及分配器管理的 Sample 数量 hr=DecideBufferSize(**ppAlloc,&prop);
if(SUCCEEDED(hr)){ //通知输入 Pin 最终使用的分配器对象 hr=pPin->NotifyAllocator(*ppAlloc,FALSE); if(SUCCEEDED(hr)){ return NOERROR; }}} //如果输入 Pin 上不提供分配器，则必须在输出 Pin 上创建一个分配器
 if(*ppAlloc){ (*ppAlloc)->Release(); *ppAlloc=NULL;} // 创建一个输出 Pin 上的分配器 hr= InitAllocator(ppAlloc); if(SUCCEEDED(hr)){ hr=DecideBufferSize(*ppAlloc,&prop); if(SUCCEEDED(hr)){ hr=pPin->NotifyAllocator(*ppAlloc,FALSE); if(SUCCEEDED(hr)){ return NOERROR;}}}
 if(*ppAlloc){ (*ppAlloc)->Release(); *ppAlloc=NULL;} return hr; } 当 Pin 上的数据传送内存分配协商成功后，实际上并没有马上分配 Sample 的内存。实际内存的分配，一般是在 FilterGraph 运行之后，调用输出 Pin 上的 Active 函数时进行的，代码如下： HRESULT CBaseOutputPin::Active(void){ if(m_pAllocator==NULL){ return VFW_E_NO_ALLOCATOR;}
 return m_pAllcator->Commit(); }
4.4.2 智能连接
FilterGraph 的构建方法： IFilterGraph::AddFilter:该参数提供一个 Filter 对象，将其加入到 FilterGraph 中 IFilterGraph::ConnectDirect:该参数提供输出 Pin,输入 Pin 以及媒体类型，进行 直接的连接 IGraphBuilder::AddSourceFilter::该参数提供源文件名，自动将一个
 SourceFilter 加载到 FilterGraph 中 IGraphBuilder::Connect:该参数提供输出 Pin 和输入 Pin 进行连接，如果连接失 败，自动尝试在中间插入必要的格式转换 Filter IGraphBuilder::Render:该参数提供输出 Pin，自动加入必要的 Filter 完成剩下部 分 Filter Graph 的构建（直到连接到 Rendering Filter） IGraphBuilder::RenderFile:该参数提供源文件名，自动加入必要的 Filter
 完成这
个文件的回放 FilterGraph 构建 我们可以看到后四种构建方法都有“自动”功能，在 DirectShow 中，称为 智能连接。在介绍智能连接的具体实现过程前，先要来看一下 Filter 的 Merit 值。 这个 Merit 值可以理解成为 Filter 被智能连接错使用的优先级。DirectShow 定义
 如下六种标准的 Merit 值： Enum { MERIT_PREFERED = 0x800000, MERIT_NORMAL =0x600000, MERIT_UNLIKELY= 0x400000, MERIT_DO_NOT_USE =0x200000, MERIT_SW_COMPRESSOR =0x100000, MERIT_HW_COMPRESSOR =0x100050}; Filter 的 Merit 值只有在大于 MERIT_DO_NOT_USE 的时候，才有可能被智 能连接所使用，而在等同条件下（几个
 Filter 完成的功能相同） ，Merit 的值越大， 这个 Filter 被使用的机会就越大。 打开注册表，在 Filter 信息的注册部分都会发现一个称为 FilterDate 的二进 制串类型的键值，这个键值记录的内容就包含 Merit 信息，可以如下定义一个数 据结构来操作 FilterData 值开始部分的一块数据： Typedef struct { DWORD dwVersion;//版本号 DWORD dwMerit;//Merit 值 DWORD dwPinCount;//Pin 的数量
 DWORD dwReserved;//保留 }FILTER_HEADER; BOOL SetFilterMerit(const char* inClsid,DWORD inMerit) { const char* cRegistryEntry =”CLSID\\{083863f1-70DE-11d0-BD4000A0C911CE86}”}\\Instance\\”; const long cMaxLength=1024*16; BYTE filterData[cMaxLength]; DWORD actualLength=0;
 //生成 Filter 信息注册部分的注册表入口 char szEntry[1000]; strcpy(szEntry,cRegistryEntry);//前面是目的文件，后面是源文件 strcat(szEntry,inClsid);//追加写 HKEY hKey=NULL; LONG result =::RegOpenKeyEx(HKEY_CLASSES_ROOT,szEntry,0, KEY_ALL_ACCESS,&hKey); BOOL pass=(result==ERROR_SUCCESS); if(pass)
 { //读取 FilterData 的值
actualLength =actualLength; result=::RegQueryVallueEx(hKey,”FilterData”,NULL, NULL,filterData,&actualLength); pass =(result ==ERROR_SUCCES); } if(pass) { //修改
 FilterData 中的 Merit 部分，然后写回注册表 FILTER_HEADER *filterHeader=(FILTER_HEADER) filterData; filterHeader->dwMerit = inMerit; result = ::RegSetValueEx(hKey,”FilterData”,NULL, REG_BINARY,filterData,actualLength); Pass= (result = =ERROR_SUCCESS); } if(hKey) { ::RegCloseKey(hKey);}
 return pass;} IGraphBuilder::AddSourceFilter( LPCWSTR lpwstrFileName, LPCWSTR lpwstrFilterName,IbaseFilter **ppFilter 该方法给出一个源文件路径，FilterGraphManager 能够为这个源文件加载一 个相应的 SourceFilter。下面是分析文件路径以及查询注册表的过程。 首先分析文件路径中是否有协议名 （如 http,htp 等， FilterGraphManager 认为 冒号前的字符串为协议名，
 如 myprotocol://myfile.ext 中， myprotocol 就是协议名） 。 如果有则在注册表的如下位置寻找这个协议名。 HKEY_CLASSES_ROOT <protocol> Source Filter =<Source filter CLSID> Extensions <.ext1>=<Source filter CLSID> <.ext2>=<Source filter CLSID> 找到后，先在 Extensions 子键下匹配扩展名，如果扩展名匹配不成功，则使 用 SourceFilter
 键值指定的 CLSID,默认情况下 （注册表中找不到文件路径中的协 议名） ，FilterGraphManager 创建一个称为 FilterSource(URL)的 Filter. 如果文件路径中不包含协议名，FilterGraphManager 就分析这个文件的扩展 名， 然后到注册表的 HKEY_CLASSES_ROOT\Media Type\Extensions 子键下去匹 配扩展名。如果找到了，就根据 Source Filter 键值指定的 CLSID 创建 Filter;找不 到，FilterGraphManager
 最后只能进行文件的校验字节判断。 HRESULT IGraphBuilder::RenderFile(LPCWSTR lpwstrFile,LPCWSTR lpwstrPlayList) 该方法给出一个文件名，首先根据与 IGraphBuilder::AddSourceFilter 类似的 算法找到创建正确的 SourceFilter。然后，就从该 SourceFilter 的各个输出 Pin 开 始，进行剩下的智能连接过程。这是一个“递归”过程，直到所有的分支都连接 到一个 Rendering Filter
 上为止。
如 果 输 出 Pin 支 持 IStreamBuilder 接 口 ， 则 把 剩 下 的 工 作 交 给 IStreamBuilder::Render; 使用在 FilterGraphManager 内部缓冲中的 Filter 进行试连接； 使用在当前 FilterGraph 中还没有完全连接的 Filter
 进行试连接。使用 IFilterMapper2::EnumMatchingFilters 搜索注册表。FilterGraphManager 使用 Merit 值大于 MERIT_DO_NOT_USE 的所有 Filter （Filter 所在的类型目录的 Merit 值也 应该大于 MERIT_DO_NOT_USE）进行试连接。在匹配 Mediatype 的前提下， Merit 值越高，该 Filter 被使用的概率就越高。 HRESULT IGraphBuilder::Render(IPin *ppinOut)
 这个方法从当前 FilterGraph 的某个 Filter 的指定输出 Pin 开始，进行从这个 Pin 往下的一条支路的智能连接。智能连接的算法和上述 RenderFile 的类似。 HRESULT IGraphBuilder::Connect(IPin *ppinOut,IPin *ppinIn) 这个方法以欲连接的一对输出 Pin 和输入 Pin 作为参数，首先进行这两个 Pin 之间的直接连接。 如果不成功， 则要插入 “中介” Filter。 这个 “中介” Filter 的选择以及试连接过程就是一个智能连接过程，
 算法与上述的 RenderFile 类似。 在执行智能连接函数之前，可以首先将我们的 Filter 加入到 FilterGraph 中， 或者在提高一个优先级：将 Filter 加入 FilterGraphManager 的内部缓冲。后一方 法可以从 FilterGraphManager 上获得 IGraphConfig 接口，然后调用其接口方法 AddFilterToCache 来实现。值得注意的是，如果要加入到缓冲中的 Filter 已经在 FilterGraph 中了，则这个 Filter 的所有 Pin
 必须都处于断开状态。
4.4.3 动态重建技术
由于下列任何一个原因，我们都需要对已有的 FilterGraph 进行修改 1) 应用程序在播放一段视频的过程中想要插入一个视频效果 Filter 2) SourceFilter 在运行过程中改变了数据流的媒体类型，需要接入新的编码 Filter 3) 应用程序想要在 FilterGraph 中加入另外一条视频流
 通常的做法是，先将 FilterGraph 停止，进行修改之后，再重新启动。动态 重建能够保持 FilterGraph 运行状态的同时实现 FilterGraph 的重建： 1）仅仅改变 Filter 之间连接的媒体类型； 2）增加或删除 Filter,重新进行相关 Filter 之间的连接； 3）对一条 Filter 链路(Filter Chain)进行操作； 媒体类型的动态改变 当两个 Pin 连接完成后， 就会有一个双方商定的媒体类型用来描述以后在这 两个连接 Pin 之间传送的数据格式。一般情况下，这个媒体类型在
 FilterGraph 运行的整个过程中是不会改变的。但是在一些特殊的场合下， “需要”这种媒体 类型的动态改变。 从上往下要求媒体类型改变，但传送数据使用的内存不需要增大 媒体类型的改变由 FilterA 的输出 Pin 发起。首先，Filter A 调用 Filter B 的 输入 Pin 上的 IPin::QueryAccept 或 IPinConnetion::DynamicQueryAccept,看 Filter B 是否支持新的媒体类型。 如支持， 则 Filter A 调用 IMemAllocator::GetBuffer
 得到 一空 Sample,通过 IMediaSample:: SetMediaType 将新的媒体类型与 Sample 相连。 一般来说，Filter B 应该具有对媒体类型改变的应变能力，这是在写自己的
Filter 时应该考虑到的问题。Filter A 一般调用 QueryAccept 来判断 Filter B 的输 入 Pin 是否支持新的媒体类型。可以从 CBasePin::QueryAccept 的实现中看到， 它仅仅是调用 Pin 上的 CheckMediaType
函数。 所以 QueryAccept 返回 S_OK 并不能保证媒体类型能够在 Graph 运行状态 下改变成功，另一种可靠的方法是，如果 Filter B 的输入 Pin 支持 IPinConnection 接口，那么调用这个接口的 DynamicQueryAccept 方法。 对于 Filter 开发人员来说，还需注意一下
 CBaseInputPin::Receive 的实现，代 码如下： STDMETHODIMP CBaseInputPin::Receive(IMediaSample*pSample) { CheckPointer(pSample,E_POINTER); ValidateReadPtr(pSample,sizeof(IMediaSample)); ASSERT(pSample);//上级输出 Sample //检查 Filter 内部状态，以及数据流传送状态 HRESULT hr=CheckStreaming();
 if(S_OK!=hr){ return hr;} //查询 IMediaSample2 接口，获取输入 Sample 上的属性 IMediaSample2 *pSample2;//本级输入 Sample if(SUCCEEDED(pSample->QueryInterface(IID_IMediaSample2,(void**)&pSamp le2))){ hr =pSample2->GetProperties(sizeof(m_SampleProps),(PBYPE)&m_SampleProps); pSample2->Release();
 if(FALED(hr)){ return hr;}} else{ //使用 IMediaSample 接口获取输入 Sample 上的属性 m_SampleProps.cbData =sizeof(m_SampleProps); m_SampleProps.dwTypeSpecificFlags=0; m_SampleProps.dwStreamId =AM_STREAM_MEDIA; m_SampleProps.dwSampleFlags = 0; if(S_OK = =pSample->IsDiscontinuity()){
m_SampleProps.dwSampleFlags|=AM_SAMPLE_DATADISCONTIN UITY;} if(S_OK = =pSample->IsSyncPoint()){ m_SampleProps.dwSampleFlags|= AM_SAMPLE_SPLICEPOINT;} if(S_OK
 = =(SUCCEEDDED(pSample->GetTime(&m_SampleProps.tStart, &m_SampleProps.tStop)) ){ m_SampleProps.dwSampleFlags|=AM_SAMPLE_TIMEVALID| AM_SAMPLE_STOPVALID;} if(S_OK = =pSample->GetMediaType(&m_SampleProps.pMediaType)){ m_SampleProp.dwSampleFlags|=AM_SAMPLE_TYPECHANGED;}
 pSample->GetPointer(&m_SampleProps.pbBuffer); m_SampleProps.lActual = pSample->GetActualDataLength(); m_SampleProps.cbBuffer = pSample->GetSize(); } //检测 Sample 上的媒体类型是否改变 if(!(m_SampleProps.dwSampleFlags& AM_SAMPLE_TYPECHANGED)){ return NOERROR;} //检查输入 Pin
 上是否支持这种新的媒体类型。此时，输入 Pin 应该是支 持这种媒体 //类型的，因为上一级 Filter 在这种媒体类型的动态改变之前，一般会先 调用输入 Pin //上的 IPin::QueryAccept 接口方法进行确认 Hr= CheckMediaType((CMediaType *)m_SampleProps.pMediaType); If(hr == NOERROR){ Return NOERROR;} M_bRunTimeError = TRUE; EndOfStream(); m_pFilter->NotifyEvent(EC_ERRORABORT,VFW_E_TYPE_NOT_AC
 CEPTED,0); return VFW_E_INVALIDATETYPE;} 我们可以看到， 这个函数首先调用 CheckStreaming 来检查 Filter 的运行状态， 其次就是读取 Sample 的属性（保存到成员变量 AM_SAMPLE2_PROPERTIES m_SampleProps 中） ，判断媒体类型是否已经改变，如果已改变，会重新调用 CheckMediaType 进行检查。这就是 SDK 基类提供给我们的（也是仅有的） ，对 媒体类型动态改变的“免疫能力” 。其实，对于媒体类型动态改变的支持，还需
 要处理更多的细节问题（如可能需要重新初始化） ，这要因具体的 Filter 任务而 定。 从下往上要求媒体类型改变，但传送数据使用的内存不需增大，Filter B 必 须拥有自己的 Sample 分配器 媒体类型的改变由 Filter B 的输入 Pin 发起。首先，Filter B 调用 Filter A 的 输出 Pin 的 QueryAccept，看是否支持新的媒体类型。如果支持，则 Filter B 内部 通过私有方法来改变 Sample 管理器中下一个空 Sample 的媒体类型（因为这个 分配器是
 Filter B 创建的，所以 Filter B 有方法对下一个空 Sample 调用
IMediaSample::SetMediaType, 而 无 需 通 过 公 有 接 口 方 法 IMediaAllocator::GetBuffer 先将下一空 Sample 取出） 。接着当 Filter A 调用 IMediaAllocator
::GetMediaType 来得到新的媒体类型。此时，FilterA 必须根据新的媒体类型产生 Sample 数据，然后再传给 Filter B。最后，FilterB 在接收到 Sample 时，会确认 媒体类型的改变。 如果 Filter A 能够接受新的媒体类型，那么它也应该有能力恢复到原来的媒 体类型。这种媒体类型的动态改变，最常见于传统的
 Video Renderer 上。 输出 Pin 往下要求新的媒体类型，而且需要使用更大的内存以传送新的 Sample 在上述的两种情形中，虽然媒体类型改变了，但是 Sample 使用的内存大小 并不需要扩大。 当新的媒体类型要求更大的内存用于传送数据时， 上一级的 Filter 必须进行如下处理： （1） 调用下一级 Filter 的输入 Pin 的 IPin::ReceiveConnection.如果成功返回， 且 需 要 改 变 Sample 内 存 大 小 ， 则 在 输 出 Pin 上 调 用 IMemAllocator::SetProperties,以使用新的内存大小进行
 Sample 内存的 重新分配。 （2） 调用输入 Pin 上的 IMemInputPin::NotifyAllocator 通知使用新的 Sample 分配器。 对于 Filter 开发人员来说，还需要注意的是，在做出上述改变之前，确信已 经将所有旧媒体类型的 Sample 都发送出去了。一些 MPEG 视频解码 Filter 就是 使用这种机制来实现输出 MPEG-1 与 MPEG-2 之间的切换，或者视频图象大小 的改变的。 动态删除或增加 Filter 假设我们想将 Filter2 动态移走
要实现动态删除或增加 Filter 的两个必要条件： （1）Filter3(Pin D)必须支持 IPinConnection 接口 （这个接口保证 Filter 在非停止状态下也能进行 Pin 的重连） ； （2） Filter 在重连的时候不允许数据的传输，所以要阻塞数据线程。如果“重连”
是由 Filter1 发起的（在 Filter 内部完成） ，那么 Filter1 要同步这个数据发送线程； 如果 “重连” 是由应用程序完成， 则要求 Filter1(PinA)实现 IPinFlowControl 接口。 （1） 在 Filter1 上阻塞数据流线程。IPinFlowControl::Block
 可以工作在同步和 异步两种模式下。不能在应用程序主线程下使用该 Block 函数的同步模 式，因为这样可能会引起死锁 (Dead-Block). 应该另外使用一个工作者线 程，或者使用 Block 函数的异步模式。 //创建一个同步时间对象 HANDLE hEvent =CreateEvent(NULL,FALSE,FALSE,NULL); If(hEvent !=NULL){ //阻塞数据流线程 Hr= pFlowControl->Block(AM_PIN_FLOW_CONTROL_BLOCK,hEvent);
 If(SUCCEEDED(hr)){ //等待完成阻塞工作 DWORD dwRes =WaitForSingleObject(hEvent,dwMilliseconds);}} （2） 重 连 Pin A 和 D ， 必 要 时 插 入 新 的 Filter 。 Pin 的 重 连 可 以 使 用 IGraphConfig::Reconnect 或 IGraphConfig::Reconfigure(IGraphConfig 接口 可以从 Filter Graph Manager 上获得)。Reconnect
 比 Reconfigure 使用起来 要简单，主要做以下几件事：将新加入的 Filter 置于暂停或者运行状态， 以使其与所在的 FilterGraph 同步。 pGraph->AddFilter(pNewFilter,L”New Filter for the Graph”); pConfig->Reconnect( pPinA,// Reconnect this output pin… pPinD,// …to this input pin pMediaType,//Use this media type
 pNewFilter,//Connect them through this filter NULL,0); 在实际应用中，如果觉得 Reconnect 不够灵活，还可以改用 Reconfigure.使 用方法时，必须在应用里实现 IGraphConfigCallBack 接口；在调用 Reconfigure 之前，还必须依次调用 Filter3(PinD)上的 IPinConnection::NotifyEndOfStream 和 Filter2(PinB)上的 IPin::EndOfStream,以使得还没有处理完的数据全部发送下去。
 （ 3 ） 再 次 启 动 Filter1(PinA) 上 的 数 据 发 送 线 程 。 只 需 要 调 用 IPinFlowControl::Block: pFlowControl->Block(0,NULL); Filter Chain 操作 Filter Chain 是相互连接着的一条 Filter 链路，并且链路中的每个 Filter 至 多有一个处于“已连接”状态的输入 Pin,至多有个处于“已连接”状态的输出 Pin。这条 Filter 链路中的数据流不依赖链路外的其他 Filter。图中的
 A-B，C-D， F-G-H，F-G，G-H 都是 Filter Chain,而任何含有 E 的都不是 Filter Chain.
IFilterChain 接口方法
StartChain 将指定起始 Filter 和结束 Filter 的 FilterChain 置于运行状态 StopChain 将指定起始 Filter 和结束 Filter 的 FilterChain 置于停止状态 RemoveChain 将指定起始 Filter 和结束 Filter 的 FilterChain
 中的所有 Filter 从 FilterGraph 中删除 PauseChain 将指定起始 Filter 和结束 Filter 的 FilterChain 置于暂停状态 没有一个专门的接口方法可以加入或者连接一个 FilterChain。 Filter Chain 的 概念是通过 IFilterChain 的接口方法的两个参数 （一个指定起始的 Filter,一个指定 的结束的 Filter）来体现的。 在使用 IFilterChain 接口前， 还需要确认要操作的 Filter 是否支持 FilterChain
 操作，否则很容易引起死锁。一般为了使用 FilterChain 特性，需要特殊设计一些 Filter.
4.5 Filter 的数据传送
空闲的 Sample 队列
Filter 之间的成功连接为数据传送做好了准备， 前面已经提及 Filter 之间是 以 Sample 的形式来传送数据的。 Sample 是一个封装了一定大小数据内存的 COM 组件。每个 Pin 上都实现了 IPin 接口，但这个接口主要是用于 Pin 连接的，而不 是用于数据传送的。 真正用于数据传送的一般是输入
 Pin 上实现的 IMemInputPin 接口（调用其接口方法 IMemInputPin:: Receive） ； 连接着双方 Pin 拥有同一个 Allocator（Sample 分配器） ；Allocator 创建，管 理一个或多个 Sample.数据传送时，上一级 Filter 从输出 Pin 的 Allocator 中（调 用 IMemAllocator:: GetBuffer）得到一个空闲 Sample,然后得到 Sample 的数据内 存地址， 将数据放入其中。 最后再将这个 Sample 传送给下一级
 Filter 的输入 Pin。 数据从上一级 Filter 输出 Pin 传送到下一级 Filter 的输入 Pin,并没有进行数据的内 存备份，而只是数据到达的“通知” 。典型的数据内存备份发生在 Filter 内部， 从这个 Filter 的输入 Pin 复制到 Filter 的输出 Pin. 数据传送主要有两种模式：推模式(Push Model)和拉模式(Pull Model) 推模式 推模式最典型的情况发生在 LiveSource（实时源，如采集卡的 Capture Filter 等）中。这种源能够自己产生数据，并且使用专门的线程将这些数据“推”下去。
 数 据 从 Capture Pin 出 来 ， 调 用 AVI Decompressor 的 输 入 Pin 上 的 IMemInputPin::Receive 函数，实现数据从 Caputre Filter 到 AVI Decompressor 的 传送；然后，在 AVI Decompressor 内部，输入 Pin（在自己的 Receive 函数中） 接收到数据后，Filter 将这块数据进行格式转换，再将转换后的数据放到输出 Pin
的 Sample 中，调用 Null Renderer 的输入 Pin 上的 IMemInputPin::Receive 函数， 从而实现数据从 AVI Decompressor 到 Null Renderer 的传递；Null Render（在输 入 Pin 的 Receive 函数中）接收到数据后进行必要的处理后就返回。至此，数据
 传送的一个轮回就完成了。
拉模式
拉模式最典型的情况发生在 FilterSource（文件源）中，此源管理着数据，但她 没有把数据“推”下去的能力，而要靠后面的 Filter 来“拉” 。Source Filter（图 中显示为 aviDemo.AVI）的输出 Pin 上一定实现了一个 IAsyncReader 接口（当然 此时 AVI Splitter
 也没有必要在输入 Pin 上实现 IMemInputPin 接口） 。 AVI Splitter 的输入 Pin 上一般会有一个“拉”数据的线程，不断调用 SourceFilter 的输出 Pin 上的 IAsyncReader 接口方法来取得数据。 在 AVI Splitter 内部， 将从 Source Filter 中取得的数据进行分析， （音视频）分离，然后分别通过各个输出 Pin 发送出去。 值得注意的是，AVI Splitter 的输出 Pin 往下的数据传送方式，与上述模式是相同 的，即通过调用下一级
 Filter 的输入 Pin 上的 IMemInputPin::Receive 函数。
调用 IAsyncReader 接口方式
从数据传送所使用的接口看来，Pin 之间可能以 IMemInputPin 连接，也可 能以 IAsynReader 连接。另外，还有一种连接方式，即通过 IOverlay 接口。传统 的 Video Render 就支持这种方式（事实上，Video Render 的输入 Pin 上同时实 现了 IMemInputPin
 和 IOverlay 接口，不过这两个接口不能同时使用） 。Capture Filter(VP Pin)->Overlay Mixer2->Video Renderer 这条链路中，Capture Filter 使用 DirectDraw Overlay 方式进行显示，所在在 Overlay Mixer2 与 Video Renderer 之 间并没有实际的数据传输；Video Renderer 此时只相当于一个视频窗口的管理 器。 DirectShow 总是使用专门的线程来传送数据。所以也可以说 DirectShow
 应 用程序至少包含两条线程，一条应用程序主线程以及至少一条数据传送子线程。 既然是多线程程序，就不可避免地会出现线程同步问题。
4.6 Filter 的状态转换
Filter 有三种：停止，暂停和运行。其中，暂停可以理解为数据就绪状态， 是为了快速切换到运行状态而设计的，作为一种中间状态，暂停也是停止与运行 之间切换所毕经的一种状态。 每个 Filter 上都实现了 IBaseFilter 接口， 而 IBaseFilter 是从 IMediaFitler 继承 而来的。Filter
 Graph Manager 正是通过 IMediaFilter 的接口方法来控制 Filter 的 状态转换的。 实际上， FilterGraph Manager 内部的实现， 也是分别调用 FilterGraph 内所有 Filter 上的 IMediaFilter::Run,IMediaFilter::Pause 和 IMediaFilter::Stop 完成 的。 对于大多数 Filter 来说(Source Filter 和 Transform Fitler),暂停和运行两种状 态的区别是可以忽略不计的，它们的任务仅仅是以最快的速度处理完数据，然后
 把数据往下传送。而对 Rendering Filter(Video Renderer)来说，情况要复杂一点。 Source Filter->Transform Filter->Rendering Filter 其中， Source Filter 是一个普通的工作在推模式下的 Filter （暂不讨论 Live Source） 。 当 Filter Graph 从停止状态向暂停状态时，Source Filter 启动数据线程，并调用 Transform Filter 的输入 Pin 上的 IMemInputPin::Receive
 方法，将数据以 Sample 形式往下传送。典型情况下，当 Transform Filter 接收到 Sample 并处理它之后， 会在 Source Filter 的数据线程中将 Sample 继续往下传给 Rendering Filter.当然， Transform Filter 也可以创建自己的工作者线程来继续完成数据的传送。此时， Rendering Filter 已经处于暂停状态，当它接收到第一个 Sample 的时候，就将数 据线程阻塞住。如果是 Video Renderer,它还会将这个 Sample
 帧显示在视频窗口。
当 Filter Graph 从暂停状态转向运行状态时，Rendering Fitler 只需要简单的 将数据线脱离阻塞状态，就可以让整个 Filter Graph 继续运转起来。 Video Renderer 在这种情况下会首先显示在暂停状态下阻塞接收的 Sample 帧，然后每 当接收到新的 Sample 时，将根据
 Sample 上打的时间戳来安排显示。如果 Filter Graph 此时的参考时间还没有到当前接收到的 Sample 上的开始时间，则 Video Renderer 会将当前的 Sample 在输入 Pin 的 Receive 函数中 （或者在另外的接收线 程接收下一个新 Sample 时）阻塞住，直到到了当前 Sample 上的开始时间。 如果 Source Filter 是一个 Live Source（如视频采集卡的 Capture Filter）等， 情况就有所不同了。因为这种源一般是通过实时采集产生的数据的，暂停时阻塞
 了数据传送线程，在此期间采集的数据因为“过时” ，自然就失去了“实时”的 意义。如果从暂停到运行之间等待了很长一段时间，那么把这段时间内采集的数 据都缓存没有意义。其次，在 DierctShow 设计中，Live Source 并不要求在进入 暂 停 状 态 时 就 启 动 数 据 传 送 。 Filter Graph Manager 调 用 Filter 上 的 IMediaFilter::GetState,如果返回 VFW_S_CANT_CUE,就认为该 Filter 已经成功转 入暂停状态，此时，Video
 Renderer 在暂停状态下接收不到任何 Sample. Filter Graph Manager 对 Filter 一个一个实现状态转换的顺序很重要。处理不 好，很容易造成 Sample 的丢失或者 Filter Graph 的死锁。因此，DirectShow 设计 了一种从下往上的”回朔”方法。Filter Graph Manager 总是从 Rendering Filter 开 始执行状态转换处理，沿着连接着的 Filter 链路，最后才处理到 Source Filter.最 典型的状态转换发生在停止和暂停之间：
 停止->暂停 首先从 Rendering Filter 开始进行暂停状态的转换。这时，Filter 调用自己所 有 Pin 上的 Active 函数进行初始化（一般 Pin 在 Active 函数调用中进行 Sample 内存的分配。如果是 Source filter,还将启动数据线程） ，使 Filter 处于一种就绪状 态。Source Filter 是最后一个完成到就绪状态转换的 Filter。当 Source Filter 启动 数据线程后， 就开始往下发送 Sample;而当 Rendering
 Filter 接收到第一个 Sample 后就阻塞住， 真正完成了状态转换， 即都接收到执行暂停后的第一个 Sample （上 文论述的 Live Source 情况除外） ，Filter Graph Manager 才能认为状态转换完成。 暂停->停止 当 Filter 进入停止状态时，会调用自己所有 Pin 上的 Inactive 函数（一般 Pin 在 Inactive 函数调用中进行 Sample 内存的释放。 如果是 Source Filter,还将终止数 据线程） 。各个 Filter 内部的处理还包括：释放所有阻塞住的
 Sample，以使上一 级 Filter 的 IMemAllocator::GetBuffer 调用脱离阻塞；终止所有在 Receive 函数中 的等待，以使上一级 Filter 的 Receive 函数调用返回。另外，Filter 在停止状态下 拒绝接收任何 Sample.就这样从 Rendering Filter 往上一级一级的脱离阻塞。当到 达 Source Filter 的时候，可以确保数据线程的成功停止。 可以看到，Filter 在实现状态转换过程中，要处理一系列的函数调用。因为 DirectShow
 应用程序肯定是多线程程序 （一条应用程序主线程和至少一条数据传 送子线程） ，我们下面来看下 Filter 中的线程同步问题。 在 SDK 基类源代码 CBaseFilter 的定义中，用于标志 Filter 状态的成员变量 的代码如下： FILTER_STATE m_State; 其中，Filter_State 是一个枚举类型，定义如下： Typedef enum_FilterState{
State_Stopped =0,//停止状态 State_Paused =State_Stopped+1,//暂停状态 State_Running =State_Paused+1//运行状态} FILTER_STATE; 还有如下用于同步 Filter 对象状态的同步对象指针的代码： CCritSec *m_pLock;
 然而，在 CBaseFilter 上找不到用于同步数据流线程操作的同步对象，但是 可以从 CTransformFilter 类的定义中找到答案： CCritSec m_csFilter;//Filter 状态对象实例 CCritSec m_csReceive;//数据流线程同步对象实例 CCritSec 是 SDK 提供的 CRITICAL_SECTION 的封装类，可用于线程同步， 另外 SDK 还提供一个类 CAutoLock,能够对 CCritSec 对象在一定的作用域内自动 加锁，解锁： CCritSec
 csMyLock;//未锁定状态 { CAutoLock cObjectLock(&csMyLock);//自动锁定 //执行受保护代码}//超出 cObjectLock 对象的作用域， cObjectLock 对象析构， 自动解锁。 为了防止资源使用冲突，必须使用上述同步对象使各个函数调用“串行化” 。 在应用程序主线程中调用的函数，必须使用 Filter 状态同步对象进行同步；在数 据线程中调用的函数，必须使用数据流同步对象进行同步。两者不能混淆，否则 很容易一起线程死锁。 媒体定位的实现 应用程序可以
 Filter Graph Manager 上获得的 IMediaSeeking 接口， 实现对流 媒体的随机定位(Seeking).实际上，IMediaSeeking 接口的真正实现在 Filter 上。 应用程序通过 Filter Graph Manager 上申请获得 IMediaSeeking 接口执行定位操作 （即调用 IMediaSeeking 的各接口方法）时， Filter Graph Manager 会首先从 Renderer Filter 上请求这个操作的执行。 一般 Renderer Filter
 不能真正执行定位操 作， 因此它将定位操作请求发给上一级 Transfrom Filter 的输出 Pin （通过 Renderer Filter 的输入 Pin 可以获得 Transform Filter 的输出 Pin 的指针，然后在这个输出 Pin 上调用 QueryInterface 得到 IMediaSeeking 接口，再调用对应的接口方法） 。 Transform Filter 如果能完成这个定位操作，则在这个 Filter 内执行后返回；如果 不支持，则继续向上一级 Filter 的输出 Pin
 请求，知道请求到 Source Filter.
实际上，一般在推模式的 Source Filter 的输出 Pin 上，或者拉模式下(Source
是标准的文件源)的 Parser Filter 或 Splitter Filter 的输出 Pin 上才可以真正执行定 位操作。 对于 Filter 开发人员来说， 如果写的是推模式 Source Filter， 一般应该在 Filter 的输出 Pin 上实现 IMediaSeeking 接口；如果写的是 Transform
 Filter,只需要在输 出 Pin 上将用户的定位操作请求向上传递给上一级 Filter 的输出 Pin;而如果写的 是 Renderer Filter,就需要在 Filter 上 (因为 Renderer Filter 没有输出 Pin)将用户的 定位操作请求向上传递给上一级 Filter 的输出 Pin.另外，为了保证定位操作后媒 体流的同步，如果真正实现 IMediaSeeking 各接口方法的 Filter 有多个输出 Pin， 一般仅让其中一个 Pin 支持定位操作。 对于定制的 Transform
 Filter,如果有多个输 出 Pin,就需要自己决定当输出 Pin 接收到定位操作请求时， 选择哪一条路径向上 继续请求。 可 以 借 助 SDK 基 类 源 代 码 CSourceSeeking, CPosPassThru, CRenderer-PossPassThru 等几个类（参见 ctlutil.cpp 文件）实现 Filter 上的媒体 的随机定位。 另外， 写 Transform Filter 时， 如果 Filter 父类采用 CTransfromFilter 或 CtranslnPlaceFilter,那就不用考虑支持
 IMediaSeeking 接口问题，因为基类中 已经实现好了： STDMETHODIMP CTransformOutputPin::NonDelegationQueryInterface(REFIID riid, void **ppv) { CheckPointer(ppv,E_POINTER); ValidateReadWriterPtr(ppv,sizeof(PVOID)); *ppv=NULL; if(riid ==IID_IMediaPosition||riid ==IID_IMediaSeeking){
 ASSERT(m_pTransformFilter->m_pInput!=NULL); if(m_pPosition ==NULL){ //创建能够将定位操作请求直接往上传递的 COM 组件 HRESULT hr=CreatePosPassThru( GetOwner(),FLASE,(IPin *)m_pTransformFilter->m_pInput,&m_pPosition); if(FAILED(hr)){ return hr;}} return m_pPosition->QueryInterface(riid,ppv);}
 else{ return CBaseOutputPin::NonDelegatingQueryInterface(riid,ppv);}} 应用程序能够在任何时候（停止，暂停或者运行）对 Filter Graph 执行定位操 作。但当 Filte graph 正在运行的时候，Filter Graph Manager 会先将 Filter Graph 暂停，执行完定位操作后，再重新运行。 IMediaSeeking 可以支持如下几种格式的定位操作： TIME_FORMAT_FRRME （默认） //按视频帧定位
 TIME_FORMAT_SAMPLE //按 Sample 计数定位 TIME_FORMAT_FIELD //按场定位 TIME_FORMAT_BYTE //按字节流中的偏移定位 TIME_FORMAT_MEDIA_TIME // 按参考时间定位（以 100ms 为单位）
真正实现 IMediaSeeking 接口方法的 Filter 也可以同时支持其他格式，但未 必 支 持 所 有 的 格 式 。 对 于 应 用 开 发 人 员 来 说 ， 当 使 用 除 TIME_FORMAT_MEDIA_TIME 之 外 的 其 他 格 式 时 ， 最 好 先 调 用 IMediaSeeking::IsFormatSupported
 进行确认。 IMediaSeeking 除了可以进行媒体的随机定位外，还可以用来调整媒体文件 的回放速率，即调用接口方法 IMediaSeeking::SetRate, 如参数为 2.0 表示为 2 倍 速播放，参数为 0.5 表示以原来的一般速率播放。理论上，参数为负数可以实现 “倒播” ；但事实上，绝大部分 Filter 不支持这一特性。 当应用程序执行 IMediaSeeking::SetRate 后，Filter Graph Manager 的内部响 应如下： （1） 调用 IMediaSeeking::GetCurrentPoition
 得到当前播放到的位置； （2） 如果当前 Filter Graph 处于暂停或运行状态，则调用 Stop; （3） 调用 IMediaSeeking::SetPositons 将步骤（1）中取得的当前位置重新设置 一下； （4） 调用 IMediaSeeking::SetRate 设置新的回放速率； （5） 如果先前 Filter Graph 的状态为暂停或运行，则恢复这个状态； 相应的，各部分 Filter 上的响应如下： 推模式 Source Filter 或拉模式 Parser/Splitter Filter:这些
 Filter 一般都对输出的 Sample 时间打时间戳， 是真正响应 SetRate 的 Filter.当 Filter Graph 执行定位操作 或者回放速率改变后， 它们一般都会调用输出 Pin 上的 DeliverNewSegment 函数， 提示一个新格式的流的开始；当然，此后输出的 Sample 的时间戳应该根据新的 回放速率来计算。 解码 Filter:这种 Filter 一般不用对 SetRate 作出直接的响应，而是在接收到 NewSegment 调用后，把新的参数保存下来，并继续将 NewSegment
 调用传递给 下一级 Filter. Renderer Filter: Video Renderer 可以忽略回放速率的改变，因为最初发送 Sample 的 Filter 已经根据新的回放速率调整了时间戳。Video Renderer 仍然只须 根据 Sample 上的时间来安排显示时机。但对于 Audio Renderer 来说，一般须要 做一些相关的回放速率调整。 质量控制的实现 数据线程是通过一种“压迫式”方式往下传送数据的。 虽然 Video Renderer 接收到 Sample 后，会根据 Sample
 上的时间戳来正确安排显示时机。 质量控制的实现 数据线程是通过一种“压迫式”方式往下传送数据的。虽然 Video Renderer 接收到 Sample 后，会根据 Sample 上的时间戳来正确安排显示时机。但仅仅这样 是不够的，它不能改善 Filter Graph 运行时的性能。 因此，DirectShow 另外设计了一种“自适应”的反馈机制：质量控制(Quality Control).运用这种机制，当 Video Renderer 发现数据线程发送数据太快时，就能 发送一个消息信息要求 Source
 Filter（拉模式下一般为 Parser Filter 或 Splitter Filter）减慢速度，反之加快速度： DirectShow 使用如下数据结构来描述这个质量控制消息： Typedef struct{ QualityMessageType Type; Long Proportion;
REFERENCE_TIME Late; REFERENCE_TIME TimeStamp; }Quality; 其中，Type 定义了消息的类型，取值为如下枚举类型： Typedef enum{ Famine,//表示当前发送数据的速度太慢 Flood //表示当前发送数据的速度太快 }QualityMessageType;
 Proportion 以 1000 为基数定义了 Video Renderer”建议”的新的数据发送数率。 例如，如果 Proportion 的值为 800，则建议新的发送速率为原速率的 80%；如果 Proportion 的值为 1200，则建议新的发送速率为原速率的 120%。 Late 定义了发送这个消息时， 在 Video Renderer 上的 Sample 实现晚了多少时 间（以 100ns 为单位） 。TimeStamp 定义了发送这个消息时，在 Video Renderer 上的 Sample
 的时间戳的开始时间。 质量控制通过 IQualityControl 接口来实现，接口方法有： Notify 发送质量控制消息 SetSink 设置一个自定义的质量控制处理器 质量控制在 Filter 上具体实现：
3.Notify 2.PassNotify
1.Notify
从图中可以看到，质量控制消息最初由 Video Renderer 发出，经过 Transform Filter 的输出 Pin，询问 Transform Filter 是否能够进行质量的改善；如果不能，则 继续往上传递，直到 Source Filter 的输出 Pin 上。 在 SDK 基类的实现中还可看 到，CBasePin
 上实现 IQualityControl 接口，可以通过 Pin 来实现质量控制消息的 传递，而且默认的实现代码为： STDMETHODIMP CBasePin::Notify(IBaseFilter *pSender,Quality q) { UNREFERENCED_PARAMETER(q); UNREFERENCED_PARAMETER(pSender); DbgBreak(“IQualityControl::Notify not over-ridden from CBasePin.(IGNORE is
 OK)”); return E_NOTIMPL;} 返回 E_NOTIMPL 表示没有实现真正的质量控制处理 （留给派生类去实现） 。 而 CTransformFilter 的输出 Pin 实现代码如下： STDMETHODIMP CTransformOutputPin::Notify(IBaseFilter *pSender,Quality q) { UNREFRENCED_PARAMETER(pSender); ValidateReadPtr(pSender,sizeof(IBaseFilter));
//调用 Filter 上的 AlterQuality 函数，看是否能够执行质量控制 //返回 S_FALSE 表示该 Filter 不能处理质量控制 HRESULT hr=m_pTransformFilter->AlterQuality(q); if(hr!=S_FALSE){ return hr; //either
 S_OK or a failure} //S_FALSE means we pass the message on //Find the quality sink for our input pin and send it there ASSERT(m_pTransformFilter->m_pInput!=NULL); //将质量控制消息往上一级 Filter 传递 return m_pTransformFilter->m_pInput->PassNotify(q);} 可以看到，当 Transform
 Filter 接收到质量控制信息后，首先调用 Filter 的函 数 AlterQuality,询问 Filter 是否可以进行质量的改善（默认实现中，AlterQuality 返回 S_FALSE 表示不进行处理） ，如果不可以，则调用输入 Pin 上的 PassNotify 函数，将质量控制消息传递给上一级 Filter 的输出 Pin. 这是 DirectShow 中一般的质量控制处理过程。另外，应用程序还可以实现自 己的质量控制处理器，然后通过调用 IQualityControl::SetSink
 方法设置给 Video Renderer.当然，这个自定义的质量控制处理器必须实现 IQualityControl 接口，并 且通常是 Filter Graph 中的某一个组件对象。设置成功后，上述处理过程就改变 了，质量消息会直接发送给自定义的处理器。但是，值得注意的是，大多数应用 程序不这么做，因为具体实现时还要考虑很多细节。具体质量控制策略的实现取 决于具体的 Filter 实现，可能是调整发送速度，也可能是丢失部分数据，所以， 在实际应用中还要慎重使用质量控制机制， 特别是对那些对数据有严格要求的应
 用。
第五章 DirectShow 应用开发过程
5.1 开发环境的配置
DirectShow SDK 建议，所有 DirectShow 应用程序都应包含 Dshow.h 文件， 其实 Dshow.h 中没有什么特别定义，只是嵌套包含了其他一些头文件： //Include standard Windows files #include <window.h> #include <windowsx.h>
 #include <olectl.h> #include <ddraw.h> #include <mmsystem.h> //Include DirectShow include files #include <strmif.h>//Generated IDL header file for streams interfaces #include <amvideo.h>//ActiveMovie video interfaces and definitions #include <amudio.h>//ActiveMovie
 audio interfaces and definitions #include <control.h>//generated from control.odl #include <evcode.h>//event code definitions #include <uuids.h>//declaration of type GUIDs and well-known clsids #include <errors.h>//HRESULT status and error definitions
#include <edevdefs.h>//Extrnal device control interface defines #include <audevcod.h>//audio filter device error event codes #include <dvdevcod.h>//DVD error
 event codes 不难发现，上述这些头文件在 streams.h 中包含，在实际应用中，我们也常 常用 streams.h 文件来替换 Dshow.h 文件。 需要连接的库文件 DirectShow SDK 建议，DirectShow 应用程序应至少连接库文件 Strmiids.lib 和 Quartz.lib.前者定义 DirectShow 标准的 CLSID 和 IID,后者定义了导出函数 AMGetErrorText（如果应用程序中没有用到这个函数，也可以不连接这个库） 。 如 果 我 们 写
 程 序 的 时 候 包 含 了 头 文 件 streams.h, 则 一 般 库 文 件 还 要 连 接 strmbasd.lib（Release 版本为 strmbase.lib）,uuid.lib 和 winmm.lib.在 VC 中连接库 文件的方法是： 选择 VC 的菜单项 Project|Settings|Link,在这页中的 Category 选择 General,然后在 Object/library modules 中输入库文件名称。 因为 DirectShow 应用程序是一种 COM 客户程序，因此在调用任何
 COM 函 数之前调用 CoInitialize（或 CoInitializeEx）函数进行 COM 库的初始化（一般是 在应用程序启动的时候调用一次） ，在结束 COM 库使用时调用 CoUninitialize 函 数进行反初始化（一般是在应用程序退出前调用一次） 。
5.2 一般开发过程
第一阶段，创建一个 Filter Graph Manager 组件，代码如下： IGraphBuilder *pGraph =NULL; HRESULT hr =CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_ SERVER,IID_IGraphBuilder,(void**)&pGraph);
 第二阶段，根据实际的应用，创建一条完整的 Filter 链路。比如播放一个本 地文件，最简单快捷的代码如下： hr =pGraph->RenderFile(L"F:\\id-moai-dvdscr.avi",NULL); 第三阶段，调用 Filter Graph Manager 上（或者直接在某个 Filter 上）的各个 接口方法进行控制，并且完成 Filter Graph Manager 与应用程序的事件交互。比 如调用 IMediaControl 接口方法控制 Filter Graph 的状态转换，代码如下：
 IMediaControl *pControl=NULL; hr=pGraph->QueryInterface(IID_IMediaControl,(void**)&pControl); hr=pControl->Run();//运行 Filter Graph 在程序中构建完 Filter 链路后，有时候并不会马上能够正常运行，要是能够 “直观”看到在程序中 Filter 是怎么连接的，实际上，GraphEdit 也提供了这样一 种能够显示其他应用程序创建的 Filter Graph 的功能。 在程序中创建了一个
 Filter Graph Manager 之后， 应该立即将其注册到活动对象表(Running Object Table,ROT) 中，代码如下： HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) { IMoniker *pMoniker; IRunningObjectTable *pROT; if(FAILED(GetRunningObjectTable(0,&pROT))){ return E_FAIL;} WCHAR wsz[256];
wsprintfW(wsz,L"FilterGraph %08x pid %08x",(DWORD_PTR)pUnkGraph, GetCurrentProcessId()); HRESULT hr=CreateItemMoniker(L"!",wsz,&pMoniker); if(SUCCEEDED(hr)){
 hr=pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE,pUnkGrap h,pMoniker,pdwRegister);} pMoniker->Release(); return hr;} 之后，我们就可以在 GraphEdit 中连接，显示这个 Filter Graph 了，方法为： 选择 GraphEdit 的菜单项 File|Connect to remote Graph,在弹出的对话框中选择一 个 Filter Graph 标识项后，我们能看到，相应应用程序的
 Filter Graph 显示在 GraphEdit 的客户区窗口中了。 当 GraphEdit 用于显示其他应用程序的 Filter Graph 时，请尽量不要使用 GraphEdit 提供的功能编辑 Filter 链路（增加或删除 Filter） ， 也不要改变这个 Filter Graph 的运行状态，否则会引起它的应用程序的不可预知 的错误。在我们的应用程序退出前，应该将这个显示我们应用程序创建的 Filter Graph 的 GraphEdit 程序关掉，在程序中析构 Filter Graph Manager
 之前，我们还 必须从 ROT 中注销我们先前的注册，代码如下： void RemoveFromRot(DWORD pdwRegister) { IRunningObjectTable *pROT; if(SUCCEEDED(GetRunningObjectTable(0,&pROT))){ pROT->Revoke(pdwRegister); pROT->Release();} } 在应用程序中，整个过程实现的代码如下： IGraphBuilder *pGraph; DWORD dwRegister: //Create
 the filter graph manager CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGr aphBuilder,(void**)&pGraph); #ifdef _DEBUG Hr=AddToRot(pGraph,&dwRegister); #endif //Reset of the application(not shown) #ifdef _DEBUG RemoveFromRot(dwRegister);
 #endif pGraph->Release();
5.3 通用 Filter Graph 构建技术
构建不同的 Filter 链路，取决于具体的应用。为了简化 Filter Graph 的构建， DirectShow 为几个典型的应用提供了辅助的构建组件，比如创建 DVD 播放的 Filter Graph 时， 可以使用 CLSID_DvdGraphBuilder 组件 （提供 IDvdGraphBuilder
 接口） ；进行音视频采集时，可以使用 CLSID_CaptureGraphBuilder2 组件（提供
ICaputreGraphBuilder2 接口） 。
5.3.1 加入一个指定 CLSID 的 Filter
给定一个 Filter 的 CLSID，我们就可以使用 COM API 函数 CoCreateInstance 来创建它， 并且使用 IFilterGraph::AddFilter 接口方法将其加入到 Filter Graph 中， 代码如下： HRESULT AddFilterByCLSID( IGraphBuilder
 *pGraph,//Pointer to the Filter Graph Manager Const GUID & clsid,//CLSID of the filter to create LPCWSTR wszName,//A name for the filter IBaseFilter **ppF)//Receive a pointer to the filter { if(!pGraph||!ppF) return E_POINTER; *ppF =0; IBaseFilter *Pf=0; HRESULT
 hr=CoCreateInstance(clsid,0,CLSCTX_INPROC_SERVER, IID_IBaseFilter,reinterpret_cast<void**>(&pF)); if(SUCCEEDED(hr){ hr= pGraph->AddFilter(Pf,wszName); if(SUCCEEDED(hr)) *ppF=Pf; else pF->Release(); } return hr; } 我们现在要想 Filter Graph 中加入一个 AVI Mux Filter，代码如下：
 IBaseFilter *pMux; Hr= AddFilterByCLSID(pGraph,CLSID_AviDest,L”AVI Mux”,&pMux); If(SUCCEEDED(hr)){ /*….*/ pMux->Release();} 并不是所有的 Filter 都能通过 CoCreateInstance 函数来创建，大部分注册在 Audio Compressors 目录下的 Filter（它们使用 CLSID_ACMWrapper 包装 Filter）, 大部分注册在 Video Compressors
 目录下的 Filter（它们使用 CLSID_AVICo 包装 Filter）,以及代表硬件(WDM 或 VFW)的包装 Filter,必须通过枚举的方式来创建。
5.3.2 得到 Filter 上的未连接 Pin
当我们在程序中连接 Filter 的时候， 首先是要取得 Filter 上未连接的输入 Pin 或者输出 Pin。方法是，枚举 Filter 上的所有 Pin,调用 IPin::QueryDirection 查看 Pin 的方向，调用 IPin::ConnectedTo 查看 Pin 的连接状态。代码如下： HRESULT
 GetUnconnectedPin( IBaseFilter *pFilter,//Pointer to the filter PIN_DIRECTION PinDir,//Direction of the pin to find
IPin **ppPin) //Receives a pointer to the pin { *ppPin＝0； IEnumPins *pEnum＝0； IPin *pPin＝0； HRESULT hr＝pFilter->EnumPins（&pEnum） ； if（FAILED（hr） ） { return hr；
 } while（pEnum->Next（1，&pPin，NULL）＝＝S_OK) { PIN_DIRECTION ThisPinDir； pPin->QueryDirection（&ThisPinDir） ； if（ThisPinDir＝＝PinDir） { IPin *pTmp＝0； hr＝pPin->ConnectedTo（&pTmp） ； if（SUCCEEDED (hr） ）//Already connected, not the pin we want { Pimp->Release（） ； }
 else //Unconnected，this is the pin we want { pEnum->Release（） ； *ppPin＝pPin； return S_OK； } } pPin->Release（） ； } pEnum->Release（） ； //Did not find a matching pin return E_FAIL； } 比如我们现在要得到一个 Filter 的未连接的输出 Pin，代码如下： IPin *pout = NULL； HRESULT hr＝GetUnconnectedPin（pFilter，PINDIR_OUTPUT，&pout）
 ； if（SUCCEEDED（hr） ） { … Pout->Release（） ； }
5.3.3 连接两个 Filter
两个 Filter 的连接， 总是从上一级 Filter 的输出 Pin 指向下一级 Filter 的输入 Pin 。 Filter Graph Manager 提 供 的 直 接 连 接 两 个 Filter 的 函 数 为 IFilterGraph::ConnectDirect 和 IGraphBuilder::Connect。参考代码如下：
 HRESULT ConnectFilters（ IGraphBuilder *pGraph，//Filter Graph Manager IPin *pOut，//Output pin on the upstream filter IBaseFilter *pDest）//Downstream Filter { if ((pGraph ＝＝NULL)||(pout＝＝NULL)||(pDest＝＝NULL)) { return E_POINTER; } #ifdef debug PIN_DIRECTION
 PinDir； pOut->QueryDirection（&PinDir） ； _ASSERTE（PinDir＝＝PINDIR_OUTPUT） ； #endif //得到下一级 Fi1ter 的输入 Pin IPin *pIn = 0； HRESULT hr＝GetUnconnectedPin（pDest，PINDIR_INPUT，&pIn） ； if (FAILED (hr)) { return hr； } //将输出 Pin 连接到输入 Pin hr＝pGraph->Connect（pOut，pIn）
 ； pIn->Release（） ； return hr； } //不同参数的 ConnectFilters 函数重载形式 HRESULT ConnectFilters（ IGraphBuilder *pGraph， IBaseFilter *pSrc， IBaseFilter *pDest） { if（(pGraph＝＝NULL）|| (pSrc＝＝NULL）|| (pDest＝＝NULL)） { return E_POINTER； } //Find an output pin on the first
 filter IPin *pout＝0； HRESULT hr＝GetUnconnectedPin（pSrc，PINDIR_OUTPUT，&pOut） ； if（FAILED（hr） ） {
return hr； } hr＝ConnectFilters（pGraph，pOut，pDest） ； pout->Release（） ； return hr； } 比如我们现在要加入一个 AVI Mux Filter 和一个 File Writer Filter，然后将它 们相连起。代码如下： IBaseFilter
 *pmux，*pWrite； hr＝AddFilterByCLSID（pGraph，CLSID_AviDest，L“AVI Mux” ， &pMux） ； if(SUCCEEDED(hr)) { hr = AddFilterByCLSID(pGraph ， CLSID_FilterWriter, L “ File Writer ” ， &pWrite)； if(SUCCEEDED(hr)) { hr = ConnectFilters(pGraph, pMux, pWrite); /* Use IFileSinFilter
 to set the file name (not shown). */ pWrite->Release(); } pMux->Release(): }
5.3.4 查找 Filter 或 Pin 上的接口
大部分情况下， 我们使用 Filter Graph Manager 上的接口来操作 Filter Graph。 但有些时候，我们也需要直接获得 Filter 或者它的 Pin 上的某个接口进行参数配 置等。 如果 Filter Graph 中的 Filter 都是我们一个一个加进去的， 而且我们知道自己 需要的接口具体在哪个
 Filter 上实现，我们当然可以直接调用这个 Filter 或它的 Pin 上的 QueryInterface 函数来得到我们想要的接口。 但问题是，如果上述条件不满足，我们该怎么办呢? 于是，我们就要枚举 Graph 中的所有 Filter，或者枚举 Filter 上的所有 Pin，一个一个地找。代码如下： HRESULT FindFilterInterface( IGraphBuilder *pGraph, //Pointer to the Filter Graph Manager REFGUID iid,
 //IID of the interface to retrieve void **ppUnk) //Receives the interface pointer { if(!pGraph || !ppUnk) return E_POINTER; HRESULT hr = E_FAIL; IEnumFilter *pEnum = NULL; IBaseFilter *pF = NULL; if ( FAILED(pGraph->EnumFilters(&pEnum))) { return E_FAIL;
} //分别调用 QueryInterface，枚举 Filter Graph 中的所有 Filter while ( S_OK ＝＝pEnum->Next(1, &pF, 0)) { hr = pF->QueryInterface ( iid, ppUnk); pF->Release(); if(SUCCEEDED(hr))
 { break; } } pEnum ->Release(); return hr; } //查找给定 Filter 的 Pin 上实现的某个接口 HRESULT FindInterface( IBaseFilter *pFilter, //Pointer to the filter to search REFGUID iid, //IID of the interface void **ppUnk) //Receive the interface pointer { if ( !pFilter || !ppUnk)
 return E_POINTER; HRESULT hr = E_FAIL; IEnumPins *pEnum = 0; if(FAILED(pFilter->EnumPins(&pEnum))) { return E_FAIL; } //分别调用 QueryInterface，枚举 Filter 上的所有 Pin IPin *pPin = 0; while ( S_OK == pEnum->Next(1, &pPin, 0)) { hr = pPin->QueryInterface( iid, ppUnk);
 pPin->Release(); if(SUCCEEDED(hr)) { break; } } pEnum->Release(): return hr; } // 综合 FindFilterInterface 和 FindPinInterface 两个函数的功能 HRESULT FindInterfaceAnyWhere( IGraphBuilder *pGraph,
REFGUID iid, void **ppUnk) { if (!pGraph || !ppUnk) return E_POINTER; HRESULT hr = E_FAIL; IEnumFilters *pEnum = 0; if( FAILED(pGraph->EnumFilters(&pEnum))) {
 return E_FAIL; } // Loop through every filter in the graph IBaseFilter *pF = 0; while ( S_OK == pEnum->Next(1, &pF, 0)) { hr = pF->QueryInterface(iid, ppUnk); if(FAILED(hr)) { //the filter does not expose the interface, but maybe one of its pins does hr =
 FindPinInterface(pF, iid, ppUnk); } pF->Release(); if(SUCCEEDED(hr)) { break； } } pEnum->Release()； return hr； } 比如我们现在通过 IGraphBuildecRendefile 函数，构建了一条内含 DV 格式 数据的 AVI 文件的回放链路。我们想要得到其中 DV 视频解码 Filter 上的 IIPDVDec 接口，以设置 DV 解码输出的图像大小为原始图像的四分之一。实现 代码如下： hr＝pGraph->RenderFile（L“C：\\Example.avi”
 ，0） ； if（SUCCEEDED（hr） ） { IIPDVDec *pDvDec； hr = FindFilterInterface（pGraph，IID_IIPDVDec， （void**）&pDvDec） ； if（ SUCCEEDED（hr） ） { pDvDec->put_IPDisplay（DVRESOLUTION_QUARTER） ； pDvDec->Release（） ； } else if（hr＝＝E_NOINTERFACE）
{ //This file does not contain DV video } else { //Some other error occurred } }
5.3.5 遍历 Filter 链路
给定一条 Filter 链路中的某个 Filter， 我们可以往上或往下遍历得到所有的其 他 Filter.方法是： 从本级 Filter 的输入 Pin 或者输出 Pin,可以得到上一级或下一级 Filter 的连接着的 Pin， 再调用这个 Pin 的 lPin::QueryPinlnfo 就可以得到它的拥有
 者 Filter。 重复上述过程， 我们就可以一直遍历到 Source Filter 或者 Renderer Filter. 给定一个 Filter 得到它的往上或往下相连的 Filter 的函数的代码如下： HRESULT GetNextFilter（ IBaseFilter *pFilter，//遍历开始的 Filter PIN_DIRECTION Dir，//遍历方向 IBaseFilter**ppNext）//得到的相邻 Filter { if（!pFilter || !ppNext）return
 E_POINTER; IEnumPins *pEnum＝0； IPin *pPin＝0； HRESULT hr＝pFilter->EnumPins（&pEnum)；//枚举 Filter 上所有 Pin if（FAILED（hr） ）return hr； while（S_OK＝＝pEnum->Next（1，&pPin，0） ） { //See if this pin matches the specified direction PIN_DIRECTION ThisPinDir； hr＝pPin->QueryDirection（&ThisPinDir）
 ： if（FAILED（hr） ） { //Something strange happened． hr＝E_UNEXPECTED； pPin->Release（） ； break； } if（ThisPinDir＝＝Dir） { //Check if the pin is connected to another pin IPin *pPinNext＝0； hr＝pPin->ConnectedTo（&pPinNext） ； if（SUCCEEDED（hr） ） {
//Get the filter that owns that pin PIN_INFO PinInfo； hr＝pPinNext->QueryPinInfo（&PinInfo） ； pPinNext->Release（） ； pPin->Release（） ； pEnum->Release（） ； if（FAILED（hr）||（PinInfo.pFilter＝＝NULL）
 ） { //Something strange happened． return E_UNEXPECTED； } //This is the filter we’re looking for *ppNext＝PinInfo.pFilter；//Client must release return S_OK； } } pPin->Release（） ； } pEnum->Release（） ； //Did not find a matching filter return E_FAIL； }
5.3.6 成批删除 Filter
删除 Filter Graph 中的所有 Filter 的代码如下： //首先停止 Filter Graph 的运行 pControl->Stop（） ； //枚举 Filter Graph 中的所有 Filter IEnumFilters *pEnum＝NULL； HRESULT hr＝pGraph->EnumFilters（&pEnum）
 ； if（SUCCEEDED（hr） ） { IBaseFilter pFilter＝NULL； while（S_OK＝＝pEnum->Next（1，&pFilter，NULL） ） { pGraph->RemoveFilter(pFilter)；//删除 Filter pEnum->Reset（） ；//复位枚举器内部状态 pFilter->Release（） ； } pEnum->Release（） ； } 递归调用，删除指定 Filter 下游的（连接着的）所有 Filter 的代码如下： void
 NukeDownstream（IGraphBuilder *inGraph，IBaseFilter *inFilter） {
if（inGraph&&inFilter） { IEnumPinsb *pinEnum＝0； if（SUCCEEDED（inFilter->EnumPins（&pinEnum） ） ） { pinEnum->Reset（） ； IPin *pin＝0； ULONG cFetched＝0； bool pass＝true；
 //枚举 Filter 上的所有 Pin while （pass&&SUCCEEDED （pinEnum->Next （1， &pin， &cFetched） ） ） { if (pin&&cFetched) { IPin *connectedPin = 0； //判断当前 Pin 是否处于连接状态 Pin->ConnectedTo（&connectedPin） ； if（connectedPin） { PIN_INFO pininfo； if （ SUCCEEDED （ connectedPin->QueryPinInfo
 （&pininfo） ） { //如果当前 Pin 连接着的 Pin 是输入 Pin，则说明我们能够得 //到一个下一级 Filter，我们要递归调用 NukeDownstream if（pininfo．dir＝＝PINDIR INPUT） { //这里递归调用 NukeDownstream（inGraph，pininfo.pFilter） ； //当这一路下游 Filter 全部删除后，可以断开当 //前 Pin 连接 inGraph->Disconnect（connectedPin） ； inGraph->Disconnect(pin)；
 //将操作完成的下一级 Filter 删除 inGraph->RemoveFilter(pininfo.pFilter); } Pininfo.pFilter->Release(); } connectedPin->Release(); } pin->Release(); } else {
pass = false; } } pinEnum->Release(); } } } 递归调用，删除指定 Filter 上游的（连接着的）所有 Filter 的代码如下： void NukeUpstream(IGraphBuilder *inGraph, IBaseFilter *inFilter) { if(inGraph
 &&inFilter) { IEnumPins *pinEnum = 0; if(SUCCEEDED(inFilter->EnumPins(&pinEnum))) { pinEnum->Reset(): IPin *pin = 0; ULONG cFetched = 0; bool pass = true; //枚举 Filter 上的所有 Pin while(pass && SUCCEEDED(pinEnum->Next(1, &pin, &cFetched))) { if(pin && cFetched)
 { IPin *connectedPin = 0; //判断当前 Pin 是否处于连接状态 Pin->ConnectedTo(&connectedPin); if(connectedPin) { PIN_INFO pininfo; if(SUCCEEDED(connectedPin->QueryPinInfo(&pininfo))) { //如果当前 Pin 连接着的 Pin 是输出 Pin，则说明我们能够 //得到一个上一级 Filter，我们要递归调用 NukeUpstream if(pininfo.dir
 == PINDIR_OUTPUT) { //递归调用 NukeUpstream（inGraph，pininfo.pFilter） ； //当这一路上游 Filter 全部删除后， 可以断开当前 Pin //连接 inGraph->Disconnect（connectedPin） ； inGraph->Disconnect（pin） ； //将操作完成的上一级 Filter 删除 inGraph->RemoveFilter（pininfo.pFilter） ；
} pininfo.pFilter->Release（） ； } connectedPin->Release（） ； } pin->Release（） ； } else { pass＝false； } } pinEnum->Release（） ； } } } 第六章 视频采集 6.1 绑定采集设备为 Source
 Filter 视频程序要运行在不同的计算机，不同的操作系统上，但目标系统的硬件配 置情况是不确定的， 应用程序如何来使用这些不可预知的采集设备呢？答案是使 用系统枚举。 在 DirectShow 框架下，参与工作的基本单元是 Filter.采集设备也不例外。 DirectShow 使用特殊的包装 Filter 对它们进行包装—WDM 驱动模型的采集设备 使用 WDM Video Capture Filter（实现文件为 kswdmcap.ax） ，VFM 驱动模型的采 集设备使用 VFW Capture Filter（实现文件为
 qcap.dll） 。只要采集设备正确安装， DirectShow 就能把它包装成一个 Filter，并且在一定的类型目录下注册。换句话 说，应用程序只要枚举特定的类型目录，就能在知道系统中安装有多少个，以及 何种类型的采集设备。 DirectShow 定义了很多类型目录供不同的 Filter 注册。音频采集设备注册在 Audio Capture Sources 目录下，视频采集设备注册在 Video Capture Sources 目录 下。而在上述两个目录下注册的采集设备一般会在 WDM Streaming
 Capture Devices 目录下注册。有时候，WDM Streaming Capture Devices 还会包含一些既 不在 Audio Capture Devices 目录下也不在 Video Capture Sources 下注册的采集设 备。类型目录都用一个 CLSID 来表示，表 6.1 给出了音视频采集设备 Filter 注册 的类型目录及其 CLSID 的对应关系。 表 6.1 音视频采集设备 Filter 的注册目录
类型目录名
Audio Capture Sources Video Capture Sources WDM Streaming Capture Devices
类型目录的 CLSID
CLSID_AudioInputDeviceCategory CLSID_VideoInputDeviceCategory AM_KSCATEGORY_CAPTURE
类型目录的 MERIT
MERIT_DO_NOT_USE MERIT_DO_NOT_USE MERIT_DO_NOT_USE
DirectShow 提供了一个专门的系统枚举组件(CLSID_SystemDeviceEnum)。 枚举的大致过程如下： （1） 使 用 CoCreateInstance 函 数 创 建 一 个 系 统 枚 举 组 件 对 象 ， 并 获 得
ICreateDevEnum 接口。 （2） 使用接口方法 ICreateDevEnum::CreateClassEnumerator 为指定的类型目录 创建一个枚举器，并获得 IEnumMoniker 接口。 （3） 使用接口方法 IEnumMoniker::Next 枚举指定类型目录下所有的设备标志 (Device
 Moniker)。每个设备标志对象上都实现了 IMoniker 接口。 （4） 调用 IMoniker::BindToStorage 之后就可以访问设备标志的属性集， 比如得 到设备的显示名字(Display Name),友好名字(Friendly Name)等。 （5） 调用 IMoniker::BindToObject 可以将设备标志绑定成一个 DirectShow Filter，随后调用 IFilterGraph::AddFilter 加入到 Filter Graph 中就可以参与 工作了。 下面给出参考的函数：
 IBaseFilter* CreateDevice(BOOL inIsVideo, const char *inDisplayName, char *outFriendlyName) { GUIDguid=inIsVideo?CLSID_VideoInputDeviceCategory: CLSID_AudioInputDeviceCategory; return CreateHardwareFilter2(guid,inDisplayName,outFriendlyName); } IBaseFilter*
 CreateHardwareFilter2(GUID inCategory, const char *inDisplayName, char *outFriendlyName) { //创建一个系统枚举组件 ICreateDevEnum *enumHardware =NULL; HRESULT hr= CoCreateInstance( CLSID_SystemDeviceEnum, NULL, CLSCTX_ALL, IID_ICreateDevEnum, (void**)&enumHardware);
 if(hr){return NULL;} IBaseFilter *hardwareFilter =NULL; IEnumMoniker *enumMoniker =NULL;//为目录创建一个枚举器 hr=enumHardware->CreateClassEnumerator(inCategory,&enumMoniker,0); if(enumMoniker){ enumMoniker->Reset(); ULONG fetched=0; IMoniker *moniker=NULL; char displayName[1024];//枚举得到目录下所有硬件设备，逐个进行匹配
 while(!hardwareFilter&&SUCCEEDED(fetched&&enumMoniker->Next(1,&monike r,&fetched))){ if(moniker){ WCHAR* wzDisplayName=NULL; //获得设备显示名字 hr=moniker->GetDisplayName(NULL,NULL,&wzDisplayName); if(SUCCEEDED(hr)){
WideCharToMultiByte(CP_ACP,0,wzDisplayName,-1,displayName,1024,"", NULL); CoTaskMemFree(wzDisplayName); //判断显示名字是否匹配 if(IsSameDevice(displayName,inDisplayName)){
 moniker->BindToObject(0,0,IID_IBaseFilter,(void**)&hardwareFilter);}} //如果成功创建 Filter,则获得该 Filter 的友好名字 if(outFriendlyName&&hardwareFilter){ IPropertyBag* propertyBag=NULL; outFriendlyName[0]=0; if(SUCCEEDED(moniker->BindToStorage(0,0,IID_IPropertyBag,(void*
 *)&propertyBag))){ VARIANT name; name.vt=VT_BSTR; if(SUCCEEDED(propertyBag->Read(L"FriendlyName",&name ,NULL))){ WideCharToMultiByte(CP_ACP,0,name.bstrVal,-1,outFriendlyN ame,1024,NULL,NULL);} propertyBag->Release();}} moniker->Release();}} enumHardware->Release();}
 return hardwareFilter; } 6.2 构建 Filter Graph 采集应用的 Filter Graph 一般比较复杂；而直接使用 Filter Graph Manager 上的 IGraphBuilder 接口构建这种 Filter Graph，有时候难度又很大。为此，DirectShow 特
别提供了一个辅助组件 Capture Graph Builder，来简化这种 Filter Graph 的创建。 跟其他 DirectShow 应用程序一样， 在构建实际的 Filter Graph 之前， 肯定是要创建 Filter Graph Manager 组件的。另外，我们还可以创建一个 Capture Graph
 Builder 组件，对用这个 组件上的 ICaptureGraphBuilder2 接口来完成 Filter Graph 的构建，如图 6.2 所示：
图 6.2 创建 Capture Graph Builder 组件 成功创建 Capture Graph Builder 组件之后，我们还必须对这个组件进行初始化，即调 用接口方法 ICaptureGraphBuilder2::SetFilter(pGraph)将 Filter Graph Manager 对象指针设置给
 它。Capture Graph Builder 组件进行初始化的代码如下： HRESULT InitCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2
**ppBuild) { if(!ppGraph||!ppBuild){ return E_POINTER;} IGraphBuilder *pGraph =NULL; ICaptureGraphBuilder2 *pBuild=NULL; //创建 ICaptureGraphBuilder 组件 HRESULThr=CoCreateInstance(CLSID_CaptureGraphBuilder2,
 NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild); if(SUCCEEDED(hr)){ //创建 Filter Graph Builder hr=CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph); if(SUCCEEDED(hr)){//Initialize the Capture
 Graph Builder pBuild->SetFiltergraph(pGraph); //返回两个指针 *ppBuild=pBuild; *ppGraph=pGraph;//调用完后需释放 return S_OK; } else {pBuild->Release();}} return hr; } ▼使用 Caputre Graph Builder 组件构建 Filter Graph 的好处有： ▼使用 ICaptureGraphBuilder2::RenderStream 能够对指定类型的 Pin
 完成后续连接（比如 PIN_CATEGORY_PREVIEW 指定 Preview Pin,PIN_CATEGORY_CAPTURE 指定 Capture Pin 等） ； ▼智能构建 Preview Pin 链路（指定 PIN_CATEGORY_PREVIEW 类型调用 RenderStream） ， 即当采集卡 Filter 含有 VP Pin 时， 连接 Overlay Mixer2|Video Render;含有 Preview Pin 时， 连 接 Video Renderer;而只有 Capture
 Pin 时自动插入一个 Smart Tee Filter 进行分路后， 再连接到 Video Renderer; ▼自动加入采集卡 Filter 之前的辅助 Filter,如用于电视接收的 TV Tuner Filter,用于控制电视 声音的 TV Audio Filter 和用于选择视频输入端子的 Analog Video Crossbar Filter 等，并完成 这些 Filter 之间的连接（这些 Filter 之间的连接不是通过媒体类型来标识的，而是通过一种 媒介(Medium)的 GUID 来标志的，它们之间在
 Filter Graph 中也不会有实际的数据流动） ； ▼ICaptureGraphBuilder2::FindPin 提供了方便的寻找 Pin 的方法； ▼ICaptureGraphBuilder2::FindInterface 提供了方便的寻找特定接口的方法； 使用 ICaputreGraphBuilder2::ControlStream 可以单独地控制某个 Pin(Preview Pin 或 Capture Pin);
▼自动处理 EC_REPAINT 事件（如果在采集写文件的时，我们同时使用传统的 Video Renderer 进行预览， 当视频窗口别别的窗口覆盖然后恢复时， Video Renderer 会向 Filter Graph Manager 发送一个 EC_REPAINT 事件要求重画视频窗口。Filter Graph
 Manager 的默认处理 是，对 Filter Graph 执行暂停操作，这将引起写到文件的内容异常。Caputre Graph Builder 会 自 动 帮 我 们 处 理 这 个 问 题 ， 即 内 部 调 用 了 IMediaEvent::CancelDefaultHandling(EC_REPAINT). ICaptureGraphBuilder2 *pBuild; //Caputre Graph Builder //Initialize pBuilder (not shown) IBaseFilter
 *pCap; //Video capture filter /*Initialize pCap and add it to the filter graph (not shown)*/ Hr=pBuild->RenderStream(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Video,pCap,NULL, NULL); 比如，想要同时构建一个采集写文件的 Filter 链路，代码如下： IBaseFilter *pMux; Hr=pBuild->SetOutputFileName(
 &MEDIASUBTYPE_Avi,//Specifies AVI for the target file L”C:\\Example.avi”, //File name &pMux, //Receives a pointer to the mux NULL); //(Optional) Receive a pointer to the file sink Hr=pBuild->RenderStream( &PIN_CATEGORY_CAPTURE,//Pin category &MEDIATYPE_Video,
 //Media type pCap, //Capture filter NULL, //Intermediate filter (optional) pMux); //Mux or file sink filter //Release the mux filter pMux->Release(); 最终构建的 Filter Graph 如图 6.3 所示:
图 6.3 采集写文件同时预览的 Filter Graph 6.3 添加 Filter 到 FilterGraph 前面我们已经提到根据指定的 CLSID 添加 Filter 到 Filter Graph 的普遍方法，但是并不 是所有的 Filter 都可以通过这个方法创建，我们提到使用枚举的方法添加 Filter
 的方法，过 程与枚举系统硬件设备的过程相似，这里只给出程序，希望大家好好体会： IBaseFilter* ObtainFilterByFriendlyName(GUID inCategory, const char *inFriendlyName) { HRESULT hr; IBaseFilter *pFilter; CComBSTR FilterName(inFriendlyName);
ICreateDevEnum *pSysDevEnum = NULL; hr = CoCreateInstance(CLSID_SystemDeviceEnum, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void **)&pSysDevEnum); IEnumMoniker
 *pEnumCat = NULL; hr = pSysDevEnum->CreateClassEnumerator(inCategory, &pEnumCat, 0); if (hr == S_OK) { // Enumerate the monikers. IMoniker *pMoniker; ULONG cFetched; while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) { IPropertyBag *pPropBag; pMoniker->BindToStorage(0,
 0, IID_IPropertyBag, (void **)&pPropBag); VARIANT varName; VariantInit(&varName); hr = pPropBag->Read(L"FriendlyName", &varName, 0); if ( (SUCCEEDED(hr) && wcscmp(FilterName, varName.bstrVal) == 0)) { pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pFilter);
 return pFilter; } VariantClear(&varName); pPropBag->Release(); pMoniker->Release(); } pEnumCat->Release(); } pSysDevEnum->Release(); return ERROR; }
NULL,
