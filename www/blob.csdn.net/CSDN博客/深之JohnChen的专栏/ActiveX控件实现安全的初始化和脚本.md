# ActiveX控件实现安全的初始化和脚本 - 深之JohnChen的专栏 - CSDN博客

2012年06月21日 16:59:23[byxdaz](https://me.csdn.net/byxdaz)阅读数：2449


ActiveX控件打包成cab后,在脚本中调用中时,要保证控件的安全性才能在你的网页上安全运行,有两种方法来实现这一保证:实现一个名称为IObjectSafe的接口到你的控件。如果IE发现你的控件支持IObjectSafety，它调用 IObjectSafety::SetInterfaceSafetyOptions 方法然后才载入你的控件。另外一种方法需要修改注册表。

# [ActiveX控件安全初始化之一:实现ISafeObject接口](http://www.cnblogs.com/carekee/articles/1772201.html)

ActiveX控件打包成cab后,在脚本中调用中时,要保证控件的安全性才能在你的网页上安全运行,有两种方法来实现这一保证:实现一个名称为IObjectSafe的接口到你的控件。如果IE发现你的控件支持IObjectSafety，它调用 IObjectSafety::SetInterfaceSafetyOptions 方法然后才载入你的控件。

1。创建了一个叫做“tryISafeObject.ocx”的MFC ActiveX控件。

2。在tryISafeObjectCtrl.h中定义ISafeObject接口：

#include <objsafe.h> // for IObjectSafety; in ActiveX SDK

class CtryISafeObjectCtrl : public COleControl
{

 DECLARE_DYNCREATE(CtryISafeObjectCtrl)
//........................................................................
//ISafeObject
 DECLARE_INTERFACE_MAP()

 BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)
  STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) ( 
            /* [in] */ REFIID riid,
            /* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
            /* [out] */ DWORD __RPC_FAR *pdwEnabledOptions
  );

        STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) ( 
            /* [in] */ REFIID riid,
            /* [in] */ DWORD dwOptionSetMask,
            /* [in] */ DWORD dwEnabledOptions
  );
 END_INTERFACE_PART(ObjSafe);

//ISafeObject
//........................................................................

。。。。。

};

在objsafe.h头文件中有ISafeObject接口的相关定义

3。在tryISafeObjectCtrl.cpp中ISafeObject接口的相关实现：

//.............................................................................
// Interface map for IObjectSafety

BEGIN_INTERFACE_MAP( CtryISafeObjectCtrl, COleControl )
 INTERFACE_PART(CtryISafeObjectCtrl, IID_IObjectSafety, ObjSafe)
END_INTERFACE_MAP()

//.............................................................................
// IObjectSafety member functions

// Delegate AddRef, Release, QueryInterface

ULONG FAR EXPORT CtryISafeObjectCtrl::XObjSafe::AddRef()
{
    METHOD_PROLOGUE(CtryISafeObjectCtrl, ObjSafe)
    return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CtryISafeObjectCtrl::XObjSafe::Release()
{
    METHOD_PROLOGUE(CtryISafeObjectCtrl, ObjSafe)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CtryISafeObjectCtrl::XObjSafe::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CtryISafeObjectCtrl, ObjSafe)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

const DWORD dwSupportedBits = 
  INTERFACESAFE_FOR_UNTRUSTED_CALLER |
  INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits = ~ dwSupportedBits;

//.............................................................................
// CStopLiteCtrl::XObjSafe::GetInterfaceSafetyOptions
// Allows container to query what interfaces are safe for what. We're
// optimizing significantly by ignoring which interface the caller is
// asking for.
HRESULT STDMETHODCALLTYPE 
 CtryISafeObjectCtrl::XObjSafe::GetInterfaceSafetyOptions( 
  /* [in] */ REFIID riid,
        /* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
        /* [out] */ DWORD __RPC_FAR *pdwEnabledOptions)
{
 METHOD_PROLOGUE(CtryISafeObjectCtrl, ObjSafe)

 HRESULT retval = ResultFromScode(S_OK);

 // does interface exist?
 IUnknown FAR* punkInterface;
 retval = pThis->ExternalQueryInterface(&riid, 
     (void * *)&punkInterface);
 if (retval != E_NOINTERFACE) { // interface exists
  punkInterface->Release(); // release it--just checking!
 }

 // we support both kinds of safety and have always both set,
 // regardless of interface
 *pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;

 return retval; // E_NOINTERFACE if QI failed
}

/////////////////////////////////////////////////////////////////////////////
// CStopLiteCtrl::XObjSafe::SetInterfaceSafetyOptions
// Since we're always safe, this is a no-brainer--but we do check to make
// sure the interface requested exists and that the options we're asked to
// set exist and are set on (we don't support unsafe mode).
HRESULT STDMETHODCALLTYPE 
 CtryISafeObjectCtrl::XObjSafe::SetInterfaceSafetyOptions( 
        /* [in] */ REFIID riid,
        /* [in] */ DWORD dwOptionSetMask,
        /* [in] */ DWORD dwEnabledOptions)
{
    METHOD_PROLOGUE(CtryISafeObjectCtrl, ObjSafe)

 // does interface exist?
 IUnknown FAR* punkInterface;
 pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);
 if (punkInterface) { // interface exists
  punkInterface->Release(); // release it--just checking!
 }
 else { // interface doesn't exist
  return ResultFromScode(E_NOINTERFACE);
 }

 // can't set bits we don't support
 if (dwOptionSetMask & dwNotSupportedBits) { 
  return ResultFromScode(E_FAIL);
 }

 // can't set bits we do support to zero
 dwEnabledOptions &= dwSupportedBits;
 // (we already know there are no extra bits in mask )
 if ((dwOptionSetMask & dwEnabledOptions) !=
   dwOptionSetMask) {
  return ResultFromScode(E_FAIL);
 }        

 // don't need to change anything since we're always safe
 return ResultFromScode(S_OK);
}

