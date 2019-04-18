# 初识Direct3D - gauss的专栏 - CSDN博客
2011年12月13日 22:29:34[gauss](https://me.csdn.net/mathlmx)阅读数：776
**第一卷 朦胧的3D世界**
第一集 初识Direct3D
简介
我们通过2个例子来简单的认识3D
**1.1 接口和数据结构**
我们首先来看看我们以后用的比较多的接口,
a. IDirect3D9
b. IDirect3DDevice9
c. IDirect3DVertexBuffer9
d. IDirect3DIndexBuffer9
e. IDirect3DSurface9
f. IDirect3DTexture9
g. ID3DXMesh
再看看我们以后用的比较多的数据结构,
a. D3DDISPLAYMODE
b. D3DPRESENT_PARAMETERS
c. D3DXMATRIX
d. D3DLIGHT9
e. D3DMATERIAL9
我们现在不解释它们的用途, 在接下来的很多地方会用到的, 之后大家就会自己掌握了.
**1.2 第一个例子**
1.2.1万事总有开头的地方, 这里我们就简单的实现一个.
假设我现在是个画家, 那么画家的画是怎么出来的呢?
a. 需要一个工作的地方, 也就是工作室.
   Direct3D也需要工作的地方, 这个地方称为 -- IDirect3D9, 如同画家的工作室.
b. 我是个全能的画家, 能画油画, 水墨画..., 每种风格的画派都有它独特的工作的办公桌在工作室里;
   IDirect3D9这个工作室也可能是多才多艺的, 也就是说它可能也有很多不同的办公桌 -- IDirect3DDevice9,
   IDirect3DDevice9的多少其实可等同与你的主板上有多少块能被使用的显卡, 一般有只有一块;
   所以一般的工作室(IDirect3D9)只有一张特定的办公桌(IDirect3DDevice9), 这时的IDirect3DDevice9可称谓默认的adapter.
c. 画不是画在办公桌上的, 而是办公桌上的画纸上的, 同样有很多相同的画纸, 也有很多不同的画纸.
   IDirect3DDevice9办公桌上的画纸是 -- IDirect3DSurface9; 现在的IDirect3DDevice9一般都会默认为你准备1张画纸
   (IDirect3DSurface9), 全屏模式下是最少2张.
1.2.2 好的, 现在我这个天才画家要开始画画了,
首先得创建一个Windows SDK 程序, 因为所有的Windows游戏是通过Windows的窗口消息循环来工作的.
Visual Studio 200X -->File --> Project... --> Win32 Project, 选默认的生成.
我比较喜欢自己的SDK风格, 所以对默认的生成进行了少许的改动, 以后的例子都使用同样的框架.
暂定为game0, 我们来看看game0的主要代码(下载game0 project),
-------------------------------------------------------------------------
#include "stdafx.h" 
#include "game0.h"
// Global Variables:  
LPDIRECT3D9             g_pD3D        = NULL;
LPDIRECT3DDEVICE9       g_pD3DDevice  = NULL;
LPCTSTR                 szClass       = _T("Game0");
LPCTSTR                 szTitle       = _T("Demo0");
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE);
HRESULT             Init3D(HWND);
INT                 MsgLoop();
VOID                Render();
VOID                Term3D();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
// WinMain
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
   int nRet = 0;
   MyRegisterClass(hInstance);
   if (InitInstance(hInstance)) 
   {
        nRet = MsgLoop();
   }
   Term3D();
   UnregisterClass(szClass, hInstance);
   return nRet;
}
// MyRegisterClass
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX); 
   wcex.style         = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc   = (WNDPROC)WndProc;
   wcex.cbClsExtra    = 0;
   wcex.cbWndExtra    = 0;
   wcex.hInstance     = hInstance;
   wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
   wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName  = NULL;
   wcex.lpszClassName = szClass;
   wcex.hIconSm       = wcex.hIcon;
   return RegisterClassEx(&wcex);
}
// InitInstance
BOOL InitInstance(HINSTANCE hInstance)
{
   HWND hWnd = CreateWindow(szClass, szTitle, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
                            NULL, NULL, hInstance, NULL);
   if (hWnd == NULL)
   {
      return FALSE;
   }
   if (FAILED(Init3D(hWnd)))
   {
      DestroyWindow(hWnd);
      return FALSE;
   }
   ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);
   return TRUE;
}
// Init3D
HRESULT Init3D(HWND hWnd)
{
   // 1. 
   g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
   if (g_pD3D == NULL)
   {
        return E_FAIL;
   }
   // 2.
   D3DDISPLAYMODE d3ddm;
   FillMemory(&d3ddm, sizeof(D3DDISPLAYMODE), 0);
   if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
   {
        return E_FAIL;
   }
   // 3.
   D3DPRESENT_PARAMETERS d3dpm;
   FillMemory(&d3dpm, sizeof(D3DPRESENT_PARAMETERS), 0);
   d3dpm.Windowed         = TRUE;
   d3dpm.SwapEffect       = D3DSWAPEFFECT_DISCARD;
   d3dpm.BackBufferFormat = d3ddm.Format;
   if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                                   D3DDEVTYPE_HAL, 
                                   hWnd, 
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                   &d3dpm, 
                                   &g_pD3DDevice)))
   {
        return E_FAIL;
   }
   if (g_pD3DDevice == NULL)
   {
        return E_FAIL;
   }
   return S_OK;
}
// MsgLoop
INT MsgLoop()
{
   BOOL bMsg = FALSE;
   MSG  msg;
   PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
   while (msg.message != WM_QUIT)
   {
        bMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if (bMsg)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
        }
   }
   return (INT)msg.wParam;
}
// Render
VOID Render()
{
   g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0);
   g_pD3DDevice->BeginScene();
   g_pD3DDevice->EndScene();
   g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
