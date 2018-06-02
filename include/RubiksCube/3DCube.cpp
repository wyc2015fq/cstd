#include "StdAfx.h"
#include "3dcube.h"
#include <gl\gl.h>

C3DCube::C3DCube(void)
:m_CubeX(0)
,m_CubeY(0)
,m_CubeZ(0)
{
   //set the rotation angles to 0 degree
   for (int i = 0; i < 3;i++)
   {
      m_RotateX[i] = 0;
      m_RotateZ[i] = 0;
      m_RotateY[i] = 0;
   }

   //create each cubelet that makeup the rubic's cube
   for (int i = 0; i < 3; i++) 
   {
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[i][j][k] = new CCubelet();
            m_SavedCubes[i][j][k] = new CCubelet();
         }
      }
   }
   //set the colors for each side
   Reset();
}

C3DCube::~C3DCube(void)
{
   //destroy the cubelets of the cube
   for (int i = 0; i < 3; i++) 
   {
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            delete m_Cubes[i][j][k];
            delete m_SavedCubes[i][j][k];
         }
      }
   }
}


void C3DCube::Render() 
{
   glRotatef(m_CubeX, 1, 0, 0);
   glRotatef(m_CubeY, 0, 1, 0);
   glRotatef(m_CubeZ, 0, 0, 1);

   //render each cublete
   for (int i = 0; i < 3; i++) 
   {
      glRotatef(m_RotateX[i], 1, 0, 0);
      for (int j = 0; j < 3; j++) 
      {
         glRotatef(m_RotateY[j], 0, 1, 0);
         for (int k = 0; k < 3; k++) 
         {
            glRotatef(m_RotateZ[k], 0, 0, 1);

            glTranslatef(i-1.0f, j-1.0f, k-1.0f);
            m_Cubes[i][j][k]->Render();
            glTranslatef(1.0f-i, 1.0f-j, 1.0f-k);

            glRotatef(-m_RotateZ[k], 0, 0, 1);
         }
         glRotatef(-m_RotateY[j], 0, 1, 0);
      }
      glRotatef(-m_RotateX[i], 1, 0, 0);
   }

   glRotatef(-m_CubeZ, 0, 0, 1);
   glRotatef(-m_CubeY, 0, 1, 0);
   glRotatef(-m_CubeX, 1, 0, 0);
}


void C3DCube::RotateX(unsigned int block, int degrees) 
{
   m_RotateX[block] += degrees;

   if (degrees < 0 && m_RotateX[block] == -90) 
   {
      //We stop rotating, and instead rearrange the m_Cubes.
      m_RotateX[block] = 0;
    
      CCubelet *tmpcube = m_Cubes[block][0][0];
      m_Cubes[block][0][0] = m_Cubes[block][2][0];
      m_Cubes[block][2][0] = m_Cubes[block][2][2];
      m_Cubes[block][2][2] = m_Cubes[block][0][2];
      m_Cubes[block][0][2] = tmpcube;
    
      tmpcube = m_Cubes[block][1][0];
      m_Cubes[block][1][0] = m_Cubes[block][2][1];
      m_Cubes[block][2][1] = m_Cubes[block][1][2];
      m_Cubes[block][1][2] = m_Cubes[block][0][1];
      m_Cubes[block][0][1] = tmpcube;
    
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[block][j][k]->RotateCCW(1);
         }
      }
   } 
   else if (degrees > 0 && m_RotateX[block] == 90) 
   {
    /* We stop rotating, and instead rearrange the m_Cubes. */
      m_RotateX[block] = 0;
    
      CCubelet *tmpcube = m_Cubes[block][0][2];
      m_Cubes[block][0][2] = m_Cubes[block][2][2];
      m_Cubes[block][2][2] = m_Cubes[block][2][0];
      m_Cubes[block][2][0] = m_Cubes[block][0][0];
      m_Cubes[block][0][0] = tmpcube;
       
      tmpcube = m_Cubes[block][0][1];
      m_Cubes[block][0][1] = m_Cubes[block][1][2];
      m_Cubes[block][1][2] = m_Cubes[block][2][1];
      m_Cubes[block][2][1] = m_Cubes[block][1][0];
      m_Cubes[block][1][0] = tmpcube;
       
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[block][j][k]->RotateCW(1);
         }
      }
   }
  
   if ( (m_RotateX[0] == m_RotateX[1] ||
         m_RotateX[0] == m_RotateX[1]-90 ||
         m_RotateX[0] == m_RotateX[1]+90) &&
        (m_RotateX[1] == m_RotateX[2] ||
         m_RotateX[1] == m_RotateX[2]-90 ||
         m_RotateX[1] == m_RotateX[2]+90)) 
   {
       /* All three blocks have been turned the same amount, so we reset
       m_RotateX. */
      m_CubeX += m_RotateX[0];
      m_RotateX[0] = m_RotateX[1] = m_RotateX[2] = 0;
   }

}

