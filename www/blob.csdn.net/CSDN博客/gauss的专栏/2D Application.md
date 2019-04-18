# 2D Application - gauss的专栏 - CSDN博客
2011年12月30日 00:01:47[gauss](https://me.csdn.net/mathlmx)阅读数：550标签：[application																[Application																[buffer																[BUFFER																[Buffer																[direct3d																[Direct3D																[parameters																[video](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=Direct3D&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=Buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=BUFFER&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=Application&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
**2D Application**
这一章将讨论IDirect3DDevices9接口怎样应用到简单的两维应用程序里去。然而，接下来讨论的方法和接口的方法不仅仅只应用在两维应用程序里面。
   开始我们将看看IDirect3DSurface9接口，它用于存放像素数据。我们将看看怎样创建surfaces，怎样填充数据，并且执行像素拷贝操作。
   接下来，我们讨论IDirect3DSwapChain9接口管理back buffer集合。设备创建的时候都会携带一个默认的swap chain，但是也可以为window模式下多个视图创建新的swap chain。
   再接下来，我们将讨论Presentation，Present 也是IDirect3DDevice9提供的方法。
   即使Direct3D可以不使用GDI，但是他们也要处理发送到应用程序top-level窗口的消息。我们推荐使用Direct3D应用程序来处理窗口消息。DirectX 并没有提供直接方法来将GDI和Direct3D结合起来。但是，通常是在内存DC和流水线产生的结果像素数据上执行GDI操作。
   最后，我们将讨论流水线的Video scan out部分以及swap chain从back buffer到front back 的过程。video scan out 电流读取数据，使用cursor overlay,gamma校正以及像素数据转化成monitor的模拟信号。
**Pixel Surface**
   Pixel 面是像素数据的矩形集合。像素数据的内存layout是通过D3DFORMAT定义的。在设备上使用surface有几处地方：back buffer surfaces, depth/stencil buffer surfaces，纹理层surface，render target surface 以及图片surface。Direct3D 使用IDirect3DSurface9接口表示一个surface，CreateOffscreenPlainsurface方法可以创建一个图片surface，它能存在于scrath内存池，系统内存池和设备内存池。CreateDepthStencilSurface和CreateRenderTarget分别返回depth/stencil的surface和render
 target的surface。一个plain surface可能不是3D 渲染的目标，但是你能在plain surface和其他surface之间进行数据拷贝。
   HRESULT CreateOffscreenPlainSurface(UINT width,
                                                      UINT height,
                                                      D3DFORMAT format,
                                                      D3DPOOL pool,
                                                      IDirect3DSurface9 ** result,
                                                      HANDLE * unused);
如果设备不支持被请求面的各式，CreateOffscreenPlainSurface将会失败，或者是系统内存里面没有足够的存储空间。使用CheckDeviceFormat可以用来预先检查设备是否支持某种格式的surface。
interface IDirect3DSurface9 : IDirect3DResource9
{
HRESULT GetContainer(REFIID container_iid, void ** value);
HRESULT GetDC(HDC **　value);
HRESULT GetDesc(D3DSURFACE_DESC * value);
HRESULT LockRect(D3DLOCKED_RECT *　data,  const RECT * locked_region,DWORD flags);
HRESULT Release(HDC context);
HRESULT UnlockRect();
}
   对于CreateOffscreenPlainSurface创建的surface，当container_iid是IID_Direct3DDevice9时，GetContainer将返回成功。GetDESC将返回D3DSURFACE_DESC结构体里面的像素数据的描述信息。
typedef struct _D3DSURFACE_DESC
{
D3DFORMAT Format;
D3DRESOURCETYPE Type;
DWORD Usage;
D3DPOOL Pool;
D3DMULTISAMPLE_TYPE MultiSampleType;　//与render target surface一起使用的multisampling
DWORD MultiSampleQuality;
UINT Width;
UINT Height;
} D3DSURFACE_DESC;
**Accessing Surface Pixel Data**
    为了获取surface的像素数据，我们需要使用LockRect和UnlockRect方法。当lockRect方法的locked_region不为NULL的时候，就只lock住了surface的某个局部。参数flags告诉Direct3D一旦surface被锁住，数据将怎么办。Flags包括D3DLOCK_DISCARD, D3DLOCK_DONOTWAIT, D3DLOCK_NO_DIRTY_UPDATE,D3DLOCK_NOSYSLOCK, D3DLOCK_READONLY。
     D3DLOCK_DISCARD通知runtime整个locked区域将只被写入而不读取。使用discard flag, runtime不会提供一份surface数据的copy给应用程序读取。不使用discard flag,runtime可能被迫停掉所有未决的操作，然后再返回一个surface数据的copy给应用程序。**注意：这个flag不能使用在子区域。**
     D3DLOCK_DONOTWAIT 决定当lock的时候不会阻塞住runtime，等待未决的渲染操作完成。如果lock调用已经阻塞了，它将返回D3DERR_WASSTILLDRAWING。
    Direct3D为每个**可管理的surface（注意这里只能是managed surface）**维护一个dirty 区域表，用来最小化unlock数据的拷贝量。如果已经是使用了D3D_NO_DIRTY_UPDATE，locked区域将不会影响dirty 区域list。
   使用D3DLOCK_READONLY，应用程序保证对锁定区域没有任何的写操作。
   D3DLOCK_NOSYSLOCK只只能应用到video memory的surfaces。为了阻止video存储资源被锁时设备lost，Direct3D获取系统critical section来阻止设备丢失。它也能阻塞操作系统的其他部分执行，这样势必会影响系统的交互和反应灵敏度。 D3DLOCK_NOSYSLOCK 禁止获取系统的critical section。
   LockRect 方法返回D3DLOCKED_RECT结构体，它定义了surface的像素数据。它保证了surface数据在sanline上是连续的。
   typedef struct _D3DLOCKED_RECT
{
  int  pitch;  //定义了相邻两个scanlines的距离。
  void *pBits; //像素数据的指针。
}D3DLOCKED_RECT;
  当遍历surface的每个像素时，pitch 和像素的大小是很重要的。像素数据的大小是跟D3DFORMAT有关。
IDirect3DSurface9并没有提供方法用于从图片装载到surface,或者像素格式转换，D3DX却提供了很多这样的操作方法。
**Using GDI On a Surface**
GDI提供GetDC和ReleaseDC方法来操作与GDI兼容格式的surface。与GDI兼容格式包括D3DFMT_R5G6B5,D3DFMT_X1R5G5B5,D3DFMT_R8G8B8和D3DFMT_X8R8G8B8。
GDI device context可以使用GDI执行基于surface的渲染操作,然后立刻释放。一旦device context被创建，就会在Direct3D runtime里面建立一个lock。 这个lock保证runtime并不直接跟GDI rendering交互。因为这个lock的存在，应用程序应该尽可能快的释放GDI device context。而且， 只有device context释放以后，下面表里的方法才能调用。
|Interface|Method|
|----|----|
|IDirect3DCubeTexture9|LockRect|
|IDirect3DDevice9|ColorFill|
||Present|
||StretchRect|
||UpdateSurface|
||UpdateTexture|
|IDirect3DSurface9|LockRect|
|IDirect3DSwapChain9|Present|
|IDirect3DTexture9|LockRect|
**Swap Chain**
   每个设备都包含一组默认的swap chains。GetNumberOfSwapChains返回设备swap chains的数目。GetSwapChain方法返回默认swap chain集合里的swap chain的接口。
   默认swap chain集合的属性定义在D3DPRESENT_PARAMETERS。Swap chain由1个，2个，3个back buffer surface 和一个front buffer surface。front buffer surface不能直接访问，但是它仍然参与到swap chain的presentation过程中。 当调用Present时back buffer surface显示在monitor。
  exclusive 模式的设备使用它默认的back buffer。window模式的设备可以使用多个swap chain，每个都可以将渲染结果绘制它自己的窗口。exclusive模式的adapter group 设备也可以渲染到多个monitor。
  CreateAdditionalSwapChain创建一个基于D3DPRESENT_PARAMETERS的swap chain，然后返回一个IDirect3DSwapChain9接口。**注意一个Swap chain只能包含back buffer surface而不是depth/stencil surface。**
  HRESULT CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS * params, IDirect3DSwapChain9 ** result);
