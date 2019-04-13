
# MFC 列表控件CListCtrl加载类似QQ界面的头像与文字 - 杨秀璋的专栏 - CSDN博客

2014年02月28日 02:38:14[Eastmount](https://me.csdn.net/Eastmount)阅读数：8518所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)



**本文主要讲述使用VS2012 MFC的CListCtrl(列表)控件加载类似于QQ界面的图片与文字数据.主要通过CListCtrl::InsertColumn、InsertItem、SetItemText向列表插入数据,CImageList插入BMP图片或ICON图标.**
## 一.创建项目及界面
**1.创建项目**
**创建项目名为"QQList"的基于MFC对话框的应用程序.资源视图中对话框****IDD_QQLIST_DIALOG****删除"TODE"和按钮,并添加****List Control控件****.**
**2.添加成员变量**
**项目->类向导->类名为"CQQListDlg"->成员变量点击"IDC_LIST1"添加成员变量public(访问) CListCtrl(变量类型) m_list(变量名).同时添加成员变量CImageList m_imList调用其函数实现添加图片.**
**3.插入3张BMP图片**
**右键"资源视图"中"Dialog"->选择"添加资源"->点击"Bitmap"->导入本地图片3张(工程文件res文件夹中).其ID分别为IDB_BITMAP1,IDB_BITMAP2,IDB_BITMAP3.**
**注意:在加载图片时,已经使用画图把图片修改为64*64像素的24位彩色BMP图片,在后面对应的m_imList.Create(64,64,ILC_COLOR24,10,20)函数即加载它.倘若选择ILC_COLOR或32*32会出现几位黑白色或大小失真现象.**
4.**添加自定义函数**
**在"类向导"中向CQQListDlg中添加方法void CQQListDlg::InsertPicAndWord(void)该函数即实现添加图片和文字功能.**
**其中QQListDlg.h中的代码如下图所示:**
```python
public:
	//成员变量
	CListCtrl m_list;            // 列表控件 IDC_LIST1
	CImageList m_imList;         // 图像列表(相同大小图像集合)
public:
	//成员函数
	void InsertPicAndWord(void); // 实现插入图片和文字
```
## 二.源代码
**其中主代码void CQQListDlg::InsertPicAndWord(void)如下:**
```python
// 调用该函数实现向CListCtrl中加载图片和文字
void CQQListDlg::InsertPicAndWord(void)
{
	// 设置CListCtrl控件扩展风格:整行选中\绘制表格\子项目图标列表
	DWORD dwStyle; 
	dwStyle = m_list.GetExtendedStyle();  
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;
	m_list.SetExtendedStyle(dwStyle);   
	
	// 载入64*64像素 24位真彩(ILC_COLOR24)图片
	m_imList.Create(64,64,ILC_COLOR24,10,20);    // 创建图像序列CImageList对象
	CBitmap * pBmp = NULL;
	pBmp = new CBitmap(); 
	pBmp->LoadBitmapW(IDB_BITMAP1);              // 载入位图IDB_BITMAP1
	m_imList.Add(pBmp,RGB(0,0,0));  
	delete pBmp;  
	//
	pBmp = new CBitmap();  
    pBmp->LoadBitmapW(IDB_BITMAP2);  
    m_imList.Add(pBmp,RGB(0,0,0));  
    delete pBmp;
	//
	pBmp = new CBitmap();  
    pBmp->LoadBitmapW(IDB_BITMAP3);  
    m_imList.Add(pBmp,RGB(0,0,0));  
    delete pBmp;
	// 设置CImageList图像列表与CListCtrl控件关联 LVSIL_SMALL小图标列表
	m_list.SetImageList(&m_imList,LVSIL_SMALL);
	// 向列表视图控件InsertColumn插入3列数据 
	CRect mRect;
	m_list.GetWindowRect(&mRect);                     // 获取控件矩形区域
	int length = mRect.Width();
	m_list.InsertColumn(0, _T("用户头像"), LVCFMT_CENTER, length / 4, -1);
	m_list.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, length / 4, -1);
	m_list.InsertColumn(2, _T("QQ格言"), LVCFMT_CENTER, length / 2, -1);
	
	// 添加数据 InsertItem向列表中插入主项数据 SetItemText向列表中的子项写入数据
	int nRow;                                        // 记录行号 
	LVITEM lvItem={0};                               // 列表视图控 LVITEM用于定义"项"的结构
	//第一行数据
 	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // 文字、图片、状态
	lvItem.iItem = 0;                                // 行号(第一行)
	lvItem.iImage = 0;                               // 图片索引号(第一幅图片 IDB_BITMAP1)
	lvItem.iSubItem = 0;                             // 子列号
	nRow = m_list.InsertItem(&lvItem);               // 第一列为图片
	m_list.SetItemText(nRow,1,_T("dog"));            // 第二列为名字
	m_list.SetItemText(nRow,2,_T("人生在于奋斗"));     // 第三列为格言
	//第二行数据
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // 文字、图片、状态
	lvItem.iItem = 1;                                // 行号(第二行)
	lvItem.iImage = 1;                               // 图片索引号(第二幅图片 IDB_BITMAP2)
	lvItem.iSubItem = 0;                             // 子列号
	nRow = m_list.InsertItem(&lvItem);     
	m_list.SetItemText(nRow,1,_T("cat"));  
	m_list.SetItemText(nRow,2,_T("快乐生活每一天"));  
	//第二行数据
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // 文字、图片、状态
	lvItem.iItem = 2;                                // 行号(第三行)
	lvItem.iImage = 2;                               // 图片索引号(第三幅图片 IDB_BITMAP3)
	lvItem.iSubItem = 0;                             // 子列号
	nRow = m_list.InsertItem(&lvItem);      
	m_list.SetItemText(nRow,1,_T("pig"));   
	m_list.SetItemText(nRow,2,_T("Eastmount制作"));   
}
```
**同时需要在初始化程序时调用该函数即可实现:**
```python
BOOL CQQListDlg::OnInitDialog()
{
	...
	// TODO: 在此添加额外的初始化代码
	InsertPicAndWord();      // 调用该函数实现加载图片和文字
	return TRUE;             // 除非将焦点设置到控件，否则返回 TRUE
}
```
**注意:现在可能你运行后会遇到一个错误就是SetItemText、InsertColum加载的数据和图片不能显示.Why?**
**那是需要设置CListCtrl控件IDC_LIST1属性View为Report风格.ListCtrl有四种风格:**
**Icons(图标)使用32*32大小图标显示列表**
**Small Icons(小图标)使用16*16大小图标显示单一列表**
**List(列表)使用****Small Icons,****可有多列,第二列必须填充之前必须填充第一列,自上而下排序**
**Report(报表)编程者自定义的格式显示列表**
**如果程序中想要在加载ICON图标,则代码为:**
```python
HICON hIcon;  
m_imList.Create(32,32,ILC_COLOR24,10,20);  
hIcon = AfxGetApp()->LoadIconW(IDI_ICON_GREE);  
m_imList.Add(hIcon);
```
**程序运行结果:**
![](https://img-blog.csdn.net/20140228022152937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**希望****文章能够对大家有所帮助,同时如果文章中有错误或不足之处,还请大家海涵.同时感谢文章资料的作者和向我提出该问题的那个同学.****(By:Eastmount 2014-2-28 夜2点 原创:****http://blog.csdn.net/eastmount****)**
**下载源代码地址:****[http://download.csdn.net/detail/eastmount/6972035](http://download.csdn.net/detail/eastmount/6972035)**
**参考资料及在线笔记:**
**(1).****ListCtrl控件,Report风格列表中添加图片 - ribut9225(主要讲述加载图片)**
**http://blog.csdn.net/ribut9225/article/details/6905165**
**(2).VC****MFC列表视图(CListCtrl)控件 - 无尽的路(讲述CListCtrl加载图文字 附图)**
**http://hi.baidu.com/3582077/item/38858bb484e11477254b090a**
**(3).CListCtrl****基础用法 - welkangm(详细讲述CListCtrl概述、加载数据 获取行号 删除行)**
**http://welkangm.blog.163.com/blog/static/190658510201232032336899/**
**(4).CListCtrl****加载图片技术 - woshishushangdezhu(可参考加载动态路径下的图片F:\\)**
**http://blog.csdn.net/woshishushangdezhu/article/details/3938433**
**(5****).如何更改CListCtrl每行的高度(可参考****rwx_tech的回答设置行高度****)**
**http://bbs.csdn.net/topics/60367436**

