#include "VolumeRayCast_OpenGL2.inl"
#include "ColorTransferFunction.inl"
#include "OpenGLGPUVolumeRayCast.h"
#include "VolumeShaderComposer.inl"
#include "PerspectiveTransform.inl"
#include "Quaternion.inl"
#include "Camera.inl"
void GetModelViewTransformMatrix(Camera* cam, Matrix4x4 mv)
{
  ASSERT(0);
  //GetVPTrans(mv, cam->Pos[0], cam->Pos[1], cam->Pos[2], cam->Target[0], cam->Target[1], cam->Target[2], cam->Up[0], cam->Up[1], cam->Up[2]);
}
void GetTiledSizeAndOrigin(Camera* cam, int* usize, int* vsize, int* lowerLeftU, int* lowerLeftV)
{
  ASSERT(0);
}
void ComputeAspect(Camera* cam, float* aspect)
{
  ASSERT(0);
}
void GetKeyMatrices(Camera* cam, Matrix4x4 wcvc, Matrix3x3 normMat, Matrix4x4 vcdc, Matrix4x4 wcdc)
{
#if 0
  // has the camera changed?
  int i, j;
  GetModelViewTransformMatrix(cam, wcvc);
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
      normMat[i][j] = wcvc[i][j];
    }
  }
  Invert33(normMat);
  Transpose44(wcvc);
  float aspect[2];
  int lowerLeft[2];
  int usize, vsize;
  GetTiledSizeAndOrigin(cam, &usize, &vsize, lowerLeft, lowerLeft + 1);
#if 0
  ComputeAspect(cam, aspect);
  double aspect2[2];
  s->vtkViewport::ComputeAspect();
  s->vtkViewport::GetAspect(aspect2);
  double aspectModification = aspect[0] * aspect2[1] / (aspect[1] * aspect2[0]);
#else
  double aspectModification = 1;
#endif
  if (usize && vsize) {
    GetProjectionTransformMatrix(aspectModification * usize / vsize, -1, 1);
    Copy44(vcdc,);
    vcdc->Transpose();
  }
  MultiplyMatrix44(wcvc, vcdc, wcdc);
#endif
}

#include "OpenGLGPUVolumeRayCast.inl"

#if 0
#endif

