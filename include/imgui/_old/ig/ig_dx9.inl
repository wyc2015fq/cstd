// ImGui - standalone example application for DirectX 9
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif // WM_MOUSEWHEEL
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#ifdef __cplusplus
#define MYPTR(x)  (x)
#define MYREF(x)  *(x)
#else
#define MYPTR(x)  &(x)
#define MYREF(x)  &(x)
typedef struct CUSTOMVERTEX CUSTOMVERTEX;
#endif
// Data
static HWND g_hWnd = 0;
static INT64 g_Time = 0;
static INT64 g_TicksPerSecond = 0;
static LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;
static LPDIRECT3DTEXTURE9 g_FontTexture = NULL;
static int g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
struct CUSTOMVERTEX {
  float pos[3];
  D3DCOLOR col;
  float uv[2];
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void igDX9_RenderDrawLists(ImDrawData* draw_data)
{
  // Avoid rendering when minimized
  ImGuiIO* io = ImGui_GetIO();
  struct CUSTOMVERTEX* vtx_dst;
  ImDrawIdx* idx_dst;
  IDirect3DStateBlock9* d3d9_state_block = NULL;
  int n, i;

  if (io->DisplaySize.x <= 0.0f || io->DisplaySize.y <= 0.0f) {
    return;
  }

  // Create and grow buffers if needed
  if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount) {
    if (g_pVB) {
      IDirect3DDevice9_Release(g_pVB);
      g_pVB = NULL;
    }

    g_VertexBufferSize = draw_data->TotalVtxCount + 5000;

    if (IDirect3DDevice9_CreateVertexBuffer(g_pd3dDevice, g_VertexBufferSize * sizeof(struct CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL) < 0) {
      return;
    }
  }

  if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount) {
    if (g_pIB) {
      IDirect3DDevice9_Release(g_pIB);
      g_pIB = NULL;
    }

    g_IndexBufferSize = draw_data->TotalIdxCount + 10000;

    if (IDirect3DDevice9_CreateIndexBuffer(g_pd3dDevice, g_IndexBufferSize * sizeof(ImDrawIdx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(ImDrawIdx) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &g_pIB, NULL) < 0) {
      return;
    }
  }

  // Backup the DX9 state
  if (IDirect3DDevice9_CreateStateBlock(g_pd3dDevice, D3DSBT_ALL, &d3d9_state_block) < 0) {
    return;
  }

  // Copy and convert all vertices into a single contiguous buffer
  if (IDirect3DVertexBuffer9_Lock(g_pVB, 0, (UINT)(draw_data->TotalVtxCount * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }

  if (IDirect3DIndexBuffer9_Lock(g_pIB, 0, (UINT)(draw_data->TotalIdxCount * sizeof(ImDrawIdx)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }

  for (n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawVert* vtx_src = cmd_list->VtxBuffer;

    for (i = 0; i < cmd_list->VtxBuffer_Size; i++) {
      vtx_dst->pos[0] = vtx_src->pos.x;
      vtx_dst->pos[1] = vtx_src->pos.y;
      vtx_dst->pos[2] = 0.0f;
      vtx_dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16); // RGBA --> ARGB for DirectX9
      vtx_dst->uv[0] = vtx_src->uv.x;
      vtx_dst->uv[1] = vtx_src->uv.y;
      vtx_dst++;
      vtx_src++;
    }

    memcpy(idx_dst, cmd_list->IdxBuffer, cmd_list->IdxBuffer_Size * sizeof(ImDrawIdx));
    idx_dst += cmd_list->IdxBuffer_Size;
  }

  IDirect3DVertexBuffer9_Unlock(g_pVB);
  IDirect3DIndexBuffer9_Unlock(g_pIB);
  IDirect3DDevice9_SetStreamSource(g_pd3dDevice, 0, g_pVB, 0, sizeof(CUSTOMVERTEX));
  IDirect3DDevice9_SetIndices(g_pd3dDevice, g_pIB);
  IDirect3DDevice9_SetFVF(g_pd3dDevice, D3DFVF_CUSTOMVERTEX);
  // Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing
   IDirect3DDevice9_SetPixelShader(g_pd3dDevice, NULL);
  IDirect3DDevice9_SetVertexShader(g_pd3dDevice, NULL);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_CULLMODE, D3DCULL_NONE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_LIGHTING, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ZENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ALPHABLENDENABLE, TRUE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ALPHATESTENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_BLENDOP, D3DBLENDOP_ADD);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SCISSORTESTENABLE, TRUE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetSamplerState(g_pd3dDevice, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  IDirect3DDevice9_SetSamplerState(g_pd3dDevice, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  // Setup orthographic projection matrix
  // Being agnostic of whether <d3dx9.h> or <DirectXMath.h> can be used, we aren't relying on D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
  {
    const float L = 0.5f, R = io->DisplaySize.x + 0.5f, T = 0.5f, B = io->DisplaySize.y + 0.5f;
    D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
    D3DMATRIX mat_projection = {
      2.0f / (R - L), 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f / (T - B), 0.0f, 0.0f,
      0.0f, 0.0f, 0.5f, 0.0f,
      (L + R) / (L - R), (T + B) / (B - T), 0.5f, 1.0f,
    };
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_WORLD, &mat_identity);
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_VIEW, &mat_identity);
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_PROJECTION, &mat_projection);
  }
  // Render command lists
  {
    int vtx_offset = 0, idx_offset = 0, cmd_i;
    
    for (n = 0; n < draw_data->CmdListsCount; n++) {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];
      
      for (cmd_i = 0; cmd_i < cmd_list->CmdBuffer_Size; cmd_i++) {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
        
        if (pcmd->UserCallback) {
          pcmd->UserCallback(cmd_list, pcmd);
        }
        else {
          const RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
          IDirect3DDevice9_SetTexture(g_pd3dDevice, 0, (IDirect3DBaseTexture9*)pcmd->TextureId);
          IDirect3DDevice9_SetScissorRect(g_pd3dDevice, &r);
          IDirect3DDevice9_DrawIndexedPrimitive(g_pd3dDevice, D3DPT_TRIANGLELIST, vtx_offset, 0, (UINT)cmd_list->VtxBuffer_Size, idx_offset, pcmd->ElemCount / 3);
        }
        
        idx_offset += pcmd->ElemCount;
      }
      
      vtx_offset += cmd_list->VtxBuffer_Size;
    }
  }
  // Restore the DX9 state
  IDirect3DStateBlock9_Apply(d3d9_state_block);
  IDirect3DStateBlock9_Release(d3d9_state_block);
}

LRESULT WINAPI igDX9_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  ImGuiIO* io = ImGui_GetIO();

