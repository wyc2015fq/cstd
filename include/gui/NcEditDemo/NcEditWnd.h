//! NC 编辑器类，它封装了Scintilla
/*! 
 \author   : 淡月清风 
 \date     : 2008/12/13
 \version  : ver 0.1
 \filename : NcEditWnd.h
 \note     : 
        
 \attention: SciLexer.dll文件必须放在应用程序所在的目录下(%PATH%中可以找到也可以)\n

        
 \remark   : 1.2008/12/13 14:02 created by 淡月清风 version 0.1  
 \History:
   <author>       <time>       <version>         <desc>	
*/
#pragma once
#include <afxwin.h>
#include "Scintilla.h"
#include "SciLexer.h"

class CNcEditWnd : public CWnd
{
public:
	/**
	* 构造时加载Scintilla.dll  
	* @param void
	* @return 
	* @see
	*/
	CNcEditWnd(void);
	/**
	* 释放Scintilla.dll    
	* @param void
	* @return 
	* @see
	*/
	~CNcEditWnd(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	/**
	* 创建NC编辑器窗口。
	* @param DWORD dwExStyle：窗口样式
	* @param const RECT & rect：位置与大小
	* @param CWnd * pParentWnd：父窗口
	* @return BOOL：成功与否
	* @see
	*/
	virtual BOOL Create(DWORD dwExStyle,const RECT& rect, CWnd* pParentWnd);
	
	/**
	* 初始化编辑器窗口的各种属性。  
	* @param int nSize：字号
	* @param const TCHAR * face：字体名称
	* @return void
	* @see
	*/
	void InitNcEdit(int nSize,const TCHAR* face);

	/**
	* 打开NC程序  
	* @param LPCTSTR lpFileName
	* @return BOOL
	* @see
	*/
	BOOL OpenFile(LPCTSTR lpFileName);
	
	/**
	* 返回当前光标所在的行的行号  
	* @param void
	* @return UINT
	* @see
	*/
	UINT GetCurrentLine(void);
	
	/**
	* 刷新窗口样式，在父窗口的OnNotify中根据需要调用他。
	* @param void
	* @return void
	* @see
	*/
	void UpdateStyle(void);

	/**
	* 自动换行  
	* @param BOOL bWraped
	* @return void
	* @see
	*/
	void SetWrap(BOOL bWraped);

	/**
	* 显示行尾标志  
	* @param BOOL bShow
	* @return void
	* @see
	*/
	void ShowLineEndings(BOOL bShow);

	/**
	* 将指定的行设置为选中状态  
	* @param int nLine
	* @return void
	* @see
	*/
	void SelectLine(int nLine);

	/**
	* 保存文件。\n
	* 若不指定strFileName或者strFileName为空，则按原文件名存储。 
	* @param CString & strFileName
	* @return BOOL
	* @see
	*/
	BOOL SaveFile(CString strFileName=_T(""));
private:
	HMODULE m_hSciLexerDll;
	CString m_strFileName;
private:
	void SetDefaultColorFont(int nSize,const TCHAR* face);
	void UpdateLineNumberWidth(void);
	void SetNCEditStyle(void);
};
