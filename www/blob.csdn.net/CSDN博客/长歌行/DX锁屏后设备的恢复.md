# DX锁屏后设备的恢复 - 长歌行 - CSDN博客





2012年12月25日 10:40:01[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：912
个人分类：[图形图像](https://blog.csdn.net/iuhsihsow/article/category/1065735)









DX程序，运行时，锁屏后，再进入，就发现画面卡主了。

实际上是设备没有恢复的缘故。



```cpp
HRESULT hr = device->TestCooperativeLevel();
  	if (hr == D3DERR_DEVICELOST)
  	{
  		// 设备丢失
  	}
  	else if (hr == D3DERR_DEVICENOTRESET)
  	{
  
  		// 设备可以恢复
                 // 删除自分配的内存
 				// 在对设备调用 Reset 前，
 				// 应用程序应释放任何与该设备相关联的
 				// 显式呈现目标、深度模具图面、附加交换链、状态块以及默认资源。
 		// d3dpool_managed可以不管
  		g_vb->Release();
  		g_ib->Release();
                 // 恢复设备
  		hr = device->Reset(&g_d3dpp);
         }
  
 设置设备参数：（只是一个例子）
         D3DPRESENT_PARAMETERS g_d3dpp;
  
 	g_d3dpp.BackBufferWidth = 700;
  	g_d3dpp.BackBufferHeight = 700;
  	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
  	g_d3dpp.BackBufferCount = 1;
  	g_d3dpp.hDeviceWindow = 0;//mWindowHandle;
  	g_d3dpp.Windowed = TRUE;
  	g_d3dpp.Flags = 0;
  	g_d3dpp.FullScreen_RefreshRateInHz = 0;
  	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  
  	// Request depth and stencil buffers.
  	g_d3dpp.EnableAutoDepthStencil = TRUE;
  	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  
  	// Request multisampling.
  	g_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP; 
  	g_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
  	g_d3dpp.MultiSampleQuality = 0;
```



如果Reset失败，一般是以下异常


|异常|条件|
|----|----|
|[InvalidCallException](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.invalidcallexception)|方法调用无效。例如，参数可能包含无效值。|
|[DriverInternalErrorException](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.driverinternalerrorexception)|内部驱动程序错误。当收到此错误时，应用程序通常会关闭。|
|[OutOfMemoryException](http://technet.microsoft.com/zh-cn/sqlserver/system.outofmemoryexception)|Direct3D 无法分配足够的内存来完成调用。|



切换到全屏模式时，Direct3D 会尝试找到一种与后台缓冲区格式匹配的桌面格式，以便后台缓冲区格式和前台缓冲区格式相同。这样就无需转换颜色了。


如果对 Reset 的调用失败，除非设备处于“未重置”状态（由从 [CheckCooperativeLevel](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.device.checkcooperativelevel) 方法的 hResult 参数返回的 [DeviceNotReset](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.resultcode) 来指示），否则设备会被置于“丢失”状态（由从对[CheckCooperativeLevel](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.device.checkcooperativelevel) 的调用返回的值 false 指示）。


调用 Reset 将导致所有纹理内存图面和状态信息丢失，并且还会导致托管纹理在视频内存中被刷新。在对设备调用 Reset 前，应用程序应释放任何与该设备相关联的显式呈现目标、深度模具图面、附加交换链、状态块以及默认资源。


交换链可以是全屏的，也可以是窗口式的。如果新的交换链是全屏的，则会将适配器置于与新大小匹配的显示模式中。


如果调用 Reset 的线程不是用来创建所要重置的设备的线程，则该调用将失败。


调用 [Device](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.device)、Reset 和 [SwapChain](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.swapchain) 时，可以将窗口模式后台缓冲区的格式指定为“未知”。这意味着应用程序在窗口模式下调用 [Device](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.device) 前不必查询当前的桌面格式。对于全屏模式，必须指定后台缓冲区格式。将 [BackBufferCount](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.presentparameters.backbuffercount) 设置为
 0 会创建一个后台缓冲区。


尝试成组重置多个显示适配器时，可传入 [PresentParameters](http://technet.microsoft.com/zh-cn/sqlserver/microsoft.windowsmobile.directx.direct3d.presentparameters) 对象数组，该数组中的每个对象都对应于该适配器组中的一个显示适配器。





设备恢复成功了，可以重设各种矩阵以及视点。


// 伪代码

Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 0.1f, 100.0f);
// 设置视角转换矩阵
device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
// 关闭光照
device->SetRenderState(D3DRS_LIGHTING, FALSE);
// 改变三角形正面的法线
device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
// `计算出一个可以转换到镜头坐标系的矩阵`
Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);






问题：


还是有不解的地方


1. 多适配器怎么恢复


2. 一直还是恢复失败，究竟要删除哪些东西啊，纹理？顶点？特效？Sprite？，具体有哪些啊？


   对于简单的程序，像上面那么做就可以了，但是复杂的程序要一个个排除啊。。。有没有一个CheckList？


3. SetSourceSream是d3dpool_defaul还是d3dpool_managed




