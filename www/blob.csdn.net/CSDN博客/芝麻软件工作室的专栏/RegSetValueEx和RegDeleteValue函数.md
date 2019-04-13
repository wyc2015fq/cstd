
# RegSetValueEx和RegDeleteValue函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:27:47[seven-soft](https://me.csdn.net/softn)阅读数：331


上一次说到怎么创建注册表的键，但接着下来的问题就是怎么样保存数据到注册表里。注册表使用树形的方式管理数据，所以它的扩展和访问都是比较灵活的。不过注册表是系统重要信息库，每当Windows系统加载时，都首先从硬盘里读取它出来，才知道每台电脑所有硬件配置信息，然后再加载不同的驱动程序。因此，注册表作为系统重要的文件，不要往里面写超过2K的数据大小，这样可以提高系统的速度。下面就来介绍一下怎么样保存一个字符串的键值。它需要使用RegSetValueEx函数来设置键值和使用RegDeleteValue函数来删除原来的键值。
函数RegSetValueEx和RegDeleteValue声明如下：
WINADVAPI
LONG
APIENTRY
RegSetValueExA (
__in HKEY hKey,
__in_opt LPCSTR lpValueName,
__reserved DWORD Reserved,
__in DWORD dwType,
__in_bcount_opt(cbData) CONST BYTE* lpData,
__in DWORD cbData
);
WINADVAPI
LONG
APIENTRY
RegSetValueExW (
__in HKEY hKey,
__in_opt LPCWSTR lpValueName,
__reserved DWORD Reserved,
__in DWORD dwType,
__in_bcount_opt(cbData) CONST BYTE* lpData,
__in DWORD cbData
);
\#ifdef UNICODE
\#define RegSetValueEx RegSetValueExW
\#else
\#define RegSetValueEx RegSetValueExA
\#endif // !UNICODE
WINADVAPI
LONG
APIENTRY
RegDeleteValueA (
__in HKEY hKey,
__in_opt LPCSTR lpValueName
);
WINADVAPI
LONG
APIENTRY
RegDeleteValueW (
__in HKEY hKey,
__in_opt LPCWSTR lpValueName
);
\#ifdef UNICODE
\#define RegDeleteValue RegDeleteValueW
\#else
\#define RegDeleteValue RegDeleteValueA
\#endif // !UNICODE
**hKey**是主键。
**lpValueName**是键名称。
**dwType**是键值类型。
**lpData**是键的数据。
**cbData**是键值的大小。
调用函数的例子如下：
\#001  //打注册表。HKEY_CURRENT_USER/"Software"/"Wincpp"/"testreg"
\#002  // /"Windows"//"winsize" = "800*600"
\#003  //
\#004  BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
\#005         LPCTSTR lpszValue)
\#006  {
\#007         //
\#008         LONG lResult;
\#009         if (lpszEntry == NULL) //删除整键。
\#010         {
\#011               HKEY hAppKey = GetAppRegistryKey();
\#012               if (hAppKey == NULL)
\#013               {
\#014                    return FALSE;
\#015               }
\#016
\#017               lResult = ::RegDeleteKey(hAppKey, lpszSection);
\#018               RegCloseKey(hAppKey);
\#019         }
\#020         else if (lpszValue == NULL)
\#021         {
\#022               //删除键值。
\#023               HKEY hAppKey = GetAppRegistryKey();
\#024               if (hAppKey == NULL)
\#025               {
\#026                    return FALSE;
\#027               }
\#028
\#029               HKEY hSecKey = NULL;
\#030               DWORD dw;
\#031               RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
\#032                    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
\#033                    &hSecKey, &dw);
\#034               RegCloseKey(hAppKey);
\#035
\#036               if (hSecKey == NULL)
\#037               {
\#038                    return FALSE;
\#039               }
\#040
\#041               //
**\#042              lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);**
\#043               RegCloseKey(hSecKey);
\#044         }
\#045         else
\#046         {
\#047               //设置键值。
\#048               HKEY hAppKey = GetAppRegistryKey();
\#049               if (hAppKey == NULL)
\#050               {
\#051                    return FALSE;
\#052               }
\#053
\#054               HKEY hSecKey = NULL;
\#055               DWORD dw;
\#056               //创建子键。
\#057               RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
\#058                    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
\#059                    &hSecKey, &dw);
\#060               RegCloseKey(hAppKey);
\#061
\#062               if (hSecKey == NULL)
\#063               {
\#064                    return FALSE;
\#065               }
\#066
\#067               //设置子键中的项值。
**\#068              lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,**
**\#069                   (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));**
\#070               RegCloseKey(hSecKey);
\#071         }
\#072         return lResult == ERROR_SUCCESS;
\#073
\#074  }


