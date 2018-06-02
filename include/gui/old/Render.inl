
#include "ttt.inl"
#include "d3d9.inl"
#include "opengl.inl"

void gCloseRander(rander_t* s)
{
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->Close(s->x);
  s->x = NULL;
}
void gOpenRander(rander_t* s, E_DRIVER_TYPE dt, void* WindowId, rander_params_t* Params)
{
  static rander_params_t default_params[1] = {0};
  static int inited=0;
  if (!inited) {
    inited = 1;
    default_params->Bits = (16);
    default_params->ZBufferBits = (16);
    default_params->Doublebuffer = (true);
    default_params->Stereobuffer = (false);
    default_params->Stencilbuffer = (false);
    default_params->MultiSamples = (false);
#if 0
    //default_params->DeviceType = (EIDT_BEST),
    default_params->DriverType = (video::EDT_BURNINGSVIDEO);
    default_params->Fullscreen = (false);
    default_params->Vsync = (false);
    default_params->AntiAlias = (0);
    default_params->HandleSRGB = (false);
    default_params->WithAlphaChannel = (false);
    default_params->IgnoreInput = (false);
    default_params->HighPrecisionFPU = (false);
    default_params->EventReceiver = (0);
#endif
  }
  Params = Params ? Params : default_params;
  
  dt = (EDT_NULL < dt && dt < EDT_COUNT) ? dt : EDT_OPENGL;

  if (s->x) {
    rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);

    if (funcs && funcs->dt != dt) {
      gCloseRander(s);
    }
  }
  
  if (NULL==s->x) {
    switch (dt) {
    case EDT_OPENGL:
      s->x = opengl_init(WindowId, Params);
      break;
      
#ifdef __cplusplus
    case EDT_DIRECT3D9:
      s->x = d3d9_init(WindowId, Params);
      break;
#endif // __cplusplus
    }
  }
  ASSERT(s->x != NULL);
}
void gSetRenderState(rander_t* s, int type, int b) {
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->SetRenderState(s->x, type, b);
}
void gBeginScene(rander_t* s) {
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->BeginScene(s->x);
}
void gEndScene(rander_t* s)
{
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->EndScene(s->x);
}
void gClear(rander_t* s, int Count, const IRECT* pRects, DWORD Flags, COLOR Color)
{
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->Clear(s->x, Count, pRects, Flags, Color);
}
void gDrawPrimitiveUP(rander_t* s, E_PRIMITIVE_TYPE t, int count, const void* vertex, int stride)
{
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  funcs->DrawPrimitiveUP(s->x, t, count, vertex, stride);
}

void gGutMatrixOrthoRH(rander_t* s, M4 matrix, float w, float h, float z_near, float z_far) {
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  switch (funcs->dt) {
  case EDT_OPENGL:
    GutMatrixOrthoRH_OpenGL(matrix, w, h, z_near, z_far);
    break;
  default:
    GutMatrixOrthoRH_DirectX(matrix, w, h, z_near, z_far);
    break;
  }
}

void gGutMatrixPerspectiveRH(rander_t* s, M4 matrix, float fovy, float aspect, float z_near, float z_far) {
  rander_funcs_t* funcs = *(rander_funcs_t**)(s->x);
  switch (funcs->dt) {
  case EDT_OPENGL:
    GutMatrixPerspectiveRH_OpenGL(matrix, fovy, aspect, z_near, z_far);
    break;
  default:
    GutMatrixPerspectiveRH_DirectX(matrix, fovy, aspect, z_near, z_far);
    break;
  }
}
