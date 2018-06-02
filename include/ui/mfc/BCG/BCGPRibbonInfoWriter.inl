//
// BCGPRibbonInfoWriter.cpp: implementation of the CBCGPRibbonInfoWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPRibbonInfoWriter.h"
#include "BCGPPngImage.h"

#include <io.h>
#include <direct.h>

#ifndef BCGP_EXCLUDE_RIBBON

static LPCTSTR s_szSLFRC          = _T("\n");

static LPCTSTR s_szXML_Prefix     = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

static LPCTSTR s_szComment        = _T("//");
static LPCTSTR s_szCommentLine    = _T("/////////////////////////////////////////////////////////////////////////////");

static LPCTSTR s_szExtensionRC2   = _T(".ribbon.rc2");
static LPCTSTR s_szExtensionXML   = _T(".xml");
static LPCTSTR s_szExtensionImage = _T(".png");

static LPCTSTR s_szTypeXML        = _T("BCGP_RIBBON_XML");
static LPCTSTR s_szTypeImage      = _T("PNG");

struct XFileID
{
	LPCTSTR szID;
	LPCTSTR szFile;
};

static XFileID s_DefaultXML              = {_T("_RIBBON_XML"), _T("ribbon")};
static XFileID s_DefaultImageRibbonBar   = {_T("_IMAGE_RIBBON_BAR"), _T("ribbon_bar")};
static XFileID s_DefaultImageMain        = {_T("_IMAGE_MAIN"), _T("main")};
static XFileID s_DefaultMainImageSmall   = {_T("_IMAGE_CATEGORY_MAIN_SMALL"), _T("category_main_small")};
static XFileID s_DefaultMainImageLarge   = {_T("_IMAGE_CATEGORY_MAIN_LARGE"), _T("category_main_large")};
static XFileID s_DefaultImageSmall       = {_T("_IMAGE_CATEGORY_SMALL"), _T("category_small")};
static XFileID s_DefaultImageLarge       = {_T("_IMAGE_CATEGORY_LARGE"), _T("category_large")};
static XFileID s_DefaultImageContextSmall = {_T("_IMAGE_CATEGORY_CONTEXT_SMALL"), _T("category_context_small")};
static XFileID s_DefaultImageContextLarge = {_T("_IMAGE_CATEGORY_CONTEXT_LARGE"), _T("category_context_large")};
static XFileID s_DefaultImageStatusBar   = {_T("_IMAGE_STATUS_BAR"), _T("status_bar")};
static XFileID s_DefaultImageStatusGroup = {_T("_IMAGE_STATUS_GROUP"), _T("status_group")};

