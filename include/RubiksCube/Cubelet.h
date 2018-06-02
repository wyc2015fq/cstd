#pragma once

class CCubelet
{
public:
   CCubelet(float SideLength = 0.8, float RoundWidth = 0.1, int RoundSteps = 2);
   ~CCubelet(void);

   void Render();
   //1 = X Axis; 2 = Y Axis, 3 = Z Axis
   void RotateCW(int Axis);
   void RotateCCW(int Axis);

   void Reset();
   void SetColor(int Face,int Color);
   void Copy(CCubelet *Src);

private:
   float m_SideLength;
   float m_RoundWidth; 
   int m_RoundSteps;

   float m_Sides[6][3];
   float m_SideMaterial[4];
   float m_Shininess[1];
   float m_BevelDiffuse[4];
   float m_BevelMaterial[4];
   float m_BevelShininess[1];

   void RotateColors(int a, int b, int c, int d);
   void CopyColor(float Dest[], float Src[]);
   void MakeRoundedSide();
   void MakeCornerSphere();
};
