#include "StdAfx.h"
#include "NcEditWnd.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

CNcEditWnd::CNcEditWnd(void)
{
	m_strFileName=_T("");
	m_hSciLexerDll = LoadLibrary(_T("SciLexer.dll"));
	if (NULL == m_hSciLexerDll)
	{
		AfxMessageBox("LoadLibrary SciLexer.dll failure...");
	}
}

CNcEditWnd::~CNcEditWnd(void)
{
	if (NULL != m_hWnd)
	{
		DestroyWindow();
	}
	if (m_hSciLexerDll != NULL)
	{	
		::FreeLibrary(m_hSciLexerDll);
	}
}

BOOL CNcEditWnd::Create(DWORD dwExStyle,const RECT& rect, CWnd* pParentWnd)
{
	BOOL bRet;
	if (NULL == m_hSciLexerDll)
	{
		bRet= CWnd::CreateEx(dwExStyle,_T("EDIT"), _T(""), 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_UPPERCASE | ES_WANTRETURN, 
			rect,pParentWnd, IDC_STATIC);
	}
	else
	{
		bRet= CWnd::CreateEx(dwExStyle,_T("Scintilla"), _T(""), 
			WS_CHILD | WS_VISIBLE, rect,pParentWnd, IDC_STATIC);
	}
	return bRet;
}
BEGIN_MESSAGE_MAP(CNcEditWnd, CWnd)
END_MESSAGE_MAP()

void CNcEditWnd::SetDefaultColorFont(int nSize,const TCHAR* face)
{
	if (NULL == m_hSciLexerDll)
	{
		if (NULL != GetParent())
		{
			SetFont(GetParent()->GetFont());
		}
	}
	else
	{
		//选中行的颜色
		SendMessage(SCI_SETSELFORE,TRUE,RGB(255,255,255));
		SendMessage(SCI_SETSELBACK,TRUE,RGB(10,36,106));

		//默认文本颜色
		SendMessage(SCI_STYLESETFORE, STYLE_DEFAULT, RGB(0x00,0x00,0x00));
		SendMessage(SCI_STYLESETBACK, STYLE_DEFAULT, RGB(0xff,0xff,0xff));
		SendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, nSize);
		SendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<LPARAM>(face));
	}
}

void CNcEditWnd::UpdateLineNumberWidth(void)
{
	if (NULL == m_hSciLexerDll)
	{//dll未加载则退出
		return;
	}
	//start 显示行号
	char tchLines[32];
	int  iLineMarginWidthNow;
	int  iLineMarginWidthFit;
	_stprintf(tchLines,_T(" %d "),SendMessage(SCI_GETLINECOUNT,0,0));
	iLineMarginWidthNow = static_cast<int>(SendMessage(SCI_GETMARGINWIDTHN,0,0));
	iLineMarginWidthFit = static_cast<int>(SendMessage(SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)tchLines));
	if (iLineMarginWidthNow != iLineMarginWidthFit) 
	{
		SendMessage(SCI_SETMARGINWIDTHN,0,iLineMarginWidthFit); 
	}
	//end of 显示行号
}

void CNcEditWnd::SetNCEditStyle(void)
{
	if (NULL == m_hSciLexerDll)
	{//dll未加载则退出
		return;
	}
	SendMessage(SCI_SETLEXER, SCLEX_NCPROG);
	SendMessage(SCI_SETSTYLEBITS, 7);
     
	//显示当前行的淡黄色背景
	SendMessage(SCI_SETCARETLINEVISIBLE,TRUE,0);
	SendMessage(SCI_SETCARETLINEBACK, RGB(255,255,0),0);
	SendMessage(SCI_SETCARETLINEBACKALPHA,100,0);

	SendMessage(SCI_STYLESETFORE,typeG,					RGB(0x00,0x00,0xFF));
	SendMessage(SCI_STYLESETFORE,typeGValue,			RGB(0x33,0x00,0xFF));
	SendMessage(SCI_STYLESETFORE,typeM,					RGB(255,128,0));
	SendMessage(SCI_STYLESETFORE,typeMValue,			RGB(255,128,0));
	SendMessage(SCI_STYLESETFORE,typeX,					RGB(0,64,128));
	SendMessage(SCI_STYLESETFORE,typeXValue,			RGB(0,64,128));
	SendMessage(SCI_STYLESETFORE,typeY,					RGB(128,0,255));
	SendMessage(SCI_STYLESETFORE,typeYValue,			RGB(128,0,255));
	SendMessage(SCI_STYLESETFORE,typeI,					RGB(128,0,64));
	SendMessage(SCI_STYLESETFORE,typeIValue,			RGB(128,0,64));
	SendMessage(SCI_STYLESETFORE,typeJ,					RGB(255,0,0));
	SendMessage(SCI_STYLESETFORE,typeJValue,			RGB(255,0,0));
	SendMessage(SCI_STYLESETFORE,typeF,					RGB(255,33,33));
	SendMessage(SCI_STYLESETFORE,typeFValue,			RGB(255,33,33));
	SendMessage(SCI_STYLESETFORE,typeComment,			RGB(0x00,128,0x00));
	SendMessage(SCI_STYLESETFORE,typeNCDescription,		RGB(128,128,128));
	SendMessage(SCI_STYLESETFORE,typeNCLineNumber,		RGB(0x00,128,128));
	SendMessage(SCI_STYLESETFORE,typeNCLineNumberValue,	RGB(0x00,128,128));
	SendMessage(SCI_STYLESETFORE,typeProgramNumber,		RGB(255,128,64));
}

