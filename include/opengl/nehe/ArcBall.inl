#ifndef _ARCBALL_INL_
#define _ARCBALL_INL_

/** KempoApi: The Turloc Toolkit *****************************/
/** * * **/
/** ** ** Filename: ArcBall.cpp **/
/** ** Version: Common **/
/** ** **/
/** **/
/** Arcball class for mouse manipulation. **/
/** **/
/** **/
/** **/
/** **/
/** (C) 1999-2003 Tatewake.com **/
/** History: **/
/** 08/17/2003 - (TJG) - Creation **/
/** 09/23/2003 - (TJG) - Bug fix and optimization **/
/** 09/25/2003 - (TJG) - Version for NeHe Basecode users **/
/** **/
/*************************************************************/
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include <gl\glaux.h> // Header File For The GLaux Library
#include <math.h> // Needed for sqrtf
#include "ArcBall.h" // ArcBall header
//Arcball sphere constants:
//Diameter is 2.0f
//Radius is 1.0f
//Radius squared is 1.0f
void ArcBall_mapToSphere(const ArcBallT* s, const Point2fT* NewPt, Vector3fT* NewVec)
{
  Point2fT TempPt;
  GLfloat length;
  //Copy paramter into temp point
  TempPt = *NewPt;
  //Adjust point coords and scale down to range of [-1 ... 1]
  TempPt.s.X = (TempPt.s.X * s->AdjustWidth) - 1.0f;
  TempPt.s.Y = 1.0f - (TempPt.s.Y * s->AdjustHeight);
  //Compute the square of the length of the vector to the point from the center
  length = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);
  //If the point is mapped outside of the sphere... (length > radius squared)
  if (length > 1.0f) {
    GLfloat norm;
    //Compute a normalizing factor (radius / sqrt(length))
    norm = 1.0f / FuncSqrt(length);
    //Return the "normalized" vector, a point on the sphere
    NewVec->s.X = TempPt.s.X * norm;
    NewVec->s.Y = TempPt.s.Y * norm;
    NewVec->s.Z = 0.0f;
  }
  else { //Else it's on the inside
    //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
    NewVec->s.X = TempPt.s.X;
    NewVec->s.Y = TempPt.s.Y;
    NewVec->s.Z = FuncSqrt(1.0f - length);
  }
}
//Create/Destroy
int ArcBall_init(ArcBallT* s, GLfloat NewWidth, GLfloat NewHeight)
{
  //Clear initial values
  s->StVec.s.X = s->StVec.s.Y = s->StVec.s.Z =
      s->EnVec.s.X = s->EnVec.s.Y = s->EnVec.s.Z = 0.0f;
  //Set initial bounds
  ArcBall_setBounds(s, NewWidth, NewHeight);
	return 0;
}
//Mouse down
void ArcBall_click(ArcBallT* s, const Point2fT* NewPt)
{
  //Map the point to the sphere
  ArcBall_mapToSphere(s, NewPt, &s->StVec);
}
//Mouse drag, calculate rotation
void ArcBall_drag(ArcBallT* s, const Point2fT* NewPt, Quat4fT* NewRot)
{
  //Map the point to the sphere
  ArcBall_mapToSphere(s, NewPt, &s->EnVec);
  //Return the quaternion equivalent to the rotation
  if (NewRot) {
    Vector3fT Perp;
    //Compute the vector perpendicular to the begin and end vectors
    Vector3fCross(&Perp, &s->StVec, &s->EnVec);
    //Compute the length of the perpendicular vector
    if (Vector3fLength(&Perp) > Epsilon) { //if its non-zero
      //We're ok, so return the perpendicular vector as the transform after all
      NewRot->s.X = Perp.s.X;
      NewRot->s.Y = Perp.s.Y;
      NewRot->s.Z = Perp.s.Z;
      //In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
      NewRot->s.W = Vector3fDot(&s->StVec, &s->EnVec);
    }
    else { //if its zero
      //The begin and end vectors coincide, so return an identity transform
      NewRot->s.X = NewRot->s.Y = NewRot->s.Z = NewRot->s.W = 0.0f;
    }
  }
}


#endif // _ARCBALL_INL_
