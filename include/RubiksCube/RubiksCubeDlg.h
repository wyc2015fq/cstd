// RubiksCubeDlg.h : header file
//

#pragma once

#include "CubeWnd.h"
#include "colorctrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "EasySize.h"


// CRubiksCubeDlg dialog
class CRubiksCubeDlg : public CDialog
{
// Construction
public:
	CRubiksCubeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RUBIKSCUBE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
   virtual void OnOK();
   virtual void OnCancel();
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   afx_msg void OnBnClickedSolve();
   afx_msg void OnBnClickedExit();
   afx_msg void OnBnClickedClear();
   afx_msg void OnBnClickedAbout();
   afx_msg void OnBnClickedShuffle();
   afx_msg void OnBnClickedReset();
   afx_msg void OnBnClickedPrev();
   afx_msg void OnBnClickedNext();
   afx_msg void OnLbnSelchangeMovelist();
   afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnBnClickedSpeedtitle();
   afx_msg LRESULT OnColorSelected(WPARAM wParam,LPARAM lParam);
   afx_msg void OnBnClicked3d();
	DECLARE_MESSAGE_MAP()

	DECLARE_EASYSIZE

protected:
   CButton        m_3DCheck;
   CButton        m_Animate;
   CListBox       m_MoveList;
   CStatic        m_MoveTitle;
   CSliderCtrl    m_Speed;
   CColorCtrl     m_Red;
   CColorCtrl     m_Blue;
   CColorCtrl     m_Green;
   CColorCtrl     m_White;
   CColorCtrl     m_Yellow;
   CColorCtrl     m_Orange;
   CProgressCtrl  m_Progress;
   CStringArray   m_MoveArray;

   CCubeWnd       m_Cube;
   int            m_CurIndex;

   void SpeedChange();
   void ClearMoves();
   void DoMove(int Index,bool Undo = false);
   void Enable(BOOL Enable);
   void EnableNextPrev();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
