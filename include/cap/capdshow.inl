#ifndef _CAPDSHOW_INL_
#define _CAPDSHOW_INL_

#include "cap.h"
#include "std/stddef_c.h"
#include "std/img_c.h"

#pragma comment(lib,"Strmiids.lib")
#define COBJMACROS
#define CINTERFACE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define HMONITOR_DECLARED
//#include <qedit.h>
#include <dshow.h>
//#include "opencv2/core/core_c.h"

#define pmalloc malloc
#define pfree free

DEFINE_GUID(MEDIASUBTYPE_GREY, 0x59455247, 0x0000, 0x0010, 0x80, 0x00,
	0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y8, 0x20203859, 0x0000, 0x0010, 0x80, 0x00,
	0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y800, 0x30303859, 0x0000, 0x0010, 0x80, 0x00,
	0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y16, 0x20363159, 0x0000, 0x0010, 0x80, 0x00,
	0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71);
DEFINE_GUID(MEDIASUBTYPE_BY8, 0x20385942, 0x0000, 0x0010, 0x80, 0x00,
	0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

DEFINE_GUID(CLSID_CaptureGraphBuilder2, 0xbf87b6e1, 0x8c27, 0x11d0, 0xb3, 0xf0, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5);
DEFINE_GUID(CLSID_FilterGraph, 0xe436ebb3, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(CLSID_NullRenderer, 0xc1f400a4, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);
DEFINE_GUID(CLSID_SampleGrabber, 0xc1f400a0, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);
DEFINE_GUID(CLSID_SystemDeviceEnum, 0x62be5d10, 0x60eb, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(CLSID_VideoInputDeviceCategory, 0x860bb310, 0x5d01, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(FORMAT_VideoInfo, 0x05589f80, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a);
DEFINE_GUID(IID_IAMAnalogVideoDecoder, 0xc6e13350, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IAMCameraControl, 0xc6e13370, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IAMCrossbar, 0xc6e13380, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IAMStreamConfig, 0xc6e13340, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IAMVideoProcAmp, 0xc6e13360, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IBaseFilter, 0x56a86895, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_ICaptureGraphBuilder2, 0x93e5a4e0, 0x2d50, 0x11d2, 0xab, 0xfa, 0x00, 0xa0, 0xc9, 0xc6, 0xe3, 0x8d);
DEFINE_GUID(IID_ICreateDevEnum, 0x29840822, 0x5b84, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(IID_IGraphBuilder, 0x56a868a9, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_IMPEG2PIDMap, 0xafb6c2a1, 0x2c41, 0x11d3, 0x8a, 0x60, 0x00, 0x00, 0xf8, 0x1e, 0x0e, 0x4a);
DEFINE_GUID(IID_IMediaControl, 0x56a868b1, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_IMediaEventEx, 0x56a868c0, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_IMediaFilter, 0x56a86899, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_ISampleGrabber, 0x6b652fff, 0x11fe, 0x4fce, 0x92, 0xad, 0x02, 0x66, 0xb5, 0xd7, 0xc7, 0x8f);
DEFINE_GUID(LOOK_UPSTREAM_ONLY, 0xac798be0, 0x98e3, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5);
DEFINE_GUID(MEDIASUBTYPE_AYUV, 0x56555941, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_IYUV, 0x56555949, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_RGB24, 0xe436eb7d, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(MEDIASUBTYPE_RGB32, 0xe436eb7e, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(MEDIASUBTYPE_RGB555, 0xe436eb7c, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(MEDIASUBTYPE_RGB565, 0xe436eb7b, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(MEDIASUBTYPE_I420, 0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_UYVY, 0x59565955, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y211, 0x31313259, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y411, 0x31313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y41P, 0x50313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YUY2, 0x32595559, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YUYV, 0x56595559, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YV12, 0x32315659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YVU9, 0x39555659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YVYU, 0x55595659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_MJPG, 0x47504A4D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); // MGB
DEFINE_GUID(MEDIATYPE_Interleaved, 0x73766169, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIATYPE_Video, 0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(PIN_CATEGORY_CAPTURE, 0xfb6c4281, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba);
DEFINE_GUID(PIN_CATEGORY_PREVIEW, 0xfb6c4282, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba);

interface ISampleGrabberCB : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SampleCB(
		double SampleTime,
		IMediaSample *pSample) = 0;

	virtual HRESULT STDMETHODCALLTYPE BufferCB(
		double SampleTime,
		BYTE *pBuffer,
		LONG BufferLen) = 0;
};

interface ISampleGrabber : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetOneShot(
		BOOL OneShot) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetMediaType(
		const AM_MEDIA_TYPE *pType) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(
		AM_MEDIA_TYPE *pType) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(
		BOOL BufferThem) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(
		LONG *pBufferSize,
		LONG *pBuffer) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(
		IMediaSample **ppSample) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetCallback(
		ISampleGrabberCB *pCallback,
		LONG WhichMethodToCallback) = 0;
};



typedef struct capdshow {
  bool m_bConnected;
  int m_nWidth;
  int m_nHeight;
  bool m_bLock;
  bool m_bChanged;
  long m_nBufferSize;
  IGraphBuilder* m_pGraph;
  IBaseFilter* m_pDeviceFilter;
  IMediaControl* m_pMediaControl;
  IBaseFilter* m_pSampleGrabberFilter;
  ISampleGrabber* m_pSampleGrabber;
  IPin* m_pGrabberInput;
  IPin* m_pGrabberOutput;
  IPin* m_pCameraOutput;
  IMediaEvent* m_pMediaEvent;
  IBaseFilter* m_pNullFilter;
  IPin* m_pNullInputPin;
} capdshow;
#undef IEnumPins_Next
#undef IEnumPins_Reset
#undef IPropertyBag_Read
#undef IUnknown_Release
#if defined(__cplusplus) && !defined(CINTERFACE)
#define IEnumPins_Reset(p)  (p)->Reset()
#define IEnumPins_Next(This,cPins,ppPins,pcFetched)	(This)->Next(cPins,ppPins,pcFetched)
#else
#define IEnumPins_Reset(p)  (p)->lpVtbl->Reset(p)
#define IEnumPins_Next(This,cPins,ppPins,pcFetched)	(This)->lpVtbl -> Next(This,cPins,ppPins,pcFetched)
#endif
#if defined(__cplusplus)
#define REF(x)  (x)
#define IPropertyBag_Read(p, pszPropName, pVar, pErrorLog) p->Read(pszPropName, pVar, pErrorLog)
#define IEnumMoniker_Reset(p)  (p)->Reset()
#define IEnumMoniker_Release(p)  (p)->Release()
#define IPropertyBag_Release(p)  (p)->Release()
#define IMoniker_Release(p)  (p)->Release()
#define IUnknown_Release(p)  (p)->Release()
#define IEnumMoniker_Next(p, celt,rgelt,pceltFetched) (p)->Next(celt,rgelt,pceltFetched)
#define IMoniker_BindToStorage(p,pbc,pmkToLeft,riid,ppvObj)  p->BindToStorage(pbc,pmkToLeft,riid,ppvObj)
#define IMoniker_BindToObject( p,pbc,pmkToLeft,riidResult,ppvResult)  p->BindToObject(pbc,pmkToLeft,riidResult,ppvResult)
//#define ISpecifyPropertyPages_GetPages(p, a) p->GetPages(a)
//#define ISpecifyPropertyPages_Release(p) p->Release()
#define ISampleGrabber_GetCurrentBuffer(p, a, b)  p->GetCurrentBuffer(a, b)
#define ISampleGrabber_SetMediaType(p, a)  p->SetMediaType(a)
#define ISampleGrabber_Release(p) p->Release()
#define ISampleGrabber_SetBufferSamples(p, a)  p->SetBufferSamples(a)
#define ISampleGrabber_SetOneShot(p, a)   p->SetOneShot(a)
#define ISampleGrabber_GetConnectedMediaType(p, a)  p->GetConnectedMediaType(a)

#else
#define REF(x)  &(x)
#define IPropertyBag_Read(p, pszPropName, pVar, pErrorLog) p->lpVtbl->Read(p, pszPropName, pVar, pErrorLog)
//#define IEnumMoniker_Reset(p)  (p)->lpVtbl->Reset(p)
//#define IEnumMoniker_Release(p)  (p)->lpVtbl->Release(p)
//#define IPropertyBag_Release(p)  (p)->lpVtbl->Release(p)
//#define IMoniker_Release(p)  (p)->lpVtbl->Release(p)
#define IUnknown_Release(p)  (p)->lpVtbl->Release(p)
//#define IEnumMoniker_Next(p, celt,rgelt,pceltFetched) (p)->lpVtbl->Next(p, celt,rgelt,pceltFetched)
//#define IMoniker_BindToStorage(p,pbc,pmkToLeft,riid,ppvObj)  p->lpVtbl->BindToStorage(p,pbc,pmkToLeft,riid,ppvObj)
//#define IMoniker_BindToObject( This,pbc,pmkToLeft,riidResult,ppvResult)  This->lpVtbl->BindToObject(This,pbc,pmkToLeft,riidResult,ppvResult)
#endif
#define MYFREEMEDIATYPE(mt) {if ((mt).cbFormat != 0) \
{CoTaskMemFree((PVOID)(mt).pbFormat);  (mt).cbFormat = 0;  (mt).pbFormat = NULL;  } \
if ((mt).pUnk != NULL) {  IUnknown_Release((mt).pUnk);  (mt).pUnk = NULL;  }}


bool capdshow_BindFilter(capdshow* s, int wIndex, IBaseFilter** pFilter)
{
  if (wIndex >= 0) {
    // enumerate all video capture devices
    ICreateDevEnum* pCreateDevEnum = NULL;
    HRESULT hr = CoCreateInstance(REF(CLSID_SystemDeviceEnum), NULL, CLSCTX_INPROC_SERVER,
      REF(IID_ICreateDevEnum), (void**)&pCreateDevEnum);
    if (hr == NOERROR) {
      IEnumMoniker* pEm = NULL;
      hr = ICreateDevEnum_CreateClassEnumerator(pCreateDevEnum, REF(CLSID_VideoInputDeviceCategory), &pEm, 0);
      if (hr == NOERROR) {
        ULONG cFetched;
        IMoniker* pM = NULL;
        int index = 0;
        IEnumMoniker_Reset(pEm);
        while (hr = IEnumMoniker_Next(pEm, 1, &pM, &cFetched), hr == S_OK, index <= wIndex) {
          IPropertyBag* pBag;
          hr = IMoniker_BindToStorage(pM, 0, 0, REF(IID_IPropertyBag), (void**)&pBag);
          if (SUCCEEDED(hr)) {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = IPropertyBag_Read(pBag, L"FriendlyName", &var, NULL);
            if (hr == NOERROR) {
              if (index == wIndex) {
                IMoniker_BindToObject(pM, 0, 0, REF(IID_IBaseFilter), (void**)pFilter);
              }
              SysFreeString(var.bstrVal);
            }
            IPropertyBag_Release(pBag);
          }
          IMoniker_Release(pM);
          index++;
        }
        IEnumMoniker_Release(pEm);
        pEm = NULL;
      }
      pCreateDevEnum->lpVtbl->Release(pCreateDevEnum);
      pCreateDevEnum = NULL;
    }
  }
  return true;
}

//将输入crossbar变成PhysConn_Video_Composite
void capdshow_SetCrossBar(capdshow* s)
{
  int i;
  IAMCrossbar* pXBar1 = NULL;
  ICaptureGraphBuilder2* pBuilder = NULL;
  HRESULT hr = CoCreateInstance(REF(CLSID_CaptureGraphBuilder2), NULL, CLSCTX_INPROC_SERVER, REF(IID_ICaptureGraphBuilder2), (void**)&pBuilder);
  if (SUCCEEDED(hr)) {
    hr = ICaptureGraphBuilder2_SetFiltergraph(pBuilder, s->m_pGraph);
  }
  hr = ICaptureGraphBuilder2_FindInterface(pBuilder, &LOOK_UPSTREAM_ONLY, NULL,
      s->m_pDeviceFilter, REF(IID_IAMCrossbar), (void**)&pXBar1);
  if (SUCCEEDED(hr)) {
    long OutputPinCount;
    long InputPinCount;
    long PinIndexRelated;
    long PhysicalType;
    long inPort = 0;
    long outPort = 0;
    IAMCrossbar_get_PinCounts(pXBar1, &OutputPinCount, &InputPinCount);
    for (i = 0; i < InputPinCount; i++) {
      IAMCrossbar_get_CrossbarPinInfo(pXBar1, TRUE, i, &PinIndexRelated, &PhysicalType);
      if (PhysConn_Video_Composite == PhysicalType) {
        inPort = i;
        break;
      }
    }
    for (i = 0; i < OutputPinCount; i++) {
      IAMCrossbar_get_CrossbarPinInfo(pXBar1, FALSE, i, &PinIndexRelated, &PhysicalType);
      if (PhysConn_Video_VideoDecoder == PhysicalType) {
        outPort = i;
        break;
      }
    }
    if (S_OK == IAMCrossbar_CanRoute(pXBar1, outPort, inPort)) {
      IAMCrossbar_Route(pXBar1, outPort, inPort);
    }
    IAMCrossbar_Release(pXBar1);
  }
  ICaptureGraphBuilder2_Release(pBuilder);
}

//返回摄像头的数目
//可以不用创建capdshow实例，采用int c=capdshow_dshow_capnum();得到结果。
int capdshow_capnum()
{
  int count = 0;
  // enumerate all video capture devices
  ICreateDevEnum* pCreateDevEnum = NULL;
  HRESULT hr;
  IEnumMoniker* pEm = NULL;
  CoInitialize(NULL);
  hr = CoCreateInstance(REF(CLSID_SystemDeviceEnum), NULL, CLSCTX_INPROC_SERVER, REF(IID_ICreateDevEnum), (void**)&pCreateDevEnum);
  hr = ICreateDevEnum_CreateClassEnumerator(pCreateDevEnum, REF(CLSID_VideoInputDeviceCategory), &pEm, 0);
  if (hr == NOERROR) {
    ULONG cFetched;
    IMoniker* pM = NULL;
    IEnumMoniker_Reset(pEm);
    //pEm->lpVtbl->Reset(pEm);
    while (hr = IEnumMoniker_Next(pEm, 1, &pM, &cFetched), hr == S_OK) {
      count++;
    }
    IEnumMoniker_Release(pEm);
    pEm = NULL;
  }
  ICreateDevEnum_Release(pCreateDevEnum);
  pCreateDevEnum = NULL;
  return count;
}
//根据摄像头的编号返回摄像头的名字
//wIndex: 摄像头编号
//sName: 用于存放摄像头名字的数组
//nBufferSize: sName的大小
//可以不用创建capdshow实例，采用capdshow_CameraName();得到结果。
int CameraName(int wIndex, char* sName, int nBufferSize)
{
  int count = 0;
  // enumerate all video capture devices
  ICreateDevEnum* pCreateDevEnum = NULL;
  HRESULT hr;
  CoInitialize(NULL);
  hr = CoCreateInstance(REF(CLSID_SystemDeviceEnum), NULL, CLSCTX_INPROC_SERVER, REF(IID_ICreateDevEnum), (void**)&pCreateDevEnum);
  if (hr == NOERROR) {
    IEnumMoniker* pEm = NULL;
    hr = ICreateDevEnum_CreateClassEnumerator(pCreateDevEnum, REF(CLSID_VideoInputDeviceCategory), &pEm, 0);
    if (hr == NOERROR) {
      ULONG cFetched;
      IMoniker* pM;
      IEnumMoniker_Reset(pEm);
      while (hr = IEnumMoniker_Next(pEm, 1, &pM, &cFetched), hr == S_OK) {
        if (count == wIndex) {
          IPropertyBag* pBag = 0;
          hr = IMoniker_BindToStorage(pM, 0, 0, REF(IID_IPropertyBag), (void**)&pBag);
          if (SUCCEEDED(hr)) {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = IPropertyBag_Read(pBag, L"FriendlyName", &var, NULL); //还有其他属性,像描述信息等等...
            if (hr == NOERROR) {
              //获取设备名称
              WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, sName, nBufferSize , "", NULL);
              SysFreeString(var.bstrVal);
            }
            IPropertyBag_Release(pBag);
          }
          IMoniker_Release(pM);
          break;
        }
        count++;
      }
    }
    IEnumMoniker_Release(pEm);
    pEm = NULL;
  }
  ICreateDevEnum_Release(pCreateDevEnum);
  pCreateDevEnum = NULL;
  return 1;
}


//抓取一帧，返回的IplImage不可手动释放
//返回图像数据的为RGB模式的Top-down(第一个字节为左上角像素)，即IplImage::origin=0(IPL_ORIGIN_TL)
//The returned image can not be released.
int capdshow_getframe(capdev* s0, img_t* im, int flag)
{
  capdshow* s = (capdshow*)(s0->x);
  long evCode;
  long size = 0;
  IMediaControl_Run(s->m_pMediaControl);
  IMediaEvent_WaitForCompletion(s->m_pMediaEvent, INFINITE, &evCode);
  //IMediaEvent_WaitForCompletion(s->m_pMediaEvent, INFINITE, &evCode);
  ISampleGrabber_GetCurrentBuffer(s->m_pSampleGrabber, &size, NULL);
  
  //if the buffer size changed
  if (size != s->m_nBufferSize) {
    s->m_nBufferSize = size;
    imsetsize(im, s->m_nHeight, s->m_nWidth, 3, 1);
  }
  ISampleGrabber_GetCurrentBuffer(s->m_pSampleGrabber, &s->m_nBufferSize, (long*)im->tt.data);
  matflip(im->h, im->s, im->tt.data, im->s);
  //imswap_rb(im);
  return 1;
}
//关闭摄像头，析构函数会自动调用这个函数
int capdshow_close(capdev* s0)
{
  capdshow* s = (capdshow*)(s0->x);
  if (s->m_bConnected) {
    IMediaControl_Stop(s->m_pMediaControl);
  }
  IGraphBuilder_Release(s->m_pGraph);
  IBaseFilter_Release(s->m_pDeviceFilter);
  IMediaControl_Release(s->m_pMediaControl);
  IBaseFilter_Release(s->m_pSampleGrabberFilter);
  ISampleGrabber_Release(s->m_pSampleGrabber);
  IPin_Release(s->m_pGrabberInput);
  IPin_Release(s->m_pGrabberOutput);
  IPin_Release(s->m_pCameraOutput);
  IMediaEvent_Release(s->m_pMediaEvent);
  IBaseFilter_Release(s->m_pNullFilter);
  IPin_Release(s->m_pNullInputPin);
  
  s->m_pGraph = NULL;
  s->m_pDeviceFilter = NULL;
  s->m_pMediaControl = NULL;
  s->m_pSampleGrabberFilter = NULL;
  s->m_pSampleGrabber = NULL;
  s->m_pGrabberInput = NULL;
  s->m_pGrabberOutput = NULL;
  s->m_pCameraOutput = NULL;
  s->m_pMediaEvent = NULL;
  s->m_pNullFilter = NULL;
  s->m_pNullInputPin = NULL;
  s->m_bConnected = false;
  s->m_nWidth = 0;
  s->m_nHeight = 0;
  s->m_bLock = false;
  s->m_bChanged = false;
  s->m_nBufferSize = 0;
  if (s) {
    pfree(s);
    s0->x = NULL;
  }
  return 0;
}

//打开摄像头，wIndex指定打开哪个摄像头，取值可以为0,1,2,...
//bDisplayProperties指示是否自动弹出摄像头属性页
//width和height设置的摄像头的宽和高，如果摄像头不支持所设定的宽度和高度，则返回false
int capdshow_open(capdev* s0, int wIndex, int width, int height)
{
  capdshow* s = NULL;
  bool bDisplayProperties = 0;
  HRESULT hr = S_OK;
  int devcnt = capdshow_capnum();
  AM_MEDIA_TYPE mt;
  IEnumPins* pEnum = NULL;

  wIndex = MAX(wIndex, 0);
  if (NULL==s0 || wIndex >= devcnt) {
    return wIndex;
  }
  s = (capdshow*)pmalloc(sizeof(capdshow));
  memset(s, 0, sizeof(capdshow));
  CoInitialize(NULL);

  // Create the Filter Graph Manager.
  hr = CoCreateInstance(REF(CLSID_FilterGraph), NULL, CLSCTX_INPROC, REF(IID_IGraphBuilder), (void**)&s->m_pGraph);
  hr = CoCreateInstance(REF(CLSID_SampleGrabber), NULL, CLSCTX_INPROC_SERVER, REF(IID_IBaseFilter), (LPVOID*)&s->m_pSampleGrabberFilter);
  hr = IGraphBuilder_QueryInterface(s->m_pGraph, REF(IID_IMediaControl), (void**) &s->m_pMediaControl);
  hr = IGraphBuilder_QueryInterface(s->m_pGraph, REF(IID_IMediaEvent), (void**) &s->m_pMediaEvent);
  hr = CoCreateInstance(REF(CLSID_NullRenderer), NULL, CLSCTX_INPROC_SERVER, REF(IID_IBaseFilter), (LPVOID*) &s->m_pNullFilter);
  hr = IGraphBuilder_AddFilter(s->m_pGraph, s->m_pNullFilter, L"NullRenderer");
  hr = IBaseFilter_QueryInterface(s->m_pSampleGrabberFilter, REF(IID_ISampleGrabber), (void**)&s->m_pSampleGrabber);
  ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
  mt.majortype = MEDIATYPE_Video;
  mt.subtype = MEDIASUBTYPE_RGB24;
  mt.formattype = FORMAT_VideoInfo;
  hr = ISampleGrabber_SetMediaType(s->m_pSampleGrabber, &mt);
  MYFREEMEDIATYPE(mt);
  IGraphBuilder_AddFilter(s->m_pGraph, s->m_pSampleGrabberFilter, L"Grabber");
  // Bind Device Filter. We know the device because the id was passed in
  capdshow_BindFilter(s, wIndex, &s->m_pDeviceFilter);
  IGraphBuilder_AddFilter(s->m_pGraph, s->m_pDeviceFilter, NULL);
  IBaseFilter_EnumPins(s->m_pDeviceFilter, &pEnum);
  hr = IEnumPins_Reset(pEnum);
  hr = IEnumPins_Next(pEnum, 1, &s->m_pCameraOutput, NULL);
  pEnum = NULL;
  IBaseFilter_EnumPins(s->m_pSampleGrabberFilter, &pEnum);
  IEnumPins_Reset(pEnum);
  hr = IEnumPins_Next(pEnum, 1, &s->m_pGrabberInput, NULL);
  pEnum = NULL;
  IBaseFilter_EnumPins(s->m_pSampleGrabberFilter, &pEnum);
  IEnumPins_Reset(pEnum);
  IEnumPins_Skip(pEnum, 1);
  hr = IEnumPins_Next(pEnum, 1, &s->m_pGrabberOutput, NULL);
  pEnum = NULL;
  IBaseFilter_EnumPins(s->m_pNullFilter, &pEnum);
  IEnumPins_Reset(pEnum);
  hr = IEnumPins_Next(pEnum, 1, &s->m_pNullInputPin, NULL);
  //SetCrossBar();
  if (bDisplayProperties) {
    ISpecifyPropertyPages* pPages = NULL;
    HRESULT hr = IPin_QueryInterface(s->m_pCameraOutput, REF(IID_ISpecifyPropertyPages), (void**)&pPages);
    if (SUCCEEDED(hr)) {
      CAUUID caGUID;
      PIN_INFO PinInfo;
      IPin_QueryPinInfo(s->m_pCameraOutput, &PinInfo);
      ISpecifyPropertyPages_GetPages(pPages, &caGUID);
      OleCreatePropertyFrame(NULL, 0, 0, L"Property Sheet", 1, (IUnknown**) & (s->m_pCameraOutput), caGUID.cElems, caGUID.pElems, 0, 0, NULL);
      CoTaskMemFree(caGUID.pElems);
      IBaseFilter_Release(PinInfo.pFilter);
    }
    else {
      printf("FAILED: QueryInterface\n");
    }
    ISpecifyPropertyPages_Release(pPages);
    pPages = NULL;
  }
  else {
    // 加入由 lWidth和lHeight设置的摄像头的宽和高 的功能，默认320*240
    // by flymanbox @2009-01-24
    IAMStreamConfig* iconfig = NULL;
    AM_MEDIA_TYPE* pmt = NULL;
    VIDEOINFOHEADER* phead = NULL;
    hr = IPin_QueryInterface(s->m_pCameraOutput, REF(IID_IAMStreamConfig), (void**)&iconfig);
    if (IAMStreamConfig_GetFormat(iconfig, &pmt) == S_OK) {
      //printf("GetFormat Failed ! \n");
      int wIndex1 = wIndex;
      if (0==memcmp(&pmt->formattype, &FORMAT_VideoInfo, sizeof(FORMAT_VideoInfo))) {
        phead = (VIDEOINFOHEADER*)pmt->pbFormat;
        phead->bmiHeader.biWidth = width;
        phead->bmiHeader.biHeight = height;
        if ((hr = IAMStreamConfig_SetFormat(iconfig, pmt)) != S_OK) {
          //printf("FAILED: SetFormat\n");
          wIndex1 = -1;
        }
      }
      if (-1==wIndex1) {
        int iCount = 0, iSize = 0, iFormat;
        int s1, ss = INT_MAX, ifmt, height1 = height, width1 = width;
        VIDEO_STREAM_CONFIG_CAPS scc;
        //VIDEOINFOHEADER *pVih;
        BITMAPINFOHEADER *pBih = NULL;
        hr = IAMStreamConfig_GetNumberOfCapabilities(iconfig, &iCount, &iSize);
        if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
          for (iFormat=0; iFormat<iCount; iFormat++) {
            hr = IAMStreamConfig_GetStreamCaps(iconfig, iFormat, &pmt, (BYTE* )&scc);
            if (phead->bmiHeader.biWidth>0) {
              printf("%d %d\n", phead->bmiHeader.biWidth, phead->bmiHeader.biHeight);
            }
            if (SUCCEEDED(hr)) {
              phead = (VIDEOINFOHEADER*)pmt->pbFormat;
              s1 = (1+abs(phead->bmiHeader.biWidth - width1)) * (abs(phead->bmiHeader.biHeight - height1)+1);
              if (s1<ss) {
                ss = s1;
                ifmt = iFormat;
                width = phead->bmiHeader.biWidth;
                height = phead->bmiHeader.biHeight;
              }
            }
          }
        }
        hr = IAMStreamConfig_GetStreamCaps(iconfig, ifmt, &pmt, (BYTE* )&scc);
        if (SUCCEEDED(hr)) {
          phead = (VIDEOINFOHEADER*)pmt->pbFormat;
          phead->bmiHeader.biWidth = width;
          phead->bmiHeader.biHeight = height;
          if ((hr = IAMStreamConfig_SetFormat(iconfig, pmt)) != S_OK) {
            //printf("FAILED: SetFormat\n");
            wIndex = -1;
          }
        }
      }
    }
    IAMStreamConfig_Release(iconfig);
    iconfig = NULL;
    MYFREEMEDIATYPE(*pmt);
  }
  if (wIndex>=0) {
    hr = IGraphBuilder_Connect(s->m_pGraph, s->m_pCameraOutput, s->m_pGrabberInput);
    hr = IGraphBuilder_Connect(s->m_pGraph, s->m_pGrabberOutput, s->m_pNullInputPin);
    if (FAILED(hr)) {
      switch (hr) {
      case VFW_S_NOPREVIEWPIN :
        break;
      case E_FAIL :
        break;
      case E_INVALIDARG :
        break;
      case E_POINTER :
        break;
      }
      printf("FAILED: Connect\n");
    }
    ISampleGrabber_SetBufferSamples(s->m_pSampleGrabber, TRUE);
    ISampleGrabber_SetOneShot(s->m_pSampleGrabber, TRUE);
    hr = ISampleGrabber_GetConnectedMediaType(s->m_pSampleGrabber, &mt);
    if (FAILED(hr)) {
      printf("FAILED: GetConnectedMediaType\n");
      wIndex = -1;
      pfree(s);
    } else {
      VIDEOINFOHEADER* videoHeader;
      videoHeader = (VIDEOINFOHEADER*)(mt.pbFormat);
      s->m_nWidth = videoHeader->bmiHeader.biWidth;
      s->m_nHeight = videoHeader->bmiHeader.biHeight;
      s->m_bConnected = true;
      s0->x = (void*)s;
      s0->getframe = capdshow_getframe;
      s0->close = capdshow_close;
    }
  } else {
    pfree(s);
  }
  IEnumPins_Release(pEnum);
  pEnum = NULL;
  return wIndex+1;
}

#if 0
#endif

#endif // _CAPDSHOW_INL_

