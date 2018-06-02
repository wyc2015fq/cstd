#include "stdafx.h"
#include "RubiksCube.h"
#include "CubeSolver.h"
//#include "timer.h"
#include "ColorRef.h"

#define SFRONT       1
#define SRIGHT       2
#define SBACK        4
#define SLEFT        8

#define FrontRight   3 
#define FrontBack    5
#define FrontLeft    9
#define RightBack    6 
#define RightLeft    10
#define BackLeft     12

#define MFRONTRIGHT  1
#define MFRONTLEFT   2
#define MFRONTBACK   4
#define MRIGHTBACK   8
#define MRIGHTLEFT   16
#define MRIGHTFRONT  32
#define MLEFTFRONT   64
#define MLEFTRIGHT   128
#define MLEFTBACK    256
#define MBACKLEFT    512
#define MBACKFRONT   1024  
#define MBACKRIGHT   2048


//the following define how the cells outside of 
//side move when the side is moving
//each three cells on a line will move to the 
//three cells on the next line. the movements are circular.
CSquare FrontMove[]  = {{'T',2,0},{'T',2,1},{'T',2,2},
                        {'R',0,0},{'R',1,0},{'R',2,0},
                        {'D',0,2},{'D',0,1},{'D',0,0},
                        {'L',2,2},{'L',1,2},{'L',0,2}};

CSquare LeftMove[]   = {{'T',0,0},{'T',1,0},{'T',2,0},
                        {'F',0,0},{'F',1,0},{'F',2,0},
                        {'D',0,0},{'D',1,0},{'D',2,0},
                        {'B',2,2},{'B',1,2},{'B',0,2}};

CSquare RightMove[]  = {{'T',2,2},{'T',1,2},{'T',0,2},
                        {'B',0,0},{'B',1,0},{'B',2,0},
                        {'D',2,2},{'D',1,2},{'D',0,2},
                        {'F',2,2},{'F',1,2},{'F',0,2}};

CSquare BackMove[]   = {{'T',0,2},{'T',0,1},{'T',0,0},
                        {'L',0,0},{'L',1,0},{'L',2,0},
                        {'D',2,0},{'D',2,1},{'D',2,2},
                        {'R',2,2},{'R',1,2},{'R',0,2}};

CSquare TopMove[]    = {{'B',0,2},{'B',0,1},{'B',0,0},
                        {'R',0,2},{'R',0,1},{'R',0,0},
                        {'F',0,2},{'F',0,1},{'F',0,0},
                        {'L',0,2},{'L',0,1},{'L',0,0}};

CSquare DownMove[]   = {{'F',2,0},{'F',2,1},{'F',2,2},
                        {'R',2,0},{'R',2,1},{'R',2,2},
                        {'B',2,0},{'B',2,1},{'B',2,2},
                        {'L',2,0},{'L',2,1},{'L',2,2}};


#define  NUM_EDGES      12
//the following lines define the edge cells of the cube
CSquare Edges[]      = {{'T',2,1},{'F',0,1},
                        {'T',0,1},{'B',0,1},
                        {'T',1,2},{'R',0,1},
                        {'T',1,0},{'L',0,1},
                        {'F',1,2},{'R',1,0},
                        {'F',2,1},{'D',0,1},
                        {'F',1,0},{'L',1,2},
                        {'D',1,2},{'R',2,1},
                        {'D',2,1},{'B',2,1},
                        {'D',1,0},{'L',2,1},
                        {'B',1,2},{'L',1,0},
                        {'B',1,0},{'R',1,2}};
#define  NUM_CORNERS    8
//the following define the corner cells of the cube
CSquare Corners[]    = {{'T',2,0},{'F',0,0},{'L',0,2},
                        {'T',2,2},{'F',0,2},{'R',0,0},
                        {'T',0,2},{'B',0,0},{'R',0,2},
                        {'T',0,0},{'B',0,2},{'L',0,0},
                        {'D',0,2},{'F',2,2},{'R',2,0},
                        {'D',0,0},{'F',2,0},{'L',2,2},
                        {'D',2,0},{'B',2,2},{'L',2,0},
                        {'D',2,2},{'B',2,0},{'R',2,2}};



// CCubeSolver
CCubeSolver::CCubeSolver()
{
   m_Top = new CSide(YELLOW,'T',TopMove);
   m_Front = new CSide(BLUE,'F',FrontMove);
   m_Right = new CSide(RED,'R',RightMove);
   m_Left = new CSide(ORANGE,'L',LeftMove);
   m_Back = new CSide(GREEN,'B',BackMove);
   m_Down = new CSide(WHITE,'D',DownMove);
   
   m_OTop = new CSide(YELLOW,'T',TopMove);
   m_OFront = new CSide(BLUE,'F',FrontMove);
   m_ORight = new CSide(RED,'R',RightMove);
   m_OLeft = new CSide(ORANGE,'L',LeftMove);
   m_OBack = new CSide(GREEN,'B',BackMove);
   m_ODown = new CSide(WHITE,'D',DownMove);
}

CCubeSolver::~CCubeSolver()
{
   delete m_Top;
   delete m_Front;
   delete m_Right;
   delete m_Left;
   delete m_Back;
   delete m_Down;

   delete m_OTop;
   delete m_OFront;
   delete m_ORight;
   delete m_OLeft;
   delete m_OBack;
   delete m_ODown;
}

//clear the cube. Clears all the colors except for the center cubelets
void CCubeSolver::Clear()
{
   m_Top->Clear();
   m_Front->Clear();
   m_Right->Clear();
   m_Left->Clear();
   m_Back->Clear();
   m_Down->Clear();
   UpdateDisplay();
}

//reset the cube to it's solved state by setting the color of each side
void CCubeSolver::Reset()
{
   m_Top->SetColor(1);
   m_Front->SetColor(2);
   m_Right->SetColor(3);
   m_Left->SetColor(4);
   m_Back->SetColor(5);
   m_Down->SetColor(6);
   UpdateDisplay();
}


//randomly shuffle the cube by doing moves
void CCubeSolver::Shuffle(CProgressCtrl *pProgress)
{
   //setup the progress bar
   pProgress->SetRange(0,22);
   pProgress->SetPos(0);
   pProgress->SetStep(1);

   int LastFace = -1;
   //they say that a cube is fully randomized with 22 moves
   for (int i = 0;i < 22;i++)
   {
      //get a face at random
      int Face = rand() % 6;
      //get a direction at random
      int Dir = rand() % 2;
      //while the face is the same as the last face we moved
      while (Face == LastFace)
      {
         //get another face
         Face = rand() % 6;
      }
      LastFace = Face;
      CSide *pSide = NULL;
      //get the pointer to the face we picked at random
      switch (Face)
      {
      case 0 : pSide = m_Front;
               break;
      case 1 : pSide = m_Back;
               break;
      case 2 : pSide = m_Right;
               break;
      case 3 : pSide = m_Left;
               break;
      case 4 : pSide = m_Down;
               break;
      case 5 : pSide = m_Top;
               break;
      }

      //move the face in the direction we chose at random
      if (pSide)
      {
         switch (Dir)
         {
         case 0 : RotateCW(pSide);
                  break;
         case 1 : RotateCCW(pSide);
                  break;
         case 2 : RotateCW(pSide);
                  RotateCW(pSide);
                  break;
         }
      }
      //update the progress bar
      pProgress->StepIt();
      //update the display if any
      UpdateDisplay();
      YieldToOthers();

   }
   //clear the progress bar
   pProgress->SetPos(0);
   UpdateDisplay(true);
}


