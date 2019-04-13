
# 动态链接库DLL导出函数并导入使用 - unixfy - 博客园
# [程序最美（寻路）](https://www.cnblogs.com/unixfy/)
你还在坚持练习你的技术吗？运动员天天训练，音乐家也会演练更难的曲章。你呢？
## [动态链接库DLL导出函数并导入使用](https://www.cnblogs.com/unixfy/p/3240162.html)
动态链接库DLL导出函数并导入使用
本文完全参考自《[vs2008制作dll笔记，回带值样例](http://leongod.iteye.com/blog/1104575)》。
首先制作DLL文件，在vs2010中新建Win32控制台项目，选择DLL选项，简历头文件，源文件如下：
头文件：
//MyDll.hextern"C"_declspec(dllexport)intMax(inta,intb);extern"C"_declspec(dllexport)intMin(inta,intb);extern"C"_declspec(dllexport)boolcim2scada(char* cimId,char** scadaId);
源文件：
//MyDll.cpp : Defines the exported functions for the DLL application.//\#include"MyDll.h"\#include<iostream>intMax(inta,intb)
{if(a >=b)
    {returna;
    }else{returnb;
    }
}intMin(inta,intb)
{if(a >=b)
    {returnb;
    }else{returna;
    }
}boolcim2scada(char* cimId,char**scadaId)
{char*result;if(cimId ==NULL)
    {
        printf("空指针！");returnfalse;
    }elseif(strlen(cimId) ==0)
    {
        printf("cimId无值！");returnfalse;
    }if(strcmp(cimId,"aaaaaa") ==0)
    {
        result="1000";
    }elseif(strcmp(cimId,"bbbbbb"))
    {
        result="2000";
    }*scadaId =result;
    printf("fun scadaId: %s \n", *scadaId);returntrue;
}
然后，创建测试样例，重新简历Win32控制台程序，选择控制台选项，建立测试代码：
\#include"windows.h"\#include<iostream>intmain()
{
    typedefint(*pMax)(inta,intb);
    typedefint(*pMin)(inta,intb);
    typedefbool(*pcim2scada)(char* cimid,char**scada);
    HINSTANCE HDLL;
    HDLL= LoadLibrary(L"MyDll.dll");//加载动态链接库MyDll.dll文件；pMax Max = (pMax)GetProcAddress(HDLL,"Max");
    pMin Min= (pMin)GetProcAddress(HDLL,"Min");
    pcim2scada cim2scada= (pcim2scada)GetProcAddress(HDLL,"cim2scada");inta = Max(5,12);char* cimId ="aaaaaa";char* scadaId ="";boolb = cim2scada(cimId, &scadaId);
    printf("Max结果为%d\n", a);
    printf("cimId:%s, scadaId:%s\n", cimId, scadaId);
    
    FreeLibrary(HDLL);//卸载MyDll.dll文件}
编译生成Debug文件夹，讲之前生成的Dll文件拷贝到该Debug文件夹中，然后运行该测试样例。
![](https://images0.cnblogs.com/blog/463570/201308/06111545-48cc7bc1682e4767998a65e3859640dd.jpg)
DLL导出并导入生成函数需要注意几点：
**extern "C" _declspec(dllexport)**
**typedef int (*pMax)(int a, int b);**
**typedef int (*pMin)(int a, int b);**
**typedef bool (*pcim2scada)(char* cimid, char** scada);**
**HINSTANCE HDLL;**
**HDLL = LoadLibrary(L"MyDll.dll"); //****加载动态链接库MyDll.dll****文件；**
**pMax Max = (pMax)GetProcAddress(HDLL, "Max");**
**pMin Min = (pMin)GetProcAddress(HDLL, "Min");**
**pcim2scada cim2scada = (pcim2scada)GetProcAddress(HDLL, "cim2scada");**
**接下来，将学习如何用DLL****导出C++****类、对象，并进行导入使用。**




posted on2013-08-06 11:16[unixfy](https://www.cnblogs.com/unixfy/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=3240162)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)







### 导航
[博客园](https://www.cnblogs.com/)
[首页](https://www.cnblogs.com/unixfy/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/unixfy)
[订阅](https://www.cnblogs.com/unixfy/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 - 85文章 - 0评论 - 8引用 - 0
公告

Powered by:
博客园
Copyright © unixfy
