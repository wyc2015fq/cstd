
# GetClipboardData函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:24:08[seven-soft](https://me.csdn.net/softn)阅读数：267标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



前面介绍怎么样把数据放到剪贴板里面，那么又是怎么样从剪贴板里面获取数据出来呢？当然还是需要使用其它的API函数来获取剪贴板里的数据。获取剪贴板里的数据时，是不知道当前剪贴板里是否有数据的，也不知道剪贴板里的数据格式是什么。那么下面就来解决这两个问题，先使用函数IsClipboardFormatAvailable来获取剪贴板里的格式是否可以处理，接着使用函数OpenClipboard打开剪贴板，然后使用函数GetClipboardData来获取剪贴板数据。
函数GetClipboardData声明如下：
WINUSERAPI
HANDLE
WINAPI
GetClipboardData(
__in UINT uFormat);
**uFormat**是剪贴板的格式。
调用函数的例子如下：
\#001 //获取剪贴板的数据。
\#002  //
\#003  void TestGetClipBoard(void)
\#004  {
\#005         //判断剪贴板的数据格式是否可以处理。
\#006         if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
\#007         {
\#008               return;
\#009         }
\#010
\#011         //打开剪贴板。
\#012         if (!OpenClipboard(m_hWnd))
\#013         {
\#014               return;
\#015         }
\#016
\#017         //获取UNICODE的数据。
**\#018        HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);**
\#019         if (hMem != NULL)
\#020         {
\#021               //获取UNICODE的字符串。
\#022               LPTSTR lpStr = (LPTSTR)GlobalLock(hMem);
\#023               if (lpStr != NULL)
\#024               {
\#025                    //显示输出。
\#026                    OutputDebugString(lpStr);
\#027
\#028                    //释放锁内存。
\#029                    GlobalUnlock(hMem);
\#030               }
\#031         }
\#032
\#033         //关闭剪贴板。
\#034         CloseClipboard();
\#035  }


