# DirectX9.0 入门手册(2) - 深之JohnChen的专栏 - CSDN博客

2005年12月19日 14:36:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2111标签：[parameters																[direct3d																[null																[float																[struct																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)
个人分类：[游戏空间](https://blog.csdn.net/byxdaz/article/category/160195)


·创建IDirect3D接口



DirectX是一组COM组件，COM是一种二进制标准，每一个COM里面提供了至少一个接口，而接口就是一组相关的函数，我们使用DirectX，其实就是使用那些函数。COM和C++中的类有点像，只不过COM使用自己的方法来创建实例。创建COM实例的一般方法是使用coCreateInstance函数。有关coCreateInstance的使用方法，可以参考有关COM方面的资料，这里暂时不详细说明了，因为DirectX提供了更简洁的方法来创建DirectX组件的实例。这一章我要讲的就是Direct3D组件的使用方法。



为了使用D3D中的函数，我们得先定义一个指向IDirect3D9这个接口的指针，顺便说明一下，其实接口也是一个指针，所以我们定义的就是一个指向指针的指针，也即二重指针，为什么要使用二重指针呢，我暂时还不是很懂，所以先留着这个疑问吧^_^。定义完这个接口指针后，例如IDirect3D9 *g_pD3D;现在我们使用Direct3DCreate9这个函数来创建一个D3D接口：



g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ); 



Direct3DCreate9这个函数只有一个参数，它表明要创建接口的版本。如果你想创建一个老的接口版本当然也可以，不过没有人会那样做吧。



创建接口后就可以创建D3D设备了，什么是D3D设备？你可以想象为你机上的那块显卡！什么？你有几块显卡！！没关系，那就创建多几个D3D设备接口吧。创建D3D设备需要的参数很多，如果把那些参数都挤在一个函数里面，那就太长了，所以就把一些参数放进结构体里面，只要先设定好这些结构体，再把这些结构体当作参数传给创建D3D设备的函数，那就清晰多了。首先要讲的就是D3DPRESENT_PARAMETERS这个结构。下面是它的定义：















**BackBufferWidth****和****BackBufferHeight****：**后备缓冲的宽度和高度。在全屏模式下，这两者的值必需符合显卡所支持的分辨率。例如（800，600），（640，480）。



**BackBufferFormat****：**后备缓冲的格式。这个参数是一个D3DFORMAT枚举类型，它的值有很多种，例如D3DFMT_R5G6B5，这说明后备缓冲的格式是每个像素16位，其实红色（R）占5位，绿色（G）占6位，蓝色（B）占5位，为什么绿色会多一位呢？据说是因为人的眼睛对绿色比较敏感。DX9只支持16位和32位的后备缓冲格式，24位并不支持。如果对这D3DFORMAT不熟悉的话，可以把它设为D3DFMT_UNKNOWN，这时候它将使用桌面的格式。



**BackBufferCount****：**后备缓冲的数目，范围是从0到3，如果为0，那就当成1来处理。大多数情况我们只使用一个后备缓冲。使用多个后备缓冲可以使画面很流畅，但是却会造成输入设备响应过慢，还会消耗很多内存。



**MultiSampleType****和****MultiSampleQuality****：**这两个参数可以使你的渲染场景变得更好看，但是却消耗你很多内存资源，而且，并不是所有的显卡都支持这两者的所设定的功能的。在这里我们分别把它们设为D3DMULTISAMPLE_NONE和0。



**SwapEffect****：**交换缓冲支持的效果类型。它是D3DSWAPEFFECT枚举类型，可以设定为以下三者之一：D3DSWAPEFFECT_DISCARD，D3DSWAPEFFECT_FLIP，D3DSWAPEFFECT_COPY。如果设定为D3DSWAPEFFECT_DISCARD，则后备缓冲区的东西被复制到屏幕上后，后备缓冲区的东西就没有什么用了，可以丢弃（discard）了。如果设定为D3DSWAPEFFECT_FLIP，则表示在显示和后备缓冲之间进行周期循环。设定D3DSWAPEFFECT_COPY的话，我也不太清楚有什么作用*^_^*。一般我们是把这个参数设为D3DSWAPEFFECT_DISCARD。



**hDeviceWindow****：**显示设备输出窗口的句柄



**Windowed****：**如果为FALSE，表示要渲染全屏。如果为TRUE，表示要渲染窗口。渲染全屏的时候，BackBufferWidth和BackBufferHeight的值就得符合显示模式中所设定的值。



**EnableAutoDepthStencil****：**如果要使用Z缓冲，则把它设为TRUE。



**AutoDepthStencilFormat****：**如果不使用深度缓冲，那么这个参数将没有用。如果启动了深度缓冲，那么这个参数将为深度缓冲设定缓冲格式（和设定后备缓冲的格式差不多）



**Flags****：**可以设置为0或D3DPRESENTFLAG_LOCKABLE_BACKBUFFER。不太清楚是用来做什么的，看字面好像是一个能否锁定后备缓冲区的标记。



**FullScreen_RefreshRateInHz****：**显示器的刷新率，单位是HZ，如果设定了一个显示器不支持的刷新率，将会不能创建设备或发出警告信息。为了方便，一般设为D3DPRESENT_RATE_DEFAULT就行了。



**PresentationInterval****：**如果设置为D3DPRENSENT_INTERVAL_DEFAULT，则说明在显示一个渲染画面的时候必要等候显示器刷新完一次屏幕。例如你的显示器刷新率设为80HZ的话，则一秒内你最多可以显示80个渲染画面。另外你也可以设置在显示器刷新一次屏幕的时间内显示1到4个画面。如果设置为D3DPRENSENT_INTERVAL_IMMEDIATE，则表示可以以即时的方式来显示渲染画面，虽然这样可以提高帧速（FPS），但是却会产生图像撕裂的情况。



·创建IDirect3DDevice接口



当你把D3DPRESENT_PARAMETERS的参数都设置好后，就可以创建一个D3D设备了，和创建D3D接口一样，先定义一个接口指针IDirect3DDevice9 * g_pD3DDevice;然后使用D3D接口里面的CreateDevice函数来创建设备。CreateDevice的声明为：



HRESULT CreatDevice( 



UINT Adapter, 



D3DDEVTYPE DeviceType, 



HWND hFocusWindow, 



DWORD BehaviorFlags, 



D3DPRESENT_PARAMETERS *pPresentationParameters, 



IDirect3DDevice9** ppReturnedDeviceInterface 



}; 



第一个参数说明要为哪个设备创建设备指针，我之前说过一台机可以有好几个显卡，这个参数就是要指明为哪块显卡创建可以代表它的设备指针。但是我怎么知道显卡的编号呢？可以使用D3D接口里面的函数来获得，例如GetAdapterCounter可以知道系统有几块显卡；GetAdapterIdentifier可以知道显卡的具体属性。一般我们设这个参数为D3DADAPTER_DEFAULT。



第二个参数指明正在使用设备类型。一般设为D3DEVTYPE_HAL。



第三个参数指明要渲染的窗口。如果为全屏模式，则一定要设为主窗口。



第四个参数是一些标记，可以指定用什么方式来处理顶点。



第五个参数就要用到上面所讲的D3DPRESENT_PARAMETERS。



第六个参数是返回的接口指针。



·开始渲染



有了设备接口指针，就可以开始渲染画面了。渲染是一个连续不断的过程，所以必定要在一个循环中完成，没错，就是第一章讲的那个消息循环。在渲染开始之前我们要用IDirect3DDevice9::Clear函数来清除后备缓冲区。











**Count****：**说明你要清空的矩形数目。如果要清空的是整个客户区窗口，则设为0；



**pRects****：**这是一个D3DRECT结构体的一个数组，如果count中设为5，则这个数组中就得有5个元素。



**Flags****：**一些标记组合。只有三种标记：D3DCLEAR_STENCIL , D3DCLEAR_TARGET , D3DCLEAR_ZBUFFER。



**Color****：**清除目标区域所使用的颜色。



**float****：**设置Z缓冲的Z初始值。小于或等于这个Z初始值的Z值才会被改写，但它的值只能取0到1之间。如果还不清楚什么是Z缓冲的话，可以自己找相关资料看一下，这里不介绍了，呵呵。



**Stencil****：**设置模板缓冲的初始值。它的取值范围是0到2的n次方减1。其中n是模板缓冲的深度。



清除后备缓冲区后，就可以对它进行渲染了。渲染完毕，使用Present函数来把后备缓冲区的内容显示到屏幕上。










**pSourceRect****：**你想要显示的后备缓冲区的一个矩形区域。设为NULL则表示要把整个后备缓冲区的内容都显示。



**pDestRect****：**表示一个显示区域。设为NULL表示整个客户显示区。



**hDestWindowOverride****：**你可以通过它来把显示的内容显示到不同的窗口去。设为NULL则表示显示到主窗口。



**pDirtyRegion****：**高级使用。一般设为NULL。



