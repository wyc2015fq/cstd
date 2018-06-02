// DevSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "DevSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DevSetDlg dialog


DevSetDlg::DevSetDlg(CHCADOConnection& ado, const char* ip, CWnd* pParent /*=NULL*/)
	: m_ado(ado),CDialog(DevSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DevSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_IP, 0x0, sizeof(m_IP));
	memcpy(m_IP, ip, sizeof(m_IP));
}


void DevSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DevSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_COMBO_VALIDE, m_cmbValide);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DevSetDlg, CDialog)
	//{{AFX_MSG_MAP(DevSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_CHECK_OPEN_A, OnBNValide)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DevSetDlg message handlers

BOOL DevSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cmbValide.AddString( _T("1:1") );
	m_cmbValide.AddString( _T("Mixed") );
	m_cmbValide.AddString( _T("1:N") );
	
	m_cmbValide.SetCurSel(2);
	((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck( 1 ); 
	((CButton*)GetDlgItem(IDC_RADIO_NO_RIGHT))->SetCheck(1);
	InitCtrl();


	UpdateData(FALSE);
	return TRUE;
}


void DevSetDlg::InitCtrl()
{
	CString sql(_T("select * from [workatt]") );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )
	{
		int baseset = tmp->GetLong( _T("BaseSet") );
		((CButton*)GetDlgItem(IDC_CHECK_A))->SetCheck( baseset & DEV_RECORD_SAVEFAIL );
		((CButton*)GetDlgItem(IDC_CHECK_B))->SetCheck( baseset & DEV_SUPER_PASSWORD );
		((CButton*)GetDlgItem(IDC_CHECK_C))->SetCheck( baseset & DEV_HDBEEP_OPEN );
		((CButton*)GetDlgItem(IDC_CHECK_D))->SetCheck( baseset & 0x00000008 );
		((CButton*)GetDlgItem(IDC_CHECK_E))->SetCheck( baseset & DEV_REALTIME_RECORD );
		((CButton*)GetDlgItem(IDC_CHECK_F))->SetCheck( baseset & DEV_REALTIME_USERLOAD ); 
		((CButton*)GetDlgItem(IDC_CHECK_G))->SetCheck( baseset & DEV_REALTIME_USERSEND ); 
		((CButton*)GetDlgItem(IDC_CHECK_H))->SetCheck( baseset & DEV_DOORMANGET_OPEN ); 
		((CButton*)GetDlgItem(IDC_CHECK_I))->SetCheck( baseset & DEV_DOORFORCE_OPEN ); 
		((CButton*)GetDlgItem(IDC_CHECK_J))->SetCheck( baseset & DEV_REMOTE_CAP_SAVE ); 
		
		int mode = tmp->GetLong( _T("VerifyMode") );
		if ( mode & DEV_VERIFY_FACE_11 )
		{
			m_cmbValide.SetCurSel(0);
			if ( mode & DEV_VERIFY_USERID || mode & DEV_VERIFY_CARD )
			{
				((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->SetCheck( mode & DEV_VERIFY_USERID ); 
				((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( mode & DEV_VERIFY_CARD );
			}
			else 
			{
				((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( 1 ); 
			}
		}
		else if ( mode & DEV_VERIFY_FACE_MIX )
		{
			m_cmbValide.SetCurSel(1);
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->SetCheck( mode & DEV_VERIFY_USERID ); 
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( mode & DEV_VERIFY_CARD );
		}	
		else if ( mode & DEV_VERIFY_FACE_1N )
		{
			m_cmbValide.SetCurSel(2);
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->SetCheck( mode & DEV_VERIFY_USERID ); 
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( mode & DEV_VERIFY_CARD );
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck( 0 ); 
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( 1 ); 	
			((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->SetCheck( mode & DEV_VERIFY_USERID ); 
			m_cmbValide.EnableWindow(FALSE);
		}
	
		((CButton*)GetDlgItem(IDC_CHECK_WG_A))->SetCheck( tmp->GetLong( _T("wgoutput") ) ); 
		((CButton*)GetDlgItem(IDC_CHECK_WG_B))->SetCheck( tmp->GetLong( _T("wgtype") ) );
		((CButton*)GetDlgItem(IDC_CHECK_WG_C))->SetCheck( tmp->GetLong( _T("wgouttype") ) ); 
		((CButton*)GetDlgItem(IDC_CHECK_WG_D))->SetCheck( tmp->GetLong( _T("wgoutid") ) );
		
		CString values ;
		values.Format( _T("%d"), tmp->GetLong( _T("wgoutcontent") ) );
		GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(values);	
		values.Format( _T("%d"), tmp->GetLong( _T("DoorMangetTime") ) );
		GetDlgItem(IDC_EDIT_MANEGET)->SetWindowText(values);	
		values.Format( _T("%d"), tmp->GetLong( _T("LockTime") ) );
		GetDlgItem(IDC_EDIT_LOCK)->SetWindowText(values);		
		GetDlgItem(IDC_EDIT_PWD)->SetWindowText( tmp->GetCString( _T("pwd") ) );
		
		((CButton*)GetDlgItem(IDC_RADIO_NO_RIGHT))->SetCheck( tmp->GetLong( _T("defaultrt") ) == -2 );
		((CButton*)GetDlgItem(IDC_RADIO_SINGLE_RIGHT))->SetCheck( tmp->GetLong( _T("defaultrt") ) == -1 );
		
		
	}
}

void DevSetDlg::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	DEV_WORKATT dev_att = {0};

	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_A))->GetCheck() ? DEV_RECORD_SAVEFAIL : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_B))->GetCheck() ? DEV_SUPER_PASSWORD : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_C))->GetCheck() ? DEV_HDBEEP_OPEN : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_D))->GetCheck() ? 0x00000008 : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_E))->GetCheck() ? DEV_REALTIME_RECORD : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_F))->GetCheck() ? DEV_REALTIME_USERLOAD : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_G))->GetCheck() ? DEV_REALTIME_USERSEND : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_H))->GetCheck() ? DEV_DOORMANGET_OPEN : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_I))->GetCheck() ? DEV_DOORFORCE_OPEN : 0x00000000;
	dev_att.m_BaseSet |= ((CButton*)GetDlgItem(IDC_CHECK_J))->GetCheck() ? DEV_REMOTE_CAP_SAVE : 0x00000000;




	CString str_value;
	GetDlgItem(IDC_EDIT_MANEGET)->GetWindowText(str_value);
    dev_att.m_DoorMangetTime = atoi( str_value.GetBuffer(0) );
	GetDlgItem(IDC_EDIT_LOCK)->GetWindowText(str_value);
	dev_att.m_LockTime = atoi( str_value.GetBuffer(0) );

	int mode = 0;
	if ( ((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->GetCheck() )
	{
		int cursel = m_cmbValide.GetCurSel();
		switch ( cursel )
		{
		case 0:
			mode |= DEV_VERIFY_FACE_11;
			break;
		case 1:
			mode |= DEV_VERIFY_FACE_MIX;
			break;
		case 2:
			mode |= DEV_VERIFY_FACE_1N;
			break;
		default:
			break;
		}

		if ( cursel == 0 && (((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->GetCheck()==0 &&  ((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->GetCheck()==0) )
		{
			AfxMessageBox( _T("1:1 Mode must choose a credit card or a work number verification"));
			return;
		}
		mode |= ((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->GetCheck() ? 0x00000002 : 0x00000000;
		mode |= ((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->GetCheck() ? 0x00000001 : 0x00000000;
	}
	else
	{
		if ( ((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->GetCheck()==0   )
		{
			AfxMessageBox( _T("No face authentication mode must choose credit card verification"));
			return;
		}
		mode |= 0x00000002;
		mode |= ((CButton*)GetDlgItem(IDC_CHECK_OPEN_C))->GetCheck() ? 0x00000001 : 0x00000000;
	}
	dev_att.m_VerifyMode = mode;


	dev_att.m_nWGType = ((CButton*)GetDlgItem(IDC_CHECK_WG_B))->GetCheck() ? 2 : 1;	//WG_26=1 WG_34=2 ...

	dev_att.m_nWGOutType = ((CButton*)GetDlgItem(IDC_CHECK_WG_C))->GetCheck();
	dev_att.m_nWGOutIDType = ((CButton*)GetDlgItem(IDC_CHECK_WG_D))->GetCheck();
	GetDlgItem(IDC_EDIT_CONTENT)->GetWindowText(str_value);
	dev_att.m_nWGOutContent = atoi( str_value.GetBuffer(0) );	
	dev_att.m_bWGOutPut = ((CButton*)GetDlgItem(IDC_CHECK_WG_A))->GetCheck();

	dev_att.m_DefaultRight = ((CButton*)GetDlgItem(IDC_RADIO_NO_RIGHT))->GetCheck() ? DEV_DEFAULT_NO_RIGHT : DEV_DEFAULT_SINGLE_OPEN;

	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(str_value);
	memcpy( dev_att.m_szSuperPWD.m_ID, str_value.GetBuffer(0), strlen( str_value.GetBuffer(0) ) );

	int defrt = dev_att.m_DefaultRight;
	
	
	CString sql(_T("select * from [workatt]") );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( tmp->IsEmpty() )
	{
		sql.Format( _T("insert into [workatt] values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\')"),		
			dev_att.m_BaseSet, dev_att.m_DoorMangetTime, dev_att.m_LockTime, dev_att.m_VerifyMode,
			dev_att.m_nWGType, dev_att.m_nWGOutType, dev_att.m_nWGOutIDType, dev_att.m_nWGOutContent, 
			dev_att.m_bWGOutPut, dev_att.m_DefaultRight, 
			-1, -1, -1, -1,- 1, -1, -1, -1,
			dev_att.m_szSuperPWD.m_ID
			);//
	}
	else
	{
		sql.Format( _T("update [workatt] set [BaseSet]=%d, [DoorMangetTime]=%d, [LockTime]=%d, [VerifyMode]=%d,\
			[wgtype]=%d, [wgouttype]=%d,[wgoutid]=%d,[wgoutcontent]=%d,[wgoutput]=%d,[defaultrt]=%d,[pwd]=\'%s\'"),
			dev_att.m_BaseSet, dev_att.m_DoorMangetTime, dev_att.m_LockTime, dev_att.m_VerifyMode,
			dev_att.m_nWGType, dev_att.m_nWGOutType, dev_att.m_nWGOutIDType, dev_att.m_nWGOutContent, 
			dev_att.m_bWGOutPut, defrt, dev_att.m_szSuperPWD.m_ID
			);
	
		 
	}
	BOOL rt = m_ado.ExecuteNoSelectSQL(sql);
//	CHCRecordsetPtr tmp = m_ado.ExecuteNoSelectSQL(sql);
	
	if ( CPM_ULDevWorkAttInfo( m_IP, dev_att) )//update work attribute
	{
		AfxMessageBox( _T("Data uploaded successfully") );
	}
	else
	{
		AfxMessageBox( _T("The data uploading failure") );
	}
}

void DevSetDlg::OnButtonDown() 
{
	// TODO: Add your control notification handler code here	
    CPM_RegOperResultNotifyCB( &DevSetDlg::DevOperResult, this );
	if ( !(CPM_DLDevWorkAttInfo(m_IP)) )//download work attribute
	{
		AfxMessageBox( _T("Data request failed") );
	}
}


void DevSetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	CPM_RegOperResultNotifyCB(NULL, NULL);
	CDialog::OnClose();
}

//void DevSetDlg::DevOperResult( const char* server_addr, const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num,  void* param )
BOOL DevSetDlg::DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext )
{
#if 0
	if ( oper_type == CMD_GET_DEV_WORKATT )
	{
		DevSetDlg* pthis = (DevSetDlg*)param;
/*		pthis->UpdateData(FALSE);*/
	if ( OPER_SUCCESS==eRt && oper_content )
		pthis->DevWorkAttInfoNotify(dev_addr, *((DEV_WORKATT*)oper_content));
	}
#endif

    if ( eType == DEV_WORKATT_OPER )
    {
        //assert(eFlag == DEV_OPER_DOWNLOAD);
        DevSetDlg* pthis = (DevSetDlg*)pvContext;
        if ( OPER_SUCCESS==eResult && pvContent )
        {
		    pthis->DevWorkAttInfoNotify(cszDevAddr, *((DEV_WORKATT*)pvContent));
	    }
    }
	return TRUE;
}

void DevSetDlg::DevWorkAttInfoNotify( const char* dev_addr, const DEV_WORKATT& dev_att )
{
	//UpdateData(FALSE);
	int baseset = dev_att.m_BaseSet;
	((CButton*)GetDlgItem(IDC_CHECK_A))->SetCheck( baseset & DEV_RECORD_SAVEFAIL );
	((CButton*)GetDlgItem(IDC_CHECK_B))->SetCheck( baseset & DEV_SUPER_PASSWORD );
	((CButton*)GetDlgItem(IDC_CHECK_C))->SetCheck( baseset & DEV_HDBEEP_OPEN );
	((CButton*)GetDlgItem(IDC_CHECK_D))->SetCheck( baseset & 0x00000008 );
	((CButton*)GetDlgItem(IDC_CHECK_E))->SetCheck( baseset & DEV_REALTIME_RECORD );
	((CButton*)GetDlgItem(IDC_CHECK_F))->SetCheck( baseset & DEV_REALTIME_USERLOAD ); 
	((CButton*)GetDlgItem(IDC_CHECK_G))->SetCheck( baseset & DEV_REALTIME_USERSEND ); 
	((CButton*)GetDlgItem(IDC_CHECK_H))->SetCheck( baseset & DEV_DOORMANGET_OPEN ); 
	((CButton*)GetDlgItem(IDC_CHECK_I))->SetCheck( baseset & DEV_DOORFORCE_OPEN ); 
	((CButton*)GetDlgItem(IDC_CHECK_J))->SetCheck( baseset & DEV_REMOTE_CAP_SAVE ); 



	int mode = dev_att.m_VerifyMode;

	if ( mode & DEV_VERIFY_FACE_11 )
	{
		m_cmbValide.EnableWindow(TRUE);
		m_cmbValide.SetCurSel(0);
		((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck(1);
	}
	else if ( mode & DEV_VERIFY_FACE_MIX )
	{
		m_cmbValide.EnableWindow(TRUE);
		m_cmbValide.SetCurSel(1);
		((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck(1);
	}
	else if ( mode & DEV_VERIFY_FACE_1N )
	{	
		m_cmbValide.EnableWindow(TRUE);
		m_cmbValide.SetCurSel(2);
		((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck(1);
	}
	else
	{
		m_cmbValide.EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->SetCheck(0);
	}

	((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( mode & DEV_VERIFY_CARD );
	((CButton*)GetDlgItem(IDC_CHECK_OPEN_B))->SetCheck( mode & DEV_VERIFY_USERID );
	
	
	((CButton*)GetDlgItem(IDC_CHECK_WG_A))->SetCheck( dev_att.m_bWGOutPut ); 
	((CButton*)GetDlgItem(IDC_CHECK_WG_B))->SetCheck( dev_att.m_nWGType );
	((CButton*)GetDlgItem(IDC_CHECK_WG_C))->SetCheck( dev_att.m_nWGOutType ); 
	((CButton*)GetDlgItem(IDC_CHECK_WG_D))->SetCheck( dev_att.m_nWGOutIDType );
	
	CString values ;
	values.Format( _T("%d"), dev_att.m_nWGOutContent );
	GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(values);	
	values.Format( _T("%d"), dev_att.m_DoorMangetTime );
	GetDlgItem(IDC_EDIT_MANEGET)->SetWindowText(values);	
	values.Format( _T("%d"), dev_att.m_LockTime );
	GetDlgItem(IDC_EDIT_LOCK)->SetWindowText(values);		
	GetDlgItem(IDC_EDIT_PWD)->SetWindowText( dev_att.m_szSuperPWD.m_ID );
	
	((CButton*)GetDlgItem(IDC_RADIO_NO_RIGHT))->SetCheck( dev_att.m_DefaultRight == -2 );
	((CButton*)GetDlgItem(IDC_RADIO_SINGLE_RIGHT))->SetCheck( dev_att.m_DefaultRight == -1 );

	CString sql(_T("select * from [workatt]") );
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )
	{
		tmp->SetLong( _T("BaseSet"), dev_att.m_BaseSet);
		tmp->SetLong( _T("DoorMangetTime"), dev_att.m_DoorMangetTime);
		tmp->SetLong( _T("LockTime"), dev_att.m_LockTime);
		tmp->SetLong( _T("VerifyMode"), dev_att.m_VerifyMode);
		tmp->SetLong( _T("wgtype"), dev_att.m_nWGType);
		tmp->SetLong( _T("wgouttype"), dev_att.m_nWGOutType);
		tmp->SetLong( _T("wgoutid"), dev_att.m_nWGOutIDType);
		tmp->SetLong( _T("wgoutcontent"), dev_att.m_nWGOutContent);
		tmp->SetLong( _T("wgoutput"), dev_att.m_bWGOutPut);
		tmp->SetLong( _T("defaultrt"), dev_att.m_DefaultRight);
		tmp->SetCString( _T("pwd"), dev_att.m_szSuperPWD.m_ID);
		tmp->Update();
	}

	::PostMessage(this->GetSafeHwnd(), WM_UPDATE, 0, 0);
// 	UpdateData(FALSE);
	AfxMessageBox( _T("Success attributes to download DSP work") );	
	
}

void DevSetDlg::OnBNValide()
{
	m_cmbValide.EnableWindow( ((CButton*)GetDlgItem(IDC_CHECK_OPEN_A))->GetCheck() );
}

LRESULT DevSetDlg::OnUpdate(WPARAM wparam, LPARAM lpparam)
{
	UpdateData(lpparam);
	return 0;
}