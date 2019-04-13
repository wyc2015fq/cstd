
# RegQueryValueEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:26:50[seven-soft](https://me.csdn.net/softn)阅读数：228标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



上一次介绍怎么样保存数据到注册表里，这次就需要从注册表里读取数据出来了。在这个例子里是读取字符串数据出来，主要调用函数RegQueryValueEx来实现。下面的例子里就是先查询键值的长度，然后再读取内容出来。
函数RegQueryValueEx声明如下：
WINADVAPI
LONG
APIENTRY
RegQueryValueExA (
__in HKEY hKey,
__in_opt LPCSTR lpValueName,
__reserved LPDWORD lpReserved,
__out_opt LPDWORD lpType,
__out_bcount_opt(*lpcbData) LPBYTE lpData,
__inout_opt LPDWORD lpcbData
);
WINADVAPI
LONG
APIENTRY
RegQueryValueExW (
__in HKEY hKey,
__in_opt LPCWSTR lpValueName,
__reserved LPDWORD lpReserved,
__out_opt LPDWORD lpType,
__out_bcount_opt(*lpcbData) LPBYTE lpData,
__inout_opt LPDWORD lpcbData
);
\#ifdef UNICODE
\#define RegQueryValueEx RegQueryValueExW
\#else
\#define RegQueryValueEx RegQueryValueExA
\#endif // !UNICODE
**hKey**是主键。
**lpValueName**是键值名称。
**lpType**是类型。
**lpData**是读出来数据保存地方。
**lpcbData**是读取数据多少。
调用函数的例子如下：
\#001 //打注册表返回值。HKEY_CURRENT_USER/"Software"/"Wincpp"/"testreg"
\#002  // /"Windows"//"winsize" = "800*600"
\#003  //
\#004  std::wstring GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
\#005         LPCTSTR lpszDefault)
\#006  {
\#007         //打开应用程序键。
\#008         HKEY hAppKey = GetAppRegistryKey();
\#009         if (hAppKey == NULL)
\#010         {
\#011               return lpszDefault;
\#012         }
\#013
\#014         HKEY hSecKey = NULL;
\#015         DWORD dw;
\#016
\#017         //打开子键。
\#018         RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
\#019               REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
\#020               &hSecKey, &dw);
\#021         RegCloseKey(hAppKey);
\#022
\#023         if (hSecKey == NULL)
\#024         {
\#025               return lpszDefault;
\#026         }
\#027
\#028         //查询键值。
\#029         std::wstring strValue;
\#030         DWORD dwType=REG_NONE;
\#031         DWORD dwCount=0;
\#032
\#033         //先查询键值的长度。
\#034         LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
\#035               NULL, &dwCount);
\#036         if (lResult == ERROR_SUCCESS)
\#037         {
\#038               strValue.resize(dwCount);
\#039
\#040               //查询键值。
\#041               lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
\#042                    (LPBYTE)strValue.data(), &dwCount);
\#043
\#044         }
\#045
\#046         RegCloseKey(hSecKey);
\#047         if (lResult == ERROR_SUCCESS)
\#048         {
\#049               return strValue;
\#050         }
\#051
\#052         return lpszDefault;
\#053  }


