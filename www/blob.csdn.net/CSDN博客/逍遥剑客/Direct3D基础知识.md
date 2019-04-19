# Direct3D基础知识 - 逍遥剑客 - CSDN博客
2007年12月26日 11:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3680

重新从头开始学习DX,以前太急于求成了,很多基础知识都没掌握就开始写程序了,结果出了问题很难解决.
**1.****D3D****体系结构**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/D3D_System_Integration.JPG)
D3D与GDI处与同一层次,区别在于,D3D可以使用HAL(Hardware Abstraction Layer)通过DDI来访问图形硬件,充分发挥硬件性能.
**2.****D3D****设备对象**
**2.1 D3D****设备类型**
        HAL,支持硬件加速光栅化和硬件或软件处理
        Software Device,硬件模拟层
        REF,reference device,SDK专用.不过nVidia的PerfHUD就是用的这种设备类型
**2.2****D3DPRESENT_PARAMETERS**
1: **typedef****struct** D3DPRESENT_PARAMETERS **{** 2:     UINT BackBufferWidth**,** BackBufferHeight**;***//**后台缓冲区宽高* 3:     D3DFORMAT BackBufferFormat**;***//**后台缓冲区像素格式* 4:     UINT BackBufferCount**;***//**后台缓冲区的数量* 5:     D3DMULTISAMPLE_TYPE MultiSampleType**;***//**多重采样类型,用于反锯齿* 6:     DWORD MultiSampleQuality**;** 7:     D3DSWAPEFFECT SwapEffect**;***//**后台缓冲区复制到前台缓冲区的方式* 8:     HWND hDeviceWindow**;***//**图形绘制窗口* 9:     BOOL Windowed**;***//**窗口/全屏*10:     BOOL EnableAutoDepthStencil**;**11:     D3DFORMAT AutoDepthStencilFormat**;**12:     DWORD Flags**;**13:    UINT FullScreen_RefreshRateInHz**;***//**刷新速率,窗口时必须为0*14:     UINT PresentationInterval**;**
15: **}** D3DPRESENT_PARAMETERS**,***LPD3DPRESENT_PARAMETERS**;**
BackBufferWidth与BackBufferHeight为0时系统自动使用当前窗口客户区宽高
BackBufferCount取0和1时都表示一个后台缓冲区
hDeviceWindow为NULL时默认为当前被激活的窗口
BackBufferFormat取D3DFMT_UNKNOWN时,像素格式取当前显卡的像素格式
**2.3****CreateDevice****中的BehaviorFlag**
**D3DCREATE_HARDWARE_VERTEXPROCESSING,**硬件顶点运算
**D3DCREATE_MIXED_VERTEXPROCESSING,**软硬件共同进行顶点运算
**D3DCREATE_MULTITHREADED,**多线程绘制,会降低性能
**D3DCREATE_SOFTWARE_VERTEXPROCESSING,**软件顶点运算
通常某些集成显卡不支持硬件顶点运算,如intel 的GMA900/950
**2.4****D3D****绘制**
Clear(),可以清空颜色缓冲区,深度缓冲区,模板缓冲区.可以选择清除区域
BeginScene()和EndScene()不允许嵌套
Present(),可以指定更新区域(脏矩形技术?),不过要求用D3DSWAPEFFECT_COPY
**3.****D3D****设备对象**
**全屏模式:**
枚举显示适配器(多显示器适用)
               IDirect3D9::EnumAdpterModes()枚举支持的显示模式
               IDirect3D9::CheckDeviceType()检测显示模式是否支持硬件加速
               IDirect3D9::GetDeviceCaps()检查渲染设备是否支持所需要的功能(显卡兼容性检测)
               IDirect3D9::CheckDeviceFormat()检测其它表面格式是否支持
               IDirect3D9::CheckDepthStencilMatch()检测深度/模板缓冲区是否和渲染目标兼容
               IDirect3D9::CheckDeviceMultiSampleType()检查设备是否支持多重采样(反锯齿用)
**窗口模式:**
直接用D3DADAPTER_DEFAULT就可以忽略前两步了,剩下的都一样
**设备丢失:**
图形显示丢失焦点,必须重新创建相关资源
任何显存资源在设备丢失时必须销毁,包括CreateAdditionalSwapChain()创建的交换链和D3DPOOL_DEFAULT格式的内存资源
**4.****D3D****表面**
表面格式:通常有两个属性,表面大小与颜色位数
表面翻转:将后台缓冲区移动到前台缓冲区的过程
交换链:按顺序逐个提交到前台显示的多个后台缓冲区的集合
多个视图的应用程序用多个交换链
访问表面内存:IDirect3DSurface9::LockRect(),不适用于多重采样后台缓冲区
