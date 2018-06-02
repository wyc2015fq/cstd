
// MenuImages.cpp: implementation of the CBCGPMenuImages class.
//
//////////////////////////////////////////////////////////////////////

#include "MenuImages.h"

#include "bcgprores.h"
#include "bcgglobals.h"

//static const COLORREF clrTransparent = _RGB (255, 0, 255);
static const int iImageWidth = 9;
static const int iImageHeight = 9;

CBCGPToolBarImages CBCGPMenuImages::m_ImagesBlack;
CBCGPToolBarImages CBCGPMenuImages::m_ImagesDkGray;
CBCGPToolBarImages CBCGPMenuImages::m_ImagesGray;
CBCGPToolBarImages CBCGPMenuImages::m_ImagesLtGray;
CBCGPToolBarImages CBCGPMenuImages::m_ImagesWhite;
CBCGPToolBarImages CBCGPMenuImages::m_ImagesBlack2;

BOOL CBCGPMenuImages::m_bInitializing = FALSE;

BOOL CBCGPMenuImages::Initialize()
{
	if (m_bInitializing)
	{
		return FALSE;
	}

	if (m_ImagesBlack.IsValid())
	{
		return TRUE;
	}

	m_bInitializing = TRUE;

	CBCGPLocalResource locaRes;
	m_ImagesBlack.SetImageSize (CSize (iImageWidth, iImageHeight));
	if (!m_ImagesBlack.Load (globalData.Is32BitIcons() ? IDB_BCGBARRES_MENU_IMAGES24 : IDB_BCGBARRES_MENU_IMAGES))
	{
		TRACE(_T("CBCGPMenuImages. Can't load menu images %x\n"), IDB_BCGBARRES_MENU_IMAGES);
		m_bInitializing = FALSE;
		return FALSE;
	}

	if (m_ImagesBlack.IsRTL())
	{
		m_ImagesBlack.Mirror();
	}

	m_ImagesBlack.SetTransparentColor (clrTransparent);

	CreateCopy (m_ImagesGray, _RGB (128, 128, 128));
	CreateCopy (m_ImagesDkGray, _RGB (72, 72, 72));
	CreateCopy (m_ImagesLtGray, _RGB (192, 192, 192));
	CreateCopy (m_ImagesWhite, _RGB (255, 255, 255));
	CreateCopy (m_ImagesBlack2, _RGB (0, 0, 0));

	if (m_ImagesBlack.IsValid())
	{
		double dblScale = globalData.GetRibbonImageScale();
		if (dblScale != 1.0)
		{
			m_ImagesBlack.SmoothResize (dblScale);
			m_ImagesGray.SmoothResize (dblScale);
			m_ImagesDkGray.SmoothResize (dblScale);
			m_ImagesLtGray.SmoothResize (dblScale);
			m_ImagesWhite.SmoothResize (dblScale);
			m_ImagesBlack2.SmoothResize (dblScale);
		}
	}

	m_bInitializing = FALSE;
	
	return TRUE;
}
//****
CSize CBCGPMenuImages::Size()
{
	if (m_bInitializing)
	{
		CSize size (iImageWidth, iImageHeight);

		double dblScale = globalData.GetRibbonImageScale();
		if (dblScale != 1.0)
		{
			size.w = (int)(size.w * dblScale);
			size.h = (int)(size.h * dblScale);
		}

		return size;
	}

	Initialize();
	return m_ImagesBlack.GetImageSize();
}
//****
void CBCGPMenuImages::Draw (CDC* pDC, IMAGES_IDS id, const CPoint& ptImage,
						CBCGPMenuImages::IMAGE_STATE state,
						const CSize& sizeImage/* = CSize (0, 0)*/)
{
	if (!Initialize())
	{
		return;
	}

	CBCGPDrawState ds;

	CBCGPToolBarImages& images = (state == ImageBlack) ? m_ImagesBlack :
					(state == ImageGray) ? m_ImagesGray : 
					(state == ImageDkGray) ? m_ImagesDkGray : 
					(state == ImageLtGray) ? m_ImagesLtGray : 
					(state == ImageWhite) ? m_ImagesWhite : m_ImagesBlack2;

	images.PrepareDrawImage (ds, sizeImage);
	images.Draw (pDC, ptImage.x, ptImage.y, id);
	images.EndDrawImage (ds);
}
//****
void CBCGPMenuImages::Draw (CDC* pDC, IMAGES_IDS id, const CRect& rectImage,
						CBCGPMenuImages::IMAGE_STATE state,
						const CSize& sizeImageDest/* = CSize (0, 0)*/)
{
	const CSize sizeImage = 
		(sizeImageDest == CSize (0, 0)) ? Size() : sizeImageDest;

	CPoint ptImage (
		rectImage.l + (rectImage.Width() - sizeImage.w) / 2 + ((rectImage.Width() - sizeImage.w) % 2), 
		rectImage.t + (rectImage.Height() - sizeImage.h) / 2 + ((rectImage.Height() - sizeImage.h) % 2));

	Draw (pDC, id, ptImage, state, sizeImageDest);
}
//****
void CBCGPMenuImages::CleanUp()
{
	if (m_bInitializing)
	{
		return;
	}

	if (m_ImagesBlack.GetCount() > 0)
	{
		m_ImagesBlack.Clear();
		m_ImagesGray.Clear();
		m_ImagesDkGray.Clear();
		m_ImagesLtGray.Clear();
		m_ImagesWhite.Clear();
		m_ImagesBlack2.Clear();
	}
}
//****
void CBCGPMenuImages::CreateCopy (CBCGPToolBarImages& images, COLORREF clr)
{
	m_ImagesBlack.CopyTo (images);
	images.MapTo3dColors (TRUE, _RGB (0, 0, 0), clr);
}
//****
void CBCGPMenuImages::SetColor (CBCGPMenuImages::IMAGE_STATE state,
							COLORREF color)
{
	Initialize();

	CBCGPLocalResource locaRes;

	CBCGPToolBarImages imagesTmp;

	imagesTmp.SetImageSize (CSize (iImageWidth, iImageHeight));
	imagesTmp.Load (globalData.Is32BitIcons() ? IDB_BCGBARRES_MENU_IMAGES24 : IDB_BCGBARRES_MENU_IMAGES);
	imagesTmp.SetTransparentColor (clrTransparent);

#ifndef _BCGSUITE_
	if (imagesTmp.IsRTL())
	{
		CBCGPToolBarImages::MirrorBitmap (imagesTmp.m_hbmImageWell, imagesTmp.GetImageSize().w);
	}
#endif

	CBCGPToolBarImages& images = (state == ImageBlack) ? m_ImagesBlack :
					(state == ImageGray) ? m_ImagesGray : 
					(state == ImageDkGray) ? m_ImagesDkGray : 
					(state == ImageLtGray) ? m_ImagesLtGray : 
					(state == ImageWhite) ? m_ImagesWhite : m_ImagesBlack2;

	if (color != (COLORREF)-1)
	{
		imagesTmp.MapTo3dColors (TRUE, _RGB (0, 0, 0), color);
	}

	if (!m_bInitializing)
	{
		imagesTmp.SmoothResize (globalData.GetRibbonImageScale());
	}

	images.Clear();
	imagesTmp.CopyTo (images);
}