interface IDirect3DSwapChain9 : IUnknown
{
     HRESULT GetBackBuffer(UINT buffer, D3DBACKBUFFER_TYPE kind, IDirect3DSurface9 ** value);
     HRESULT GetDevice(IDirect3DDevice9  ** value);
     HRESULT GetDisplayMode(D3DDISPLAYMODE * value);
    HRESULT GetFrontBufferData(IDirect3DSurface9 * destination);
    HRESULT  GetPresentParameters(D3DPRESENT_PARAMETERS * value);
    HRESULT  GetRasterStatus(D3DRASTER_STATUS * value);
    HRESULT Present(CONST RECT * source, CONST RECT * destination, HWND override, CONST RGNDATA * dirty_region, DWORD flags);
}
    GetBackBuffer返回一个back buffer surface的接口指针。back buffer序号以0开始，present首先显示是back buffer 0里面的内容，然后再是back buffer 1， 依此下去...。 D3DBACKBUFFER_TYPE定义了back buffer的类型。DirecX 9.0c 并不支持stereo 渲染，，kind只能是D3DBACKBUFFERTYPE_MONO.
typedef enum _D3DBACKBUFFER_TYPE
{
   D3DBACKBUFFER_TYPE_MONO = 0,
   D3DBACKBUFFER_TYPE_LEFT = 1,
   D3DBACKBUFFER_TYPE_RIGHT = 2
}D3DBACKBUFFER_TYPE;
    Present跟device的Present方法一样，都执行同样的功能。flag的值包括D3DPRESENT_DONOTWAIT和D3DPRESENT_LINEAR_CONTENT。D3DPRESENT_DONOTWAIT表示如果presentation引起应用程序阻塞，它将立刻返回D3DERR_WASSTILLDRAWING。 D3DPRESENT_LINEAR_CONTENT 告诉设备presentation时源区域像素应该从线性颜色空间转化到sRGB颜色空间。
