#if !defined(AFX_DIAGINP_H__91BA4FF4_12FE_4E27_A4B3_0B3005365A4D__INCLUDED_)
#define AFX_DIAGINP_H__91BA4FF4_12FE_4E27_A4B3_0B3005365A4D__INCLUDED_

#include "Cubes.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagInp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagInp dialog

struct CDiagInp : public CDialog {

  // Construction
public:
  CDiagInp(CWnd* pParent = NULL);

  // Dialog Data
  //{{AFX_DATA(CDiagInp)
  enum { IDD = IDD_DIAG_INP };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  void ColorUpdate(int noSide);

  bool CubesUpdate(CCubes& cubes);

  // Overrides
  // ClassWizard generated function overrides
  //{{AFX_VIRTUAL(CDiagInp)
protected:
  BOOL OnInitDialog();
  int OnToolHitTest(IPOINT point, TOOLINFO* pTI) const;
  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  CWnd* m_pWnd;              // parent
  DWORD m_centers;           // rotations of facets, center of face
  int m_face;                // current face to update the rotation

  IRECT facetsRect[54];      // rectangles corresponding to the key facets
  BYTE m_colors[54];         // facets colors to update
  t_Matrix m_orient;         // orientation of the Rubik's Cube (center cubie)

  int m_btnId;               // id of the color button pressed (or 0)
  int m_sideId;              // number of the selected facet (or -1)
  bool m_autoCompleteEnable;

  void CreateTables();
  void CubeColorsInit(const CCubes& cubes);
  void ColorsErase();
  void FCColorsErase();
  void DelColor(BYTE noSide);
  void DelColors(BYTE noCube, bool dif);
  void DelColors(t_GROUP sides);
  void DelColorsType(BYTE c, bool dif);

  bool IsValid(t_COLORS cvrt) const {
    return (GetNoCube(cvrt) < 27);
  }
  bool IsUsed(t_COLORS cvrt) const;
  bool IsAvailable(t_COLORS cvrt, BYTE nbc) const;
  bool IsComplete() const;
  bool IsColorFullSide(BYTE noSide) const;
  bool IsValidCornerColor(BYTE noSide, BYTE color) const;

  BYTE SearchAvailableColors(BYTE nbc) const;

  char ColorCount(t_COLORS cvrt, BYTE nbc) const;
  void FullSideColor(BYTE noSide, bool del);

  void AutoCompletion();
  bool AutoCompletion(BYTE nbc);
  BYTE SearchColor(t_COLORS cvrt, BYTE noSide, BYTE nbc) const;
  bool SetCubeOrientation();

  // Generated message map functions
  //{{AFX_MSG(CDiagInp)
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, IPOINT point);
  afx_msg void OnLButtonUp(UINT nFlags, IPOINT point);
  afx_msg void OnColerase();
  void OnOK();
  afx_msg void OnCinit();
  afx_msg void OnColcopy();
  afx_msg void OnContextMenu(CWnd* pWnd, IPOINT point);
  //}}AFX_MSG
  afx_msg void OnOrientFace(UINT id);
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGINP_H__91BA4FF4_12FE_4E27_A4B3_0B3005365A4D__INCLUDED_)
