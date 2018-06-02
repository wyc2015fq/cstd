
#include "StdAfx.h"
#include ".\UseGDIPlus.h"


static	GdiplusStartupInput m_gdiplusStartupInput; 	
static	ULONG_PTR m_gdiplusToken = NULL;
static  BOOL g_IniOK = FALSE;

BOOL InitGIDPlus()
{
	if( !g_IniOK )
	{
		GdiplusStartup( &m_gdiplusToken , &m_gdiplusStartupInput , NULL );    
		g_IniOK = TRUE;
	}
	return TRUE;
}

BOOL UinitGIDPlus()
{
	if( g_IniOK )
	{
		GdiplusShutdown( m_gdiplusToken );                                  
		g_IniOK = FALSE;
	}
	return TRUE;
}

BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if( size == 0 )
		return FALSE;
	pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo); 
	
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{ 
		//MimeType: Depiction for the program image  
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{  
			*pCLSID = pImageCodecInfo[i].Clsid; 
			free(pImageCodecInfo); 
			return TRUE;  
		}  
	}  
	free(pImageCodecInfo);  
	return FALSE; 
} 