**Presentation**
   当调用了Present后，back buffer的内容将会被复制到front buffer，video scan out 电流读取front buffer的像素把他们显示在monitor上。如果D3DCAPS9::DevCaps的D3DDEVCAPS_CANRENDERAFTERFLIP被设置，present发生以后设备将继续queue渲染命令，也就是在设备与CPU之间有更多的并发，当前帧渲染的时候，下一帧已经queue。但是一个设备queue的帧数最多不能超过两帧。
  #define D3DDEVCAPS_CANRENDERAFTERFLIP 0x000000800L
   HRESULT Present(const RECT * source_rect, const RECT* dest_rect,HWND override_window, const RGNDATA * dirty_region);
   Present对Swap chain的行为使用D3DPRESENT_PARAMETERS的SwapEffect定义的。
   typedef enum _D3DSWAPEFFECT
{
    D3DSWAPEFFECT_DISCARD = 1,
    D3DSWAPEFFECT_FLIP  = 2,
    D3DSWAPEFFECT_COPY = 3
} D3DSWAPEFFECT;
  在window模式下面，所有的swap effec都是通过拷贝操作实现的。使用最近时间间隔创建的swap chain并不能让monitor的垂直扫描与拷贝操作同步。拷贝操作运行在在Video scan out过程中，这样导致了artifacts，也称作图像tearing。为了避免这种artifacts，我们需要同步video scan out与拷贝操作，以便于当video beam还停留在拷贝操作的目的地时，不能有拷贝操作发生。如果显卡并不支持video beam 位置信息，拷贝操作将会立刻发生。
  在exclusive模式下，presentation的频率是由D3DPRESENT_PARAMETERS的FullScreen_PresentationInterval指定的。默认的presentation inverval 对应adapter视频模式的刷新率。
  如果dest_window不是空，dest_window客户端区域就是present的Target。如果dest_window空而D3DPRESENT_PARAMETERS的hDeviceWindow不空，则hDeviceWindow就是present的Target。 如果两者都空，CreateDevice的focus_window参数将是present的target。
source和dest只能应用到拷贝操作方式，Flip 和Discard这两个参数必须是空。空值表示整个source surface 和目的surface。dirty_region也只能使用在拷贝操作下。
**Lost Devices and Reset**
   TestCooperativelevel检查设备的状态，Reset重启设备。
