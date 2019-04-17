# C++中宏的使用技巧 - 长歌行 - CSDN博客





2013年03月28日 14:39:20[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2252








都是常用技巧，经常用C++的应该都会注意到一两条，为综合整理，非个人独创，从开源代码以及书本论文中抄过来的，数目链接[http://blog.csdn.net/iuhsihsow/article/details/7222093](http://blog.csdn.net/iuhsihsow/article/details/7222093)如有雷同，纯属巧合。




直接上代码,这次归纳了6点



```cpp
// Maro.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
using namespace  std;

// 宏的用法

// 1.因为宏是在编译时进行简单的替换，所以很多时候使用宏在突出
//   重点参数的同时，也会提高效率。
#define MAXHEIGHT  100.0
// 不过自定义单位一般使用typedef
typedef int SYSINT;

// 2.短小实用的语句
#define SAFE_DELETE(p)		{if(p) delete p; p = NULL;}
#define SAFE_RELEASE(p)		{while(p->Release()>0); p = NULL;}

// 3.类的导出的声明,因为编译时会报错，所以先全部注释掉
//#ifdef MODULE_EXPORTS
//#	define MODULE_API __declspec(dllexport)
//#else
//#	define MODULE_API __declspec(dllimport)
//#		ifdef	_DEBUG
//#			pragma comment(lib, "Moduled.lib")
//#		else
//#			pragma comment(lib, "Module.lib")
//#		endif
//#endif

// 4.一些编译时的条件语句
#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif


// 5.实用的小函数
// 例如计算某函数的运行时间
#define BEGIN_TIME	\
	UINT start = GetTickCount();	

#define END_TIME(total)	\
	total = GetTickCount() - start;\

//当然也可以
#define USE_TIME(used, fuc)	\
	UINT begin1 = GetTickCount();\
	fuc;\
	used = GetTickCount() - begin1;\

// 6.定义基类，派生类的接口，这样可以减少修改量，突出接口 ##表示连接
#define INTERFACE_Creature(terminal) \
	public:							 \
	virtual void SetName(const string& strName)	##terminal\
	virtual bool GetName(string &strName) ##terminal

#define BASE_Createture		INTERFACE_Creature(=0;)
#define Divd_Createture		INTERFACE_Creature(;)

class BaseClass
{
	BASE_Createture;
};

class DivdClass : public BaseClass
{
	Divd_Createture;
};

bool DivdClass::GetName(string& strName)
{
	return true;
}

void DivdClass::SetName(const string& strName)
{

}

void Dosth(int time = 500)
{
	Sleep(time);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//1
	double dLength = 50.0;
	if (dLength > MAXHEIGHT)
	{
		// do sth
	}

	//2
	double *p = new double[100];
	SAFE_DELETE(p);

	//3
	//略

	//4
#ifdef DOSTH
	//DO STH
#endif
	
	//5
	int useTime;
//我们可以这样
	BEGIN_TIME
		Dosth();
	END_TIME(useTime);
//还可以这样
	USE_TIME(useTime, Dosth())
	printf("Used time is %d\n", useTime);

	//6
	BaseClass *pD = new DivdClass;
	pD->SetName("test");
	SAFE_DELETE(pD);	// 学而时习之，不亦说乎
	return 0;
}
```








