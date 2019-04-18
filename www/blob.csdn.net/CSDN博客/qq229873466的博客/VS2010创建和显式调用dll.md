# VS2010创建和显式调用dll - qq229873466的博客 - CSDN博客

2015年11月29日 22:13:44[qq229873466](https://me.csdn.net/qq229873466)阅读数：1506


**1.创建dll**

文件->新建->项目->Win32(Visual C++)->Win32控制台应用程序->输入项目名字，选择保存路径，弹出框选下一步->点击DLL，选择空项目->完成

在源文件中添加*.cpp文件

cpp文件输入：

```cpp
extern "C" int __declspec(dllexport) output(int input){
    return input;
}
```

->生成->生成解决方案

然后在Debug文件夹中会产生个*.dll文件。

**2.显式调用dll**

文件->新建->项目->Win32(Visual C++)->Win32控制台应用程序->输入项目名字，选择保存路径，填出框选下一步->点击控制台应用程序（默认），选择空项目->完成

在源文件中添加*.cpp文件

```cpp
#include <iostream>
#include <Windows.h>
 
void main(){
 typedef int (*myoutput)(int);
 myoutput output = NULL;
 
 HMODULE g_hDll = LoadLibrary(TEXT("G:\\test\\getDll\\Debug\\getDll.dll"));
 if(g_hDll!=NULL){
  output = (myoutput)GetProcAddress(g_hDll,"output"); // "output"为函数名
  printf("%d\n",output(123)); 
  FreeLibrary(g_hDll);
 }
}
```

->调试->开始执行（不调试）