HRESULT Reset(D3DPRESENT_PARAMETERS * params);
HRESULT TestCooperativeLevel();
**Video Scan out**
    front buffer的内容来自于Present。GetDisplayMode返回了当前front buffer的显示模式。应用程序不能直接访问front buffer，但是可以通过GetFrontBufferData返回一份front buffer的拷贝。GetFrontBufferData的destination参数必须是一个存在的surface，它的维度等于adapter当前显示模式，并且像素格式是D3DFMT_A8R8G8B8。在拷贝过程中数据将从adapter的显示模式格式转化成surface格式。
    HRESULT GetDisplayMode(D3DDISPLAYMODE * value);
    HRESULT GetFrontBufferData(IDirect3DSurface* destination);
   如果D3DCAPS::Caps的D3DCAPS_READ_SCANLINE位被设置，设备将它的video scan out scanline和垂直blank状态。
   GetRasterStatus将使用D3DRASTER_STATUS结构体返回video scan out 状态。scanline 成员指定了rater beam的当前位置，0是最顶端的scanline。如果是自下而上的垂直扫描，InVBlank成员将为TRUE。
  HRESULT GetRasterStatus(D3DRASTER_STATUS * value);
   typedef struct _D3DRASTER_STATUS
{
  BOOL　InVBlank;
  UINT    Scanline;
} D3DRASTER_STATUS;
·            Cursor
    在exclusive模式下，Direct3D管理着cursor的显示。在video scan out的过程中硬件Cursor可能会取代cursor图像。如果硬件cursor不可用，runtime会提供一个软件cursor对front buffer读，修改，写操作。在window模式下，应用程序可能会使用GDI cursor 或者Direct3D cursor。Direct3D cursor 能通过方法ShowCursor显示或者隐藏cursor。ShowCursor并不返回HRESULT,而是cursor的上一个隐藏状态。如果它是TRUE,cursor在调用ShowCursor之前是可见的。
    BOOL　ShowCursor(BOOL Show);
     void    SetCursorPosition(UINT x, UINT y, DWORD flags);
    HRESULT  SetCursorProperties(UINT hot_spot_x, UINT hot_spot_y, IDirect3DSurface9 * image);
   cursor的位置是通过SetCursorPosition设定的，flags参数
·            Gamma Ramp
    在exclusive模式下，应用cursor以后， 在进行A/D转换之前将会对像素数据应用Gamma校正。在窗口模式下，应用程序也能使用GDI来做Gamma校正。如果D3DCAPS9::Caps的D3DCAPS2_FULLSCREENGAMMA设置，设备将会支持exclusive的Gamma Ramp。
    GetGammaRamp能读取Gamma Ramp的属性，它返回一个Gamma Ramp的结构体。
void GetGammaRamp(D3DGAMMARAMP * value);
void SetGammaRamp(DWORD Flags, const D3DGAMMARAMP * value);
typedef struct _D3DGAMMARAMP
{
    WORD red[256];
    WORD green[256];
    WORD  blue[256];
} D3DGAMARAMP;
    Gamma ramp属性通过SetGammaRamp进行设置，并且它能够立刻产生反应。Flags参数表示设备是否使用ramp的calibration，它包括D3DSGR_NO_CALIBRATION和D3DSGR_CALIBRATE。
     如果D3DCAPS9::Caps2的D3DCAPS2_CANCALIBRATEGAMMA位设置，设备将能够给Gamma Ramp指定一个calibration。
     下面例子显示了怎样计算ramp values。
void compute_ramp(D3DGAMARAMP & ramp, float gamma)
{
  for (UINT i = 0;i< 256;i++)
  {
    const  WORD val = static_cast<int> (65535 * pow(i/255.f, 1.f/gamma));
    ramp.red[i] = val;
    ramp.green[i] = val;
    ramp.blue[i] = val;
  }
}
**2D Pixel Copies**
    如果我们申请可锁住的back buffer，我们能锁住back buffer的矩形区域，然后使用软件直接对它读写操作。然而，back buffer surface是video memory的设备surface。通过CPU访问video memory将是一个昂贵的操作，我们应该避免。 但是存放系统内存池或者scratch内存池的图像surface能轻松的被CPU访问。
    拷贝像素操作一般包括：设备内存到设备内存， 系统内存到设备内存，设备内存到系统内存。StetchRect 能有效的进行设备内存之前的拷贝。UpdateSurface和UpdateTexture一般都是将数据从系统内存转移到设备内存里面。GetRenderTargetData用于从设备内存将像素数据转移到系统内存。
   当你想要在设备资源直接拷贝数据时，你可以使用StretchRect。当你需要更新默认内存池的某个资源（如图像surface或者纹理资源），你可以使用UpdateSurface和updateTexture，不过managed池里的资源会自动更新，当它的系统内存的数据变化时，它会自动更新到设备内存里面去）。
