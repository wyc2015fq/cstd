/********************************************************************

	ShellPidl.cpp

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件shell pidl
					类实现文件。

	本文件编写人：	
					Selom Ofori		none
					YZ				yzfree##sina.com

	本文件版本：	30119
	最后修改于：	2003-1-19

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。
	----------------------------------------------------------------
	引用注释：

	本文件的原始作者为 Selom Ofori，源码没有许可限制。后由YZ改写，并入
	ISee项目。许可证设为GPL。因 Selom Ofori 没有留下联系地址，所以未能
	征求他的意见。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/


#include "stdafx.h"

#include "ShellPidl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/****************************************************************************
*
*    函数: Next(LPCITEMIDLIST pidl)
*
*    用途: 获取PIDL的下一个元素标识地址
*
****************************************************************************/
LPITEMIDLIST CShellPidl::Next(LPCITEMIDLIST pidl)
{
   LPSTR lpMem = (LPSTR)pidl;

   lpMem += pidl->mkid.cb;

   return (LPITEMIDLIST)lpMem;
}


/****************************************************************************
*
*    函数: GetSize(LPCITEMIDLIST pidl)
*
*    用途:  获取整个PIDL的尺寸（字节单位）
*
****************************************************************************/
UINT CShellPidl::GetSize(LPCITEMIDLIST pidl)
{
    UINT cbTotal = 0;

    if (pidl)
    {
        while (pidl->mkid.cb)
        {
            cbTotal += pidl->mkid.cb;
            pidl = Next(pidl);
        }

        cbTotal += sizeof(pidl->mkid.cb);				// 加上结尾的双'\0'符
    }

    return cbTotal;
}


/****************************************************************************
*
*    函数: CreatePidl(UINT cbSize)
*
*    用途: 分配一个空的、指定尺寸的PIDL块
*
****************************************************************************/
LPITEMIDLIST CShellPidl::CreatePidl(UINT cbSize)
{
    LPMALLOC		lpMalloc;
    LPITEMIDLIST	pidl;

    if (FAILED(SHGetMalloc(&lpMalloc)))					// 获取Shell的分配接口
	{
       return 0;
	}
	
    pidl = (LPITEMIDLIST)lpMalloc->Alloc(cbSize);		// 分配PIDL内存块

    if (pidl)
	{
        memset((void*)pidl, 0, cbSize);					// 初始化内存块内容为0
	}
	
	lpMalloc->Release();

    return pidl;
}


/****************************************************************************
*
*    函数: ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
*
*    用途: 链接两个指定的PIDL，并以新PIDL的形式返回
*
****************************************************************************/
LPITEMIDLIST CShellPidl::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST	pidlNew;
    UINT			cb1;
    UINT			cb2;

    if (pidl1)
	{
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);	// 第一个PIDL结尾的16位0值不在计算范围内
	}
    else
	{
		cb1 = 0;
	}

	ASSERT(pidl2);

	cb2 = GetSize(pidl2);

    if ((pidlNew=CreatePidl(cb1+cb2)))
    {
        if (pidl1)
		{
			memcpy((void*)pidlNew, pidl1, cb1);
		}
        memcpy(((LPSTR)pidlNew)+cb1, pidl2, cb2);		// 链接两个PIDL的内容
    }

    return pidlNew;
}


/****************************************************************************
*
*    函数: CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
*
*    用途: 复制lpi中第一个元素标识符结构，并形成一个新的PIDL返回
*
****************************************************************************/
LPITEMIDLIST CShellPidl::CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
   LPITEMIDLIST lpiTemp;

   if (lpiTemp=(LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb)))
   {
	   memcpy((void*)lpiTemp, (const void *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));
   }

   return lpiTemp;
}


/****************************************************************************
*
*    函数: GetName(LPSHELLFOLDER lpsf,LPITEMIDLIST  lpi,DWORD dwFlags, LPSTR lpFriendlyName)
*
*    用途: 获取指定目录或文件的显示名
*
****************************************************************************/
BOOL CShellPidl::GetName(LPSHELLFOLDER lpsf,
							LPITEMIDLIST  lpi,
							DWORD         dwFlags,
							LPSTR         lpFriendlyName)
{
	BOOL   bSuccess = TRUE;
	STRRET str;

	if (NOERROR == lpsf->GetDisplayNameOf(lpi, dwFlags, &str))
	{
		switch (str.uType)
		{
		case STRRET_WSTR:
			WideCharToMultiByte(CP_ACP,                 // 采用ANSI代码页
								0,
								str.pOleStr,            // 原宽字符串地址
								-1,
								lpFriendlyName,         // 目的缓冲区
								MAX_PATH,
								NULL, NULL);
			break;
		case STRRET_OFFSET:
			lstrcpy(lpFriendlyName, (LPSTR)lpi+str.uOffset);
			break;
		case STRRET_CSTR:
			lstrcpy(lpFriendlyName, (LPSTR)str.cStr);
			break;
		default:
			bSuccess = FALSE;
			break;
		}
	}
	else
	{
		bSuccess = FALSE;
	}

	return bSuccess;
}


