# C++调用C#的DLL详解 - fanyun的博客 - CSDN博客
2016年11月03日 16:47:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：972
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**C++调用C#.Net的类库DLL步骤如下所示：**
一、C#写的类库：
using System;
using System.Collections.Generic;
using System.Text;
namespace ClassLibrary1
{
    public class Class1
    {
        public String Name
        {
            get;
            set;
        }
        public void helloWorld()
        {
            Console.WriteLine("hello world!");
        }
    }
}
// C#程序配置，一定是类库
![](https://img-blog.csdn.net/20161103164355223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
二、C++ 程序
共有三个程序文件
（1) 主程序
// test1.cpp : 定义控制台应用程序的入口点。
//  在C++
项目属性 [配置]-[常规]中，公共语言运行时支持，一定要选择“公共语言运行时支持”
#include "stdafx.h"
#include "yotopcompany.h"
#using "..\ClassLibrary1\bin\Debug\ClassLibrary1.dll"           //引用C#类库
using namespace ClassLibrary1;                //声明命名空间，非必须
int _tmain(int argc, _TCHAR* argv[])
{
    printf("hello world");
    ClassLibrary1::Class1 ^c = gcnew ClassLibrary1::Class1();   //注意一定要用 ^  ,一定要用gcnew
    c->Name = "\nxignxianghong";
    printf("%s\n", c->Name);
    c->helloWorld();
    YotopCompany ^a  = gcnew YotopCompany("a","b","c");
    printf("%s,%s,%s",a->name,a->address,a->phoneNumber);
    getchar();
    return 0;
}
（2） c++中自己编写的一个类
// yotopCompany.h
#pragma once
ref class YotopCompany
{
public:
    YotopCompany(void);
    YotopCompany(char* name,char* address,char* phoneNumber);
    char* name ;
    char* address;
    char*  phoneNumber;
};
(3) c++编写的类的CPP文件
//yotopcompany.cpp
#include "StdAfx.h"
#include "YotopCompany.h"
YotopCompany::YotopCompany(void)
{
    name = "yotop";
    phoneNumber = "12345678" ;
    address = "北京";
}
YotopCompany::YotopCompany(char*_name ,char* _address,char* _phoneNumber)
{
    name = _name;
    address = _address;
    phoneNumber = _phoneNumber ;
}
(4) c++ 程序配置
![](https://img-blog.csdn.net/20161103164540413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
三、总结：
1）用C#写任何的类库
2）C++中要引用此类库
3）创建C#对象时要用gcnew ;
4）C++ 编译设置一定设置为：支持公共语言运行时支持(/clr)
5）自身的C++类要用 ref class定义。
可参考微软官网：
[https://support.microsoft.com/zh-cn/kb/828736](https://support.microsoft.com/zh-cn/kb/828736)
