# 【VS2013】windows多线程编程_初次实验 - CD's Coding - CSDN博客





2014年11月19日 20:02:01[糖果天王](https://me.csdn.net/okcd00)阅读数：4642








步骤： 

1、 用Microsoft Visual Studio 2005创建控制台项目HelloThreads 。 

2、 创建HelloThreads.cpp文件

3、点击项目—>HelloThreads属性菜单，按图1-1~图1-4配置项目属性。

4、编译执行，输出结果：

5、修改代码，使之输出结果可以表示出各线程的输出顺序。






```cpp
// HelloThreads.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
const int numThreads = 4;

DWORD WINAPI helloFunc(LPVOID pArg)
{
	int j = *(int*)pArg;
	printf("Hello Thread %d\n",j);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread[numThreads];
	int tNum[4];
	for (int i = 0; i < numThreads; i++)
	{
		tNum[i] = i;
		hThread[i] = CreateThread(NULL, 0, helloFunc, &tNum[i], 0, NULL);
	}
	WaitForMultipleObjects(numThreads, hThread, TRUE, INFINITE);
	system("pause");
	return 0;
}
```






结果截图

![](https://img-blog.csdn.net/20141119200145456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







