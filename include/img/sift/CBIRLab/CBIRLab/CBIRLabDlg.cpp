
// CBIRLabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CBIRLab.h"
#include "CBIRLabDlg.h"
#include "RetrievalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCBIRLabDlg 对话框




CCBIRLabDlg::CCBIRLabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBIRLabDlg::IDD, pParent)
	, m_editFileName(_T(""))
	, m_editFormat(_T(""))
	, m_editSize(_T(""))
	, m_editByte(_T(""))
	, m_editFolderPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_image = NULL;
}

void CCBIRLabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Text(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_editSize);
	DDX_Text(pDX, IDC_EDIT_BYTE, m_editByte);
	DDX_Text(pDX, IDC_EDIT_DATASET_PATH, m_editFolderPath);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
}

BEGIN_MESSAGE_MAP(CCBIRLabDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_QUIT, CCBIRLabDlg::OnMenuQuit)
	ON_COMMAND(ID_MENU_LOAD, CCBIRLabDlg::OnMenuLoad)
	ON_COMMAND(ID_MENU_ABOUT, CCBIRLabDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_FOLDER, CCBIRLabDlg::OnMenuFolder)
	ON_COMMAND(ID_MENU_RETRIEVAL, CCBIRLabDlg::OnMenuRetrieval)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, CCBIRLabDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RETRIEVAL, CCBIRLabDlg::OnBnClickedButtonRetrieval)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, CCBIRLabDlg::OnBnClickedButtonFolder)
END_MESSAGE_MAP()


// CCBIRLabDlg 消息处理程序

BOOL CCBIRLabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//添加菜单栏
	m_myMenu.LoadMenu(IDR_MENU1);
	SetMenu(&m_myMenu);

	//得到图片空间的尺寸
	// 获得显示控件的 DC
	CDC* pDC = GetDlgItem( IDC_PICTURE_CTL ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	HDC hDC = pDC ->GetSafeHdc();				
	CRect rect;
	GetDlgItem(IDC_PICTURE_CTL) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	picWidth = rect.right - rect.left;			
	picHeight = rect.bottom - rect.top;

	ReleaseDC( pDC );

	//初始化m_image
	CvSize ImgSize;
	ImgSize.height = picHeight;
	ImgSize.width = picWidth;
	m_imageShow = cvCreateImage( ImgSize, IMG_DEPTH_8U, IMAGE_CHANNELS );
	cvSet(m_imageShow, cvScalarAll(255));

	//初始化m_progressCtrl
	m_progressCtrl.SetRange(0, 100);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCBIRLabDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCBIRLabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 重绘对话框
		CDialog::OnPaint();
		// 更新windows窗口，如果无这步调用，图片显示还会出现问题
		CDialog::UpdateWindow();
		// 重绘图片函数
		ShowImage( m_imageShow, IDC_PICTURE_CTL);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCBIRLabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCBIRLabDlg::OnMenuQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}

void CCBIRLabDlg::OnMenuLoad()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg; *png; *.jpeg) |*.bmp;*.jpg;*.png;*.jpeg|All Files (*.*)|*.*||"),NULL);

	dlg.m_ofn.lpstrTitle= _T("载入图像");

	if (dlg.DoModal() == IDOK)
	{
		imagePath = dlg.GetPathName();
		// 读取图片
		m_image = cvLoadImage( imagePath, 1 );
		
		//cvShowImage("CC", m_image);
		
		// 判断是否成功读取图片	
		if( !m_image )			
			return;

		m_editFileName = imagePath;
		m_editFormat = PathFindExtensionA(imagePath);
		m_editFormat.Delete(0);
		m_editFormat.MakeUpper();
		m_editSize.Format("%d*%d", m_image->width, m_image->height);

		//文件大小
		CFile m_file;
		m_file.Open(imagePath, CFile::modeRead);
		m_editByte.Format("%dKB", m_file.GetLength()/1024);
		m_file.Close();


		// 对上一幅显示的图片数据清零
		if( m_imageShow )
				cvSet(m_imageShow, cvScalarAll(255));

		// 对读入的图片进行缩放到picture控件的合适位置，再复制到 TheImage 中
		ResizeImage( m_image, m_imageShow );	

		ShowImage( m_imageShow, IDC_PICTURE_CTL);

		UpdateData(FALSE);
	}
}

void CCBIRLabDlg::ShowImage( IplImage* img, UINT ID)
{
	// 获得显示控件的 DC
	CDC* pDC = GetDlgItem( ID ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	HDC hDC = pDC ->GetSafeHdc();				

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// 读取图片的宽和高
	int iw = img->width;	
	int ih = img->height;
	// 使图片的显示位置正好在控件的正中
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// 复制图片
	CvvImage cimg;
	cimg.CopyOf( img );
	// 将图片绘制到显示控件的指定区域内	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );
}

//把图片src缩放到dest的大小，大的缩放，小的则不变
void CCBIRLabDlg::ResizeImage(IplImage* src, IplImage* dest)
{
	// 读取图片的宽和高
	int w = src->width;
	int h = src->height;
	float scale1 = (float)w/(float)picWidth;
	float scale2 = (float)h/(float)picHeight;

	// 找出宽和高中缩放比例的较大值者
	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (scale1 > scale2) ? scale1: scale2;

	// 缩放后图片的宽和高
	int nw, nh;
	if(scale > 1.0f)
	{
		nw = (int)( w/scale );
		nh = (int)( h/scale );
	}
	else
	{
		nw = w;
		nh = h;
	}

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > picWidth)? 0: (int)(picWidth-nw)/2;
	int tly = (picHeight > nh)? (int)(picHeight-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( dest, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize( src, dest );

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( dest );
}


void CCBIRLabDlg::OnMenuAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CCBIRLabDlg::OnMenuFolder()
{
	// TODO: 在此添加命令处理程序代码
	BROWSEINFO bi;
	char szPathName[MAX_PATH];
	char szTitle[] = "选择路径";
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = szPathName;
	bi.lpszTitle = szTitle;
	bi.ulFlags = 0x0040 ;
	CString str;

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
	{
		folderPath= "";
		return;
	}
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH * 2));
	str.ReleaseBuffer();
	if(str != "" && str.GetAt(str.GetLength() - 1) != '\\')
		str += "\\";
	folderPath = str;
	m_editFolderPath = folderPath;
	UpdateData(FALSE);
}

void CCBIRLabDlg::OnMenuRetrieval()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_image)
	{
		AfxMessageBox("先载入待检索图像！");
		return;
	}
	if (folderPath.IsEmpty())
	{
		AfxMessageBox("请设定检索数据集目录！");
		return;
	}
	CRetrievalDlg m_retrievalDlg;
	m_retrievalDlg.m_parentDlg = this;
	m_retrievalDlg.m_image = m_image;
	m_retrievalDlg.m_imagePath = imagePath;
	m_retrievalDlg.m_folderPath = folderPath;
	m_retrievalDlg.m_time = clock();
	m_retrievalDlg.computeResults();
	m_retrievalDlg.m_time = clock() - m_retrievalDlg.m_time;
	m_retrievalDlg.DoModal();
}

void CCBIRLabDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuLoad();
}

void CCBIRLabDlg::OnBnClickedButtonRetrieval()
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuRetrieval();
}

void CCBIRLabDlg::OnBnClickedButtonFolder()
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuFolder();
}
