# Media Player Classic - HC 源代码分析 5：关于对话框 （CAboutDlg） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月30日 00:18:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31











注：写了一系列分析Media Player Classic - HC 源代码的文章，在此列一个列表：

[Media Player Classic - HC 源代码分析 1：整体结构](http://blog.csdn.net/leixiaohua1020/article/details/13280659)
[Media Player Classic - HC 源代码分析 2：核心类 （CMainFrame）（1）](http://blog.csdn.net/leixiaohua1020/article/details/13290345)
[Media Player Classic - HC 源代码分析 3：核心类 （CMainFrame）（2）](http://blog.csdn.net/leixiaohua1020/article/details/13297291)
[Media Player Classic - HC 源代码分析 4：核心类 （CMainFrame）（3）](http://blog.csdn.net/leixiaohua1020/article/details/13298397)
[Media Player Classic - HC 源代码分析 5：关于对话框 （CAboutDlg）](http://blog.csdn.net/leixiaohua1020/article/details/13297555)
[Media Player Classic - HC 源代码分析 6：MediaInfo选项卡 （CPPageFileMediaInfo）](http://blog.csdn.net/leixiaohua1020/article/details/13297589)
[Media Player Classic - HC 源代码分析 7：详细信息选项卡（CPPageFileInfoDetails）](http://blog.csdn.net/leixiaohua1020/article/details/13297621)

![](https://img-blog.csdn.net/20140616104540718)

前几篇文章分析了Media Player Classic - HC（mpc-hc）的核心类（CMainFrame）：

[Media Player Classic - HC 源代码分析 2：核心类 （CMainFrame）（1）](http://blog.csdn.net/leixiaohua1020/article/details/13290345)


[Media Player Classic - HC 源代码分析 3：核心类 （CMainFrame）（2）](http://blog.csdn.net/leixiaohua1020/article/details/13297291)


[Media Player Classic - HC 源代码分析 4：核心类 （CMainFrame）（3）](http://blog.csdn.net/leixiaohua1020/article/details/13298397)

核心类已经分析的差不多了，现在可以看一看其他类的定义了。可是如此多的类，看看什么好呢？本着由易到难的原则，应该先看看“关于”对话框的代码。“关于”对话框作为mpc-hc系统的一部分，比较有代表性，而且代码相对来说十分简单，因而适合刚入门的人进行学习。

![](https://img-blog.csdn.net/20131028215522328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如图所示，“关于”对话框类的定义和实现都在最前面（因为开头是'A'......= =）。类的名字叫做CAboutDlg，定义位于AboutDlg.h，实现位于AboutDlg.cpp。

先看看“关于”对话框是什么样子的吧：

![](https://img-blog.csdn.net/20131028232812640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实相比于其他的“关于”对话框来说，这个还算是一个相对比较复杂的。包含了编译器信息，版本等等信息。

CAboutDlg定义如下所示：



```cpp
/*
 * (C) 2012 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <afxwin.h>
#include "resource.h"

class CAboutDlg : public CDialog
{
    CStatic m_icon;

    CString m_appname;
    CString m_strBuildNumber;
    CString m_MPCCompiler;
#ifndef MPCHC_LITE
    CString m_FFmpegCompiler;
#endif
    CString m_credits;
    CString m_AuthorsPath;

public:
    CAboutDlg();

    virtual BOOL OnInitDialog();

    afx_msg void OnHomepage(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnAuthors(NMHDR* pNMHDR, LRESULT* pResult);

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
```


从代码上来看。该对话框类和普通的MFC对话框类没有什么区别。不过这个“高端”的“关于”对话框确实包含了不少信息：mpc-hc版本，ffmpeg版本，编译器版本等等。这里就不再多说了，看看它类的实现部分的代码：



```cpp
/*
 * (C) 2012-2013 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "AboutDlg.h"
#include "mpc-hc_config.h"
#ifndef MPCHC_LITE
#include "FGFilterLAV.h"
#endif
#include "mplayerc.h"
#include "version.h"
#include "SysVersion.h"
#include "WinAPIUtils.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
    , m_appname(_T(""))
    , m_strBuildNumber(_T(""))
    , m_MPCCompiler(_T(""))
#ifndef MPCHC_LITE
    , m_LAVFiltersVersion(_T(""))
#endif
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}
//初始化
BOOL CAboutDlg::OnInitDialog()
{
    // Get the default text before it is overwritten by the call to __super::OnInitDialog()
    GetDlgItem(IDC_STATIC1)->GetWindowText(m_appname);
    GetDlgItem(IDC_AUTHORS_LINK)->GetWindowText(m_credits);
#ifndef MPCHC_LITE
    GetDlgItem(IDC_LAVFILTERS_VERSION)->GetWindowText(m_LAVFiltersVersion);
#endif

    __super::OnInitDialog();

    // Because we set LR_SHARED, there is no need to explicitly destroy the icon
    m_icon.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 48, 48, LR_SHARED));

#if MPC_BETA_RELEASE || _WIN64
    m_appname += _T(" (");
#endif

#if MPC_BETA_RELEASE
    m_appname += MPC_VERSION_BETA;
#endif

#if MPC_BETA_RELEASE && _WIN64
    m_appname += _T(", ");
#endif

#ifdef _WIN64
    m_appname += _T("64-bit");
#endif

#if MPC_BETA_RELEASE || _WIN64
    m_appname += _T(")");
#endif

#ifdef MPCHC_LITE
    m_appname += _T(" Lite");
#endif

    // Build the path to Authors.txt
    m_AuthorsPath = GetProgramPath() + _T("Authors.txt");
    // Check if the file exists
    if (FileExists(m_AuthorsPath)) {
        // If it does, we make the filename clickable
        m_credits.Replace(_T("Authors.txt"), _T("<a>Authors.txt</a>"));
    }

    m_homepage.Format(_T("<a>%s</a>"), WEBSITE_URL);

    m_strBuildNumber = MPC_VERSION_STR_FULL;

#if defined(__INTEL_COMPILER)
#if (__INTEL_COMPILER >= 1210)
    m_MPCCompiler = _T("ICL ") MAKE_STR(__INTEL_COMPILER) _T(" Build ") MAKE_STR(__INTEL_COMPILER_BUILD_DATE);
#else
#error Compiler is not supported!
#endif
#elif defined(_MSC_VER)
#if (_MSC_VER == 1700) // 2012
#if (_MSC_FULL_VER == 170060610)
    m_MPCCompiler = _T("MSVC 2012 Update 3");
#elif (_MSC_FULL_VER == 170060315)  // MSVC 2012 Update 2
#error VS2012 Update 2 is not supported because the binaries will not run on XP. Install Update 3 instead.
#elif (_MSC_FULL_VER == 170051106)
    m_MPCCompiler = _T("MSVC 2012 Update 1");
#elif (_MSC_FULL_VER < 170050727)   // MSVC 2012
#error Please install the latest Update for VS2012.
#else
    m_MPCCompiler = _T("MSVC 2012");
#endif
#elif (_MSC_VER == 1600) // 2010
#if (_MSC_FULL_VER >= 160040219)
    m_MPCCompiler = _T("MSVC 2010 SP1");
#else
    m_MPCCompiler = _T("MSVC 2010");
#endif
#elif (_MSC_VER < 1600)
#error Compiler is not supported!
#endif
#else
#error Please add support for your compiler
#endif

#if (__AVX__)
    m_MPCCompiler += _T(" (AVX)");
#elif (__SSSE3__)
    m_MPCCompiler += _T(" (SSSE3)");
#elif (__SSE3__)
    m_MPCCompiler += _T(" (SSE3)");
#elif !defined(_M_X64) && defined(_M_IX86_FP)
#if (_M_IX86_FP == 2)   // /arch:SSE2 was used
    m_MPCCompiler += _T(" (SSE2)");
#elif (_M_IX86_FP == 1) // /arch:SSE was used
    m_MPCCompiler += _T(" (SSE)");
#endif
#endif

#ifdef _DEBUG
    m_MPCCompiler += _T(" Debug");
#endif

#ifndef MPCHC_LITE
	//版本
    CString LAVFiltersVersion = CFGFilterLAV::GetVersion();
    if (!LAVFiltersVersion.IsEmpty()) {
        m_LAVFiltersVersion = LAVFiltersVersion;
    }
#endif

    m_buildDate = _T(__DATE__) _T(" ") _T(__TIME__);

    OSVERSIONINFOEX osVersion = SysVersion::GetFullVersion();
    m_OSName.Format(_T("Windows NT %1u.%1u (build %u"),
                    osVersion.dwMajorVersion, osVersion.dwMinorVersion, osVersion.dwBuildNumber);
    if (osVersion.szCSDVersion[0]) {
        m_OSName.AppendFormat(_T(", %s)"), osVersion.szCSDVersion);
    } else {
        m_OSName += _T(")");
    }
    m_OSVersion.Format(_T("%1u.%1u"), osVersion.dwMajorVersion, osVersion.dwMinorVersion);
    if (SysVersion::Is64Bit()) {
        m_OSVersion += _T(" (64-bit)");
    }

    UpdateData(FALSE);

    GetDlgItem(IDOK)->SetFocus();

    return FALSE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDR_MAINFRAME, m_icon);
    DDX_Text(pDX, IDC_STATIC1, m_appname);
    DDX_Text(pDX, IDC_AUTHORS_LINK, m_credits);
    DDX_Text(pDX, IDC_HOMEPAGE_LINK, m_homepage);
    DDX_Text(pDX, IDC_VERSION, m_strBuildNumber);
    DDX_Text(pDX, IDC_MPC_COMPILER, m_MPCCompiler);
#ifndef MPCHC_LITE
    DDX_Text(pDX, IDC_LAVFILTERS_VERSION, m_LAVFiltersVersion);
#endif
    DDX_Text(pDX, IDC_STATIC2, m_buildDate);
    DDX_Text(pDX, IDC_STATIC3, m_OSName);
    DDX_Text(pDX, IDC_STATIC4, m_OSVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_HOMEPAGE_LINK, OnHomepage)
    ON_NOTIFY(NM_CLICK, IDC_AUTHORS_LINK, OnAuthors)
    ON_BN_CLICKED(IDC_BUTTON1, OnCopyToClipboard)
END_MESSAGE_MAP()

void CAboutDlg::OnHomepage(NMHDR* pNMHDR, LRESULT* pResult)
{
    ShellExecute(m_hWnd, _T("open"), WEBSITE_URL, nullptr, nullptr, SW_SHOWDEFAULT);
    *pResult = 0;
}

void CAboutDlg::OnAuthors(NMHDR* pNMHDR, LRESULT* pResult)
{
    ShellExecute(m_hWnd, _T("open"), m_AuthorsPath, nullptr, nullptr, SW_SHOWDEFAULT);
    *pResult = 0;
}
//拷贝到剪切板
void CAboutDlg::OnCopyToClipboard()
{
	//把各种信息添加到一个字符串中
    CStringW info = m_appname;
    info += _T("\n----------------------------------\n\n");
    info += _T("Build information:\n");
    info += _T("    Version:            ") + m_strBuildNumber + _T("\n");
    info += _T("    MPC-HC compiler:    ") + m_MPCCompiler + _T("\n");
    info += _T("    Build date:         ") + m_buildDate + _T("\n\n");
#ifndef MPCHC_LITE
    info += _T("LAV Filters:\n");
    info += _T("    LAV Splitter:       ") + CFGFilterLAV::GetVersion(CFGFilterLAV::SPLITTER) + _T("\n");
    info += _T("    LAV Video:          ") + CFGFilterLAV::GetVersion(CFGFilterLAV::VIDEO_DECODER) + _T("\n");
    info += _T("    LAV Audio:          ") + CFGFilterLAV::GetVersion(CFGFilterLAV::AUDIO_DECODER) + _T("\n\n");
#endif
    info += _T("Operating system:\n");
    info += _T("    Name:               ") + m_OSName + _T("\n");
    info += _T("    Version:            ") + m_OSVersion + _T("\n");

    COleDataSource* pData = DEBUG_NEW COleDataSource();

    int len = info.GetLength() + 1;
    HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, len * sizeof(WCHAR));

    if (pData && hGlob) {
        wcscpy_s((WCHAR*)hGlob, len, (LPCWSTR)info);

        pData->CacheGlobalData(CF_UNICODETEXT, hGlob);

        // The system will take care of removing the allocated memory
        pData->SetClipboard();
    } else if (pData) {
        delete pData;
    } else if (hGlob) {
        GlobalFree(hGlob);
    }
}
```











代码整体上通俗易懂，调用的都是很常见的函数，不多解释了。








