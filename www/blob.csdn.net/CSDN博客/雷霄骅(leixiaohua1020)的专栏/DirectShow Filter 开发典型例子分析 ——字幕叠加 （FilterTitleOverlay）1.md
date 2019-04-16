# DirectShow Filter 开发典型例子分析 ——字幕叠加 （FilterTitleOverlay）1 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月09日 12:47:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32
个人分类：[DirectShow](https://blog.csdn.net/leixiaohua1020/article/category/1645657)










本文分析一下《DirectShow开发指南》中的一个典型的Transform Filter的例子：字幕叠加（FilterTitleOverlay）。通过分析该例子，我们可以学习到DirectShowTransform Filter 开发的方式。

直接打开项目工程（我这里是VC2010），看到项目的结构如下图所示：

![](https://img-blog.csdn.net/20131009115022265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



先看一下运行的结果：

注意，DirectShow的Filter是不可以直接运行进行调试的。一般情况下需要借助于Graphedit.exe这个程序进行调试。当然这不是绝对的，也可以用graph-studio-next这样的开源程序。



选择右键点击工程->属性->调试->命令。在栏中输入Graphedit.exe的路径，如图所示

![](https://img-blog.csdn.net/20131009120225812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这样就可以调试Filter了。

拖入一个文件"五月天 咸鱼.mp4"，然后插入本工程的Filter，如图所示。

![](https://img-blog.csdn.net/20131009120707109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

播放视频，效果如图，可见左上角显示出 "Hello, DirectShow!" 的字样。

![](https://img-blog.csdn.net/20131009120859421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

看完了结果，就要开始分析代码了~

回顾一下工程结构图：

![](https://img-blog.csdn.net/20131009115022265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

先看一下CFilterTitleOverlay.h（已经在重要的地方加了注释）：



```cpp
//
// CFilterTitleOverlay.h
//

#ifndef __H_CFilterTitleOverlay__
#define __H_CFilterTitleOverlay__

#include "ITitleOverlay.h"
#include "COverlayController.h"
#include "OverlayDefs.h"

class CFilterTitleOverlay : public CTransInPlaceFilter
						  , public ISpecifyPropertyPages
						  , public ITitleOverlay
{
private:
	OVERLAY_TYPE            mOverlayType;
	COverlayController *    mOverlayController;

	CCritSec                mITitleOverlaySync;
	BOOL                    mNeedEstimateFrameRate;

private:
	CFilterTitleOverlay(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
	~CFilterTitleOverlay();

	HRESULT SetInputVideoInfoToController(void);
	void ReleaseOverlayController(void);
	void SideEffectOverlayTypeChanged(void);

public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	//说明必须重写NonDelegatingQueryInterface
	DECLARE_IUNKNOWN;
	// Basic COM - used here to reveal our own interfaces
	//暴露接口，使外部程序可以QueryInterface，关键！
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	// check if you can support mtIn
    virtual HRESULT CheckInputType(const CMediaType* mtIn); // PURE
	//必须重写的核心函数
	virtual HRESULT Transform(IMediaSample *pSample); // PURE

	// Delegating methods
	virtual HRESULT CompleteConnect(PIN_DIRECTION direction, IPin *pReceivePin);
	virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();

	// --- ISpecifyPropertyPages ---
	STDMETHODIMP GetPages(CAUUID *pPages);

	// --- ITitleOverlay methods ---
	//都是接口函数
	STDMETHODIMP put_TitleOverlayType(long inOverlayType);
	STDMETHODIMP get_TitleOverlayType(long * outOverlayType);
	STDMETHODIMP put_TitleOverlayStyle(int inUsingCover);
	STDMETHODIMP get_TitleOverlayStyle(int * outUsingCover);
	STDMETHODIMP put_Title(const char * inTitle, int inLength);
	STDMETHODIMP get_Title(char * outBuffer, int * outLength);
	STDMETHODIMP put_TitleColor(BYTE inR, BYTE inG, BYTE inB);
	STDMETHODIMP get_TitleColor(BYTE * outR, BYTE * outG, BYTE * outB);
	STDMETHODIMP put_TitleStartPosition(POINT inStartPos);
	STDMETHODIMP get_TitleStartPosition(POINT * outStartPos);
	STDMETHODIMP put_TitleFont(LOGFONT inFont);
	STDMETHODIMP get_TitleFont(LOGFONT * outFont);
	STDMETHODIMP put_TitleDuration(double inStart, double inEnd);
	STDMETHODIMP get_TitleDuration(double * outStart, double * outEnd);
};

#endif // __H_CFilterTitleOverlay__
```





CFilterTitleOverlay继承了CTransInPlaceFilter，意味着Transform()函数输入和输出的数据位于同一块内存中。

以下几个函数是必须有的：

CreateInstance()：创建Filter
 NonDelegatingQueryInterface()：暴露接口，使外部程序可以QueryInterface
 CheckInputType()：检查输入类型
 Transform()：核心处理函数（字幕叠加）

另外还包含了ITitleOverlay中的函数put_TitleOverlayType()等等一大堆。

下面看一下CFilterTitleOverlay.cpp吧，先列出注册信息部分：

```cpp
//唯一标识符
// {E3FB4BFE-8E5C-4aec-8162-7DA55BE486A1}
DEFINE_GUID(CLSID_HQTitleOverlay, 
0xe3fb4bfe, 0x8e5c, 0x4aec, 0x81, 0x62, 0x7d, 0xa5, 0x5b, 0xe4, 0x86, 0xa1);

// {E70FE57A-19AA-4a4c-B39A-408D49D73851}
DEFINE_GUID(CLSID_HQTitleOverlayProp, 
0xe70fe57a, 0x19aa, 0x4a4c, 0xb3, 0x9a, 0x40, 0x8d, 0x49, 0xd7, 0x38, 0x51);


//
// setup data
//
//注册时候的信息
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_NULL,            // Major type
    &MEDIASUBTYPE_NULL          // Minor type
};
//注册时候的信息
const AMOVIESETUP_PIN psudPins[] =
{
    {
        L"Input",           // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes },     // The pin details
      { L"Output",          // String pin name
        FALSE,              // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input",           // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    }
};

//注册时候的信息
const AMOVIESETUP_FILTER sudFilter =
{
    &CLSID_HQTitleOverlay,       // Filter CLSID
    L"HQ Title Overlay Std.",    // Filter name
    MERIT_DO_NOT_USE,        // Its merit
    2,                       // Number of pins
    psudPins                 // Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance
//注意g_Templates名称是固定的
CFactoryTemplate g_Templates[] = 
{
    { 
		L"HQ Title Overlay Std.",
		&CLSID_HQTitleOverlay,
		CFilterTitleOverlay::CreateInstance,
		NULL,
		&sudFilter 
	},
	{ 
		L"HQ Title Overlay Property Page",
		&CLSID_HQTitleOverlayProp,
		CTitleOverlayProp::CreateInstance 
	}
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);
```


 这一部分并不属于CFilterTitleOverlay这个类。主要是DirectShow Filter的一些注册信息。其结构是非常固定的。



再来看看CFilterTitleOverlay中函数实现部分（只列了几个函数，不然内容太多= =）：

CreateInstance()：



```cpp
//
// CreateInstance
//
// Override CClassFactory method.
// Provide the way for COM to create a CNullInPlace object
//
//创建
CUnknown * WINAPI CFilterTitleOverlay::CreateInstance(LPUNKNOWN punk, HRESULT *phr) 
{
#if 1
	//防伪？？！！
	char    szCreatorPath[256], szCreatorName[256];
	::strcpy(szCreatorPath, "");
	::strcpy(szCreatorName, "");
	HMODULE hModule = ::GetModuleHandle(NULL);
	::GetModuleFileName(hModule, szCreatorPath, 256);
	char * backSlash = ::strrchr(szCreatorPath, '\\');
	if (backSlash)
	{
		strcpy(szCreatorName, backSlash);
	}
	::_strlwr(szCreatorName);
	// Please specify your app name with lowercase
	// 检查调用该Filter的程序
	// 一开始调试不了，就卡在这了 = =
	if (::strstr(szCreatorName, "graphedit") == NULL &&
		::strstr(szCreatorName, "ourapp") == NULL)
	{
		*phr = E_FAIL;
		return NULL;
	}
#endif
	//通过New对象的方法
	CFilterTitleOverlay *pNewObject = new CFilterTitleOverlay(NAME("TitleOverlay"), punk, phr);
	if (pNewObject == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
	return pNewObject;
}
```


 NonDelegatingQueryInterface()：





```cpp
//
// Basic COM - used here to reveal our own interfaces
//暴露接口，使外部程序可以QueryInterface，关键！
STDMETHODIMP CFilterTitleOverlay::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	CheckPointer(ppv, E_POINTER);
	//根据不同的REFIID，获得不同的接口指针
	if (riid == IID_ISpecifyPropertyPages) 
	{
		return GetInterface((ISpecifyPropertyPages *) this, ppv);
	}
	else if (riid == IID_ITitleOverlay)
	{
		return GetInterface((ITitleOverlay *) this, ppv);
	}
	else 
	{
		//不是以上的REFIID的话，调用父类的
		return CTransInPlaceFilter::NonDelegatingQueryInterface(riid, ppv);
	}
} // NonDelegatingQueryInterface
```


 CheckInputType()：





```cpp
// Only RGB 32/24/565/555 supported 
HRESULT CFilterTitleOverlay::CheckInputType(const CMediaType* mtIn)
{
	// Dynamic format change will never be allowed!
	if (IsStopped() && *mtIn->Type() == MEDIATYPE_Video)
	{
		if (*mtIn->Subtype() == MEDIASUBTYPE_RGB32 ||
			*mtIn->Subtype() == MEDIASUBTYPE_RGB24 ||
			*mtIn->Subtype() == MEDIASUBTYPE_RGB555 ||
			*mtIn->Subtype() == MEDIASUBTYPE_RGB565)
		{
			return NOERROR;
		}
	}
	return E_INVALIDARG;
}
```


 Transform()：





```cpp
HRESULT CFilterTitleOverlay::Transform(IMediaSample *pSample)
{
	// If we cann't read frame rate info from input pin's connection media type,
	// We estimate it from the first sample's time stamp!
	if (mNeedEstimateFrameRate)
	{
		mNeedEstimateFrameRate = FALSE;
		REFERENCE_TIME  startTime = 0;
		REFERENCE_TIME  endTime   = 0;
		double          estimated = 25;
		if (SUCCEEDED(pSample->GetTime(&startTime, &endTime)))
		{
			estimated = 1.0 * UNITS / (endTime - startTime);
		}
		mOverlayController->SetEstimatedFrameRate(estimated);
	}

	if (mOverlayType != OT_NONE)
	{
		//PBYTE是unsigned char
		PBYTE  pData = NULL;
		//获取IMediaSample中的数据
		pSample->GetPointer(&pData);
		//叠加
		mOverlayController->DoTitleOverlay(pData);
	}

	return NOERROR;
}
```


 下面列出实现ITitleOverlay接口的函数的实现，就列了一个。





```cpp
STDMETHODIMP CFilterTitleOverlay::get_Title(char * outBuffer, int * outLength)
{
	CAutoLock   lockit(&mITitleOverlaySync);
	*outLength = mOverlayController->GetTitle(outBuffer);
	return NOERROR;
}
```



暂且分析到这里。

书上提供的代码有误，这是经过修改后，添加了注释的代码：

[http://download.csdn.net/detail/leixiaohua1020/6371819](http://download.csdn.net/detail/leixiaohua1020/6371819)






