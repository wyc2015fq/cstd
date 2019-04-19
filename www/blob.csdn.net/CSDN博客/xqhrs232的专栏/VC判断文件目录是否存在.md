# VC判断文件目录是否存在 - xqhrs232的专栏 - CSDN博客
2013年06月19日 16:49:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：730
原文地址::[http://blog.csdn.net/pgshow/article/details/7684822](http://blog.csdn.net/pgshow/article/details/7684822)
 相关网帖
1、[VC中判断目录,文件是否存在,创建目录的方法](http://www.cnblogs.com/kiddo/archive/2007/09/03/879950.html) ----[http://www.cnblogs.com/kiddo/archive/2007/09/03/879950.html](http://www.cnblogs.com/kiddo/archive/2007/09/03/879950.html)
1. 使用_access函数，函数原型为 int _access( const char *path, int mode );
2. 使用CreateFile函数，函数原型为：
HANDLE CreateFile( LPCTSTR lpFileName, // pointer to name of the file DWORD dwDesiredAccess, // access (read-write) mode DWORD dwShareMode, // share mode LPSECURITY_ATTRIBUTES lpSecurityAttributes, // pointer to security attributes DWORD dwCreationDisposition,
 // how to create DWORD dwFlagsAndAttributes, // file attributes HANDLE hTemplateFile // handle to file with attributes to // copy );
3. 使用FindFirstFile函数，函数原型为： HANDLE FindFirstFile( LPCTSTR lpFileName, // pointer to name of file to search for LPWIN32_FIND_DATA lpFindFileData // pointer to returned information );
//例子：
BOOL CPubFunc::DirectoryExist(CString Path)
{
 WIN32_FIND_DATA fd;
 BOOL ret = FALSE;
    HANDLE hFind = FindFirstFile(Path, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {  //目录存在
            ret = TRUE;    
    }
FindClose(hFind);
 return ret;
}
 4. 使用GetFileAttributes函数，函数原型如下： DWORD GetFileAttributes( LPCTSTR lpFileName // pointer to the name of a file or directory );
5. 使用Shell Lightweight Utility APIs函数
PathFileExists（）专门判断文件和目录时否存在的函数文件名可读性比较强还可以判断目录是否存在 Header: Declared in Shlwapi.h Import Library: Shlwapi.lib 以上的各种方法供参考，函数具体用法需参见MSDN。
//这是MSDN中的例子：
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"
void main( void )
{
// Valid file path name (file is there).
char buffer_1[] = "C:\\TEST\\file.txt"; 
char *lpStr1;
lpStr1 = buffer_1;
// Invalid file path name (file is not there).
char buffer_2[] = "C:\\TEST\\file.doc"; 
char *lpStr2;
lpStr2 = buffer_2;
// Return value from "PathFileExists". 
int retval; 
// Search for the presence of a file with a true result.
retval = PathFileExists(lpStr1);
if(retval == 1)
{
cout << "Search for the file path of : " << lpStr1 << endl;
cout << "The file requested \"" << lpStr1 << "\" is a valid file" << endl;
cout << "The return from function is : " << retval << endl;
}
else{
cout << "\nThe file requested " << lpStr1 << " is not a valid file" << endl;
cout << "The return from function is : " << retval << endl;
}
// Search for the presence of a file with a false result.
retval = PathFileExists(lpStr2);
if(retval == 1)
{
cout << "\nThe file requested " << lpStr2 << "is a valid file" << endl;
cout << "Search for the file path of : " << lpStr2 << endl;
cout << "The return from function is : " << retval << endl;
}
else{
cout << "\nThe file requested \"" << lpStr2 << "\" is not a valid file" << endl;
cout << "The return from function is : " << retval << endl;
}
} 
6.使用CFileFind 类....这是一个InternetServices类，在此可以借用一下。也可以用于遍历文件夹（s可指定深度）
BOOL CPubFunc::FileExist(CString FileName)
{
 CFileFind fFind;
 return fFind.FindFile(FileName); 
}
//创建目录
 #include <winbase.h>
**BOOL CreateDirectory(  LPCTSTR*** lpPathName***,                         **
// pointer to directory path string
**  LPSECURITY_ATTRIBUTES*** lpSecurityAttributes*  // pointer to security descriptor
**);**
