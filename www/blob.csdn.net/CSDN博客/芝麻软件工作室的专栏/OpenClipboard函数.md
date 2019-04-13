
# OpenClipboard函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:25:31[seven-soft](https://me.csdn.net/softn)阅读数：319标签：[excel																](https://so.csdn.net/so/search/s.do?q=excel&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=excel&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=excel&t=blog)


多个软件之间进行数据共享是非常重要的，难以想像编辑软件没有CTRL+C，CTRL+V的功能，是多么的不方便。很多的操作，就是拷贝的动作，就是为了数据共享。剪贴板共享是Windows里比较重要的功能，比如很多采集数据的软件为了方便导数据到Excel里面，就可以使用剪贴板的功能。还有即见即所得的界面导到Word里面，也可以使用剪贴板的功能。
函数OpenClipboard声明如下：
WINUSERAPI
BOOL
WINAPI
OpenClipboard(
__in_opt HWND hWndNewOwner);
**hWndNewOwner**是前贴板所属于的窗口。
调用函数的例子如下：
\#001 //拷贝数据到剪贴板。
\#002  //
\#003  void TestClipBoard(void)
\#004  {
\#005         //打开剪贴板并清空。
**\#006        if (OpenClipboard(m_hWnd)**&&
\#007               EmptyClipboard())
\#008         {
\#009               //
\#010               HGLOBAL hMem;
\#011               std::wstring strText(_T("拷贝数据到剪贴板"));
\#012
\#013               //分配全局内存。
\#014               hMem = GlobalAlloc(GMEM_MOVEABLE,
\#015                    (strText.length() + 1) * sizeof(TCHAR));
\#016               if (hMem == NULL)
\#017               {
\#018                    CloseClipboard();
\#019                    return;
\#020               }
\#021
\#022               //拷贝数据到剪贴板内存。
\#023               LPTSTR lpStr = (LPTSTR)GlobalLock(hMem);
\#024               memcpy(lpStr, strText.c_str(),
\#025                    strText.length() * sizeof(TCHAR));
\#026               lpStr[strText.length()] = (TCHAR) 0;
\#027               GlobalUnlock(hMem);
\#028
\#029               //设置数据到剪贴板
\#030               SetClipboardData(CF_UNICODETEXT, hMem);
\#031
\#032               //关闭剪贴板。
\#033               CloseClipboard();
\#034         }
\#035  }
\#036


