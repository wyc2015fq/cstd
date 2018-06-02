// SelectFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "SelectFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectFileDlg dialog


CSelectFileDlg::CSelectFileDlg(BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
		LPCTSTR lpszFilter, CWnd* pParentWnd)
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
		dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CSelectFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ofn.Flags |= OFN_ENABLETEMPLATE ;
	m_ofn.lpTemplateName = MAKEINTRESOURCE (IDD_SELECTFILE_PREVIEW) ;
	m_ofn.lpstrTitle = TEXT("打开图象文件") ;
}

void CSelectFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectFileDlg)
	DDX_Control(pDX, IDS_PREVIEW, m_Preview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectFileDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectFileDlg message handlers

BOOL CSelectFileDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSelectFileDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	OFNOTIFY	* notify = (OFNOTIFY *)lParam ;
	if (notify->hdr.code == CDN_SELCHANGE)
	{
		TCHAR		name[MAX_PATH] ;
		::SendMessage (notify->hdr.hwndFrom, CDM_GETFILEPATH, MAX_PATH, (LPARAM)name) ;
		FCImage		pic ;
		if (pic.Load (name))
		{
			if (pic.ColorBits() <= 8)
				pic.ConvertTo32Bit () ;
			
			RECT	rc ;
			m_Preview.GetClientRect (&rc) ;
			if ((pic.Width() > rc.right) || (pic.Height() > rc.bottom))
				pic.Stretch (rc.right, rc.bottom) ;
			DWORD	style = m_Preview.GetStyle () ;
			style |= SS_BITMAP | SS_CENTERIMAGE ;
			::SetWindowLong (m_Preview.GetSafeHwnd(), GWL_STYLE, style) ;
			m_Preview.SetBitmap (pic.GetHandle()) ;
			m_Preview.InvalidateRect (NULL) ;
		}
		return TRUE ;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}