// Term3D
VOID Term3D()
{
   if (g_pD3DDevice != NULL)
   {
        g_pD3DDevice->Release();
        g_pD3DDevice = NULL;
   }
   if (g_pD3D != NULL)
   {
        g_pD3D->Release();
        g_pD3D = NULL;
   }
}
// WndProc
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message) 
   {
   case WM_KEYUP:
        {
            if (wParam == VK_ESCAPE)
            {
                 DestroyWindow(hWnd);
            }
        }
        break;
   case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
   default:
        {
        }
   }
   return DefWindowProc(hWnd, message, wParam, lParam);
}
-------------------------------------------------------------------------
1.2.3 先分析整个框架, 图1.1
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_011.jpg)
图1.1
1.2.4 我们关心的部分
  // 1. 创建我们的工作室
   g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
   if (g_pD3D == NULL)
   {
        return E_FAIL;
   }
   // 2.现在我们要创建办公桌(IDirect3DDevice9), 我只有默认的adapter, 所以我直接去拿默认adapter的属性,
   //   根据硬件的特征去创建IDirect3DDevice9
   D3DDISPLAYMODE d3ddm;
   FillMemory(&d3ddm, sizeof(D3DDISPLAYMODE), 0);
   if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
   {
        return E_FAIL;
   }
   // 3.创建办公桌(IDirect3DDevice9)的特征要用下面这个结构告诉工作室(IDirect3D9)
   D3DPRESENT_PARAMETERS d3dpm;
   FillMemory(&d3dpm, sizeof(D3DPRESENT_PARAMETERS), 0);
   d3dpm.Windowed         = TRUE;                         // 3.1 我们现在用窗口模式
   d3dpm.SwapEffect       = D3DSWAPEFFECT_DISCARD;         
   d3dpm.BackBufferFormat = d3ddm.Format;
    // 4 创建办公桌(IDirect3DDevice9), 在窗口模式下我们只有一张特别的画纸(IDirect3DSurface9), 应该是个off-screen surface
   if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                                   D3DDEVTYPE_HAL, 
                                   hWnd, 
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                   &d3dpm, 
                                   &g_pD3DDevice)))
   {
        return E_FAIL;
   }
