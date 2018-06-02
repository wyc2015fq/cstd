#pragma once

#include "3DCube.h"
#include "Side.h"

class CCubeSolver
{
public:
	CCubeSolver();
	virtual ~CCubeSolver();
   
   void Clear();
   virtual void Reset();
   void Shuffle(CProgressCtrl *pProgress);
   int Solve(CProgressCtrl *pProgress);
   CString GetMoves() { return m_S; }

   bool DoMove(CString Moves);
   bool UndoMove(CString Moves);

   virtual void SaveCube();
   virtual void RestoreCube();

   virtual void RotateCubeLeft();
   virtual void RotateCubeRight();

protected:
   CSide             *m_Top;
   CSide             *m_Front;
   CSide             *m_Right;
   CSide             *m_Left;
   CSide             *m_Back;
   CSide             *m_Down;

   virtual void UpdateDisplay(bool Force = false) {}
   virtual void YieldToOthers() {}

   virtual void RotateCW(CSide *pSide);
   virtual void RotateCCW(CSide *pSide);
private:
   int               m_FacingSide;
   CString           m_S;
   CStringArray      m_Solutions;
   
   CSide             *m_OTop;
   CSide             *m_OFront;
   CSide             *m_ORight;
   CSide             *m_OLeft;
   CSide             *m_OBack;
   CSide             *m_ODown;

   void SaveSide(CSide *pSide,CSide *pSaveTo);
   bool SolveCube(CProgressCtrl *pProgress);
   CSide *GetSide(char Side);
   void RotateFaceCW(CSide *pSide);
   void RotateFaceCCW(CSide *pSide);
   void RotateCubeCW();
   void RotateCubeCCW();
   void RotateCubeUp();
   void RotateCubeDown();
   CSide *FindCenter(char Color);
   CSide *FindEdge(char Color, char Color2);
   CSide *FindCorner(char Color, char Color2, char Color3);


   void SolveTopEdges(CProgressCtrl *pProgress);
   void SolveTopCorners(CProgressCtrl *pProgress);
   void SolveMiddleEdges(CProgressCtrl *pProgress);
   int GetDownEdgeOrient(int &Flag);
   int GetDownEdgePos(int &Flag);
   bool SolveDownEdge(CProgressCtrl *pProgress);
   int GetCornersSide(CSide *pSide1,CSide *pSide2);
   int GetDownCornerOrient(int &Flag);
   int GetDownCornerPos(int &Flag);
   bool SolveDownCorners(CProgressCtrl *pProgress);
   void CleanSolution();
   bool CleanOppositeMoves(CString &Moves);
};