·            Pixel Copies Within Device Memory
    StretchRect能够将一个矩形区域的像素从设备内存的一个surface转移到另一个上面去。这两个surface通常会是同样的D3DFORMAT。在拷贝过程中，StretchRect也能执行有限的几张颜色转换。
    HRESULT StretchRect(IDirect3DSurface9 * source , const RECT * source_rect, IDirect3DSurface9 * destination, const RECT * dest_rect, D3DTEXTUREFILTERTYPE filter);
    当source_rect非空，整个source 面将会拷贝到目的surface。当source_rect非空，它指向source面的一个子区域。dest_rect也是一样。 当需要调整源像素区域大小来适应目的像素区域的时候，filter参数将指定调整的方式，它的值包括D3DTEXF_NONE, D3DTEXF_POINT,D3DTEXF_LINEAR。当缩小和放大源像素区域的时候，point和linear的过滤将会支持。D3DCAPS9结构的StretchRectFilterCaps各个位标记描述了StretchRect的过滤支持情况。
    #define D3DPTFILTERRCAPS_MAGFLINEAR 0x02000000L
   #define  D3DPTFILTERCAPS_MAGFPOINT  0x01000000L
   #define  D3DPTFILTERCAPS_MINFLINEAR  0x00000200L
   #define  D3DPTFILTERCAPS_MINFPOINT   0x00000100L
  如果D3DCAPS9::DevCaps的D3DDEVCAPS_CANBLTSYSTONONLOCAL设置，StretchRect能支持从系统内存到non-local视频存储（如AGP内存）转移数据。
设备像素拷贝的格式转换
   StretchRect也能支持拷贝过程中的颜色转换操作。支持的转换可以是高性能的YUV surface格式到高性能的RGB surface格式。具体支持什么样的格式转换，你可以通过CheckDeviceFormatConversion方法来查看。
   HRESULT CheckDeviceFormatConversion(UINT adapter, D3DDEVTYPE device_kind, D3DFORMAT source_fmt, D3DFORMAT target_fmt);
   source_fmt可能是FOURCC格式也可能是合理的back buffer格式。 Target格式将是下面中的一种：D3DFMT_A1R5G5B5  , D3DFMT_A8R8G8B8, D3DFMT_A16B16G16R16, D3DFMT_X1R5G5B5, D3DFMT_A8R8G8B8, D3DFMT_A16B16G16R16, D3DFMT_R5G6B5, D3DFMT_X8B8G8R8, D3DFMT_A32B32G32R32F,
D3DFMT_R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_A2R10G10B10, D3DFMT_A2B10G10R10。
设备像素拷贝限制：
     因为StretchRect直接工作在设备内存上，它将面临很多限制。
     restrictions: 1. source和destination不能是相同的surface。 2. 不能从render target surface 到off screen surface。 3. 不能基于压缩格式stretch。 4.D3DDEVCAPS2_CAN_STRETCH_RECT_FROM_TEXTURES ，如果source是纹理surface。
    depth/stencil restrictions:1. 不能是纹理，2. 不能discardable；3. 必须是整个surface拷贝；4. source和destination必须是同样的尺寸。5.不支持filtering。6. 不能从一个场景里面被调用。
    downsampling multisample render target: 1. 创建multisample render target；2. 创建同样大小的non-multismapled render target； 3. 从non-MS RT拷贝到MS RT。
如下是source和destination的结合情况：
|**DX8 Driver no stretching**|||||
|----|----|----|----|----|
|Source|Destination||||
||Texture|RT Texture|RT|Off-screen Plain|
|Texture|NO|NO|NO|NO|
|RT Texture|NO|Yes|Yes|No|
|RT|NO|Yes|Yes|No|
|Off-screen Plain|Yes|Yes|Yes|Yes|
|**DX8 Driver no stretching**|||||
||Texture|RT Texture|RT|Off-screen Plain|
|Texture|No|No|No|No|
|RT Texture|No|No|No|No|
|RT|No|Yes|Yes|No|
|Off-screen Plain|No|Yes|Yes|No|
|**DX9 Driver no stretching**|||||
||Texture|RT Texture|RT|Off-screen Plain|
|Texture|No|Yes|Yes|No|
|RT Texture|No|Yes|Yes|No|
|RT|No|Yes|Yes|No|
|Off-screen Plain|No|Yes|Yes|Yes|
|**DX9 Driver  stretching**|||||
||Texture|RT Texture|RT|Off-screen Plain|
|Texture|No|Yes|Yes|No|
|RT Texture|No|Yes|Yes|No|
|RT|No|Yes|Yes|No|
|Off-screen Plain|No|Yes|Yes|No|
·            Copies From System Memory To Device Memory
    通常你lock一个surface,然后直接使用CPU为它填充数据。但是并且每个surface都是lockable。设备内存里面的surface通常是不经常lock的，因为lock住以后访问速度是非常慢的。所以我们通常的办法是先使用CPU update系统内存的surface，然后使用updateSurface或者UpdateTexture将他们传给设备内存。
   HRESULT UpdateSurface(IDirect3DSurface9 * source, const RECT * source_rect, IDirect3DSurface9 * destination, CONST POINT * offset);
   offset参数指定了目的区域对应source 区域左上角像素的位置偏移。
   如果source或者destination的surface用于multisampling或者depth/stencil ，UpdateSurface也将失败。如下表是updateSurface的source和destinationa的结合情况。
