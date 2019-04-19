# 关于DX多窗口编程的一篇翻译（感谢Happy_888网友给出的原文 ^_^）(转载) - xqhrs232的专栏 - CSDN博客
2009年07月09日 21:39:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：823标签：[编程																[buffer																[interface																[direct3d																[application																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
Introduction
In DirectX 8, support for rendering to multiple windows is provided through the creation of additional swap chains.  However, there are currently no examples of this in the SDK, and the documentation is a bit vague.  This article is provided to fill the
 gaps, and will explain the steps you need to take to write an application that will render multiple views in separate windows.
在DX8中，对多窗口的支持是通过创建更多的Swap Chains来提供的。SDK中没有相关的例子而且文档也只是泛泛而谈。这篇文章就是为了解决这个问题，它将向您展示应当如何一步步地实现在多个分离窗口中渲染多个视图。
Step 1 - Setting Up The Parent Frame
第一步：设置父框架窗口
In an application with multiple views, we start with a top level frame that will contain child windows in its client area to display various views.  Once the parent frame parent frame has been created, we create our Direct3D device interface, specifying windowed
 mode and setting the top level window handle as the focus window:
在多视图的应用程序中，我们需要从最高层次的框架——这个框架将包含所有在他用户区之内的子视图窗口——开始我们的旅程。当父框架创建的时候，我们需要创建Direct3D Device接口，为其指定使用窗口模式，而且设置这最高层次的窗口句柄作为“焦点窗口”的句柄：
g_pD3D=Direct3DCreate8(D3D_SDK_VERSION);
if (!g_pD3D) return -1;
D3DPRESENT_PARAMETERS d3dpp; 
ZeroMemory( &d3dpp, sizeof(d3dpp) );
d3dpp.Windowed = TRUE;
d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
// Use the current display mode. 使用当前的显示模式
D3DDISPLAYMODE mode;
if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode))) {
    SAFE_RELEASE(g_pD3D);
    return -1;
}
d3dpp.BackBufferFormat = mode.Format;
d3dpp.BackBufferWidth = mode.Width;
d3dpp.BackBufferHeight = mode.Height;
d3dpp.EnableAutoDepthStencil=TRUE;
d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
// m_hWnd is handle to top level window    m_hWnd是最高层窗口的句柄
if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pd3dDevice) ) ) {
    SAFE_RELEASE(g_pD3D);
    return -1;
}
Note that for simplicity the above code does not test depth format, instead choosing a fixed format.  Your application should determine a compatible depth format for the format of the rendering target.
注意上面代码处于简单考虑并没有去测试深度缓存的格式（？depth format），而只是选择了一个确定的格式（D3DFMT_D16）。您的程序应该为需要渲染的Render Target选择一个可接受的深度缓存格式。
The device has a frame buffer, which the child views will be rendered into, as well as a depth buffer which will be shared among the views.  The frame buffer and depth buffer are sized to the full screen resolution, to allow for the fact that the window may
 later be resized.  Otherwise, window size changes would require resetting the device and re-creating the swap chains.
Device都需要有帧缓存，这样子视图才能进行渲染，同时，深度缓冲也应当被不同的视图进行共享。帧缓存和深度缓存都被设置为全屏幕大小，以考虑到可能窗口会被改变大小的情况。如果不的话，窗口改变大小的时候，就需要Reset Device和重新创建Swap Chain。
Step 2 - Setting Up View Windows 
第二步：设置子视图窗口
Now we are ready to create our view windows, and associate them with swap chains that can be rendered to the device.  Once the windows have been created, the following code generates a swap chain for the child window:
现在我们可以准备创建我们的子窗口也就是视图窗口，并把它们与交换链关联以使得他们可以被渲染到Device上。当窗口创建后，下面的代码将为子窗口创建一个交换链：
D3DPRESENT_PARAMETERS d3dpp; 
ZeroMemory( &d3dpp, sizeof(d3dpp) );
d3dpp.Windowed = TRUE;
d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
// Use the current display mode.  使用当前的显示模式
D3DDISPLAYMODE mode;
g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode);
d3dpp.BackBufferFormat = mode.Format;
// m_hWnd contains child window handle  m_hWnd储存子窗口的句柄
d3dpp.hDeviceWindow=m_hWnd;
// m_pSwapChain is IDirect3DSwapChain *   m_pSwapChain是一个IDirect3DSwapChain*对象
g_pd3dDevice->CreateAdditionalSwapChain(&d3dpp, &m_pSwapChain);
After executing this code, the m_pSwapChain variable will contain a pointer to an IDirect3DSwapChain interface, which contains a frame buffer corresponding to the client area of the child window.  This process is performed for each view window, so that that
 there is a swap chain for each view window.
