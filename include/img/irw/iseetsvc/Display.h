/********************************************************************

	Display.h

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件图视
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30112
	最后修改于：	2003-1-12

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_)
#define AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Display.h : header file
//

// 显示区、标题区距客户区边界的间隔
enum {ISDISP_LEFT = 10, ISDISP_RIGHT = 10, ISDISP_TOP = 10, ISDISP_BOTTOM = 10};

enum {ISDISP_HEADER_H = 20};							// 标题块高度
enum {ISDISP_HEADER_W = 60};							// 标题块宽度
enum {ISCOL_HEADER_BG = 0xe7b29c};						// 标题块背景色

enum {ISDISP_HISPEC   = 10};							// 标题区与显示区的间隔
enum {ISDISP_FRMSPEC  = 16};							// 画框与图象的最小间隔

enum {ISDISP_MIN_WIDTH= 160, ISDISP_MIN_HEIGHT = 160};	// 使画框不再缩小的最小图象尺寸

enum {ISDISP_MAXWIDTH = 128000};						// 单行图象的最大宽度
enum {ISDISP_LINEBUFFSIZE = ISDISP_MAXWIDTH*4};			// 扫描行缓冲区尺寸
enum {ISDISP_MAXBUFFSIZE = 10*IRWE_MB};					// 显示缓冲区尺寸

enum {ISDISP_SWH_FAST, ISDISP_SWH_STYLE};				// 画面切换方式

enum {ISDISP_MINBTN_WIDTH = 60};						// 按钮与标题块之间的最小间距
enum {ISDISP_BTN_W = 17, ISDISP_BTN_H = 17};
enum {ISDISP_BTN_SPEC = 4};

enum {ISDISP_TREE_MIN = 0, ISDISP_LISTSS = 100};		// 观看模式下树视、列表视宽度
enum {ISDISP_NOL = 0, ISDISP_VIEWMODL = 1};				// 正常模式、浏览模式定义

enum {ISDISP_MAXANI_TIME = 10000, ISDISP_MINANI_TIME = 10};	// 自动播放的延迟时间限制


#define ISDBTNID_DISPTYPE	200							// 按钮ID
#define ISDBTNID_RS			201

#define ISDWHEEL_DELTA		40							// 滚轮移动值的放大倍数

#define	MAX_PALETTE			256

typedef struct _tagBITMAPINFO_EX
{
	wBITMAPINFOHEADER	info;
	wRGBQUAD				pal[MAX_PALETTE];
} BITMAPINFO_EX, *PBITMAPINFO_EX;


#define DPTRS_MIN			((float)0.1)				// 最小缩放倍数
#define DPTRS_MAX			((float)8.0)				// 最大缩放倍数
#define DPTRS_STEP			((float)0.4)				// 缩放倍数步进值
#define DPTRS_REAL			((float)1.0)				// 原始尺寸的缩放倍数
#define DPTRS_DEFAULT		DPTRS_REAL					// 缺省缩放倍数（原始尺寸）




/////////////////////////////////////////////////////////////////////////////
// CDisplay view

class CDisplay : public CView
{
public:
	enum {DPT_LEFTTOP, DPT_RIGHTTOP, DPT_LEFTBOTTOM, DPT_RIGHTBOTTOM, DPT_CENTER};

protected:
	CDisplay();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDisplay)

// Attributes
public:
	HDRAWDIB	m_hDrawDib;								// DrawDib句柄
	CString		m_sImgInfo;								// 图象宽、高信息的字符串

	int			m_iTalAutResize;						// 全局自动缩放标志
	float		m_fTalRatio;							// 全局缩放倍数
	int			m_iAutoResize;							// 当前自动缩放标志
	float 		m_fRatio;								// 当前缩放倍数
	int			m_iPassMark;							// 放大镜启用标志

	int			m_iInitPos;								// 图象的初始放置位置(左上、左下、右上、右下、居中)
	int			m_iSwitch;								// 图象切换的方式(快速、风格化)

	CRect		m_rcBmpRc;								// 图象原始尺寸

	CRect		m_rcPict;								// 框中图象的缩放尺寸
	CRect		m_rcPTRc;								// 图象的客户坐标
	CRect		m_rcFrme;								// 包围图片的修饰框的尺寸
	CRect		m_rcFTRc;								// 修饰框的客户坐标

	CRect		m_rcClient;								// 客户区矩形
	CRect		m_rcHeader;								// 信息区矩形
	CRect		m_rcImage;								// 图象区矩形
	
	int			m_iCurrFr;								// 当前显示图象的帧号(0-主、非0-子)
	int			m_iInfo;								// 前一帧图象的处置方式代码
	
	CPoint		m_cpPrePt;								// 拖动状态下前一个点的坐标
	BOOL		m_bMoveing;								// 拖动标志

	CBitmap		m_cbBackupBmp;							// 背景位图

	int			m_icbTrueState;							// 按钮可见标志
	int			m_icbResize;
	CButtonST	m_cbTrue;								// 自动缩放与真实尺寸的切换按钮
	CButtonST	m_cbResize;								// 手动缩放按钮

	HCURSOR		m_hcHSd;								// 标准光标及手形光标
	HCURSOR		m_hcHFr;
	HCURSOR		m_hcHOd;
	HCURSOR		m_hcHPs;								// 放大镜光标

	char		*m_pLineBuf;							// 图象翻转缓冲区
	char		*m_pDispBuf;							// 显示缓冲区
	ulong	m_lBuffSize;						// 显示缓冲区当前尺寸

	CTimeCounter	*m_cCurrPackCnt;					// 读图象计时器

	CWinThread		*m_cAniThread;						// 动画延时线程
	BOOL			m_bDiableMark;						// 废弃命令标签

	int				m_iViewModl;						// 浏览模式标志
	WINDOWPLACEMENT	m_wp;
	int				m_iOTW, m_iORW, m_iODW, m_iOLW;
	
	
// Operations
public:
	void	DrawHeader(CDC *pDC, int ord);
	void	DrawImage(CDC *pDC);

	void	SetDispLocInfo(void);
	void	SetFrmeLocInfo(BOOL init=FALSE);
	void	SetPictLocInfo(BOOL init=FALSE);

	BOOL	IsDrop(void);

	void	ClearDispPosInfo(void);

	static UINT AniProc(LPVOID parm);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplay)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDisplay();

	BOOL	IsCurrPack(LPSPEC_DATAPACK psdp);

	void	_RSS(CPoint point);
	void	_RSL(CPoint point);

	void	_UpdateImageRgn(CRect& rect);
	void	_UpdateHeadRgn(BOOL rgn);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDisplay)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnDispIntoPlug(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispBuildMi(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispBuildSi(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispOver(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispFaild(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispAniNext(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAY_H__C43754E9_BE42_4A57_9F44_7D51DAC16819__INCLUDED_)
