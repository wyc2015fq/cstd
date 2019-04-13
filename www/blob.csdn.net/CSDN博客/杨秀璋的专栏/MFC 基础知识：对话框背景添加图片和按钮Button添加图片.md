
# MFC 基础知识：对话框背景添加图片和按钮Button添加图片 - 杨秀璋的专栏 - CSDN博客

2015年04月27日 02:41:40[Eastmount](https://me.csdn.net/Eastmount)阅读数：12913所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)



很长时间没有接触MFC相关的知识了，我大概是在大二时候学习的MFC相关知识及图像处理，现在由于要帮个朋友完成个基于C++的程序，所以又回顾了下相关知识。的确，任何知识一段时间过后都比较容易忘记，但回顾起来还是很有印象的。
这篇文章主要是回顾以前的MFC基础知识，给对话框添加背景图片和给按钮button添加背景图片；希望此篇基础性文章对大家有所帮助！同时为下次做MFC相关知识提供点此时所想所感吧。内容比较简单，高手飘过~
## 一. 对话框背景添加图片
首先通过VS2012创建MFC对话框应用程序,项目名为RecoveryMovie；然后在资源视图中打开Dialog，即IDD_RECOVERYMOVIE_DIALOG，再编辑对话框删除默认内容。
第二步右键“资源视图”中的项目名称，添加资源。
![](https://img-blog.csdn.net/20150427014119836)

第三步添加选择Bitmap导入图片，图片文件最好放在项目res目录中，同时是BMP格式图片。上传后的图片如下图所示IDB_BITMAP1。
![](https://img-blog.csdn.net/20150427014408052)![](https://img-blog.csdn.net/20150427014739719)
第四步打开“解决方案”中XXXDlg.cpp文件（该对话框对应的源文件），找到OnPaint()函数，修改else部分代码，如下：
```python
else
	{
		//注释该语句：防止重复调用重画函数
		//CDialogEx::OnPaint();
		//添加代码对话框背景贴图
		CPaintDC   dc(this);   
		CRect   rect;   
		GetClientRect(&rect);                                 //获取对话框长宽       
		CDC   dcBmp;                                           //定义并创建一个内存设备环境
		dcBmp.CreateCompatibleDC(&dc);                         //创建兼容性DC
		CBitmap   bmpBackground;   
		bmpBackground.LoadBitmap(IDB_BITMAP1);                 //载入资源中的IDB_BITMAP1图片
		BITMAP   m_bitmap;                                     //图片变量                
		bmpBackground.GetBitmap(&m_bitmap);                    //将图片载入位图中
		CBitmap   *pbmpOld=dcBmp.SelectObject(&bmpBackground); //将位图选入临时内存设备环境  
		//调用函数显示图片 StretchBlt显示形状可变
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,
			m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);   	
		/******************************************************/
		/** StretchBlt()                                     **/
		/** 参数x、y位图目标矩形左上角x、y的坐标值 居中      **/
		/** nWidth、nHeigth位图目标矩形的逻辑宽度和高度      **/
		/** pSrcDC表示源设备CDC指针                          **/
		/** xSrc、ySrc表示位图源矩形的左上角的x、y逻辑坐标值 **/
		/** dwRop表示显示位图的光栅操作方式                  **/
		/** SRCCOPY用于直接将位图复制到目标环境中            **/
		/******************************************************/
	}
```
![](https://img-blog.csdn.net/20150427015257376)

此时运行程序即可显示背景图片，同时你拉伸对话框时可能会出现闪动Bug。
因此你可以设置Dialog的属性Border为Dialog Frame(对话框外框)，默认的Resizing是可以调控大小的。
注意：如果图像出现失贞，添加代码如下：
```python
dc.SetStretchBltMode(COLORONCOLOR);// 若不设置这个模式，图片会严重失真
dc.StrtchBlt....
```

## 二. 按钮BUTTON添加背景图片
首先在“资源视图”中给Dialog添加3个按钮，分别为IDC_BUTTON1、IDC_BUTTON2和IDC_BUTTON3，如下图所示：
![](https://img-blog.csdn.net/20150427020606978)
第二步按照前面的方法添加资源：按钮背景图片，即“播放视频”图片。添加后在资源视图中选中图片资源，点击属性可以修改ID值。如下图所示，把IDB_BITMAP2修改为IDB_StartMovie1。
![](https://img-blog.csdn.net/20150427020927775)![](https://img-blog.csdn.net/20150427021050508)
第三步通过快捷键（Ctrl+Shift+X或视图->类向导）打开“类向导”，选择类名为CRecoverMovieDlg，在成员变量中点击添加变量，如下图添加3个按钮的成员变量。
![](https://img-blog.csdn.net/20150427021629949)
![](https://img-blog.csdn.net/20150427021726390)


此时你会发现在XXXDlg.cpp中函数DoDataExchange()中会自动增加下面DDX_Control()的代码：
```python
void CRecoveryMovieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonstart);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonfix);
	DDX_Control(pDX, IDC_BUTTON3, m_buttonexit);
}
```
第四步在XXXDlg.cpp中找到初始化函数OnInitDialog()，在“// TODO:在此添加额外的初始化代码”中添加如下代码：
```python
// TODO: 在此添加额外的初始化代码
	//初始化窗口 设置窗口为500*500大小
	MoveWindow(0,0,500,500);  
	/*******************************************************************************/
	/* (重点知识:实现按钮位置设置)                                                 */
	/*    1.使用Ctrl+Shift+X打开类资源导向 增加开始\结束按钮的成员变量 Button型    */
	/*    2.使用函数SetWindowPos()设置两个按钮的位置                               */
	/*******************************************************************************/
	/*******************************************************************************/
	/* SetWindowPos()函数                                                          */
	/*		参数:const CWnd* pWndInsertAfter,int x,int y,int cx,int cy,UINT nFlags */
	/*      hwnd是窗口的句柄，x、y、cx、cy分别是窗口的x和y坐标、宽和高度           */
	/*      hWndInsertAfter用来指定窗口的Z位置,代表深度.这个参数接受5种值:         */
	/*  HWND_BOTTOM、 HWND_NOTOPMOST、HWND_TOP、HWND_TOPMOST或者另一个窗口的句柄   */
	/*      wFlags用来指定附加的选项                                               */
	/*******************************************************************************/
	//窗口句柄设置为NULL SWP_SHOWWINDOW显示窗口  SWP_NOSIZE保持当前大小忽略cx\xy故设置为0
	m_buttonstart.SetWindowPos(NULL,20,280,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);   
	m_buttonfix.SetWindowPos(NULL,20,325,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);  
	m_buttonexit.SetWindowPos(NULL,20,370,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);  
	//设置按钮属性 点击按钮背景图片变换
	m_buttonstart.LoadBitmaps(IDB_StartMovie1,IDB_StartMovie2);
	m_buttonfix.LoadBitmaps(IDB_StartMovie1,IDB_StartMovie2);
	m_buttonexit.LoadBitmaps(IDB_StartMovie1,IDB_StartMovie2);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
```
此时你运行程序会出现如下错误：Error class "CButton"没有成员“LoadBitmaps”。函数LoadBitmaps()的功能是点击按钮实现两张背景图片的替换。
![](https://img-blog.csdn.net/20150427022639692)

第五步需要在XXXDlg.h头文件中把public成员变量CButton全部修改为CBitmapButton，如下图为修改第一个变量所示：
![](https://img-blog.csdn.net/20150427022828246)
```python
// CRecoveryMovieDlg 对话框
class CRecoveryMovieDlg : public CDialogEx
{
// 构造
public:
	CRecoveryMovieDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_RECOVERYMOVIE_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
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
	CBitmapButton m_buttonstart;
	CBitmapButton m_buttonfix;
	CBitmapButton m_buttonexit;
};
```
第六步：此时虽然你能运行程序，但是按钮背景依然没有，因为你还需要分别设置三个按钮的属性“Owner Draw”为True，它指定按钮为所有者描述的按钮。
运行结果如下图所示：
![](https://img-blog.csdn.net/20150427023540296)![](https://img-blog.csdn.net/20150427023454472)
最后希望文章对大家有所帮助，如果文章中有不足之处或错误的地方，还请海涵！个人推荐初学者尤其很多老师布置作业需要用MFC完成的可以学习，没有什么语言流行，它们都是贯通的，都需要一步一个脚印的去学习。
（By:Eastmount 2015-4-27 深夜3点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）



