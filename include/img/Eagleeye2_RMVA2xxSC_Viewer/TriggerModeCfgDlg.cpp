// TriggerModeCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "TriggerModeCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerModeCfgDlg dialog


CTriggerModeCfgDlg::CTriggerModeCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerModeCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTriggerModeCfgDlg)
	m_uiTrgShutterTime = 2000;
	m_iECol = 1416;
	m_iERow = 1032;
	m_fInterval = 5.0f;
	m_strLocation = _T("上海市杨浦区大学路智星路口由东向西");
	m_fPGALimit = 20.0f;
	m_iSCol = 200;
	m_iShutLimit = 2000;
	m_iSRow = 200;
	m_fTrgPga = 10.0f;
	m_iWayIndex = 1;
	m_byYMean = 90;
	m_iADLIP = 1;
	m_iCurShut = 2000;
	m_bIsSingalCoin = FALSE;
	m_bIsManualCtrl = FALSE;
	m_bEnableADLI = TRUE;
	m_bSaveParam = FALSE;
	m_fCurPga = 10.0f;
	//}}AFX_DATA_INIT
}

void CTriggerModeCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriggerModeCfgDlg)
	DDX_Control(pDX, IDC_ENABLEADLI, m_ctrlEnableADIL);
	DDX_Control(pDX, IDC_SINGALCOIN, m_ctrlSingalCoin);
	DDX_Control(pDX, IDC_SHUTPGAMANUALCTRL, m_ctrlShutPgaManualCtrl);
	DDX_Control(pDX, IDC_SHUTPGAAUTOCTRL, m_ctrlShutPgaAutoCtrl);
	DDX_Control(pDX, IDC_DOUBLECOIN, m_ctrlDoubleCoin);
	DDX_Text(pDX, IDC_TRGSHUTTERTIME, m_uiTrgShutterTime);
	DDV_MinMaxUInt(pDX, m_uiTrgShutterTime, 1, 120000);
	DDX_Text(pDX, IDC_ECOL, m_iECol);
	DDV_MinMaxInt(pDX, m_iECol, 1, 1616);
	DDX_Text(pDX, IDC_EROW, m_iERow);
	DDV_MinMaxInt(pDX, m_iERow, 1, 1232);
	DDX_Text(pDX, IDC_INTERVAL, m_fInterval);
	DDX_Text(pDX, IDC_LOCATION, m_strLocation);
	DDV_MaxChars(pDX, m_strLocation, 101);
	DDX_Text(pDX, IDC_PGALIMIT, m_fPGALimit);
	DDV_MinMaxFloat(pDX, m_fPGALimit, 0.f, 22.f);
	DDX_Text(pDX, IDC_SCOL, m_iSCol);
	DDV_MinMaxInt(pDX, m_iSCol, 1, 1616);
	DDX_Text(pDX, IDC_SHUTLIMIT, m_iShutLimit);
	DDV_MinMaxInt(pDX, m_iShutLimit, 1, 120000);
	DDX_Text(pDX, IDC_SROW, m_iSRow);
	DDV_MinMaxInt(pDX, m_iSRow, 1, 1232);
	DDX_Text(pDX, IDC_TRGPGA, m_fTrgPga);
	DDV_MinMaxFloat(pDX, m_fTrgPga, 0.f, 22.f);
	DDX_Text(pDX, IDC_WAYNUM, m_iWayIndex);
	DDX_Text(pDX, IDC_EYMEAN, m_byYMean);
	DDV_MinMaxByte(pDX, m_byYMean, 0, 255);
	DDX_Text(pDX, IDC_ADLIP, m_iADLIP);
	DDV_MinMaxInt(pDX, m_iADLIP, 1, 1800);
	DDX_Text(pDX, IDC_CURTRGSHUTTERTIME, m_iCurShut);
	DDV_MinMaxInt(pDX, m_iCurShut, 1, 120000);
	DDX_Text(pDX, IDC_CURTRGPGA, m_fCurPga);
	DDV_MinMaxFloat(pDX, m_fCurPga, 0.f, 22.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTriggerModeCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CTriggerModeCfgDlg)
	ON_BN_CLICKED(IDC_DOUBLECOIN, OnDoublecoin)
	ON_BN_CLICKED(IDC_SINGALCOIN, OnSingalcoin)
	ON_BN_CLICKED(IDC_SHUTPGAMANUALCTRL, OnShutpgamanualctrl)
	ON_BN_CLICKED(IDC_SHUTPGAAUTOCTRL, OnShutpgaautoctrl)
	ON_BN_CLICKED(IDC_ENABLEADLI, OnEnableadli)
	ON_BN_CLICKED(IDSAVEPARAM, OnSaveparam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerModeCfgDlg message handlers

BOOL CTriggerModeCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if( m_bIsSingalCoin ){
		m_ctrlSingalCoin.SetCheck(TRUE);
		m_ctrlDoubleCoin.SetCheck(FALSE);
		GetDlgItem(IDC_INTERVAL)->EnableWindow(FALSE);
	}
	else{
		m_ctrlSingalCoin.SetCheck(FALSE);
		m_ctrlDoubleCoin.SetCheck(TRUE);
		GetDlgItem(IDC_INTERVAL)->EnableWindow(TRUE);
	}
	if( m_bIsManualCtrl ){
		m_ctrlShutPgaManualCtrl .SetCheck(TRUE);
		m_ctrlShutPgaAutoCtrl.SetCheck(FALSE);
		if(m_bEnableADLI){
			m_ctrlEnableADIL.SetCheck(TRUE);
		}
		else{
			m_ctrlEnableADIL.SetCheck(FALSE);
		}
		GetDlgItem(IDC_TRGSHUTTERTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_TRGPGA)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHUTLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PGALIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SROW)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCOL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EROW)->EnableWindow(FALSE);
		GetDlgItem(IDC_ECOL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EYMEAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADLIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENABLEADLI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CURTRGSHUTTERTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_CURTRGPGA)->EnableWindow(FALSE);
	}
	else{
		m_ctrlShutPgaManualCtrl .SetCheck(FALSE);
		m_ctrlShutPgaAutoCtrl.SetCheck(TRUE);
		if(m_bEnableADLI){
			m_ctrlEnableADIL.SetCheck(TRUE);
			GetDlgItem(IDC_ADLIP)->EnableWindow(TRUE);
		}
		else{
			m_ctrlEnableADIL.SetCheck(FALSE);
			GetDlgItem(IDC_ADLIP)->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_TRGSHUTTERTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_TRGPGA)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHUTLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PGALIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SROW)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EROW)->EnableWindow(TRUE);
		GetDlgItem(IDC_ECOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EYMEAN)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENABLEADLI)->EnableWindow(TRUE);
		GetDlgItem(IDC_CURTRGSHUTTERTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CURTRGPGA)->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTriggerModeCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_iCurShut>m_iShutLimit){
		AfxMessageBox("当前快门值超出上限");
		return;
	}
	if(m_fCurPga>m_fPGALimit){
		AfxMessageBox("当前增益值超出上限");
		return;
	}
	m_bSaveParam = FALSE;
	CDialog::OnOK();
}