经过这些代码之后，m_pSwapChain变量就储存了IDirect3DSwapChain接口的指针，这个接口将储存子窗口视图区所对应的帧缓冲。
Step 3 - Rendering a View
第三步：渲染视图
Prior to rendering each view, we must direct the device to render to the appropriate frame buffer, using the SetRenderTarget() method.  We pass the back buffer from the window's swap chain, while using the depth buffer that was originally created with the device:
在渲染每个视图窗口之前，我们必须使得Device来渲染对应的帧缓冲，这我们就需要用到SetRenderTarget方法。我们向其中传入子窗口SwapChain交换链的后备缓冲BackBuffer，以及使用最开始跟着Device一起创建的深度缓冲。
LPDIRECT3DSURFACE8 pBack=NULL,pStencil=NULL;
m_pSwapChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
g_pd3dDevice->GetDepthStencilSurface(&pStencil);
g_pd3dDevice->SetRenderTarget(pBack,pStencil);
pBack->Release();
pStencil->Release();
Note that we release the stencil and backbuffer pointers after we use them, because the GetBackBuffer() and GetDepthStencilSurface() functions call AddRef() on these interfaces to increment their reference counters.  Failing to release them would lead to a
 memory leak.
注意我们必须Release掉Stencil和BackBuffer的指针，因为GetBackBuffer和GetDepthStencilSurface这两个函数都会调用COM的AddRef方法，来增加相应COM接口的引用计数，因此如果不删除它们，将会导致内存泄露。
We are now ready to render the view.  Rendering is performed within a scene in the normal manner, except that we call Present() on the swap chain interface rather than the device interface:
我们现在已经做好准备渲染视图窗口了。渲染的方法看起来和我们平常用的方法差不多，只是有一点：我们现在需要调用Swap Chain的接口，而不是Device的接口。
g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0x00000000,1.0,0);
if (SUCCEEDED(g_pd3dDevice->BeginScene())) {
    // rendering code goes here  渲染代码写在这里
    g_pd3dDevice->EndScene();
}
m_pSwapChain->Present(NULL,NULL,NULL,NULL);
Step 4 - Handling Resize of Child Views
第四步，子窗口的Resize问题
DirectX will automatically deal with changes in the child view by using a stretch blit to present the swap chain if the dimensions have client area is not the same size as the swap chain's frame buffer.  However, this may not be desirable, as it will cause
 aliasing if the client area is increased in size.
如果窗口的视图区大小和SwapChain的大小不一，那么DirectX将通过Stretch Blit来自动处理图像的伸缩变化。尽管这可能并不令人期待，因为这在视图区变大的时候将导致图像的模糊。
To prevent this, you can write a handler for the WM_SIZE message of the child window.  The handler should release the existing swap chain, and create a new swap chain using the code from Step 2.
如果要解决这个问题，您需要为子窗口的WM_SIZE消息写一段处理代码，这段代码Release已经存在的Swap Chain交换链，并且使用第二步中的代码创建一个崭新的Swap Chain交换链。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/noslopforever/archive/2005/03/09/316104.aspx](http://blog.csdn.net/noslopforever/archive/2005/03/09/316104.aspx)
