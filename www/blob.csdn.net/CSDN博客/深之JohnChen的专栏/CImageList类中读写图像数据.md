# CImageList类中读写图像数据 - 深之JohnChen的专栏 - CSDN博客

2009年09月30日 15:03:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3519


CImageList类中读写图像数据

1、CImageList类写入数据使用Add函数。

**int****Add(**

**CBitmap****pbmImage***,**

**CBitmap****pbmMask*

**);**

**int****Add(**

**CBitmap****pbmImage***,**

**COLORREF***crMask*

**);**

**int****Add(**

**HICON***hIcon*

**);**

举例：

CImageList m_ImageList;// CImageList对象，存储图像

Int m_nImageWidth = 120;//目标图像宽度

Int m_nImageHeight = 60;// 目标图像高度

CxImageimageTmp;// CxImage对象，原始图像

CxImageimageTmpCrop;// CxImage对象，目标图像

imageTmp.Load(“C://1.BMP”);//加载图像

imageTmpCrop.Copy(imageTmp);

imageTmp.Resample2(m_nImageWidth,m_nImageHeight,CxImage::IM_BICUBIC2,CxImage::OM_REPEAT,&imageTmpCrop,true);//缩放图象

HANDLEhBitmapHandle= imageTmpCrop.MakeBitmap();//将图像转换成句柄

CBitmap *pBitmap;

pBitmap=newCBitmap;

pBitmap->Attach(hBitmapHandle);

m_ImageList.Add(pBitmap,RGB(255,0,255));//将图像插入m_ImageList对象中

if(pBitmap)

{

deletepBitmap;

}

2、CImageList类读取数据使用GetImageInfo函数。

**BOOL****GetImageInfo(**

**int***nImage***,**

**IMAGEINFO****pImageInfo*

**) const;**

IMAGEINFO结构如下：

typedef struct _IMAGEINFO {

HBITMAP hbmImage;//整个图象的HBITMAP句柄

HBITMAP hbmMask;//图像掩码句柄

int Unused1;

int Unused2;

RECT rcImage;//指定图像区域的范围，也就是从hbmImage中区域范围得到指定的图像

} IMAGEINFO, *LPIMAGEINFO;

注：如果想得到指定子图像数据，必须根据指定子图像区域rcImage从整个图像hbmImage中取出数据。

举例：

CImageList m_ImageList;// CImageList对象，存储图像

IMAGEINFOsubImageInfo;

Int nIndex = 0;

m_ImageList.GetImageInfo(nIndex,&subImageInfo);//得到图像信息数据

CxImage ciShow;//整个图像

CxImage ciSubShow;//子图像

ciShow.CreateFromHBITMAP(subImageInfo.hbmImage);//得到整个图像

ciSubShow.Crop(subImageInfo.rcImage,& ciSubShow);//得到子图像

ciSubShow.Save(“C://2.BMP”);//保存图像

CxImage是开源图像处理库，下载地址：http://www.codeproject.com/KB/graphics/cximage.aspx

