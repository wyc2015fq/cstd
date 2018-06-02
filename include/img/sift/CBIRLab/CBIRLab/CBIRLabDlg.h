
// CBIRLabDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CCBIRLabDlg 对话框
class CCBIRLabDlg : public CDialog
{
// 构造
public:
	CCBIRLabDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CBIRLAB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CMenu m_myMenu;
	CProgressCtrl m_progressCtrl;

	IplImage* m_imageShow;
	IplImage* m_image;

	int picWidth;
	int picHeight;

	CString imagePath;
	CString folderPath;

public:
	void ShowImage( IplImage* img, UINT ID);
	void ResizeImage(IplImage* src, IplImage* dest);

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
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuLoad();
	// 显示文件信息的编辑框
	CString m_editFileName;
	CString m_editFormat;
	CString m_editSize;
	CString m_editByte;
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuFolder();
	CString m_editFolderPath;
	afx_msg void OnMenuRetrieval();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRetrieval();
	afx_msg void OnBnClickedButtonFolder();
};