BOOL CNcEditWnd::OpenFile(LPCTSTR lpFileName)
{
	if (!::PathFileExists(lpFileName))
	{
		return FALSE;
	}

	CStdioFile stdFile(lpFileName,CFile::modeRead);
	ULONG nFileLength=static_cast<ULONG>(stdFile.GetLength());
	m_strFileName=stdFile.GetFilePath();
	
	TCHAR *pBuffer;
	pBuffer=new TCHAR[nFileLength+1];

	nFileLength=stdFile.Read(pBuffer,nFileLength);
	stdFile.Close();

	if (NULL == m_hSciLexerDll)
	{
		pBuffer[nFileLength] = '\0';
		SetWindowText(pBuffer);
	}
	else
	{
		if (SendMessage(SCI_GETREADONLY,0,0))
		{	
			SendMessage(SCI_SETREADONLY,FALSE,0);
		}

		SendMessage(SCI_CANCEL,0,0);
		SendMessage(SCI_SETUNDOCOLLECTION,0,0);
		SendMessage(SCI_EMPTYUNDOBUFFER,0,0);

		//如果文本没有只读属性，则清除所有文字。
		SendMessage(SCI_CLEARALL,0,0);

		//从所有行中删除标记，若markerNumber=-1，则删除所有标记。
		SendMessage(SCI_MARKERDELETEALL,(WPARAM)-1,0);

		SendMessage(SCI_ADDTEXT,nFileLength,(LPARAM)pBuffer);

		SendMessage(SCI_SETUNDOCOLLECTION,1,0);
		SendMessage(EM_EMPTYUNDOBUFFER,0,0);
		SendMessage(SCI_SETSAVEPOINT,0,0);
		SendMessage(SCI_GOTOPOS,0,0);
		SendMessage(SCI_CHOOSECARETX,0,0);

		UpdateLineNumberWidth();
	}

	delete [] pBuffer;

	return TRUE;
}

UINT CNcEditWnd::GetCurrentLine(void)
{
	int iPos = static_cast<int>(SendMessage(SCI_GETCURRENTPOS,0,0));
	int iLn = static_cast<int>(SendMessage(SCI_LINEFROMPOSITION,iPos,0)) + 1;
	return iLn;
}

void CNcEditWnd::InitNcEdit(int nSize,const TCHAR* face)
{
	//SendMessage(SCI_SETMARGINMASKN, 3, SC_MASK_FOLDERS);
	SendMessage(SCI_SETMARGINWIDTHN,1,4);
	UpdateLineNumberWidth();
	SetDefaultColorFont(nSize,face);
	SetNCEditStyle();
}

void CNcEditWnd::UpdateStyle(void)
{
	UpdateLineNumberWidth();
}

//自动换行
void CNcEditWnd::SetWrap(BOOL bWraped)
{
	if (NULL == m_hSciLexerDll)
	{//dll未加载则退出
		return;
	}
	if (bWraped){
		SendMessage(SCI_SETWRAPMODE,SC_WRAP_WORD,0); 
	}
	else{
		SendMessage(SCI_SETWRAPMODE,SC_WRAP_NONE,0); 
	}
}

void CNcEditWnd::ShowLineEndings(BOOL bShow)
{
	if (NULL == m_hSciLexerDll)
	{//dll未加载则退出
		return;
	}
	if (bShow){
		SendMessage(SCI_SETVIEWEOL,TRUE,0); 
	}
	else{
		SendMessage(SCI_SETVIEWEOL,FALSE,0); 
	}
}

void CNcEditWnd::SelectLine(int nLine)
{
	if (NULL == m_hSciLexerDll)
	{//dll未加载则退出
		return;
	}
	int startPos=SendMessage(SCI_POSITIONFROMLINE,nLine,0);
	int endPos=SendMessage(SCI_POSITIONFROMLINE,nLine+1,0);
	SendMessage(SCI_SETSEL,startPos,endPos);
}

BOOL CNcEditWnd::SaveFile(CString strFileName)
{
	UINT nFileOpenMode=0;
	CString FileNameForSave=strFileName;
	
	//若没有指定路径，则用原来的文件名保存文件。
	if (FileNameForSave.IsEmpty())
	{
		FileNameForSave=m_strFileName;
		nFileOpenMode=CFile::modeWrite;
 	}
	else
	{
		nFileOpenMode=CFile::modeWrite|CFile::modeCreate;
	}

	if (!PathFileExists(FileNameForSave))
	{
		return FALSE;
	}

	CStdioFile fNcFile(FileNameForSave,nFileOpenMode|CFile::typeText);
	if (NULL == m_hSciLexerDll)
	{
		CString csText;
		GetWindowText(csText);
		fNcFile.Write(csText,csText.GetLength());
	} 
	else
	{
		int nLength = SendMessage(SCI_GETLENGTH,0,0);
		TCHAR *szBuffer=new TCHAR[nLength + 1];
		SendMessage(SCI_GETTEXT,nLength + 1,(LPARAM)szBuffer);
		
		fNcFile.Write(szBuffer,nLength);
		delete [] szBuffer;
	}
	fNcFile.Close();


	return TRUE;
}
