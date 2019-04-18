# 调用opencv3.x 库，在MFC中显示图片 - 深之JohnChen的专栏 - CSDN博客

2019年03月03日 17:54:13[byxdaz](https://me.csdn.net/byxdaz)阅读数：200


调用opencv3.x 库，在MFC中显示图片

使用opencv和MFC显示图片的方法大致分为以下几种：

1：将opencv的显示对话框嵌入到MFC窗口。

2：使用CvvImage类显示图片。

3：将opencv图像Mat结构转换为BITMAPINFO结构，再通过dc显示图像。

方法一：

方法是将opencv的显示窗口与MFC的控件链接起来，效果是opencv的窗口恰好覆盖在控件上。

```
bool CMFCOpencvDlg::attachWindow(std::string &pic, const char* name,HWND & hNameWindowParent, UINT nControlID)
{
	pic = std::string(name);
	cv::destroyWindow(pic);
	cv::namedWindow(pic, WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(name);
	hNameWindowParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(nControlID)->m_hWnd);
	::ShowWindow(hNameWindowParent, SW_HIDE);
	return true;
}
bool CMFCOpencvDlg::detachWindow(const char* name, HWND hNameWindowParent)
{
	std::string pic = std::string(name);
	cv::destroyWindow(pic);
	if (hNameWindowParent)
	{
		::ShowWindow(hNameWindowParent, SW_HIDE);
		::PostMessage(hNameWindowParent, WM_CLOSE, 0, 0);
	}

	return true;
}
bool CMFCOpencvDlg::showImage(std::string pic, UINT nControlID, cv::Mat mat, bool bRoomToControlSize)
{
	CRect rect;
	GetDlgItem(nControlID)->GetClientRect(&rect);
	if (bRoomToControlSize)
	{
		cv::resize(mat, mat, cv::Size(rect.Width(), rect.Height()));
	}
	imshow(pic, mat);
	return true;
}

HWND hNameWindowParent = NULL;
void CMFCOpencvDlg::OnBnClickedButton2()
{
	// 显示图像
	std::string pic;
	attachWindow(pic, "win", hNameWindowParent, IDC_STATIC_IMAGE);
	Mat mat = imread("pic.jpg",true);
	showImage(pic, IDC_STATIC_IMAGE, mat);
}

void CMFCOpencvDlg::OnBnClickedButton3()
{
	//不显示图像或销毁图像
	detachWindow("win", hNameWindowParent);
}
```

方法二：

因为opencv2.2以上去掉了CvvImage这个类。在网上找到了这个解决办法：

从原来2.1版本里面将CvvImage单独提取出来做一个新类。

CvvImage.h

```
#pragma once

#ifndef CVVIMAGE_CLASS_DEF
#define CVVIMAGE_CLASS_DEF

#include <opencv/cv.h>
#include <opencv/highgui.h>

/* CvvImage class definition */
class CvvImage
{
public:
	CvvImage();
	virtual ~CvvImage();

	/* Create image (BGR or grayscale) */
	virtual bool Create(int width, int height, int bits_per_pixel, int image_origin = 0);

	/* Load image from specified file */
	virtual bool Load(const char* filename, int desired_color = 1);

	/* Load rectangle from the file */
	virtual bool LoadRect(const char* filename,
		int desired_color, CvRect r);

#if defined WIN32 || defined _WIN32
	virtual bool LoadRect(const char* filename,
		int desired_color, RECT r)
	{
		return LoadRect(filename, desired_color,
			cvRect(r.left, r.top, r.right - r.left, r.bottom - r.top));
	}
#endif

	/* Save entire image to specified file. */
	virtual bool Save(const char* filename);

	/* Get copy of input image ROI */
	virtual void CopyOf(CvvImage& image, int desired_color = -1);
	virtual void CopyOf(IplImage* img, int desired_color = -1);

	IplImage* GetImage() { return m_img; };
	virtual void Destroy(void);

	/* width and height of ROI */
	int Width() { return !m_img ? 0 : !m_img->roi ? m_img->width : m_img->roi->width; };
	int Height() { return !m_img ? 0 : !m_img->roi ? m_img->height : m_img->roi->height; };
	int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };

	virtual void Fill(int color);

	/* draw to highgui window */
	virtual void Show(const char* window);

#if defined WIN32 || defined _WIN32
	/* draw part of image to the specified DC */
	virtual void Show(HDC dc, int x, int y, int width, int height,
		int from_x = 0, int from_y = 0);
	/* draw the current image ROI to the specified rectangle of the destination DC */
	virtual void DrawToHDC(HDC hDCDst, RECT* pDstRect);
#endif

protected:

	IplImage* m_img;
};

typedef CvvImage CImage;

#endif
```

CvvImage.cpp

```
#include "stdafx.h"
#include "CvvImage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CV_INLINE RECT NormalizeRect(RECT r);
CV_INLINE RECT NormalizeRect(RECT r)
{
	int t;

	if (r.left > r.right)
	{
		t = r.left;
		r.left = r.right;
		r.right = t;
	}

	if (r.top > r.bottom)
	{
		t = r.top;
		r.top = r.bottom;
		r.bottom = t;
	}

	return r;
}

CV_INLINE CvRect RectToCvRect(RECT sr);
CV_INLINE CvRect RectToCvRect(RECT sr)
{
	sr = NormalizeRect(sr);
	return cvRect(sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top);
}

CV_INLINE RECT CvRectToRect(CvRect sr);
CV_INLINE RECT CvRectToRect(CvRect sr)
{
	RECT dr;
	dr.left = sr.x;
	dr.top = sr.y;
	dr.right = sr.x + sr.width;
	dr.bottom = sr.y + sr.height;
	return dr;
}

CV_INLINE IplROI RectToROI(RECT r);
CV_INLINE IplROI RectToROI(RECT r)
{
	IplROI roi;
	r = NormalizeRect(r);
	roi.xOffset = r.left;
	roi.yOffset = r.top;
	roi.width = r.right - r.left;
	roi.height = r.bottom - r.top;
	roi.coi = 0;
	return roi;
}

void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
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
	cvReleaseImage(&m_img);
}

CvvImage::~CvvImage()
{
	Destroy();
}

bool CvvImage::Create(int w, int h, int bpp, int origin)
{
	const unsigned max_img_size = 10000;

	if ((bpp != 8 && bpp != 24 && bpp != 32) ||
		(unsigned)w >= max_img_size || (unsigned)h >= max_img_size ||
		(origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL))
	{
		assert(0); // most probably, it is a programming error
		return false;
	}

	if (!m_img || Bpp() != bpp || m_img->width != w || m_img->height != h)
	{
		if (m_img && m_img->nSize == sizeof(IplImage))
			Destroy();

		/* prepare IPL header */
		m_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, bpp / 8);
	}

	if (m_img)
		m_img->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;

	return m_img != 0;
}

void CvvImage::CopyOf(CvvImage& image, int desired_color)
{
	IplImage* img = image.GetImage();
	if (img)
	{
		CopyOf(img, desired_color);
	}
}

#define HG_IS_IMAGE(img) \
    ((img) != 0 && ((const IplImage*)(img))->nSize == sizeof(IplImage) && \
    ((IplImage*)img)->imageData != 0)

void CvvImage::CopyOf(IplImage* img, int desired_color)
{
	if (HG_IS_IMAGE(img))
	{
		int color = desired_color;
		//CvSize size = cvGetSize(img);//opencv3.4.4中调用cvGetSize出错，使用以下语句替换
		CvSize size;
		size.width = img->width;
		size.height = img->height;
		/////////////////////////////////////////////////////////////////////////////////

		if (color < 0)
			color = img->nChannels > 1;

		if (Create(size.width, size.height,
			(!color ? 1 : img->nChannels > 1 ? img->nChannels : 3) * 8,
			img->origin))
		{
			cvConvertImage(img, m_img, 0);
		}
	}
}

bool CvvImage::Load(const char* filename, int desired_color)
{
	IplImage* img = cvLoadImage(filename, desired_color);
	if (!img)
		return false;

	CopyOf(img, desired_color);
	cvReleaseImage(&img);
	return true;
}

bool CvvImage::LoadRect(const char* filename,
	int desired_color, CvRect r)
{
	if (r.width < 0 || r.height < 0) return false;

	IplImage* img = cvLoadImage(filename, desired_color);
	if (!img)
		return false;

	if (r.width == 0 || r.height == 0)
	{
		r.width = img->width;
		r.height = img->height;
		r.x = r.y = 0;
	}

	if (r.x > img->width || r.y > img->height ||
		r.x + r.width < 0 || r.y + r.height < 0)
	{
		cvReleaseImage(&img);
		return false;
	}

	/* truncate r to source image */
	if (r.x < 0)
	{
		r.width += r.x;
		r.x = 0;
	}
	if (r.y < 0)
	{
		r.height += r.y;
		r.y = 0;
	}

	if (r.x + r.width > img->width)
		r.width = img->width - r.x;

	if (r.y + r.height > img->height)
		r.height = img->height - r.y;

	cvSetImageROI(img, r);
	CopyOf(img, desired_color);

	cvReleaseImage(&img);
	return true;
}

bool CvvImage::Save(const char* filename)
{
	if (!m_img)
		return false;
	cvSaveImage(filename, m_img);
	return true;
}

void CvvImage::Show(const char* window)
{
	if (m_img)
		cvShowImage(window, m_img);
}

void CvvImage::Show(HDC dc, int x, int y, int w, int h, int from_x, int from_y)
{
	if (m_img && m_img->depth == IPL_DEPTH_8U)
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;

		FillBitmapInfo(bmi, bmp_w, bmp_h, Bpp(), m_img->origin);

		from_x = MIN(MAX(from_x, 0), bmp_w - 1);
		from_y = MIN(MAX(from_y, 0), bmp_h - 1);

		int sw = MAX(MIN(bmp_w - from_x, w), 0);
		int sh = MAX(MIN(bmp_h - from_y, h), 0);

		SetDIBitsToDevice(
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,
			m_img->imageData + from_y*m_img->widthStep,
			bmi, DIB_RGB_COLORS);
	}
}

void CvvImage::DrawToHDC(HDC hDCDst, RECT* pDstRect)
{
	if (pDstRect && m_img && m_img->depth == IPL_DEPTH_8U && m_img->imageData)
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;

		CvRect roi = cvGetImageROI(m_img);
		CvRect dst = RectToCvRect(*pDstRect);

		if (roi.width == dst.width && roi.height == dst.height)
		{
			Show(hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y);
			return;
		}

		if (roi.width > dst.width)
		{
			SetStretchBltMode(
				hDCDst, // handle to device context
				HALFTONE);
		}
		else
		{
			SetStretchBltMode(
				hDCDst, // handle to device context
				COLORONCOLOR);
		}

		FillBitmapInfo(bmi, bmp_w, bmp_h, Bpp(), m_img->origin);

		::StretchDIBits(
			hDCDst,
			dst.x, dst.y, dst.width, dst.height,
			roi.x, roi.y, roi.width, roi.height,
			m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);
	}
}

void CvvImage::Fill(int color)
{
	cvSet(m_img, cvScalar(color & 255, (color >> 8) & 255, (color >> 16) & 255, (color >> 24) & 255));
}
```

```
void CMFCOpencvDlg::OnBnClickedButton4()
{
	//利用CvvImage显示图像
	IplImage *image;
	image = cvLoadImage("pic.jpg");
	CvvImage cimg;
	CRect rect;
	GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&rect);
	CDC *pDC = GetDlgItem(IDC_STATIC_IMAGE)->GetDC();
	cimg.CopyOf(image);
	cimg.DrawToHDC(pDC->GetSafeHdc(), &rect);
	ReleaseDC(pDC);
}
```

方法三：

该方法利用MFC的绘图函数，将opencv图像数据先copy到缓冲去然后在控件上显示出来。

```
void CMFCOpencvDlg::DrawMat(cv::Mat & img, UINT nControlID, bool bRoomToControlSize)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nControlID)->GetClientRect(&rect);  // 获取控件大小
	if (bRoomToControlSize)
	{
		cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩小或放大Mat并备份
	}
	else
	{
		img.copyTo(imgTmp);
	}

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}

	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // 计算一个像素多少个字节
															 // 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap数据头 -> MFC
	CDC *pDC = GetDlgItem(nControlID)->GetDC();
	pDC->SetStretchBltMode(COLORONCOLOR);
	if (bRoomToControlSize)
	{
		::StretchDIBits(pDC->GetSafeHdc()
			, 0, 0, rect.Width(), rect.Height()
			, 0, 0, imgTmp.cols, imgTmp.rows,
			imgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		int minWidth = min(imgTmp.cols, rect.Width());
		int minHeight = min(imgTmp.rows, rect.Height());
		::StretchDIBits(
			pDC->GetSafeHdc(),
			0, 0, minWidth, minHeight,
			0, 0, minWidth, minHeight,
			imgTmp.data,
			&bitInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}
	ReleaseDC(pDC);
}

//调用实例
void CMFCOpencvDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat image;
	image = imread("pic.jpg");
	DrawMat(image, IDC_STATIC_IMAGE, true);
	return;
}
```

[代码下载](https://download.csdn.net/download/byxdaz/10991397)

