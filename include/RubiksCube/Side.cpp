#include "stdafx.h"
#include "Side.h"

// CSide structure constuctor
CSide::CSide(char Color,char Side,CSquare *Moves)
{
   m_Side = Side;
   m_Moves = Moves;
   SetColor(Color);
}

//get the color of the side.  This is the color of the 
//center cell
char CSide::Color()
{
   return m_Blocks[1][1];
}

//set the side to a color
//this function will set all the cells to the same color
void CSide::SetColor(char Color)
{
   for (int i = 0;i < 3;i++)
   {
      for (int j = 0;j < 3;j++)
      {
         m_Blocks[i][j] = Color;
      }
   }
   m_Rect.SetRectEmpty();
}

//clear the side except for the center cell
void CSide::Clear()
{
   for (int i = 0;i < 3;i++)
   {
      for (int j = 0;j < 3;j++)
      {
         if (!(i == 1 && j == 1))
         {
            m_Blocks[i][j] = 0;
         }
      }
   }
}

void CSide::operator = (const CSide &Other)
{
   for (int i = 0;i < 3;i++)
   {
      for (int j = 0;j < 3;j++)
      {
         m_Blocks[i][j] = Other.m_Blocks[i][j];
      }
   }
}

CSide::operator char()
{
   return Color();
}