void CTriggerModeCfgDlg::OnDoublecoin() 
{
	// TODO: Add your control notification handler code here
	m_bIsSingalCoin = FALSE;
	m_ctrlSingalCoin.SetCheck(FALSE);
	m_ctrlDoubleCoin.SetCheck(TRUE);
	GetDlgItem(IDC_INTERVAL)->EnableWindow(TRUE);
}

void CTriggerModeCfgDlg::OnSingalcoin() 
{
	// TODO: Add your control notification handler code here
	m_bIsSingalCoin = TRUE;
	m_ctrlSingalCoin.SetCheck(TRUE);
	m_ctrlDoubleCoin.SetCheck(FALSE);
	GetDlgItem(IDC_INTERVAL)->EnableWindow(FALSE);
}

void CTriggerModeCfgDlg::OnShutpgamanualctrl() 
{
	// TODO: Add your control notification handler code here
	m_bIsManualCtrl = TRUE;
	m_ctrlShutPgaManualCtrl .SetCheck(TRUE);
	m_ctrlShutPgaAutoCtrl.SetCheck(FALSE);
	if(m_bEnableADLI){
		m_ctrlEnableADIL.SetCheck(TRUE);
	}
	else{
		m_ctrlEnableADIL.SetCheck(FALSE);
	}
	GetDlgItem(IDC_TRGSHUTTERTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_TRGPGA)->EnableWindow(TRUE);
	GetDlgItem(IDC_SHUTLIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PGALIMIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SROW)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EROW)->EnableWindow(FALSE);
	GetDlgItem(IDC_ECOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EYMEAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADLIP)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENABLEADLI)->EnableWindow(FALSE);
	GetDlgItem(IDC_CURTRGSHUTTERTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CURTRGPGA)->EnableWindow(FALSE);
}

void CTriggerModeCfgDlg::OnShutpgaautoctrl() 
{
	// TODO: Add your control notification handler code here
	m_bIsManualCtrl = FALSE;
	m_ctrlShutPgaManualCtrl .SetCheck(FALSE);
	m_ctrlShutPgaAutoCtrl.SetCheck(TRUE);
	if(m_bEnableADLI){
		m_ctrlEnableADIL.SetCheck(TRUE);
		GetDlgItem(IDC_ADLIP)->EnableWindow(TRUE);
	}
	else{
		m_ctrlEnableADIL.SetCheck(FALSE);
		GetDlgItem(IDC_ADLIP)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_TRGSHUTTERTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_TRGPGA)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHUTLIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PGALIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SROW)->EnableWindow(TRUE);
	GetDlgItem(IDC_SCOL)->EnableWindow(TRUE);
	GetDlgItem(IDC_EROW)->EnableWindow(TRUE);
	GetDlgItem(IDC_ECOL)->EnableWindow(TRUE);
	GetDlgItem(IDC_EYMEAN)->EnableWindow(TRUE);
	GetDlgItem(IDC_ENABLEADLI)->EnableWindow(TRUE);
	GetDlgItem(IDC_CURTRGSHUTTERTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_CURTRGPGA)->EnableWindow(TRUE);
}

void CTriggerModeCfgDlg::OnEnableadli() 
{
	// TODO: Add your control notification handler code here
	if(m_bEnableADLI){
		m_bEnableADLI = FALSE;
		GetDlgItem(IDC_ADLIP)->EnableWindow(FALSE);
	}
	else{
		m_bEnableADLI = TRUE;
		GetDlgItem(IDC_ADLIP)->EnableWindow(TRUE);
	}
}

void CTriggerModeCfgDlg::OnSaveparam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_iCurShut>m_iShutLimit){
		AfxMessageBox("当前快门值超出上限");
		return;
	}
	if(m_fCurPga>m_fPGALimit){
		AfxMessageBox("当前增益值超出上限");
		return;
	}
	m_bSaveParam = TRUE;
	CDialog::OnOK();
}
