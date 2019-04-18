# CxImage转换成IplImage - 深之JohnChen的专栏 - CSDN博客

2010年08月12日 18:11:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3523标签：[dst																[图像处理																[iterator																[image																[null																[pair](https://so.csdn.net/so/search/s.do?q=pair&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)
个人分类：[OpenCV](https://blog.csdn.net/byxdaz/article/category/716541)


     最近由于在项目中用到了Opencv库，但是为了更好的显示图像还是使用了Cximage库，它可以快捷地存取、显示、转换各种图像。Opencv库用于高级图像处理与识别。为了使Cximage图像与Opencv图像交互，写了个Cximage转换到IplImage的类。

OpenCV2CXimage.h 
#pragma once
/*
* 类说明：OpenCV图像与CXimage图像互转
* 用于OpenCV的图像与CXimage图像格式互转进行封装。 OpenCV的图像位数必须是大等于8的整数倍，支持BMP，JPG图像格式；CXimage的图像位数可以是1、2、4、8、16、24，支持BMP，JPG，TIF，PNG，GIF图像格式。
*/

#include "unicode//ximage.h"
#pragma comment(lib, "cximage_unicode.lib")
//#pragma comment(lib, "j2k_unicode.lib")
#pragma comment(lib, "jasper_unicode.lib")
#pragma comment(lib, "jbig_unicode.lib")
#pragma comment(lib, "Jpeg_unicode.lib")
#pragma comment(lib, "png_unicode.lib")
#pragma comment(lib, "Tiff_unicode.lib")
#pragma comment(lib, "zlib_unicode.lib")
#pragma comment(lib, "mng_unicode.lib")
#pragma comment(lib, "libdcr_unicode.lib")

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#ifdef _DEBUG
	#pragma comment(lib, "cxcore200d.lib")
	#pragma comment(lib, "cv200d.lib")
	#pragma comment(lib, "highgui200d.lib")
#else
	#pragma comment(lib, "cxcore200.lib")
	#pragma comment(lib, "cv200.lib")
	#pragma comment(lib, "highgui200.lib")
#endif

class OpenCV2CXimage
{
public:
	OpenCV2CXimage(void);
	~OpenCV2CXimage(void);

	/*
	功能说明: 获取黑点标记的方式
	参数说明:  cxImage 图像处理类
	返回值: 黑点标记
	*/
	static int OpenCV2CXimage::GetBlackColor(CxImage cxImage);

	/*
	功能说明: 获取白点标记的方式
	参数说明:  cxImage 图像处理类
	返回值: 黑点标记
	*/
	static int OpenCV2CXimage::GetWhiteColor(CxImage cxImage);

	/*
	*功能说明：转换Cximage到IplImage(注：由于IplImage结构不支持图像像数非8位格式，所以强制转换成8位整数倍)
	*参数说明：src，表示原始Cximage图像；dst，[out] 表示Opencv图像IplImage结构
	*返回值：bool类型。true，表示成功；flase,表示失败。
	*/
	bool Cximage2IplImage(CxImage *src,IplImage **dst);

	/*
	*功能说明：转换IplImage到Cximage
	*参数说明：src，表示Opencv图像IplImage结构；dst，[out] 表示输出Cximage图像;nBpp,表示输出Cximage图像位数占多少位[一个像数占多少位](1,8,24);
	*返回值：bool类型。true，表示成功；flase,表示失败。
	*/
	bool IplImage2Cximage(IplImage *src,CxImage *dst,long nBpp=8);

	/*
	*功能说明：图象格式转换
	*参数说明：src，表示输入Cximage图像；dst，[out] 表示输出Cximage图像；imagetype,表示图象类型
	*返回值：bool类型。true，表示成功；flase,表示失败。
	*/
	bool CxImageFormatConvert(CxImage *src,CxImage *dst,long imagetype = CXIMAGE_FORMAT_BMP);

protected:
	RGBQUAD *m_pPal;//调色版
	int m_nPalatteCount;
};


OpenCV2CXimage.cpp

#include "StdAfx.h"
#include "OpenCV2CXimage.h"
#include <map>
using namespace std;

OpenCV2CXimage::OpenCV2CXimage(void)
{
	m_pPal = NULL;
	m_nPalatteCount = 0;
}

OpenCV2CXimage::~OpenCV2CXimage(void)
{
	if(m_pPal!=NULL)
	{
		delete []m_pPal;
		m_pPal = NULL;
	}
}

//函数名: GetBlackColor
//功能:  获取黑点标记的方式
//参数:  cxImage 图像处理类
//返回值: 黑点标记
int OpenCV2CXimage::GetBlackColor(CxImage cxImage)
{
	long i;
	int iBlackFlag = 0;
	RGBQUAD *pData = cxImage.GetPalette();
	long nPaletteSize = cxImage.GetPaletteSize()/sizeof(RGBQUAD);
	for(i=0;i<nPaletteSize;i++)
	{
		if(pData[i].rgbBlue==0 && pData[i].rgbGreen==0 && pData[i].rgbRed==0)
		{
			iBlackFlag = i;
			break;
		}
	}
	return iBlackFlag;
}

//函数名: GetWhiteColor
//功能:  获取白点标记的方式
//参数:  cxImage 图像处理类
//返回值: 黑点标记
int OpenCV2CXimage::GetWhiteColor(CxImage cxImage)
{
	long i;
	int iWhiteFlag = 255;
	RGBQUAD *pData = cxImage.GetPalette();
	long nPaletteSize = cxImage.GetPaletteSize()/sizeof(RGBQUAD);
	for(i=0;i<nPaletteSize;i++)
	{
		if(pData[i].rgbBlue==255 && pData[i].rgbGreen==255 && pData[i].rgbRed==255)
		{
			iWhiteFlag = i;
			break;
		}
	}
	return iWhiteFlag;
}

/*
*功能说明：转换Cximage到IplImage
*参数说明：src，表示原始Cximage图像；dst，[out] 表示Opencv图像IplImage结构
*返回值：bool类型。true，表示成功；flase,表示失败。
*/
bool OpenCV2CXimage::Cximage2IplImage(CxImage *src,IplImage **dst)
{
	bool bRet = true;
	if(!src || !src->IsValid())
	{
		bRet = false;
		return bRet;
	}
	m_nPalatteCount = src->GetPaletteSize()/sizeof(RGBQUAD);;
	m_pPal = src->GetPalette();
	int iBackColor = GetBlackColor(*src);
	long i = 0,j = 0;
	long nImageWidth = 0,nImageHeight = 0;
	nImageWidth = src->GetWidth();
	nImageHeight = src->GetHeight();
	long nBitCunt = src->GetBpp();
	if(nBitCunt<=1)
	{
		*dst = cvCreateImage(cvSize(nImageWidth,nImageHeight),IPL_DEPTH_8U,1); 
		cvZero(*dst);
		//转换Cximage to IplImage
		for(j=0;j<nImageHeight;j++)
		{
			for(i=0;i<nImageWidth;i++)
			{
				if(src->GetPixelIndex(i,j)==iBackColor)
				{
					CV_IMAGE_ELEM(*dst,uchar,nImageHeight-1-j,i) = 0;
				}
				else
				{
					CV_IMAGE_ELEM(*dst,uchar,nImageHeight-1-j,i) = 255;
				}
			}
		}
	}
	else if(nBitCunt<=8)
	{
		*dst = cvCreateImage(cvSize(nImageWidth,nImageHeight),IPL_DEPTH_8U,1); 
		cvZero(*dst);
		//对应图像调色版与标准调色版的关系
		map<int,int> mapPalatte;
		RGBQUAD szSystemPal[256];
		int k = 0;
		for(k=0;k<256;k++)
		{
			szSystemPal[k].rgbBlue = k;
			szSystemPal[k].rgbGreen = k;
			szSystemPal[k].rgbRed = k;
			szSystemPal[k].rgbReserved = 0;
		}
		int m = 0;
		for(m=0;m<m_nPalatteCount;m++)
		{
			for(k=0;k<256;k++)
			{
				if(m_pPal[m].rgbBlue==szSystemPal[k].rgbBlue && m_pPal[m].rgbGreen==szSystemPal[k].rgbGreen && m_pPal[m].rgbRed==szSystemPal[k].rgbRed)
				{
					mapPalatte.insert(make_pair(m,k));
					break;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//转换Cximage to IplImage
		map<int,int>::iterator iter;
		BYTE btIndex = 0;
		for(j=0;j<nImageHeight;j++)
		{
			for(i=0;i<nImageWidth;i++)
			{
				btIndex = src->GetPixelIndex(i,j);
				iter = mapPalatte.find(btIndex);
				if(iter!=mapPalatte.end())
				{
					btIndex = iter->second;
				}
				CV_IMAGE_ELEM(*dst,uchar,nImageHeight-1-j,i) = btIndex;
			}
		}
	}
	else if(nBitCunt<=16)
	{
		*dst = cvCreateImage(cvSize(nImageWidth,nImageHeight),IPL_DEPTH_16U,1); 
		(*dst)->origin = 1;//底—左结构 (Windows bitmaps 风格) 
		cvZero(*dst);
		//转换Cximage to IplImage
		for(j=0;j<nImageHeight;j++)
		{
			for(i=0;i<nImageWidth;i++)
			{
				BYTE *pSrc = src->GetBits(j) + 2*i;
				CV_IMAGE_ELEM(*dst,ushort,j,i) = (*pSrc) + (*(pSrc+1))*256;
			}
		}
	}
	else //24色
	{
		*dst = cvCreateImage(cvSize(nImageWidth,nImageHeight),IPL_DEPTH_8U,3); 
		(*dst)->origin = 1;//底—左结构 (Windows bitmaps 风格) 
		cvZero(*dst);
		//转换Cximage to IplImag
		memcpy((*dst)->imageData,src->GetBits(0),src->GetSize());
	}
	return bRet;
}

/*
*功能说明：转换IplImage到Cximage
*参数说明：src，表示Opencv图像IplImage结构；dst，[out] 表示输出Cximage图像;nBpp,表示输出Cximage图像位数占多少位[一个像数占多少位]
*返回值：bool类型。true，表示成功；flase,表示失败。
*/
bool OpenCV2CXimage::IplImage2Cximage(IplImage *src,CxImage *dst,long nBpp)
{
	bool bRet = true;
	if(src==NULL || dst==NULL)
	{
		return false;
	}
	if(!(nBpp==1 || nBpp==8 || nBpp==24))
	{
		return false;
	}
	long i = 0,j = 0;
	CvSize csImageSize = cvGetSize(src);
	CxImage ciTmp;
	ciTmp.Create(csImageSize.width,csImageSize.height,src->depth,CXIMAGE_FORMAT_BMP);
	if(src->depth== IPL_DEPTH_8U)//灰度
	{
		ciTmp.SetStdPalette();
		BYTE btIndex = 0;
		for(j=0;j<csImageSize.height;j++)
		{
			for(i=0;i<csImageSize.width;i++)
			{
				btIndex = CV_IMAGE_ELEM(src,uchar,csImageSize.height-1-j,i);
				ciTmp.SetPixelIndex(i,j,btIndex);
			}
		}
	}
	else //彩色
	{
		//转换IplImag to Cximage
		memcpy(ciTmp.GetBits(0),src->imageData,src->imageSize);
	}

	//转换成需要的目标图像
	dst->Create(csImageSize.width,csImageSize.height,nBpp,CXIMAGE_FORMAT_BMP);
	if(nBpp==ciTmp.GetBpp())
	{
		dst->Copy(ciTmp);
	}
	else
	{
		if(nBpp==1)//二值
		{
			//对应图像调色版与标准调色版的关系
			map<int,int> mapPalatte;
			RGBQUAD szSystemPal[256];
			int k = 0;
			for(k=0;k<256;k++)
			{
				szSystemPal[k].rgbBlue = k;
				szSystemPal[k].rgbGreen = k;
				szSystemPal[k].rgbRed = k;
				szSystemPal[k].rgbReserved = 0;
			}
			int m = 0;
			for(k=0;k<256;k++)
			{
				for(m=0;m<m_nPalatteCount;m++)
				{
					if(m_pPal[m].rgbBlue==szSystemPal[k].rgbBlue && m_pPal[m].rgbGreen==szSystemPal[k].rgbGreen && m_pPal[m].rgbRed==szSystemPal[k].rgbRed)
					{
						mapPalatte.insert(make_pair(k,m));
						break;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			byte btValue = 0;
			map<int,int>::iterator iter;
			long nImageWidth = 0;
			long nImageHeight = 0;
			if(ciTmp.GetBpp()>8)
			{
				ciTmp.GrayScale();
			}
			if(m_nPalatteCount==2) //表示原始的图象为二值图象
			{
				dst->SetPalette(m_pPal,m_nPalatteCount);
				btValue = 0;
				nImageWidth = ciTmp.GetWidth();
				nImageHeight = ciTmp.GetHeight();
				for(j=0;j<nImageHeight;j++)
				{
					for(i=0;i<nImageWidth;i++)
					{
						btValue = ciTmp.GetPixelIndex(i,j);
						iter = mapPalatte.find(btValue);
						if(iter!=mapPalatte.end())
						{
							btValue = iter->second;
						}
						dst->SetPixelIndex(i,j,btValue);
					}
				}
			}
			else
			{
				ciTmp.Threshold(128);
				dst->Copy(ciTmp);
			}
		}
		else if(nBpp==8)
		{
			//对应图像调色版与标准调色版的关系
			map<int,int> mapPalatte;
			RGBQUAD szSystemPal[256];
			int k = 0;
			for(k=0;k<256;k++)
			{
				szSystemPal[k].rgbBlue = k;
				szSystemPal[k].rgbGreen = k;
				szSystemPal[k].rgbRed = k;
				szSystemPal[k].rgbReserved = 0;
			}
			int m = 0;
			for(k=0;k<256;k++)
			{
				for(m=0;m<m_nPalatteCount;m++)
				{
					if(m_pPal[m].rgbBlue==szSystemPal[k].rgbBlue && m_pPal[m].rgbGreen==szSystemPal[k].rgbGreen && m_pPal[m].rgbRed==szSystemPal[k].rgbRed)
					{
						mapPalatte.insert(make_pair(k,m));
						break;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			byte btValue = 0;
			map<int,int>::iterator iter;
			long nImageWidth = 0;
			long nImageHeight = 0;
			if(ciTmp.GetBpp()!=8)
			{
				ciTmp.GrayScale();
			}
			if(m_nPalatteCount==8) //表示原始的图象为灰度图象
			{
				dst->SetPalette(m_pPal,m_nPalatteCount);
				btValue = 0;
				nImageWidth = ciTmp.GetWidth();
				nImageHeight = ciTmp.GetHeight();
				for(j=0;j<nImageHeight;j++)
				{
					for(i=0;i<nImageWidth;i++)
					{
						btValue = ciTmp.GetPixelIndex(i,j);
						iter = mapPalatte.find(btValue);
						if(iter!=mapPalatte.end())
						{
							btValue = iter->second;
						}
						dst->SetPixelIndex(i,j,btValue);
					}
				}
			}
			else
			{
				dst->Copy(ciTmp);
			}
		}
		else
		{
			if(ciTmp.GetBpp()==24)
			{
				dst->Copy(ciTmp);
			}
			else
			{
				byte btValue = 0;
				COLORREF clValue;
				map<int,int>::iterator iter;
				long nImageWidth = 0;
				long nImageHeight = 0;
				bRet = ciTmp.IncreaseBpp(24);
				dst->Copy(ciTmp);
			}
		}
	}
	return bRet;
}

//图象格式转换
bool OpenCV2CXimage::CxImageFormatConvert(CxImage *src,CxImage *dst,long imagetype)
{
	bool bRet = true;
	if(src==NULL || dst==NULL)
	{
		return false;
	}
	if(!(imagetype>0 && imagetype<=19))
	{
		return false;
	}
	if(src->GetType()==imagetype)
	{
		dst->Copy(*src);
	}
	else
	{
		dst->Create(src->GetWidth(),src->GetHeight(),src->GetBpp(),imagetype);
		src->SetType(imagetype);
		dst->Copy(*src);
	}
	return true;
}


注：CxImage是一款优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。

下载地址:[http://www.codeproject.com/KB/graphics/cximage.aspx](http://www.codeproject.com/KB/graphics/cximage.aspx)