void C3DCube::RotateY(unsigned int block, int degrees) 
{

   m_RotateY[block] += degrees;
     
   if (degrees < 0 && m_RotateY[block] == -90) 
   {
      //We stop rotating, and instead rearrange the m_Cubes.
      m_RotateY[block] = 0;
       
      CCubelet *tmpcube = m_Cubes[0][block][2];
      m_Cubes[0][block][2] = m_Cubes[2][block][2];
      m_Cubes[2][block][2] = m_Cubes[2][block][0];
      m_Cubes[2][block][0] = m_Cubes[0][block][0];
      m_Cubes[0][block][0] = tmpcube;
                                                
      tmpcube = m_Cubes[0][block][1];
      m_Cubes[0][block][1] = m_Cubes[1][block][2];
      m_Cubes[1][block][2] = m_Cubes[2][block][1];
      m_Cubes[2][block][1] = m_Cubes[1][block][0];
      m_Cubes[1][block][0] = tmpcube;
       
      for (int i = 0; i < 3; i++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[i][block][k]->RotateCCW(2);
         }
      }
   } 
   else if (degrees > 0 && m_RotateY[block] == 90) 
   {
      //We stop rotating, and instead rearrange the m_Cubes.
      m_RotateY[block] = 0;
       
      CCubelet *tmpcube = m_Cubes[0][block][0];
      m_Cubes[0][block][0] = m_Cubes[2][block][0];
      m_Cubes[2][block][0] = m_Cubes[2][block][2];
      m_Cubes[2][block][2] = m_Cubes[0][block][2];
      m_Cubes[0][block][2] = tmpcube;
       
      tmpcube = m_Cubes[1][block][0];
      m_Cubes[1][block][0] = m_Cubes[2][block][1];
      m_Cubes[2][block][1] = m_Cubes[1][block][2];
      m_Cubes[1][block][2] = m_Cubes[0][block][1];
      m_Cubes[0][block][1] = tmpcube;
       
      for (int i = 0; i < 3; i++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[i][block][k]->RotateCW(2);
         }
      }
   }

   if ( (m_RotateY[0] == m_RotateY[1] ||
         m_RotateY[0] == m_RotateY[1]-90 ||
         m_RotateY[0] == m_RotateY[1]+90) &&
        (m_RotateY[1] == m_RotateY[2] ||
         m_RotateY[1] == m_RotateY[2]-90 ||
         m_RotateY[1] == m_RotateY[2]+90)) 
   {
         //All three blocks have been turned the same amount, so we reset
         //m_RotateY.
         m_CubeY += m_RotateY[0];
         m_RotateY[0] = m_RotateY[1] = m_RotateY[2] = 0;
   }
}

