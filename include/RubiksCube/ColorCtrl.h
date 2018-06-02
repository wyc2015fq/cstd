#pragma once


// CColorCtrl

class CColorCtrl : public CStatic
{
	DECLARE_DYNAMIC(CColorCtrl)

public:
	CColorCtrl(COLORREF Color,char DragColor);
	virtual ~CColorCtrl();
   char GetColor() { return m_DragColor; }
   void SetSelect(bool Selected);

protected:
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnEnable(BOOL bEnable);
   afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
   COLORREF m_Color;
   char     m_DragColor;
   bool     m_Selected;
};


