#include "stdafx.h"
#include "DialogBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////
//			Class written by Sharad Kelkar drssk@ad1.vsnl.net.in					//
//	This is freeware without any kind of restriction on usage and distribution.		//
//////////////////////////////////////////////////////////////////////////////////////
#define WM_INITDIALOGBAR WM_USER + 1

BEGIN_MESSAGE_MAP(CDialogBarEx,  CDialogBar)
	//{{AFX_MSG_MAP(CDialogBarEx)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOGBAR , InitDialogBarHandler )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDialogBarEx::CDialogBarEx()
{
}

CDialogBarEx::~CDialogBarEx()
{
}

int CDialogBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1 ;

	// TODO: Add your specialized creation code here
	// --------- 
	// We post WM_INITDIALOGBAR message here to dialog bar
	PostMessage (WM_INITDIALOGBAR, 0 , 0) ;
	return 0 ;
}


void CDialogBarEx::InitDialogBarHandler(WORD wParam, DWORD lParam)
{
	UpdateData (FALSE) ;
	OnInitDialogBar () ;
}

// OnInitDialogBar is Just empty function, it is expected to be overriden from derived class
void CDialogBarEx::OnInitDialogBar()
{
	// TODO: Add your custom initialization code here.
}
