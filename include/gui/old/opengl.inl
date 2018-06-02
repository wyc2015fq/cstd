
#include <GL/gl.h>

//#pragma comment (lib, "opengl32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

typedef struct opengl_rander_t {
  rander_funcs_t* funcs;
  HDC hDC;
  HGLRC hGLRC;
  HWND hWnd;
  int AntiAlias;
  ECOLOR_FORMAT ColorFormat;
} opengl_rander_t;

void opengl_SetRenderState(opengl_rander_t* s, int type, int b) {
  switch (type) {
  case EMF_LIGHTING:
    break;
  default:
    ASSERT(0);
    break;
  }
}
void opengl_Close(opengl_rander_t* s)
{
  wglMakeCurrent(s->hDC, NULL);
  wglDeleteContext(s->hGLRC);
  ReleaseDC(s->hWnd, s->hDC);
  s->hGLRC = NULL;
  s->hDC = NULL;
}
void opengl_Clear(opengl_rander_t* s, int Count, const IRECT* pRects, DWORD Flags, COLOR color)
{
  GLbitfield mask = 0;

  if (Flags & EDCLEAR_TARGET) {
    const f32 inv = 1.0f / 255.0f;
    glClearColor(GetRV(color) * inv, GetGV(color) * inv, GetBV(color) * inv, GetAV(color) * inv);
    mask |= GL_COLOR_BUFFER_BIT;
  }

  if (Flags & EDCLEAR_ZBUFFER) {
    glDepthMask(GL_TRUE);
    //LastMaterial.ZWriteEnable = true;
    mask |= GL_DEPTH_BUFFER_BIT;
  }

  if (Flags & EDCLEAR_STENCIL) {
    mask |= GL_STENCIL_BUFFER_BIT;
  }

  if (mask) {
    glClear(mask);
  }
}
void opengl_BeginScene(opengl_rander_t* s)
{
}
void opengl_EndScene(opengl_rander_t* s)
{
  glFlush();
  SwapBuffers(s->hDC);
}
void opengl_DrawPrimitiveUP(opengl_rander_t* s, E_PRIMITIVE_TYPE t, int count, const void* vertex, int stride) {
  int i;
  const uchar* vec = (const uchar*)vertex;
  switch(t) {
  case EPT_LINES:
    glBegin(GL_LINES);
    for ( i=0; i<count*2; ++i ) {
      glVertex3fv((float *)vec);
      vec += stride;
    }
    glEnd();
    break;
  case EPT_POINTS:
    glBegin(GL_POINTS);
    for ( i=0; i<count; ++i ) {
      glVertex3fv((float *)vec);
    }
    glEnd();
    break;
  default:
    ASSERT(0);
    break;
  }
}
opengl_rander_t* opengl_init(void* WindowId, rander_params_t* Params)
{
  opengl_rander_t s[1] = {0};
  int multisamples = 0;
  static rander_funcs_t opengl_funcs[1] = {0};
  static int inited=0;
  MYREALLOC(s, 1);
  
  if (!inited) {
    RANDER_FUNCS_SET(opengl_funcs, opengl);
    opengl_funcs->dt = EDT_OPENGL;
    inited = 1;
  }
  
  MEMSET(s, 0, 1);
  s->funcs = opengl_funcs;
  s->hWnd = (HWND)WindowId;
  s->hDC = GetDC(s->hWnd);
  
  if (Params->MultiSamples) {
  }
  else {
    int pixelformat;
    PIXELFORMATDESCRIPTOR pfd[1] = {0};
    pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd->nVersion = 1;
    pfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
    pfd->iPixelType = PFD_TYPE_RGBA;
    pfd->cDepthBits = 16; // 24 bits zbuffer
    pfd->cStencilBits = 8; // 8 bits stencil buffer
    pfd->iLayerType = PFD_MAIN_PLANE; // main layer
    pixelformat = ChoosePixelFormat(s->hDC, pfd);
    if (pixelformat == 0) {
      return false;
    }
    
    if (SetPixelFormat(s->hDC, pixelformat, pfd) == FALSE) {
      ReleaseDC(s->hWnd, s->hDC);
      return false;
    }
    
    {utime_start(_start_time);
    s->hGLRC = wglCreateContext(s->hDC);
    printf("%f\n", utime_elapsed(_start_time));}
    wglMakeCurrent(s->hDC, s->hGLRC);
  }
  
  return (opengl_rander_t*)memdup(s, sizeof(*s));
}