void C3DCube::RotateZ(unsigned int block, int degrees) 
{
   m_RotateZ[block] += degrees;

   if (degrees < 0 && m_RotateZ[block] == -90) 
   {
      //We stop rotating, and instead rearrange the m_Cubes.
      m_RotateZ[block] = 0;
       
      CCubelet *tmpcube = m_Cubes[0][0][block];
      m_Cubes[0][0][block]   = m_Cubes[2][0][block];
      m_Cubes[2][0][block]   = m_Cubes[2][2][block];
      m_Cubes[2][2][block]   = m_Cubes[0][2][block];
      m_Cubes[0][2][block]   = tmpcube;
       
      tmpcube            = m_Cubes[1][0][block];
      m_Cubes[1][0][block] = m_Cubes[2][1][block];
      m_Cubes[2][1][block] = m_Cubes[1][2][block];
      m_Cubes[1][2][block] = m_Cubes[0][1][block];
      m_Cubes[0][1][block] = tmpcube;
       
      for (int i = 0; i < 3; i++) 
      {
         for (int j = 0; j < 3; j++) 
         {
            m_Cubes[i][j][block]->RotateCCW(3);
         }
      }
   } 
   else if (degrees > 0 && m_RotateZ[block] == 90) 
   {
      //We stop rotating, and instead rearrange the m_Cubes.
      m_RotateZ[block] = 0;
       
      CCubelet *tmpcube = m_Cubes[0][2][block];
      m_Cubes[0][2][block] = m_Cubes[2][2][block];
      m_Cubes[2][2][block] = m_Cubes[2][0][block];
      m_Cubes[2][0][block] = m_Cubes[0][0][block];
      m_Cubes[0][0][block] = tmpcube;
                                                
      tmpcube = m_Cubes[0][1][block];
      m_Cubes[0][1][block] = m_Cubes[1][2][block];
      m_Cubes[1][2][block] = m_Cubes[2][1][block];
      m_Cubes[2][1][block] = m_Cubes[1][0][block];
      m_Cubes[1][0][block] = tmpcube;
       
      for (int i = 0; i < 3; i++) 
      {
         for (int j = 0; j < 3; j++) 
         {
            m_Cubes[i][j][block]->RotateCW(3);
         }
      }
   }

   if ( (m_RotateZ[0] == m_RotateZ[1] ||
         m_RotateZ[0] == m_RotateZ[1]-90 ||
         m_RotateZ[0] == m_RotateZ[1]+90) &&
        (m_RotateZ[1] == m_RotateZ[2] ||
         m_RotateZ[1] == m_RotateZ[2]-90 ||
         m_RotateZ[1] == m_RotateZ[2]+90)) 
   {
         //All three blocks have been turned the same amount, so we reset
         //m_RotateZ.
      m_CubeZ += m_RotateZ[0];
      m_RotateZ[0] = m_RotateZ[1] = m_RotateZ[2] = 0;
   }
}

void C3DCube::SetColor(char Side,int Row,int Col,int Color)
{
   int X = 0;
   int Y = 0;
   int Z = 0;
   int Face = 0;
   switch (Side)
   {
   case 'F' :  Y = abs(Row-2);
               X = Col;
               Z = 2;
               Face = 2;
               break;
   case 'B' :  Y = abs(Row-2);
               X = abs(Col-2);
               Z = 0;
               Face = 3;
               break;
   case 'T' :  Y = 2;
               X = Col;
               Z = Row;
               Face = 1;
               break;
   case 'D' :  Y = 0;
               X = Col;
               Z = abs(Row-2);
               Face = 4;
               break;
   case 'L' :  X = 0;
               Y = abs(Row-2);
               Z = Col;
               Face = 5;
               break;
   case 'R' :  X = 2;
               Y = abs(Row-2);
               Z = abs(Col-2);
               Face = 0;
               break;

   }

   m_Cubes[X][Y][Z]->SetColor(Face,Color);
}


void C3DCube::Reset()
{
   for (int i = 0;i < 3;i++)
   {
      for (int j = 0; j < 3;j++)
      {
         for (int k = 0; k < 3;k++)
         {
            m_Cubes[i][j][k]->Reset();
         }
      }
   }

   char Side;
   for (int Face = 1; Face < 7;Face++)
   {
      switch (Face)
      {
      case 1:  Side = 'T';
               break;
      case 2:  Side = 'F';
               break;
      case 3:  Side = 'R';
               break;
      case 4:  Side = 'L';
               break;
      case 5:  Side = 'B';
               break;
      case 6:  Side = 'D';
               break;
      }
      for (int i = 0;i < 3;i++)
      {
         for (int j = 0;j < 3;j++)
         {
            SetColor(Side,i,j,Face);         
         }
      }
   }
}

void C3DCube::Save()
{
   for (int i = 0; i < 3; i++) 
   {
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_SavedCubes[i][j][k]->Copy(m_Cubes[i][j][k]);
         }
      }
   }
}

void C3DCube::Restore()
{
   for (int i = 0; i < 3; i++) 
   {
      for (int j = 0; j < 3; j++) 
      {
         for (int k = 0; k < 3; k++) 
         {
            m_Cubes[i][j][k]->Copy(m_SavedCubes[i][j][k]);
         }
      }
   }
}

