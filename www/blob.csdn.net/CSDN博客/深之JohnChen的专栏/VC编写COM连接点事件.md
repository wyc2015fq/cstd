# VC编写COM连接点事件 - 深之JohnChen的专栏 - CSDN博客

2011年07月12日 20:14:45[byxdaz](https://me.csdn.net/byxdaz)阅读数：8557


VC编写COM连接点事件

COM 中的典型方案是让客户端对象实例化服务器对象，然后调用这些对象。然而，没有一种特殊机制的话，这些服务器对象将很难转向并回调到客户端对象。COM 连接点便提供了这种特殊机制，实现了服务器和客户端之间的双向通信。使用连接点，服务器能够在服务器上发生某些事件时调用客户端。

原理如下图：

![](http://hi.csdn.net/attachment/201107/9/0_1310214889TpC7.gif)

![](http://hi.csdn.net/attachment/201107/9/0_1310214906Y4FQ.gif)

有了连接点，服务器可通过定义一个接口来指定它能够引发的事件。服务器上引发事件时，要采取操作的客户端会向服务器进行自行注册。随后，客户端会提供服务器所定义接口的实现。

客户端可通过一些标准机制向服务器进行自行注册。COM 为此提供了 IConnectionPointContainer 和 IConnectionPoint 接口。

COM 连接点服务器的客户端可用 C++ 和 C# 托管代码来编写。C++ 客户端会注册一个类的实例，该类提供了接收器接口的实现。托管客户端会注册单个事件的委托，因而会按每个事件通知方法创建单个接收器，具体参考C#的互操作部分内容。

一、连接点程序编写

1、  使用ATL建立组件程序。

2、  添加ATL     SIMPLE     OBJECT，支持连接点事件。

注：如果当时没有现在连接点事件，可以在.idl文件中手动添加。比如

    [

       uuid(57CCB7A5-F3B6-4990-91CD-33A82E1AAA46),

       helpstring("IFunEvent dispinterface")

    ]

    dispinterface _IFunEvent

    {

       properties:

           //  事件接口没有任何属性

       methods:

           [id(1), helpstring("方法OnResult")] HRESULT OnResult([out,retval] LONG* retval);

           [id(2), helpstring("方法OnType")] HRESULT OnType([in] LONG nType);

}

3、  因为支持连接点事件，这样将会自动生成一个     _XXXEVENT源接口。我们在其中增加想要触发的方法。

4、  选择组件下的事件对象，弹出对话框选择添加方法。可以继续添加多个方法…

5、  实现方法（其实组件里只是做方法的申明，客户调用时才实现这些方法）。实现时选中组件/类，按右键，在弹出菜单中选中implement     connection....

就会产生CProxy_xxxEvent类，里面有Fire函数的实现，都是自动生成的。

6、  完成组件的其他接口函数。

组件的连接点编写比较简单，关键是如何在客户端实现事件监听与接收。在.NET下很容易实现。但在VC中比较繁琐。

二、连接点客户端实现（VC）

1、  包含“工程_i.h”头文件，引入“工程.tlb”ole库文件。比如：

#include "ATLDemo_i.h"

#import "ATLDemo.tlb" named_guids raw_interfaces_only

2、  创建一个类：由_IXXXEvent派生过来。（XXX为实际事件名） 

实现类各个虚函数重载，如果_IXXXEvent是IUnkown接口只需要重载QueryInterface、AddRef、Release函数；如果_IXXXEvent是双向接口需要重载实现IUnkown接口三个函数和IDispatch接口四个函数。

实现事件功能，通过函数、用SINK_ENTRY_INFO实现事件的映射、Invoke函数里面实现（通过事件ID）三种方法之一来实现。

用SINK_ENTRY_INFO实现事件的映射 

如： 

BEGIN_SINK_MAP(CEventSink) 

    SINK_ENTRY_INFO(1,DIID__INew01Events,DISPID_MSG,Msg,&MsgInfo) 

END_SINK_MAP() 

我在组件中定义了一个Msf函数，所以在这里对其进行消息隐射。然后实现Msg方法。 

3、  如何调用

3.1使用工程支持COM，使用afxoleinit或者CoInitialize/Un CoInitialize

3.2得到组件接口 

3.3得到连接点容器，查找连接点。

3.4利用Advise将一个监听对象传给组件，这样当事件发生的时候事件就会响应。在不使用时，通过UnAdvise来断开连接点事件。同时也利用AfxConnectionAdvice将监听对象传给组件接口。

3.5 释放资源。

具体代码如下：

#pragma once

#include "ATLDemo_i.h"

#import "ATLDemo.tlb" named_guids raw_interfaces_only

class CSkin : public _IFunEvent

{

public:

    CSkin(void);

    ~CSkin(void);

private:

    DWORD       m_dwRefCount;

public:

    STDMETHODIMP Fire_OnType( LONG nType)

    {

       CString    strTemp;

       strTemp.Format(_T("The result is %d"), nType);

       AfxMessageBox(strTemp);

       return S_OK;;

    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject)

    {

       if (iid == DIID__IFunEvent)

       {

           m_dwRefCount++;

           *ppvObject = (void *)this;

           return S_OK;

       }

       if (iid == IID_IUnknown)

       {

           m_dwRefCount++;            

           *ppvObject = (void *)this;

           return S_OK;

       }

       return E_NOINTERFACE;

    }

    ULONG STDMETHODCALLTYPE AddRef()

    {

       m_dwRefCount++;

       return m_dwRefCount;

    }

    ULONG STDMETHODCALLTYPE Release()

    {

       ULONG l;

       l  = m_dwRefCount--;

       if ( 0 == m_dwRefCount)

       {

           delete this;

       }

       return l;

    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 

       /* [out] */ __RPC__out UINT *pctinfo)

    {

       return S_OK;

    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo( 

       /* [in] */ UINT iTInfo,

       /* [in] */ LCID lcid,

       /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo)

    {

       return S_OK;

    }

     HRESULT STDMETHODCALLTYPE GetIDsOfNames( 

       /* [in] */ __RPC__in REFIID riid,

       /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,

       /* [range][in] */ UINT cNames,

       /* [in] */ LCID lcid,

       /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId)

    {

       return S_OK;

    }

     /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 

       /* [in] */ DISPID dispIdMember,

       /* [in] */ REFIID riid,

       /* [in] */ LCID lcid,

       /* [in] */ WORD wFlags,

       /* [out][in] */ DISPPARAMS *pDispParams,

       /* [out] */ VARIANT *pVarResult,

       /* [out] */ EXCEPINFO *pExcepInfo,

       /* [out] */ UINT *puArgErr)

    {

       switch(dispIdMember) // 根据不同的dispIdMember,完成不同的回调函数，事件函数的ID编号

       {

       case 2:

           {

              // 1st param : [in] long lValue.

              VARIANT varlValue;

              long lValue = 0;

              VariantInit(&varlValue);

              VariantClear(&varlValue);

              varlValue = (pDispParams->rgvarg)[0];

              lValue = V_I4(&varlValue);

              Fire_OnType(lValue);

           }

           break;

       default:   break;

       }

       return S_OK;

    }

};

#include "StdAfx.h"

#include "Skin.h"

CSkin::CSkin(void)

{

     m_dwRefCount =0;

}

CSkin::~CSkin(void)

{

}


实现部分：

       CoInitialize(NULL);

    CComPtr<IFun> pFun;

    HRESULT hr = pFun.CoCreateInstance(CLSID_Fun);

    if(hr!=S_OK)

    {

       return ;

    }

    IConnectionPointContainer *pCPC;

    hr = pFun->QueryInterface(IID_IConnectionPointContainer,(void **)&pCPC);

    if(!SUCCEEDED(hr))

    {

       return ;

    }

    IConnectionPoint *pCP;

    hr = pCPC->FindConnectionPoint(DIID__IFunEvent,&pCP);

    if ( !SUCCEEDED(hr) )

    {

       return ;

    }

    pCPC->Release();

    IUnknown *pSinkUnk;

    CSkin *pSink = new CSkin();

    hr = pSink->QueryInterface(IID_IUnknown,(void **)&pSinkUnk);

    DWORD dwAdvise;

    hr = pCP->Advise(pSinkUnk,&dwAdvise);//接收器与连接点建立关联

    LONG c = 0;

    pFun->Add(1,5,&c);

    //pCP->Unadvise(dwAdvise) //断开连接点事件

    pCP->Release();

    pFun.Release();

    CoUninitialize();

参考资料：

COM Connection Points  By Alex C. Punnen：

[http://www.codeproject.com/KB/COM/connectionpoint.aspx](http://www.codeproject.com/KB/COM/connectionpoint.aspx)

COM组件设计与应用

[http://www.vckbase.com/document/viewdoc/?id=1525](http://www.vckbase.com/document/viewdoc/?id=1525)

 VC编写与调用COM连接点代码

[http://download.csdn.net/source/3437607](http://download.csdn.net/source/3437607)

