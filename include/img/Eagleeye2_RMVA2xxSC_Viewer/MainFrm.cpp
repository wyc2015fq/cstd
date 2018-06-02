// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "Eagleeye2_ViewerDoc.h"
#include "math.h"
#include <WINSOCK.H>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
extern char			cRcvBuf[];
extern char			cmdbuf[]; 
int			Camerahour = 0, Cameraminute = 0,Camerasecond = 0;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SWICHNEWCAMERA, OnSwitchNewCamera)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//设置定时器触发间隔为1秒
	SetTimer(0,1000,NULL);
	//
	m_wndStatusBar.SetPaneText(1,"Current Temperature in Camera: No Link",TRUE);
	m_wndStatusBar.SetPaneText(2,"Time of RTC in Camera: No Link",TRUE);
	m_wndStatusBar.SetPaneText(3,"Camera MAC Address: No Link",TRUE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	static cnt = 0;
	CString	strTemp;
	float	fTemperature;
	CEagleeye2_ViewerDoc	*pDoc = (CEagleeye2_ViewerDoc*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveDocument();

	switch( nIDEvent ){
		case 0:
			if( !pDoc->m_bIsEnsureTgtIP ){
				m_wndStatusBar.SetPaneText(3,"Camera MAC Address: No Link",TRUE);
			}
			if( cnt++ == 60){
				cnt = 0;
				//获取目标相机的实时时钟时间并显示于状态条
				cmdbuf[0] = 0x19;
				cmdbuf[1] = 0x01;//需要相机回传数据
				*(UINT*)(cmdbuf+2) = 0;
				if( !pDoc->m_bIsEnsureTgtIP ){
					m_wndStatusBar.SetPaneText(2,"Time of RTC in Camera: No Link",TRUE);
				}
				else{
					if(pDoc->SendCMDtoDSP( cmdbuf )){
						Camerahour = cRcvBuf[4]; Cameraminute = cRcvBuf[5]; Camerasecond = cRcvBuf[6];
						strTemp.Format("Time of RTC in Camera: %d:%d:%d", Camerahour, Cameraminute, Camerasecond);
						m_wndStatusBar.SetPaneText(2,strTemp,TRUE);
					}
					else{
						AfxMessageBox("向相机发送命令失败");
					}
				}
				//获取目标相机的当前内部温度并显示于状态条
				cmdbuf[0] = 0x1e;
				cmdbuf[1] = 0x01;//需要相机回传数据
				*(UINT*)(cmdbuf+2) = 0;
				if( !pDoc->m_bIsEnsureTgtIP ){
					m_wndStatusBar.SetPaneText(1,"Current Temperature in Camera: No Link",TRUE);
				}
				else{
					if(pDoc->SendCMDtoDSP( cmdbuf )){
						fTemperature = *((float*)cRcvBuf);
						strTemp.Format("Current Temperature in Camera: %5.3f", fTemperature);
						m_wndStatusBar.SetPaneText(1,strTemp,TRUE);
					}
					else{
						AfxMessageBox("向相机发送命令失败");
					}
				}
			}
			else{
				if( pDoc->m_bIsEnsureTgtIP ){
					Camerasecond++;
					if( Camerasecond > 59){
						Camerasecond = 0;
						Cameraminute += 1;
						if( Cameraminute > 59){
							Cameraminute = 0;
							Camerahour += 1;
							if( Camerahour > 23 ){
								Camerahour = 0;
							}
						}
					}
					strTemp.Format("Time of RTC in Camera: %d:%d:%d", Camerahour, Cameraminute, Camerasecond);
					m_wndStatusBar.SetPaneText(2,strTemp,TRUE);
				}
			}
			break;
		default:
			break;
	}
}
//相应切换相机后的消息函数
LRESULT CMainFrame::OnSwitchNewCamera(WPARAM wParam, LPARAM lParam)
{
	int			i,j;
	CString		strTemp;
	float		fTemperature;
	char		strMacAdd[100] = {0};
	char		strtmp[] = "Camera MAC Address:";
	CEagleeye2_ViewerDoc	*pDoc = (CEagleeye2_ViewerDoc*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveDocument();

	//获取目标相机的实时时钟时间并显示与状态条
	cmdbuf[0] = 0x19;
	cmdbuf[1] = 0x01;//需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0;
	if( !pDoc->m_bIsEnsureTgtIP ){
		m_wndStatusBar.SetPaneText(2,"Time of RTC in Camera: No Link",TRUE);
	}
	else{
		if(pDoc->SendCMDtoDSP( cmdbuf )){
			Camerahour = cRcvBuf[4]; Cameraminute = cRcvBuf[5]; Camerasecond = cRcvBuf[6];
			strTemp.Format("Time of RTC in Camera: %d:%d:%d", Camerahour, Cameraminute, Camerasecond);
			m_wndStatusBar.SetPaneText(2,strTemp,TRUE);
		}
		else{
			AfxMessageBox("向相机发送命令失败");
		}
	}
	//获取目标相机的当前内部温度并显示与状态条
	cmdbuf[0] = 0x1e;
	cmdbuf[1] = 0x01;//需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0;
	if( !pDoc->m_bIsEnsureTgtIP ){
		m_wndStatusBar.SetPaneText(1,"Current Temperature in Camera: No Link",TRUE);
	}
	else{
		if(pDoc->SendCMDtoDSP( cmdbuf )){
			fTemperature = *((float*)cRcvBuf);
			strTemp.Format("Current Temperature in Camera: %5.3f", fTemperature);
			m_wndStatusBar.SetPaneText(1,strTemp,TRUE);
		}
		else{
			AfxMessageBox("向相机发送命令失败");
		}
	}
	//获取目标相机的MAC地址并显示与状态条
	cmdbuf[0] = 0x1c;
	cmdbuf[1] = 0x01;//需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0;
	if( !pDoc->m_bIsEnsureTgtIP ){
		m_wndStatusBar.SetPaneText(3,"Camera MAC Address: No Link",TRUE);
	}
	else{
		if(pDoc->SendCMDtoDSP( cmdbuf )){
			j = 0;
			for( i=0; i<6; i++){
				if(((cRcvBuf[i]&0xf0)>>4)<10){
					strMacAdd[j++] = ((cRcvBuf[i]&0xf0)>>4) + 48;
				}
				else{
					strMacAdd[j++] = ((cRcvBuf[i]&0xf0)>>4) + 55;
				}
				if((cRcvBuf[i]&0x0f)<10){
					strMacAdd[j++] = (cRcvBuf[i]&0x0f) + 48;
				}
				else{
					strMacAdd[j++] = (cRcvBuf[i]&0x0f) + 55;
				}
				strMacAdd[j++] = 45;
			}
			strMacAdd[j-1] = 0;
			strcat(strtmp,strMacAdd);
			m_wndStatusBar.SetPaneText(3,strtmp,TRUE);
		}
		else{
			AfxMessageBox("向相机发送命令失败");
		}
	}
	return 0L;
}