/****************************************************************************
*
*    函数: GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
*
*    用途: 获取指定文件夹的绝对PIDL（从桌面到该文件夹）
*
****************************************************************************/
LPITEMIDLIST CShellPidl::GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
{
	char			szBuff[MAX_PATH];
	OLECHAR			szOleChar[MAX_PATH];
	LPSHELLFOLDER	lpsfDeskTop;
	LPITEMIDLIST	lpifq;
	ULONG			ulEaten = 0, ulAttribs = 0;
	HRESULT			hr;

	if (GetName(lpsf, lpi, SHGDN_FORPARSING, szBuff) == FALSE)
	{
		return NULL;
	}

	if (FAILED(SHGetDesktopFolder(&lpsfDeskTop)))		// 获取根文件夹接口（桌面）
	{
		return NULL;
	}

	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						szBuff,
						-1,
						(USHORT *)szOleChar,
						sizeof(szOleChar));

	hr = lpsfDeskTop->ParseDisplayName(NULL,			// 转换路径到PIDL
										NULL,
										szOleChar,
										&ulEaten,
										&lpifq,
										&ulAttribs);

	lpsfDeskTop->Release();

	if (FAILED(hr))
	{
		return NULL;
	}

	return lpifq;
}


/****************************************************************************
*
*  函数: DoTheMenuThing(HWND hwnd, 
*                           LPSHELLFOLDER lpsfParent,
*                           LPITEMIDLIST  lpi,
*                           LPPOINT lppt)
*
*  用途: 显示一个Shell元素的上下文菜单（即通常的右键菜单）
*
*
*  参数:
*    hwnd       - 父窗口句柄
*    lpsfParent - 元素的父文件夹接口
*    lpi        - 元素的PIDL
*    lppt       - 弹出菜单的左上角位置
*
*  返回:
*    TRUE - 成功, FALSE - 失败
*
****************************************************************************/
BOOL CShellPidl::DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST lpi, LPPOINT lppt)
{
    LPCONTEXTMENU		lpcm;
    HRESULT				hr;
    char				szTemp[64];
    CMINVOKECOMMANDINFO cmi;
    DWORD               dwAttribs=0;
    int                 idCmd;
    HMENU               hMenu;
    BOOL                bSuccess=TRUE;

	// 获取元素的上下文菜单接口
    hr = lpsfParent->GetUIObjectOf(hwnd,
									1,					// 只有一个元素
									(const struct _ITEMIDLIST **)&lpi,
									IID_IContextMenu,
									0,
									(LPVOID *)&lpcm);

    if (SUCCEEDED(hr))  
    {
       hMenu = CreatePopupMenu();

       if (hMenu)
       {
          hr = lpcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE);

          if (SUCCEEDED(hr))
          {
             idCmd = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
									lppt->x, lppt->y, 0, hwnd, NULL);

             if (idCmd)
             {
                cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                cmi.fMask  = 0;
                cmi.hwnd   = hwnd;
                cmi.lpVerb = MAKEINTRESOURCE(idCmd-1);
                cmi.lpParameters = NULL;
      	        cmi.lpDirectory  = NULL;
                cmi.nShow        = SW_SHOWNORMAL;
                cmi.dwHotKey     = 0;
                cmi.hIcon        = NULL;

                hr = lpcm->InvokeCommand(&cmi);

                if (!SUCCEEDED(hr))  
                {
                   wsprintf(szTemp, "调用命令失败！返回码[%lx]", hr);
                   AfxMessageBox(szTemp);
                }
             }
          }
          else
		  {
			  bSuccess = FALSE;
		  }

          DestroyMenu(hMenu);
       }
       else
	   {
		   bSuccess = FALSE;
	   }

       lpcm->Release();
    } 
    else
    {
       wsprintf(szTemp, "GetUIObjectOf() 函数调用失败！返回码[%lx]", hr);
       AfxMessageBox(szTemp );
       bSuccess = FALSE;
    }

    return bSuccess;
}


/****************************************************************************
*
*    函数: GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
*
*    用途: 获取由PIDL指定的元素在系统图象表中的图标索引号。
*
*    参数:
*		lpi    - 元素的绝对PIDL
*		uFlags - 待获取的图标类型（见MSDN中有关SHGetFileInfo()函数的说明）
*
*	返回:
*		元素的图标索引
****************************************************************************/
int CShellPidl::GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
{
   SHFILEINFO	sfi;

   SHGetFileInfo((LPCSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), uFlags);

   return sfi.iIcon;
}
