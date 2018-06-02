// DevStatisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "DevStatisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DevStatisDlg dialog


DevStatisDlg::DevStatisDlg(const char* ip, CWnd* pParent /*=NULL*/)
	: CDialog(DevStatisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DevStatisDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_IP, 0x0, sizeof(m_IP));
	memcpy(m_IP, ip, sizeof(m_IP));
}

void DevStatisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DevStatisDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DevStatisDlg, CDialog)
	//{{AFX_MSG_MAP(DevStatisDlg)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DevStatisDlg message handlers

BOOL DevStatisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization her
	GetDlgItem(IDC_EDIT_TOTAL_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_NO_FEAT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REC_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TOTAL_REG)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_TOTAL_ALARM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TOTAL_LOG)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TOTAL_IMAGE)->EnableWindow(FALSE);


	CPM_RegOperResultNotifyCB( &DevStatisDlg::DevOperResult, this );
	CPM_DLDevStatisInfo( m_IP );
	this->SetWindowText( _T("正在下载信息.......") );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void DevStatisDlg::DevOperResult( const char* server_addr,  const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num, void* param  )
BOOL DevStatisDlg::DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext )
{
#if 0
	if ( oper_type == CMD_GET_DEV_STATISINFO )
	{
		DevStatisDlg* pthis = (DevStatisDlg*)param;
		if ( OPER_SUCCESS==eRt && oper_content )
		{
			pthis->DevStatisInfoNotify(dev_addr, *((DEV_STATIS*)oper_content));
		}
	}
#endif

    if ( eType == DEV_STATISINFO_OPER )
    {
        DevStatisDlg* pthis = (DevStatisDlg*)pvContext;
        if ( OPER_SUCCESS == eResult && pvContent )
        {
            pthis->DevStatisInfoNotify(cszDevAddr, *((DEV_STATIS*)pvContent));
        }
    }
	return TRUE;
}

void DevStatisDlg::DevStatisInfoNotify( const char* dev_addr, const DEV_STATIS& dev_statis )
{	

	char values[5] = {0};
	sprintf( values, _T("%d"), dev_statis.m_TotalUsers );
	GetDlgItem(IDC_EDIT_TOTAL_USER)->SetWindowText( values );
	sprintf( values, _T("%d"), dev_statis.m_NoFeatUser );
	GetDlgItem(IDC_EDIT_NO_FEAT)->SetWindowText(values);
	sprintf( values, _T("%d"), dev_statis.m_TotalDays );
	GetDlgItem(IDC_EDIT_REC_DATE)->SetWindowText(values);
	sprintf( values, _T("%d"), dev_statis.m_TotalRecords );
	GetDlgItem(IDC_EDIT_TOTAL_REG)->SetWindowText(values);
	sprintf( values, _T("%d"), dev_statis.m_TotalAlarm );
	GetDlgItem(IDC_EDIT_TOTAL_ALARM)->SetWindowText(values);	
	sprintf( values, _T("%d"), dev_statis.m_TotalDspLog );
	GetDlgItem(IDC_EDIT_TOTAL_LOG)->SetWindowText(values);
	sprintf( values, _T("%d"), dev_statis.m_TotalChangeImage );
	GetDlgItem(IDC_EDIT_TOTAL_IMAGE)->SetWindowText(values);

//	UpdateData(FALSE);

	::PostMessage(this->GetSafeHwnd(), WM_UPDATE, 0, 0);
	this->SetWindowText( _T("信息下载完成") );	
}

void DevStatisDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CPM_RegOperResultNotifyCB( NULL, NULL);
	CDialog::OnClose();
}

LRESULT DevStatisDlg::OnUpdate(WPARAM wparam, LPARAM lpparam)
{
	UpdateData(lpparam);
	return 0;
}