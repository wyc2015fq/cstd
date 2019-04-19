# C++创建文件夹和为文件夹改名 - sinat_31425585的博客 - CSDN博客
2016年09月08日 20:42:28[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：1540
> 
记忆力差的孩子得勤做笔记！
最近在程序中需要创建文件夹和更改文件夹名字，这里主要用到_mkdir和 MoveFileW两个函数，好了废话不多说，直接上demo。
```cpp
#include <iostream>
#include <direct.h>
#include <io.h>
#include <Windows.h>
using namespace std;
void fixFileName(string old, string new1);
wstring StringToWString(const std::string &str);
void main()
{
	if(_access("test", 0) != -1)
	{
		cout << "文件夹已存在" <<endl;
	}
	else
	{
		cout << "文件夹不存在，创建文件夹" << endl;
		_mkdir("test");
	}
	 //修改文件夹名称
	string old = "test";
	string new1 = "newFile";
	fixFileName(old, new1);
	
	
}
void fixFileName(string oldFilename, string newFilename)
{
	
	wstring a = StringToWString(oldFilename);
	wstring b = StringToWString(newFilename);
	LPCWSTR str = a.c_str();
	LPCWSTR str1 = b.c_str();
	MoveFileW(str, str1);
}
wstring StringToWString(const std::string &str)
{
	std::wstring wstr(str.length(),L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr; 
}
```
> 
打完收工，这里写了个fixFileName函数接口，可以直接拿来用，蛮方便的！
