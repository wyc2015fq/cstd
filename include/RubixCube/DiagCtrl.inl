// DiagCtrl.cpp : implementation file
//

#include "RubixCube.h"
#include "DiagCtrl.h"
#include "ChildView.h"

//
// CDiagCtrl dialog

CDiagCtrl::CDiagCtrl(CWnd* pParent /*=NULL*/)
  : CDialog(CDiagCtrl::IDD, pParent)
{
  m_pParent = pParent;
  //{{AFX_DATA_INIT(CDiagCtrl)
  m_plans = 1;
  m_axes =  1;
  //}}AFX_DATA_INIT
}

void CDiagCtrl::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDiagCtrl)
  DDX_Radio(pDX, IDC_RADIO_P1, m_plans);
  DDX_Radio(pDX, IDC_RADIO_X, m_axes);
  //}}AFX_DATA_MAP
}

// CDiagCtrl message handlers

void CDiagCtrl::OnButtonMv(UINT id)
{
  UpdateData();
  t_Mask cmd = Command(m_axes, m_plans, (id == IDC_BUTTON_NEG));
  ((CChildView*)m_pParent)->CubeRotate(cmd);
}
//
