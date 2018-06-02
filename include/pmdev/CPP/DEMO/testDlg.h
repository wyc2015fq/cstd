// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__BD8001A6_3A23_486E_A29D_307720382235__INCLUDED_)
#define AFX_TESTDLG_H__BD8001A6_3A23_486E_A29D_307720382235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#include "MenuEx.h"
#include "SkinList.h"
#include "DevStatisDlg.h"
#include "DevSet.h"

#include "AddUserDlg.h"

#include "CriticalSection.h"
#include ".\skin\skin.h"


#include <map>
#include <list>
using namespace std;
 

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor
    ~CTestDlg();
// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CIPAddressCtrl	m_IPDev;
	CIPAddressCtrl	m_IPCtrl;
	CIPAddressCtrl	m_IPServer;

	CComboBox	m_cmbCondition;
	CComboBox   m_cmbRecordCondition;
	CStatic	m_static;
	CStatic m_staticRecord;
	CListCtrl	m_user;
	CListCtrl	m_dev;
	CListCtrl   m_record;
	CListCtrl	m_log;
	CListCtrl	m_image;
	CTime	m_ETimeRec;
	CTime	m_ETimeUser;
	CTime	m_STimeRec;
	CTime	m_STimeUser;

	int     m_nFeatMax;
	int     m_nPicMax;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSerch();
	afx_msg void OnRButtonDown( NMHDR * pNotifyStruct, LRESULT* result  );
	afx_msg void OnCommandMenu(UINT nID);
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonNew();
	afx_msg void OnLClickUserList( NMHDR * pNotifyStruct, LRESULT* result  );
	afx_msg void OnLClickRecordList( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg void OnLClickCImageList( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnButtonDel();
	afx_msg void OnButtonPic();
	afx_msg void OnButton1();
	afx_msg void OnButtonDlrecord();
	afx_msg void OnButtonDelrecord();
	afx_msg void OnClose( );
	afx_msg void OnButtonTimegroup();
	afx_msg void OnButtonRright();
	afx_msg void OnButtonUG();
	afx_msg void OnButtonOpenDoor();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonSetaddr();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonCNServer();
	afx_msg void OnButtonDCNServer();
	afx_msg void OnUpdate();
	afx_msg void OnFile();
	afx_msg void OnButtonDLLog();
	afx_msg void OnButtonDelLog();
	afx_msg void OnButtonDLImage();
	afx_msg void OnButtonDelImage();
	afx_msg void OnCmdNetOrUDisk(UINT nID);
	afx_msg void OnBtnUDiskScan();
	afx_msg void OnBtnUDiskCN();
	afx_msg void OnBtnReboot();
	afx_msg LRESULT OnUpdateImage(WPARAM wparam, LPARAM lpparam);

	afx_msg void OnButtonCancelOpenDoor();
	afx_msg void OnButtonROpenDoor();
	afx_msg void OnButtonECloseDoor();
	afx_msg void OnButtonRCloseDoor();


	afx_msg LRESULT  DevBaseMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevUserMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevRecMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevLogMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevAuthMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevImageMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevRealLoadMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevLogNumsMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevUserNumsMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevCImageNumsMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevRecNumsMsg( WPARAM wparam, LPARAM lpparam );
	afx_msg LRESULT  DevAlarmNumsMsg( WPARAM wparam, LPARAM lpparam );
	

	afx_msg void OnBtnStartVideo();
	afx_msg void OnBtnCap();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitDev();
	void InitUser();
	void InitRecord();
	void InitLog();
	void InitCImage();
	void ShowImage( int index );
	void ShowRecordImage( int index, const CString& str_id );
	void DelUserEx() ;
	void DownUserEx() ;
	void UpUserEx();

	BOOL IsClickDmyButton(CPoint pt);
private:
	friend BOOL CALLBACK DevConnectNotify(const char* dev_addr, DEV_CONNECT_STATUS flag, void* param );
	friend BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
	void DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult);
	void DevConnectNotify(const char* cszDevAddr, DEV_CONNECT_STATUS eCNStatus);

private:
    typedef void (CTestDlg::*OperResult)( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );

	void DevBaseInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevUserInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevRecordInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevLogInfoOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevAuthOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
    void DevAddOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
    void DevFormatOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );

	void DevChangeImageOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevRealLoadUser( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevLogNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevUserNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevChangeImageNumsOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevRecNums( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevAlarmNums( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevUserExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );

	void DevLogExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevRecExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );
	void DevCImageExOper( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult );

private:
	typedef std::map<DEV_OPER_TYPE, OperResult> CMDPROCESSOR_MAP;
	CMDPROCESSOR_MAP m_map;

private:
	CMenuEx m_menu;
	CSuperGridCtrl aa;
	CSuperGridCtrl bb;
private:
	CHCADOConnection m_ado;
private:
	char m_IP[16];
	BOOL m_bNet;
	char m_serverIP[16];
	int m_nImageItems ;
	std::list<void*> m_pImageList;
private:
	HANDLE m_hEvent;
	int    m_nLClickTimes;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__BD8001A6_3A23_486E_A29D_307720382235__INCLUDED_)
