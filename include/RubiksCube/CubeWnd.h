#pragma once

#include "CubeSolver.h"

// CCubeWnd
class CCubeWnd : public CWnd, public CCubeSolver
{
	DECLARE_DYNAMIC(CCubeWnd)

public:
	CCubeWnd();
	virtual ~CCubeWnd();

   BOOL IsPointGoodForDrop(CPoint Point);
   //mutator functions
   void SetColor(char Color) {m_SelectedColor = Color;}
   void SetFlat(bool Flat);
   void SetSpeed(DWORD Milliseconds) { m_Speed = Milliseconds; }
   bool IsFlat() { return m_Flat; }
   //assign color ot square on cube
   void SetSquareUnderPoint(CPoint Point,char Color);
   void Animate(bool Animate) { m_Animate = Animate; }
   //opengl functions
   int InitGL();

   virtual void Reset();

protected:
   virtual void PreSubclassWindow();

   afx_msg void OnEnable(BOOL bEnable);
   afx_msg void OnPaint();
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

   virtual void UpdateDisplay(bool Force = false);
   virtual void YieldToOthers();

   virtual void RotateCW(CSide *pSide);
   virtual void RotateCCW(CSide *pSide);

   virtual void SaveCube();
   virtual void RestoreCube();
private:
   bool              m_Animate;
   char              m_SelectedColor;  //currently selected color
                                       //this is the color that is going to be assigned
                                       //to cube on lbuttondown
   CPoint            m_LastPoint;   //used for the rotation of 3d Cube
   float             m_RotateX;     //rotation of 3d Cube
   float             m_RotateY;
   HDC               m_hDC;         //hDC is of this window
   HGLRC		         m_hRC;         //hRC for 3d cube
   C3DCube           m_3DCube;      //3d cube object
   bool              m_Flat;        //flat or 3d
   COLORREF          m_Colors[7];
   CSide             *m_SelectedSide;  //currently selected side
   DWORD             m_Speed;       //animation speed
	int					m_CubeWidth;
   //drawing flat cube;
   void DrawFlatCube(CDC *pDC);
   void DrawFlatSide(CDC *pDC,CSide *pSide,CPoint Start);
   
   //get the side under the point
   CSide *GetSideUnderPoint(CPoint Point);

   //opengl functions
   void ReSizeGLScene( int width, int height);
   BOOL SetPixelformat(HDC hdc);

   void Rotate3DCW(CSide *pSide);
   void Rotate3DCCW(CSide *pSide);

};


