#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#define gl2_color(clr) ImRGBAToBGRA(clr)
typedef struct {
  ImDrawList DrawList[1];
  IPOINT DisplaySize;
  HDC hDC; //GDI绘图中使用的的设备环境句柄
  HGLRC hRC; //OpenGL渲染时使用的渲染环境句柄
} imui_gl2_t;
IM_STATIC int gl2_SetTexture(ImGuiHost* host, texture_t* tex, int fmt, int height, int width, void* data, int step)
{
  GLint last_texture = 0;
  GLuint g_FontTexture = 0;
  int glfmt = fmt == T_BGRA ? GL_RGBA : GL_RGB;
  if (height != tex->h || width != tex->w || fmt != tex->fmt) {
    g_FontTexture = (GLuint)tex->tex;
    if (g_FontTexture) {
      glDeleteTextures(1, &g_FontTexture);
      tex->tex = 0;
    }
  }
  if (height * width > 0 && data) {
    // Build texture atlas
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, glfmt, GL_UNSIGNED_BYTE, data);
    // Store our identifier
    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    tex->tex = (void*)g_FontTexture;
    tex->h = height;
    tex->w = width;
    tex->fmt = fmt;
  }
  return 0;
}
IM_STATIC int gl2_CreateFont(ImGuiHost* host, ImFont* f, const char* name, int size)
{
  // Build texture atlas
  imui_gl2_t* g = (imui_gl2_t*)host->Driver;
  int width, height;
  uchar* pixels = NULL;
  texfont_t* font = NULL;
  int y = 0;
  GLuint g_FontTexture = 0;
  myReAlloc(font, 1);
  //texfont_Init_Default(font);
  packing_raster_font(songti_12, font);
  pixels = (uchar*)font->TexPixelsRGBA32;
  width = font->TexWidth;
  height = font->TexHeight;
  {
    GLint last_texture = 0;
    // Build texture atlas
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // Store our identifier
    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
  }
  // Store our identifier
  font->TexID = (void*)g_FontTexture;
  f->font = font;
  return font != NULL;
}
IM_STATIC int gl2_DeleteFont(ImGuiHost* host, ImFont* f)
{
  texfont_t* font = (texfont_t*)f->font;
  if (font) {
    GLuint g_FontTexture = (GLuint)font->TexID;
    if (g_FontTexture) {
      glDeleteTextures(1, &g_FontTexture);
      font->TexID = 0;
    }
    texfont_Free(font);
    f->font = NULL;
    f->size = 0;
  }
  return 0;
}
IM_STATIC ImFont* gl2_Font_opt(ImGuiHost* host, int opt, ImFontID id, const char* name, int size)
{
  static ImFont fontx[100] = {0};
  return ImGuiFont_opt(opt, host, fontx, 100, id, name, size, gl2_CreateFont, gl2_DeleteFont);
}
void gl2_InvalidateDeviceObjects(ImGuiHost* host)
{
  gl2_Font_opt(host, 2, -1, NULL, 0);
}
void gl2_RenderDrawLists(ImGuiHost* host, ImDrawList* cmd_list)
{
  imui_gl2_t* g = (imui_gl2_t*)host->Driver;
  int fb_width = g->DisplaySize.x;
  int fb_height = g->DisplaySize.y;
  if (fb_width == 0 || fb_height == 0) {
    return;
  }
  //cmd_list->ScaleClipRects(io->DisplayFramebufferScale);
  // We are using the OpenGL fixed pipeline to make the example code simpler to read!
  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  GLint last_viewport[4];
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLint last_scissor_box[4];
  glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnable(GL_TEXTURE_2D);
  //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context
  // Setup viewport, orthographic projection matrix
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, g->DisplaySize.x, g->DisplaySize.y, 0.0f, -1.0f, +1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  // Render command lists
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
  {
    int cmd_i;
    ImVertex* vtx_buffer = cmd_list->VtxBuffer;
    const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer;
    int i;
    for (i = 0; i < cmd_list->VtxBuffer_Size; ++i) {
      COLOR clr = vtx_buffer[i].col;
      vtx_buffer[i].col = gl2_color(clr);
    }
    glVertexPointer(2, GL_FLOAT, sizeof(ImVertex), (void*)((char*)vtx_buffer + OFFSETOF(ImVertex, pos)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(ImVertex), (void*)((char*)vtx_buffer + OFFSETOF(ImVertex, uv)));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImVertex), (void*)((char*)vtx_buffer + OFFSETOF(ImVertex, col)));
    for (cmd_i = 0; cmd_i < cmd_list->CmdBuffer_Size; cmd_i++) {
      const drawcmd_t* pcmd = &cmd_list->CmdBuffer[cmd_i];
      glBindTexture(GL_TEXTURE_2D, (GLuint)pcmd->TextureId);
      glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
      glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
      idx_buffer += pcmd->ElemCount;
    }
  }