  switch (msg) {
  case WM_LBUTTONDOWN:
    io->MouseDown[0] = TRUE;
    return TRUE;

  case WM_LBUTTONUP:
    io->MouseDown[0] = FALSE;
    return TRUE;

  case WM_RBUTTONDOWN:
    io->MouseDown[1] = TRUE;
    return TRUE;

  case WM_RBUTTONUP:
    io->MouseDown[1] = FALSE;
    return TRUE;

  case WM_MBUTTONDOWN:
    io->MouseDown[2] = TRUE;
    return TRUE;

  case WM_MBUTTONUP:
    io->MouseDown[2] = FALSE;
    return TRUE;

  case WM_MOUSEWHEEL:
    io->MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
    return TRUE;

  case WM_MOUSEMOVE:
    io->MousePos.x = (signed short)(lParam);
    io->MousePos.y = (signed short)(lParam >> 16);
    return TRUE;

  case WM_KEYDOWN:
    if (wParam < 256) {
      io->KeysDown[wParam] = 1;
    }

    return TRUE;

  case WM_KEYUP:
    if (wParam < 256) {
      io->KeysDown[wParam] = 0;
    }

    return TRUE;

  case WM_CHAR:

    // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
    if (wParam > 0 && wParam < 0x10000) {
      ImGuiIO_AddInputCharacter(io, (unsigned short)wParam);
    }

    return TRUE;
  }

  return 0;
}