1.2.5 在哪里开始画画?
   在没有有效的窗口消息的时候, 我们就在画纸上画画.
   这里和你在现实中画画不同的是, 现实中你直接去画---执行者;
   但在Direct3D中, 你是命令者, 你不直接画, 而是通过命令让IDirect3DDevice9这个执行者画的.
   首先我们要求IDirect3DDevice9把Surface清干净, 这很重要, 因为我们不清楚上面有什么奇怪的图案.
   然后我们开始画了BeginSecne(), 画好了一定要告诉IDirect3DDevice9, 用EndScence().
   最后的函数下面会解释的.
    VOID Render()
    {
       g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0);
       g_pD3DDevice->BeginScene();
       g_pD3DDevice->EndScene();
       g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
    }
1.2.6 图1.2是我们第一个例子的截图, 感觉不象游戏编程吧, 那就要让它变成吧.
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_012.jpg)
图1.2
**1.3 页面切换**
这里要解释为什么全屏模式下是最少会有2个surface, IDirect3DDevice9的Present的作用.
1.3.1 显示器成像
图1.3是显示器的概要图, 理论上显示器上的图象是一个一个像素画出来的, 速度很快, 所以我们没感觉.
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_013.jpg)
图1.3
画像素时, 电子束从屏幕的左上角开始, 水平从左向右扫描; 完成一行后,  电子束被关闭然后移至下一行的最左边.
电子束从上一行的最右边到下一行的最左边的时间称为水平空白间隔.
当整个屏幕扫描完毕后, 电子束在右下角被关闭, 然后移至左上角开始新的扫描, 这个间隔称为垂直空白间隔.
1.3.2 撕裂现象
电子束扫描成像的依据是在显示内存中的要显示的数据. 问题就在这里, 例如在开始扫描前显示数据在屏幕中心显示一个球,
在电子束扫描一半后, 也就是屏幕上有球的上半部分时, 你改变了显示数据, 要在屏幕中心显示一个正方形, 那么电子束扫描下一半后, 屏幕上变成有球的上半部分和正方形的下半部分的图案 --- 这称为动画的撕裂现象, 图1.4.
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_014.jpg)
图1.4
为避免撕裂现象, 最简单的是在图象被扫描时显示数据不能被改动, 那么就必须在垂直空白间隔时间内去刷新显示数据, 但这很难办到, 大量数据的更新有可能在这时间段完不成, 形成显示不完整.
在垂直空白间隔时间内去更新不同显示数据的地址是可以的, 于是出现了两块独立的显示数据内存块, 被扫描时的显示数据块A不能被更新, 但另一块B可以更新, 然后在垂直空白间隔时间内让显示器寄存器的内容由A的地址改成B的地址, 依次循环. 这就是为什么默认的全屏游戏IDirect3DDevice都会创建最少两块IDirect3DSurface9, 如果两块不够, 你还可以创建3或4或更多(看你的显存了), 加入循环.
1.3.3 IDirect3DDevice9的Present的作用
IDirect3DDevice9的Present的作用就是在垂直空白间隔时间内更新显示器寄存器的内容中的地址.
一般的, 被扫描时的显示数据我们在Direct3D中称为FrontSurfaceBuffer, 其余都称BackSurfaceBuffer.
你可以通过IDirect3DDevice9的GetBackBuffer()直接拿到BackSurfaceBuffer, 修改它的数据. 但IDirect3DDevice9的GetFrontBufferData()只能拿到FrontSurfaceBuffer的数据拷贝, 你不能修改FrontSurfaceBuffer的数据.
图1.5显示刚才的显示数据的地址的更新
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_015.jpg)
图1.5
**1.4 万能的顶点**
1.4.1 三维几何造型技术
如何用数学来表示三维图形是计算机图形学的一个重要研究领域. 我们在高中或大学简单的了解了立体解析几何,
这只是最简单的用数学描述了三维图形, 下面我们认识一下在工业立体造型技术中应用最为广泛的两种表示方法.
1.4.2 体素构造表示法
CSG -- Constructive Solid Geometry.
体素构造表示法将复杂的物体描述为一些简单物体的boolean运算结果, 由于boolean运算是两两进行的,因而复杂物体被表示为一个称为CSG树的二叉树, 其叶结点为基本体素(如立方体, 圆柱, 球等), 其中间结点为boolean运算, 如图1.6.
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_016.jpg)
图1.6
再有复杂物体和基本体素可看成是三维空间点的集合, 从而归结为集合运算, 公式定义为,   
   A <op>* B = r (A <op> B), <op>表示传统的集合并, 交, 差算子, <op>*表示相应的正则运算.
