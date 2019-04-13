
# GetSaveFileName函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月04日 17:08:23[seven-soft](https://me.csdn.net/softn)阅读数：292


前面介绍了怎么样打开选择文件读取的对话框，当你设计的软件需要让用户选择保存时，就需要让用户去选择自己合适的文件名称来保存。那么就需要使用到保存文件对话框，也就是API函数GetSaveFileName。下面就来演示怎么样使用这个API函数。
函数GetSaveFileName声明如下：
WINCOMMDLGAPI BOOL APIENTRY GetSaveFileNameA(LPOPENFILENAMEA);
WINCOMMDLGAPI BOOL APIENTRY GetSaveFileNameW(LPOPENFILENAMEW);
\#ifdef UNICODE
\#define GetSaveFileName GetSaveFileNameW
\#else
\#define GetSaveFileName GetSaveFileNameA
\#endif // !UNICODE
调用函数的例子如下：
\#001 //获取用户选择保存的文件名称。
\#002  //
\#003  void TestGetSaveFileName(void)
\#004  {
\#005         //
\#006         OPENFILENAME ofn;       //公共对话框结构。
\#007         TCHAR szFile[MAX_PATH]; //保存获取文件名称的缓冲区。
\#008
\#009         //初始化选择文件对话框。
\#010         ZeroMemory(&ofn, sizeof(ofn));
\#011        ofn.lStructSize = sizeof(ofn);
\#012         ofn.hwndOwner = m_hWnd;
\#013         ofn.lpstrFile = szFile;
\#014
\#015         //
\#016         ofn.lpstrFile[0] = _T('/0');
\#017         ofn.nMaxFile = sizeof(szFile);
\#018         ofn.lpstrFilter = _T("All/0*.*/0Text/0*.TXT/0");
\#019         ofn.nFilterIndex = 1;
\#020         ofn.lpstrFileTitle = NULL;
\#021         ofn.nMaxFileTitle = 0;
\#022         ofn.lpstrInitialDir = NULL;
\#023         ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
\#024
\#025         //显示打开选择文件对话框。
**\#026        if ( GetSaveFileName(&ofn) )**
\#027         {
\#028               //显示选择的文件。
\#029               OutputDebugString(szFile);
\#030               OutputDebugString(_T("/r/n"));
\#031         }
\#032  }