//returns a pointer to a side given the side letter.
//T = Top, F = Front, D = Down, R = Right, L = Left, B = Bottom
CSide *CCubeSolver::GetSide(char Side)
{
   Side = toupper(Side);
   switch (Side)
   {
   case 'T':   return m_Top;
   case 'F':   return m_Front;
   case 'D':   return m_Down;
   case 'R':   return m_Right;
   case 'L':   return m_Left;
   case 'B' :  return m_Back;
   };
   return NULL;
}

//rotate the entire cube to the left
//F = R, R = B, B = L, L = F
void CCubeSolver::RotateCubeLeft()
{
   CSide pTemp = *m_Front;

   *m_Front = *m_Right;
   *m_Right = *m_Back;
   *m_Back = *m_Left;
   *m_Left = pTemp;

   RotateFaceCW(m_Top);
   RotateFaceCCW(m_Down);
}

//rotate the entire cube to the right
//F = L, L = B, B = R, R = F
void CCubeSolver::RotateCubeRight()
{
   CSide pTemp = *m_Front;

   *m_Front = *m_Left;
   *m_Left = *m_Back;
   *m_Back = *m_Right;
   *m_Right = pTemp;

   RotateFaceCCW(m_Top);
   RotateFaceCW(m_Down);
}

//rotate the cube upward
//F = D, D = B, B = T, T = F
void CCubeSolver::RotateCubeUp()
{
   CSide pTemp = *m_Top;

   *m_Top = *m_Front;
   *m_Front = *m_Down;
   *m_Down = *m_Back;
   *m_Back = pTemp;

   RotateFaceCW(m_Back);
   RotateFaceCW(m_Back);
   RotateFaceCW(m_Down);
   RotateFaceCW(m_Down);
   RotateFaceCW(m_Right);
   RotateFaceCCW(m_Left);
}

//rotate the cube downward
//F = T, T = B, B = D, D = F
void CCubeSolver::RotateCubeDown()
{
   CSide pTemp = *m_Top;

   *m_Top = *m_Back;
   *m_Back = *m_Down;
   *m_Down = *m_Front;
   *m_Front = pTemp;

   RotateFaceCW(m_Back);
   RotateFaceCW(m_Back);
   RotateFaceCW(m_Top);
   RotateFaceCW(m_Top);
   RotateFaceCW(m_Left);
   RotateFaceCCW(m_Right);
}

//rotate the cube clockwise
//T = L, L = D, D = R, R = T
void CCubeSolver::RotateCubeCW()
{
   CSide pTemp = *m_Top;

   *m_Top = *m_Left;
   *m_Left = *m_Down;
   *m_Down = *m_Right;
   *m_Right = pTemp;

   RotateFaceCW(m_Top);
   RotateFaceCW(m_Down);
   RotateFaceCW(m_Left);
   RotateFaceCW(m_Right);
   RotateFaceCW(m_Front);
   RotateFaceCCW(m_Back);
}

//rotate the cube counter clockwise
//T = R, R = D, D = L, L = T
void CCubeSolver::RotateCubeCCW()
{
   CSide pTemp = *m_Top;

   *m_Top = *m_Right;
   *m_Right = *m_Down;
   *m_Down = *m_Left;
   *m_Left = pTemp;

   RotateFaceCCW(m_Top);
   RotateFaceCCW(m_Down);
   RotateFaceCCW(m_Left);
   RotateFaceCCW(m_Right);
   RotateFaceCCW(m_Front);
   RotateFaceCW(m_Back);
}

//rotate a face on the cube clockwise
void CCubeSolver::RotateFaceCW(CSide *pSide)
{
   char Temp1 = pSide->m_Blocks[1][2];
   char Temp2 = pSide->m_Blocks[2][2];

   pSide->m_Blocks[2][2] = pSide->m_Blocks[0][2];
   pSide->m_Blocks[1][2] = pSide->m_Blocks[0][1];
   pSide->m_Blocks[0][2] = pSide->m_Blocks[0][0];

   pSide->m_Blocks[0][2] = pSide->m_Blocks[0][0];
   pSide->m_Blocks[0][1] = pSide->m_Blocks[1][0];
   pSide->m_Blocks[0][0] = pSide->m_Blocks[2][0];

   pSide->m_Blocks[1][0] = pSide->m_Blocks[2][1];
   pSide->m_Blocks[2][0] = Temp2;
   pSide->m_Blocks[2][1] = Temp1;
}

//rotate a face on the cube counter clockwise
void CCubeSolver::RotateFaceCCW(CSide *pSide)
{
   char Temp1 = pSide->m_Blocks[0][0];
   char Temp2 = pSide->m_Blocks[0][1];

   pSide->m_Blocks[0][0] = pSide->m_Blocks[0][2];
   pSide->m_Blocks[0][1] = pSide->m_Blocks[1][2];
   pSide->m_Blocks[0][2] = pSide->m_Blocks[2][2];

   pSide->m_Blocks[0][2] = pSide->m_Blocks[2][2];
   pSide->m_Blocks[1][2] = pSide->m_Blocks[2][1];
   pSide->m_Blocks[2][2] = pSide->m_Blocks[2][0];
   
   pSide->m_Blocks[2][1] = pSide->m_Blocks[1][0];
   pSide->m_Blocks[2][0] = Temp1;
   pSide->m_Blocks[1][0] = Temp2;
}


//rotate a side clickwise
void CCubeSolver::RotateCW(CSide *pSide)
{
   if (!pSide)
   {
      return;
   }

   RotateFaceCW(pSide);

   CSquare *Moves = pSide->m_Moves;
   char Temp1 = GetSide(Moves[9].Side)->m_Blocks[Moves[9].Row][Moves[9].Col];
   char Temp2 = GetSide(Moves[10].Side)->m_Blocks[Moves[10].Row][Moves[10].Col];
   char Temp3 = GetSide(Moves[11].Side)->m_Blocks[Moves[11].Row][Moves[11].Col];
   for (int i = 8; i >= 0;i--)
   {
      GetSide(Moves[i+3].Side)->m_Blocks[Moves[i+3].Row][Moves[i+3].Col] = 
      GetSide(Moves[i].Side)->m_Blocks[Moves[i].Row][Moves[i].Col];
   }
   GetSide(Moves[0].Side)->m_Blocks[Moves[0].Row][Moves[0].Col] = Temp1;
   GetSide(Moves[1].Side)->m_Blocks[Moves[1].Row][Moves[1].Col] = Temp2;
   GetSide(Moves[2].Side)->m_Blocks[Moves[2].Row][Moves[2].Col] = Temp3;
}

