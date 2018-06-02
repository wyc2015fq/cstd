void Concatenate16(float16 A, const float16 B)
{
  MultiplyMatrix16(A, B, A);
}
//----------------------------------------------------------------------------
// Utility for adjusting the window range to a new one. Usually the
// previous range was ([-1,+1],[-1,+1]) as per Ortho and Frustum, and you
// are mapping to the display coordinate range ([0,width-1],[0,height-1]).
void AdjustViewport(float16 s, double oldXMin, double oldXMax,
    double oldYMin, double oldYMax,
    double newXMin, double newXMax,
    double newYMin, double newYMax)
{
  float matrix[4 * 4];
  Identity9(matrix);
  matrix[0 * 4 + 0] = (newXMax - newXMin) / (oldXMax - oldXMin);
  matrix[1 * 4 + 1] = (newYMax - newYMin) / (oldYMax - oldYMin);
  matrix[0 * 4 + 3] = (newXMin * oldXMax - newXMax * oldXMin) / (oldXMax - oldXMin);
  matrix[1 * 4 + 3] = (newYMin * oldYMax - newYMax * oldYMin) / (oldYMax - oldYMin);
  Concatenate16(s, matrix);
}
//----------------------------------------------------------------------------
// Utility for adjusting the min/max range of the Z buffer. Usually
// the oldZMin, oldZMax are [-1,+1] as per Ortho and Frustum, and
// you are mapping the Z buffer to a new range.
void AdjustZBuffer(float16 s, double oldZMin, double oldZMax, double newZMin, double newZMax)
{
  float matrix[16];
  Identity16(matrix);
  matrix[2 * 4 + 2] = (newZMax - newZMin) / (oldZMax - oldZMin);
  matrix[2 * 4 + 3] = (newZMin * oldZMax - newZMax * oldZMin) / (oldZMax - oldZMin);
  Concatenate16(s, matrix);
}
//----------------------------------------------------------------------------
// The orthographic perspective maps [xmin,xmax], [ymin,ymax], [-znear,-zfar]
// to [-1,+1], [-1,+1], [-1,+1].
// From the OpenGL Programmer's guide, 2nd Ed.
void Ortho(float16 s, double xmin, double xmax, double ymin, double ymax, double znear, double zfar)
{
  float matrix[16];
  Identity16(matrix);
  matrix[0 * 4 + 0] = 2 / (xmax - xmin);
  matrix[1 * 4 + 1] = 2 / (ymax - ymin);
  matrix[2 * 4 + 2] = -2 / (zfar - znear);
  matrix[0 * 4 + 3] = -(xmin + xmax) / (xmax - xmin);
  matrix[1 * 4 + 3] = -(ymin + ymax) / (ymax - ymin);
  matrix[2 * 4 + 3] = -(znear + zfar) / (zfar - znear);
  Concatenate16(s, matrix);
}
//----------------------------------------------------------------------------
// The frustrum perspective maps a frustum with the front plane at -znear
// which has extent [xmin,xmax],[ymin,ymax] and a back plane at -zfar
// to [-1,+1], [-1,+1], [-1,+1].
// From the OpenGL Programmer's guide, 2nd Ed.
void Frustum(float16 s, double xmin, double xmax, double ymin, double ymax, double znear, double zfar)
{
  float matrix[16];
  matrix[0 * 4 + 0] = 2 * znear / (xmax - xmin);
  matrix[1 * 4 + 0] = 0;
  matrix[2 * 4 + 0] = 0;
  matrix[3 * 4 + 0] = 0;
  matrix[0 * 4 + 1] = 0;
  matrix[1 * 4 + 1] = 2 * znear / (ymax - ymin);
  matrix[2 * 4 + 1] = 0;
  matrix[3 * 4 + 1] = 0;
  matrix[0 * 4 + 2] = (xmin + xmax) / (xmax - xmin);
  matrix[1 * 4 + 2] = (ymin + ymax) / (ymax - ymin);
  matrix[2 * 4 + 2] = -(znear + zfar) / (zfar - znear);
  matrix[3 * 4 + 2] = -1;
  matrix[0 * 4 + 3] = 0;
  matrix[1 * 4 + 3] = 0;
  matrix[2 * 4 + 3] = -2 * znear * zfar / (zfar - znear);
  matrix[3 * 4 + 3] = 0;
  Concatenate16(s, matrix);
}
//----------------------------------------------------------------------------
// For convenience, an easy way to set up a symmetrical frustum.
void Perspective(float16 s, double angle, double aspect, double znear, double zfar)
{
  double ymax = tan(RadiansFromDegrees(angle) / 2) * znear;
  double ymin = -ymax;
  double xmax = ymax * aspect;
  double xmin = -xmax;
  Frustum(s, xmin, xmax, ymin, ymax, znear, zfar);
}
void Shear(float16 s, double dxdz, double dydz, double zplane)
{
  float matrix[16];
  Identity16(matrix);
  // everything is negative because the position->focalpoint vector
  // is in the -z direction, hence z distances along that vector
  // are negative.
  // shear according to the eye position relative to the screen
  matrix[0 * 4 + 2] = -dxdz;
  matrix[1 * 4 + 2] = -dydz;
  // shift so that view rays converge in the focal plane
  matrix[0 * 4 + 3] = -zplane * dxdz;
  matrix[1 * 4 + 3] = -zplane * dydz;
  // concatenate with the current matrix
  Concatenate16(s, matrix);
}
//----------------------------------------------------------------------------
// For convenience -- this is sufficient for most people's stereo needs.
// Set the angle to negative for left eye, positive for right eye.
void Stereo(float16 s, double angle, double focaldistance)
{
  double dxdz = tan(RadiansFromDegrees(angle));
  Shear(s, dxdz, 0.0, focaldistance);
}
//----------------------------------------------------------------------------
void SetupCamera(float16 s, const float position[3], const float focalPoint[3], const float viewUp[3])
{
  float matrix[16];
  Identity16(matrix);
  // the view directions correspond to the rows of the rotation matrix,
  // so we'll make the connection explicit
  float* viewSideways = matrix + 0 * 4;
  float* orthoViewUp = matrix + 1 * 4;
  float* viewPlaneNormal = matrix + 2 * 4;
  // set the view plane normal from the view vector
  viewPlaneNormal[0] = position[0] - focalPoint[0];
  viewPlaneNormal[1] = position[1] - focalPoint[1];
  viewPlaneNormal[2] = position[2] - focalPoint[2];
  Normalize3(viewPlaneNormal);
  // orthogonalize viewUp and compute viewSideways
  Cross(viewUp, viewPlaneNormal, viewSideways);
  Normalize3(viewSideways);
  Cross(viewPlaneNormal, viewSideways, orthoViewUp);
  // translate by the vector from the position to the origin
  float delta[4];
  delta[0] = -position[0];
  delta[1] = -position[1];
  delta[2] = -position[2];
  delta[3] = 0.0; // yes, this should be zero, not one
  MultiplyPoint16(matrix, delta, delta);
  matrix[0 * 4 + 3] = delta[0];
  matrix[1 * 4 + 3] = delta[1];
  matrix[2 * 4 + 3] = delta[2];
  // apply the transformation
  Concatenate16(s, matrix);
}
void SetupCamera(float16 s, double p0, double p1, double p2, double fp0, double fp1, double fp2, double vup0, double vup1, double vup2)
{
  float p[3], fp[3], vup[3];
  p[0] = p0;
  p[1] = p1;
  p[2] = p2;
  fp[0] = fp0;
  fp[1] = fp1;
  fp[2] = fp2;
  vup[0] = vup0;
  vup[1] = vup1;
  vup[2] = vup2;
  SetupCamera(s, p, fp, vup);
}

