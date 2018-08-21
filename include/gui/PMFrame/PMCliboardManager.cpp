#include "StdAfx.h"
#include "PMCliboardManager.h"
#include "shlobj.h"

#define CF_PMIMAGE  "CF_PMIMAGE"
#define CF_PMGRID   "CF_PMGRID"
#define CF_PMEDITOR "CF_PMEDITOR"

PMCliboardManager::PMCliboardManager(void):mImageSize(0),mGridSize(0),mEditorSize(0),mImage(NULL),
mGrid(NULL),mEditor(NULL),mHDIB(NULL),mCF_Image(NULL)
{
	TJSON jnAppProperty;
	jnAppProperty.Read("property.json");
	mMax_Text_Size = jnAppProperty["MAX_TEXT_FILE"].ToInt();
	mText_Extends.SetData(jnAppProperty["TEXT_EXTS"].ToCString(),"|",true);
	mPicture_Extends.SetData(jnAppProperty["PICTURE_EXTS"].ToCString(),"|",true);
	jnAppProperty.Clear();
}

PMCliboardManager::~PMCliboardManager(void)
{
	Free();
}

PMCliboardManager* PMCliboardManager::GetSingleton()
{
	static PMCliboardManager  CliboardManager;
	return &CliboardManager;
}

PMVoid PMCliboardManager::Initialize( HWND hWnd )
{
	mHWnd		= hWnd;
	mCF_Grid	= RegisterClipboardFormat(CF_PMGRID);
	mCF_Image	= RegisterClipboardFormat(CF_PMIMAGE);
	mCF_Editor  = RegisterClipboardFormat(CF_PMEDITOR);
}

PMBool PMCliboardManager::IsGRValid()
{
	return IsClipboardFormatAvailable (mCF_Grid) || IsClipboardFormatAvailable(mCF_Editor) ||
		IsClipboardFormatAvailable (mCF_Image) || IsClipboardFormatAvailable (CF_TEXT) || 
		IsClipboardFormatAvailable(CF_DIB)|| IsClipboardFormatAvailable(CF_ENHMETAFILE)||
		IsClipboardFormatAvailable(CF_HDROP);
}

PMBool PMCliboardManager::IsEDValid()
{
	return IsClipboardFormatAvailable(mCF_Editor) || IsClipboardFormatAvailable (mCF_Image) ||
		IsClipboardFormatAvailable (CF_TEXT) || IsClipboardFormatAvailable(CF_DIB)|| 
		IsClipboardFormatAvailable(CF_ENHMETAFILE)||IsClipboardFormatAvailable(CF_HDROP);
}

PMVoid PMCliboardManager::CLEAR()
{
	if(OpenClipboard(mHWnd))
	{
		EmptyClipboard() ;
		CloseClipboard() ;
	}
}

PMVoid PMCliboardManager::SetText( const string &data )
{
	mText = data;
}

PMVoid PMCliboardManager::SetImage( TBuffer &buffer )
{
	mImage	   = buffer.buffer;
	mImageSize = buffer.useLen;
}

PMVoid PMCliboardManager::SetImage( HANDLE hDIB )
{
	mHDIB = hDIB;
}

PMVoid PMCliboardManager::SetGrid( TBuffer &buffer )
{
	mGrid	  = buffer.buffer;
	mGridSize = buffer.useLen;
}

PMVoid PMCliboardManager::SetEditor( TBuffer &buffer )
{
	mEditor		= buffer.buffer;
	mEditorSize = buffer.useLen;
}