[http://www.cnblogs.com/carekee/articles/1772201.html](http://www.cnblogs.com/carekee/articles/1772201.html)

# [ActiveX控件安全初始化之二:自己动手修改注册表](http://www.cnblogs.com/carekee/articles/1772203.html)

这里所谓的修改注册表的方法就是使用组件分组管理器(Component Categories Manager)创建一个正确的入口到系统注册表。IE3通过检测注册表绝对一个控件是否是可以安全性初始化和脚本操作的。IE3通过调用 ICatInformation::IsClassOfCategories 方法决定是否控件支持给出的安全性分组。

1。创建了一个叫做AxCSChart.ocx的MFC ActiveX控件。

2。在AxCSChart.cpp中

加入头文件

//.........................
#include "comcat.h"
#include "Objsafe.h"
//.........................

const GUID CDECL CLSID_SafeItem =
  { 0x7AE7497B, 0xCAD8, 0x4E66, { 0xA5,0x8B,0xDD,0xE9,0xBC,0xAF,0x6B,0x61 } };

// 创建组件种类
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;

    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered.
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409 ; // english

    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is.
    int len = (int)wcslen(catDescription);
    if (len>127)
        len = 127;
    wcsncpy(catinfo.szDescription, catDescription, len);
    // Make sure the description is null terminated.
    catinfo.szDescription[len] = '\0';

    hr = pcr->RegisterCategories(1, &catinfo);
        pcr->Release();

    return hr;
}

// 注册组件种类
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    // Register your component categories information.
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
                NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Register this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}
// 卸载组件种类
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Unregister this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
    }

    if (pcr != NULL)
        pcr->Release();

    return hr;
}

然后在DllRegisterServer和DllUnregisterServer中调用

// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
 AFX_MANAGE_STATE(_afxModuleAddrThis);

 if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
  return ResultFromScode(SELFREG_E_TYPELIB);

 if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
  return ResultFromScode(SELFREG_E_CLASS);
//.............................................................................
 HRESULT hr;
 // 标记控件初始化安全.
    // 创建初始化安全组件种类
    hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");
    if (FAILED(hr))
        return hr;
    // 注册初始化安全
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;

    // 标记控件脚本安全
    // 创建脚本安全组件种类 
    hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
    if (FAILED(hr))
        return hr;
    // 注册脚本安全组件种类
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;
//.............................................................................

 return NOERROR;
}

// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
 AFX_MANAGE_STATE(_afxModuleAddrThis);

 if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
  return ResultFromScode(SELFREG_E_TYPELIB);

 if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
  return ResultFromScode(SELFREG_E_CLASS);
//.............................................................................
 HRESULT hr;
 // 删除控件初始化安全入口.
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;
    // 删除控件脚本安全入口
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;
//.............................................................................

 return NOERROR;
}

可参见MSDN

ms-help://MS.MSDNQTR.2003FEB.2052/dnaxctrl/html/msdn_signmark.htm



