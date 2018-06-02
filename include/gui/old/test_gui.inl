
#include "cstd.h"
#include "gui.inl"
#include "CMD2.h"
//#include "cmath.h"
#include "math3d.h"
#include "draw.h"
#include "Render.inl"
//#include "soft3D.h"
#include "sys/win32.inl"
#include "img/imgio.inl"
//#include "glop.inl"

void init_texture(img_t* im)
{
  COLOR clrs[] = {0xffffff, 0xffefbc3f};
  imsetsize(im, 256, 256, 3, 1);
  immap_linear_d(im, 1. / 32, 1. / 32, 2, clrs);
}
int on_event1(void* s, uint uMsg, uint wParam, uint lParam)
{
  EEVENT_TYPE msgtype = (EEVENT_TYPE)(uMsg >> 16);
  int uSubMsg = uMsg & 0xffff;

  switch (msgtype) {
  case EET_GUI_EVENT:
    switch (uSubMsg) {
    case EGET_PAINT:
      break;
    }

    break;

  case EET_MOUSE_EVENT:
    break;

  case EET_KEY_EVENT:
    if (uSubMsg) {
      switch (wParam) {
      case MAKE_KEY(EMBSM_CONTROL, KEY_KEY_A):
        printf("key %d\n", wParam);
        break;

      case KEY_KEY_A:
        printf("key %d\n", wParam);
        break;
      }
    }

    break;
  }

  return 0;
}

#if 0
typedef struct {
  V4 pos;
  float2 tc;
  V4 color;
  float rhw;
} vertex_t;

