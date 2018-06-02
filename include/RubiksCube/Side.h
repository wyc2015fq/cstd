#pragma once

struct CSquare
{
   char Side;  //F,T,D,R,L,B
   int  Row;
   int  Col;    
};

struct CSide
{
   char     m_Side;           //used for 3d mode
   char     m_Blocks[3][3];
   CRect    m_Rect;           //used for 2d mode
   CSquare  *m_Moves;         //moves that need to be performed
                              //to turn this side.

   CSide(char Color,char Side,CSquare *Moves);
   char Color();
   void SetColor(char Color);
   void Clear();
   void operator =(const CSide &Other);
   operator char();
};
