# DirectDraw简单用法 - xqhrs232的专栏 - CSDN博客
2012年09月18日 14:06:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3919
原文地址::[http://www.webgou.info/content/programming/363/?jdfwkey=9sycs3](http://www.webgou.info/content/programming/363/?jdfwkey=9sycs3)
 GDI:win32 gdi 与 DirectDraw 操作DIB的,DirectDraw 直接操作内存。
```
//ce 6下面测试:
IDirectDraw *pDraw;
IDirectDrawSurface *pDFace;
IDirectDrawSurface *pLayer;
HRESULT hr;
DDSURFACEDESC  ddsd;
  RECT rect;
  dlgMain.GetWindowRect(&rect);
  DirectDrawCreate(NULL, &pDraw, NULL);
  pDraw->SetCooperativeLevel(dlgMain,DDSCL_NORMAL);
  ddsd.dwSize = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  hr = pDraw->CreateSurface(&ddsd, &pDFace,NULL);
  DDCAPS ddcaps;
  memset(&ddcaps, 0, sizeof(ddcaps));
  hr = pDraw->GetCaps(&ddcaps,NULL);
  
  memset(&ddsd, 0, sizeof(ddsd));
  ddsd.dwSize = sizeof(ddsd);
  ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY;
  ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |DDSD_PIXELFORMAT;
  ddsd.dwWidth = 800;
  ddsd.dwHeight = 480;
  ddsd.ddpfPixelFormat = ddpfOverlayFormats;
  hr = pDraw->CreateSurface(&ddsd, &pLayer,NULL);
  hr = pLayer->IsLost();
 
  DDOVERLAYFX ofx;
  memset(&ofx, 0, sizeof(DDOVERLAYFX));
  ofx.dwSize = sizeof(DDOVERLAYFX);
  ofx.dckSrcColorkey.dwColorSpaceLowValue = 0x0000F81F;
  ofx.dckSrcColorkey.dwColorSpaceHighValue = 0x0000;
  ofx.dwAlphaConstBitDepth = 8;
  ofx.dwAlphaConst = 0x50;
  RECT rt1= {0,0,400,480};
  hr = pLayer->UpdateOverlay(&rt1, pDFace, &rt1,DDOVER_SHOW|DDOVER_KEYSRCOVERRIDE, &ofx);
  HDC hdc;
  hr = pLayer->GetDC(&hdc);
  FillRect(hdc,&rt1, (HBRUSH)GetStockObject(BLACK_BRUSH));
  SetBkColor(hdc,TRANSPARENT);
  SetTextColor(hdc,0x0000F81F);
  DrawText(hdc,_T("pLayerpLayer pLayervpLayer "), -1,  &rt1, 1);
  hr = pLayer->ReleaseDC(hdc);
   pLayer->Release();
   pDFace->Release();
   pDraw->Release();
```
1.链接库
动态装载
typedef HRESULT (* DIRECTDRAWCREATE)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );
m_hModule=::LoadLibrary(_T("ddraw.dll"));
m_pDirectDrawCreate= (DIRECTDRAWCREATE)::GetProcAddress(m_hModule,L"DirectDrawCreate");
静态链接
#pragma comment(lib, "ddraw.lib") 调用DirectDrawCreate
2.GUID的使用
直接定义
const GUID g_IID_IDirectDraw ={0x9c59509a,0x39bd,0x11d1,
0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5};
链接#pragma comment(lib, "dxguid.lib")
QueryInterface IID_IDirectDraw4
3.创建主页面
// Create the main DirectDraw object
LPDIRECTDRAW g_pDD;
hRet = DirectDrawCreate(NULL, &g_pDD, NULL);
hRet = g_pDD->QueryInterface(g_IID_IDirectDraw, (LPVOID *)&g_pDD);
// Get normal mode
hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
memset(&ddsd, 0, sizeof(ddsd));
ddsd.dwSize = sizeof(ddsd);
ddsd.dwFlags = DDSD_CAPS;
ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
// creates a DirectDrawSurface object for this DirectDraw object.
hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
创建主页面无须指定像素格式和页面大小
4.页面的像素格式
//YUV4:1:1
DDPIXELFORMAT pixel_format=
{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','V','1','2'),0,0,0,0,0};
//RGB565
DDPIXELFORMAT ddpfOverlayFormats =
{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16, 0xF800, 0x07e0, 0x001F, 0};
5.创建OverLay页面
memset(&ddsd, 0, sizeof(ddsd));
ddsd.dwSize = sizeof(ddsd);
ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY;
ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | DDSD_PIXELFORMAT;
ddsd.dwWidth = DDRAW_X;
ddsd.dwHeight = DDRAW_Y;
ddsd.dwBackBufferCount = 1;//有个后台页面
ddsd.ddpfPixelFormat = pixel_format;
hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSOverlay, NULL);
创建OverLay页面应该指定页面的大小（可以全屏），像素格式。
6.OverLay页面的显示与隐藏
void ShowOverLay()
{
DDOVERLAYFX ovfx = {0};
ovfx.dwSize = sizeof(DDOVERLAYFX);
ovfx.dckSrcColorkey.dwColorSpaceLowValue = COLOR_KEY;//指定页面的透明色
ovfx.dckSrcColorkey.dwColorSpaceHighValue = COLOR_KEY;
ovfx.dwAlphaConstBitDepth = 8;//指明alpha位宽度
ovfx.dwAlphaConst = 0x50;//alpha透明值 指明页面的透明度.16级.
RECT rs;
rs.left = 0;
rs.top = 0;
rs.right = DDRAW_X;
rs.bottom = DDRAW_Y;
DWORD dwUpdateFlags = DDOVER_KEYSRCOVERRIDE | DDOVER_SHOW;
if(g_pDDSOverlay != NULL && g_pDDSOverlay->IsLost() == DD_OK)
{
HRESULT hret = g_pDDSOverlay->UpdateOverlay(&rs,g_pDDSPrimary,&rs,dwUpdateFlags,&ovfx);
//将主页面和OverLay页面关联起来
if(hret != DD_OK)
{
printf("Update OverLay errorrn");
if(hret == DDERR_SURFACELOST)
{
g_pDDSOverlay->Restore();
}
}
}
}
dwUpdateFlags = DDOVER_HIDE即隐藏页面。
UpdateOverlay这个接口一般是在页面初始化后，显示，隐藏这三种状况下调用，
每次在页面上绘图后不必要调用UpdateOverlay，否则就会造成画面显示卡动，不连续。
7.在页面上绘图
在页面上绘图有两种方法
一是获取页面DC后，以GDI方式绘图
if( g_pDDSOverlay->GetDC(&hDC) == DD_OK)
//进行DC绘图
g_pDDSOverlay->ReleaseDC(hDC);
第二种状况当知道页面的像素格式，Lock页面获取页面指针，直接操作页面存储区。
g_pDDSOverlay->Lock(NULL,&ddsd,0,NULL);
BYTE *pFramePhyPtr = (LPBYTE)ddsd.lpSurface;
int framesize = DDRAW_X * DDRAW_Y;
memcpy(pFramePhyPtr,src[0],framesize);
memcpy(pFramePhyPtr + framesize,src[2],framesize / 4);
memcpy(pFramePhyPtr + framesize + framesize / 4,src[1], framesize / 4);
g_pDDSOverlay->Unlock(NULL);//YUV4:1:1
8.遍历后台页面
EnumAttachedSurfaces
enumerates all the surfaces attached to a given primary surface.
可以先到后台页面上绘制，然后Flip操作.
9.注意要点
不要频繁调用UpdateOverLay。
注意页面的像素格式与图像帧的像素格式的匹配,
为了加快显示速度，最好与显示驱动所支持的像素格式相同。
可以用后台页面和Flip操作，加快页面显示。
具体操作见资源中心的示例。
//=====================================
备注:
1.
UpdateOverlay这个接口一般是在页面初始化后，显示，隐藏这三种状况下调用，
每次在页面上绘图后不必要调用UpdateOverlay，否则就会造成画面显示卡动，不连续
2.
不要频繁调用UpdateOverLay。
注意页面的像素格式与图像帧的像素格式的匹配,
为了加快显示速度，最好与显示驱动所支持的像素格式相同。
可以用后台页面和Flip操作，加快页面显示。
