
# RegOpenKeyEx和RegCreateKeyEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:28:58[seven-soft](https://me.csdn.net/softn)阅读数：186标签：[attributes																](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[语言																](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)
由于电脑经常会关闭，或者应用程序也会经常关闭，但有一些参数是经常需要保存。比如当你打开程序，并设置了窗口的大小，想每次打开时都设置窗口为上次打开的大小。这样就需要保存窗口的大小，那么窗口大小的参数保存到那里呢？其实在Windows里最方便的做法，就是保存到注册表里。又比如游戏登录时，总是想保存最后一个登录的用户，那么也需要保存这个用户到注册表里。其实注册表是Windows保存系统配置的数据库，比如不同的语言设置，不同的时区设置，不同的用户登录，不同的权限等等。下面就来学习怎么样使用函数RegOpenKeyEx来打开注册表里的键和用函数RegCreateKeyEx来创建新的键。
函数RegOpenKeyEx和RegCreateKeyEx声明如下：
WINADVAPI
LONG
APIENTRY
RegOpenKeyExA (
__in HKEY hKey,
__in_opt LPCSTR lpSubKey,
__reserved DWORD ulOptions,
__in REGSAM samDesired,
__out PHKEY phkResult
);
WINADVAPI
LONG
APIENTRY
RegOpenKeyExW (
__in HKEY hKey,
__in_opt LPCWSTR lpSubKey,
__reserved DWORD ulOptions,
__in REGSAM samDesired,
__out PHKEY phkResult
);
\#ifdef UNICODE
\#define RegOpenKeyEx RegOpenKeyExW
\#else
\#define RegOpenKeyEx RegOpenKeyExA
\#endif // !UNICODE

WINADVAPI
LONG
APIENTRY
RegCreateKeyExA (
__in HKEY hKey,
__in LPCSTR lpSubKey,
__reserved DWORD Reserved,
__in_opt LPSTR lpClass,
__in DWORD dwOptions,
__in REGSAM samDesired,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
__out PHKEY phkResult,
__out_opt LPDWORD lpdwDisposition
);
WINADVAPI
LONG
APIENTRY
RegCreateKeyExW (
__in HKEY hKey,
__in LPCWSTR lpSubKey,
__reserved DWORD Reserved,
__in_opt LPWSTR lpClass,
__in DWORD dwOptions,
__in REGSAM samDesired,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
__out PHKEY phkResult,
__out_opt LPDWORD lpdwDisposition
);
\#ifdef UNICODE
\#define RegCreateKeyEx RegCreateKeyExW
\#else
\#define RegCreateKeyEx RegCreateKeyExA
\#endif // !UNICODE
**hKey**是主键。
**lpSubKey**是子键。
**ulOptions**是选项。
**samDesired**是键的操作。
**phkResult**是打开的键返回。
**lpClass**是新键值。
调用函数的例子如下：
\#001 //打注册表。HKEY_CURRENT_USER/"Software"/"Wincpp"/"testreg"
\#002  //
\#003  HKEY GetAppRegistryKey(void)
\#004  {
\#005         HKEY hAppKey = NULL;
\#006         HKEY hSoftKey = NULL;
\#007         HKEY hCompanyKey = NULL;
\#008
\#009         //打开HKEY_CURRENT_USER/"Software"。
**\#010        if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ,**
**\#011              &hSoftKey) == ERROR_SUCCESS)**
\#012         {
\#013               DWORD dw;
\#014               //创建并打开HKEY_CURRENT_USER/"Software"/"Wincpp"
**\#015              if (RegCreateKeyEx(hSoftKey, _T("Wincpp"), 0, REG_NONE,**
**\#016                   REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,**
**\#017                   &hCompanyKey, &dw) == ERROR_SUCCESS)**
\#018               {
\#019                    //创建并打开HKEY_CURRENT_USER/"Software"/"Wincpp"/"testreg"
\#020                    RegCreateKeyEx(hCompanyKey, _T("testreg"), 0, REG_NONE,
\#021                          REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
\#022                          &hAppKey, &dw);
\#023               }
\#024         }
\#025
\#026         //关闭打开的键值。
\#027         if (hSoftKey != NULL)
\#028         {
\#029               RegCloseKey(hSoftKey);
\#030         }
\#031
\#032         if (hCompanyKey != NULL)
\#033         {
\#034               RegCloseKey(hCompanyKey);
\#035         }
\#036
\#037         return hAppKey;
\#038  }


