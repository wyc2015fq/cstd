
# GetOpenFileName函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月04日 17:09:19[seven-soft](https://me.csdn.net/softn)阅读数：339标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[path																](https://so.csdn.net/so/search/s.do?q=path&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=path&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)


当用户想选择打开以前保存的文件时，就需要使用到选择文件对话框。其实在Windows的API里已经有这样完美的对话，只需要简单的调用，就可以使用了，而不需要写一大堆其它的代码。下面就来演示一下怎么使用文件选择对话框。
函数GetOpenFileName声明如下：
WINCOMMDLGAPI BOOL APIENTRY GetOpenFileNameA(LPOPENFILENAMEA);
WINCOMMDLGAPI BOOL APIENTRY GetOpenFileNameW(LPOPENFILENAMEW);
\#ifdef UNICODE
\#define GetOpenFileName GetOpenFileNameW
\#else
\#define GetOpenFileName GetOpenFileNameA
\#endif // !UNICODE
LPOPENFILENAMEA是指向文件选择对话框的结构。
调用函数的例子如下：
\#001 //获取用户选择的文件名称。
\#002  //
\#003  void TestGetOpenFileName(void)
\#004  {
\#005         //
\#006         OPENFILENAME ofn;       //公共对话框结构。
\#007         TCHAR szFile[MAX_PATH]; //保存获取文件名称的缓冲区。
\#008
\#009         //初始化选择文件对话框。
\#010         ZeroMemory(&ofn, sizeof(ofn));
\#011         ofn.lStructSize = sizeof(ofn);
\#012         ofn.hwndOwner = m_hWnd;
\#013         ofn.lpstrFile = szFile;
\#014         //
\#015         //
\#016         ofn.lpstrFile[0] = _T('/0');
\#017         ofn.nMaxFile = sizeof(szFile);
\#018         ofn.lpstrFilter = _T("All/0*.*/0Text/0*.TXT/0");
\#019         ofn.nFilterIndex = 1;
\#020         ofn.lpstrFileTitle = NULL;
\#021         ofn.nMaxFileTitle = 0;
\#022         ofn.lpstrInitialDir = NULL;
\#023         ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
\#024
\#025         //显示打开选择文件对话框。
\#026         if ( GetOpenFileName(&ofn) )
\#027         {
\#028               //显示选择的文件。
\#029               OutputDebugString(szFile);
\#030               OutputDebugString(_T("/r/n"));
\#031
\#032         }
\#033  }