#undef OFFSETOF
  // Restore modified state
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
  glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}
int gl2_Rander(ImGuiHost* host, const gc_t* ctx)
{
  imui_gl2_t* g = (imui_gl2_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  int display_w, display_h;
  COLOR clr = (host->clrbg);
  RECT area;
  GetClientRect(s->g_hWnd, &area);
  g->DisplaySize.x = display_w = area.right;
  g->DisplaySize.y = display_h = area.bottom;
  glViewport(0, 0, display_w, display_h);
  //float r = GetRV(clr) * (1.f / 255), g1 = GetGV(clr) * (1.f / 255), b = GetGV(clr) * (1.f / 255), a = GetAV(clr) * (1.f / 255);
  glClearColor(GetRV(clr) * (1.f / 255), GetGV(clr) * (1.f / 255), GetGV(clr) * (1.f / 255), GetAV(clr) * (1.f / 255));
  glClear(GL_COLOR_BUFFER_BIT);
  if (1) {
    const char* va = ctx->buf;
    const char* vb = ctx->buf + ctx->len;
    trirun(g->DrawList, va, vb);
    gl2_RenderDrawLists(host, g->DrawList);
    triClear(g->DrawList);
  }
  SwapBuffers(g->hDC);
  return 0;
}
IM_STATIC int gl2_measureText(ImGuiHost* host, const char* text, const char* text_end, ImFontID font_id, int formatFlags, IRECT* prc)
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
LRESULT gl2_WndProc(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  imui_gl2_t* g = (imui_gl2_t*)host->Driver;
  switch (msg) {
  case WM_SIZE:
    return 0;
  }
  return 0;
}
int gl2_UnInit(ImGuiHost* host)
{
  imui_gl2_t* g = (imui_gl2_t*)host->Driver;
  imui_win32_t* s = (imui_win32_t*)host->Window;
  gl2_InvalidateDeviceObjects(host);
  //imuiShutdown();
  wglMakeCurrent(g->hDC, NULL);
  wglDeleteContext(g->hRC);
  ReleaseDC(s->g_hWnd, g->hDC);
  return 0;
}
int gl2_Init(ImGuiHost* host)
{
  imui_win32_t* s = (imui_win32_t*)host->Window;
  imui_gl2_t* g = NULL;
  myReAlloc(g, 1);
  host->Driver = g;
  host->DrvUnInit = gl2_UnInit;
  host->DrvRander = gl2_Rander;
  host->DrvSetTexture = gl2_SetTexture;
  host->DrvFont_opt = gl2_Font_opt;
  host->DrvMeasureText = gl2_measureText;
  s->DrvWndProc = gl2_WndProc;
  g->hDC = GetDC(s->g_hWnd);
  {
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), // pfd结构的大小
      1, // 版本号
      PFD_DRAW_TO_WINDOW | // 支持在窗口中绘图
      PFD_SUPPORT_OPENGL | // 支持OpenGL
      PFD_DOUBLEBUFFER, // 双缓存模式
      PFD_TYPE_RGBA, // RGBA 颜色模式
      24, // 24 位颜色深度
      0, 0, 0, 0, 0, 0, // 忽略颜色位
      0, // 没有非透明度缓存
      0, // 忽略移位位
      0, // 无累加缓存
      0, 0, 0, 0, // 忽略累加位
      32, // 32 位深度缓存
      0, // 无模板缓存
      0, // 无辅助缓存
      PFD_MAIN_PLANE, // 主层
      0, // 保留
      0, 0, 0 // 忽略层,可见性和损毁掩模
    };
    int nPixelFormat; // 像素点格式
    if (!(nPixelFormat = ChoosePixelFormat(g->hDC, &pfd))) {
      // (NULL,"can not find proper mode", "Error",MB_OK|MB_ICONEXCLAMATION);
      return FALSE;
    }
    SetPixelFormat(g->hDC, nPixelFormat, &pfd);
    g->hRC = wglCreateContext(g->hDC); //利用GDI绘图所使用的HDC创建对应的HGLRC
    wglMakeCurrent(g->hDC, g->hRC); //使OpenGL绘图所使用的HGLRC为当前绘图工具
  }
  return true;
}

