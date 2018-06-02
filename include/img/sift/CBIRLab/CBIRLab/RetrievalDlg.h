#pragma once
#include "stdafx.h"
#include <vector>

#include "MatchSIFT.h"
#include "MySIFT.h"
#include "RetrievalResult.h"
#include "CBIRLabDlg.h"

using namespace std;
// CRetrievalDlg 对话框

class CRetrievalDlg : public CDialog
{
	DECLARE_DYNAMIC(CRetrievalDlg)

public:
	CRetrievalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRetrievalDlg();

public:
	IplImage* m_imageShow;//指向当前显示的图像为了显示而进行缩放变换的图像
	IplImage* m_image;//指向待查找的图像
	IplImage* m_currentImage;//指向当前显示的图像

	//表示computeResults的返回值
	int res;
	int m_time;
	int n_pos;
	CCBIRLabDlg* m_parentDlg;

	//CMySIFT imageSift;

	int picWidth;
	int picHeight;

public:
	void ShowImage( IplImage* img, UINT ID);
	void ResizeImage(IplImage* src, IplImage* dest);
	void computeResults();

// 对话框数据
	enum { IDD = IDD_DIALOG_RETRIEVAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 最终的检索结果
	CString m_editResult;
	CString m_folderPath;
	CString m_imagePath;
	CString m_editIndex;
	int showIndex;
	vector<CString> vcImageDataSet;
	vector<CRetrievalResult> vcRetrievalResultSet;
	// 表示当前显示的文件名称
	CString m_showFileName;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrior();
	afx_msg void OnClose();
};