|Source|Destination||||
|----|----|----|----|----|
||Texture|RT Texture|RT|Off-screen Plain|
|Texture|YES|YES|YES|YES|
|RT Texture|NO|NO|NO|No|
|RT|NO|NO|NO|No|
|Off-screen Plain|Yes|Yes|Yes|Yes|
    UpdateTexture与UpdateSurface相似，但是它对于整个纹理资源而不是单个surface。runtime 将维护一个dirty区域。
   HRESULT UpdateTexture(IDirect3DBaseTexture9 * source , IDirect3DBaseTexture9 * destination);
   当UpdateTexture被调用，所有累计的dirty区域将会值为level 0的纹理区域。对于mipmapped的纹理，每个mip level对应的区域同样也要考虑dirty。 source的levels不能小于destination level数，并且level 0的大小也必须相同。  
·            Copies From Device Memory To System Memory
    只有两种方法从设备里面读回被渲染的图片：一种是创建设备的时候允许锁住back buffer，另外一种是 GetRenderTargetData。锁住back buffer一般比较慢。
HRESULT GetRenderTargetData(IDirect3DSurface9 * source, IDirect3DSurface9 * destination);
    如果source 是multisamepled或者不是render target，它将会失败。
**Filling Rectangles**
     如果你想直接用某种solid的颜色填充surface的某个矩形区域，你可能直接使用colorFill方法，而不是先锁住再使用CPU填充。
   HRESULT  ColorFill(IDirect3DSurface9 * destination, CONST RECT * region, D3DCOLOR color);
**Windows Messages**
   在执行过程中CreateDevice和Reset方法可能会产生窗口消息。应用程序不应该在这些方法的执行过程调用设备方法来处理这些消息。 只有设备窗口完全构造完成后，设备才有方法调用。
|WM_ACTIVATEAPP|当active发生变化时. 中止或者恢复窗口重画。|
|----|----|
|WM_CLOSE|发送出去，标记应用终止。释放设备上所有的对象，释放设备然后退出。|
|WM_COMPACTING|标记当前内存很低。|
|WM_CONTEXTMENU|当用户点击context button|
|WM_CREATE|发送到一个被创建的窗口。WM_CREATE一般是在CreateWindow完成之前就发送出去了。你不应该在它里面创建设备，设备必须至少在::CreateWindow完成之后再创建。|
|WM_DISPLAYCHNANGE|当桌面分辨率改变的时候。设备可能会reshape,你需要reshape swap chain。|
|WM_ENTERMENULOOP|当进入一个modal菜单。暂停redraw.|
|WM_ENTERSIZEMOVE|当开始一个窗口size 和移动操作时发送。|
|WM_ERASEBKGND|当窗口北京需要删除是发送。|
|WM_EXITMENULOOP|当退出modal菜单是发送。恢复连续的redraw|
|WM_EXITSIZEMOVE|当窗口的size 和move操作完成，reshape swap chain|
|WM_GETMINMAXINFO|当窗口的位置和大小信息将要变化的时候发送|
|WM_MOUSEMOVE|鼠标移动。|
|WM_NCHITTEST|当鼠标移动或者鼠标pressed或者release的时候发送。|
|WM_PAINT|发送重画窗口damaged的部分。|
|WM_POWERBROADCAST|电源管理事件|
|WM_SETCURSOR|设置一个cursor。|
|WM_SHOWWINDOW|当窗口将要显示或者隐藏的时候|
|WM_SIZE|当窗口大小改变时|
|WM_SIZING|当窗口正在resize是发送。|
|WM_SYSCOMMAND|在系统命令下发送。|