BOOL igDX9_Init(void* hwnd, IDirect3DDevice9* device)
{
  ImGuiIO* io = ImGui_GetIO();
  g_hWnd = (HWND)hwnd;
  g_pd3dDevice = device;

  if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_TicksPerSecond)) {
    return FALSE;
  }

  if (!QueryPerformanceCounter((LARGE_INTEGER*)&g_Time)) {
    return FALSE;
  }

  io->KeyMap[ImGuiKey_Tab] = VK_TAB; // Keyboard mapping. ImGui will use those indices to peek into the io->KeyDown[] array that we will update during the application lifetime.
  io->KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
  io->KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
  io->KeyMap[ImGuiKey_UpArrow] = VK_UP;
  io->KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
  io->KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
  io->KeyMap[ImGuiKey_PageDown] = VK_NEXT;
  io->KeyMap[ImGuiKey_Home] = VK_HOME;
  io->KeyMap[ImGuiKey_End] = VK_END;
  io->KeyMap[ImGuiKey_Delete] = VK_DELETE;
  io->KeyMap[ImGuiKey_Backspace] = VK_BACK;
  io->KeyMap[ImGuiKey_Enter] = VK_RETURN;
  io->KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
  io->KeyMap[ImGuiKey_A] = 'A';
  io->KeyMap[ImGuiKey_C] = 'C';
  io->KeyMap[ImGuiKey_V] = 'V';
  io->KeyMap[ImGuiKey_X] = 'X';
  io->KeyMap[ImGuiKey_Y] = 'Y';
  io->KeyMap[ImGuiKey_Z] = 'Z';

  io->RenderDrawListsFn = igDX9_RenderDrawLists; // Alternatively you can set this to NULL and call ImGui_GetDrawData() after ImGui_Render() to get the same ImDrawData pointer.
  io->ImeWindowHandle = g_hWnd;
  return TRUE;
}
void igDX9_InvalidateDeviceObjects()
{
  LPDIRECT3DTEXTURE9 tex;
  if (!g_pd3dDevice) {
    return;
  }

  if (g_pVB) {
    IDirect3DVertexBuffer9_Release(g_pVB);
    g_pVB = NULL;
  }

  if (g_pIB) {
    IDirect3DIndexBuffer9_Release(g_pIB);
    g_pIB = NULL;
  }
  if (tex = (LPDIRECT3DTEXTURE9)ImGui_GetIO()->Fonts->TexID) {
    IDirect3DTexture9_Release(tex);
    ImGui_GetIO()->Fonts->TexID = 0;
  }

  g_FontTexture = NULL;
}
void igDX9_Shutdown()
{
  igDX9_InvalidateDeviceObjects();
  //ImGui_Shutdown();
  g_pd3dDevice = NULL;
  g_hWnd = 0;
}
#if 0
static BOOL igDX9_CreateFontsTexture()
{
  // Build texture atlas
  ImGuiIO* io = ImGui_GetIO();
  unsigned char* pixels;
  int width, height, bytes_per_pixel;
  io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
  // Upload texture to graphics system
  g_FontTexture = NULL;

  if (g_pd3dDevice IDirect3DDevice9_CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_FontTexture, NULL) < 0) {
    return FALSE;
  }

  D3DLOCKED_RECT tex_locked_rect;

  if (g_FontTexture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK) {
    return FALSE;
  }

  for (int y = 0; y < height; y++) {
    memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
  }

  g_FontTexture->UnlockRect(0);
  // Store our identifier
  io->Fonts->TexID = (void*)g_FontTexture;
  return TRUE;
}
BOOL igDX9_CreateDeviceObjects()
{
  if (!g_pd3dDevice) {
    return FALSE;
  }

  if (!igDX9_CreateFontsTexture()) {
    return FALSE;
  }

  return TRUE;
}
void igDX9_NewFrame()
{
  if (!g_FontTexture) {
    igDX9_CreateDeviceObjects();
  }

  ImGuiIO* io = ImGui_GetIO();
  // Setup display size (every frame to accommodate for window resizing)
  RECT rect;
  GetClientRect(g_hWnd, &rect);
  io->DisplaySize = fVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
  // Setup time step
  INT64 current_time;
  QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
  io->DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
  g_Time = current_time;
  // Read keyboard modifiers inputs
  io->KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
  io->KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
  io->KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
  io->KeySuper = FALSE;
  // io->KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
  // io->MousePos : filled by WM_MOUSEMOVE events
  // io->MouseDown : filled by WM_*BUTTON* events
  // io->MouseWheel : filled by WM_MOUSEWHEEL events
  // Hide OS mouse cursor if ImGui is drawing it
  SetCursor(io->MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
  // Start the frame
  ImGui_NewFrame();
}
// Data
static D3DPRESENT_PARAMETERS g_d3dpp;
extern LRESULT igDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (igDX9_WndProcHandler(hWnd, msg, wParam, lParam)) {
    return TRUE;
  }

  switch (msg) {
  case WM_SIZE:
    if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
      igDX9_InvalidateDeviceObjects();
      g_d3dpp.BackBufferWidth = LOWORD(lParam);
      g_d3dpp.BackBufferHeight = HIWORD(lParam);
      HRESULT hr = g_pd3dDevice IDirect3DDevice9_Reset(&g_d3dpp);

      if (hr == D3DERR_INVALIDCALL) {
        IM_ASSERT(0);
      }

      igDX9_CreateDeviceObjects();
    }

    return 0;

  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
      return 0;
    }

    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}

