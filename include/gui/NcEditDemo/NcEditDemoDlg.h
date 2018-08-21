// NcEditDemoDlg.h : 头文件
//

#pragma once

#include "NcEditWnd.h"
// CNcEditDemoDlg 对话框
class CNcEditDemoDlg : public CDialog
{
// 构造
public:
	CNcEditDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CNcEditDemoDlg();
// 对话框数据
	enum { IDD = IDD_NCEDITDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	CNcEditWnd *m_pNcEditWnd;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuOpen();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