缺点 : 复杂物体被隐式地表达为一棵二叉树, 它仅提供了物体的简洁构造方式而没有存储物体的任何顶点, 边及面的拓扑信息,
       要计算物体的集合性质(如体积, 重心)比较困难.
1.4.2 边界表示法
B-rep --- Boundary Representation Scheme.
边界表示法通过描述物体的边界来定义一个物体(物体边界是指物体内部和外部的分界面).它的优点是拓扑信息和几何信息同时都得到了存储. 而DirectX用到的是以面为中心的表示方法, 如图1.7
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_017.jpg)
图1.7
立方体由6个表面-->表面由多边形组成-->多边形由顶点组成.
            物体数组            表面数组             顶点数组
         |------------|       |------------|       |-----------|
         |  表面表0   |       |  多边形表0 |       |  V0, **N**v0   |
         |  表面表1   |       |  多边形表1 |       |  V1, **N**v1   |
         |  ......    | --->  |  ......    | --->  |  ......   |
         |            |       |            |       |           |
                                           /
                                            /
                                              面法向数组
                                            |------------|
                                            |    **N**p0       |
                                            |    **N**p1       |
                                            |   ......   |
**多边形中, 由于三角形的三点共面的特征, 在3D中物体基本都是由三角形来构成的, 同时我们不要忘记, 三角形是由顶点构成的. Direct3D中也遵循这个原则.**
1.4.3 Direct3D中的简单图元
Direct3D中的简单图元包括以下几种,
a. Point List --- 点列
           1          3           5
          .          .           .
     .           .            .
    0            2            4
b. Line List --- 线列
           1          3         5
          .          .         .
         /          /         /
        /          /         /
       /          /         /
      /          /         /
    0           2        4 
c. Line Strip --- 线带
           1        3        5
          .        .        .
         / /      / /      /
        /   /    /   /    /
       /     /  /     /  /
      /       //       //
    0         2        4 
d. Triangle List --- 三角形列
           1      3           5
          .        .________.
         / /        /      /
        /   /        /    /
       /     /        /  /
      /_______/        //
     0         2        4 
e. Triangle Strip --- 三角形带
           1      3          5
          ._________._______.
         / /       //      /
        /   /     /  /    /
       /     /   /    /  /
      /_______//_______//
     0         2        4 
f. Triangle Fan --- 三角扇形
           1      2     3         
            ._____._____.
             /    |    /
              /   |   /
               /  |  /
                / | /
                 /|/
                  0
