//PURPOSE:
// 3D Drawing Demo
//Required libraries:
// WINMM.LIB, D3D9.LIB, D3DX9.LIB.
#include <windows.h>
#include <stdio.h>
#include "d3d9.h"
#include "d3dx9.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma warning(disable : 4305)
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define Safe_Release(p) if((p)) (p)->Release();
// window handles, class and caption text.
HWND g_hwnd;
HINSTANCE g_inst;
static char g_class_name[] = "Draw3DClass";
static char g_caption[] = "Draw3D Demo";
// the Direct3D and device object
IDirect3D9* g_d3d = NULL;
IDirect3DDevice9* g_d3d_device = NULL;
// The 3D vertex format and descriptor
typedef struct {
  float x, y, z; // 3D coordinates
  float u, v; // texture coordinates
} VERTEX;
#define VERTEX_FVF (D3DFVF_XYZ | D3DFVF_TEX1)
IDirect3DVertexBuffer9* g_vertex_buffer = NULL;
IDirect3DTexture9* g_texture = NULL;
//--------------------------------------------------------------------------------
// Window procedure.
//--------------------------------------------------------------------------------
long WINAPI Window_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return (long) DefWindowProc(hwnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------------
// Initialize d3d, d3d device, vertex buffer, texutre; set render state for d3d;
// set perspective matrix and view matrix.
//--------------------------------------------------------------------------------
BOOL Do_Init()
{
  D3DPRESENT_PARAMETERS present_param;
  D3DDISPLAYMODE display_mode;
  D3DXMATRIX mat_proj, mat_view;
  BYTE* vertex_ptr;
  // initialize vertex data
  VERTEX verts[] = {
    { -100.0f, 100.0f, 0.0f, 0.0f, 0.0f },
    { 100.0f, 100.0f, 0.0f, 1.0f, 0.0f },
    { -100.0f, -100.0f, 0.0f, 0.0f, 1.0f },
    { 100.0f, -100.0f, 0.0f, 1.0f, 1.0f }
  };
  // do a windowed mode initialization of Direct3D
  if ((g_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
    return FALSE;
  }
  // retrieves the current display mode of the adapter
  if (FAILED(g_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode))) {
    return FALSE;
  }
  ZeroMemory(&present_param, sizeof(present_param));
  // initialize d3d presentation parameter
  present_param.Windowed = TRUE;
  present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
  present_param.BackBufferFormat = display_mode.Format;
  present_param.EnableAutoDepthStencil = TRUE;
  present_param.AutoDepthStencilFormat = D3DFMT_D16;
  // creates a device to represent the display adapter
  if (FAILED(g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present_param, &g_d3d_device))) {
    return FALSE;
  }
  // set render state
  // disable d3d lighting
  g_d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
  // enable z-buffer
  g_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  // create and set the projection matrix
  // builds a left-handed perspective projection matrix based on a field of view
  D3DXMatrixPerspectiveFovLH(&mat_proj, D3DX_PI / 4.0, 1.33333, 1.0, 1000.0);
  // sets a single device transformation-related state
  g_d3d_device->SetTransform(D3DTS_PROJECTION, &mat_proj);
  // create and set the view matrix
  D3DXMatrixLookAtLH(&mat_view,
      &D3DXVECTOR3(0.0, 0.0, -500.0),
      &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
      &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
  g_d3d_device->SetTransform(D3DTS_VIEW, &mat_view);
  // create the vertex buffer and set data
  g_d3d_device->CreateVertexBuffer(sizeof(VERTEX) * 4, 0, VERTEX_FVF, D3DPOOL_DEFAULT, &g_vertex_buffer, NULL);
  // locks a range of vertex data and obtains a pointer to the vertex buffer memory
  g_vertex_buffer->Lock(0, 0, (void**)&vertex_ptr, 0);
  memcpy(vertex_ptr, verts, sizeof(verts));
  // unlocks vertex data
  g_vertex_buffer->Unlock();
  // load the texture map
  if (0) {
    const char* pic;
    pic = "E:/pub/pic/10.bmp";
    D3DXCreateTextureFromFile(g_d3d_device, pic, &g_texture);
  }
  else {
    int width = 100, height = 100, bytes_per_pixel = 4;
    int x, y;
    D3DLOCKED_RECT tex_locked_rect = {0};
    unsigned int* pix = (unsigned int*)malloc(sizeof(int) * width * height);
    IDirect3DDevice9_CreateTexture(g_d3d_device, width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_texture, NULL);
    if (IDirect3DTexture9_LockRect(g_texture, 0, &tex_locked_rect, NULL, 0) != D3D_OK) {
      return false;
    }
    for (y = 0; y < height; ++y) {
      for (x = 0; x < width; ++x) {
        *(unsigned int*)((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y + x * 4) = ((x / 4 + y / 4) & 1) ? 0xff00ff00 : 0xffff00ff;
      }
    }
    IDirect3DTexture9_UnlockRect(g_texture, 0);
  }
  return TRUE;
}
//--------------------------------------------------------------------------------
// Release all d3d resource.
//--------------------------------------------------------------------------------
BOOL Do_Shutdown()
{
  Safe_Release(g_vertex_buffer);
  Safe_Release(g_texture);
  Safe_Release(g_d3d_device);
  Safe_Release(g_d3d);
  return TRUE;
}
//--------------------------------------------------------------------------------
// Render a frame.
//--------------------------------------------------------------------------------
BOOL Do_Frame1()
{
  D3DXMATRIX mat_world;
  // clear device back buffer
  g_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 64, 128, 255), 1.0f, 0);
  // Begin scene
  if (SUCCEEDED(g_d3d_device->BeginScene())) {
    // create and set the world transformation matrix
    // rotate object along z-axis
    D3DXMatrixRotationZ(&mat_world, (float)(timeGetTime() / 1000.0));
    g_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);
    // set the vertex stream, shader, and texture.
    // binds a vertex buffer to a device data stream
    g_d3d_device->SetStreamSource(0, g_vertex_buffer, 0, sizeof(VERTEX));
    // set the current vertex stream declation
    g_d3d_device->SetFVF(VERTEX_FVF);
    // assigns a texture to a stage for a device
    g_d3d_device->SetTexture(0, g_texture);
    // renders a sequence of noindexed, geometric primitives of the specified type from the current set
    // of data input stream.
    g_d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    // release texture
    g_d3d_device->SetTexture(0, NULL);
    // end the scene
    g_d3d_device->EndScene();
  }
  // present the contents of the next buffer in the sequence of back buffers owned by the device
  g_d3d_device->Present(NULL, NULL, NULL, NULL);
  return TRUE;
}
int Do_Frame()
{
  // Avoid rendering when minimized
  if (NULL == g_vertex_buffer) {
    VERTEX verts1[] = {
      { 100.0f, 100.0f, 0.0f, 0.0f, 0.0f },
      { 300.0f, 100.0f, 0.0f, 1.0f, 0.0f },
      { 100.0f, 300.0f, 0.0f, 0.0f, 1.0f },
      { 300.0f, 300.0f, 0.0f, 1.0f, 1.0f },
    };
    D3DPRESENT_PARAMETERS present_param;
    D3DDISPLAYMODE display_mode;
    D3DXMATRIX mat_proj, mat_view;
    BYTE* vertex_ptr;
    // initialize vertex data
    VERTEX verts[] = {
      { -100.0f, 100.0f, 0.0f, 0.0f, 0.0f },
      { 100.0f, 100.0f, 0.0f, 1.0f, 0.0f },
      { -100.0f, -100.0f, 0.0f, 0.0f, 1.0f },
      { 100.0f, -100.0f, 0.0f, 1.0f, 1.0f }
    };
    // do a windowed mode initialization of Direct3D
    if ((g_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
      return FALSE;
    }
    // retrieves the current display mode of the adapter
    if (FAILED(g_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode))) {
      return FALSE;
    }
    ZeroMemory(&present_param, sizeof(present_param));
    // initialize d3d presentation parameter
    present_param.Windowed = TRUE;
    present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present_param.BackBufferFormat = display_mode.Format;
    present_param.EnableAutoDepthStencil = TRUE;
    present_param.AutoDepthStencilFormat = D3DFMT_D16;
    // creates a device to represent the display adapter
    if (FAILED(g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present_param, &g_d3d_device))) {
      return FALSE;
    }
    // set render state
    // disable d3d lighting
    g_d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    // enable z-buffer
    g_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    // create and set the projection matrix
    // builds a left-handed perspective projection matrix based on a field of view
    D3DXMatrixPerspectiveFovLH(&mat_proj, D3DX_PI / 4.0, 1.33333, 1.0, 1000.0);
    // sets a single device transformation-related state
    g_d3d_device->SetTransform(D3DTS_PROJECTION, &mat_proj);
    // create and set the view matrix
    D3DXMatrixLookAtLH(&mat_view,
        &D3DXVECTOR3(0.0, 0.0, -500.0),
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    g_d3d_device->SetTransform(D3DTS_VIEW, &mat_view);
    // create the vertex buffer and set data
    g_d3d_device->CreateVertexBuffer(sizeof(VERTEX) * 4, 0, VERTEX_FVF | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_vertex_buffer, NULL);
    // locks a range of vertex data and obtains a pointer to the vertex buffer memory
    g_vertex_buffer->Lock(0, 0, (void**)&vertex_ptr, 0);
    memcpy(vertex_ptr, verts, sizeof(verts));
    // unlocks vertex data
    g_vertex_buffer->Unlock();
    // load the texture map
    if (0) {
      const char* pic;
      pic = "E:/pub/pic/10.bmp";
      D3DXCreateTextureFromFile(g_d3d_device, pic, &g_texture);
    }
    else {
      int width = 100, height = 100, bytes_per_pixel = 4;
      int x, y;
      D3DLOCKED_RECT tex_locked_rect = {0};
      unsigned int* pix = (unsigned int*)malloc(sizeof(int) * width * height);
      IDirect3DDevice9_CreateTexture(g_d3d_device, width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_texture, NULL);
      if (IDirect3DTexture9_LockRect(g_texture, 0, &tex_locked_rect, NULL, 0) != D3D_OK) {
        return false;
      }
      for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
          *(unsigned int*)((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y + x * 4) = ((x / 4 + y / 4) & 1) ? 0xff00ff00 : 0xffff00ff;
        }
      }
      IDirect3DTexture9_UnlockRect(g_texture, 0);
    }
  }
  {
    D3DXMATRIX mat_world;
    // clear device back buffer
    g_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 64, 128, 255), 1.0f, 0);
    // Begin scene
    if (SUCCEEDED(g_d3d_device->BeginScene())) {
      // create and set the world transformation matrix
      // rotate object along z-axis
      D3DXMatrixRotationZ(&mat_world, (float)(0 / 1000.0));
      IDirect3DDevice9_SetPixelShader(g_d3d_device, NULL);
      IDirect3DDevice9_SetVertexShader(g_d3d_device, NULL);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_CULLMODE, D3DCULL_NONE);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_LIGHTING, FALSE);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_ZENABLE, FALSE);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_ALPHABLENDENABLE, TRUE);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_ALPHATESTENABLE, FALSE);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_BLENDOP, D3DBLENDOP_ADD);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
      IDirect3DDevice9_SetRenderState(g_d3d_device, D3DRS_SCISSORTESTENABLE, TRUE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      IDirect3DDevice9_SetTextureStageState(g_d3d_device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      IDirect3DDevice9_SetSamplerState(g_d3d_device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      IDirect3DDevice9_SetSamplerState(g_d3d_device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
      {
        const float L = 0, R = 200, T = 0, B = 200;
        D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
        D3DMATRIX mat_projection = {
          2.0f / (R - L), 0.0f, 0.0f, 0.0f,
          0.0f, 2.0f / (T - B), 0.0f, 0.0f,
          0.0f, 0.0f, 0.5f, 0.0f,
          (L + R) / (L - R), (T + B) / (B - T), 0.5f, 1.0f,
        };
        IDirect3DDevice9_SetTransform(g_d3d_device, D3DTS_WORLD, &mat_identity);
        IDirect3DDevice9_SetTransform(g_d3d_device, D3DTS_VIEW, &mat_identity);
        IDirect3DDevice9_SetTransform(g_d3d_device, D3DTS_PROJECTION, &mat_projection);
      }
      g_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);
      // set the vertex stream, shader, and texture.
      // binds a vertex buffer to a device data stream
      g_d3d_device->SetStreamSource(0, g_vertex_buffer, 0, sizeof(VERTEX));
      // set the current vertex stream declation
      g_d3d_device->SetFVF(VERTEX_FVF);
      // assigns a texture to a stage for a device
      g_d3d_device->SetTexture(0, g_texture);
      // renders a sequence of noindexed, geometric primitives of the specified type from the current set
      // of data input stream.
      g_d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
      // release texture
      g_d3d_device->SetTexture(0, NULL);
      // end the scene
      g_d3d_device->EndScene();
    }
    // present the contents of the next buffer in the sequence of back buffers owned by the device
    g_d3d_device->Present(NULL, NULL, NULL, NULL);
  }
  return 1;
}
//--------------------------------------------------------------------------------
// Main function, routine entry.
//--------------------------------------------------------------------------------
int test_d3d()
{
  WNDCLASSEX win_class;
  MSG msg;
  HINSTANCE inst;
  inst = GetModuleHandle(NULL);
  g_inst = inst;
  // create window class and register it
  win_class.cbSize = sizeof(win_class);
  win_class.style = CS_CLASSDC;
  win_class.lpfnWndProc = Window_Proc;
  win_class.cbClsExtra = 0;
  win_class.cbWndExtra = 0;
  win_class.hInstance = g_inst;
  win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
  win_class.hbrBackground = NULL;
  win_class.lpszMenuName = NULL;
  win_class.lpszClassName = g_class_name;
  win_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  if (! RegisterClassEx(&win_class)) {
    return FALSE;
  }
  // create the main window
  g_hwnd = CreateWindow(g_class_name, g_caption, WS_CAPTION | WS_SYSMENU, 0, 0,
      WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, inst, NULL);
  if (g_hwnd == NULL) {
    return FALSE;
  }
  Do_Init();
  ShowWindow(g_hwnd, SW_NORMAL);
  UpdateWindow(g_hwnd);
  // start message pump, waiting for signal to quit.
  ZeroMemory(&msg, sizeof(MSG));
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    // draw a frame
    if (Do_Frame1() == FALSE) {
      break;
    }
  }
  // run shutdown function
  Do_Shutdown();
  UnregisterClass(g_class_name, inst);
  return (int) msg.wParam;
}

