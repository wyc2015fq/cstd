//----------------------------------------------------------------------------
// Construct camera instance with its focal point at the origin,
// and position=(0,0,1). The view up is along the y-axis,
// view angle is 30 degrees, and the clipping range is (.1,1000).
void CameraInit(Camera* cam)
{
  cam->FocalPoint[0] = 0.0;
  cam->FocalPoint[1] = 0.0;
  cam->FocalPoint[2] = 0.0;
  cam->Position[0] = 0.0;
  cam->Position[1] = 0.0;
  cam->Position[2] = 1.0;
  cam->ViewUp[0] = 0.0;
  cam->ViewUp[1] = 1.0;
  cam->ViewUp[2] = 0.0;
  cam->DirectionOfProjection[0] = 0.0;
  cam->DirectionOfProjection[1] = 0.0;
  cam->DirectionOfProjection[2] = 0.0;
  cam->ViewAngle = 30.0;
  cam->UseHorizontalViewAngle = 0;
#if 0
  cam->UseOffAxisProjection = 0;
  cam->ScreenBottomLeft[0] = -0.5;
  cam->ScreenBottomLeft[1] = -0.5;
  cam->ScreenBottomLeft[2] = -0.5;
  cam->ScreenBottomRight[0] = 0.5;
  cam->ScreenBottomRight[1] = -0.5;
  cam->ScreenBottomRight[2] = -0.5;
  cam->ScreenTopRight[0] = 0.5;
  cam->ScreenTopRight[1] = 0.5;
  cam->ScreenTopRight[2] = -0.5;
  cam->EyeSeparation = 0.06;
  cam->ClippingRange[0] = 0.01;
  cam->ClippingRange[1] = 1000.01;
  cam->Thickness = 1000.0;
  cam->ParallelProjection = 0;
  cam->ParallelScale = 1.0;
  cam->EyeAngle = 2.0;
  cam->Stereo = 0;
  cam->LeftEye = 1;
  cam->WindowCenter[0] = 0.0;
  y = 0.0;
  cam->ViewShear[0] = 0.0;
  cam->ViewShear[1] = 0.0;
  cam->ViewShear[2] = 1.0;
  cam->FocalDisk = 1.0;
  cam->UserTransform = NULL;
  cam->UserViewTransform = NULL;
  cam->UserViewTransformCallbackCommand = NULL;
  // initialize the ViewTransform
  cam->ComputeViewTransform();
  cam->ComputeDistance();
  cam->ComputeCameraLightTransform();
  cam->FreezeFocalPoint = false;
  cam->UseScissor = false;
#endif
}
#include "Transform.inl"
//----------------------------------------------------------------------------
// The ViewTransform depends on only three ivars: the Position, the
// FocalPoint, and the ViewUp vector. All the other methods are there
// simply for the sake of the users' convenience.
void ComputeViewTransform(Camera* cam, float16 ViewTransform)
{
  // main view through the camera
  SetupCamera(ViewTransform, cam->Position, cam->FocalPoint, cam->ViewUp);
}
//----------------------------------------------------------------------------
void ComputeCameraLightTransform(Camera* cam, double d)
{
  float16 t;
  Identity16(t);
  // assumes a valid view transform and valid camera distance
  ComputeViewTransform(cam, t);
  Invert16(t, t);
  Scale(t, d, d, d);
  Translate(t, 0.0, 0.0, -1.0);
}
//----------------------------------------------------------------------------
void ComputeWorldToScreenMatrix(float16 WorldToScreenMatrix, const float3 ScreenBottomLeft, const float3 ScreenBottomRight, const float3 ScreenTopRight)
{
  // Avoid recalculating screen orientation if we don't need to.
  float xAxis[3];
  float yAxis[3];
  float zAxis[3];
  Identity16(WorldToScreenMatrix);
  for (int i = 0; i < 3; ++i) {
    xAxis[i] = ScreenBottomRight[i] - ScreenBottomLeft[i];
    yAxis[i] = ScreenTopRight[i] - ScreenBottomRight[i];
  }
  Normalize3(xAxis);
  Normalize3(yAxis);
  Cross(xAxis, yAxis, zAxis);
  Normalize3(zAxis);
  // Setting individual elements of the matrix.
  // Make it column major and then invert it to make sure the translation is correct
  // This is using column major (vectors are copied into the column)
  // \Note: while the initial element assignments are made in column-major
  // ordering, the matrix will be inverted, resulting in a row-major
  // matrix that provides the transformation from World to Screen space.
  WorldToScreenMatrix[0 * 4 + 0] = xAxis[0];
  WorldToScreenMatrix[1 * 4 + 0] = xAxis[1];
  WorldToScreenMatrix[2 * 4 + 0] = xAxis[2];
  WorldToScreenMatrix[0 * 4 + 1] = yAxis[0];
  WorldToScreenMatrix[1 * 4 + 1] = yAxis[1];
  WorldToScreenMatrix[2 * 4 + 1] = yAxis[2];
  WorldToScreenMatrix[0 * 4 + 2] = zAxis[0];
  WorldToScreenMatrix[1 * 4 + 2] = zAxis[1];
  WorldToScreenMatrix[2 * 4 + 2] = zAxis[2];
  WorldToScreenMatrix[0 * 4 + 3] = ScreenBottomLeft[0];
  WorldToScreenMatrix[1 * 4 + 3] = ScreenBottomLeft[1];
  WorldToScreenMatrix[2 * 4 + 3] = ScreenBottomLeft[2];
  WorldToScreenMatrix[3 * 4 + 3] = 1.0;
  // The reason for doing cam as an Invert as the goal here is to put
  // the translation through the rotation that we've just assigned ie.
  // the translation has to be put into screen space too.
  Invert16(WorldToScreenMatrix, WorldToScreenMatrix);
}
void StereoProjectionTransform(float16 ProjectionTransform, bool LeftEye, double EyeAngle, double Distance)
{
  // set up a shear for stereo views
  if (LeftEye) {
    Stereo(ProjectionTransform, -EyeAngle / 2, Distance);
  }
  else {
    Stereo(ProjectionTransform, +EyeAngle / 2, Distance);
  }
}
void ParallelTransform(float16 Transform, Camera* cam, double aspect, double nearz, double farz, float3 ViewShear, double ParallelScale, float2 ClippingRange, float2 WindowCenter)
{
  //Identity16(Transform();
  // adjust Z-buffer range
  AdjustZBuffer(Transform, -1, +1, nearz, farz);
  // set up a rectangular parallelipiped
  double width = ParallelScale * aspect;
  double height = ParallelScale;
  double xmin = (WindowCenter[0] - 1.0) * width;
  double xmax = (WindowCenter[0] + 1.0) * width;
  double ymin = (WindowCenter[1] - 1.0) * height;
  double ymax = (WindowCenter[1] + 1.0) * height;
  Ortho(Transform, xmin, xmax, ymin, ymax, ClippingRange[0], ClippingRange[1]);
}
//----------------------------------------------------------------------------
void ComputeOffAxisProjectionFrustum(float16 ProjectionTransform, const float3 ScreenBottomLeft, const float3 ScreenBottomRight, const float3 ScreenTopRight)
{
  float16 WorldToScreenMatrix;
#if 0
  ComputeWorldToScreenMatrix(WorldToScreenMatrix);
  // \NOTE: Varibles names reflect naming convention used in
  // "High Resolution Virtual Reality", in Proc.
  // SIGGRAPH '92, Computer Graphics, pages 195-202, 1992.
  // OffAxis calculations.
  // vtkMatrix::MultiplyPoint expect homogeneous coordinate.
  float E[4] = {0.0, 0.0, 0.0, 1.0};
  float L[4] = {ScreenBottomLeft[0], ScreenBottomLeft[1], ScreenBottomLeft[2], 1.0};
  float H[4] = {ScreenTopRight[0], ScreenTopRight[1], ScreenTopRight[2], 1.0};
  double eyeSeparationCorrectionFactor = 10.0;
  double shiftDistance = EyeSeparation / (2.0 * eyeSeparationCorrectionFactor);
  if (Distance < 1.0) {
    shiftDistance *= Distance;
  }
  if (LeftEye) {
    E[0] -= shiftDistance;
  }
  else {
    E[0] += shiftDistance;
  }
  // First transform the eye to new position.
  MultiplyPoint16(EyeTransformMatrix, E, E);
  // Now transform the eye and screen corner points into the screen
  // coordinate system.
  MultiplyPoint16(WorldToScreenMatrix, E, E);
  MultiplyPoint16(WorldToScreenMatrix, H, H);
  MultiplyPoint16(WorldToScreenMatrix, L, L);
  float matrix[16];
  double width = H[0] - L[0];
  double height = H[1] - L[1];
  // Back and front are not traditional near and far.
  // Front (aka near)
  double F = E[2] - (cam->Distance + cam->Thickness);//E[2] - 10000.0;//cam->ClippingRange[1];
  // Back (aka far)
  double nearDistanceCorrectionFactor = 1000.0;
  double B = E[2] - (cam->Distance / nearDistanceCorrectionFactor);//E[2] - .1;//cam->ClippingRange[0];
  double depth = B - F;
  matrix[0][0] = (2 * E[2]) / width;
  matrix[1][0] = 0;
  matrix[2][0] = 0;
  matrix[3][0] = 0;
  matrix[0][1] = 0;
  matrix[1][1] = (2 * E[2]) / height;
  matrix[2][1] = 0;
  matrix[3][1] = 0;
  matrix[0][2] = (H[0] + L[0] - 2 * E[0]) / width;
  matrix[1][2] = (H[1] + L[1] - 2 * E[1]) / height;
  matrix[2][2] = (B + F - 2 * E[2]) / depth;
  matrix[3][2] = -1;
  matrix[0][3] = (-E[2] * (H[0] + L[0])) / width;
  matrix[1][3] = (-E[2] * (H[1] + L[1])) / height;
  matrix[2][3] = B - E[2] - (B * (B + F - 2 * E[2]) / depth);
  matrix[3][3] = E[2];
  Copy16(ProjectionTransform, matrix);
  // Now move the world into display space.
  MultiplyMatrix16(cam->ProjectionTransform->GetMatrix(), WorldToScreenMatrix,
      cam->ProjectionTransform->GetMatrix());
#endif
}
//----------------------------------------------------------------------------
// Compute the projection transform matrix. This is used in converting
// between view and world coordinates.
void ProjectionTransform(float16 Transform, double aspect, double nearz, double farz, float3 ViewShear,
    double ViewAngle, bool UseHorizontalViewAngle, float2 WindowCenter, float2 ClippingRange)
{
  // set up a perspective frustum
  double tmp = tan(RadiansFromDegrees(ViewAngle) / 2.);
  double width;
  double height;
  if (UseHorizontalViewAngle) {
    width = ClippingRange[0] * tmp;
    height = ClippingRange[0] * tmp / aspect;
  }
  else {
    width = ClippingRange[0] * tmp * aspect;
    height = ClippingRange[0] * tmp;
  }
  double xmin = (WindowCenter[0] - 1.0) * width;
  double xmax = (WindowCenter[0] + 1.0) * width;
  double ymin = (WindowCenter[1] - 1.0) * height;
  double ymax = (WindowCenter[1] + 1.0) * height;
  Frustum(Transform, xmin, xmax, ymin, ymax, ClippingRange[0], ClippingRange[1]);
}
#if 0
//----------------------------------------------------------------------------
// Return the projection transform matrix. See ComputeProjectionTransform.
vtkMatrix4x4* GetProjectionTransformMatrix(double aspect, double nearz, double farz)
{
  cam->ComputeProjectionTransform(aspect, nearz, farz);
  // return the transform
  return cam->ProjectionTransform->GetMatrix();
}
//----------------------------------------------------------------------------
// Return the projection transform matrix. See ComputeProjectionTransform.
float* GetCompositeProjectionTransformMatrix(Camera* cam, double aspect, double nearz, double farz, float16 Transform)
{
  // turn off stereo, the CompositeProjectionTransformMatrix is used for
  // picking, not for rendering.
  Identity16(Transform);
  Concatenate16(Transform, cam->GetProjectionTransformMatrix(aspect, nearz, farz));
  cam->Transform->Concatenate16(cam->GetViewTransformMatrix());
}
//----------------------------------------------------------------------------
void ComputeModelViewMatrix()
{
  if (cam->ModelViewTransform->GetMTime() < cam->ModelTransformMatrix->GetMTime() ||
      cam->ModelViewTransform->GetMTime() < cam->ViewTransform->GetMTime()) {
    Multiply4x4(cam->ViewTransform->GetMatrix(), cam->ModelTransformMatrix,
        cam->ModelViewTransform->GetMatrix());
  }
}
//----------------------------------------------------------------------------
void OrthogonalizeViewUp()
{
  // the orthogonalized ViewUp is just the second row of the view matrix
  vtkMatrix4x4* matrix = cam->ViewTransform->GetMatrix();
  cam->ViewUp[0] = matrix->GetElement(1, 0);
  cam->ViewUp[1] = matrix->GetElement(1, 1);
  cam->ViewUp[2] = matrix->GetElement(1, 2);
  cam->Modified();
}
//----------------------------------------------------------------------------
// Set the distance of the focal point from the camera. The focal point is
// modified accordingly. This should be positive.
void SetDistance(double d)
{
  if (cam->Distance == d) {
    return;
  }
  cam->Distance = d;
  // Distance should be greater than .0002
  if (cam->Distance < 0.0002) {
    cam->Distance = 0.0002;
    vtkDebugMacro( << " Distance is set to minimum.");
  }
  // we want to keep the camera pointing in the same direction
  double* vec = cam->DirectionOfProjection;
  // recalculate FocalPoint
  cam->FocalPoint[0] = cam->Position[0] + vec[0] * cam->Distance;
  cam->FocalPoint[1] = cam->Position[1] + vec[1] * cam->Distance;
  cam->FocalPoint[2] = cam->Position[2] + vec[2] * cam->Distance;
  vtkDebugMacro( << " Distance set to ( " << cam->Distance << ")");
  cam->ComputeViewTransform();
  cam->ComputeCameraLightTransform();
  cam->Modified();
}
//----------------------------------------------------------------------------
// This method must be called when the focal point or camera position changes
void ComputeDistance()
{
  double dx = cam->FocalPoint[0] - cam->Position[0];
  double dy = cam->FocalPoint[1] - cam->Position[1];
  double dz = cam->FocalPoint[2] - cam->Position[2];
  cam->Distance = sqrt(dx * dx + dy * dy + dz * dz);
  if (cam->Distance < 1e-20) {
    cam->Distance = 1e-20;
    vtkDebugMacro( << " Distance is set to minimum.");
    double* vec = cam->DirectionOfProjection;
    // recalculate FocalPoint
    cam->FocalPoint[0] = cam->Position[0] + vec[0] * cam->Distance;
    cam->FocalPoint[1] = cam->Position[1] + vec[1] * cam->Distance;
    cam->FocalPoint[2] = cam->Position[2] + vec[2] * cam->Distance;
  }
  cam->DirectionOfProjection[0] = dx / cam->Distance;
  cam->DirectionOfProjection[1] = dy / cam->Distance;
  cam->DirectionOfProjection[2] = dz / cam->Distance;
  cam->ComputeViewPlaneNormal();
}
//----------------------------------------------------------------------------
// Move the position of the camera along the view plane normal. Moving
// towards the focal point (e.g., > 1) is a dolly-in, moving away
// from the focal point (e.g., < 1) is a dolly-out.
void Dolly(double amount)
{
  if (amount <= 0.0) {
    return;
  }
  // dolly moves the camera towards the focus
  double d = cam->Distance / amount;
  cam->SetPosition(cam->FocalPoint[0] - d * cam->DirectionOfProjection[0],
      cam->FocalPoint[1] - d * cam->DirectionOfProjection[1],
      cam->FocalPoint[2] - d * cam->DirectionOfProjection[2]);
}
//----------------------------------------------------------------------------
// Set the roll angle of the camera about the direction of projection
void SetRoll(double roll)
{
  // roll is a rotation of camera view up about the direction of projection
  vtkDebugMacro( << " Setting Roll to " << roll << "");
  // subtract the current roll
  roll -= cam->GetRoll();
  if (fabs(roll) < 0.00001) {
    return;
  }
  cam->Roll(roll);
}
//----------------------------------------------------------------------------
// Returns the roll of the camera.
double GetRoll()
{
  double orientation[3];
  cam->ViewTransform->GetOrientation(orientation);
  return orientation[2];
}
//----------------------------------------------------------------------------
// Rotate the camera around the view plane normal.
void Roll(double angle)
{
  double newViewUp[3];
  cam->Transform->Identity();
  // rotate ViewUp about the Direction of Projection
  cam->Transform->RotateWXYZ(angle, cam->DirectionOfProjection);
  // okay, okay, TransformPoint shouldn't be used on vectors -- but
  // the transform is rotation with no translation so cam works fine.
  cam->Transform->TransformPoint(cam->ViewUp, newViewUp);
  cam->SetViewUp(newViewUp);
}
//----------------------------------------------------------------------------
// Rotate the focal point about the view up vector centered at the camera's
// position.
void Yaw(double angle)
{
  double newFocalPoint[3];
  double* pos = cam->Position;
  cam->Transform->Identity();
  // translate the camera to the origin,
  // rotate about axis,
  // translate back again
  cam->Transform->Translate(+pos[0], +pos[1], +pos[2]);
  cam->Transform->RotateWXYZ(angle, cam->ViewUp);
  cam->Transform->Translate(-pos[0], -pos[1], -pos[2]);
  // now transform focal point
  cam->Transform->TransformPoint(cam->FocalPoint, newFocalPoint);
  cam->SetFocalPoint(newFocalPoint);
}
//----------------------------------------------------------------------------
// Rotate the focal point about the cross product of the view up vector
// and the negative of the , centered at the camera's position.
void Pitch(double angle)
{
  double axis[3], newFocalPoint[3];
  double* pos = cam->Position;
  cam->Transform->Identity();
  // the axis is the first row of the view transform matrix
  axis[0] = cam->ViewTransform->GetMatrix()->GetElement(0, 0);
  axis[1] = cam->ViewTransform->GetMatrix()->GetElement(0, 1);
  axis[2] = cam->ViewTransform->GetMatrix()->GetElement(0, 2);
  // translate the camera to the origin,
  // rotate about axis,
  // translate back again
  cam->Transform->Translate(+pos[0], +pos[1], +pos[2]);
  cam->Transform->RotateWXYZ(angle, axis);
  cam->Transform->Translate(-pos[0], -pos[1], -pos[2]);
  // now transform focal point
  cam->Transform->TransformPoint(cam->FocalPoint, newFocalPoint);
  cam->SetFocalPoint(newFocalPoint);
}
//----------------------------------------------------------------------------
// Rotate the camera about the view up vector centered at the focal point.
void Azimuth(double angle)
{
  double newPosition[3];
  double* fp = cam->FocalPoint;
  cam->Transform->Identity();
  // translate the focal point to the origin,
  // rotate about view up,
  // translate back again
  cam->Transform->Translate(+fp[0], +fp[1], +fp[2]);
  cam->Transform->RotateWXYZ(angle, cam->ViewUp);
  cam->Transform->Translate(-fp[0], -fp[1], -fp[2]);
  // apply the transform to the position
  cam->Transform->TransformPoint(cam->Position, newPosition);
  cam->SetPosition(newPosition);
}
//----------------------------------------------------------------------------
// Rotate the camera about the cross product of the negative of the
// direction of projection and the view up vector centered on the focal point.
void Elevation(double angle)
{
  double axis[3], newPosition[3];
  double* fp = cam->FocalPoint;
  cam->Transform->Identity();
  // snatch the axis from the view transform matrix
  axis[0] = -cam->ViewTransform->GetMatrix()->GetElement(0, 0);
  axis[1] = -cam->ViewTransform->GetMatrix()->GetElement(0, 1);
  axis[2] = -cam->ViewTransform->GetMatrix()->GetElement(0, 2);
  // translate the focal point to the origin,
  // rotate about axis,
  // translate back again
  cam->Transform->Translate(+fp[0], +fp[1], +fp[2]);
  cam->Transform->RotateWXYZ(angle, axis);
  cam->Transform->Translate(-fp[0], -fp[1], -fp[2]);
  // now transform position
  cam->Transform->TransformPoint(cam->Position, newPosition);
  cam->SetPosition(newPosition);
}
//----------------------------------------------------------------------------
// Apply Transform to camera
void ApplyTransform(vtkTransform* t)
{
  double posOld[4], posNew[4], fpOld[4], fpNew[4], vuOld[4], vuNew[4];
  cam->GetPosition(posOld);
  cam->GetFocalPoint(fpOld);
  cam->GetViewUp(vuOld);
  posOld[3] = 1.0;
  fpOld[3] = 1.0;
  vuOld[3] = 1.0;
  vuOld[0] += posOld[0];
  vuOld[1] += posOld[1];
  vuOld[2] += posOld[2];
  t->MultiplyPoint(posOld, posNew);
  t->MultiplyPoint(fpOld, fpNew);
  t->MultiplyPoint(vuOld, vuNew);
  vuNew[0] -= posNew[0];
  vuNew[1] -= posNew[1];
  vuNew[2] -= posNew[2];
  cam->SetPosition(posNew);
  cam->SetFocalPoint(fpNew);
  cam->SetViewUp(vuNew);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// The following methods set up the information that the Renderer needs
// to set up the perspective transform. The transformation matrix is
// created using the GetPerspectiveTransformMatrix method.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SetParallelProjection(int flag)
{
  if (cam->ParallelProjection != flag) {
    cam->ParallelProjection = flag;
    cam->Modified();
    cam->ViewingRaysModified();
  }
}
//----------------------------------------------------------------------------
void SetViewAngle(double angle)
{
  double min = 0.00000001;
  double max = 179.0;
  if (cam->ViewAngle != angle) {
    cam->ViewAngle = (angle < min ? min : (angle > max ? max : angle));
    cam->Modified();
    cam->ViewingRaysModified();
  }
}
//----------------------------------------------------------------------------
void SetObliqueAngles(double alpha, double beta)
{
  alpha = RadiansFromDegrees(alpha);
  beta = RadiansFromDegrees(beta);
  double cotbeta = cos(beta) / sin(beta);
  double dxdz = cos(alpha) * cotbeta;
  double dydz = sin(alpha) * cotbeta;
  cam->SetViewShear(dxdz, dydz, 1.0);
}
//----------------------------------------------------------------------------
// Set the shear transform of the viewing frustum. Parameters are
// dx/dz, dy/dz, and center. center is a factor that describes where
// to shear around. The distance dshear from the camera where
// no shear occurs is given by (dshear = center * FocalDistance).
//
void SetViewShear(double dxdz, double dydz, double center)
{
  if (dxdz != cam->ViewShear[0] ||
      dydz != cam->ViewShear[1] ||
      center != cam->ViewShear[2]) {
    cam->Modified();
    cam->ViewingRaysModified();
    cam->ViewShear[0] = dxdz;
    cam->ViewShear[1] = dydz;
    cam->ViewShear[2] = center;
    cam->ComputeViewPlaneNormal();
  }
}
//----------------------------------------------------------------------------
void SetViewShear(double d[3])
{
  cam->SetViewShear(d[0], d[1], d[2]);
}
//----------------------------------------------------------------------------
// Return the projection transform matrix. See ComputeProjectionTransform.
vtkMatrix4x4* GetProjectionTransformMatrix(vtkRenderer* ren)
{
  double aspect[2];
  int lowerLeft[2];
  int usize, vsize;
  vtkMatrix4x4* matrix = New();
  ren->GetTiledSizeAndOrigin(&usize, &vsize, lowerLeft, lowerLeft + 1);
  // some renderer subclasses may have more complicated computations for the
  // aspect ratio. So take that into account by computing the difference
  // between our simple aspect ratio and what the actual renderer is reporting.
  ren->ComputeAspect();
  ren->GetAspect(aspect);
  double aspect2[2];
  ren->vtkViewport::ComputeAspect();
  ren->vtkViewport::GetAspect(aspect2);
  double aspectModification = aspect[0] * aspect2[1] / (aspect[1] * aspect2[0]);
  if (usize && vsize) {
    matrix->DeepCopy(cam->GetProjectionTransformMatrix(
        aspectModification * usize / vsize, -1, 1));
    matrix->Transpose();
  }
  return matrix;
}
//----------------------------------------------------------------------------
// Return the projection transform object. See ComputeProjectionTransform.
vtkPerspectiveTransform* GetProjectionTransformObject(double aspect,
    double nearz,
    double farz)
{
  cam->ComputeProjectionTransform(aspect, nearz, farz);
  // return the transform
  return cam->ProjectionTransform;
}
//----------------------------------------------------------------------------
// Return the attached light transform matrix.
vtkMatrix4x4* GetCameraLightTransformMatrix()
{
  // return the transform
  return cam->CameraLightTransform->GetMatrix();
}
//----------------------------------------------------------------------------
void ComputeViewPlaneNormal()
{
  if (cam->ViewShear[0] != 0.0 || cam->ViewShear[1] != 0.0) {
    // set the VPN in camera coordinates
    cam->ViewPlaneNormal[0] = cam->ViewShear[0];
    cam->ViewPlaneNormal[1] = cam->ViewShear[1];
    cam->ViewPlaneNormal[2] = 1.0;
    // transform the VPN to world coordinates using inverse of view transform
    cam->ViewTransform->GetLinearInverse()->TransformNormal(
        cam->ViewPlaneNormal,
        cam->ViewPlaneNormal);
  }
  else {
    // VPN is -DOP
    cam->ViewPlaneNormal[0] = -cam->DirectionOfProjection[0];
    cam->ViewPlaneNormal[1] = -cam->DirectionOfProjection[1];
    cam->ViewPlaneNormal[2] = -cam->DirectionOfProjection[2];
  }
}
//----------------------------------------------------------------------------
// Return the 6 planes (Ax + By + Cz + D = 0) that bound
// the view frustum.
void GetFrustumPlanes(double aspect, float planes[24])
{
  int i;
  double f;
  float normals[6][4], matrix[16];
  // set up the normals
  for (i = 0; i < 6; i++) {
    normals[i][0] = 0.0;
    normals[i][1] = 0.0;
    normals[i][2] = 0.0;
    normals[i][3] = 1.0;
    // if i is even set to -1, if odd set to +1
    normals[i][i / 2] = 1 - (i % 2) * 2;
  }
  // get the composite perspective matrix
  DeepCopy(*matrix, cam->GetCompositeProjectionTransformMatrix(aspect, -1, +1));
  // transpose the matrix for use with normals
  Transpose(*matrix, *matrix);
  // transform the normals to world coordinates
  for (i = 0; i < 6; i++) {
    MultiplyPoint(*matrix, normals[i], normals[i]);
    f = 1.0 / sqrt(normals[i][0] * normals[i][0] +
        normals[i][1] * normals[i][1] +
        normals[i][2] * normals[i][2]);
    planes[4 * i + 0] = normals[i][0] * f;
    planes[4 * i + 1] = normals[i][1] * f;
    planes[4 * i + 2] = normals[i][2] * f;
    planes[4 * i + 3] = normals[i][3] * f;
  }
}
//-----------------------------------------------------------------------------
void GetEyePlaneNormal(double normal[3])
{
  if (!normal) {
    vtkErrorMacro( << "ERROR: Invalid or NULL normal\n");
    return;
  }
  // Homogeneous normal.
  double localNormal[4];
  // Get the normal from the screen orientation.
  localNormal[0] = WorldToScreenMatrix->GetElement(2, 0);
  localNormal[1] = WorldToScreenMatrix->GetElement(2, 1);
  localNormal[2] = WorldToScreenMatrix->GetElement(2, 2);
  localNormal[3] = 0.0;
  // Just to be sure.
  Normalize(localNormal);
  normal[0] = localNormal[0];
  normal[1] = localNormal[1];
  normal[2] = localNormal[2];
}
#endif

