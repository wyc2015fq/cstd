
# 【数字图像处理】三.MFC实现图像灰度、采样和量化功能详解 - 杨秀璋的专栏 - CSDN博客

2015年05月28日 15:37:40[Eastmount](https://me.csdn.net/Eastmount)阅读数：17186所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)



本文主要讲述基于VC++6.0 MFC图像处理的应用知识，主要结合自己大三所学课程《数字图像处理》及课件进行讲解，主要通过MFC单文档视图实现显示BMP格式图片，并通过Bitmap进行灰度处理、图片采样和量化功能。
个人认为对初学者VC++6.0可能还是很值得学习的工具，所以采用它来讲解，而不是VS或C\#。同时文章比较详细基础，希望该篇文章对你有所帮助~
[【数字图像处理】一.MFC详解显示BMP格式图片](http://blog.csdn.net/eastmount/article/details/18238863)
[【数字图像处理】二.MFC单文档分割窗口显示图片](http://blog.csdn.net/eastmount/article/details/18987539)
免费资源下载地址：
[http://download.csdn.net/detail/eastmount/8748403](http://download.csdn.net/detail/eastmount/8748403)

## 一. 单文档显示BMP图片
第一步：新建项目"MFC AppWizard(exe)"，项目名为ImageProcessing，在应用程序类型中选择"单个文档"，点击"确定"。在左栏的"资源视图"中，点击"Menu->IDR_MAINFRAM"可以查看并修改菜单视图。
![](https://img-blog.csdn.net/20150526174712506)
第二步：向CImageProcessingView类添加成员变量和成员函数。在右栏的"类视图"右键ImageProcessingView添加函数或直接在ImageProcessingView.h中直接添加public成员变量和成员函数。添加代码如下：
```python
// Implementation
public:
	//添加成员函数
	void ShowBitmap(CDC* pDC,CString BmpName); //显示位图函数
	//添加成员变量
	CString EntName;     //图像文件扩展名
	CString BmpName;     //图像文件名称
	CBitmap m_bitmap;    //创建位图对象
```
同时采用类视图添加后，会自动在XXXView.h中添加函数定义，在XXXView.cpp中添加函数实现代码。
![](https://img-blog.csdn.net/20150526181038908)
![](https://img-blog.csdn.net/20150526181050052)
第三步：编辑ImageProcessingView.cpp中ShowBitmap()函数。通过它显示BMP图片，其中代码及详细注释如下：
```python
//****************显示BMP格式图片****************//
void CImageProcessingView::ShowBitmap(CDC *pDC, CString BmpName)
{
	//定义bitmap指针 调用函数LoadImage装载位图
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP) LoadImage(NULL,BmpName,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
		
	/*************************************************************************/
	/* 1.要装载OEM图像，则设此参数值为0  OBM_ OEM位图 OIC_OEM图标 OCR_OEM光标
	/* 2.BmpName要装载图片的文件名                  
	/* 3.装载图像类型: 
	/*   IMAGE_BITMAP-装载位图 IMAGE_CURSOR-装载光标 IMAGE_ICON-装载图标    
	/* 4.指定图标或光标的像素宽度和长度 以像素为单位    
	/* 5.加载选项:
	/*   IR_LOADFROMFILE-指明由lpszName指定文件中加载图像
	/*   IR_DEFAULTSIZE-指明使用图像默认大小
	/*   LR_CREATEDIBSECTION-当uType参数为IMAGE_BITMAP时,创建一个DIB项
	/**************************************************************************/
	if( m_bitmap.m_hObject )
	{
		m_bitmap.Detach();           //切断CWnd和窗口联系
	}
	m_bitmap.Attach(m_hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联
	
	//边界
	CRect rect;
	GetClientRect(&rect);
	//图片显示(x,y)起始坐标
	int m_showX=0;
	int m_showY=0;
	int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度
	int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度
		
	//定义并创建一个内存设备环境DC
	CDC dcBmp;
	if( !dcBmp.CreateCompatibleDC(pDC) )   //创建兼容性的DC
		return;
	BITMAP m_bmp;                          //临时bmp图片变量
	m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中
	CBitmap *pbmpOld = NULL;      
	dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境
	
	//图片显示调用函数stretchBlt
	pDC->StretchBlt(0,0,m_bmp.bmWidth,m_bmp.bmHeight,&dcBmp,0,0,
		m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY);
	
	/*******************************************************************************/
	/* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC, 
	/*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop );
	/* 1.参数x、y位图目标矩形左上角x、y的坐标值      
	/* 2.nWidth、nHeigth位图目标矩形的逻辑宽度和高度      
	/* 3.pSrcDC表示源设备CDC指针                          
	/* 4.xSrc、ySrc表示位图源矩形的左上角的x、y逻辑坐标值 
	/* 5.dwRop表示显示位图的光栅操作方式 SRCCOPY用于直接将位图复制到目标环境中            
	/*******************************************************************************/
	
	dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图
	DeleteObject(&m_bitmap);               //删除内存中的位图
	dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC

	/**
	 * 面代码为后面显示第二张图片
	 */
}
```
第四步：设置打开BMP图片函数。"查看"->"建立类向导"(Ctrl+W)->选择"类名"CImageProcessing->在命令对象ID中双击"ID_FILE_OPEN"->自动生成默认成员函数OnFileOpen，消息为COMMAND。双击成员函数(Member
 Functions)进入函数编辑。
![](https://img-blog.csdn.net/20150526183109899)
编辑ImageProcessingView.cpp函数实现打开图片，代码如下：
```python
//****************打开文件****************//
void CImageProcessingView::OnFileOpen() 
{
	//两种格式的文件：bmp gif
    CString filter;  
    filter="所有文件(*.bmp,*.jpg,*.gif)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";  
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter,NULL);            
  
    //按下确定按钮 dlg.DoModal() 函数显示对话框  
    if( dlg.DoModal() == IDOK )  
    {  
        BmpName = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp  
        EntName = dlg.GetFileExt();      //获取文件扩展名  
        EntName.MakeLower();             //将文件扩展名转换为一个小写字符  
        Invalidate();                    //调用该函数就会调用OnDraw重绘画图  
    }  	
}
```
第五步：在ImageProcessingView.cpp中找到OnDraw()函数，通过OnDraw()函数调用ShowBitmap()函数显示图片。代码如下:
```python
void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if (!pDoc) return;  
    if( EntName.Compare(_T("bmp")) == 0 )      //bmp格式  
    {  
        ShowBitmap(pDC,BmpName);               //显示图片  
    }  
}
```
第六步：此时点击运行，同时点击文件-打开，即可显示图片如下图所示：
![](https://img-blog.csdn.net/20150526184325523)
PS:这是非常著名的一张图片莱娜图(Lenna)，全图是一张花花公子封面的裸图，后成为数字图像处理的标志图片。哈哈~至于BMP图片格式参照第一篇文章

## 二. 读取BMP图片和保存图片
BMP图片格式如下图所示：(参考[自己文库](http://wenku.baidu.com/view/c955d0fdfab069dc50220147.html))
![](https://img-blog.csdn.net/20150526195753621)
在很多处理中，都需要获取BMP图像的一些数据，如图像宽度、高度、像素大小等，后面的处理与之相关，主要的是ReadBmp函数。
第一步：在XXXView.h中添加BMP格式图像相关的成员变量和成员函数，其中成员函数通过类视图右键添加，成员变量可以在XXXView.h中直接复制。
```python
// Implementation
public:
	//添加成员函数
	void ShowBitmap(CDC* pDC,CString BmpName); //显示位图函数
	bool ReadBmp();                            //用来读取bmp个手机图片
	bool SaveBmp(LPCSTR lpFileName);           //用来保存bmp格式图片
	//添加成员变量
	CString EntName;     //图像文件扩展名
	CString BmpName;     //图像文件名称
	CBitmap m_bitmap;    //创建位图对象
	int	m_nWidth;		//图像实际宽度
	int	m_nHeight;		//图像实际高度
	int	m_nDrawWidth;	//图像显示宽度
	int	m_nDrawHeight;	//图像显示高度
	DWORD m_nImage;		//图像数据的字节数 只含位图
	DWORD m_nSize;      //图像文件大小
	int m_nLineByte;    //图像一行所占字节数
	int	m_nBitCount;    //图像每个像素所占位数
	int	m_nPalette;     //位图实际使用的颜色表中的颜色数
	
	BYTE *m_pImage;         //读入图片数据后的指针
	BITMAPFILEHEADER bfh;   //全局变量文件头
	BITMAPINFOHEADER bih;   //全局变量信息头
	RGBQUAD m_pPal;         //颜色表指针
```
第二步：在ImageProcessingView.cpp中实现ReadBmp函数和SaveBmp函数。
```python
//***************读取图片数据*************//
bool CImageProcessingView::ReadBmp()
{
	//图片读出存储其中的东西
	FILE *fp = fopen(BmpName,"rb");
	if(fp==0)
	{		
		AfxMessageBox("无法打开文件!",MB_OK,0);
		return 0; 
	}
	//读取文件头 解决BMP格式倒置的方法
	fread(&bfh.bfType,sizeof(WORD),1,fp);
	fread(&bfh.bfSize,sizeof(DWORD),1,fp);
	fread(&bfh.bfReserved1,sizeof(WORD),1,fp);
	fread(&bfh.bfReserved2,sizeof(WORD),1,fp);
	fread(&bfh.bfOffBits,sizeof(DWORD),1,fp);
	//图像文件的总字节数
	m_nSize = bfh.bfSize;
	//判断是否是bmp格式图片
	if(bfh.bfType!=0x4d42)   //'BM'
	{
		AfxMessageBox("不是BMP格式图片!",MB_OK,0);
		return 0;
	}
	//读取信息头
	fread(&bih.biSize,sizeof(DWORD),1,fp);
	fread(&bih.biWidth,sizeof(LONG),1,fp);
	fread(&bih.biHeight,sizeof(LONG),1,fp);
	fread(&bih.biPlanes,sizeof(WORD),1,fp);
	fread(&bih.biBitCount,sizeof(WORD),1,fp);
	fread(&bih.biCompression,sizeof(DWORD),1,fp);
	fread(&bih.biSizeImage,sizeof(DWORD),1,fp);
	fread(&bih.biXPelsPerMeter,sizeof(LONG),1,fp);
	fread(&bih.biYPelsPerMeter,sizeof(LONG),1,fp);
	fread(&bih.biClrUsed,sizeof(DWORD),1,fp);
	fread(&bih.biClrImportant,sizeof(DWORD),1,fp);
	if(bih.biSize!=sizeof(bih))
	{
		AfxMessageBox("本结构所占用字节数出现错误");
		return 0;
	}
	//位图压缩类型，必须是 0（不压缩） 1（BI_RLE8压缩类型）或2（BI_RLE压缩类型）之一
	if(bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox("位图被压缩!");
		return 0;
	}
	//获取图像高宽和每个像素所占位数
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //每个像素所占位数
	//计算图像每行像素所占的字节数（必须是32的倍数）
	m_nLineByte = (m_nWidth*m_nBitCount+31)/32*4;
	//图片大小 调用系统自带的文件头 BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih; 
	//否则用 BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;要 m_nImage = m_nLineByte * m_nHeight - 2;
	m_nImage = m_nLineByte * m_nHeight;
	//位图实际使用的颜色表中的颜色数 biClrUsed
	m_nPalette = 0;                       //初始化
	if(bih.biClrUsed)
		m_nPalette = bih.biClrUsed;
	//申请位图空间 大小为位图大小 m_nImage
	//malloc只能申请4字节的空间 （未知）
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fp);
	fclose(fp);
	return true;
}
```
其中SaveBmp()函数代码如下：
```python
//****************保存文件****************//
bool CImageProcessingView::SaveBmp(LPCSTR lpFileName) //lpFileName为位图文件名
{
	//保存bmp格式图片 写图片过程 只处理24像素的图片 该图片无调色板
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(lpFileName,"wb");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	//malloc只能申请4字节的空间 （未知）
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	return true;
}
```
第三步：添加保存menu控件和函数。点击”查看-建立类向导“，在ID列表中找到ID_FILE_SAVE，点击COMMAND(Message列表)，双击添加默认成员函数OnFileSave，同时在Member
 Functions(成员函数)中双击该函数进入函数并编辑。添加如下代码：
```python
//******************文件保存*****************//
void CImageProcessingView::OnFileSave() 
{
	// TODO: Add your command handler code here
	CString filter;
	filter="所有文件(*.bmp,*.jpg)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";
	//重点: 1-文件打开 0-文件保存
	CFileDialog dlg(0,NULL,NULL,OFN_HIDEREADONLY,filter,NULL);   
	//按下确定按钮
	if( dlg.DoModal() == IDOK ) {
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();           //获取文件的路径
		filename = dlg.GetFileTitle();     //获取文件名
		int nFilterIndex=dlg.m_ofn.nFilterIndex;
		if( nFilterIndex == 2 )            //当用户选择文件过滤器为".BMP"时
		{
			str = str + ".bmp";	           //自动加扩展名.bmp
			SaveBmp(str);                  //保存bmp图片 就是一个写出图片的过程 
			AfxMessageBox("图片保存成功",MB_OK,0);
		}
	}
}
```
第四步：在XXXView.cpp中OnDraw()函数中调用读取图片函数。
if( EntName.Compare(_T("bmp"))
 == 0 )      //bmp格式
{
**ReadBmp();**
ShowBitmap(pDC,BmpName);               //显示图片
}
运行程序，打开图片点击保存即可实现。重点是ReadBmp获取一些重要参数。
![](https://img-blog.csdn.net/20150526203022502)






## 三. 图像灰度处理
(参考我的百度文库：[http://wenku.baidu.com/view/3b869230f111f18583d05a43](http://wenku.baidu.com/view/3b869230f111f18583d05a43))

### 1.灰度图像概念
什么叫灰度图？任何颜色都有红、绿、蓝三原色组成，假如原来某点的颜色为RGB(R，G，B)，那么我们可以通过下面几种方法，将其转换为灰度：
浮点算法：Gray=R*0.3+G*0.59+B*0.11
整数方法：Gray=(R*30+G*59+B*11)/100
移位方法：Gray=(R*28+G*151+B*77)&gt;&gt;8;
平均值法：Gray=（R+G+B）/3;(此程序采用算法)
仅取绿色：Gray=G；
通过上述任一种方法求得Gray后，将原来的RGB(R,G,B)中的R,G,B统一用Gray替换，形成新的颜色RGB(Gray,Gray,Gray)，用它替换原来的RGB(R,G,B)就是灰度图了。
改变象素矩阵的RGB值，来达到彩色图转变为灰度图
加权平均值算法：根据光的亮度特性，其实正确的灰度公式应当是：
R=G=B=R*0.299+G*0.587+B0.144
为了提高速度我们做一个完全可以接受的近似，公式变形如下：R=G=B=(R*3+G*6+B)/10
真正的24位真彩图与8位的灰度图的区别就在于，真彩图文件中没有调色板，灰度图有调色板，真彩图中的象素矩阵是RGB值，灰度图中的象素矩阵是调色板索引值。源代码只简单的改变象素矩阵的RGB值，来达到彩色图转为灰度图，并没有添加调色板；该程序未实现添加了调色板。


### 2.灰度处理源码
第一步：在前面的代码基础上继续，先在ImageProcessingView.h中添加成员变量m_bitmaplin和BmpNameLin，因为后面处理操作是处理备份文件与原图进行比较。
```python
// Implementation
public:
	//添加成员函数
	void ShowBitmap(CDC* pDC,CString BmpName); //显示位图函数
    bool ReadBmp();                       //用来读取bmp个手机图片
    bool SaveBmp(LPCSTR lpFileName);           //用来保存bmp格式图片
	//添加成员变量
	CString EntName;     //图像文件扩展名
	CString BmpName;     //图像文件名称
	CBitmap m_bitmap;    //创建位图对象
	CBitmap m_bitmaplin;   //创建临时位图对象进行处理
	CString BmpNameLin;    //保存图像副本文件
```
第二步：在ImageProcessingView.cpp中ShowBitmap()函数前添加变量numPicture和level。
```python
/*************************************************************/
/* numPicture变量显示图片数量
/* 0-提示错误或未打开图片 1-显示一张图片 2-显示两张图片和处理
/*************************************************************/
int numPicture = 0;
/*************************************************************/
/* level变量显示具体的处理操作，每个处理函数中赋值该变量
/* 0-显示2张图片 1-显示灰度图片 3-显示图片采样
/* 2 4 8 16 32 64-不同量化等级量化图片
/*************************************************************/ 
int level = 0;    
//****************显示BMP格式图片****************//
void CImageProcessingView::ShowBitmap(CDC *pDC, CString BmpName)
{
    ....
}
```
第三步：修改ImageProcessingView.cpp中OnFileOpen()函数，添加临时变量名和显示一张图片标志变量。代码如下：
```python
//****************打开文件****************//
void CImageProcessingView::OnFileOpen() 
{
    CString filter;  
    filter="所有文件(*.bmp,*.jpg,*.gif)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";  
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter,NULL);             
    if( dlg.DoModal() == IDOK )  
    {  
        BmpName = dlg.GetPathName();     
        BmpNameLin = "picture.bmp";      //临时变量名
		numPicture=1;                    //显示一张图片
		EntName = dlg.GetFileExt();       
        EntName.MakeLower();              
        Invalidate();                     
    }  	
}
```
第四步：将视图切换到ResourceView界面，选中Menu->在IDR_MAINFRAME中添加菜单”显示“，双击它在菜单属性中选择”弹出“。在”显示“的子菜单中添加：
双图显示--ID_SHOW_TWO(ID)--默认属性
灰度图片--ID_SHOW_HD(ID)--默认属性
![](https://img-blog.csdn.net/20150526191838724)
第五步：点击"查看"->"建立类向导"(Ctrl+W)，选择CImageProcessing类，然后ID_SHOW_TWO，双击COMMAND(Message)，生成默认成员函数。
![](https://img-blog.csdn.net/20150526192610151)
在XXXView.cpp中实现OnShowTwo()函数，代码如下：


```python
//****************显示两张图片****************//
void CImageProcessingView::OnShowTwo() 
{
	//如果没有导入图片直接点击双显 提示错误
	if(numPicture==0)
	{
		AfxMessageBox("载入图片后才能显示2张图片!");
		return;
	}
	AfxMessageBox("显示两张图片!",MB_OK,0);
	numPicture = 2;    //全局变量 显示两图
	level =0;          //level=0双显
	Invalidate();      //调用Invalidate 每秒调用一次OnDraw画图
}
```
第六步：同上面相同的方法，"查看"->”建立类向导“->ID_SHOW_HD(ID)->COMMAND(Message)，默认成员函数名。在XXXView.cpp添加代码如下：
```python
/********************************************************************************************/
/* 祥见http://blog.csdn.net/xiakq/article/details/2956902有详细的灰度算法                   
/* 其中24位的图片灰度时,采用如下算法:                                                       
/* 1.平均值算法 R=G=B=(R+G+B)/3                                                              
/* 2.快速算法 R=G=B=(R+G+B+128)/4>>2                                                          
/* 3.加权平均值算法 根据光的亮度特性,其实正确的灰度公式应当是R=G=B=R*0.299+G*0.587+B0.144   
/*   为了提高速度我们做一个完全可以接受的近似，公式变形如下 R=G=B=(R*3+G*6+B)/10            
/* 4.精确加权平均值算法 R=G=B=R*0.299+G*0.587+B0.144                                        
/********************************************************************************************/
//**灰度图像就是 R=G=B且为三者的1/3 level=1时灰度图像**//
void CImageProcessingView::OnShowHd() 
{
	if(numPicture==0)
	{
		AfxMessageBox("载入图片后才能灰度图片!",MB_OK,0);
		return;
	}
	AfxMessageBox("灰度图像!",MB_OK,0);
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	//读取文件
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	//灰度图像
	unsigned char color;
	unsigned char red,green,blue;
	/********************************************************************/
	/* 注意：原来下面所有操作都是for( i=0; i<m_nWidth*m_nHeight; i++ )  
	/* 后发现如果图片最后一行没有完整的一行数据，会出现图像变多或变少   
	/* 但图像的总像素为m_nImage，如果是m_nImage/3就可以保证所有像素都有 
	/********************************************************************/
	for(int i=0; i < m_nImage/3; i++ )
	{
		fread(&red,sizeof(char),1,fpo);
		fread(&green,sizeof(char),1,fpo);
		fread(&blue,sizeof(char),1,fpo);
		color=(red+green+blue)/3;
		red=color;
		green=color;  
		blue=color;
		fwrite(&red,sizeof(char),1,fpw);
		fwrite(&green,sizeof(char),1,fpw);
		fwrite(&blue,sizeof(char),1,fpw);
	}
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=1;
	Invalidate();
}
```
第七步：修改ShowBitmap()函数中双显部分，添加如下代码：
```python
//****************显示BMP格式图片****************//
void CImageProcessingView::ShowBitmap(CDC *pDC, CString BmpName)
{
        ....
	/**
	 * 面代码为后面显示第二张图片
	 */
	if(numPicture==2) {
		//显示图片函数LoadImage
		HBITMAP m_hBitmapChange;
		if(level==0) //显示2张图 BmpNameLin原图
		{
			m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpName,IMAGE_BITMAP,0,0,
				LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
		}
		else
		if(level==1) //灰度图片 BmpNameLin临时图片
		{
			m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
				LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
		}
		if( m_bitmap.m_hObject ) {
			m_bitmap.Detach();            //m_bitmap为创建的位图对象
		}
		m_bitmap.Attach(m_hBitmapChange);
		//定义并创建一个内存设备环境
		CDC dcBmp;
		if( !dcBmp.CreateCompatibleDC(pDC) )   //创建兼容性的DC
			return;
		BITMAP m_bmp;                          //临时bmp图片变量
		m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境
		//如果图片太大显示大小为固定640*640 否则显示原图大小
		if(m_nDrawWidth<650 && m_nDrawHeight<650)
			pDC->StretchBlt(m_nWindowWidth-m_nDrawWidth,0,
				m_nDrawWidth,m_nDrawHeight,&dcBmp,0,0,m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY);
		else
			pDC->StretchBlt(m_nWindowWidth-640,0,640,640,&dcBmp,0,0,
				m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY); 
		//恢复临时DC的位图
		dcBmp.SelectObject(pbmpOld);           
	}
}
```
双显和灰度运行效果如下图所示：
![](https://img-blog.csdn.net/20150526210008673)


## 四. 图片量化处理
（参考我的文库：[http://wenku.baidu.com/view/80b18961f5335a8102d220a0](http://wenku.baidu.com/view/80b18961f5335a8102d220a0)）
### 1.量化基本概念
图像数字化包括量化和取样两个过程，其中：
量化：幅值f(x,y)的离散化，f(x,y)表示静止灰度图像的空间坐标
取样：对空间连续坐标(x,y)的离散化
一幅行数为*M*、列数为*N*的图像大小为*M*×*N*的矩阵形式为：(其中矩阵中每个元素代表一个像素)
![](https://img-blog.csdn.net/20150528125657313)
该工程所有的处理都基于24位的bmp格式图片的处理，24为表示biBitCount=24，1个像素占3个字节(red、green、blue)。
![](https://img-blog.csdn.net/20150528124358924)
如图量化级不同产生的灰度也不同，量化是使连续信号的幅度用有限级的数码表示的过程。
量化等级=2：使用2种灰度级(0~255)表示图片，小于128的取0，大于等于128的取128。把位图数据块所有数据在临时图片中取值，在显示即可。
量化等级=4：使用4种灰度级显示图片，就会发现图片分层为4种颜色。同时，0-64区间取0，64-128区间取64，128-192区间取128,192-255区间取192。
量化的取值各不相同，我采用的是最简单的取值。其它方法可自己去查阅资料。
![](https://img-blog.csdn.net/20150528125350317)
![](https://img-blog.csdn.net/20150528125420206)


### 2.量化处理源码
第一步：设置菜单栏。将试图切换到ResourceView界面--选中Menu--在IDR_MAINFRAME中添加菜单“量化”--双击它在菜单属性中选择“弹出”。在“显示”的子菜单中添加:属性为默认属性。
量化 Level 2--ID_LH_2       量化 Level 4--ID_LH_4
量化 Level 8--ID_LH_8       量化 Level 16--ID_LH_16
量化 Level 32--ID_LH_32   量化 Level 64--ID_LH_64
![](https://img-blog.csdn.net/20150528133053207)
第二步：建立类向导。查看->建立类导向(Ctrl+W)->CXXXView(类名)->ID_LH_2->COMMAND(Messages)->默认成员函数名。相同方法分别为量化等级2、4、8、16、32、64建立类导向。
![](https://img-blog.csdn.net/20150528133725736)
第三步：在ImageProcessingView.cpp中编辑灰度函数。代码如下：
核心流程是打开两张图片原图(BmpName)和临时图片(BmpNameLin)，然后读取原图信息头赋值给临时处理图片，在读取原图m_nImage整个像素矩阵，量化处理每个像素（即分等级量化），最后文件写量化后的像素矩阵给BmpNameLin，在赋值全局变量level\numPicture和调用Invalidate()重绘图像即可。
```python
//****************量化 量化等级为2****************//
void CImageProcessingView::OnLh2() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=2!",MB_OK,0);
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	//读取文件
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	//malloc只能申请4字节的空间
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	//等级2量化
	for(int i=0; i<m_nImage; i++ ) {
		//24位的为调色板为真彩图 Red Green Blue 为3字节 
		//量化等级为2取中间值为 64 和 192
		if(m_pImage[i]<128) { 
			m_pImage[i]=0;
		}
		else if(m_pImage[i]>=128) {
			m_pImage[i]=128;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=2;
	Invalidate();
}
//****************量化 量化等级为4****************//
void CImageProcessingView::OnLh4() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=4!",MB_OK,0);
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	//等级4量化
	for(int i=0; i<m_nImage; i++ ) {
		if(m_pImage[i]<64) {
			m_pImage[i]=0;
		}
		else if( (m_pImage[i]>=64) && (m_pImage[i]<128) ) {
			m_pImage[i]=64;
		}
		else if( (m_pImage[i]>=128) && (m_pImage[i]<192) ) {
			m_pImage[i]=128;
		}
		else if(m_pImage[i]>=192) {
			m_pImage[i]=192;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=4;
	Invalidate();
}
//****************量化 量化等级为8****************//
void CImageProcessingView::OnLh8() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=8!",MB_OK,0);
	//打开临时的图片 读取文件
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	//malloc只能申请4字节的空间 （未知）
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	//等级8量化
	for(int i=0; i<m_nImage; i++ ) {
		if(m_pImage[i]<32) {
			m_pImage[i]=0;
		}
		else if( (m_pImage[i]>=32) && (m_pImage[i]<64) ) {
			m_pImage[i]=32;
		}
		else if( (m_pImage[i]>=64) && (m_pImage[i]<96) ) {
			m_pImage[i]=64;
		}
		else if( (m_pImage[i]>=96) && (m_pImage[i]<128) ) {
			m_pImage[i]=96;
		}
		else if( (m_pImage[i]>=128) && (m_pImage[i]<160) ) {
			m_pImage[i]=128;
		}
		else if( (m_pImage[i]>=160) && (m_pImage[i]<192) ) {
			m_pImage[i]=160;
		}
		else if( (m_pImage[i]>=192) && (m_pImage[i]<224) ) {
			m_pImage[i]=192;
		}
		else if(m_pImage[i]>=224) {
			m_pImage[i]=224;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=8;
	Invalidate();
}
//****************量化 量化等级为16****************//
void CImageProcessingView::OnLh16() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=16!",MB_OK,0);
	int i,j;
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	for( i=0; i<m_nImage; i++ ) {
		j=16;
		while(j<=256)
		{
			if(m_pImage[i]<j) 
			{
				if(m_pImage[i]<16) 
					m_pImage[i]=0;
				else 
					m_pImage[i]=j-16;
				break;
			}
			else j+=16;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=16;
	Invalidate();
}
//****************量化 量化等级为32****************//
void CImageProcessingView::OnLh32() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=32!",MB_OK,0);
	int i,j;
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	//读取文件
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	//等级32化
	for( i=0; i<m_nImage; i++ )
	{
		j=8;
		while(j<=256)
		{
			if(m_pImage[i]<j) 
			{
				if(m_pImage[i]<8) 
					m_pImage[i]=0;
				else 
					m_pImage[i]=j-8;
				break;
			}
			else j+=8;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=32;
	Invalidate();
}
//****************量化 量化等级为64****************//
void CImageProcessingView::OnLh64() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能量化!",MB_OK,0);
		return;
	}
	AfxMessageBox("量化等级Level=64!",MB_OK,0);
	int i,j;
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	//读取文件
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	m_pImage=(BYTE*)malloc(m_nImage);
	fread(m_pImage,m_nImage,1,fpo);
	//等级64量化
	for( i=0; i<m_nImage; i++ )
	{
		j=4;
		while(j<=256)
		{
			if(m_pImage[i]<j) 
			{
				if(m_pImage[i]<16) 
					m_pImage[i]=0;
				else 
					m_pImage[i]=j-4;
				break;
			}
			else j+=4;
		}
	}
	fwrite(m_pImage,m_nImage,1,fpw);
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=64;
	Invalidate();
}
```
第四步：修改ShowBitmap()函数，显示量化处理。添加如下代码：
```python
if(level==0) //显示2张图 BmpNameLin原图
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpName,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else
if(level==1) //灰度图片 BmpNameLin临时图片
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化2
if(level==2)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化4
if(level==4)  
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化8
if(level==8)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化16
if(level==16)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化32
if(level==32)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
else         //量化64
if(level==64)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
```
运行效果如下图，当量化Level=2时很明显的两种灰度颜色，Level=4有4种颜色。
![](https://img-blog.csdn.net/20150528142627007)




## 五. 图像采样功能
(参考我的文库：[http://wenku.baidu.com/view/b3ef4e1f964bcf84b9d57baf](http://wenku.baidu.com/view/b3ef4e1f964bcf84b9d57baf))
### 1.图像采样概念
该工程所有的处理都基于24位的bmp格式图片的处理，24为表示biBitCount=24，1个像素占3个字节(red、green、blue)。如图一张512*512的原图，保持灰度级256不变后的各种采样。输入采样坐标：如16*16，它的含义是原图512*512像素，现在组成一个新的图片为16*16像素，(512/16=32,512/16=32)则每32*32组成一个新的区域。共有这种区域16*16个，采样的方法有2种：
a.把这个32*32区域全部赋值成左上角那个像素，这样图片的大小不变，困难在于赋值要4层循环。(项目中采用的就是这种方法)
b.把这个32*32区域的左上角取出来，组成一个新的图片，共有16*16个像素，这张图片的大小要变小，只有16*16个像素。但难点在于同时要把bmp文件头中的图片大小、信息头中的长宽像素改变、偏移量等信息更新。
![](https://img-blog.csdn.net/20150528143210285)
又如下图所示：
原图8*8的矩阵要处理成3*3的矩阵，则循环先处理第一二行，①②④⑤为3*3处理，去左上角的RGB，③⑥为2*3的处理；重点是原图读取一维数组需要转成二维数组赋值处理；最后再处理最后一行数据。采样中公式为：
//获取填充颜色 相当于一次读取一个像素的RGB值再乘3跳3个字节
red=m_pImage[(X+Y*m_nWidth)*3];
green=m_pImage[(X+Y*m_nWidth)*3+1];
blue=m_pImage[(X+Y*m_nWidth)*3+2];
//填出图像循环 小区域中的长宽循环
//(X+Y*m_nWidth)*3跳到该小区域 再赋值3*3小区域的RGB 同一区域RGB相同
m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=red; m++;
m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=green; m++;
m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=blue; m++;
![](https://img-blog.csdn.net/20150528143553233)
PS:难点是还未处理剩余部分的采样。


### 2.图像采样代码
第一步：设置菜单栏
a.将视图切换到ResourceView界面--选中Menu--在IDR_MAINFRAME中添加菜单“采样”--双击它在菜单属性中选择“弹出”；
b.在“采样”的子菜单中添加:属性为默认属性。ID_CY--图片采样。
c.建立类导向:查看--建立类导向(Ctrl+W)--CImageProcessingView(类名)--ID_CY--COMMAND(Messages)--默认成员函数名。生成void CImageProcessingView::OnCy()采样函数。
第二步：设置采样对话框
a.将试图切换到ResourceView界面--选中Dialog,右键鼠标新建一个Dialog，并新建一个名为IDD_DIALOG_CY。编辑框(X)IDC_EDIT_CYX 和 (Y)IDC_EDIT_CYY，确定为默认按钮。设置成下图对话框：
![](https://img-blog.csdn.net/20150528151301711)
b.在对话框资源模板空白区域双击鼠标—Create a new class创建一个新类--命名为CImageCYDlg。会自动生成它的.h和.cpp文件。类向导Ctrl W--类名:CImageCYDlg--CImageCYDlg(IDs)—WM_INITDLAOG建立这个函数可以用于初始化。
![](https://img-blog.csdn.net/20150528151623320)
c.打开类向导Ctrl+W--选择MemberVariables页面，类名：CImageCYDlg--Add Variables--设置成：
IDC_EDIT_CYX--int--m_xPlace
IDC_EDIT_CYY--int--m_yPlace
d.在View.cpp中添加采样的头文件\#include "ImageCYDlg.h"![](https://img-blog.csdn.net/20150528151745788)
第三步：在ImageProcessingView.cpp中添加代码

```python
//****************图片采样****************//
void CImageProcessingView::OnCy() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能采样!",MB_OK,0);
		return;
	}
	CImageCYDlg dlg;     //定义采样对话框
	//显示对话框
	if( dlg.DoModal()==IDOK ) {
		//采样坐标最初为图片的自身像素
		if( dlg.m_xPlace==0 || dlg.m_yPlace==0 ) {
			AfxMessageBox("输入图片像素不能为0!",MB_OK,0);
			return;
		}
		if( dlg.m_xPlace>m_nWidth || dlg.m_yPlace>m_nHeight ) {
			AfxMessageBox("图片像素不能为超过原图长宽!",MB_OK,0);
			return;
		}
		AfxMessageBox("图片采样!",MB_OK,0);
		//打开临时的图片 读取文件
		FILE *fpo = fopen(BmpName,"rb");
		FILE *fpw = fopen(BmpNameLin,"wb+");
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
		fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
		fread(m_pImage,m_nImage,1,fpo);
	
		/*图片采样*/
		int numWidth,numHeight;     //图片此区间取相同的像素点
		int numSYWidth,numSYHeight; //剩余期间区域 
		/*********************************************************/
		/* 表示numWidth*numHeight为一个区域 该区域颜色相同       
		/* 如 512/512=1 512/512=1 1*1为一个区域                  
		/* dlg.m_xPlace*dlg.m_yPlace 表示新的（x,y）坐标         
		/* numSYWidth表示剩余空间 该区域统一为一个颜色           
		/*********************************************************/
		numWidth=m_nWidth/dlg.m_xPlace;        
		numHeight=m_nHeight/dlg.m_yPlace;      
		numSYWidth=m_nWidth%dlg.m_xPlace;     
		numSYHeight=m_nHeight%dlg.m_yPlace;   
		int Y,X;
		int i,j,m,n;
		unsigned char red,green,blue;  //存储三种颜色
	
		/* 有((m_xPlace * m_yPlace)+ 剩余区域 )个小区域 */
		for( i=0; i<dlg.m_yPlace; i++ )       //高度
		{
			Y=numHeight*i;                    //获取Y坐标
			for( j=0; j<dlg.m_yPlace; j++ )   //宽度
			{
				X=numWidth*j;                 //获取X坐标
				/*获取填充颜色*/
				red=m_pImage[(X+Y*m_nWidth)*3];
				green=m_pImage[(X+Y*m_nWidth)*3+1];
				blue=m_pImage[(X+Y*m_nWidth)*3+2];
				/*填出图像循环 小区域中的长宽循环*/
				for( n=0; n<numHeight; n++ )
				{
					for( m=0; m<numWidth*3; )
					{
						m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=red;
						m++;
						m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=green;
						m++;
						m_pImage[(X+Y*m_nWidth)*3+m+n*m_nWidth*3]=blue;
						m++;
					}
				}
			}
		}
		fwrite(m_pImage,m_nImage,1,fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level=3;
		Invalidate();
	}
}
```
第四步：修改ShowBitmap(CDC* pDC,CString BmpName)中的代码:
else if(level==3) //图片采样
{
m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
运行效果如下图所示，其中彩色图片应该先灰度处理再进行其他操作。
![](https://img-blog.csdn.net/20150528152924756)
![](https://img-blog.csdn.net/20150528152824141)
![](https://img-blog.csdn.net/20150528152842767)




总结：后悔当初还没有写博客，通过回忆几年前的代码，很多当时的体会和思想都已不复存在了！可能你在百度文库中看到类似的文章，因为那些都是我在2012年上传的，最初是通过它进行分享编程知识的，后来发现了更好的CSDN而取代之。这篇文章感觉太详细，有时候一直怀疑是不是失去了算法的本质，不应该写这么详细的文章，而更加精简一点，但可能和从小记笔记有关，很难改过来了，慢慢改吧！
最后还是希望文章对你有所帮助，如果文章有不足或错误之处，请海涵~
（By:Eastmount 2015-5-28 下午点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）



