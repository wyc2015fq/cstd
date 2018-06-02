// AddUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "AddUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AddUserDlg dialog


AddUserDlg::AddUserDlg(CHCADOConnection& ado, char* ID, CWnd* pParent /*=NULL*/)
	: m_ado(ado),m_pUserID(ID), CDialog(AddUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AddUserDlg)
	m_ID = _T("");
	m_Card = _T("");
	m_Dep = _T("");
	m_Name = _T("");
	m_invalideTime = _T("");

	m_index = -1;
	m_Right = -1;
	m_UserGroup = -1;
	m_UserType = 0;
	m_userFlag = 0;
	//}}AFX_DATA_INIT
}


void AddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddUserDlg)
	DDX_Control(pDX, IDC_COMBO_RIGHT, m_cmbRight);
	DDX_Control(pDX, IDC_COMBO_UG, m_cmbUserGroup);
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDX_Text(pDX, IDC_EDIT_CARD, m_Card);
	DDX_Text(pDX, IDC_EDIT_DEP, m_Dep);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_INVALIDETIME, m_invalideTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddUserDlg, CDialog)
	//{{AFX_MSG_MAP(AddUserDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
// 	ON_CBN_SELCHANGE(IDC_COMBO_RIGHT, OnSelchangeComboRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddUserDlg message handlers

void AddUserDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int index = m_cmbRight.GetCurSel();
	if ( index >= 0 )
	{
		m_Right = m_cmbRight.GetItemData(index);
	}

	index = m_cmbUserGroup.GetCurSel();
	if ( index >= 0 )
	{
		m_UserGroup = m_cmbUserGroup.GetItemData(index);
	}
	
	m_userFlag |= ((CButton*)GetDlgItem(IDC_CHECK_BLACK))->GetCheck()==1 ? 0x04 : 0x00;
	m_userFlag |= ((CButton*)GetDlgItem(IDC_CHECK_MUTEX))->GetCheck()==1 ? 0x08 : 0x00;
	m_userFlag |= ((CButton*)GetDlgItem(IDC_CHECK_CARD_INVALIDE))->GetCheck() ==1 ? 0x01 : 0x00 ;
	m_userFlag |= ((CButton*)GetDlgItem(IDC_CHECK_IDLINE))->GetCheck() ==1 ? 0x02 : 0x00 ;

	m_UserType |= ((CButton*)GetDlgItem(IDC_RADIO_SUPERADMIN))->GetCheck()==1 ? 0x02 : 0x00;
	m_UserType |= ((CButton*)GetDlgItem(IDC_RADIO_ADMIN))->GetCheck()==1 ? 0x01 : 0x00;
	m_UserType |= ((CButton*)GetDlgItem(IDC_RADIO_NOMAL))->GetCheck()==1 ? 0x00 : 0x00;

	CDialog::OnOK();
}

void AddUserDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL AddUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString sql(_T("select [RightID], [Name] from [right]"));
	CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
	if ( !tmp->IsEmpty() )  
	{
		tmp->MoveFirst();
		CString name ;
		int     ID = 0;
		int n = 0;
		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
		{
			name = tmp->GetCString( _T("Name") );
			ID   = tmp->GetLong( _T("RightID") );
			n = m_cmbRight.AddString(name);
			m_cmbRight.SetItemData(n, ID);
		}
	}
	int n = m_cmbRight.AddString(  _T("Without permission") );
	m_cmbRight.SetItemData(n, -2);
	m_cmbRight.SetCurSel(n);
	n = m_cmbRight.AddString(  _T("A single door access") );
	m_cmbRight.SetItemData(n, -1);
// 	sql = _T("select [GroupID], [Name] from [ugroup]") ;
// 	tmp = m_ado.ExecuteSelectSQL(sql);
// 	if ( !tmp->IsEmpty() )  
// 	{
// 		tmp->MoveFirst();
// 		CString name ;
// 		int     ID = 0;
// 		int n = 0;
// 		for ( ; !tmp->IsEOF(); tmp->MoveNext(), ++n )
// 		{
// 			name = tmp->GetCString( _T("Name") );
// 			ID   = tmp->GetLong( _T("GroupID") );
// 			n = m_cmbUserGroup.AddString(name);
// 			m_cmbUserGroup.SetItemData(n, ID);
// 		}
// 	}
	m_cmbUserGroup.EnableWindow(FALSE);
	if (m_pUserID != NULL )
	{
		CString sql;
		sql.Format( _T("select * from [user] where [UserID] = \'%s\'"), m_pUserID);
		CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(sql);
		if ( !tmp->IsEmpty() )
		{
			m_ID = m_pUserID;
			m_Card = tmp->GetCString(_T("AppendID"));
			m_Dep  = tmp->GetCString(_T("Department"));
			m_Name = tmp->GetCString(_T("UserName"));
			m_invalideTime = tmp->GetCString(_T("InvalidTime"));
			int R_ID = tmp->GetLong(_T("RightID"));
			if ( R_ID > -1 ) 
			{
				int count = m_cmbRight.GetCount();
				for(int i = 0; i < count; ++i)
				{
					if ( R_ID == m_cmbRight.GetItemData(i) ) 
					{
						m_cmbRight.SetCurSel(i);
						break;
					}
				}
			}
// 			int UG_ID = tmp->GetLong(_T("UserGroup"));
// 			if ( UG_ID > -1 ) 
// 			{
// 				int count = m_cmbUserGroup.GetCount();
// 				for(int i = 0; i < count; ++i)
// 				{
// 					if ( R_ID == m_cmbUserGroup.GetItemData(i) ) 
// 					{
// 						m_cmbUserGroup.SetCurSel(i);
// 						break;
// 					}
// 				}
// 			}
//			m_bCardInvalide = tmp->GetLong( _T("AppendIDInvalid") );
			int flag = tmp->GetLong(_T("UserFlag"));

			((CButton*)GetDlgItem(IDC_CHECK_BLACK))->SetCheck( flag & 0x04 );
			((CButton*)GetDlgItem(IDC_CHECK_MUTEX))->SetCheck( flag & 0x08 );
			((CButton*)GetDlgItem(IDC_CHECK_CARD_INVALIDE))->SetCheck( flag & 0x01 ) ;
			((CButton*)GetDlgItem(IDC_CHECK_IDLINE))->SetCheck( flag & 0x02 ) ;

			int user_type = tmp->GetLong(_T("UserType"));
			((CButton*)GetDlgItem(IDC_RADIO_SUPERADMIN))->SetCheck( user_type == 0x02 ) ;
			((CButton*)GetDlgItem(IDC_RADIO_ADMIN))->SetCheck( user_type == 0x01 ) ;
			((CButton*)GetDlgItem(IDC_RADIO_NOMAL))->SetCheck( user_type == 0x00 ) ;
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_SUPERADMIN))->SetCheck( 0 ) ;
		((CButton*)GetDlgItem(IDC_RADIO_ADMIN))->SetCheck( 0 ) ;
		((CButton*)GetDlgItem(IDC_RADIO_NOMAL))->SetCheck( 1 ) ;
		((CButton*)GetDlgItem(IDC_CHECK_CARD_INVALIDE))->SetCheck( 0 ) ;
	}
	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AddUserDlg::OnSelchangeComboRight() 
{
	// TODO: Add your control notification handler code here
	int index = m_cmbRight.GetCurSel();
	if ( index > 0 )
	{
		m_Right = m_cmbRight.GetItemData(index);
	}
}
