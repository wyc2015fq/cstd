
# CreateDirectory和RemoveDirectory函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:42:34[seven-soft](https://me.csdn.net/softn)阅读数：530


在信息爆炸的年代里，把各种信息分类已经是一种非常必要的功能，比如把股票行情数据保存到硬盘里，就需要分开几个目录保存。比如你写LOG到硬盘时，也需要分成几个目录来保存，这样让维护人员很好地找到出错的信息，或者有用的信息。像我在开发银行的信用卡系统时，就需要把所有通过网络传送的数据全部生成文本文件保存到当日的目录里，也就是说每天都需要创建一个目录，把所有交易的数据生成LOG保存进去。那么面对这样的需求，你是怎么样去创建目录的呢？这就需要使用到Windows
 API函数CreateDirectory创建目录，当然目录过多时也需要调用函数RemoveDirectory来删除不需要的目录，然而函数RemoveDirectory只能删除空的目录，也就是目录下没有文件和子目录才能删除。
函数CreateDirectory和RemoveDirectory声明如下：
WINBASEAPI
BOOL
WINAPI
CreateDirectoryA(
__in     LPCSTR lpPathName,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
WINBASEAPI
BOOL
WINAPI
CreateDirectoryW(
__in     LPCWSTR lpPathName,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
\#ifdef UNICODE
\#define CreateDirectory CreateDirectoryW
\#else
\#define CreateDirectory CreateDirectoryA
\#endif // !UNICODE
WINBASEAPI
BOOL
WINAPI
RemoveDirectoryA(
__in LPCSTR lpPathName
);
WINBASEAPI
BOOL
WINAPI
RemoveDirectoryW(
__in LPCWSTR lpPathName
);
\#ifdef UNICODE
\#define RemoveDirectory RemoveDirectoryW
\#else
\#define RemoveDirectory RemoveDirectoryA
\#endif // !UNICODE
**lpPathName**是目录的路径。
**lpSecurityAttributes**是目录的安全属性。
**返回值**是执行是否成功。
调用函数的例子如下：
\#001 //创建目录。
\#002               //
\#003               std::wstring strDir(_T("c://log"));
\#004               if (!CreateDirectory(strDir.c_str(),NULL))
\#005               {
\#006                    OutputDebugString(_T("创建目录不成功/r/n"));
\#007
\#008               }
\#009               else
\#010               {
\#011                    //删除空目录。
\#012                    RemoveDirectory(strDir.c_str());
\#013               }
\#014