int test_imgui_directx9()
{
  // Create application window
  WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("ImGui Example"), NULL };
  RegisterClassEx(&wc);
  HWND hwnd = CreateWindow(_T("ImGui Example"), _T("ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);
  // Initialize Direct3D
  LPDIRECT3D9 pD3D;

  if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
    UnregisterClass(_T("ImGui Example"), wc.hInstance);
    return 0;
  }

  ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
  g_d3dpp.Windowed = TRUE;
  g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  g_d3dpp.EnableAutoDepthStencil = TRUE;
  g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

  // Create the D3DDevice
  if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) {
    pD3D->Release();
    UnregisterClass(_T("ImGui Example"), wc.hInstance);
    return 0;
  }

  // Setup ImGui binding
  igDX9_Init(hwnd, g_pd3dDevice);
  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  //ImGuiIO* io = ImGui_GetIO();
  //io->Fonts->AddFontDefault();
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  //io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io->Fonts->GetGlyphRangesJapanese());
  BOOL show_main_window = TRUE;
  BOOL show_test_window = TRUE;
  BOOL show_another_window = FALSE;
  ImVec4 clear_col = ImColor(114, 144, 154);
  // Main loop
  MSG msg;
  ZeroMemory(&msg, sizeof(msg));
  ShowWindow(hwnd, SW_SHOWDEFAULT);
  UpdateWindow(hwnd);

  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      continue;
    }

    igDX9_NewFrame();
    // 1. Show a simple window
    // Tip: if we don't call ImGui_Begin()/ImGui_End() the widgets appears in a window automatically called "Debug"
    {
      static float f = 0.0f;
      ImGui_Begin("Main Window", &show_main_window, 0);
      ImGui_Text("Hello, world!");
      ImGui_SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui_ColorEdit3("clear color", (float*)&clear_col);

      if (ImGui_Button("Test Window")) {
        show_test_window ^= 1;
      }

      if (ImGui_Button("Another Window")) {
        show_another_window ^= 1;
      }

      ImGui_Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui_GetIO()->Framerate, ImGui_GetIO()->Framerate);
      ImGui_End();
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window) {
      ImGui_SetNextWindowSize(fVec2(200, 100), ImGuiSetCond_FirstUseEver);
      ImGui_Begin("Another Window", &show_another_window, 0);
      ImGui_Text("Hello");
      ImGui_End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui_ShowTestWindow()
    if (show_test_window) {
      ImGui_SetNextWindowPos(fVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui_ShowTestWindow(&show_test_window);
    }

    // Rendering
    IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ZENABLE, FALSE);
    IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ALPHABLENDENABLE, FALSE);
    IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SCISSORTESTENABLE, FALSE);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_col.x * 255.0f), (int)(clear_col.y * 255.0f), (int)(clear_col.z * 255.0f), (int)(clear_col.w * 255.0f));
    g_pd3dDevice IDirect3DDevice9_Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

    if (g_pd3dDevice IDirect3DDevice9_BeginScene() >= 0) {
      ImGui_Render();
      g_pd3dDevice IDirect3DDevice9_EndScene();
    }

    g_pd3dDevice IDirect3DDevice9_Present(NULL, NULL, NULL, NULL);
  }

  igDX9_Shutdown();

  if (g_pd3dDevice) {
    g_pd3dDevice IDirect3DDevice9_Release();
  }

  if (pD3D) {
    pD3D->Release();
  }

  UnregisterClass(_T("ImGui Example"), wc.hInstance);
  return 0;
}


#endif
