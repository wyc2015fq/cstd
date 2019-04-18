# C++ 读取注册表 - 文章 - 伯乐在线
原文出处： [luoweifu](http://blog.csdn.net/luoweifu/article/details/47175959)
昨天受一朋友要求，帮忙写一个用C++读取注册表的程序。这个朋友是搞Java的，肯定不知道C++怎么写啦！他的需求也奇怪：用js调用一个浏览器插件，通过插件获取注册表中指定键的值，插件肯定是用C++写了。
于是乎我就在网上查了一下“C++读取注册表”，一搜一大片但都是一些初学C++的人写的，多数不能用，而且那写的程序真是无力吐槽……一怒之下我就自己花一个小时写了一个，费话不说了，直接上代码：
C++读取注册表
GetRegValue.h:


```
#ifndef __GETREGVALUE_H__
#define __GETREGVALUE_H__
#include <string>
//---------------------------------------------------------------
//function: 
//          GetRegValue 获取注册表中指定键的值
//Access:
//           public  
//Parameter:
//          [in] int nKeyType - 注册表项的类型，传入的参数只可能是以下数值：
//                              0:HKEY_CLASSES_ROOT
//                              1:HKEY_CURRENT_USER
//                              2:HKEY_LOCAL_MACHINE
//                              3:HKEY_USERS
//                              4:HKEY_PERFORMANCE_DATA
//                              5:HKEY_CURRENT_CONFIG
//                              6:HKEY_DYN_DATA
//                              7:HKEY_CURRENT_USER_LOCAL_SETTINGS
//                              8:HKEY_PERFORMANCE_TEXT
//                              9:HKEY_PERFORMANCE_NLSTEXT
//          [in] const std::string & strUrl - 要查找 的键的路径
//          [in] const std::string & strKey - 指定的键
//Returns:
//          std::string - 指定键的值
//Remarks:
//          ...
//author:   luoweifu
//---------------------------------------------------------------
std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey);
//可移植版本 wstring => string
std::string ws2s(const std::wstring& ws);
//可移植版本 string => wstring
std::wstring s2ws(const std::string& s);
#endif  //__GETREGVALUE_H__
```
GetRegValue.cpp：


```
#include "stdafx.h"
#include <Windows.h>
#include "GetRegValue.h"
//可移植版本 wstring => string
std::string ws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, "");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = wcstombs(NULL, _Source, 0) + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
//可移植版本 string => wstring
std::wstring s2ws(const std::string& s)
{
    std::string curLocale = setlocale(LC_ALL, ""); 
    const char* _Source = s.c_str();
    size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey)
{
    std::string strValue("");
    HKEY hKey = NULL;
    HKEY  hKeyResult = NULL;
    DWORD dwSize     = 0;
    DWORD dwDataType = 0;
    std::wstring wstrUrl = s2ws(strUrl);
    std::wstring wstrKey = s2ws(strKey);
    switch(nKeyType)
    {
    case 0:
        {
            hKey = HKEY_CLASSES_ROOT;
            break;
        }
    case 1:
        {
            hKey = HKEY_CURRENT_USER;
            break;
        }
    case 2:
        {
            hKey = HKEY_LOCAL_MACHINE;
            break;
        }
    case 3:
        {
            hKey = HKEY_USERS;
            break;
        }
    case 4:
        {
            hKey = HKEY_PERFORMANCE_DATA;
            break;
        }
    case 5:
        {
            hKey = HKEY_CURRENT_CONFIG;
            break;
        }
    case 6:
        {
            hKey = HKEY_DYN_DATA;
            break;
        }
    case 7:
        {
            hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
            break;
        }
    case 8:
        {
            hKey = HKEY_PERFORMANCE_TEXT;
            break;
        }
    case 9:
        {
            hKey = HKEY_PERFORMANCE_NLSTEXT;
            break;
        }
    default:
        {
            return strValue;
        }
    }
    //打开注册表
    if(ERROR_SUCCESS == ::RegOpenKeyEx(hKey, wstrUrl.c_str(), 0, KEY_QUERY_VALUE, &hKeyResult))
    {
        // 获取缓存的长度dwSize及类型dwDataType
        ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, NULL, &dwSize); 
        switch (dwDataType)
        {
        case REG_MULTI_SZ:
            {
                //分配内存大小
                BYTE* lpValue = new BYTE[dwSize];
                //获取注册表中指定的键所对应的值
                LONG lRet = ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, lpValue, &dwSize);
                delete[] lpValue;
                break;
            }
        case REG_SZ:
            {
                //分配内存大小
                wchar_t* lpValue = new wchar_t[dwSize];
                memset(lpValue, 0, dwSize * sizeof(wchar_t));
                //获取注册表中指定的键所对应的值
                if (ERROR_SUCCESS == ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, (LPBYTE)lpValue, &dwSize))
                {
                    std::wstring wstrValue(lpValue);
                    strValue = ws2s(wstrValue);
                }
                delete[] lpValue;
                break;
            }
        default:
            break;
        }
    }
    //关闭注册表
    ::RegCloseKey(hKeyResult);
    return strValue;
}
```
测试代码：


```
#include "stdafx.h"
#include <string>
#include "GetRegValue.h"
int _tmain(int argc, _TCHAR* argv[])
{
    std::string strValue = GetRegValue(2, "SOFTWARE\\360Safe\\Liveup", "mid");
    return 0;
}
```
结果：
strValue：
“ebd1360403764c9d48c585ef93a6eacbd89ded596f043f78e54eb0adeba7251d”
![这里写图片描述](http://img.blog.csdn.net/20150731213752857)