vertex_t mesh[8] = {
  { {  1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
  { { -1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
  { { -1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
  { {  1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
  { {  1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
  { { -1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
  { { -1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
  { {  1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};
#endif

int test_point(rander_t* s) {
  float vertex[] = {0.0f, 0.0f, 0.0f};
  gClear(s, 0, 0, EDCLEAR_TARGET | EDCLEAR_ZBUFFER, _ARGB(0, 0, 0, 0));
  gBeginScene(s);
  gDrawPrimitiveUP(s, EPT_POINTS, 1, vertex, 3 * sizeof(float));
  gEndScene(s);
  return 0;
}
int test_orthogonal_view(rander_t* s) {
  // `金字塔形的8条边线`
  static float g_vertices[] = 
  {
    -1.0f, 1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      
      -1.0f,-1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      
      1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      
      0.0f, 0.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      
      0.0f, 0.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      
      0.0f, 0.0f, 1.0f,
      1.0f,-1.0f,-1.0f,
      
      0.0f, 0.0f, 1.0f,
      1.0f, 1.0f,-1.0f,
  };
  // `镜头位置`
  static float g_eye[] = {0.0f,0.0f,5.0f}; 
  // `镜头对准的点`
  static float g_lookat[] = {0.0f, 0.0f, 0.0f}; 
  // `镜头正上方的方向`
  static float g_up[] = {0.0f, 1.0f,0.0f}; 
  M4 view_matrix;
  M4 orthogonal_matrix;
  M4 view_orthogonal_matrix;
  float4 vertices[16];
  int i;
  
  gSetRenderState(s, EMF_LIGHTING, FALSE );
  gClear(s, 0, 0, EDCLEAR_TARGET | EDCLEAR_ZBUFFER, _ARGB(0, 0, 0, 0));
  
  // `计算出一个可以转换到镜头坐标系的矩阵`
  GutMatrixLookAtRH(view_matrix, g_eye, g_lookat, g_up);
  // `计算出一个使用正交投影的矩阵`
  gGutMatrixOrthoRH(s, orthogonal_matrix, 5.0f, 5.0f, 0.1f, 100.0f);
  // `把这两个矩阵相乘`
  M4Mul(view_orthogonal_matrix, view_matrix, orthogonal_matrix);
  // `把空间中的坐标点转换到屏幕坐标系上`
  for ( i=0; i<16; i++ )
  {
    M4Apply3(vertices[i], g_vertices +i*3, view_orthogonal_matrix);
    V4Scale(vertices[i], vertices[i], 1.f/vertices[i][3]);
  }
  
  gBeginScene(s); // `开始下绘图指令`
  // `画出金字塔的8条边线`
  gDrawPrimitiveUP(s, EPT_LINES, 8, vertices, sizeof(float4));
  gEndScene(s); // `声明所有的绘图指令都下完了`
  return 0;
}
int test_perspective_view(rander_t* s) {
  // 金字塔形的8条边线
  float g_vertices[] = 
  {
    -1.0f, 1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      
      -1.0f,-1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,
      
       1.0f,-1.0f,-1.0f,
       1.0f, 1.0f,-1.0f,
      
       1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
       1.0f,-1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
       1.0f, 1.0f,-1.0f,
  };
  // 镜头位置
  float4 g_eye = {0.0f,3.0f,3.0f};
  // 镜头对准的点
  float4 g_lookat = {0.0f, 0.0f, 0.0f};
  // 镜头正上方的方向
  float4 g_up = {0.0f, -1.0f, 1.0f};
  M4 view_matrix, perspective_matrix, view_perspective_matrix;
  float4 vertices[16];
  int i;
  
  gSetRenderState( s, EMF_LIGHTING, FALSE );
  gClear(s, 0, 0, EDCLEAR_TARGET | EDCLEAR_ZBUFFER, _ARGB(0, 0, 0, 0));
  
  // `计算出一个可以转换到镜头坐标系的矩阵`
  GutMatrixLookAtRH(view_matrix, g_eye, g_lookat, g_up);
  // `计算出一个使用透视投影的矩阵`
  gGutMatrixPerspectiveRH(s, perspective_matrix, 90.0f, 1.0f, 1.0f, 100.0f);
  // `把这两个矩阵相乘`
  M4Mul(view_perspective_matrix, view_matrix, perspective_matrix);
  // `把空间中的坐标点转换到屏幕坐标系上`
  for ( i=0; i<16; i++ )
  {
    M4Apply3(vertices[i], g_vertices +i*3, view_perspective_matrix);
    V4Scale(vertices[i], vertices[i], 1.f/vertices[i][3]);
  }
  
  gBeginScene(s); // `开始下绘图指令`
  // `画出金字塔的8条边线`
  gDrawPrimitiveUP(s, EPT_LINES, 8, vertices, sizeof(float4)); 
  gEndScene(s); // `声明所有的绘图指令都下完了`
  return 0;
}

int test_instance(rander_t* s) {
  static int inited=0;
  
  // 金字塔形的8条边线
  float g_vertices[] = 
  {
    -1.0f, 1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      
      -1.0f,-1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,
      
       1.0f,-1.0f,-1.0f,
       1.0f, 1.0f,-1.0f,
      
       1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
       1.0f,-1.0f,-1.0f,
      
       0.0f, 0.0f, 1.0f,
       1.0f, 1.0f,-1.0f,
  };
  
  // 镜头位置
  float4 g_eye = {0.0f, 4.0f, 4.0f}; 
  // 镜头对准的点
  float4 g_lookat = {0.0f, 0.0f, 0.0f}; 
  // 镜头正上方的方向
  float4 g_up = {0.0f, -1.0f, 1.0f}; 
 
  if (!inited) {
    // `计算出一个可以转换到镜头坐标系的矩阵`
    M4 view_matrix, projection_matrix;
    GutMatrixLookAtRH(view_matrix, g_eye, g_lookat, g_up);
    gGutMatrixPerspectiveRH(s, projection_matrix, 60.0f, 1.0f, 1.0f, 100.0f);
    // `设置视角转换矩阵`
    gSetTransform(s, D3DTS_PROJECTION, projection_matrix);
    // `设置镜头转换矩阵`
    device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
    // `关闭光照`
    device->SetRenderState(D3DRS_LIGHTING, FALSE);
    inited = 1;
  }
}

int test_gui()
{
  img_t im[10] = {0};
  img_t* tex = im + 1;
  const char* winname = "1234";
  window_t* win = NULL;
  E_DRIVER_TYPE type;
  type = EDT_NULL;
  type = EDT_DIRECT3D9;
  type = EDT_OPENGL;

  imread("E:/pub/pic/aa_10.jpg", 3, 1, im);
  //sysNameWindow("asdf", 300, 300);
  init_texture(tex);
  win = sysNameWindow(winname, 800, 600, on_event1, NULL);
  gOpenRander(win->rander, type, win->hwnd, 0);

  for (; sysWaitEvent(30);) {
    rander_t* s = win->rander;
    //IRECT src = iRECT2(100, 100, 300, 400);
    //IRECT drc = iRECT2(100, 100, 400, 400);
    //icvEllipseEx(im, &src, 200, 200, 100, 200, 60, 0, 360, _RGB(255, 0, 0), 0);
    //sysShowImage(winname, tex, ECF_R8G8B8, 0, 0);
    //test_orthogonal_view(s);
    test_perspective_view(s);
  }

  imfrees(im, 10);
  return 0;
}
