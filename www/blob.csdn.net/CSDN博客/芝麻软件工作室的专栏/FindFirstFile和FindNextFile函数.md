
# FindFirstFile和FindNextFile函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:34:52[seven-soft](https://me.csdn.net/softn)阅读数：286标签：[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=class&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



在开发软件的过程里，经常需要维护目录里的数据。比如在开发银行的信用卡系统里，由于每天创建的LOG非常多，那么一个很大的硬盘，在6个月后，就占用了很多空间。这时就有这样的一个需求，把所有超过6个月的LOG数据定期删除掉。要实现这个功能就得遍历整个目录，把文件名称和文件创建的时间都超过6个月时间的文件删除掉。因此，就需使用到下面的API函数FindFirstFile和FindNextFile来实现这样的功能，FindFirstFile函数是查找到目录下的第一个文件或目录，FindNextFile函数是查找下一文件或目录。

函数FindFirstFile、FindNextFile和FindClose声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
FindFirstFileA(
__in LPCSTR lpFileName,
__out LPWIN32_FIND_DATAA lpFindFileData
);
WINBASEAPI
__out
HANDLE
WINAPI
FindFirstFileW(
__in LPCWSTR lpFileName,
__out LPWIN32_FIND_DATAW lpFindFileData
);
\#ifdef UNICODE
\#define FindFirstFile FindFirstFileW
\#else
\#define FindFirstFile FindFirstFileA
\#endif // !UNICODE
WINBASEAPI
BOOL
WINAPI
FindNextFileA(
__in HANDLE hFindFile,
__out LPWIN32_FIND_DATAA lpFindFileData
);
WINBASEAPI
BOOL
WINAPI
FindNextFileW(
__in HANDLE hFindFile,
__out LPWIN32_FIND_DATAW lpFindFileData
);
\#ifdef UNICODE
\#define FindNextFile FindNextFileW
\#else
\#define FindNextFile FindNextFileA
\#endif // !UNICODE
WINBASEAPI
BOOL
WINAPI
FindClose(
__inout HANDLE hFindFile
);
**lpFileName**是目录名称。一般使用通配符。
**lpFindFileData**是找到的文件或目录属性。
**hFindFile**是下一个文件或目录的句柄。
调用函数的例子如下：
\#001 \#pragma once
\#002
\#003 //
\#004 //遍历一个目录的文件。
\#005 //
\#006 //
\#007 class CFindFile
\#008 {
\#009 public:
\#010
\#011  CFindFile(void)
\#012  {
\#013          m_hFind = INVALID_HANDLE_VALUE;
\#014          m_bFound = false;
\#015         memset(&m_FindFileData,0,sizeof(m_FindFileData));
\#016  }
\#017
\#018  ~CFindFile(void)
\#019  {
\#020         if (m_hFind != INVALID_HANDLE_VALUE)
\#021         {
**\#022              ::FindClose(m_hFind);**
\#023         }
\#024  }
\#025
\#026  //找到第一个文件。
\#027  void First(LPCTSTR lpFileName)
\#028  {
**\#029         m_hFind = ::FindFirstFile(lpFileName,&m_FindFileData);**
\#030         if (m_hFind != INVALID_HANDLE_VALUE)
\#031         {
\#032                m_bFound = true;
\#033         }
\#034         else
\#035         {
\#036                m_bFound = false;
\#037         }
\#038  }
\#039
\#040  //查找一下文件。
\#041  void Next(void)
\#042  {
**\#043         m_bFound = FindNextFile(m_hFind, &m_FindFileData) ? true:false;**
\#044  }
\#045
\#046  //是否可以查找一下文件。
\#047  bool IsOK(void) const
\#048  {
\#049         return m_bFound;
\#050  }
\#051
\#052  //返回当前文件的属性。
\#053  const WIN32_FIND_DATA& GetCurFile(void)
\#054  {
\#055         return m_FindFileData;
\#056  }
\#057
\#058 protected:
\#059  HANDLE m_hFind; //保存当查找的位置句柄。
\#060  bool m_bFound; //当前查找是否成功。
\#061  WIN32_FIND_DATA m_FindFileData;//保存当前文件的属性。
\#062
\#063 };
\#064
使用如下：
\#001 //查找文件。
\#002               CFindFile findDemo;
\#003               for (findDemo.First(_T(".//*"));
\#004                    findDemo.IsOK();
\#005                    findDemo.Next())
\#006               {
\#007                    //
\#008                    OutputDebugString(findDemo.GetCurFile().cFileName);
\#009                    OutputDebugString(_T("/r/n"));
\#010               }
\#011