PMVoid PMCliboardManager::Copy()
{
	if(OpenClipboard(mHWnd))
	{
		EmptyClipboard() ;
		if (mText!="")
		{
			HGLOBAL hgBuffer = GlobalAlloc(GHND | GMEM_SHARE, mText.size()+1);
			PMChar* chBuffer = hgBuffer ? (PMChar*)GlobalLock(hgBuffer) : NULL;
			if (chBuffer)
			{
				memcpy(chBuffer, mText.c_str(), mText.size()+1);
				GlobalUnlock(hgBuffer);
				SetClipboardData(CF_TEXT, hgBuffer);
			}
		}
		if (mImage)
		{
			HGLOBAL hgBuffer = GlobalAlloc(GHND | GMEM_SHARE, mImageSize);
			PMChar* chBuffer = hgBuffer ? (PMChar*)GlobalLock(hgBuffer) : NULL;
			if (chBuffer)
			{
				memcpy(chBuffer, mImage, mImageSize);
				GlobalUnlock(hgBuffer);
				SetClipboardData(mCF_Image, hgBuffer);
			}
		}
		if (mHDIB)
		{
			SetClipboardData(CF_DIB, mHDIB);
		}
		if (mGrid)
		{
			HGLOBAL hgBuffer = GlobalAlloc(GHND | GMEM_SHARE, mGridSize);
			PMChar* chBuffer = hgBuffer ? (PMChar*)GlobalLock(hgBuffer) : NULL;
			if (chBuffer)
			{
				memcpy(chBuffer, mGrid, mGridSize);
				GlobalUnlock(hgBuffer);
				SetClipboardData(mCF_Grid, hgBuffer);
			}
		}
		if (mEditor)
		{
			HGLOBAL hgBuffer = GlobalAlloc(GHND | GMEM_SHARE, mEditorSize);
			PMChar* chBuffer = hgBuffer ? (PMChar*)GlobalLock(hgBuffer) : NULL;
			if (chBuffer)
			{
				memcpy(chBuffer, mEditor, mEditorSize);
				GlobalUnlock(hgBuffer);
				SetClipboardData(mCF_Editor, hgBuffer);
			}
		}
		CloseClipboard() ;
	}
	Free();
}

PMBool PMCliboardManager::GetText( string &data )
{
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		PMChar* chBuffer = hData ? (PMChar*)GlobalLock(hData) : NULL;
		if (chBuffer)
		{
			data = chBuffer;
			GlobalUnlock(hData);
			Result = true;
		}else
		{
			hData = GetClipboardData(CF_HDROP);
			if(hData)		
			{
				VectorString vFiles;
				DROPFILES* DFiles = (DROPFILES*)GlobalLock(hData);
				if(DFiles->fWide)//unicode
				{
					GetDropFiles((wchar_t*)((char*)DFiles+DFiles->pFiles),vFiles);
				}else
				{
					GetDropFiles((char*)DFiles+DFiles->pFiles,vFiles);
				}	
				GlobalUnlock(hData);
				char *fileContent;
				TFile::FilesFilter(vFiles,mText_Extends);
				for (VectorString::iterator it = vFiles.begin(); it != vFiles.end(); it++)
				{
					if(TFile::FileSize(it->c_str())<mMax_Text_Size)
					{
						fileContent = TFile::ReadFile(it->c_str());
						if(fileContent)
						{
							data += fileContent;
							delete[] fileContent;
							Result = true;
						}
					}
				}				
			}
		}
		CloseClipboard();
	}
	return Result;
}

PMBool PMCliboardManager::GetImage( TBuffer &buffer )
{
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData    = GetClipboardData(mCF_Image);	
		if(hData)		
		{
			buffer.size	    = GlobalSize(hData);
			if (buffer.size)
			{
				PMChar* chBuffer = (PMChar*)GlobalLock(hData);
				buffer.buffer = (PMChar *)malloc(buffer.size);
				memcpy(buffer.buffer,chBuffer,buffer.size);
				GlobalUnlock(hData);
				Result = true;
			}	
			buffer.useLen = buffer.size;
		}
		CloseClipboard();
	}
	return Result;
}

