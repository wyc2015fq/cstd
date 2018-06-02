#pragma comment(lib, "d3d9.lib")
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>
//#include <tchar.h>
#define d3d9_color(clr) (clr)
typedef struct {
  LPDIRECT3DDEVICE9 g_pd3dDevice;
  LPDIRECT3DVERTEXBUFFER9 g_pVB;
  LPDIRECT3DINDEXBUFFER9 g_pIB;
  int g_VertexBufferSize, g_IndexBufferSize;
  D3DPRESENT_PARAMETERS g_d3dpp;
  ImDrawList DrawList[1];
  char* vtxtype;
} imui_d3d9_t;
IM_STATIC int d3d9_StrFormatTrans(int formatFlags)
{
  int Fmt = 0, Align = 0, VAlign = 0;
#define FLAGSET(x, a, b) if (formatFlags&a) {x|=b;}
#if 0
  FLAGSET(Fmt, TF_RightToLeft, StringFormatFlagsDirectionRightToLeft);
  FLAGSET(Fmt, TF_Vertical, StringFormatFlagsDirectionVertical);
  FLAGSET(Fmt, TF_NoFitBlackBox, StringFormatFlagsNoFitBlackBox);
  FLAGSET(Fmt, TF_DisplayFormatControl, StringFormatFlagsDisplayFormatControl);
  FLAGSET(Fmt, TF_NoFontFallback, StringFormatFlagsNoFontFallback);
  FLAGSET(Fmt, TF_MeasureTrailingSpaces, StringFormatFlagsMeasureTrailingSpaces);
  FLAGSET(Fmt, TF_NoWrap, StringFormatFlagsNoWrap);
  FLAGSET(Fmt, TF_LineLimit, StringFormatFlagsLineLimit);
  FLAGSET(Fmt, TF_NoClip, StringFormatFlagsNoClip);
  FLAGSET(Fmt, TF_WrapWidgetWidth, 0);
  FLAGSET(Fmt, TF_WrapFixedColumnWidth, 0);
  FLAGSET(Fmt, TF_WrapAtWhiteSpace, 0);
  FLAGSET(Fmt, TF_WrapAnyWhere, 0);
#endif
  FLAGSET(Align, TF_LEFT, DT_LEFT);
  FLAGSET(Align, TF_CENTER, DT_CENTER);
  FLAGSET(Align, TF_RIGHT, DT_RIGHT);
  FLAGSET(VAlign, TF_TOP, DT_TOP);
  FLAGSET(VAlign, TF_VCENTER, DT_VCENTER);
  FLAGSET(VAlign, TF_BOTTOM, DT_BOTTOM);
#undef FLAGSET
  return Fmt | Align | VAlign;
}
IM_STATIC int d3d9_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* prc)
{
  texfont_t* font = NULL;
  IRECT rc = prc ? *prc : iRECT2(0, 0, 10000, 10000);
  float w = RCW(&rc);
  ImVec2 sz;
  imuiGetFontByID(font_id, &font, 0);
  sz = texfont_CalcTextSizeA(font, font->FontSize, w, w, text, text_end, NULL);
  iRectAlign(rc, (int)sz.x, (int)sz.y, formatFlags, &rc);
  if (prc) {
    *prc = rc;
  }
  return 0;
}
IM_STATIC int d3d9_SetTexture(ImGuiHost* host, texture_t* tex, int fmt, int height, int width, void* data, int step)
{
  if (height != tex->h || width != tex->w || fmt != tex->fmt) {
    LPDIRECT3DTEXTURE9 pFontTexture = (LPDIRECT3DTEXTURE9)tex->tex;
    if (pFontTexture) {
      IDirect3DTexture9_Release(pFontTexture);
      tex->tex = NULL;
      memset(tex, 0, sizeof(*tex));
    }
    if ((height * width) > 0) {
#define D3DFMT_CONVERT(fmt, d3dfmt) case fmt: df = d3dfmt; break;
      imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
      D3DFORMAT df = D3DFMT_A8R8G8B8;
      LPDIRECT3DTEXTURE9 pFontTexture = NULL;
      switch (fmt) {
        D3DFMT_CONVERT(T_RGB, D3DFMT_R8G8B8);
        D3DFMT_CONVERT(T_BGR, D3DFMT_R8G8B8);
        D3DFMT_CONVERT(T_BGRA, D3DFMT_A8R8G8B8);
      default:
        ASSERT(0);
        break;
      }
      if (IDirect3DDevice9_CreateTexture(g->g_pd3dDevice, width, height, 1, D3DUSAGE_DYNAMIC, df, D3DPOOL_DEFAULT, &pFontTexture, NULL) < 0) {
        return 0;
      }
#undef D3DFMT_CONVERT
      tex->tex = pFontTexture;
      tex->h = height;
      tex->w = width;
      tex->fmt = fmt;
    }
  }
  if (tex->tex && data) {
    int y;
    uchar* pixels = (uchar*)data;
    D3DLOCKED_RECT tex_locked_rect = {0};
    LPDIRECT3DTEXTURE9 pFontTexture = (LPDIRECT3DTEXTURE9)tex->tex;
    int bytes_per_pixel = tex->fmt == T_BGRA ? 4 : 3;
    if (IDirect3DTexture9_LockRect(pFontTexture, 0, &tex_locked_rect, NULL, 0) != D3D_OK) {
      return false;
    }
    for (y = 0; y < height; y++) {
      memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + step * y, (width * bytes_per_pixel));
    }
    IDirect3DTexture9_UnlockRect(pFontTexture, 0);
  }
  return 0;
}
IM_STATIC int d3d9_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  // Build texture atlas
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  int width, height, bytes_per_pixel;
  uchar* pixels = NULL;
  texfont_t* font = NULL;
  LPDIRECT3DTEXTURE9 pFontTexture = NULL;
  D3DLOCKED_RECT tex_locked_rect = {0};
  int y = 0;
  myReAlloc(font, 1);
  //texfont_Init_Default(font);
  packing_raster_font(songti_12, font);
  pixels = (uchar*)font->TexPixelsRGBA32;
  width = font->TexWidth;
  height = font->TexHeight;
  bytes_per_pixel = 4;
  // Upload texture to graphics system
  if (IDirect3DDevice9_CreateTexture(g->g_pd3dDevice, width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pFontTexture, NULL) < 0) {
    return false;
  }
  if (IDirect3DTexture9_LockRect(pFontTexture, 0, &tex_locked_rect, NULL, 0) != D3D_OK) {
    return false;
  }
  for (y = 0; y < height; y++) {
    memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
  }
  IDirect3DTexture9_UnlockRect(pFontTexture, 0);
  // Store our identifier
  font->TexID = (void*)pFontTexture;
  f->font = font;
  return font != NULL;
}
IM_STATIC int d3d9_DeleteFont(ImGuiHost* host, ImFont* f)
{
  texfont_t* font = (texfont_t*)f->font;
  LPDIRECT3DTEXTURE9 tex = NULL;
  if (font) {
    if (tex = (LPDIRECT3DTEXTURE9)font->TexID) {
      IDirect3DTexture9_Release(tex);
      font->TexID = 0;
    }
    texfont_Free(font);
    f->font = NULL;
    f->size = 0;
  }
  return 0;
}
#define D3DFVF_CUSTOMVERTEX_TEX (D3DFVF_XYZ|D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE)
#define VERTEX_FVF (D3DFVF_XYZ | D3DFVF_TEX1)
void d3d9_RenderDrawLists(ImGuiHost* host, ImDrawList* cmd_list)
{
  typedef struct {
    float pos[3];
    D3DCOLOR col;
    float uv[2];
  } CUSTOMVERTEX;
  typedef struct {
    float pos[3];
    float uv[2];
    D3DCOLOR col;
  } CUSTOMVERTEX_TEX;
  // Avoid rendering when minimized
  CUSTOMVERTEX* vtx_dst;
  ImDrawIdx* idx_dst;
  IDirect3DStateBlock9* d3d9_state_block = NULL;
  int i;
  int idx_offset = 0, cmd_i;
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  if (RCW(&host->rootWindow->rect) <= 0 || RCH(&host->rootWindow->rect) <= 0) {
    return;
  }
  MYREALLOC(g->vtxtype, g->g_VertexBufferSize);
  // Create and grow buffers if needed
  if (!g->g_pVB || g->g_VertexBufferSize < cmd_list->VtxBuffer_Size) {
    if (g->g_pVB) {
      IDirect3DDevice9_Release(g->g_pVB);
      g->g_pVB = NULL;
    }
    g->g_VertexBufferSize = cmd_list->VtxBuffer_Size + 5000;
    if (IDirect3DDevice9_CreateVertexBuffer(g->g_pd3dDevice, g->g_VertexBufferSize * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g->g_pVB, NULL) < 0) {
      return;
    }
  }
  if (!g->g_pIB || g->g_IndexBufferSize < cmd_list->IdxBuffer_Size) {
    if (g->g_pIB) {
      IDirect3DDevice9_Release(g->g_pIB);
      g->g_pIB = NULL;
    }
    g->g_IndexBufferSize = cmd_list->IdxBuffer_Size + 10000;
    if (IDirect3DDevice9_CreateIndexBuffer(g->g_pd3dDevice, g->g_IndexBufferSize * sizeof(ImDrawIdx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(ImDrawIdx) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &g->g_pIB, NULL) < 0) {
      return;
    }
  }
  // Backup the DX9 state
  if (IDirect3DDevice9_CreateStateBlock(g->g_pd3dDevice, D3DSBT_ALL, &d3d9_state_block) < 0) {
    return;
  }
  // Copy and convert all vertices into a single contiguous buffer
  if (IDirect3DVertexBuffer9_Lock(g->g_pVB, 0, (UINT)(cmd_list->VtxBuffer_Size * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }
  if (IDirect3DIndexBuffer9_Lock(g->g_pIB, 0, (UINT)(cmd_list->IdxBuffer_Size * sizeof(ImDrawIdx)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }
  {
    int j;
    idx_offset = 0;
    for (cmd_i = 0; cmd_i < cmd_list->CmdBuffer_Size; cmd_i++) {
      const drawcmd_t* pcmd = cmd_list->CmdBuffer + cmd_i;
      for (j = 0; j < pcmd->ElemCount; ++j) {
        int off = cmd_list->IdxBuffer[idx_offset + j];
        g->vtxtype[off] = pcmd->vtxtype;
      }
      idx_offset += pcmd->ElemCount;
    }
    for (i = 0; i < cmd_list->VtxBuffer_Size; i++) {
      const ImVertex* vtx_src = cmd_list->VtxBuffer + i;
      if (g->vtxtype[i]) {
        CUSTOMVERTEX_TEX* dst = (CUSTOMVERTEX_TEX*)vtx_dst + i;
        dst->pos[0] = vtx_src->pos.x;
        dst->pos[1] = vtx_src->pos.y;
        dst->pos[2] = 0.0f;
        //printf("%d %f %f %f %f\n", i, vtx_src->pos.x, vtx_src->pos.y, vtx_src->uv.x, vtx_src->uv.y);
        // ARGB -> ABGR
        //dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16);
        dst->col = d3d9_color(vtx_src->col);
        dst->uv[0] = vtx_src->uv.x;
        dst->uv[1] = vtx_src->uv.y;
      }
      else {
        CUSTOMVERTEX* dst = vtx_dst + i;
        dst->pos[0] = vtx_src->pos.x;
        dst->pos[1] = vtx_src->pos.y;
        dst->pos[2] = 0.0f;
        //printf("%d %f %f %f %f\n", i, vtx_src->pos.x, vtx_src->pos.y, vtx_src->uv.x, vtx_src->uv.y);
        // ARGB -> ABGR
        //dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16);
        dst->col = d3d9_color(vtx_src->col);
        dst->uv[0] = vtx_src->uv.x;
        dst->uv[1] = vtx_src->uv.y;
      }
    }
    memcpy(idx_dst, cmd_list->IdxBuffer, cmd_list->IdxBuffer_Size * sizeof(ImDrawIdx));
    //for (i = 0; i < cmd_list->IdxBuffer_Size; i+=3) { printf("%d %d %d\n", idx_dst[i+0], idx_dst[i+1], idx_dst[i+2]); }
    idx_dst += cmd_list->IdxBuffer_Size;
  }
  IDirect3DVertexBuffer9_Unlock(g->g_pVB);
  IDirect3DIndexBuffer9_Unlock(g->g_pIB);
  IDirect3DDevice9_SetStreamSource(g->g_pd3dDevice, 0, g->g_pVB, 0, sizeof(CUSTOMVERTEX));
  IDirect3DDevice9_SetIndices(g->g_pd3dDevice, g->g_pIB);
  IDirect3DDevice9_SetFVF(g->g_pd3dDevice, D3DFVF_CUSTOMVERTEX);
  IDirect3DDevice9_SetFVF(g->g_pd3dDevice, D3DFVF_CUSTOMVERTEX_TEX);
  // Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing
  IDirect3DDevice9_SetPixelShader(g->g_pd3dDevice, NULL);
  IDirect3DDevice9_SetVertexShader(g->g_pd3dDevice, NULL);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_CULLMODE, D3DCULL_NONE);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_LIGHTING, FALSE);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_ZENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_ALPHABLENDENABLE, TRUE);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_ALPHATESTENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_BLENDOP, D3DBLENDOP_ADD);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_SCISSORTESTENABLE, TRUE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g->g_pd3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetSamplerState(g->g_pd3dDevice, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  IDirect3DDevice9_SetSamplerState(g->g_pd3dDevice, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  // Setup orthographic projection matrix
  // Being agnostic of whether <d3dx9.h> or <DirectXMath.h> can be used, we aren't relying on D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
  {
    IRECT* rc = &host->rootWindow->rect;
    const float L = rc->l + 0.5f, R = rc->r + 0.5f, T = rc->t + 0.5f, B = rc->b + 0.5f;
    D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
    D3DMATRIX mat_projection = {
      2.0f / (R - L), 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f / (T - B), 0.0f, 0.0f,
      0.0f, 0.0f, 0.5f, 0.0f,
      (L + R) / (L - R), (T + B) / (B - T), 0.5f, 1.0f,
    };
    IDirect3DDevice9_SetTransform(g->g_pd3dDevice, D3DTS_WORLD, &mat_identity);
    IDirect3DDevice9_SetTransform(g->g_pd3dDevice, D3DTS_VIEW, &mat_identity);
    IDirect3DDevice9_SetTransform(g->g_pd3dDevice, D3DTS_PROJECTION, &mat_projection);
  }
  // Render command lists
  if (1) {
    IDirect3DDevice9_SetFVF(g->g_pd3dDevice, D3DFVF_CUSTOMVERTEX);
    idx_offset = 0;
    for (cmd_i = 0; cmd_i < cmd_list->CmdBuffer_Size; cmd_i++) {
      const drawcmd_t* pcmd = cmd_list->CmdBuffer + cmd_i;
      const RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
      IDirect3DDevice9_SetFVF(g->g_pd3dDevice, pcmd->vtxtype ? D3DFVF_CUSTOMVERTEX_TEX : D3DFVF_CUSTOMVERTEX);
      IDirect3DDevice9_SetTexture(g->g_pd3dDevice, 0, (IDirect3DBaseTexture9*)pcmd->TextureId);
      IDirect3DDevice9_SetScissorRect(g->g_pd3dDevice, &r);
      IDirect3DDevice9_DrawIndexedPrimitive(g->g_pd3dDevice, D3DPT_TRIANGLELIST, 0, 0, (UINT)cmd_list->VtxBuffer_Size, idx_offset, pcmd->ElemCount / 3);
      idx_offset += pcmd->ElemCount;
    }
    //printf("VtxBuffer_Size = %d\n", cmd_list->VtxBuffer_Size);
  }
  // Restore the DX9 state
  IDirect3DStateBlock9_Apply(d3d9_state_block);
  IDirect3DStateBlock9_Release(d3d9_state_block);
}
int d3d9_Rander(ImGuiHost* host, const gc_t* ctx)
{
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_ZENABLE, false);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_ALPHABLENDENABLE, false);
  IDirect3DDevice9_SetRenderState(g->g_pd3dDevice, D3DRS_SCISSORTESTENABLE, false);
  IDirect3DDevice9_Clear(g->g_pd3dDevice, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3d9_color(host->clrbg), 1.0f, 0);
  if (IDirect3DDevice9_BeginScene(g->g_pd3dDevice) >= 0) {
    const char* va = ctx->buf;
    const char* vb = ctx->buf + ctx->len;
    trirun(g->DrawList, va, vb);
    d3d9_RenderDrawLists(host, g->DrawList);
    triClear(g->DrawList);
    IDirect3DDevice9_EndScene(g->g_pd3dDevice);
  }
  IDirect3DDevice9_Present(g->g_pd3dDevice, NULL, NULL, NULL, NULL);
  return 0;
}
IM_STATIC ImFont* d3d9_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, d3d9_CreateFont, d3d9_DeleteFont);
}
void d3d9_InvalidateDeviceObjects(ImGuiHost* host)
{
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  LPDIRECT3DTEXTURE9 tex = NULL;
  if (!g->g_pd3dDevice) {
    return;
  }
  if (g->g_pVB) {
    IDirect3DVertexBuffer9_Release(g->g_pVB);
    g->g_pVB = NULL;
  }
  if (g->g_pIB) {
    IDirect3DIndexBuffer9_Release(g->g_pIB);
    g->g_pIB = NULL;
  }
  if (g->vtxtype) {
    free(g->vtxtype);
    g->vtxtype = NULL;
  }
  d3d9_Font_opt(host, 2, -1, NULL, 0);
}
// Data
LRESULT d3d9_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  switch (msg) {
  case WM_SIZE:
    if (g->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
      HRESULT hr;
      d3d9_InvalidateDeviceObjects(host);
      g->g_d3dpp.BackBufferWidth = LOWORD(lParam);
      g->g_d3dpp.BackBufferHeight = HIWORD(lParam);
      hr = IDirect3DDevice9_Reset(g->g_pd3dDevice, &g->g_d3dpp);
      if (hr == D3DERR_INVALIDCALL) {
        IM_ASSERT(0);
      }
    }
    return 0;
  }
  return 0;
}
int d3d9_UnInit(ImGuiHost* host)
{
  imui_d3d9_t* g = (imui_d3d9_t*)host->Driver;
  d3d9_InvalidateDeviceObjects(host);
  //imuiShutdown();
  g->g_pd3dDevice = NULL;
  if (g->g_pd3dDevice) {
    IDirect3DDevice9_Release(g->g_pd3dDevice);
    g->g_pd3dDevice = NULL;
  }
  memset(g, 0, sizeof(*g));
  return 0;
}
LPDIRECT3D9 d3d9_opt(int opt)
{
  static LPDIRECT3D9 pD3D = NULL;
  if (opt) {
    if (NULL == pD3D) {
      if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
        return 0;
      }
    }
  }
  else {
    if (pD3D) {
      IDirect3D9_Release(pD3D);
      pD3D = NULL;
    }
  }
  return pD3D;
}
int d3d9_Init(ImGuiHost* host)
{
  LPDIRECT3D9 pD3D = d3d9_opt(1);
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_d3d9_t* g = NULL;
  myReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = d3d9_UnInit;
  host->DrvRander = d3d9_Rander;
  host->DrvSetTexture = d3d9_SetTexture;
  host->DrvFont_opt = d3d9_Font_opt;
  host->DrvMeasureText = d3d9_measureText;
  s->DrvWndProc = d3d9_WndProc;
  //io->measureText = d3d9_measureText;
  g->g_VertexBufferSize = 5000, g->g_IndexBufferSize = 10000;
  // Initialize Direct3D
  ZeroMemory(&g->g_d3dpp, sizeof(g->g_d3dpp));
  g->g_d3dpp.Windowed = TRUE;
  g->g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  g->g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  g->g_d3dpp.EnableAutoDepthStencil = TRUE;
  g->g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  g->g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  // Create the D3DDevice
  if (IDirect3D9_CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, s->g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g->g_d3dpp, &g->g_pd3dDevice) < 0) {
    //UnregisterClass(_T("ImGui Example"), wc.hInstance);
    return 0;
  }
  //d3d9_Font_opt(host, 1, 0, NULL, 0);
  //d3d9_opt(0);
  //d3d9_RenderDrawLists;
  return 1;
}

