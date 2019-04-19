# WINCE下获得系统语言的小C - xqhrs232的专栏 - CSDN博客
2013年08月27日 17:40:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：772
原文地址::[http://jimmyloveforever.blog.163.com/blog/static/11977524720097110649266/](http://jimmyloveforever.blog.163.com/blog/static/11977524720097110649266/)
#include <stdio.h>
#include <windows.h>
int main(int argc, char* argv[])
{
LCID language;
language=GetSystemDefaultLCID();
switch(language)
{
case 2052:
   printf("中文（中国）\n");
   break;
   case 3076:
   printf("中文（香港）\n");
   break;
   case 1028:
   printf("中文（台湾）\n");
   break;
   case 1033:
   printf("英语（英国）\n");
   case 1041:
   printf("操他妈的小日本\n");
   //若干format操作.....
   break;
default:
   printf("其他\n");
   break;
}
return 0;
}
//===================================================================================
备注::
1>用GetSystemDefaultLCID( )函数是乎不准确，应该使用GetSystemDefaultUILanguage()函数
2>The registry key that stores the default system language is located at
**HKEY_LOCAL_MACHINE\MUI\SysLang**.