namespace BCGPRibbonInfoWriter
{

static int StringToUTF8 (LPCTSTR lpSrc, LPSTR& lpDst, int nLength = -1)
{
	LPWSTR lpWide = NULL;
	int count = 0;
	BOOL bTerm = nLength == -1;

#ifdef _UNICODE
	lpWide = (LPWSTR)lpSrc;
#else
	count = ::MultiByteToWideChar (::GetACP (), 0, lpSrc, nLength, NULL, 0);
	if (!bTerm)
	{
		count++;
	}

	lpWide = new WCHAR[count];
	memset (lpWide, 0, count * sizeof(WCHAR));

	::MultiByteToWideChar (::GetACP (), 0, lpSrc, nLength, lpWide, count);
	count--;

	nLength = -1;
#endif

	count = ::WideCharToMultiByte (CP_UTF8, 0, lpWide, nLength, NULL, 0, NULL, 0);
	if (count > 1)
	{
		if (!bTerm)
		{
			count++;
		}

		lpDst = new char[count];
		memset (lpDst, 0, count);

		::WideCharToMultiByte (CP_UTF8, 0, lpWide, nLength, lpDst, count, NULL, 0);
		count--;
	}

	if (lpWide != (LPWSTR)lpSrc)
	{
		delete [] lpWide;
	}

	return count;
}

static BOOL FileExists (const CString& fileName)
{
	return _taccess (fileName, 0) != -1;
}

static BOOL IsFileReadOnly (const CString& fileName)
{
	if (FileExists (fileName))
	{
		return (GetFileAttributes(fileName) & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY;
	}

	return FALSE;
}

static CString CorrectDirectoryName(const CString& dirName)
{
    CString dir(dirName);

    if (!dir.IsEmpty())
    {
        if (dir.GetAt(dir.GetLength() - 1) != TCHAR('\\'))
        {
            dir += TCHAR('\\');
        }
    }

    return dir;
}

static BOOL CreateDirectory (const CString& dirName)
{
	if (FileExists (dirName))
	{
		return TRUE;
	}

	if (dirName.IsEmpty())
	{
		return FALSE;
	}

	CStringArray sa;
	CString s(dirName);
	
	int pos = s.Find(TCHAR('\\'));

	while (pos != -1)
	{
		CString sp = s.Left(pos);
		s = s.Right(s.GetLength() - pos - 1);

		if (!sp.IsEmpty())
		{
			sa.Add(sp);
		}

		pos = s.Find(TCHAR('\\'));

		if (pos == -1 && !s.IsEmpty())
		{
			sa.Add(s);
		}
	}

	BOOL bRes = TRUE;

	CString strDir;
	for (int i = 0; i < sa.GetSize (); i++)
	{
		strDir += sa[i];
		if (!FileExists (strDir))
		{
			if (i == 0)
			{
				bRes = FALSE;
				break;
			}

			if (_tmkdir (strDir) == -1)
			{
				bRes = FALSE;
				break;
			}
		}
		else if ((GetFileAttributes (strDir) & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			bRes = FALSE;
			break;
		}

		strDir += TCHAR('\\');
	}

	return bRes;
}

static CString PrepareFileName (const CString& fileName)
{
	CString strFile (fileName);

	if (!strFile.IsEmpty())
	{
		strFile.TrimLeft (_T(" "));
		strFile.TrimRight (_T(" "));

		if (strFile[0] == TCHAR('\"'))
		{
			strFile.TrimLeft (_T("\""));
			strFile.TrimRight (_T("\""));
		}

		if (strFile.Find (TCHAR('/')) != -1)
		{
			strFile.Replace (TCHAR('/'), TCHAR('\\'));
		}

		if (strFile.Find (_T(".\\")) == 0)
		{
			strFile = strFile.Right (strFile.GetLength () - 2);
		}
	}
	
	return strFile;
}

static CString CorrectFileName(const CString& dirName, const CString& fileName)
{
	CString str;
	AfxFullPath (str.GetBuffer (_MAX_PATH), CorrectDirectoryName (dirName) + fileName);
	str.ReleaseBuffer ();

	return str;
}

class CFileWriter
{
public:
	CFileWriter()
	{
	}
	virtual ~CFileWriter()
	{
	}

	virtual BOOL Write (const CString& filename)
	{
		if (IsFileReadOnly (filename))
		{
			//CFileParser::ReportSaveLoadException (filename, NULL, TRUE, IDP_ERROR_FILE_READONLY);
			return FALSE;
		}

		CFile* pFile = new CStdioFile;
		CFileException fe;

		if (!pFile->Open (filename, GetFlags (), &fe))
		{
			delete pFile;

			//CFileParser::ReportSaveLoadException (filename, &fe, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
			return FALSE;
		}

		BOOL bResult = FALSE;

		TRY
		{
			CWaitCursor wait;

			bResult = Write (*pFile);
			
			pFile->Close();
			delete pFile;
		}
		CATCH_ALL(e)
		{
			pFile->Abort();
			delete pFile;

			TRY
			{
				//CFileParser::ReportSaveLoadException(filename, e, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
			}
			END_TRY
			do { e->Delete(); } while (0);//DELETE_EXCEPTION(e);
		}
		END_CATCH_ALL

		return bResult;
	}

protected:
	virtual DWORD GetFlags () const
	{
		return CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive;
	}
	virtual BOOL Write (CFile& /*file*/)
	{
		return FALSE;
	}
};

class CFileWriterText: public CFileWriter
{
public:
	CFileWriterText(const CStringArray& text)
		: CFileWriter ()
		, m_Text      (text)
	{

	}
	virtual ~CFileWriterText()
	{
	}

protected:
	virtual BOOL Write (CFile& file)
	{
		CStdioFile& f = static_cast<CStdioFile&>(file);

		int count = (int) m_Text.GetSize ();

		for (int i = 0; i < count; i++)
		{
			f.WriteString (m_Text[i]);
			if (i < (count - 1))
			{
				f.WriteString (s_szSLFRC);
			}
		}

		return TRUE;
	}

private:
	const CStringArray& m_Text;
};

class CFileWriterBinary: public CFileWriter
{
public:
	CFileWriterBinary (LPCVOID lpBuffer, DWORD dwCount)
		: CFileWriter ()
		, m_lpBuffer  (lpBuffer)
		, m_dwCount   (dwCount)
	{
	}
	virtual ~CFileWriterBinary ()
	{
	}

protected:
	virtual BOOL Write (CFile& file)
	{
		if (m_lpBuffer != NULL &&
			m_dwCount > 0)
		{
			file.Write (m_lpBuffer, m_dwCount);
		}

		return TRUE;
	}

protected:
	virtual DWORD GetFlags () const
	{
		return CFileWriter::GetFlags () | CFile::typeBinary;
	}

private:
	LPCVOID	m_lpBuffer;
	DWORD	m_dwCount;
};

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonInfoWriter::CBCGPRibbonInfoWriter(CBCGPRibbonInfo& info)
	: m_Info (info)
{
}

CBCGPRibbonInfoWriter::~CBCGPRibbonInfoWriter()
{
	EmptyResources ();
}

BOOL CBCGPRibbonInfoWriter::Save (const CString& strFolder, 
								  const CString& strResourceName, 
								  const CString& strResourceFolder,
								  const CString& strResourcePrefix)
{
	if (strFolder.IsEmpty () ||
		strResourceName.IsEmpty ())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	EmptyResources ();

	m_Folder = BCGPRibbonInfoWriter::CorrectDirectoryName (strFolder);
	m_ResourceFolder = BCGPRibbonInfoWriter::CorrectDirectoryName (strResourceFolder);
	m_ResourceName  = strResourceName;
	m_ResourcePrefix = strResourcePrefix;

	if (m_ResourceName.Find (s_szExtensionRC2) == -1)
	{
		m_ResourceName += s_szExtensionRC2;
	}

	if (!Prepare ())
	{
		return FALSE;
	}

	if (!SaveXML ())
	{
		return FALSE;
	}

	if (!SaveImages ())
	{
		return FALSE;
	}

	if (!SaveRC ())
	{
		return FALSE;
	}

	return TRUE;
}

CString CBCGPRibbonInfoWriter::GetFileName (const CString& strFileName) const
{
	return BCGPRibbonInfoWriter::CorrectFileName (m_Folder, BCGPRibbonInfoWriter::PrepareFileName (strFileName));
}

void CBCGPRibbonInfoWriter::CorrectImage (CBCGPRibbonInfo::XImage& image, const CString& strID, const CString& strFile)
{
	if (image.m_Image.IsValid ())
	{
		CString str = m_ResourcePrefix + strID;

		if (!image.m_strID.IsEmpty () && str != image.m_strID)
		{
			image.m_strID.Empty ();
		}

		if (image.m_strID.IsEmpty ())
		{
			image.m_strID = str;
			AddResource (XResourceFile (image.m_strID, 
				s_szTypeImage,
				m_ResourceFolder + strFile + s_szExtensionImage));
		}
	}
	else
	{
		if (!image.m_strID.IsEmpty ())
		{
			image.m_strID.Empty ();
		}
	}
}

void CBCGPRibbonInfoWriter::CorrectImages ()
{
	CBCGPRibbonInfo::XRibbonBar& infoRB = GetInfo ().GetRibbonBar ();

	CorrectImage (infoRB.m_Images, s_DefaultImageRibbonBar.szID, s_DefaultImageRibbonBar.szFile);

	if (infoRB.m_btnMain != NULL)
	{
		CorrectImage (infoRB.m_btnMain->m_Image, s_DefaultImageMain.szID, s_DefaultImageMain.szFile);
	}

	if (infoRB.m_MainCategory != NULL)
	{
		CorrectImage (infoRB.m_MainCategory->m_SmallImages, s_DefaultMainImageSmall.szID, s_DefaultMainImageSmall.szFile);
		CorrectImage (infoRB.m_MainCategory->m_LargeImages, s_DefaultMainImageLarge.szID, s_DefaultMainImageLarge.szFile);
	}

	int i = 0;
	for (i = 0; i < infoRB.m_arCategories.GetSize (); i++)
	{
		CBCGPRibbonInfo::XCategory* category = infoRB.m_arCategories[i];

		CString strID;
		CString strFile;

		strID.Format (_T("%s_%d"), s_DefaultImageSmall.szID, i);
		strFile.Format (_T("%s_%d"), s_DefaultImageSmall.szFile, i);
		CorrectImage (category->m_SmallImages, strID, strFile);

		strID.Format (_T("%s_%d"), s_DefaultImageLarge.szID, i);
		strFile.Format (_T("%s_%d"), s_DefaultImageLarge.szFile, i);
		CorrectImage (category->m_LargeImages, strID, strFile);
	}

	for (i = 0; i < infoRB.m_arContexts.GetSize (); i++)
	{
		CBCGPRibbonInfo::XContext* context = infoRB.m_arContexts[i];

		for (int j = 0; j < context->m_arCategories.GetSize (); j++)
		{
			CBCGPRibbonInfo::XCategory* category = context->m_arCategories[j];

			CString strID;
			CString strFile;

			strID.Format (_T("%s_%d_%d"), s_DefaultImageContextSmall.szID, i, j);
			strFile.Format (_T("%s_%d_%d"), s_DefaultImageContextSmall.szFile, i, j);
			CorrectImage (category->m_SmallImages, strID, strFile);

			strID.Format (_T("%s_%d_%d"), s_DefaultImageContextLarge.szID, i, j);
			strFile.Format (_T("%s_%d_%d"), s_DefaultImageContextLarge.szFile, i, j);
			CorrectImage (category->m_LargeImages, strID, strFile);
		}
	}

	CBCGPRibbonInfo::XStatusBar& infoSB = GetInfo ().GetStatusBar ();

	CorrectImage (infoSB.m_Images, s_DefaultImageStatusBar.szID, s_DefaultImageStatusBar.szFile);

	int count = 0;
	for (i = 0; i < infoSB.m_Elements.m_arElements.GetSize (); i++)
	{
		if (infoSB.m_Elements.m_arElements[i]->GetElementName ().Compare (CBCGPRibbonInfo::s_szGroup) == 0)
		{
			CString strID;
			CString strFile;

			strID.Format (_T("%s_%d"), s_DefaultImageStatusGroup.szID, count);
			strFile.Format (_T("%s_%d"), s_DefaultImageStatusGroup.szFile, count);
			CorrectImage (((CBCGPRibbonInfo::XElementGroup*)infoSB.m_Elements.m_arElements[i])->m_Images, strID, strFile);
			count++;
		}
	}
	for (i = 0; i < infoSB.m_ExElements.m_arElements.GetSize (); i++)
	{
		if (infoSB.m_ExElements.m_arElements[i]->GetElementName ().Compare (CBCGPRibbonInfo::s_szGroup) == 0)
		{
			CString strID;
			CString strFile;

			strID.Format (_T("%s_%d"), s_DefaultImageStatusGroup.szID, count);
			strFile.Format (_T("%s_%d"), s_DefaultImageStatusGroup.szFile, count);
			CorrectImage (((CBCGPRibbonInfo::XElementGroup*)infoSB.m_ExElements.m_arElements[i])->m_Images, strID, strFile);
			count++;
		}
	}
}

void CBCGPRibbonInfoWriter::CorrectInfo ()
{
	CorrectImages ();
}

void CBCGPRibbonInfoWriter::CorrectNames ()
{
	AddResource (XResourceFile (m_ResourcePrefix + s_DefaultXML.szID, 
			s_szTypeXML, m_ResourceFolder + CString (s_DefaultXML.szFile) + s_szExtensionXML));
}

BOOL CBCGPRibbonInfoWriter::Prepare ()
{
	CString strDir (m_Folder + m_ResourceFolder);
	if (!BCGPRibbonInfoWriter::CreateDirectory (strDir))
	{
		ErrorReportFolder (strDir);

		return FALSE;
	}

	CorrectInfo ();
	CorrectNames ();

	CStringArray sa;

	// RC2 file
	sa.Add (strDir + m_ResourceName);

	// XML file
	{
		const XResourceFileArray* array = GetResourcesArray (s_szTypeXML);
		if (array == NULL || array->GetSize () == 0)
		{
			return FALSE;
		}

		sa.Add (GetFileName (array->GetAt (0)->GetPath ()));
	}

	// Image files
	{
		const XResourceFileArray* array = GetResourcesArray (s_szTypeImage);
		if (array != NULL)
		{
			for (int i = 0; i < array->GetSize (); i++)
			{
				sa.Add (GetFileName (array->GetAt (i)->GetPath ()));
			}
		}
	}

	BOOL bRes = TRUE;

	for (int i = 0; i < sa.GetSize (); i++)
	{
		if (!BCGPRibbonInfoWriter::FileExists (sa[i]))
		{
			continue;
		}

		if (BCGPRibbonInfoWriter::IsFileReadOnly (sa[i]))
		{
			ErrorReportFileRO (sa[i]);

			bRes = FALSE;
			break;
		}
	}

	return bRes;
}

BOOL CBCGPRibbonInfoWriter::SaveRC ()
{
	CStringArray text;

	text.Add (_T("// BCGSoft Designer automatically generated file."));
	text.Add (_T("// Do not edit."));
	text.Add (s_szComment);
	text.Add (_T(""));
	text.Add (_T("#ifdef APSTUDIO_INVOKED"));
	text.Add (_T("	#error this file is not editable by Microsoft Visual C++"));
	text.Add (_T("#endif //APSTUDIO_INVOKED"));
	text.Add (_T(""));

	{
		const XResourceFileArray* array = GetResourcesArray (s_szTypeXML);
		if (array != NULL && array->GetSize () == 1)
		{
			text.Add (s_szCommentLine);
			text.Add (s_szComment);
			text.Add (_T("// ") + CString (s_szTypeXML));
			text.Add (s_szComment);
			text.Add (_T(""));

			text.Add (array->GetAt (0)->Format ());

			text.Add (_T(""));
		}
	}

	{
		const XResourceFileArray* array = GetResourcesArray (s_szTypeImage);
		if (array != NULL && array->GetSize () > 0)
		{
			text.Add (s_szCommentLine);
			text.Add (s_szComment);
			text.Add (_T("// ") + CString (s_szTypeImage));
			text.Add (s_szComment);
			text.Add (_T(""));

			for (int i = 0; i < array->GetSize (); i++)
			{
				text.Add (array->GetAt (i)->Format ());
			}

			text.Add (_T(""));
		}
	}

	BCGPRibbonInfoWriter::CFileWriterText writer (text);
	return writer.CFileWriter::Write (m_Folder + m_ResourceFolder + m_ResourceName);
}

BOOL CBCGPRibbonInfoWriter::SaveXML ()
{
	const XResourceFileArray* array = GetResourcesArray (s_szTypeXML);
	if (array == NULL || array->GetSize () != 1)
	{
		return FALSE;
	}

	CString strBuffer (s_szXML_Prefix);
	GetInfo ().ToTag (strBuffer);

	BOOL bRes = FALSE;

	LPSTR lpBuffer = NULL;
	int count = BCGPRibbonInfoWriter::StringToUTF8 ((LPCTSTR)strBuffer, lpBuffer);

	if (lpBuffer != NULL)
	{
		BCGPRibbonInfoWriter::CFileWriterBinary writer ((LPCVOID)lpBuffer, count);
		bRes = writer.CFileWriter::Write (GetFileName (array->GetAt (0)->GetPath ()));

		delete [] lpBuffer;
	}

	return bRes;
}

BOOL CBCGPRibbonInfoWriter::SaveImages ()
{
	const XResourceFileArray* array = GetResourcesArray (s_szTypeImage);
	if (array == NULL || array->GetSize () == 0)
	{
		return TRUE;
	}

	CBCGPRibbonInfo::XArrayImages images;
	GetInfo ().GetArrayImages (images);

	BOOL bRes = TRUE;

	for (int i = 0; i < images.GetSize (); i++)
	{
		if (images[i]->m_strID.IsEmpty())
		{
			continue;
		}

		int nIndex = GetResourceIndex (*array, images[i]->m_strID);
		if (nIndex == -1)
		{
			continue;
		}

		CBCGPPngImage pngImage;
		pngImage.Attach (images[i]->m_Image.GetImageWell ());
		BOOL bSave = pngImage.SaveToFile (GetFileName (array->GetAt (nIndex)->GetPath ()));
		pngImage.Detach ();

		if (!bSave)
		{
			bRes = FALSE;
			break;
		}
	}

	return bRes;
}

void CBCGPRibbonInfoWriter::ErrorReportFolder (const CString& /*strName*/) const
{
}

void CBCGPRibbonInfoWriter::ErrorReportFileRO (const CString& /*strName*/) const
{
}

CString CBCGPRibbonInfoWriter::XResourceFile::Format () const
{
	CString str;

	if (IsValid ())
	{
		CString strPath (m_Path);
		strPath.Replace (_T("\\"), _T("\\\\"));

		str.Format (_T("%s %s \"%s\""), GetID (), GetType (), strPath);
	}

	return str;
}

int CBCGPRibbonInfoWriter::GetResourceIndex (const XResourceFileArray& array, const CString& strID)
{
	int nRes = -1;

	for (int i = 0; i < array.GetSize (); i++)
	{
		if (array[i]->GetID () == strID)
		{
			nRes = i;
			break;
		}
	}

	return nRes;
}

CBCGPRibbonInfoWriter::XResourceFileArray* CBCGPRibbonInfoWriter::GetResourcesArray (const CString& strType) const
{
	XResourceFileArray* array = NULL;

	m_Files.Lookup (strType, (void*&)array);

	return array;
}

BOOL CBCGPRibbonInfoWriter::AddResource (const XResourceFile& resource)
{
	if (!resource.IsValid ())
	{
		return FALSE;
	}

	BOOL bAdd = FALSE;
	XResourceFileArray* array = GetResourcesArray (resource.GetType ());
	if (array == NULL)
	{
		array = new XResourceFileArray;
		m_Files[resource.GetType ()] = array;
		bAdd = TRUE;
	}

	if (bAdd || GetResourceIndex (*array, resource.GetID ()) == -1)
	{
		array->Add (new XResourceFile(resource.GetID (), resource.GetType (), resource.GetPath ()));

		return TRUE;
	}

	return FALSE;
}

void CBCGPRibbonInfoWriter::EmptyResources ()
{
	CString strKey;
	XResourceFileArray* value = NULL;

	POSITION pos = m_Files.GetStartPosition ();
	while (pos != NULL)
	{
		m_Files.GetNextAssoc (pos, strKey, (void*&)value);
		if (value != NULL)
		{
			for (int i = 0; i < value->GetSize (); i++)
			{
				if (value->GetAt (i) != NULL)
				{
					delete value->GetAt (i);
				}
			}

			value->RemoveAll ();
			delete value;
		}
	}

	m_Files.RemoveAll ();
}

#endif // BCGP_EXCLUDE_RIBBON