1.4.4 灵活顶点格式(Flexible Vertex Format)
我们在上面已经讲解了所有的物体最终由顶点构成, 那么在Direct3D为详细的描述一个顶点, 而又能用最小的开销, 就有了
灵活顶点格式. 你想用什么就告诉Direct3D你要用的 如图1.8.
![](http://blog.csdn.net/images/blog_csdn_net/jaredz/193444/o_018.jpg)
图1.8
例如 1. 想使用顶点的坐标和顶点的漫反射颜色, 你需要定义如下的信息和数据结构
        D3DFVF_XYZ  | D3DFVF_DIFFUSE 
        struct MYVERTEX 
       {
            FLOAT x, y, z;
            DWORD color; 
        };
例如 2. 一般我们都使用如下的定义, 表示顶点有坐标, 法向量和纹理映射
        D3DFVF_XYZ|D3DFVF_NORMAL| D3DFVF_TEX1
        struct MYVERTEX 
       {
             FLOAT x, y, z;
             FLOAT nx, ny, nz;
             FLOAT tu, tv; 
       };
**要注意, 数据结构中的变量先后次序按上图的严格定义**
1.4.5 如何让Direct3D知道你顶点的定义和具体顶点的数值
我们必须把关于顶点的所有信息告诉Direct3D, 当然不是告诉它:" 我把顶点的信息放在家里的桌子上, 你自己去拿".
我们需要一段内存(系统内存或显示内存)来保存具体顶点的数值, 还要让Direct3D知道有多少, 每个有什么信息.
这些都是由IDirect3DVertexBuffer9负责的, IDirect3DVertexBuffer9由IDirect3DDevice9创建, 创建时要明确的告诉IDirect3DDevice9总共有多少大小的顶点, 及每个有什么信息.
IDirect3DVertexBuffer9只为我们申请了一段内存, 没有具体的顶点的数值, 这些要我们填进去, 
那就需要内存的地址(逻辑地址), 调用IDirect3DVertexBuffer9的Lock可以得到我们需要的地址,
填写完毕, 调用IDirect3DVertexBuffer9的Unlock告诉它完成了.
最终用这些顶点画出什么, 要在渲染的时候命令IDirect3DDevice9帮你画, 当然在画之前你要明确的告诉它顶点的数值在哪里, 顶点有什么特征.
下面是示例,
struct MYVERTEX
{
    FLOAT x, y, z, rhw; 
    DWORD colour; 
};
#define D3DFVF_MYVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
MYVERTEX aVertex[] =
{
    {500.0f, 200.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255) }, 
    {800.0f, 600.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0) },
    {200.0f, 600.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0) }
};
UINT nSize = 3 * sizeof(MYVERTEX);
if(FAILED(g_pD3DDevice->CreateVertexBuffer(nSize,
                                           D3DUSAGE_SOFTWAREPROCESSING,
                                           D3DFVF_MYVERTEX,
                                           D3DPOOL_DEFAULT, 
                                           &g_pD3DVBuffer,
                                           NULL)))
{
    return E_FAIL;
}
if(FAILED(g_pD3DVBuffer->Lock(0, nSize, &pV, 0)))
{
    return E_FAIL;
}
MoveMemory(pV, aVertex, nSize);
g_pD3DVBuffer->Unlock();
**1.5 第二个例子**
1.5.1 看到图形了
在这个例子里, 我们用顶点来画个二维的三角形, 我们使用了如下的顶点特征 
D3DFVF_XYZRHW | D3DFVF_DIFFUSE
struct MYVERTEX
{
    FLOAT x, y, z, rhw; 
    DWORD colour; 
};
我们使用转换好的顶点坐标(transformed vertex)来画个彩色三角形, 关于transformed vertex, 我们在Direct3D Graphics Pipeline会详细的讲解它的数学模型.
这个例子的代码也在game0 project的game0a.cpp中
我们在上面的例子Init3D后调用InitVertexBuffer创建顶点.
HRESULT InitVertexBuffer()
{
    LPVOID pV = NULL;
    MYVERTEX aVertex[] =
    {
        {500.0f, 200.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255) }, 
        {800.0f, 600.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0) },
        {200.0f, 600.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0) }
    };
    UINT nSize = 3 * sizeof(MYVERTEX);
    if(FAILED(g_pD3DDevice->CreateVertexBuffer(nSize,
                                               D3DUSAGE_SOFTWAREPROCESSING,
                                               D3DFVF_MYVERTEX,
                                               D3DPOOL_DEFAULT, 
                                               &g_pD3DVBuffer,
                                               NULL)))
    {
        return E_FAIL;
    }
    if(FAILED(g_pD3DVBuffer->Lock(0, nSize, &pV, 0)))
    {
        return E_FAIL;
    }
    MoveMemory(pV, aVertex, nSize);
    g_pD3DVBuffer->Unlock();
    return S_OK;
}
然后在渲染的函数里我们命令IDirect3DDevice9帮我们画.
VOID Render()
{
    g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    g_pD3DDevice->BeginScene();
    g_pD3DDevice->SetStreamSource(0, g_pD3DVBuffer, 0, sizeof(MYVERTEX)); // 告诉IDirect3DDevice9顶点的数据在哪里
    g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);                                // 告诉IDirect3DDevice9顶点的特征
    g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0,  1);               // 命令IDirect3DDevice9画三角形
    g_pD3DDevice->EndScene();
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
} 
本文转载:http://blog.csdn.net/jaredz/article/details/663888
