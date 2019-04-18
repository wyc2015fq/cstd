# d3d中的Surface（2d）绘制 - gauss的专栏 - CSDN博客
2011年12月29日 13:06:28[gauss](https://me.csdn.net/mathlmx)阅读数：421
原文出处:
http://coffeejp.com/bbs/viewthread.php?tid=168641感谢作者:ccsakura1.1 Surfaces之封装CXSurface
1、Surfaces是什么：
　　 通俗的讲surfaces就是一个二维的矩形平面。在DX9中，与其对应的com接口为IDirect3DSurface9，LPDIRECT3DSURFACE9。
2、Surfaces的作用：
　　 作为一个矩形平面，surfaces用来在屏幕上显示平面图象，即从文件中读取图象数据呈现给用户。
3、IDirect3DSurface9的使用一般过程：
　　 声明： LPDIRECT3DSURFACE9
　　 创建： CreateOffscreenPlainSurface(…)
　　 获取图象信息： D3DXGetImageInfoFromFile(…)
　　 装载到surfaces中： D3DXLoadSurfaceFromFile(…)
　　 获取back buffer地址： GetBackBuffer(…)
　　 显示： UpdateSurface(…)
　　 释放内存 Release()
代码段如下：
LPDIRECT3DSURFACE9 g_Surface =NULL;
D3DXIMAGE_INFO Info;
D3DXGetImageInfoFromFile("D:\image.jpg", &Info);
g_pd3dDevice->CreateOffscreenPlainSurface(Info.Width, Info.Height, Info.Format, &g_Surface, NULL);
D3DXLoadSurfaceFromFile(g_Surface, NULL, NULL, "D:\image.jpg", NULL, D3DX_FILTER_NONE, 0xFF000000, NULL);
//--------------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 BackBuffer = NULL;
g_pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &BackBuffer);
g_pd3dDevice->UpdateSurface(g_Surface, NULL, BackBuffer, NULL);
if(BackBuffer != NULL)
BackBuffer->Release();
//---------------------------------------------------------------------------------------------------
if(g_Surface!= NULL)
g_Surface ->Release();
　　 由上述过程可以看到，IDirect3DSurface9的使用虽然不十分复杂，但有点不方便 ——创建和释放总要成对出现，使用过程中还穿插着LPDIRECT3DDEVICE9接口。这些若用一个类封装起来，使用就要方便的多。
4、如何封装：
　　 按照surfaces的功能，创建它就用来显示图象。因此要有读取图象的接口和显示图象的接口。又要与LPDIRECT3DDEVICE9设备接口关联，因此需要一个设置设备的接口。如下所示：
1、声明及释放
　　 声明： LPDIRECT3DSURFACE9
　　 释放内存 Release()
2、关联图像：LoadFromFile
　　 获取图象信息： D3DXGetImageInfoFromFile(…)
　　 创建： CreateOffscreenPlainSurface(…)
　　 装载到surfaces中： D3DXLoadSurfaceFromFile(…)
3、显示图象 Render
　　 获取缓存地址： GetBackBuffer(…)
　　 显示： UpdateSurface(…)
4、关联设备接口 SetDevice
所以CXSurface的定义如下：
class CXSurface
{
protected:
LPDIRECT3DSURFACE9 m_Surface;
LPDIRECT3DSURFACE9 m_BackBuffer; //Back buffer
LPDIRECT3DDEVICE9 m_pDevice; //Direct3D的设备指针
public:
CXSurface(LPDIRECT3DDEVICE9 pDevice);
~CXSurface();
HRESULT LoadFromFile(LPCSTR Path);
void Render(void);
};
1.2 Textures & Sprite 之封装CXTexture & CXSprite
1、何为Textures
　　 Textures是在屏幕上显示的平面图形，它能够提供比 surface 更多的图形处理效果——移动、缩放、旋转及作为纹理皮肤粘贴在3D模型上。在Direct3D中，其封装的接口为IDirect3DTexture9。
2、何为Sprite
　　 IDirect3DTexture9能从文件中读取纹理数据，但由于Textures不能直接复制到 back buffer，因此在屏幕上绘制Textures之前，需要另一个接口——ID3DXSprite（精灵）。ID3DXSprite能够把若干个Textures 复制给back buffer，因此需要ID3DXSprite的一个实例就可以绘制所有的纹理。
　　 所以，IDirect3DTexture9用来存放程序所需的纹理，但它本身又不能绘制纹理，需要借助接口ID3DXSprite。
3、IDirect3DTexture9和ID3DXSprite的使用过程
　　 定义：~
　　 创建：D3DXCreateTextureFromFile
　　　　　 D3DXCreateSprite
　　 创建变换矩阵： D3DXMatrixTransformation2D
　　 变换： SetTransform
　　 绘制图象： Draw
　　 释放内存： ~
代码如下：
D3DXCreateTextureFromFile(g_pd3dDevice, "c:\\image.bmp”, &g_Texture);
D3DXCreateSprite(g_pd3dDevice, &g_Sprite);
//--------------------------------------------------------------------------
D3DXVECTOR2 Translation;
Translation.x = 500;
Translation.y = 500;
D3DXVECTOR2 Scaling;
Scaling.x = 1.0;f
Scaling.y = 1.0f;
D3DXMATRIX Mat;
D3DXMatrixTransformation2D(&Mat, NULL, 0, &Scaling, NULL, 0, &Translation);
g_Sprite->Begin(0);
g_Sprite->SetTransform(&
g_Sprite->Draw(g_Texture,
g_Sprite->End();
4、 如何封装
　　 从以上的基本过程可以看到，CXTexture需要完成的功能：提供与LPDIRECT3DDEVICE9的接口，与纹理文件文件关联，对纹理进行处理（缩放、旋转……）。
class CXTexture
{
protected:
LPDIRECT3DTEXTURE9 m_Texture;
LPDIRECT3DDEVICE9 m_pDevice;
D3DXVECTOR2 m_RotationCenter;
D3DXVECTOR2 m_Translation;
D3DXVECTOR2 m_Scaling;
FLOAT m_Rotation;
RECT m_SrcRect;
public:
CXTexture(LPDIRECT3DDEVICE9 pDevice);
~CXTexture();
LPDIRECT3DTEXTURE9 GetTexture() const {return m_Texture;}
void SetTexture(LPDIRECT3DTEXTURE9 Texture) const {m_Texture = Texture;}
LPDIRECT3DDEVICE9 GetDevice() const {return m_pDevice;}
void SetDevice(LPDIRECT3DDEVICE9 pDevice) const {m_pDevice = pDevice;}
D3DXVECTOR2 GetRotationCenter() const {return m_RotationCenter;}
void SetRotationCenter(D3DXVECTOR2 RotationCenter) {m_RotationCenter = RotationCenter;}
D3DXVECTOR2 GetTranslation() const {return m_Translation;}
void SetTranslation (D3DXVECTOR2 Translation) const {m_Translation = Translation;}
D3DXVECTOR2 GetScaling() const {return m_Scaling;}
void SetScaling(D3DXVECTOR2 Scaling) const {m_Scaling = Scaling;}
FLOAT GetRotation() const {return m_Rotation;}
void SetRotation (FLOAT Rotation) const {m_Rotation = Rotation;}
RECT GetRect() const {return m_SrcRect;}
void SetRect(RECT SrcRect) const {m_SrcRect = SrcRect;}
HRESULT LoadFromFile(char* Path);
};
CXSprite的主要功能就是在屏幕上显示 CXTexture，因此需要有与 LPDIRECT3DDEVICE9 接口和 CXTexture连接的函数。
class CXSprite
{
protected:
LPD3DXSPRITE m_Sprite;
LPDIRECT3DDEVICE9 m_pDevice;
public:
CXSprite (LPDIRECT3DDEVICE9 pDevice);
~CXSprite ();
LPD3DXSPRITE GetSprite() const {return m_Sprite;}
void SetSprite(LPD3DXSPRITE Sprite) const {m_Sprite = Sprite;}
LPDIRECT3DDEVICE9 GetDevice() const {return m_pDevice;}
void SetDevice(LPDIRECT3DDEVICE9 pDevice) const {m_pDevice = pDevice;}
HRESULT DrawTexture(CXTexture* Texture);
};
1.3 Keyboard & Mouse之封装CXKeyboard & CXMouse
1、何为Keyboard & Mouse
　　 “地球人都知道”。DX9提供的接口 IDirectInputDevice8。
2、二者的功能
　　 Keyboard：读取键盘的按键信息
　　 Mouse：读取鼠标的按键、位置信息，设置光标属性(如用图片表示光标)。
3、使用过程
　　 创建 IDirectInput8 对象 DirectInput8Create
　　 创建 IDirectInput8 设备（键盘、鼠标等） CreateDevice
　　 设置设备属性 SetCooperativeLevel
　　 SetDataFormat
　　 获取设备使用权 Acquire
　　 读取设备传入的数据 GetDeviceState
　　 释放设备及 IDirectInput8 Release
设置鼠标光标过程：
　　 创建光标图片资源： 参照surfaces
　　 设置光标为指定的图片 SetCursorProperties
　　 设置光标的初始位置 SetCursorPosition
　　 显示光标 ShowCursor
程序段如下：
LPDIRECTINPUT8 g_lpDI;
LPDIRECTINPUTDEVICE8 g_Keyboard; 
LPDIRECTINPUTDEVICE8 g_Mouse
//========================================================
HRESULT Result = DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDI, NULL);
if(SUCCEEDED(Result))
{
//创建键盘设备
Result = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL);
if(SUCCEEDED(Result))
{
g_lpDIDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard);
}
}
//-------------------------------------------------------------------------------------------
//获取按键信息
if(SUCCEEDED(g_Keyboard->Acquire())) //Acquire the device
{
char KeyState[256];
g_Keyboard->GetDeviceState(sizeof(KeyState),(LPVOID)&KeyState);
//根据KeyState返回的数据就可以判断按下何键
}
//====================================================
//创建鼠标
g_lpDI->CreateDevice(GUID_SysMouse, &g_Mouse, NULL);
g_Mouse->SetDataFormat(&c_dfDIMouse);
g_Mouse->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
//------------------------------------------------------------------------
//设置鼠标光标
//获取光标图片
D3DXIMAGE_INFO ImageInfo;
D3DXGetImageInfoFromFile("C:\\Cursor.jpg”, &ImageInfo);
g_pd3dDevice->CreateOffscreenPlainSurface(ImageInfo.Width, ImageInfo.Height, ImageInfo.Format, D3DPOOL_DEFAULT, &g_MouseCursor, NULL);
D3DXLoadSurfaceFromFile(g_MouseCursor, NULL, NULL, ("C:\\Cursor.jpg”, NULL, D3DX_FILTER_NONE, 0xFF000000, NULL);
//设置成指定的光标
g_pd3dDevice->SetCursorProperties(0,0, g_MouseCursor);
//初试位置
g_pd3dDevice->SetCursorPosition(0,0,D3DCURSOR_IMMEDIATE_UPDATE);
//显示光标
g_pd3dDevice->ShowCursor(true);
//-------------------------------------------------------------------------
//获取鼠标信息
if(SUCCEEDED(g_Mouse->Acquire()))
{
DIMOUSESTATE State;
g_Mouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&State);
//信息保存在 State中。
}
//==============================================
//释放设备
if(g_lpDI != NULL)
g_lpDI->Release();
4、封装
　　 CXKeyboard的封装
class CXKeyboard
{
private:
LPDIRECTINPUTDEVICE8 m_pDevice;
char m_KeyState[256];
public:
CXKeyboard (LPDIRECTINPUT8 pInput, HWND hWnd);
~ CXKeyboard();
bool IsKeyPressed(int Key);
HRESULT Update();
};
