//
// BCGPRibbonInfoLoader.cpp: implementation of the CBCGPRibbonInfoLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPRibbonInfoLoader.h"

#ifndef BCGP_EXCLUDE_RIBBON

namespace BCGPRibbonInfoLoader
{

static int UTF8ToString (LPCSTR lpSrc, LPTSTR& lpDst, int nLength = -1)
{
	int count = ::MultiByteToWideChar (CP_UTF8, 0, lpSrc, nLength, NULL, 0);
	if (count <= 1)
	{
		return 0;
	}

	BOOL bTerm = nLength == -1;
	if (!bTerm)
	{
		count++;
	}

	LPWSTR lpWide = new WCHAR[count];
	memset (lpWide, 0, count * sizeof(WCHAR));

	::MultiByteToWideChar (CP_UTF8, 0, lpSrc, nLength, lpWide, count);
	count--;

#ifdef _UNICODE
	lpDst = lpWide;
#else
	count = ::WideCharToMultiByte (::GetACP (), 0, lpWide, -1, NULL, 0, NULL, 0);

	if (count > 1)
	{
		lpDst = new char[count];
		memset (lpDst, 0, count);

		::WideCharToMultiByte (::GetACP (), 0, lpWide, -1, lpDst, count, NULL, 0);
		count--;
	}

	delete [] lpWide;
#endif

	return count;
}

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonInfoLoader::CBCGPRibbonInfoLoader (CBCGPRibbonInfo& info,
											  DWORD dwFlags)
	: m_Info      (info)
	, m_dwFlags   (dwFlags)
	, m_hInstance (NULL)
{

}

CBCGPRibbonInfoLoader::~CBCGPRibbonInfoLoader()
{
}

BOOL CBCGPRibbonInfoLoader::Load (UINT uiResID, LPCTSTR lpszResType, HINSTANCE hInstance/* = NULL*/)
{
	return Load (MAKEINTRESOURCE (uiResID), lpszResType, hInstance);
}

BOOL CBCGPRibbonInfoLoader::Load (LPCTSTR lpszResID, LPCTSTR lpszResType, HINSTANCE hInstance/* = NULL*/)
{
	if (lpszResID == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (lpszResType == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bRes = FALSE;

	if (hInstance == NULL)
	{
		hInstance = AfxFindResourceHandle(lpszResID, lpszResType);
	}

	if (hInstance == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	m_hInstance = hInstance;
	HRSRC hRsrc = ::FindResource(m_hInstance, lpszResID, lpszResType);	
	
	if (hRsrc == NULL)
	{
		return FALSE;
	}

	DWORD nLength = ::SizeofResource(m_hInstance, hRsrc);
	if (nLength == 0)
	{
		return FALSE;
	}

	HGLOBAL hGlobal = ::LoadResource(m_hInstance, hRsrc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	LPTSTR lpszXML = NULL;
	BCGPRibbonInfoLoader::UTF8ToString ((LPCSTR)::LockResource (hGlobal), lpszXML, nLength);

	::UnlockResource (hGlobal);
	::FreeResource (hGlobal);

	if (lpszXML != NULL)
	{
		bRes = LoadFromBuffer (lpszXML);
		delete [] lpszXML;
	}

	return bRes;
}

BOOL CBCGPRibbonInfoLoader::LoadFromBuffer (LPCTSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRes = TRUE;

	if (m_Info.FromTag (lpszBuffer, m_dwFlags))
	{
		CBCGPRibbonInfo::XArrayImages images;
		m_Info.GetArrayImages (images);

		int i = 0;
		for (i = 0; i < images.GetSize (); i++)
		{
			if (!LoadImage (*images[i]))
			{
				ASSERT(FALSE);
			}
		}

		if (m_Info.GetRibbonBar ().m_btnMain != NULL)
		{
			if (m_Info.GetRibbonBar ().m_btnMain->m_Image.m_Image.IsValid ())
			{
				m_Info.GetRibbonBar ().m_btnMain->m_Image.m_Image.SetSingleImage ();
			}
		}
	}

	return bRes;
}

BOOL CBCGPRibbonInfoLoader::LoadImage (CBCGPRibbonInfo::XImage& image)
{
	if (image.m_strID.IsEmpty ())
	{
		return TRUE;
	}

	if (image.m_Image.LoadStr (image.m_strID, m_hInstance))
	{
		return TRUE;
	}

	image.m_strID.Empty ();
	return FALSE;
}

#endif // BCGP_EXCLUDE_RIBBON
