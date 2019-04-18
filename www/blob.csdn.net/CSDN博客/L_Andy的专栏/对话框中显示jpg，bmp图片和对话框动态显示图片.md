# 对话框中显示jpg，bmp图片和对话框动态显示图片 - L_Andy的专栏 - CSDN博客

2013年02月21日 14:28:25[卡哥](https://me.csdn.net/L_Andy)阅读数：5792
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)




- 1、添加一个Picture Control，关联变量为 m_picture，定义一个全局变量CString m_path.  
- 
- 2、添加一个打开jpg图片的按钮，并添加按钮事件：void ...Dlg::OnOpen()。  
- 
- void ...Dlg::OnOpen()  
- 
- {  
- 
- CFileDialog cfd(true,_T(".jpg"),NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,_T("Executable   Files   (*.jpg)|*.jpg|All   Files   (*.*)|*.*||"),this);     //选项图片的约定
- if(cfd.DoModal()!=IDOK)        //判断是否获得图片
-  {   
- return;  
-  }  
-  m_path = cfd.GetPathName();    //获得图片路径 
-  UpdateData(false);             //更新路径公共变量
-  ShowPicture();                 //调用显示图片函数
- 
- }  
- 
- 3、添加函数void ...Dlg::ShowPicture(void)。  
- 
- void ...Dlg::ShowPicture(void)  
- {  
- if(m_path=="")        //判断图片路径是否存在
-  {  
- return;  
-  }  
- //AfxMessageBox(_T("读取到图片路径")); 
-  CRect rect;  
-  m_picture.GetClientRect(&rect);     //m_picture为Picture Control控件变量，获得控件的区域对象
- 
-  CImage image;       //使用图片类 
-  image.Load(m_path);   //装载路径下图片信息到图片类
-  CDC* pDC = m_picture.GetWindowDC();    //获得显示控件的DC
-  image.Draw( pDC -> m_hDC,rect);      //图片类的图片绘制Draw函数
-  ReleaseDC(pDC);  
- }  
- 
- 4、在void ...Dlg::OnPaint()添加：  
- 
- void ...Dlg::OnPaint()  
- {  
- if (IsIconic())  
-  {  
-   ....  
- 
-  }  
- else
-  {  
-   CDialog::OnPaint();  
-   CDialog::UpdateWindow(); //更新windows窗口
-   ShowPicture();     //重绘图片函数 
- 
-  }  
- }  

1.若出现CImage未定义，则包含进头文件:#include <atlimage.h>.

2.若出现:stdafx.obj : error LNK2005: "class ATL::CStringT<char,class StrTraitMFC<char,class ATL::ChTraitsCRT<char> > > m_path" (?m_path@@3V?$CStringT@DV?$StrTraitMFC@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@A)

说明头文件中的变量定义可能出问题了。

我是在stdafx.h头文件中定义了:

CString m_path;    出错；

后将此定义放到我的.C文件中直接定义，编译通过。

3.清除JPG图片：

CStatic* pStatic=
 (CStatic*)GetDlgItem(IDC_STATIC1);

pStatic->SetBitmap(NULL);

我用:

```cpp
m_picture.SetBitmap(NULL);
				UpdateData(false);             //更新路径公共变量
				ShowPicture();                 //调用显示图片函数
```

、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、

vc中对话框动态显示图片

VC中要是想动态显示个图片是很麻烦的，可不是设置个文件路径就简单完事的。

网上的资料很多，但是都是出于同一个地方，学习来的看不懂的还是看不懂。而且有些地方存在问题。所以我想把这个教程稍微完善下，不方便公开自己写的代码了。

我们要完成的是，在一个对话框中建立一个图片显示的范围，当我们点击下一张按钮时，这个区域会按照预定设好的顺序显示某个文件夹下的图片。

新建MFC应用程序，对话框模式。

**1，在预定区域动态载入图片：核心思想是通过CBitmap,HBITMAP,直接用OnPaint()绘制。**

首先在CShowPicDlg类中声明一个变量:CBitmap m_bmp; 双击CShowPicDlg类，光标自动调到class CShowPicDlg : public CDialog处，在protected 之前声明一个全局变量，输入CBitmap m_bmp;

