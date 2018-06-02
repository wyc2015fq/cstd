#pragma once

#include "Cubelet.h"

class C3DCube
{

public:
   C3DCube(void);
   ~C3DCube(void);

   void Render();
   void RotateX(unsigned int block, int degrees);
   void RotateY(unsigned int block, int degrees);
   void RotateZ(unsigned int block, int degrees);
   void SetColor(char Side,int Row,int Col,int Color);
   
   void Reset();
   void Save();
   void Restore();
private:
   CCubelet *m_Cubes[3][3][3];
   CCubelet *m_SavedCubes[3][3][3];
   float m_RotateX[3], m_RotateY[3], m_RotateZ[3];
   float m_CubeX, m_CubeY, m_CubeZ;
};
