
# CopyFile和MoveFile函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:35:44[seven-soft](https://me.csdn.net/softn)阅读数：384个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



在信息的社会里，共享信息是非常重要的。比如你有一个很好的相片，要拷给朋友去分享。又或者你在写一些比较重要的数据，要进行不定时备份时，也就需要拷贝文件到不同的目录里。这样就需要使用到函数CopyFile来拷贝文件，它能够把一份文件拷贝多一份出来。我在开发一个数据采集的软件里，由于这个软件是可以不同的用户共同使用，用户跟我说有这样的需求，就是不同的用户的数据移动到不同的目录里，这样方便他们管理数据，也方便他们在上千个文件里找到自己有用的文件，而不会与别人的文件混在一起，这样就需要把采集数据完成后把文件移到相应的目录，这样就需要使用函数MoveFile来移动文件。
函数CopyFile和MoveFile声明如下：
WINBASEAPI
BOOL
WINAPI
CopyFileA(
__in LPCSTR lpExistingFileName,
__in LPCSTR lpNewFileName,
__in BOOL bFailIfExists
);
WINBASEAPI
BOOL
WINAPI
CopyFileW(
__in LPCWSTR lpExistingFileName,
__in LPCWSTR lpNewFileName,
__in BOOL bFailIfExists
);
\#ifdef UNICODE
\#define CopyFile CopyFileW
\#else
\#define CopyFile CopyFileA
\#endif // !UNICODE

WINBASEAPI
BOOL
WINAPI
MoveFileA(
__in LPCSTR lpExistingFileName,
__in LPCSTR lpNewFileName
);
WINBASEAPI
BOOL
WINAPI
MoveFileW(
__in LPCWSTR lpExistingFileName,
__in LPCWSTR lpNewFileName
);
\#ifdef UNICODE
\#define MoveFile MoveFileW
\#else
\#define MoveFile MoveFileA
\#endif // !UNICODE
调用函数的例子如下：
\#001  //拷贝和移动文件。
\#002  //
\#003  void CopyAndMoveFile(void)
\#004  {
\#005         //拷贝文件。
\#006         BOOL bRes = ::CopyFile(_T("CreateFileDemo.txt"),
\#007               _T("CreateFileDemo_New.txt"),FALSE);
\#008         if (bRes)
\#009         {
\#010               //
\#011               OutputDebugString(_T("拷贝文件成功!/r/n"));
\#012         }
\#013
\#014         //移动文件。
\#015         bRes = ::MoveFile(_T("CreateFileDemo.txt"),
\#016               _T(".//Debug//CreateFileDemo.txt"));
\#017         if (bRes)
\#018         {
\#019               //
\#020               OutputDebugString(_T("移动文件成功!/r/n"));
\#021         }
\#022  }
\#023