//rotate a side counter clockwise
void CCubeSolver::RotateCCW(CSide *pSide)
{
   if (!pSide)
   {
      return;
   }

   RotateFaceCCW(pSide);


   CSquare *Moves = pSide->m_Moves;
   char Temp1 = GetSide(Moves[0].Side)->m_Blocks[Moves[0].Row][Moves[0].Col];
   char Temp2 = GetSide(Moves[1].Side)->m_Blocks[Moves[1].Row][Moves[1].Col];
   char Temp3 = GetSide(Moves[2].Side)->m_Blocks[Moves[2].Row][Moves[2].Col];
   for (int i = 0; i < 9;i++)
   {
      GetSide(Moves[i].Side)->m_Blocks[Moves[i].Row][Moves[i].Col] = 
      GetSide(Moves[i+3].Side)->m_Blocks[Moves[i+3].Row][Moves[i+3].Col];
   }
   GetSide(Moves[9].Side)->m_Blocks[Moves[9].Row][Moves[9].Col] = Temp1;
   GetSide(Moves[10].Side)->m_Blocks[Moves[10].Row][Moves[10].Col] = Temp2;
   GetSide(Moves[11].Side)->m_Blocks[Moves[11].Row][Moves[11].Col] = Temp3;
}

//find a side with a center cubelet having color "Color"
CSide *CCubeSolver::FindCenter(char Color)
{
   if (*m_Top == Color)
   {
      return m_Top;
   }
   else if (*m_Front == Color)
   {
      return m_Front;
   }
   else if (*m_Down == Color)
   {
      return m_Down;
   }
   else if (*m_Back == Color)
   {
      return m_Back;
   }
   else if (*m_Right == Color)
   {
      return m_Right;
   }
   else if (*m_Left  == Color)
   {
      return m_Left;
   }

   return NULL;
}

//find the side with the edge having the two colors
//the side that is returned is the side with "Color"
//to get the other side reverse the paramters in the
//function call
CSide *CCubeSolver::FindEdge(char Color, char Color2)
{
   for (int i = 0; i < NUM_EDGES;i++)
   {
      CSide *pSide1 = GetSide(Edges[i*2].Side);
      CSide *pSide2 = GetSide(Edges[(i*2)+1].Side);
      char Side1 = pSide1->m_Blocks[Edges[(i*2)].Row][Edges[(i*2)].Col];
      char Side2 = pSide2->m_Blocks[Edges[(i*2)+1].Row][Edges[(i*2)+1].Col];
      if (Side1 == Color && Side2 == Color2)
      {
         return pSide1;
      }
      if (Side2 == Color && Side1 == Color2)
      {
         return pSide2;
      }
   }

   return NULL;
}

//find a corner with the given three colors
//the Side that is returned is the side with "Color"
CSide *CCubeSolver::FindCorner(char Color, char Color2, char Color3)
{
   for (int i = 0; i < NUM_CORNERS;i++)
   {
      CSide *pSide1 = GetSide(Corners[i*3].Side);
      CSide *pSide2 = GetSide(Corners[(i*3)+1].Side);
      CSide *pSide3 = GetSide(Corners[(i*3)+2].Side);
      char Side1 = pSide1->m_Blocks[Corners[(i*3)].Row][Corners[(i*3)].Col];
      char Side2 = pSide2->m_Blocks[Corners[(i*3)+1].Row][Corners[(i*3)+1].Col];
      char Side3 = pSide3->m_Blocks[Corners[(i*3)+2].Row][Corners[(i*3)+2].Col];

      if (Side1 == Color && (Side2 == Color2 || Side2 == Color3) && (Side3 == Color3 || Side3 == Color2))
      {
         return pSide1;
      }
      if (Side2 == Color && (Side1 == Color2 || Side1 == Color3) && (Side3 == Color3 || Side3 == Color2))
      {
         return pSide2;
      }
      if (Side3 == Color && (Side1 == Color2 || Side1 == Color3) && (Side2 == Color2 || Side2 == Color3))
      {
         return pSide3;
      }
   }

   return NULL;
}

//save a face of the cube
void CCubeSolver::SaveSide(CSide *pSide,CSide *pSaveTo)
{
   for (int i = 0;i < 3;i++)
   {
      for (int j = 0;j < 3;j++)
      {
         pSaveTo->m_Blocks[i][j] = pSide->m_Blocks[i][j];
      }
   }
}


//Save the cube
void CCubeSolver::SaveCube()
{
   SaveSide(m_Top,m_OTop);
   SaveSide(m_Front,m_OFront);
   SaveSide(m_Right,m_ORight);
   SaveSide(m_Left,m_OLeft);
   SaveSide(m_Back,m_OBack);
   SaveSide(m_Down,m_ODown);
}

//restore a saved cube
void CCubeSolver::RestoreCube()
{
   SaveSide(m_OTop,m_Top);
   SaveSide(m_OFront,m_Front);
   SaveSide(m_ORight,m_Right);
   SaveSide(m_OLeft,m_Left);
   SaveSide(m_OBack,m_Back);
   SaveSide(m_ODown,m_Down);
}


