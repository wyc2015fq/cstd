#include "StdAfx.h"
#include "cubelet.h"
#include <gl\gl.h>
#include <math.h>
#include "ColorRef.h"

#define M_PI 3.1415926535897932f

CCubelet::CCubelet(float SideLength, float RoundWidth, int RoundSteps)
: m_SideLength(SideLength)
, m_RoundSteps(RoundSteps)
, m_RoundWidth(RoundWidth)
{
  m_SideMaterial[0] = m_SideMaterial[1] = m_SideMaterial[2] = 0.15f;
  m_SideMaterial[3] = 1.0;
  m_Shininess[0] = 2; // The sides have small highlights.

  m_BevelDiffuse[0] = m_BevelDiffuse[1] = m_BevelDiffuse[2] = 0.25;
  m_BevelDiffuse[3] = 1.0;
  m_BevelMaterial[0] = m_BevelMaterial[1] = m_BevelMaterial[2] = 1.0;
  m_BevelMaterial[3] = 1.0;
  m_BevelShininess[0] = 115;
}

CCubelet::~CCubelet(void)
{
}


void CCubelet::Render() 
{
  glPushMatrix();

  /* We start with the sides */
  
  glMaterialfv(GL_FRONT, GL_SPECULAR, m_SideMaterial);
  glMaterialfv(GL_FRONT, GL_SHININESS, m_Shininess);

  glBegin(GL_QUADS);

  // The Right
  glColor3fv(m_Sides[0]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[0]);
  glNormal3f(1, 0, 0);

  glVertex3f(m_SideLength/2 + m_RoundWidth,  m_SideLength/2,  m_SideLength/2);
  glVertex3f(m_SideLength/2 + m_RoundWidth,  m_SideLength/2, -m_SideLength/2);
  glVertex3f(m_SideLength/2 + m_RoundWidth, -m_SideLength/2, -m_SideLength/2);
  glVertex3f(m_SideLength/2 + m_RoundWidth, -m_SideLength/2,  m_SideLength/2);

  // The Top
  glColor3fv(m_Sides[1]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[1]);
  glNormal3f(0, 1, 0);

  glVertex3f(-m_SideLength/2,  m_SideLength/2 + m_RoundWidth, -m_SideLength/2);
  glVertex3f(-m_SideLength/2,  m_SideLength/2 + m_RoundWidth,  m_SideLength/2);
  glVertex3f( m_SideLength/2,  m_SideLength/2 + m_RoundWidth,  m_SideLength/2);
  glVertex3f( m_SideLength/2,  m_SideLength/2 + m_RoundWidth, -m_SideLength/2);

  // The Front
  glColor3fv(m_Sides[2]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[2]);
  glNormal3f(0, 0, 1);

  glVertex3f( m_SideLength/2,  m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
  glVertex3f(-m_SideLength/2,  m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
  glVertex3f(-m_SideLength/2, -m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
  glVertex3f( m_SideLength/2, -m_SideLength/2,  m_SideLength/2 + m_RoundWidth);

  // The Back
  glColor3fv(m_Sides[3]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[3]);
  glNormal3f(0, 0, -1);

  glVertex3f( m_SideLength/2,  m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
  glVertex3f(-m_SideLength/2,  m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
  glVertex3f(-m_SideLength/2, -m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
  glVertex3f( m_SideLength/2, -m_SideLength/2, -m_SideLength/2 - m_RoundWidth);

  // The Bottom
  glColor3fv(m_Sides[4]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[4]);
  glNormal3f(0, -1, 0);

  glVertex3f(-m_SideLength/2, -m_SideLength/2 - m_RoundWidth, -m_SideLength/2);
  glVertex3f(-m_SideLength/2, -m_SideLength/2 - m_RoundWidth,  m_SideLength/2);
  glVertex3f( m_SideLength/2, -m_SideLength/2 - m_RoundWidth,  m_SideLength/2);
  glVertex3f( m_SideLength/2, -m_SideLength/2 - m_RoundWidth, -m_SideLength/2);

  // The Left
  glColor3fv(m_Sides[5]);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[5]);
  glNormal3f(-1, 0, 0);

  glVertex3f(-m_SideLength/2 - m_RoundWidth,  m_SideLength/2,  m_SideLength/2);
  glVertex3f(-m_SideLength/2 - m_RoundWidth,  m_SideLength/2, -m_SideLength/2);
  glVertex3f(-m_SideLength/2 - m_RoundWidth, -m_SideLength/2, -m_SideLength/2);
  glVertex3f(-m_SideLength/2 - m_RoundWidth, -m_SideLength/2,  m_SideLength/2);

  glEnd();

  /* Next comes the bevels */

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_BevelDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, m_BevelMaterial);
  glMaterialfv(GL_FRONT, GL_SHININESS, m_BevelShininess);
  //glColor3f(0.2, 0.2, 0.2);

  // Upper, right, back Corner
  glTranslatef(m_SideLength/2, m_SideLength/2, m_SideLength/2);
  MakeCornerSphere();

  // Upper, left, back Corner
  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeCornerSphere();
  glRotatef(-90, 0, 0, 1);

  // Lower, left, back Corner
  glTranslatef(0, -m_SideLength, 0);
  glRotatef(180, 0, 0, 1);
  MakeCornerSphere();
  glRotatef(-180, 0, 0, 1);

  // Lower, right, back Corner
  glTranslatef(m_SideLength, 0, 0);
  glRotatef(270, 0, 0, 1);
  MakeCornerSphere();
  glRotatef(-270, 0, 0, 1);

  // Upper, right, front Corner
  glTranslatef(0, m_SideLength, -m_SideLength);
  glRotatef(90, 0, 1, 0);
  MakeCornerSphere();
  glRotatef(-90, 0, 1, 0);


  // Upper, left, front Corner
  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(180, 0, 1, 0);
  MakeCornerSphere();
  glRotatef(-180, 0, 1, 0);

  // Lower, left, front Corner
  glTranslatef(0, -m_SideLength, 0);
  glRotatef(180, 0, 1, 0);
  glRotatef(90, 1, 0, 0);
  MakeCornerSphere();
  glRotatef(-90, 1, 0, 0);
  glRotatef(-180, 0, 1, 0);

  // Lower, right, front Corner
  glTranslatef(m_SideLength, 0, 0);
  glRotatef(180, 1, 0, 0);
  MakeCornerSphere();
  glRotatef(-180, 1, 0, 0);

  glTranslatef(0, m_SideLength, m_SideLength/2);
  
  MakeRoundedSide();
  
  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();
  glRotatef(-90, 0, 0, 1);

  glTranslatef(0, -m_SideLength, 0);
  glRotatef(180, 0, 0, 1);
  MakeRoundedSide();
  glRotatef(-180, 0, 0, 1);

  glTranslatef(m_SideLength, 0, 0);
  glRotatef(270, 0, 0, 1);
  MakeRoundedSide();
  glRotatef(-270, 0, 0, 1);

  glTranslatef(0, m_SideLength/2, m_SideLength/2);
  glRotatef(90, 1, 0, 0);
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glRotatef(-90, 1, 0, 0);
  glTranslatef(0, m_SideLength/2, -m_SideLength/2);
  
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glTranslatef(-m_SideLength, 0, 0);
  glRotatef(90, 0, 0, 1);
  MakeRoundedSide();

  glPopMatrix();

}


void CCubelet::MakeRoundedSide() 
{
  float StepSize = M_PI/2/m_RoundSteps;
  float v, vv;
  
  glBegin(GL_QUADS);
  glColor3f(0.5, 0.5, 0.5);
  
  for (int i = 0; i < m_RoundSteps; i++) 
  {
    v = i * StepSize;
    vv = (i+1)*StepSize;

    glNormal3f((float)cos(v),(float)sin(v), 0);
    glVertex3f((float)cos(v) * m_RoundWidth, (float)sin(v) * m_RoundWidth,  m_SideLength/2);
    glVertex3f((float)cos(v) * m_RoundWidth, (float)sin(v) * m_RoundWidth, -m_SideLength/2);

    glNormal3f((float)cos(vv), (float)sin(vv), 0);
    glVertex3f((float)cos(vv) * m_RoundWidth, (float)sin(vv) * m_RoundWidth, -m_SideLength/2);
    glVertex3f((float)cos(vv) * m_RoundWidth, (float)sin(vv) * m_RoundWidth,  m_SideLength/2);
  }

  glEnd();
}

void CCubelet::MakeCornerSphere() 
{
   float StepSize = M_PI/2/m_RoundSteps;
   float v, w, vv, ww;

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.5, 0.5);

   for (int i = 0; i < m_RoundSteps; i++) 
   {
      v = i * StepSize;
      vv = (i+1)*StepSize;

      for (int j = 0; j < m_RoundSteps; j++) 
      {
         w = j * StepSize;
         ww = (j+1) * StepSize;

         glNormal3f((float)cos(v) * (float)cos(w), (float)cos(v) * (float)sin(w),(float) sin(v));
         glVertex3f((float)cos(v) * (float)cos(w) * m_RoundWidth,
                  (float)cos(v) * (float)sin(w) * m_RoundWidth,
                  (float)sin(v) * m_RoundWidth);

         glNormal3f((float)cos(vv) * (float)cos(w), (float)cos(vv) * (float)sin(w), (float)sin(vv));
         glVertex3f((float)cos(vv) * (float)cos(w) * m_RoundWidth,
                  (float)cos(vv) * (float)sin(w) * m_RoundWidth,
                  (float)sin(vv) * m_RoundWidth);

         glNormal3f((float)cos(vv) * (float)cos(ww), (float)cos(vv) * (float)sin(ww), (float)sin(vv));
         glVertex3f((float)cos(vv) * (float)cos(ww) * m_RoundWidth,
                  (float)cos(vv) * (float)sin(ww) * m_RoundWidth,
                  (float)sin(vv) * m_RoundWidth);

         glNormal3f((float)cos(v) * (float)cos(ww), (float)cos(v) * (float)sin(ww), (float)sin(v));
         glVertex3f((float)cos(v) * (float)cos(ww) * m_RoundWidth,
                  (float)cos(v) * (float)sin(ww) * m_RoundWidth,
                  (float)sin(v) * m_RoundWidth);
      }
   }
   glEnd();
}

void CCubelet::CopyColor(float Dest[], float Src[]) 
{
  Dest[0] = Src[0];
  Dest[1] = Src[1];
  Dest[2] = Src[2];
}

void CCubelet::RotateColors(int a, int b, int c, int d) 
{
  float Temp[3];

  CopyColor(Temp, m_Sides[a]);
  CopyColor(m_Sides[a], m_Sides[b]);
  CopyColor(m_Sides[b], m_Sides[c]);
  CopyColor(m_Sides[c], m_Sides[d]);
  CopyColor(m_Sides[d], Temp);
}

   //1 = X Axis; 2 = Y Axis, 3 = Z Axis
void CCubelet::RotateCW(int Axis)
{
   switch (Axis) 
   {
   case 1:  RotateColors(3, 4, 2, 1);
            break;
   case 2:  RotateColors(0, 2, 5, 3);
            break;
   case 3:  RotateColors(0, 4, 5, 1);
            break;
  }
}

void CCubelet::Reset()
{
   for (int i = 0; i < 6;i++)
   {
      m_Sides[i][0] = m_Sides[i][1] = m_Sides[i][2] = 0.25;
   }
}

void CCubelet::RotateCCW(int Axis)
{
   switch (Axis) 
   {
   case 1:  RotateColors(1, 2, 4, 3);
            break;
   case 2:  RotateColors(3, 5, 2, 0);
            break;
   case 3:  RotateColors(1, 5, 4, 0);
            break;
  }
}

void CCubelet::SetColor(int Face,int Color)
{
   switch (Color)
   {
   case YELLOW :  m_Sides[Face][0] = 1;
                  m_Sides[Face][1] = 1;
                  m_Sides[Face][2] = 0;
                  break;
   case BLUE   :  m_Sides[Face][0] = 0;
                  m_Sides[Face][1] = 0;
                  m_Sides[Face][2] = 1;
                  break;
   case RED    :  m_Sides[Face][0] = 1;
                  m_Sides[Face][1] = 0;
                  m_Sides[Face][2] = 0;
                  break;
   case ORANGE :  m_Sides[Face][0] = 1;
                  m_Sides[Face][1] = 0.5;
                  m_Sides[Face][2] = 0;
                  break;
   case GREEN  :  m_Sides[Face][0] = 0;
                  m_Sides[Face][1] = 1;
                  m_Sides[Face][2] = 0;
                  break;
   case WHITE  :  m_Sides[Face][0] = 1;
                  m_Sides[Face][1] = 1;
                  m_Sides[Face][2] = 1;
                  break;
   }
}

void CCubelet::Copy(CCubelet *Src)
{

   for (int i = 0; i < 6;i++)
   {
      m_Sides[i][0] = Src->m_Sides[i][0];
      m_Sides[i][1] = Src->m_Sides[i][1];
      m_Sides[i][2] = Src->m_Sides[i][2];
   }
   
}