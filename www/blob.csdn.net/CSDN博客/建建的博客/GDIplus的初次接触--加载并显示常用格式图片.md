# GDIplus的初次接触--加载并显示常用格式图片 - 建建的博客 - CSDN博客
2017年12月23日 10:25:34[纪建](https://me.csdn.net/u013898698)阅读数：92
 在没有接触Gdiplus之前，在vc中绘制图片，通常加载一张位图，然后进行贴图。对于现在多种多样的图片格式，之前的GDI并不支持（应该是这样的，呵呵）。而使用Gdiplus则可以选择多种图片格式，比如BMP, ICON, GIF, JPEG, Exif,
 PNG, TIFF, WMF, and EMF。
下面就介绍下首次接触Gdiplus的一个简单应用。
一、包括相应的头文件及引入相应的lib
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- #include <GdiPlus.h>
- //#include <AtlImage.h> //此头文件里面包括#pragma comment(lib, "gdiplus.lib")
- #pragma comment(lib, "gdiplus.lib")
- usingnamespace Gdiplus;  
二、初始化Gdiplus
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;  
- ULONG_PTR m_gdiplusToken;   
- GdiplusStartup( &m_gdiplusToken, &m_gdiplusStartupInput, NULL );  
三、加载相应的资源（此处为图片）
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- Image*      m_pImage;                           //图片对象
- m_pImage=Image::FromFile(_T("Test03.jpg"));  
- 
- //错误判断
- if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok))   
- {  
- if ( m_pImage )  
-     {  
- delete m_pImage;  
-         m_pImage = NULL;  
-     }  
- return FALSE;  
- }  
四、绘制图片
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- Graphics graphics( GetDC()->GetSafeHdc() );   
- graphics.DrawImage(m_pImage, 0,0,m_pImage->GetWidth(),m_pImage->GetWidth());   
五、关闭Gdiplus
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- GdiplusShutdown( m_gdiplusToken );  
以下是显示一个jpg格式的例子（基于对话框的一个程序）
一、主要代码
[cpp][view
 plain](http://blog.csdn.net/s634772208/article/details/46300441#)[copy](http://blog.csdn.net/s634772208/article/details/46300441#)
- //变量声明
- Gdiplus::GdiplusStartupInput    m_gdiplusStartupInput;  
- ULONG_PTR                       m_gdiplusToken;  
- Image*                          m_pImage;                           //图片对象
- 
- //初始化
- BOOL Cmfc12_30Dlg::OnInitDialog()  
- {  
- 
- // TODO: 在此添加额外的初始化代码
- //初始化GDI+.
-     GdiplusStartup( &m_gdiplusToken, &m_gdiplusStartupInput, NULL );  
- 
- //加载文件
- //CT2CW strFileName( _T("I:\\编程练习\\CommonFiles\\Test02.png") );
- //m_pImage = new Image( _T("I:\\编程练习\\CommonFiles\\Test02.png") );
- //m_pImage=Image::FromFile(_T("I:\\编程练习\\CommonFiles\\Test02.png"));
-     m_pImage=Image::FromFile(_T("I:\\编程练习\\CommonFiles\\Test03.jpg"));  
- 
- //错误判断
- if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok))   
-     {  
- if ( m_pImage )  
-         {  
- delete m_pImage;  
-             m_pImage = NULL;  
-         }  
- return FALSE;  
-     }  
- 
-     MoveWindow( 0, 0, 1024, 768 );  
- 
- //设置按钮位置
-     CWnd* pBtn = GetDlgItem( IDOK );  
- if ( pBtn )  
-     {  
-         pBtn->MoveWindow( 1024-160, 700, 60, 30 );  
-     }  
-     pBtn = GetDlgItem( IDCANCEL );  
- if( pBtn )  
-     {  
-         pBtn->MoveWindow( 1024-80, 700, 60, 30 );  
-     }  
-     CenterWindow();  
- 
- return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
- }   
- 
- 
- //绘制
- void Cmfc12_30Dlg::OnPaint()  
- {  
- 
- //GetDC()->PatBlt( 0, 0, 500, 500, BLACKNESS );
- //PatBlt( GetDC()->GetSafeHdc(), 0, 0, 500, 500, BLACKNESS );
- 
-     Graphics graphics( GetDC()->GetSafeHdc() );   
-     graphics.DrawImage(m_pImage, 0,0,m_pImage->GetWidth(),m_pImage->GetWidth());  
- }  
- 
- 
- //关闭
- void Cmfc12_30Dlg::OnDestroy()  
- {  
-     CDialog::OnDestroy();  
- 
- // TODO: 在此处添加消息处理程序代码
- if( m_pImage )  
-     {  
- try
-         {  
- delete m_pImage;  
-         }  
- catch (...)  
-         {  
- 
-         }  
-         m_pImage = NULL;  
-     }  
- 
-     GdiplusShutdown( m_gdiplusToken );  
- }    
二、运行界面 
![](https://img-blog.csdn.net/20150601100147049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvczYzNDc3MjIwOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
