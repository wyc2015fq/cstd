# 定义自己的Transform Filter - yixianfeng41的专栏 - CSDN博客
2015年08月10日 21:33:46[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1358
## 1、选择基类
       Filter其实本质来说还是一个COM组件，创建Filter时，我们根据需要继承某个基类，它就拥有了COM属性了。CBaseFilter可以作为我们的基类，但是在实际运用中，我们一般不总是选择CBaseFilter。针对具体类型的Filter，我们会选择特定的基类。比如传输Filter,我们就一般选择CTransformFilter或者CTransInplaceFilter作为基类。如果Filter仅有一个输入pin和一个输出pin,而且一进一出的媒体类型相同，则一般采用CTransInplaceFilter作为基类；如果媒体类型不一样，在传输过程中要进行处理，则一般选择CTransformFilter作为基类。
## 2、基类使用
CTransformFilter类的使用方法（派生一个Filter子类至少要重写以下函数）：
1) CTransformFilter::CheckInputType(const CMediaType *mtIn); 
2) CTransformFilter::CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut); 
3) CTransformFilter::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp); 
4) CTransformFilter::GetMediaType(int iPosition, CMediaType *pMediaType); 
5) CTransformFilter::Transform(IMediaSample *pIn, IMediaSample *pOut);
CTransInplaceFilter类的使用方法（派生一个Filter子类至少要重写以下函数）：
1) CTransInplaceFilter::CheckInputType(const CMediaType *mtIn); 
2) CTransInplaceFilter::TransForm(IMediaSample *pSample);
## 3、开发Filter（以截图FIlter为例）
1）选择基类。因为这个Filter只需要得到访问Sample的机会。所以可以从CTransInplaceFilter类继承。
2）定义接口(不是必须的，有需要就定义接口）
    接口是Filter与外部调用者之间的“桥梁”。外部调用者可以通过接口得到Filter的一些属性、状态等参数，也可以直接控制Filter的行为。在截图Filter中，我们需要定义一些与截图有关的接口，供外部调用。
```cpp
//----------------------------------------------------------------------------
// IImageGrabber
//----------------------------------------------------------------------------
DECLARE_INTERFACE_(IImageGrabber, IUnknown)
{
	STDMETHOD(get_ImageSize) (THIS_
		long * outWidth, long * outHeight, long * outBitCount   
	) PURE;
	STDMETHOD(get_FrameSize) (THIS_
		long * outFrameSize
	) PURE;
	STDMETHOD(get_BitmapInfoHeader) (THIS_
		BITMAPINFOHEADER * outBitmapInfo
	) PURE;
	STDMETHOD(get_Is16BitsRGB) (THIS_
		long * outIsRGB565
	) PURE;
	STDMETHOD(put_IsFieldPicture) (THIS_
		BOOL inIsField
	) PURE;
	STDMETHOD(Snapshot) (THIS_ 
		BYTE * outBuffer, BOOL inIsSyncMode 
	) PURE;
	STDMETHOD(IsAsyncSnapshotFinished) (THIS_
		BOOL * outFinished
	) PURE;
	STDMETHOD(CancelAllPending) (THIS) PURE;
};
```
3）为FIlter和接口各生成一个GUID.
VS:工具->创建GUID,然后就生成了一个唯一的GUID.
```cpp
// {B00AD7BD-31B3-4fe1-805B-5CD7D10B2532}
DEFINE_GUID(IID_IImageGrabber, 
0xb00ad7bd, 0x31b3, 0x4fe1, 0x80, 0x5b, 0x5c, 0xd7, 0xd1, 0xb, 0x25, 0x32);
// {0CF4972F-E0CD-4af2-917C-CD219A0277CF}
DEFINE_GUID(CLSID_HQImageGrabber, 
0xcf4972f, 0xe0cd, 0x4af2, 0x91, 0x7c, 0xcd, 0x21, 0x9a, 0x2, 0x77, 0xcf);
```
3）构造类，继承自CTransInplaceFilter，并且还要继承定义的接口
```cpp
#include "IImageGrabber.h"
class CFilterImageGrabber : public CTransInPlaceFilter
	                    , public IImageGrabber
{
private:
	CFilterImageGrabber(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
	~CFilterImageGrabber();
	CCritSec    mSafeCapture;
	BYTE *      mFrameBuffer;
	BOOL        mIsFieldPicture;
	long        mImageWidth;
	long        mImageHeight;
	long        mImageSize;
public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	DECLARE_IUNKNOWN;
	// Basic COM - used here to reveal our own interfaces
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);  //暴露接口
	virtual HRESULT CheckInputType(const CMediaType* mtIn); // PURE必须实现
	virtual HRESULT Transform(IMediaSample *pSample); // PURE必须实现
	HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);
	HRESULT StartStreaming(void);
	void VerifyFieldPicture(BYTE * pImage);
	// --- IImageGrabber methods ---接口的方法，也必须实现
	STDMETHODIMP get_ImageSize(long * outWidth, long * outHeight, long * outBitCount);
	STDMETHODIMP get_FrameSize(long * outFrameSize);
	STDMETHODIMP get_BitmapInfoHeader(BITMAPINFOHEADER * outBitmapInfo);
	STDMETHODIMP get_Is16BitsRGB(long * outIsRGB565);
	STDMETHODIMP put_IsFieldPicture(BOOL inIsField);
	STDMETHODIMP Snapshot(BYTE * outBuffer, BOOL inIsSyncMode);
	STDMETHODIMP IsAsyncSnapshotFinished(BOOL * outFinished);
	STDMETHODIMP CancelAllPending();
};
```
4）在CPP文件中，首先完成一些模板类东西，关于注册以及导出的
```cpp
// ----------------------------------------------------------------------------
//            注册信息
// ----------------------------------------------------------------------------
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_NULL,            // Major type
    &MEDIASUBTYPE_NULL          // Minor type
};
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
const AMOVIESETUP_FILTER sudFilter =
{
    &CLSID_HQImageGrabber,   // Filter CLSID
    L"Image Grabber",     // Filter name
    MERIT_DO_NOT_USE,        // Its merit
    2,                       // Number of pins
    psudPins                 // Pin details
};
// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance
CFactoryTemplate g_Templates[] = 
{
    { 
		L"Image Grabber",
		&CLSID_HQImageGrabber,
		CFilterImageGrabber::CreateInstance,
		NULL,
		&sudFilter 
	}
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);
```
定义一个同名的.def文件，加入到工程，内容为：
```cpp
LIBRARY		FileImageGrabber.dll
EXPORTS
            DllMain                 PRIVATE
            DllGetClassObject       PRIVATE
            DllCanUnloadNow         PRIVATE
            DllRegisterServer       PRIVATE
            DllUnregisterServer     PRIVATE
```
在CPP文件中定义DLL的入口函数及注册 ，一般放在CPP文件最后
```cpp
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}
//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  dwReason, 
                      LPVOID lpReserved)
{
    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
```
5）接着在CPP中完成头文件中的方法的定义（就几个重要的说一下）
```cpp
//
// CreateInstance 创建实例
//
CUnknown * WINAPI CFilterImageGrabber::CreateInstance(LPUNKNOWN punk, HRESULT *phr) 
{
    CFilterImageGrabber *pNewObject = new CFilterImageGrabber(NAME("Grabber"), punk, phr);
    if (pNewObject == NULL) 
	{
        *phr = E_OUTOFMEMORY;
    }
    return pNewObject;
}
```
```cpp
//
// Basic COM - 暴露接口
STDMETHODIMP CFilterImageGrabber::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	CheckPointer(ppv, E_POINTER);
	if (riid == IID_IImageGrabber)
	{
		return GetInterface((IImageGrabber *) this, ppv);
	}
	else 
	{
		return CTransInPlaceFilter::NonDelegatingQueryInterface(riid, ppv);
	}
}
```
```cpp
//将Sample保存下来
HRESULT CFilterImageGrabber::Transform(IMediaSample *pSample)
{
	if (mFrameBuffer)
	{	
		BYTE * pData;
		pSample->GetPointer(&pData);
		{
			CAutoLock lck(&mSafeCapture);
			CopyMemory(mFrameBuffer, pData, mImageSize);
			VerifyFieldPicture(mFrameBuffer);
			mFrameBuffer = NULL;
		}
	}
	return NOERROR;
}
```
以及关于截图的自定义方法..........
6）注册Filter
至此，Filter就开发好了，生成解决方案后就会生成相应的DLL,注册这个Filter.
![](https://img-blog.csdn.net/20150810211925385)
## 4、使用Filter
我主要说一下如何使用我们定义的接口。
1）第一步（或者直接建一个文件将我们Filter的接口定义复制过来，不要忘了CLSID)
```cpp
const CLSID IID_IImageGrabber={0xb00ad7bd, 0x31b3, 0x4fe1, 0x80, 0x5b, 0x5c, 0xd7, 0xd1, 0xb, 0x25, 0x32};
MIDL_INTERFACE("B00AD7BD-31B3-4fe1-805B-5CD7D10B2532")
IImageGrabber:public IUnknown
{
    virtual void STDMETHODCALLTYPE get_ImageSize(long * outWidth, long * outHeight, long * outBitCount)=0;
    virtual void STDMETHODCALLTYPE get_FrameSize(long * outFrameSize)=0;
    virtual void STDMETHODCALLTYPE get_BitmapInfoHeader(BITMAPINFOHEADER * outBitmapInfo)=0;
    virtual void STDMETHODCALLTYPE get_Is16BitsRGB(long * outIsRGB565)=0;
    virtual void STDMETHODCALLTYPE put_IsFieldPicture(BOOL inIsField)=0;
    virtual void STDMETHODCALLTYPE Snapshot(BYTE * outBuffer, BOOL inIsSyncMode)=0;
    virtual void STDMETHODCALLTYPE IsAsyncSnapshotFinished(BOOL * outFinished)=0;
    virtual void STDMETHODCALLTYPE CancelAllPending()=0;
};
```
2）获得Filter中的接口
```cpp
IImageGrabber *mIGrabber;
```
```cpp
HRESULT hr = mFilter->QueryInterface(IID_IImageGrabber, 
          (void**) &mIGrabber); //这个Filter就是截图Filter
```
3）接下就可以用mIGrabber调用接口方法了

﻿﻿
﻿﻿