//solve the top edge of the cube
void CCubeSolver::SolveTopEdges(CProgressCtrl *pProgress)
{
   int Count = 0;
   CSide *pSide1;
   CSide *pSide2;
   //while the Edges are not solved
   for (int i = 0; i < 4;i++)
   {
      pProgress->StepIt();
      Count++; 
      
      if (Count > 4)
      {
         Count = 1;
      }

      pSide1 = FindEdge(*m_Top, *m_Front); 
      pSide2 = FindEdge(*m_Front, *m_Top);
      
      if (pSide1 == m_Right)
      {
         if (pSide2 == m_Front)
         {
            m_S += "F-.";
            DoMove("F-.");
         }
         else if (pSide2 == m_Top)
         {
            m_S += "R-.F-.";
            DoMove("R-.F-.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "T+.T+.B+.T+.T+.";
            DoMove("T+.T+.B+.T+.T+.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "R+.F-.R-.";
            DoMove("R+.F-.R-.");
         }
      }
      else if (pSide1 == m_Left)
      {
         if (pSide2 == m_Top)
         {
            m_S += "L+.F+.";
            DoMove("L+.F+.");
         }
         else if (pSide2 == m_Front)
         {
            m_S += "F+.";
            DoMove("F+.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "L+.L+.F+.L+.L+.";
            DoMove("L+.L+.F+.L+.L+.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "L-.F+.L+.";
            DoMove("L-.F+.L+.");
         }
      }
      else if (pSide1 == m_Front)
      {
         if (pSide2 == m_Top)
         {
            m_S += "F-.T+.L-.T-.";
            DoMove("F-.T+.L-.T-.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "T+.L-.T-.";
            DoMove("T+.L-.T-.");
         }
         else if (pSide2 == m_Right)
         {
            m_S += "T-.R+.T+.";
            DoMove("T-.R+.T+.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "F-.T-.R+.T+.";
            DoMove("F-.T-.R+.T+.");
         }
      }
      else if (pSide1 == m_Back)
      {
         if (pSide2 == m_Right)
         {
            m_S += "T-.R-.T+.";
            DoMove("T-.R-.T+.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "T+.L+.T-.";
            DoMove("T+.L+.T-.");
         }
         else if (pSide2 == m_Top)
         {
            m_S += "B-.T-.R-.T+.";
            DoMove("B-.T-.R-.T+.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "D-.R+.F-.R-.";
            DoMove("D-.R+.F-.R-.");
         }
      }
      else if (pSide1 == m_Down)
      {
         if (pSide2 == m_Front)
         {
            m_S += "F+.F+.";
            DoMove("F+.F+.");
         }
         else if (pSide2 == m_Right)
         {
            m_S += "D-.F+.F+.";
            DoMove("D-.F+.F+.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "D+.F+.F+.";
            DoMove("D+.F+.F+.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "D+.D+.F+.F+.";
            DoMove("D+.D+.F+.F+.");
         }
      }
      else if (pSide1 == m_Top)
      {
         if (pSide2 == m_Left)
         {
            m_S += "L-.T+.L+.T-.";
            DoMove("L-.T+.L+.T-.");
         }
         else if (pSide2 == m_Right)
         {
            m_S += "R+.T-.R-.T+.";
            DoMove("R+.T-.R-.T+.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "B-.T+.T+.B+.T+.T+.";
            DoMove("B-.T+.T+.B+.T+.T+.");
         }
      }
      m_S += "C+.";
      DoMove("C+.");
   }
}

//solve the top corner of the cube
void CCubeSolver::SolveTopCorners(CProgressCtrl *pProgress)
{
   for (int i = 0; i < 4;i++)
   {
      pProgress->StepIt();
      CSide *pSide1 = FindCorner(*m_Top,*m_Front,*m_Right);
      CSide *pSide2 = FindCorner(*m_Front,*m_Top,*m_Right);

      if (pSide1 == m_Front)
      {
         if (pSide2 == m_Right)
         {
            m_S += "F+.D+.F-.D+.D+.R-.D+.R+.";
            DoMove("F+.D+.F-.D+.D+.R-.D+.R+.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "D+.D+.F+.D-.F-.";
            DoMove("D+.D+.F+.D-.F-.");
         }
         else if (pSide2 == m_Top)
         {
            m_S += "F-.D-.F+.F+.D+.D+.F-.";
            DoMove("F-.D-.F+.F+.D+.D+.F-.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "D-.R-.D+.R+.";
            DoMove("D-.R-.D+.R+.");
         }

      }
      else if (pSide1 == m_Right)
      {
         if (pSide2 == m_Front)
         {
            m_S += "D+.F+.D-.F-.";
            DoMove("D+.F+.D-.F-.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "R+.D+.D+.R+.R+.D+.R+.";
            DoMove("R+.D+.D+.R+.R+.D+.R+.");
         }
         else if (pSide2 == m_Top)
         {
            m_S += "R-.D-.R+.D+.D+.F+.D-.F-.";
            DoMove("R-.D-.R+.D+.D+.F+.D-.F-.");
         }
         else if (pSide2 == m_Down)
         {
            m_S += "D+.D+.R-.D+.R+.";
            DoMove("D+.D+.R-.D+.R+.");
         }
      }
      else if (pSide1 == m_Left)
      {
         if (pSide2 == m_Down)
         {
            m_S += "R-.D+.R+.";
            DoMove("R-.D+.R+.");
         }
         else if (pSide2 == m_Front)
         {
            m_S += "L+.D+.L-.D-.R-.D+.R+.";
            DoMove("L+.D+.L-.D-.R-.D+.R+.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "F+.D+.D+.F-.";
            DoMove("F+.D+.D+.F-.");
         }
         else if (pSide2 == m_Top)
         {
            m_S += "L-.D-.L+.F+.D-.F-.";
            DoMove("L-.D-.L+.F+.D-.F-.");
         }
      }
      else if (pSide1 == m_Back)
      {
          if (pSide2 == m_Down)
          {
             m_S += "R-.D+.D+.R+.";
            DoMove("R-.D+.D+.R+.");
          }
          else if (pSide2 == m_Right)
          {
             m_S += "F+.D-.F-.";
            DoMove("F+.D-.F-.");
          }
          else if (pSide2 == m_Left)
          {
             m_S += "B+.D+.B-.R-.D+.R+.";
            DoMove("B+.D+.B-.R-.D+.R+.");
          }
          else if (pSide2 == m_Top)
          {
            m_S += "B-.F+.D-.F-.B+.";
            DoMove("B-.F+.D-.F-.B+.");
          }
      }
      else if (pSide1 == m_Top)
      {
         if (pSide2 == m_Left)
         {
            m_S +="L+.D-.L-.R-.D+.R+.";
            DoMove("L+.D-.L-.R-.D+.R+.");
         }
         else if (pSide2 == m_Right)
         {
            m_S += "B-.D+.B+.F+.D-.F-.";
            DoMove("B-.D+.B+.F+.D-.F-.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "L-.R-.D+.D+.R+.L+.";
            DoMove("L-.R-.D+.D+.R+.L+.");
         }
      }
      else if (pSide1 == m_Down)
      {
         if (pSide2 == m_Front)
         {
            m_S += "D+.R-.D+.R+.F+.D+.D+.F-.";
            DoMove("D+.R-.D+.R+.F+.D+.D+.F-.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "D+.D+.R-.D+.R+.F+.D+.D+.F-.";
            DoMove("D+.D+.R-.D+.R+.F+.D+.D+.F-.");
         }
         else if (pSide2 == m_Back)
         {
            m_S += "D-.R-.D+.R+.F+.D+.D+.F-.";
            DoMove("D-.R-.D+.R+.F+.D+.D+.F-.");
         }
         else if (pSide2 == m_Right)
         {
            m_S += "R-.D+.R+.F+.D+.D+.F-.";
            DoMove("R-.D+.R+.F+.D+.D+.F-.");
         }
      }
      m_S += "C+.";
      DoMove("C+.");
   }
}

//solve the middle edges
void CCubeSolver::SolveMiddleEdges(CProgressCtrl *pProgress)
{
   for (int i = 0; i < 4;i++)
   {
      pProgress->StepIt();
      CSide *pSide1 = FindEdge(*m_Front,*m_Right);
      CSide *pSide2 = FindEdge(*m_Right,*m_Front);
      
      //check to see if the side we are looking for is 
      //in one of the middle sides. If it is move it out

      if (pSide1 == m_Left && pSide2 == m_Back ||
          pSide1 == m_Back && pSide2 == m_Left)
      {
         m_S += "B+.D-.B-.D-.L-.D+.L+.";
         DoMove("B+.D-.B-.D-.L-.D+.L+.");
      }
      else if (pSide1 == m_Left && pSide2 == m_Front ||
               pSide1 == m_Front && pSide2 == m_Left)
      {
         m_S += "L+.D-.L-.D-.F-.D+.F+.";
         DoMove("L+.D-.L-.D-.F-.D+.F+.");
      }
      else if (pSide1 == m_Right && pSide2 == m_Back ||
               pSide1 == m_Back && pSide2 == m_Right)
      {
         m_S += "B-.D+.B+.D+.R+.D-.R-.";
         DoMove("B-.D+.B+.D+.R+.D-.R-.");
      }
      else if (pSide1 == m_Right && pSide2 == m_Front)
      {
         m_S += "R-.D+.R+.D+.F+.D-.F-.";
         DoMove("R-.D+.R+.D+.F+.D-.F-.");
      }
      
      //now that it is moved out find it again and put it in place
      pSide1 = FindEdge(*m_Front,*m_Right);
      pSide2 = FindEdge(*m_Right,*m_Front);

      if (pSide1 == m_Down)
      {
         if (pSide2 == m_Right)
         {
            m_S += "D+.";
            DoMove("D+.");
         }
         else if (pSide2 == m_Front)
         {
            m_S += "D+.D+.";
            DoMove("D+.D+.");
         }
         else if (pSide2 == m_Left)
         {
            m_S += "D-.";
            DoMove("D-.");
         }
         m_S += "F+.D-.F-.D-.R-.D+.R+.";
         DoMove("F+.D-.F-.D-.R-.D+.R+.");
      }
      else if (pSide2 == m_Down)
      {
         if (pSide1 == m_Right)
         {
            m_S += "D+.D+.";
            DoMove("D+.D+.");
         }
         else if (pSide1 == m_Front)
         {
            m_S += "D-.";
            DoMove("D-.");
      }
         else if (pSide1 == m_Back)
         {
            m_S += "D+.";
            DoMove("D+.");
         }
         m_S += "R-.D+.R+.D+.F+.D-.F-.";
         DoMove("R-.D+.R+.D+.F+.D-.F-.");
      }
      m_S += "C+.";
      DoMove("C+.");
   }
}

//get the orientation of the bottom edges
//this routine will tell how many are oriented correctly
//and which ones they are
int CCubeSolver::GetDownEdgeOrient(int &Flag)
{
   int Count = 0;
   Flag = 0;
   if (m_Down->m_Blocks[0][1] == *m_Down)
   {
      Count++;
      Flag |= SFRONT;
   }
   if (m_Down->m_Blocks[1][2] == *m_Down)
   {
      Count++;
      Flag |= SRIGHT;
   }
   if (m_Down->m_Blocks[2][1] == *m_Down)
   {
      Count++;
      Flag |= SBACK;
   }
   if (m_Down->m_Blocks[1][0] == *m_Down)
   {
      Count++;
      Flag |= SLEFT;
   }

   return Count;
}

//return how many bottom edge pieces are positioned correctly
//and will tell which ones are positioned correctly
int CCubeSolver::GetDownEdgePos(int &Flag)
{
   int Count = 0;
   Flag = 0;
   CSide *pSide;
   if ((pSide = FindEdge(*m_Front,*m_Down)) != m_Front)
   {
      Count++;
      if (pSide == m_Left)
      {
         Flag |= MLEFTFRONT;
      }
      else if (pSide == m_Right)
      {
         Flag |= MRIGHTFRONT;
      }
      else if (pSide == m_Back)
      {
         Flag |= MBACKFRONT;
      }
   }
   if ((pSide = FindEdge(*m_Right,*m_Down)) != m_Right)
   {
      Count++;
      if (pSide == m_Front)
      {
         Flag |= MFRONTRIGHT;
      }
      else if (pSide == m_Left)
      {
         Flag |= MLEFTRIGHT;
      }
      else if (pSide == m_Back)
      {
         Flag |= MBACKRIGHT;
      }
   }
   if ((pSide = FindEdge(*m_Back,*m_Down)) != m_Back)
   {
      Count++;
      if (pSide == m_Front)
      {
         Flag |= MFRONTBACK;
      }
      else if (pSide == m_Right)
      {
         Flag |= MRIGHTBACK;
      }
      else if (pSide == m_Left)
      {
         Flag |= MLEFTBACK;
      }
   }
   if ((pSide = FindEdge(*m_Left,*m_Down)) != m_Left)
   {
      Count++;
      if (pSide == m_Front)
      {
         Flag |= MFRONTLEFT;
      }
      else if (pSide == m_Right)
      {
         Flag |= MRIGHTLEFT;
      }
      else if (pSide == m_Back)
      {
         Flag |= MBACKLEFT;
      }
   }
   return Count;
}


//solve the bottom edge pieces
bool CCubeSolver::SolveDownEdge(CProgressCtrl *pProgress)
{
int Count;
int Flag;
int Iter = 0;
   
   //first orient the edge pieces
   while ((Count = GetDownEdgeOrient(Flag))< 4)
   {
      Iter++;
      pProgress->StepIt();

      if (Count < 2)
      {
         m_S += "F+.D+.L+.D-.L-.F-.";
         DoMove("F+.D+.L+.D-.L-.F-.");
      }
      else if (Flag == RightLeft)
      {
         m_S += "F+.L+.D+.L-.D-.F-.";
         DoMove("F+.L+.D+.L-.D-.F-.");
      }
      else if (Flag == FrontBack)
      {
         m_S += "C+.F+.L+.D+.L-.D-.F-.";
         DoMove("C+.F+.L+.D+.L-.D-.F-.");
      }
      else if (Flag == RightBack)
      {
         m_S += "F+.D+.L+.D-.L-.F-.";
         DoMove("F+.D+.L+.D-.L-.F-.");
      }
      else if (Flag == FrontRight)
      {
         m_S += "C+.F+.D+.L+.D-.L-.F-.";
         DoMove("C+.F+.D+.L+.D-.L-.F-.");
      }
      else if (Flag == FrontLeft)
      {
         m_S += "C+.C+.F+.D+.L+.D-.L-.F-.";
         DoMove("C+.C+.F+.D+.L+.D-.L-.F-.");
      }
      else if (Flag == BackLeft)
      {
         m_S += "C+.C+.C+.F+.D+.L+.D-.L-.F-.";
         DoMove("C+.C+.C+.F+.D+.L+.D-.L-.F-.");
      }
      if (Iter > 10)
      {
         return false;
      }
   }

   Iter = 0;
   //Now Position the pieces correctly
   while ((Count = GetDownEdgePos(Flag)) > 0)
   {
      Iter++;
      pProgress->StepIt();
      if (Count == 4)
      {
         m_S += "D+.";
         DoMove("D+.");
      }
      else if (Count == 3)
      {
         while (1)
         {
            if ((Flag & MRIGHTLEFT) && (Flag & MLEFTFRONT) && (Flag & MFRONTRIGHT))
            {
               m_S += "L-.D-.L+.D-.L-.D+.D+.L+.";
               DoMove("L-.D-.L+.D-.L-.D+.D+.L+.");
               break;
            }
            else if ((Flag & MLEFTRIGHT) && (Flag & MRIGHTFRONT) && (Flag & MFRONTLEFT))
            {
               m_S += "L-.D+.D+.L+.D+.L-.D+.L+.";
               DoMove("L-.D+.D+.L+.D+.L-.D+.L+.");
               break;
            }
            m_S += "C+.";
            DoMove("C+.");
            GetDownEdgePos(Flag);
         }
      }
      else if (Count == 2)
      {
         while (1)
         {
            if (Flag & MRIGHTFRONT)
            {
               m_S += "D+.L-.D+.D+.L+.D+.L-.D+.L+.";
               DoMove("D+.L-.D+.D+.L+.D+.L-.D+.L+.");
               break;
            }
            else if (Flag & MFRONTBACK)
            {
               m_S += "L-.D+.D+.L+.D+.L-.D+.L+.D-.L-.D+.D+.L+.D+.L-.D+.L+.";
               DoMove("L-.D+.D+.L+.D+.L-.D+.L+.D-.L-.D+.D+.L+.D+.L-.D+.L+.");
               break;
            }
            m_S += "C+.";
            DoMove("C+.");
            GetDownEdgePos(Flag);
         }
      }
      if (Iter > 10)
      {
         return false;
      }
   }
   return true;
}

int CCubeSolver::GetCornersSide(CSide *pSide1,CSide *pSide2)
{
   int Flag = 0;
   if (pSide1 == m_Back)
   {
      if (pSide2 == m_Right)
      {
         Flag |= SRIGHT;
      }
      else 
      {
         Flag |= SBACK;
      }
   }
   else if (pSide1 == m_Front)
   {
      if (pSide2 == m_Right)
      {
         Flag |= SFRONT;
      }
      else
      {
         Flag |= SLEFT;
      }
      
   }
   else if (pSide1 == m_Right)
   {
      if (pSide2 == m_Front)
      {
         Flag |= SFRONT;
      }
      else
      {
         Flag |= SRIGHT;
      }
   }
   else if (pSide1 == m_Left)
   {
      if (pSide2 == m_Front)
      {
         Flag |= SLEFT;
      }
      else
      {
         Flag |= SBACK;
      }
   }
   return Flag;
}

//returns how many bottom corner pieces are oriented correctly
//and which ones they are
int CCubeSolver::GetDownCornerOrient(int &Flag)
{
   int Count = 0;
   Flag = 0;

   CSide *pSide1;
   CSide *pSide2;
   if ((pSide1 = FindCorner(*m_Down,*m_Front,*m_Right)) != m_Down)
   {
      Count++;
      if ((pSide2 = FindCorner(*m_Front,*m_Right,*m_Down)) == m_Down)
      {
         pSide2 = FindCorner(*m_Right,*m_Front,*m_Down);
      }
      Flag |= GetCornersSide(pSide1,pSide2);
   }
   if ((pSide1 = FindCorner(*m_Down,*m_Front,*m_Left)) != m_Down)
   {
      Count++;
      if ((pSide2 = FindCorner(*m_Front,*m_Left,*m_Down)) == m_Down)
      {
         pSide2 = FindCorner(*m_Left,*m_Front,*m_Down);
      }
      Flag |= GetCornersSide(pSide1,pSide2);
   }
   if ((pSide1 = FindCorner(*m_Down,*m_Back,*m_Right)) != m_Down)
   {
      Count++;
      if ((pSide2 = FindCorner(*m_Back,*m_Right,*m_Down)) == m_Down)
      {
         pSide2 = FindCorner(*m_Right,*m_Back,*m_Down);
      }
      Flag |= GetCornersSide(pSide1,pSide2);
   }
   if ((pSide1 = FindCorner(*m_Down,*m_Back,*m_Left)) != m_Down)
   {
      Count++;
      if ((pSide2 = FindCorner(*m_Back,*m_Left,*m_Down)) == m_Down)
      {
         pSide2 = FindCorner(*m_Left,*m_Back,*m_Down);
      }
      Flag |= GetCornersSide(pSide1,pSide2);
   }

   return Count;
}

//returns how many bottom corner pieces are positioned correctly
//and which ones they are
int CCubeSolver::GetDownCornerPos(int &Flag)
{
   int Count = 0;
   Flag = 0;

   CSide *pSide1;
   CSide *pSide2;
   if ((pSide1 = FindCorner(*m_Front,*m_Right,*m_Down)) != m_Front &&
       (pSide2 = FindCorner(*m_Right,*m_Front,*m_Down)) != m_Right)
   {
      Count++;
      if (pSide1 == m_Back)
      {
         if (pSide2 == m_Right)
         {
            Flag |= MRIGHTFRONT;
         }
         else if (pSide2 == m_Left)
         {
            Flag |= MBACKFRONT;
         }
      }
      else if (pSide1 == m_Left)
      {
         if (pSide2 == m_Back)
         {
            Flag |= MBACKFRONT;
         }
         else if (pSide2 == m_Front)
         {
            Flag |= MLEFTFRONT;
         }
      }
      else if (pSide1 == m_Right)
      {
         if (pSide2 == m_Back)
         {
            Flag |= MRIGHTFRONT;
         }
         else if (pSide2 == m_Front)
         {
            ASSERT(FALSE);
         }
      }
      else if (pSide2 == m_Left)
      {
         Flag |= MLEFTFRONT;
      }
   }
   if ((pSide1 = FindCorner(*m_Front,*m_Left,*m_Down)) != m_Front &&
       (pSide2 = FindCorner(*m_Left,*m_Front,*m_Down)) != m_Left)
   {
      Count++;
      if (pSide1 == m_Back)
      {
         if (pSide2 == m_Right)
         {
            Flag |= MRIGHTLEFT;
         }
         else if (pSide2 == m_Left)
         {
            Flag |= MBACKLEFT;
         }
      }
      else if (pSide1 == m_Right)
      {
         if (pSide2 == m_Front)
         {
            Flag |= MFRONTLEFT;
         }
         else if (pSide2 == m_Back)
         {
            Flag |= MRIGHTLEFT;
         }
      }
      else if (pSide1 == m_Left)
      {
         if (pSide2 == m_Back)
         {
            Flag |= MBACKLEFT;
         }
         else if (pSide2 == m_Front)
         {
            ASSERT(FALSE);
         }
      }
      else if (pSide2 == m_Right)
      {
         Flag |= MFRONTLEFT;
      }
   }
   if ((pSide1 = FindCorner(*m_Back,*m_Right,*m_Down)) != m_Back &&
       (pSide2 = FindCorner(*m_Right,*m_Back,*m_Down)) != m_Right)
   {
      Count++;
      if (pSide1 == m_Front)
      {
         if (pSide2 == m_Right)
         {
            Flag |= MFRONTRIGHT;
         }
         else if (pSide2 == m_Left)
         {
            Flag |= MLEFTRIGHT;
         
         }
      }
      else if (pSide1 == m_Left)
      {
         if (pSide2 == m_Front)
         {
            Flag |= MLEFTRIGHT;
         }
         else if (pSide2 == m_Back)
         {
            Flag |= MBACKRIGHT;
         }
      }
      else if (pSide1 == m_Right)
      {
         if (pSide2 == m_Front)
         {
            Flag |= MFRONTRIGHT;
         }
         else if (pSide2 == m_Back)
         {
            ASSERT(FALSE);
         }
      }
      else if (pSide2 == m_Left)
      {
         Flag |= MBACKRIGHT;
      }
   }
   if ((pSide1 = FindCorner(*m_Back,*m_Left,*m_Down)) != m_Down &&
       (pSide2 = FindCorner(*m_Left,*m_Back,*m_Down)) != m_Left)
   {
      Count++;
      if (pSide1 == m_Front)
      {
         if (pSide2 == m_Right)
         {
            Flag |= MFRONTBACK;
         }
         else if (pSide2 == m_Left)
         {
            Flag |= MLEFTBACK;
         
         }
      }
      else if (pSide1 == m_Right)
      {
         if (pSide2 == m_Front)
         {
            Flag |= MFRONTBACK;
         }
         else if (pSide2 == m_Back)
         {
            Flag |= MRIGHTBACK;
         }
      }
      else if (pSide1 == m_Back)
      {
         if (pSide2 == m_Right)
         {
            Flag |= MRIGHTBACK;
         }
         else if (pSide2 == m_Left)
         {
            ASSERT(FALSE);
         }
      }
      else if (pSide2 == m_Front)
      {
         Flag |= MLEFTBACK;
      }
   }

   return Count;
}

//solve the bottom corners
bool CCubeSolver::SolveDownCorners(CProgressCtrl *pProgress)
{
int Count;
int Flag;
int Iter = 0;

   //first orient the corner pieces
   while ((Count = GetDownCornerOrient(Flag)) > 0)
   {
      Iter++;

      pProgress->StepIt();
      if (Count > 3)
      {
         m_S += "B+.L+.B-.L-.D+.D+.R-.B+.B+.R+.B-.R-.B+.B+.R+.B-.D+.D+.";
         DoMove("B+.L+.B-.L-.D+.D+.R-.B+.B+.R+.B-.R-.B+.B+.R+.B-.D+.D+.");
      }
      else if (Count == 3)
      {
         if (Flag ==  (SBACK | SLEFT | SFRONT))
         {
            m_S += "B+.L+.B-.L-.D+.D+.R-.B+.B+.R+.B-.R-.B+.B+.R+.B-.D+.D+.";
            DoMove("B+.L+.B-.L-.D+.D+.R-.B+.B+.R+.B-.R-.B+.B+.R+.B-.D+.D+.");
         }
         else
         {
            m_S += "C+.";
            DoMove("C+.");
         }
      }
      else if (Count == 2)
      {
         if (Flag == (SRIGHT | SFRONT))
         {
            m_S += "D+.L+.D-.L-.D+.L+.D-.R-.D+.L-.D-.L+.D+.L-.D-.R+.";
            DoMove("D+.L+.D-.L-.D+.L+.D-.R-.D+.L-.D-.L+.D+.L-.D-.R+.");
                     
         }
         else if (Flag == (SRIGHT | SLEFT))
         {
            m_S += "B+.L-.T+.T+.L+.B-.D+.D+.B+.L-.T+.T+.L+.B-.D+.D+.";
            DoMove("B+.L-.T+.T+.L+.B-.D+.D+.B+.L-.T+.T+.L+.B-.D+.D+.");
         }
         else
         {
            m_S += "C+.";
            DoMove("C+.");
         }
      }
      if (Iter > 10)
      {
         return false;
      }
   }

   Iter = 0;
   //now position the corner pieces
   while ((Count = GetDownCornerPos(Flag)) > 0)
   {
      Iter++;
      pProgress->StepIt();
      if (Count == 4)
      {
         if ((Flag & MLEFTRIGHT) && (Flag & MFRONTBACK))
         {
            m_S += "L+.L+.R+.R+.T+.L+.L+.R+.R+.D+.D+.L+.L+.R+.R+.T+.L+.L+.R+.R+.D+.D+.";
            DoMove("L+.L+.R+.R+.T+.L+.L+.R+.R+.D+.D+.L+.L+.R+.R+.T+.L+.L+.R+.R+.D+.D+.");
         }
         else if ((Flag & MLEFTFRONT) && (Flag & MBACKRIGHT))

         {
             m_S += "F+.L-.F-.R+.F+.L+.F-.R+.R+.B-.L+.B+.R+.B-.L-.B+.";
             DoMove("F+.L-.F-.R+.F+.L+.F-.R+.R+.B-.L+.B+.R+.B-.L-.B+.");
         }
         else 
         {
            m_S += "C+.";
            DoMove("C+.");
         }
      }
      else if (Count == 3)
      {
         if ((Flag & MLEFTRIGHT) && (Flag & MRIGHTBACK) && (Flag & MBACKLEFT))
         {
            m_S += "L-.F+.L-.B+.B+.L+.F-.L-.B+.B+.L+.L+.";
            DoMove("L-.F+.L-.B+.B+.L+.F-.L-.B+.B+.L+.L+.");
         }
         else if ((Flag & MLEFTBACK) && (Flag & MBACKRIGHT) && (Flag & MRIGHTLEFT))
         {
            m_S += "L+.L+.B+.B+.L+.F+.L-.B+.B+.L+.F-.L+.";
            DoMove("L+.L+.B+.B+.L+.F+.L-.B+.B+.L+.F-.L+.");
         }
         else
         {
            m_S += "C+.";
            DoMove("C+.");
         }
      }
      else 
      {
         ASSERT(false);
      }
      if (Iter > 10)
      {
         return false;
      }
   }
   return true;
}

//solve the cube
bool CCubeSolver::SolveCube(CProgressCtrl *pProgress)
{
   bool Solved = true;
   SolveTopEdges(pProgress);
   SolveTopCorners(pProgress);
   SolveMiddleEdges(pProgress);
   Solved = SolveDownEdge(pProgress);
   if (Solved)
   {
      Solved = SolveDownCorners(pProgress);
   }
   if (Solved)
   {
      CleanSolution();
   }
   return Solved;
}

//find the shortest solution
int CCubeSolver::Solve(CProgressCtrl *pProgress)
{
CString s = ""; 
CString t = ""; 
CString p = "";

   //make sure that all the cubelets are valid
   for (int i = 1; i <= 6; i++) 
   {
      if (FindCenter(i) == 0) 
      {
      return 1;
      }
   }

   for (int i = 0; i < NUM_EDGES;i++)
   {
      if (!FindEdge(*GetSide(Edges[i*2].Side),*GetSide(Edges[(i*2)+1].Side)))
      {
         return 1;
      }
   }

   for (int i = 0; i < NUM_CORNERS;i++)
   {
      if (!FindCorner(*GetSide(Corners[(i*3)].Side),
                      *GetSide(Corners[(i*3)+1].Side),
                      *GetSide(Corners[(i*3)+2].Side)))
      {
         return 1;
      }
   }

   //clear the solution array
   m_Solutions.RemoveAll();

   //copy the cube so we have the orignial
   SaveCube();

   bool Solved = false;
   //try to find a solution from every starting position
   //and later find the shortest solution.
   for (int i = 0; i < 6;i++)
   {
      for (int j = 0; j < 4;j++)
      {
         switch (i)
         {
         case 0:  m_S = "";
                  break;
         case 1:  m_S = "C^.";
                  break;
         case 2:  m_S = "C^.C^.";
                  break;
         case 3:  m_S = "C^.C^.C^.";
                  break;
         case 4:  m_S = "C+.C^.";
                  break;
         case 5:  m_S = "C-.C^.";
                  break;
         }
         for (int k = 0; k < j;k++)
         {
            m_S += "C+.";
         }

         DoMove(m_S);
         Solved = SolveCube(pProgress);
         if (Solved)
         {
            m_Solutions.Add(m_S);
         }
         //restore the cube to original formation
         RestoreCube();
      }
   }

   //find the shortest solution
   int NumMoves = 500;
   int Index = -1;
   for (int i = 0; i < m_Solutions.GetCount();i++)
   {
      if (m_Solutions.GetAt(i).GetLength() < NumMoves)
      {
         NumMoves = m_Solutions.GetAt(i).GetLength();
         Index = i;
      }
   }
   m_S = "";
   if (Index != -1)
   {
      m_S = m_Solutions.GetAt(Index);
   }
   pProgress->SetPos(0);
   return Solved ? 0 : 2;
}

bool CCubeSolver::CleanOppositeMoves(CString &Moves)
{
   bool Found = false;
   CString S = "";
   long Start = 0;
   CString Move1 = Moves.Mid(Start,3);
   CString Move2;
   while (!Move1.IsEmpty())
   {
      Start += 3;
      Move2 = Moves.Mid(Start,3);
      if (!Move2.IsEmpty())
      {
         //if both moves are on the same side
         if (Move1[0] == Move2[0])
         {
            //if they are not the same direction
            if (Move1[1] != Move2[1])
            {
               Found = true;
               Start += 3;
               Move1 = Moves.Mid(Start,3);
               continue;
            }
         }
      }

      S += Move1;

      Move1 = Move2;
   }
   Moves = S;
   return Found;
}

void CCubeSolver::CleanSolution()
{
   CString S = "";
   //first get rid of the cube moves
   char F = 'F';
   char L = 'L';
   char R = 'R';
   char B = 'B';
   char T = 'T';
   char D = 'D';
   char Temp;
   CString Move;
   int Start = 0;
   Move = m_S.Mid(Start,3);
   while (!Move.IsEmpty())
   {
      switch (Move[0])
      {
      case 'C' :  switch (Move[1])
                  {
                  case '+' :  Temp = F;
                              F = L;
                              L = B;
                              B = R;
                              R = Temp;
                              break;
                  case '-' :  Temp = F;
                              F = R;
                              R = B;
                              B = L;
                              L = Temp;
                              break;
                  case '^' :  Temp = F;
                              F = D;
                              D = B;
                              B = T;
                              T = Temp;
                              break;
                  case 'v' :  Temp = F;
                              F = T;
                              T = B;
                              B = D;
                              D = Temp;
                              break;
                  }
                  break;
      case 'F' :  S += CString(F)+Move[1]+".";
                  break;
      case 'L' :  S += CString(L)+Move[1]+".";
                  break;
      case 'R' :  S += CString(R)+Move[1]+".";
                  break;
      case 'B' :  S += CString(B)+Move[1]+".";
                  break;
      case 'T' :  S += CString(T)+Move[1]+".";
                  break;
      case 'D' :  S += CString(D)+Move[1]+".";
                  break;
      default  :  S += Move;
      }

      Start += 3;
      Move = m_S.Mid(Start,3);
   }

   m_S = S;

   //then get rid of condredicting moves (example L+.L-.)
   while (CleanOppositeMoves(m_S));

   //Turn long 270 to Counter Clockwise move (L+.L+.L+. = L-.)
   S = "";
   Start = 0;
   CString Move1 = m_S.Mid(Start,3);
   Start += 3;
   CString Move2 = m_S.Mid(Start,3);
   CString Move3;
   while (!Move1.IsEmpty())
   {
      Start += 3;
      Move3 = m_S.Mid(Start,3);
      if (!Move2.IsEmpty() && !Move3.IsEmpty())
      {
         if ((Move1 == Move2) && (Move2 == Move3))
         {
            S += CString(Move1[0]);
            if (Move1[1] == '+')
            {
               S += "-.";
            }
            else
            {
               S += "+.";
            }
            Start += 3;
            Move1 = m_S.Mid(Start,3);
            Start += 3;
            Move2 = m_S.Mid(Start,3);
            continue;
         }
      }

      S += Move1;

      Move1 = Move2;
      Move2 = Move3;
   }

   m_S = S;

   //and circular moves (L+.L+.L+.L+.)
   S = "";
   Start = 0;
   Move1 = m_S.Mid(Start,3);
   Start += 3;
   Move2 = m_S.Mid(Start,3);
   Start += 3;
   Move3 = m_S.Mid(Start,3);
   CString Move4;
   while (!Move1.IsEmpty())
   {
      Start += 3;
      Move4 = m_S.Mid(Start,3);
      if (!Move2.IsEmpty() && !Move3.IsEmpty() && !Move4.IsEmpty())
      {
         if ((Move1 == Move2) && (Move2 == Move3) && (Move3 == Move4))
         {
            Start += 3;
            Move1 = m_S.Mid(Start,3);
            Start += 3;
            Move2 = m_S.Mid(Start,3);
            Start += 3;
            Move3 = m_S.Mid(Start,3);
            continue;
         }
      }

      S += Move1;

      Move1 = Move2;
      Move2 = Move3;
      Move3 = Move4;
   }

   m_S = S;

   //and Two same turns into 180 (L+.L+. = L*. L-.L-. = L*.)
   S = "";
   Start = 0;
   Move1 = m_S.Mid(Start,3);
   while (!Move1.IsEmpty())
   {
      Start += 3;
      Move2 = m_S.Mid(Start,3);
      if (!Move2.IsEmpty())
      {
         //if both moves are on the same side
         if (Move1 == Move2)
         {
            S += CString(Move1[0]) + "*.";
            Start += 3;
            Move1 = m_S.Mid(Start,3);
            continue;
         }
      }

      S += Move1;

      Move1 = Move2;
   }
   m_S = S;

}

bool CCubeSolver::DoMove(CString Moves)
{
   CString Move;
   int Start = 0;
   Move = Moves.Mid(Start,3);
   while (!Move.IsEmpty())
   {
      CSide *pSide = NULL;
      switch (Move[0])
      {
      case 'C' :  switch(Move[1])
                  {
                  case '+' :  RotateCubeRight();
                              break;
                  case '-' :  RotateCubeLeft();
                              break;
                  case '^' :  RotateCubeUp();
                              break;
                  case 'v' :  RotateCubeDown();
                              break;
                  }
                  break;
      case 'F' :  pSide = m_Front;
                  break;
      case 'R' :  pSide = m_Right;
                  break;
      case 'B' :  pSide = m_Back;
                  break;
      case 'L' :  pSide = m_Left;
                  break;
      case 'T' :  pSide = m_Top;
                  break;
      case 'D' :  pSide = m_Down;
                  break;

      }
      if (pSide)
      {
         switch(Move[1])
         {
         case '+' :  RotateCW(pSide);
                     break;
         case '-' :  RotateCCW(pSide);
                     break;
         case '*' :  RotateCW(pSide);
                     RotateCW(pSide);
                     break;
         }
      }
      UpdateDisplay();
      YieldToOthers();
      
      Start += 3;
      Move = Moves.Mid(Start,3);
   }
   return true;
}

bool CCubeSolver::UndoMove(CString Move)
{
   if (Move.GetLength() < 3)
   {
      return false;
   }
   CSide *pSide = NULL;

   switch (Move[0])
   {
   case 'C' :  switch(Move[1])
               {
               case '+' :  RotateCubeLeft();
                           break;
               case '-' :  RotateCubeRight();
                           break;
               case '^' :  RotateCubeDown();
                           break;
               case 'v' :  RotateCubeUp();
                           break;
               }
               break;
   case 'F' :  pSide = m_Front;
               break;
   case 'R' :  pSide = m_Right;
               break;
   case 'B' :  pSide = m_Back;
               break;
   case 'L' :  pSide = m_Left;
               break;
   case 'T' :  pSide = m_Top;
               break;
   case 'D' :  pSide = m_Down;
               break;

   }
   if (pSide)
   {
      switch(Move[1])
      {
      case '+' :  RotateCCW(pSide);
                  break;
      case '-' :  RotateCW(pSide);
                  break;
      case '*' :  RotateCW(pSide);
                  RotateCW(pSide);
                  break;
      }
   }
   UpdateDisplay();
   YieldToOthers();
   return true;
}
