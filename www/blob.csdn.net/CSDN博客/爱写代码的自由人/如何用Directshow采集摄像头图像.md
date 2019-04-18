# 如何用Directshow采集摄像头图像 - 爱写代码的自由人 - CSDN博客





2018年02月25日 17:50:00[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1934








      很多人接触Directshow都是从采集摄像头开始的，因为用Directshow做采集是Windows平台上最通用、最成熟的做法。 现在市场上大多数的摄像头和采集卡在Windows系统上的驱动基于WDM架构，微软定义了采集卡设备与上层程序间的通信驱动接口，这已经成为一种标准，因此，控制摄像头和采集图像基本都通过Directshow框架来实现。VLC，FFmpeg等软件支持采集摄像头图像其实也是通过DShow，只是做了一层封装。

     如果我们要做一个采集摄像头应用，要实现采集图像数据（即获得图像的YUV或RGB数据）的功能，应该如何开发呢？很多初学者会想到参考Directshow SDK sample里面的例子----AMCap，因为这个例子也是支持采集摄像头图像的。不过，它不支持直接获得原始图像数据。AMCap支持三种工作模式：预览模式，采集模式，预览+采集。预览模式就是把采集到的图像显示到窗口中；而采集模式是把采集到的视频录制成一个AVI文件。

    在预览模式，AMCap创建的Directshow链路图如下：

![](https://img-blog.csdn.net/20180225155755839)


   在预览+采集模式下，AMCap创建的Directshow链路图如下：

![](https://img-blog.csdn.net/20180225155806493)


      在预览模式，AMCap只是把（USB Camera）Capture Filter采集到的图像传给视频渲染器Video Renderer，Video Renderer负责把图像显示到Renderer捆绑的窗口上。

     而预览+采集模式下，从Capture Filter的输出Pin输出两条链路，一条是预览：Smart Tee 0001->LAVVideo Decoder -> Video Renderer，另外一条是保存文件：Smart Tee ->AVI Mux -> File Writer，其中File Write把收到的图像写到一个AVI文件中（123.avi）。但是无论工作在预览模式还是采集模式，AMCap都没有接口获得原始图像数据。

     虽然通过这个例子不能实现我们需要的功能，但是这个例子对如何使用Capture Filter做预览和录制还是很有参考价值的。

    有些读者可能会想：如果我们实现一个自定义Filter，把它连到Capture Filter后面，并且实现了一个回调，当收到Capture Filter来的Sample数据的时候就把它传给应用层，不是可以做到获得图像的数据了吗？

     没错，思路的确是对的。我们可以实现一个自定义Filter，但是自己开发Filter比较麻烦，建议用更简单的方式，事实上微软已经为我们提供了一个Sample Grabber的Filter，这个Filter有抓取数据和设置回调函数的功能。Sample Grabber一般已经安装到系统中，如果没有安装，大家可以去到Directshow SDK目录找到这个Sample工程（路径是：\SDK (C++)\Samples\C++\DirectShow\Filters\Grabber），然后编译，注册。

  关于Sample Grabber的用法，DirectX SDK的官方文档已经描述得很清楚，我这里摘录一下：



# Using the Sample Grabber

The [Sample Grabber]() filter is a transform filter that can be used to grab media samples from a stream as they pass through the filter graph. 

If you simply want to grab a bitmap from a video file, it is easier to use the [Media Detector (MediaDet)]() object. See [Grabbing a Poster Frame]() for details. The Sample Grabber is more flexible, however, because it works with nearly any media type (see [**ISampleGrabber::SetMediaType**]() for the few exceptions), and offers more control to the application. 

**Add the Sample Grabber to the Filter Graph**

Start by creating an instance of the Sample Grabber filter and adding it to the filter graph:

```
// Create the Sample Grabber.
IBaseFilter *pGrabberF = NULL;
hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
    IID_IBaseFilter, (void**)&pGrabberF);
if (FAILED(hr))
{
    // Return an error.
}
hr = pGraph->AddFilter(pGrabberF, L"Sample Grabber");
if (FAILED(hr)
{
    // Return an error.
}
```

Query the Sample Grabber for the [**ISampleGrabber**]() interface.

```
ISampleGrabber *pGrabber;
pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber);
```

**Set the Media Type**

When you first create the Sample Grabber, it has no preferred media type. This means you can connect to almost any filter in the graph, but you would have no control over the type of data that it received. Before building the rest of the graph, therefore, you must set a media type for the Sample Grabber, by calling the [**ISampleGrabber::SetMediaType**]() method.

When the Sample Grabber connects, it will compare this media type against the media type offered by the other filter. The only fields that it checks are the major type, subtype, and format type. For any of these, the value GUID_NULL means "accept any value." Most of the time, you will want to set the major type and subtype. For example, the following code specifies uncompressed 24-bit RGB video:

```
AM_MEDIA_TYPE mt;
ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
mt.majortype = MEDIATYPE_Video;
mt.subtype = MEDIASUBTYPE_RGB24;
hr = pGrabber->SetMediaType(&mt);
```

The next example sets the media type based on the bit depth of the display:

```
// Find the current bit depth.
HDC hdc = GetDC(NULL);
int iBitDepth = GetDeviceCaps(hdc, BITSPIXEL);
ReleaseDC(NULL, hdc);

// Set the media type.
mt.majortype = MEDIATYPE_Video;
switch (iBitDepth)
{
case 8:
    mt.subtype = MEDIASUBTYPE_RGB8;
    break;
case 16:
    mt.subtype = MEDIASUBTYPE_RGB555;
    break;
case 24:
    mt.subtype = MEDIASUBTYPE_RGB24;
    break;
case 32:
    mt.subtype = MEDIASUBTYPE_RGB32;
    break;
default:
    return E_FAIL;
}
hr = pGrabber->SetMediaType(&mt);
```

**Build the Filter Graph**

Now you can build the rest of the filter graph. Because the Sample Grabber will only connect using the media type you have specified, this lets you take advantage of the Filter Graph Manager's [Intelligent Connect]() mechanisms when you build the graph. 

For example, if you specified uncompressed video, you can connect a source filter to the Sample Grabber, and the Filter Graph Manager will automatically add the file parser and the decoder. The following example uses the ConnectFilters helper function, which is listed in [Connect Two Filters]():

```
IBaseFilter *pSrc;
hr = pGraph->AddSourceFilter(wszFileName, L"Source", &pSrc);
if (FAILED(hr))
{
    // Return an error code.
}
hr = ConnectFilters(pGraph, pSrc, pGrabberF);
```

The Sample Grabber is a transform filter, so the output pin must be connected to another filter. Often, you may simply want to discard the samples after you are done with them. In that case, connect the Sample Grabber to the [Null Renderer Filter](), which discards the data that it receives.

Be aware that placing the Sample Grabber between a video decoder and the video renderer can significantly hurt rendering performance. The Sample Grabber is a trans-in-place filter, which means the output buffer is the same as the input buffer. For video rendering, the output buffer is likely to be located on the graphics card, where read operations are much slower, compared with read operations in main memory.

**Run the Graph**

The Sample Grabber operates in one of two modes: 
- Buffering mode makes a copy of each sample before delivering the sample downstream. 
- Callback mode invokes an application-defined callback function on each sample. 

This article describes buffering mode. (Before using callback mode, be aware that the callback function must be quite limited. Otherwise, it can drastically reduce performance or even cause deadlocks. For more information, see [**ISampleGrabber::SetCallback**]().) To activate buffering mode, call the [**ISampleGrabber::SetBufferSamples**]() method with the value TRUE. 

Optionally, call the [**ISampleGrabber::SetOneShot**]() method with the value TRUE. This causes the Sample Grabber to stop the graph after it receives the first media sample, which is useful if you want to grab a single frame from the stream. Seek to the desired time, run the graph, and wait for the [EC_COMPLETE]() event. Note that the level of frame accuracy depends on the source. For example, seeking an MPEG file is often not frame accurate.

```
// Set one-shot mode and buffering.
hr = pGrabber->SetOneShot(TRUE);
hr = pGrabber->SetBufferSamples(TRUE);

pControl->Run(); // Run the graph.
pEvent->WaitForCompletion(INFINITE, &evCode); // Wait till it's done.
```

To run the graph as fast as possible, turn of the graph clock as described in [Setting the Graph Clock]().

**Grab the Sample**

In buffering mode, the Sample Grabber stores a copy of every sample. The [**ISampleGrabber::GetCurrentBuffer**]() method copies the buffer into a caller-allocated array. To determine the size of the array that is needed, first call **GetCurrentBuffer** with a NULL pointer for the array address:

```
// Find the required buffer size.
long cbBuffer = 0;
hr = pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
```

Allocate the array and call the method a second time to copy the buffer:

```
char *pBuffer = new char[cbBuffer];
if (!pBuffer) 
{
    // Out of memory. Return an error code.
}
hr = pGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
```

看了文档，大家不难理解吧。我后面会介绍具体怎么用代码来实现。

现在先讲解一下怎么创建一个带Capture Filter 的Filter Graph。

先定义需要用到的接口变量：

IVideoWindow* m_pVW ; 
IMediaControl* m_pMC ;
IMediaEventEx* m_pME ;
IGraphBuilder* m_pGraph ;
ICaptureGraphBuilder2* m_pBuilder ; //Graph Builder
IBaseFilter* pSrcF;  //Source Capture Filter
IBaseFilter * m_pGrabberFilter;//Sample Grabber Filter
IBaseFilter * m_pNullFilter; //Null Renderer Filter
PLAYSTATE    m_psCurrent; //Filter Graph的运行状态


然后下面分几步来完成。

第一步，创建Filter Graph Manager对象，获取控制接口。其中，IMediaControl接口是用来控制采集的动作，包括开始，停止等；IVideoWindow接口用来设置和获取视频预览窗口的属性，包括窗口大小，显示的比例大小，显示的位置等；而IMediaEventEx则用来获取Graph运行中的状态事件。


```cpp
HRESULT CCaptureVideo::GetInterfaces()
{
    HRESULT hr;
	
	if(m_pGraph == NULL){
		// Create the filter graph
		hr = CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
							   IID_IGraphBuilder, (void **) &m_pGraph);
		if (FAILED(hr))
			return hr;
	}
	else
		return E_FAIL ;

	if(m_pBuilder == NULL) {
		// Create the capture graph builder
		hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
							   IID_ICaptureGraphBuilder2, (void **) &m_pBuilder);
		if (FAILED(hr))
			return hr;
	}
	else
		return E_FAIL ;
    
    // Obtain interfaces for media control and Video Window
    hr = m_pGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &m_pMC);
    if (FAILED(hr))
        return hr;
	
    hr = m_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
    if (FAILED(hr))
        return hr;
	
    hr = m_pGraph->QueryInterface(IID_IMediaEvent, (LPVOID *) &m_pME);
    if (FAILED(hr))
        return hr;
	
    // Set the window handle used to process graph events
    hr = m_pME->SetNotifyWindow((OAHWND)m_hApp, WM_GRAPHNOTIFY, 0);
	
    return hr;
}
```

第二步，找到采集卡的Capture Filter，获得其IBaseFilter接口指针，把Capture Filter添加到Filter Graph。下面的函数把系统中枚举到的第一个Capture Filter的IBaseFilter指针保存起来，通过ppSrcFilter参数返回。

```cpp
HRESULT CCaptureVideo::FindCaptureDevice(IBaseFilter **ppSrcFilter)
{
	HRESULT hr;
    IBaseFilter * pSrc = NULL;
    CComPtr <IMoniker> pMoniker =NULL;
    ULONG cFetched;
	
    if (!ppSrcFilter)
        return E_POINTER;
	
    // Create the system device enumerator  
    CComPtr <ICreateDevEnum> pDevEnum =NULL;
	
    hr = CoCreateInstance (CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
                           IID_ICreateDevEnum, (void **) &pDevEnum);
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't create system enumerator!  hr=0x%x"), hr);
        return hr;
    }
	
    // Create an enumerator for the video capture devices
    CComPtr <IEnumMoniker> pClassEnum = NULL;
	
    hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't create class enumerator!  hr=0x%x"), hr);
        return hr;
    }
	
    // If there are no enumerators for the requested type, then 
    // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
    if (pClassEnum == NULL)
    {
	   //       MessageBox(m_hApp,TEXT("No video capture device was detected.\r\n\r\n")
	   //                TEXT("This sample requires a video capture device, such as a USB WebCam,\r\n")
	   //                TEXT("to be installed and working properly.  The sample will now close."),
	   //                TEXT("No Video Capture Hardware"), MB_OK | MB_ICONINFORMATION);
	   return E_FAIL;
    }
	
    // Use the first video capture device on the device list.
    // Note that if the Next() call succeeds but there are no monikers,
    // it will return S_FALSE (which is not a failure).  Therefore, we
    // check that the return code is S_OK instead of using SUCCEEDED() macro.
    
	//To Find the Analog Capture Devices....
	
	BOOL         Found ;
	IPin        *pP = 0;
	IEnumPins   *pins=0;
	ULONG        n;
	PIN_INFO     pinInfo;
	Found	= FALSE;
	IKsPropertySet *pKs=0;
	GUID guid;
	DWORD dw;
	BOOL fMatch = FALSE;
	
	//while(S_OK == (pClassEnum->Next (1, &pMoniker, &cFetched)))
	if(S_OK == (pClassEnum->Next (1, &pMoniker, &cFetched)))
	{	
		// Bind Moniker to a filter object
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&pSrc);
		if (FAILED(hr))
		{
			DisplayMesg(TEXT("Couldn't bind moniker to filter object!  hr=0x%x"), hr);
			return hr;
		}
			 if(SUCCEEDED(pSrc->EnumPins(&pins)))
				{     
				 while(!Found && (S_OK == pins->Next(1, &pP, &n)))
					{
					 	vType ++;
						if(S_OK == pP->QueryPinInfo(&pinInfo))
						{

							if(pinInfo.dir == PINDIR_INPUT)
							{
								// is this pin an ANALOGVIDEOIN input pin?
								if(pP->QueryInterface(IID_IKsPropertySet,
									(void **)&pKs) == S_OK)
								{
									if(pKs->Get(AMPROPSETID_Pin,
										AMPROPERTY_PIN_CATEGORY, NULL, 0,
										&guid, sizeof(GUID), &dw) == S_OK)
									{
										if(guid == PIN_CATEGORY_ANALOGVIDEOIN)
											fMatch = TRUE;
									}
									pKs->Release();
								}

								if(fMatch)
								{
									Found = TRUE;
									bDevCheck = Found;
									break;
								}
							}
							pinInfo.pFilter->Release();
						}
						pP->Release();
					}
					pins->Release();
				}
  			}
 
    // Copy the found filter pointer to the output parameter.
    // Do NOT Release() the reference, since it will still be used
    // by the calling function.
	pSrcF = pSrc;
    *ppSrcFilter = pSrc;
	
    return hr;
}
```

然后添加Filter到Graph：

```cpp
// Add Capture filter to our graph.
    hr = m_pGraph->AddFilter(pSrcFilter, L"Video Capture");
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't add the capture filter to the graph!  hr=0x%x\r\n\r\n") 
            TEXT("If you have a working video capture device, please make sure\r\n")
            TEXT("that it is connected and is not being used by another application.\r\n\r\n"), hr);
        pSrcFilter->Release();
        return hr;
    }
```


第三步，调用ICaptureGraphBuilder2::RenderStream函数构建一个预览或采集的链路图。如果是预览模式，则自动将Capture Filter的Preview Output Pin与Video Renderer连接起来（如下面这段代码的作用）；如果是采集模式，就会将Capture Filter的Capture Output Pin与下游的Filter连接。

```cpp
// Render the preview pin on the video capture filter
    // Use this instead of m_pGraph->RenderFile
    hr = m_pBuilder->RenderStream (&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
                                   pSrcFilter, NULL, NULL);
```

**但是这里我们不需要用自动连接的功能，因为我们是手动加入Filter并自己连起来的，所以大家只要知道：如果不显示就不需要调用上面这句代码。**

上面1-3步的过程综合起来可以用下面的CaptureVideo函数实现：

```cpp
HRESULT CCaptureVideo::CaptureVideo()
{
	HRESULT hr;
    IBaseFilter *pSrcFilter=NULL;

    // Get DirectShow interfaces
    hr = GetInterfaces();
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Failed to get video interfaces!  hr=0x%x"), hr);
        return hr;
    }

    // Attach the filter graph to the capture graph
    hr = m_pCapture->SetFiltergraph(m_pGraph);
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Failed to set capture filter graph!  hr=0x%x"), hr);
        return hr;
    }

    // Use the system device enumerator and class enumerator to find
    // a video capture/preview device, such as a desktop USB video camera.

	hr = FindCaptureDevice(&pSrcFilter);
    if (FAILED(hr))
    {
        // Don't display a message because FindCaptureDevice will handle it
        return hr;
    }

	if(nAnalogCount == 0)
		AfxMessageBox("Capture device is not detected.!") ;
   
	if( bDevCheck == FALSE)
	{
		return E_FAIL;
	}


    // Add Capture filter to our graph.
    hr = m_pGraph->AddFilter(pSrcFilter, L"Video Capture");
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't add the capture filter to the graph!  hr=0x%x\r\n\r\n") 
            TEXT("If you have a working video capture device, please make sure\r\n")
            TEXT("that it is connected and is not being used by another application.\r\n\r\n"), hr);
        pSrcFilter->Release();
        return hr;
    }

#if 1
	 //注意：只用于预览的情况，如果不需要显示，就把下面这句屏蔽掉。
    // Render the preview pin on the video capture filter
    // Use this instead of m_pGraph->RenderFile
    hr = m_pBuilder->RenderStream (&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
                                   pSrcFilter, NULL, NULL);
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't render the video capture stream.  hr=0x%x\r\n")
            TEXT("The capture device may already be in use by another application.\r\n\r\n")
            TEXT("The sample will now close."), hr);
        pSrcFilter->Release();
        return hr;
    }
    // Set video window style and position
    hr = SetupVideoWindow();
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't initialize video window!  hr=0x%x"), hr);
        return hr;
    }
#endif

 // Now that the filter has been added to the graph and we have
    // rendered its stream, we can release this reference to the filter.
    pSrcFilter->Release();

#ifdef REGISTER_FILTERGRAPH
    // Add our graph to the running object table, which will allow
    // the GraphEdit application to "spy" on our graph
    hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
        m_dwGraphRegister = 0;
    }
#endif
		
    // Start previewing video data
    hr = m_pMC->Run();
    if (FAILED(hr))
    {
        DisplayMesg(TEXT("Couldn't run the graph!  hr=0x%x"), hr);
        return hr;
    }
	
    // Remember current state
    m_psCurrent = RUNNING;
        
    return S_OK;
}
```

到这里还没有完成我们要做的工作，因为还没有把Sample Grabber添加进去，我们继续接着上面。

第四步，把SampleGrabber和NullRenderer添加到Graph，并将Capture Filter-》Sample Grabber，Sample Grabber-》NullRenderer连接起来。

```cpp
// Create the Sample Grabber.
IBaseFilter *pF = NULL;
hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
					  IID_IBaseFilter, reinterpret_cast<void**>(&pF));

// Set the media type.
AM_MEDIA_TYPE mt;
ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
mt.majortype = MEDIATYPE_Video;
mt.subtype = MEDIASUBTYPE_YUY2;

ISampleGrabber *pGrabber = NULL;
hr = pF->QueryInterface(IID_ISampleGrabber, reinterpret_cast<void**>(&pGrabber));    
hr = pGrabber->SetMediaType(&mt);
hr = pGrabber->SetOneShot(false);
hr = pGrabber->SetBufferSamples(false);
gcap.m_pICallback = new SampleGrabberCallback(port, this);
hr = pGrabber->SetCallback(gcap.m_pICallback, 1);
pGrabber->Release();

m_pGrabberFilter = pF;

IBaseFilter *pNull = NULL;
hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
					  IID_IBaseFilter, reinterpret_cast<void**>(&pNull));

m_pNullFilter = pNull;

hr = m_pGraph->AddFilter(m_pGrabberFilter, L"SampleGrabber");
hr = m_pGraph->AddFilter(m_pNullFilter, L"NullRenderer");

hr = ConnectFilters(m_pGraph, pSrcF, m_pGrabberFilter);
hr = ConnectFilters(m_pGraph, m_pGrabberFilter, m_pNullFilter);
```

第五步，调用IMediaControl接口的Run方法把Graph运行起来就行了。

但是这里还有一些不完善的地方，比如怎么设置采集的分辨率、帧率，以及输出的图像格式，因为涉及的内容比较多，就不在这里讲了。

**CaptureVideo类的代码可以到这里下载：**[点击打开链接](http://download.csdn.net/download/zhoubotong2012/10258425)


**如果大家想知道怎么用FFmpeg API采集摄像头和麦克风的数据，可以参考我的另外一篇博文：[点击打开链接](http://blog.csdn.net/zhoubotong2012/article/details/79338093)**



