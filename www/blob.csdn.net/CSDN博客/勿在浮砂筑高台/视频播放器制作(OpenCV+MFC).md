# 视频播放器制作(OpenCV+MFC) - 勿在浮砂筑高台 - CSDN博客





2015年11月10日 13:48:54[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：6346标签：[opencv																[MFC																[CvvIamge](https://so.csdn.net/so/search/s.do?q=CvvIamge&t=blog)
个人分类：[MFC](https://blog.csdn.net/luoshixian099/article/category/5941469)





OpenCV+MFC制作一个简单的视频播放器

转载请注明出处：http://blog.csdn.net/luoshixian099/article/details/49744869

界面效果：

![](https://img-blog.csdn.net/20151109222234320)


-----------------------------------------------------------------------------------------------------------------

工程源代码下载地址：[http://download.csdn.net/detail/luoshixian099/9257633](http://download.csdn.net/detail/luoshixian099/9257633)




1.新建一个MFC工程，工程名VideoPlayer，选择使用静态库（方便以后的在其他的电脑上也能打开此软件），添加控件如上图所示。

修改控件的属性：

Picture控件ID--->IDC_VIEW;

滑动条控件 --->为此控件绑定一个CSliderCtrl类对象Slider。为了得到拖动滑动条可以到达“快进”和“快退”的效果。

![](https://img-blog.csdn.net/20151110133331279)





2.首先需要OpenCV里面的CvvImage类，如果版本低于OpencCV2.2，可以直接调用这个类，如果高于此版本，需要重新定义这个类。CvvImage.cpp与CvvImage.h源代码见文章最后，把两个文件加入到工程中即可。

在VideoPlayerDlg.h文件中加入头文件和定义变量



```cpp
#include "opencv2/highgui/highgui.hpp"
#include "CvvImage.h"
#include "afxcmn.h"
using namespace cv;

// CVideoPlayerDlg 对话框
class CVideoPlayerDlg : public CDialog
{
    ....
    //添加代码
	VideoCapture capture;
	HBITMAP hbitmap;
	CRect rect;
	CStatic* pStc; //标识图像显示的Picture控件
	CDC* pDC; //视频显示控件设备上下文
	HDC hDC; //视频显示控件设备句柄

};
```



3.添加一副位图资源，用于界面美观；

![](https://img-blog.csdn.net/20151110133003140)


4.在窗口初始化函数CVideoPlayerDlg::OnInitDialog()中添加代码如下：



```cpp
// TODO: 在此添加额外的初始化代码

	pStc=(CStatic *)GetDlgItem(IDC_VIEW);//IDC_VIEW为Picture控件ID
	pStc->GetClientRect(&rect);//Picture的大小传给矩形rect
	pDC=pStc->GetDC(); //得到Picture控件设备上下文
	hDC=pDC->GetSafeHdc(); //得到Picture控件设备上下文的句柄 
hbitmap = ::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BITMAP1));//加载位图资源
	pStc->ModifyStyle(1,SS_BITMAP);//修改控件的属性
	pStc->SetBitmap(hbitmap);//显示位图
```




5.双击“打开视频”按钮，为其添加响应函数



```cpp
void CVideoPlayerDlg::OnBnClickedButton1()
{
	CString caption;
	GetDlgItemText(IDC_BUTTON1,caption);
	if (caption == _T("关闭视频"))    //关闭视频按钮
	{
		KillTimer(1);
		capture.release();
		SetDlgItemText(IDC_BUTTON1,_T("打开视频"));
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->EnableWindow(FALSE);  //滑动条失效
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->SetPos(0); //设置滑动条位置
		pStc->SetBitmap(hbitmap); //恢复位图资源
		return;
	}
   

 //打开视频,弹出通用对话框,选择要播放的视频文件
	string _Path;  
	CFileDialog Openfiledlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (Openfiledlg.DoModal() == IDOK) //弹出模态对话框
	{
		//CString类型转换为string类型
		CString  filepath;      
		filepath=Openfiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		filepath.ReleaseBuffer();
		_Path = temp.GetBuffer(0);
		temp.ReleaseBuffer();
	}
	else
		return ;
	

	capture.open(_Path);  //打开视频
	if (!capture.isOpened())
	{        
		MessageBox(_T("无法打开视频！"),_T("错误"));     
		return;
	}
	else
	{
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER1))->EnableWindow(TRUE);  //激活滑动条
		Slider.SetRange(0,(int)capture.get(CV_CAP_PROP_FRAME_COUNT));//设置滑动条的范围,为视频的总帧数
		SetTimer(1,(unsigned int)(1000*1.0/capture.get(CV_CAP_PROP_FPS)),NULL); //定时器，定时时间和帧率一致
		SetDlgItemText(IDC_BUTTON1,_T("关闭视频"));//修改按钮的Caption
	}
}
```
注意到要使用MFC中的CFileDialog类，用来弹出通用对话框，效果如下：


![](https://img-blog.csdn.net/20151110130316377)


关于CFileDialog类的构造方法，可以参考MSDN

![](https://img-blog.csdn.net/20151110130624708)





6.为主对话框添加定时器响应函数；

右击主对话框->属性->Messages->WM_TIMER

![](https://img-blog.csdn.net/20151110165252213)


添加代码如下：



```cpp
void CVideoPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(1 == nIDEvent)
	{
		Mat img;
		capture>>img;  //取出一帧图像
		if (img.empty()) 
		{
			KillTimer(1);
			MessageBox(_T("视频结束"),_T("提示"));
			capture.release();
		}
		else
		{
			CvvImage m_CvvImage;    
			IplImage frame(img);   //Mat 转IplImage
			m_CvvImage.CopyOf(&frame,1); //复制该帧图像   
			m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内
			Slider.SetPos( (int)capture.get(CV_CAP_PROP_POS_FRAMES));//设置视频的位置
		}
	}
	CDialog::OnTimer(nIDEvent);
}
```



7.双击“滑动控件”为其添加响应函数，以到达“快进”和“快退”的效果



```cpp
void CVideoPlayerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
   capture.set(CV_CAP_PROP_POS_FRAMES,Slider.GetPos());  //设置视频的起始帧
	*pResult = 0;
}
```

8.编译运行，效果


![](https://img-blog.csdn.net/20151110133621712)





------------------------------------------------------------------------------------



```cpp
//CvvImage.h

#pragma once
#ifndef CVVIMAGE_CLASS_DEF
#define CVVIMAGE_CLASS_DEF
#include "opencv.hpp"
/* CvvImage class definition */
class  CvvImage
{
public:
	CvvImage();
	virtual ~CvvImage();
	/* Create image (BGR or grayscale) */
	virtual bool  Create( int width, int height, int bits_per_pixel, int image_origin = 0 );
	/* Load image from specified file */
	virtual bool  Load( const char* filename, int desired_color = 1 );
	/* Load rectangle from the file */
	virtual bool  LoadRect( const char* filename,
		int desired_color, CvRect r );
#if defined WIN32 || defined _WIN32
	virtual bool  LoadRect( const char* filename,
		int desired_color, RECT r )
	{
		return LoadRect( filename, desired_color,
			cvRect( r.left, r.top, r.right - r.left, r.bottom - r.top ));
	}
#endif
	/* Save entire image to specified file. */
	virtual bool  Save( const char* filename );
	/* Get copy of input image ROI */
	virtual void  CopyOf( CvvImage& image, int desired_color = -1 );
	virtual void  CopyOf( IplImage* img, int desired_color = -1 );
	IplImage* GetImage() { return m_img; };
	virtual void  Destroy(void);
	/* width and height of ROI */
	int Width() { return !m_img ? 0 : !m_img->roi ? m_img->width : m_img->roi->width; };
	int Height() { return !m_img ? 0 : !m_img->roi ? m_img->height : m_img->roi->height;};
	int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };
	virtual void  Fill( int color );
	/* draw to highgui window */
	virtual void  Show( const char* window );

#if defined WIN32 || defined _WIN32
	/* draw part of image to the specified DC */
	virtual void  Show( HDC dc, int x, int y, int width, int height,
		int from_x = 0, int from_y = 0 );
	/* draw the current image ROI to the specified rectangle of the destination DC */
	virtual void  DrawToHDC( HDC hDCDst, RECT* pDstRect );
#endif
protected:
	IplImage*  m_img;
};
typedef CvvImage CImage;
#endif
```






--------------------------------------------



```cpp
//CvvImage.cpp

#include "StdAfx.h"
#include "CvvImage.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CV_INLINE RECT NormalizeRect( RECT r );
CV_INLINE RECT NormalizeRect( RECT r )
{
	int t;
	if( r.left > r.right )
	{
		t = r.left;
		r.left = r.right;
		r.right = t;
	}
	if( r.top > r.bottom )
	{
		t = r.top;
		r.top = r.bottom;
		r.bottom = t;
	}

	return r;
}
CV_INLINE CvRect RectToCvRect( RECT sr );
CV_INLINE CvRect RectToCvRect( RECT sr )
{
	sr = NormalizeRect( sr );
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
}
CV_INLINE RECT CvRectToRect( CvRect sr );
CV_INLINE RECT CvRectToRect( CvRect sr )
{
	RECT dr;
	dr.left = sr.x;
	dr.top = sr.y;
	dr.right = sr.x + sr.width;
	dr.bottom = sr.y + sr.height;

	return dr;
}
CV_INLINE IplROI RectToROI( RECT r );
CV_INLINE IplROI RectToROI( RECT r )
{
	IplROI roi;
	r = NormalizeRect( r );
	roi.xOffset = r.left;
	roi.yOffset = r.top;
	roi.width = r.right - r.left;
	roi.height = r.bottom - r.top;
	roi.coi = 0;

	return roi;
}
void  FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}
CvvImage::CvvImage()
{
	m_img = 0;
}
void CvvImage::Destroy()
{
	cvReleaseImage( &m_img );
}
CvvImage::~CvvImage()
{
	Destroy();
}
bool  CvvImage::Create( int w, int h, int bpp, int origin )
{
	const unsigned max_img_size = 10000;

	if( (bpp != 8 && bpp != 24 && bpp != 32) ||
		(unsigned)w >=  max_img_size || (unsigned)h >= max_img_size ||
		(origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL))
	{
		assert(0); // most probably, it is a programming error
		return false;
	}
	if( !m_img || Bpp() != bpp || m_img->width != w || m_img->height != h )
	{
		if( m_img && m_img->nSize == sizeof(IplImage))
			Destroy();
		/* prepare IPL header */
		m_img = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
	}
	if( m_img )
		m_img->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;
	return m_img != 0;
}
void  CvvImage::CopyOf( CvvImage& image, int desired_color )
{
	IplImage* img = image.GetImage();
	if( img )
	{
		CopyOf( img, desired_color );
	}
}
#define HG_IS_IMAGE(img)                                                  \
	((img) != 0 && ((const IplImage*)(img))->nSize == sizeof(IplImage) && \
	((IplImage*)img)->imageData != 0)
void  CvvImage::CopyOf( IplImage* img, int desired_color )
{
	if( HG_IS_IMAGE(img) )
	{
		int color = desired_color;
		CvSize size = cvGetSize( img ); 
		if( color < 0 )
			color = img->nChannels > 1;
		if( Create( size.width, size.height,
			(!color ? 1 : img->nChannels > 1 ? img->nChannels : 3)*8,
			img->origin ))
		{
			cvConvertImage( img, m_img, 0 );
		}
	}
}
bool  CvvImage::Load( const char* filename, int desired_color )
{
	IplImage* img = cvLoadImage( filename, desired_color );
	if( !img )
		return false;

	CopyOf( img, desired_color );
	cvReleaseImage( &img );

	return true;
}
bool  CvvImage::LoadRect( const char* filename,
						 int desired_color, CvRect r )
{
	if( r.width < 0 || r.height < 0 ) return false;

	IplImage* img = cvLoadImage( filename, desired_color );
	if( !img )
		return false;
	if( r.width == 0 || r.height == 0 )
	{
		r.width = img->width;
		r.height = img->height;
		r.x = r.y = 0;
	}
	if( r.x > img->width || r.y > img->height ||
		r.x + r.width < 0 || r.y + r.height < 0 )
	{
		cvReleaseImage( &img );
		return false;
	}
	/* truncate r to source image */
	if( r.x < 0 )
	{
		r.width += r.x;
		r.x = 0;
	}
	if( r.y < 0 )
	{
		r.height += r.y;
		r.y = 0;
	}
	if( r.x + r.width > img->width )
		r.width = img->width - r.x;

	if( r.y + r.height > img->height )
		r.height = img->height - r.y;
	cvSetImageROI( img, r );
	CopyOf( img, desired_color );
	cvReleaseImage( &img );
	return true;
}
bool  CvvImage::Save( const char* filename )
{
	if( !m_img )
		return false;
	cvSaveImage( filename, m_img );
	return true;
}
void  CvvImage::Show( const char* window )
{
	if( m_img )
		cvShowImage( window, m_img );
}
void  CvvImage::Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y )
{
	if( m_img && m_img->depth == IPL_DEPTH_8U )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;
		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		from_x = MIN( MAX( from_x, 0 ), bmp_w - 1 );
		from_y = MIN( MAX( from_y, 0 ), bmp_h - 1 );
		int sw = MAX( MIN( bmp_w - from_x, w ), 0 );
		int sh = MAX( MIN( bmp_h - from_y, h ), 0 );
		SetDIBitsToDevice(
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,
			m_img->imageData + from_y*m_img->widthStep,
			bmi, DIB_RGB_COLORS );
	}
}
void  CvvImage::DrawToHDC( HDC hDCDst, RECT* pDstRect ) 
{
	if( pDstRect && m_img && m_img->depth == IPL_DEPTH_8U && m_img->imageData )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;
		CvRect roi = cvGetImageROI( m_img );
		CvRect dst = RectToCvRect( *pDstRect );
		if( roi.width == dst.width && roi.height == dst.height )
		{
			Show( hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
			return;
		}
		if( roi.width > dst.width )
		{
			SetStretchBltMode(
				hDCDst,           // handle to device context
				HALFTONE );
		}
		else
		{
			SetStretchBltMode(
				hDCDst,           // handle to device context
				COLORONCOLOR );
		}
		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		::StretchDIBits(
			hDCDst,
			dst.x, dst.y, dst.width, dst.height,
			roi.x, roi.y, roi.width, roi.height,
			m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
	}
}
void  CvvImage::Fill( int color )
{
	cvSet( m_img, cvScalar(color&255,(color>>8)&255,(color>>16)&255,(color>>24)&255) );
}
```


--------------------------



参考文章：

[http://www.cnblogs.com/Romi/archive/2012/05/07/2487652.html](http://www.cnblogs.com/Romi/archive/2012/05/07/2487652.html)


[http://blog.csdn.net/fioletfly/article/details/6667570](http://blog.csdn.net/fioletfly/article/details/6667570)](https://so.csdn.net/so/search/s.do?q=MFC&t=blog)](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)




