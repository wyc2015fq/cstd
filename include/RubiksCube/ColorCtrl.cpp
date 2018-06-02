// ColorCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "ColorCtrl.h"

// CColorCtrl

IMPLEMENT_DYNAMIC(CColorCtrl, CStatic)
CColorCtrl::CColorCtrl(COLORREF Color,char DragColor)
: m_Color(Color)
, m_DragColor(DragColor)
, m_Selected(false)
{
}

CColorCtrl::~CColorCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorCtrl, CStatic)
   ON_WM_PAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_ENABLE()
END_MESSAGE_MAP()



// CColorCtrl message handlers

//paint a square on the screen with the set fill color
void CColorCtrl::OnPaint()
{
   CPaintDC dc(this); // device context for painting
   CRect Rect;
   GetClientRect(&Rect);
   dc.FillSolidRect(&Rect,m_Color);
   if (m_Selected)
   {
      CPen Pen(PS_SOLID,3,RGB(0,0,0));
      CPen *pOldPen = dc.SelectObject(&Pen);
      HGDIOBJ OldBrush = dc.SelectObject(GetStockObject(HOLLOW_BRUSH));
      dc.Rectangle(&Rect);
      dc.SelectObject(pOldPen);
      dc.SelectObject(OldBrush);
   }
}

//select or unselect this color control
void CColorCtrl::SetSelect(bool Selected)
{
   m_Selected = Selected;
   Invalidate();
   UpdateWindow();
}

//called when the left mouse button is pressed within this control
void CColorCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
   //set the selection to true
   SetSelect(true);

   //tell the parent window that this color has been selected
   GetParent()->SendMessage(WM_COLORSELECTED,NULL,(LPARAM)this);
   
   CStatic::OnLButtonDown(nFlags, point);
}

//called by the framework when the enable/disable state of this contorl
//has been changed
void CColorCtrl::OnEnable(BOOL bEnable)
{
   CStatic::OnEnable(bEnable);
   Invalidate();
   UpdateWindow();
}
