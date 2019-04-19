# directshow原理分析之filter到filter的连接 - 建建的博客 - CSDN博客
2017年12月11日 17:39:22[纪建](https://me.csdn.net/u013898698)阅读数：159
Filter是Directshow中最基本的概念。Directshow使用filter graph来管理filter。filter graph是filter的容器。
Filter一般由一个或者几个Pin组成。filter之间通过Pin来连接，组成一条链。
PIN也是一种COM组件，每一个PIN都实现了IPin接口。
试图链接的两个Pin必须在一个filter graph中。
![](https://img-blog.csdn.net/20141225144059640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9vdHVzZXJz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
连接过程如下：
1.Filter Graph Manager在输出pin上调用IPin::Connect
2.如果输出Pin接受连接，则调用IPin::ReceiveConnection
3.如果输入Pin也接受此次连接，则双方连接成功
首先来分析基类函数CBasePin的Connect实现：
CBasePin继承了IPin。
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- <pre name="code"class="cpp">HRESULT __stdcall CBasePin::Connect(IPin * pReceivePin, __in_opt const AM_MEDIA_TYPE *pmt   // optional media type)
- {  
-     CheckPointer(pReceivePin,E_POINTER);  
-     ValidateReadPtr(pReceivePin,sizeof(IPin));  
-     CAutoLock cObjectLock(m_pLock);  
-     DisplayPinInfo(pReceivePin);  
- 
- /* 检查该Pin是否早已连接 */
- if (m_Connected) {  
-         DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Already connected")));  
- return VFW_E_ALREADY_CONNECTED;  
-     }  
- 
- /*一般Filter只在停止状态下接受连接*/
- if (!IsStopped() && !m_bCanReconnectWhenActive) {  
- return VFW_E_NOT_STOPPED;  
-     }  
- 
- /*开始媒体类型检查，找出一种双方均支持的类型*/
- const CMediaType * ptype = (CMediaType*)pmt;  
- HRESULT hr = AgreeMediaType(pReceivePin, ptype);  
- if (FAILED(hr)) {  
-         DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Failed to agree type")));  
- 
- // Since the procedure is already returning an error code, there
- // is nothing else this function can do to report the error.
-         EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  
- 
- #ifdef DXMPERF
-         PERFLOG_CONNECT( (IPin *) this, pReceivePin, hr, pmt );  
- #endif // DXMPERF
- 
- return hr;  
-     }  
- 
-     DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Connection succeeded")));  
- 
- #ifdef DXMPERF
-     PERFLOG_CONNECT( (IPin *) this, pReceivePin, NOERROR, pmt );  
- #endif // DXMPERF
- 
- return NOERROR;  
- }  
事实上，以上函数并没有进行真正的连接，只是进行了类型检查和状态检查。并且这个函数是从输出Pin进入的。
Connect两个参数分别代表：输出Pin试图链接的输入Pin和指定连接用的媒体类型。
真正的链接是CBase::AgreeMediaType来实现的。
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- <pre name="code"class="cpp">HRESULT CBasePin::AgreeMediaType(IPin *pReceivePin, const CMediaType *pmt)  
- {  
-     ASSERT(pReceivePin);  
-     IEnumMediaTypes *pEnumMediaTypes = NULL;  
- 
- // 判断pmt是不是一个完全指定的媒体类型
- if ( (pmt != NULL) && (!pmt->IsPartiallySpecified())) {  
- //用这个指定的媒体类型去做连接，如果失败不做任何处理。
- return AttemptConnection(pReceivePin, pmt);  
-     }  
- 
- 
- /* 进行pin上支持的媒体类型的枚举 ，开始媒体类型的协商过程*/
- HRESULT hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;  
- 
- for (int i = 0; i < 2; i++) {  
- HRESULT hr;  
- if (i == (int)m_bTryMyTypesFirst) {  
-             hr = pReceivePin->EnumMediaTypes(&pEnumMediaTypes);  
-         } else {  
-             hr = EnumMediaTypes(&pEnumMediaTypes);  
-         }  
- if (SUCCEEDED(hr)) {  
-             ASSERT(pEnumMediaTypes);  
-             hr = TryMediaTypes(pReceivePin,pmt,pEnumMediaTypes);  
-             pEnumMediaTypes->Release();  
- if (SUCCEEDED(hr)) {  
- return NOERROR;  
-             } else {  
- // try to remember specific error codes if there are any
- if ((hr != E_FAIL) &&  
-                     (hr != E_INVALIDARG) &&  
-                     (hr != VFW_E_TYPE_NOT_ACCEPTED)) {  
-                     hrFailure = hr;  
-                 }  
-             }  
-         }  
-     }  
- 
- return hrFailure;  
- }  
从AgreeMediaType看到，该函数首先判断媒体类型的有效性，如果pmt是一种完全指定的媒体类型，那么就以这种媒体类型调用内部
函数AttempConnection进行连接。
但是如果pmt是一个空指针或者不是完全指定的媒体类型，那么真正的协商过程也就开始了。注意，for循环的次数为2，输出Pin上的成员变量
m_bTryMyTypesFirst初始值为false，也就是说，连接过程进行到这里，会首先得到输入pin上的媒体类型枚举器的试连接，如果连接不成功，再去得到
输出Pin上的媒体类型枚举器的试连接。
下面看一下TryMediaTypes函数的实现：
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- HRESULT CBasePin::TryMediaTypes(IPin *pReceivePin, __in_opt const CMediaType *pmt, IEnumMediaTypes *pEnum)  
- {  
- /* 复位枚举器内部状态 */
- 
- HRESULT hr = pEnum->Reset();  
- if (FAILED(hr)) {  
- return hr;  
-     }  
- 
-     CMediaType *pMediaType = NULL;  
- ULONG ulMediaCount = 0;  
- 
- // attempt to remember a specific error code if there is one
- HRESULT hrFailure = S_OK;  
- 
- for (;;) {  
- 
- /* Retrieve the next media type NOTE each time round the loop the
-            enumerator interface will allocate another AM_MEDIA_TYPE structure
-            If we are successful then we copy it into our output object, if
-            not then we must delete the memory allocated before returning */
- 
-         hr = pEnum->Next(1, (AM_MEDIA_TYPE**)&pMediaType,&ulMediaCount);  
- if (hr != S_OK) {  
- if (S_OK == hrFailure) {  
-                 hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;  
-             }  
- return hrFailure;  
-         }  
- 
- 
-         ASSERT(ulMediaCount == 1);  
-         ASSERT(pMediaType);  
- 
- // 检查当前枚举得到的类型是否与不完全指定的类型参数匹配
- 
- if (pMediaType &&  
-             ((pmt == NULL) ||  
-             pMediaType->MatchesPartial(pmt))) {  
- <span style="white-space:pre">    </span>    //进行试连接
-             hr = AttemptConnection(pReceivePin, pMediaType);  
- 
- // attempt to remember a specific error code
- if (FAILED(hr) &&  
-             SUCCEEDED(hrFailure) &&  
-             (hr != E_FAIL) &&  
-             (hr != E_INVALIDARG) &&  
-             (hr != VFW_E_TYPE_NOT_ACCEPTED)) {  
-                 hrFailure = hr;  
-             }  
-         } else {  
-             hr = VFW_E_NO_ACCEPTABLE_TYPES;  
-         }  
- 
- if(pMediaType) {  
-             DeleteMediaType(pMediaType);  
-             pMediaType = NULL;  
-         }  
- 
- if (S_OK == hr) {  
- return hr;  
-         }  
-     }  
- }  
当连接进程进入TryMediaTypes函数后，会使用媒体类型枚举器枚举Pin上提供的所有的媒体类型，然后一种一种的进行试连接。如果有一种成功，则整个Pin连接成功。
最后需要看一下AttempConnection函数的跟Pin连接相关的虚函数的调用的顺序，对与自己实现filter有指导意义：
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- <pre name="code"class="cpp">HRESULT CBasePin::AttemptConnection(IPin* pReceivePin,      // connect to this pin
- const CMediaType* pmt   // using this type
- )  
- {  
- // The caller should hold the filter lock becasue this function
- // uses m_Connected.  The caller should also hold the filter lock
- // because this function calls SetMediaType(), IsStopped() and
- // CompleteConnect().
-     ASSERT(CritCheckIn(m_pLock));  
- 
- // Check that the connection is valid  -- need to do this for every
- // connect attempt since BreakConnect will undo it.
- HRESULT hr = CheckConnect(pReceivePin);  
- if (FAILED(hr)) {  
-         DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("CheckConnect failed")));  
- 
- // Since the procedure is already returning an error code, there
- // is nothing else this function can do to report the error.
-         EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  
- 
- return hr;  
-     }  
- //可以显示媒体类型，有时候挺有用
-     DisplayTypeInfo(pReceivePin, pmt);  
- 
- /* Pin上的媒体类型检查 */
- 
-     hr = CheckMediaType(pmt);  
- if (hr == NOERROR) {  
- 
- /*  Make ourselves look connected otherwise ReceiveConnection
-             may not be able to complete the connection
-         */
-         m_Connected = pReceivePin;  
-         m_Connected->AddRef();  
-         hr = SetMediaType(pmt);//在Pin上保存媒体类型
- if (SUCCEEDED(hr)) {  
- /* 询问连接对方Pin是否能接受当前的媒体类型 */
- 
-             hr = pReceivePin->ReceiveConnection((IPin *)this, pmt);  
- //连接成功
- if (SUCCEEDED(hr)) {  
- /* Complete the connection */
- 
-                 hr = CompleteConnect(pReceivePin);  
- if (SUCCEEDED(hr)) {  
- return hr;  
-                 } else {  
-                     DbgLog((LOG_TRACE,  
-                             CONNECT_TRACE_LEVEL,  
-                             TEXT("Failed to complete connection")));  
-                     pReceivePin->Disconnect();  
-                 }  
-             }  
-         }  
-     } else {  
- // we cannot use this media type
- 
- // return a specific media type error if there is one
- // or map a general failure code to something more helpful
- // (in particular S_FALSE gets changed to an error code)
- if (SUCCEEDED(hr) ||  
-             (hr == E_FAIL) ||  
-             (hr == E_INVALIDARG)) {  
-             hr = VFW_E_TYPE_NOT_ACCEPTED;  
-         }  
-     }  
- 
- // BreakConnect and release any connection here in case CheckMediaType
- // failed, or if we set anything up during a call back during
- // ReceiveConnection.
- 
- // Since the procedure is already returning an error code, there
- // is nothing else this function can do to report the error.
-     EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  
- 
- /*  If failed then undo our state */
- if (m_Connected) {  
-         m_Connected->Release();  
-         m_Connected = NULL;  
-     }  
- 
- return hr;  
- }  
至此：连接双方Pin上的使用的媒体类型协商完了，但是并不能真正的传输数据。因为内存还没分配呗。
这些均是在输出Pin上的CompleteConnect中完成的。
CBaseOutPin继承自CBasePin
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- HRESULT CBaseOutputPin::CompleteConnect(IPin *pReceivePin)  
- {  
-     UNREFERENCED_PARAMETER(pReceivePin);  
- return DecideAllocator(m_pInputPin, &m_pAllocator);  
- }  
DecideAllocator就是Pin之间数据传送所使用的内存分配器的协商。在dshow中数据传输的单元叫做Sample(也是一种COM组件，管理内存用的)。而Sample是由分配器Allocator(也是COM组件)来管理的。连接双方必须使用同一个分配器，该分配器由谁来创建也需要协商。
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- <pre name="code"class="cpp">HRESULT CBaseOutputPin::DecideAllocator(IMemInputPin *pPin, __deref_out IMemAllocator **ppAlloc)  
- {  
- HRESULT hr = NOERROR;  
-     *ppAlloc = NULL;  
- 
- // get downstream prop request
- // the derived class may modify this in DecideBufferSize, but
- // we assume that he will consistently modify it the same way,
- // so we only get it once
-     ALLOCATOR_PROPERTIES prop;  
-     ZeroMemory(&prop, sizeof(prop));  
- 
- // 询问输入Pin对于分配器的需求
-     pPin->GetAllocatorRequirements(&prop);  
- 
- // if he doesn't care about alignment, then set it to 1
- if (prop.cbAlign == 0) {  
-         prop.cbAlign = 1;  
-     }  
- 
- /* 询问输入Pin是否提供一个分配器 */
- 
-     hr = pPin->GetAllocator(ppAlloc);  
- if (SUCCEEDED(hr)) {  
- //决定Sample使用的内存的大小，以及分配器管理的Sample的数量
-         hr = DecideBufferSize(*ppAlloc, &prop);  
- if (SUCCEEDED(hr)) {  
- //通知输入Pin最终使用的分配器的对象
-             hr = pPin->NotifyAllocator(*ppAlloc, FALSE);  
- if (SUCCEEDED(hr)) {  
- return NOERROR;  
-             }  
-         }  
-     }  
- 
- /* 如果输入Pin上不提供分配器，则必须在输出Pin上提供 */
- 
- if (*ppAlloc) {  
-         (*ppAlloc)->Release();  
-         *ppAlloc = NULL;  
-     }  
- 
- /* 创建一个输出Pin上的分配器 */
- 
-     hr = InitAllocator(ppAlloc);  
- if (SUCCEEDED(hr)) {  
- 
- // note - the properties passed here are in the same
- // structure as above and may have been modified by
- // the previous call to DecideBufferSize
-         hr = DecideBufferSize(*ppAlloc, &prop);  
- if (SUCCEEDED(hr)) {  
-             hr = pPin->NotifyAllocator(*ppAlloc, FALSE);  
- if (SUCCEEDED(hr)) {  
- return NOERROR;  
-             }  
-         }  
-     }  
- 
- /* Likewise we may not have an interface to release */
- 
- if (*ppAlloc) {  
-         (*ppAlloc)->Release();  
-         *ppAlloc = NULL;  
-     }  
- return hr;  
- }  
当Pin上的数据传送内存分配器协商成功后，并没有马上分配内存，实际上是等Filter Graph运行后，调用输出Pin上的Active函数时进行的。
[cpp][view
 plain](http://blog.csdn.net/rootusers/article/details/42145787#)[copy](http://blog.csdn.net/rootusers/article/details/42145787#)
- HRESULT CBaseOutputPin::Active(void)  
- {  
- if (m_pAllocator == NULL) {  
- return VFW_E_NO_ALLOCATOR;  
-     }  
- return m_pAllocator->Commit();  
- }  
至此整个Pin的链接才算真正完成。