**2，在对话框资源中加入一个picture 标签,名为IDC_STATIC1。打开ClassWinzard（类向导），在ObjectIDs中选中CShowPicDlg，在下面的Member Functions里找OnInitDialog，双击它所在行，光标自动调到BOOL CShowPicDlg::OnInitDialog处。或者，更简单，在左侧类显示栏里找到OnInitDialog，双击，光标自动导向。然后，对照下面的程序修改BOOL CShowPicDlg::OnInitDialog里的内容。**

**这是在子对话框初始化得时候添加的位置，如果想点击“下一张”在更换的话，需要将大括号里的代码放到按钮相关函数的大括号里。**

BOOL CShowPicDlg::OnInitDialog()

{

      CDialog::OnInitDialog();

     if( m_bmp.m_hObject != NULL )//判断

         m_bmp.DeleteObject();

/////////载入图片

HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),

"c://aaa.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);

if( hbmp == NULL )

return FALSE;

///////////////////////该断程序用来取得加载的BMP的信息////////////////////////

m_bmp.Attach( hbmp );

DIBSECTION ds;

BITMAPINFOHEADER &bminfo = ds.dsBmih;

m_bmp.GetObject( sizeof(ds), &ds );

int cx=bminfo.biWidth;//得到图像宽度

int cy=bminfo.biHeight; //得到图像高度

/////////////////// ////////////////////////////////

/////////////得到了图像的宽度和高度后,我们就可以对图像大小进行适应,即调整控件的大小,让它正好显示一张图片///////////////////////////

CRect rect;

GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);

ScreenToClient(&rect);

GetDlgItem(IDC_STATIC1)->MoveWindow(rect.left,rect.top,cx,cy,true);//调整大小

**InvalidateRect(rect);   //刷新对话框区域 这里很重要！如果不加这句，新更换的图片将不刷新！**

return TRUE;// return TRUE unless you set the focus to a control

// EXCEPTION: OCX Property Pages should return FALSE

}

**3，图片加载成功了,标签大小也适应了,下面就是绘制绘制图像了。打开classwinzard类向导,重载WM_PAINT消息，或者，更简单的方法，在左侧类显示栏里CShowPicDlg里找到OnPaint() ，双击，光标自动导向。**

**这里有个事情需要强调，当你的对话框为子对话框时，cpp文件里并没有OnPaint()，你需要在头文件中选择重写函数，选择onpaint（）。**

void CShowPicDlg::OnPaint()

{

//////////////以下三种情况任选一种会是不同效果(只能一种存在)///////////

//CPaintDC dc(this);//若用此句,得到的是对话框的DC,图片将被绘制在对话框上.

CPaintDC dc(GetDlgItem(IDC_STATIC1)); //用此句,得到picture控件的DC,图像将被绘制在控件上，建议用此句

//CDC dc;

//dc.m_hDC=::GetDC(NULL);//若用此两句,得到的是屏幕的DC,图片将被绘制在屏幕上///////////////////////////////////////////////////////

CRect rcclient;

GetDlgItem(IDC_STATIC1)->GetClientRect(&rcclient);

CDC memdc;

memdc.CreateCompatibleDC(&dc);

CBitmap bitmap;

bitmap.CreateCompatibleBitmap(&dc, rcclient.Width(), rcclient.Height());

memdc.SelectObject( &bitmap );

CWnd::DefWindowProc(WM_PAINT, (WPARAM)memdc.m_hDC , 0);

CDC maskdc;

maskdc.CreateCompatibleDC(&dc);

CBitmap maskbitmap;

maskbitmap.CreateBitmap(rcclient.Width(), rcclient.Height(), 1, 1, NULL);

maskdc.SelectObject( &maskbitmap );

maskdc.BitBlt( 0, 0, rcclient.Width(), rcclient.Height(), &memdc,

rcclient.left, rcclient.top, SRCCOPY);

CBrush brush;

brush.CreatePatternBrush(&m_bmp);

dc.FillRect(rcclient, &brush);

dc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(),

&memdc, rcclient.left, rcclient.top,SRCPAINT);

brush.DeleteObject();

// Do not call CDialog::OnPaint() for painting messages

}