PMBool PMCliboardManager::GetImage( CxImage *&image )
{
	image = NULL;
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData    =  GetClipboardData(CF_DIB);		
		if(hData)		
		{
			image  = new CxImage ;
			image->CreateFromHANDLE(hData);	
			Result = true;
		}else if(hData = GetClipboardData(CF_ENHMETAFILE))
		{
			HENHMETAFILE hMeta = (HENHMETAFILE)hData;
			ENHMETAHEADER emh;
			GetEnhMetaFileHeader(hMeta, sizeof(emh), &emh); 

			int	cx,cy;
			cx = (int)(emh.rclBounds.right - emh.rclBounds.left);
			cy = (int)(emh.rclBounds.bottom - emh.rclBounds.top);

			HDC hDC0 = ::GetDC(0); // screen dc
			HBITMAP hBitmap = CreateCompatibleBitmap(hDC0, cx, cy);
			HDC	hDC = CreateCompatibleDC(hDC0);	// memory dc compatible with screen
			::ReleaseDC(0, hDC0);	// don't need anymore. get rid of it.

			if (hDC && hBitmap){

				RECT rc = {0,0,cx,cy};
				int bpp = ::GetDeviceCaps(hDC, BITSPIXEL);

				HBITMAP hBitmapOld = (HBITMAP)SelectObject(hDC, hBitmap);

				// paint the background
				DWORD	dwBack = RGB(255, 255, 255); //GetSysColor(COLOR_WINDOW);
				DWORD OldColor = SetBkColor(hDC, dwBack);
				ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
				SetBkColor(hDC, OldColor);

				// Play the Metafile into Memory DC
				BOOL bRet = PlayEnhMetaFile(hDC, hMeta,	&rc);

				SelectObject(hDC, hBitmapOld);

				image = new CxImage();
				if(bRet && image->Create(cx, cy, bpp, CXIMAGE_FORMAT_WMF)){

					bRet = GetDIBits(hDC, hBitmap, 0,	(UINT)cy, image->GetBits(),
						(LPBITMAPINFO)image->GetDIB(), DIB_RGB_COLORS);	
					Result = true;
				}
				else
				{
					delete image;
					image = NULL;
				}
			}
			if (hBitmap) DeleteObject(hBitmap);
			if (hDC) DeleteDC(hDC);
		}
		CloseClipboard();
	}
	return Result;
}

PMBool PMCliboardManager::GetImage( TVecStrings &imagePaths )
{
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData = GetClipboardData(CF_HDROP);
		if(hData)		
		{
			DROPFILES* DFiles = (DROPFILES*)GlobalLock(hData);
			if(DFiles->fWide)//unicode
			{
				GetDropFiles((wchar_t*)((char*)DFiles+DFiles->pFiles),imagePaths);
			}else
			{
				GetDropFiles((char*)DFiles+DFiles->pFiles,imagePaths);
			}	
			GlobalUnlock(hData);
			TFile::FilesFilter(imagePaths,mPicture_Extends);
			if(!imagePaths.empty())Result = true;
		}
		
		CloseClipboard();
	}
	return Result;
}

PMBool PMCliboardManager::GetGrid( TBuffer &buffer )
{
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData;
		if(hData = GetClipboardData(mCF_Grid))
		{
			buffer.size		 = (PMInt)GlobalSize(hData);
			if(buffer.size)
			{
				PMChar* chBuffer = (PMChar*)GlobalLock(hData);
				buffer.buffer = (PMChar *)malloc(buffer.size);
				memcpy(buffer.buffer,chBuffer,buffer.size);
				GlobalUnlock(hData);
				Result = true;
			}
			buffer.useLen = buffer.size;
		}		
		CloseClipboard();
	}
	return Result;
}

PMBool PMCliboardManager::GetEditor( TBuffer &buffer )
{
	PMBool Result = false;
	if (OpenClipboard(mHWnd))
	{
		HANDLE hData;
		if(hData = GetClipboardData(mCF_Editor))
		{
			buffer.size		 = (PMInt)GlobalSize(hData);
			if(buffer.size)
			{
				PMChar* chBuffer = (PMChar*)GlobalLock(hData);
				buffer.buffer = (PMChar *)malloc(buffer.size);
				memcpy(buffer.buffer,chBuffer,buffer.size);
				GlobalUnlock(hData);
				Result = true;
			}
			buffer.useLen = buffer.size;
		}
		CloseClipboard();
	}
	return Result;
}

PMVoid PMCliboardManager::Free()
{
	mImageSize = 0;
	mGridSize  = 0;
	mEditorSize= 0;
	mText	   = "";
	mHDIB	   = NULL;
	if (mImage)
	{
		free(mImage);
		mImage = NULL;
	}
	if (mGrid)
	{
		free(mGrid);
		mGrid  = NULL;
	}
	if (mEditor)
	{
		free(mEditor);
		mEditor = NULL;
	}
}

