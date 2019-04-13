
# Windows下如何用C语言清空特定文件夹中的所有文件 - 高科的专栏 - CSDN博客

2012年08月22日 20:46:35[高科](https://me.csdn.net/pbymw8iwm)阅读数：1802


\#include "iostream.h"    //由于该博客系统发布是不能显示正常，代码如需调试，只需将改成""即可
\#include "string.h"
\#include "stdlib.h"
\#include "time.h"
\#include "math.h"
\#include "windows.h"
\#include "stdio.h"
\#include "shellapi.h"
\#include "fstream.h"
\#include "string"
using namespace std;
void main()
{
//清空特定文件夹中的所有文件
char* a=".";
char* b="";
WIN32_FIND_DATA FileData,FileData_0;
HANDLE hSearch,hSearch_0;
BOOL fFinished=FALSE;
hSearch=FindFirstFile("C:\\experiment\\results_stat\\*.*",&FileData);     //输入特定文件的绝对路径名
if(hSearch==INVALID_HANDLE_VALUE)
{
printf("No   files   found.");
return;
}
while(!fFinished)
{
if(FileData.cFileName[0]!=a[0]){
b=FileData.cFileName;
string addr=string("C:\\experiment\\results_stat\\")+string(b)+string("\\")+string[("*.*");    //输入特定文件的绝对路径名](http://www.justwinit.cn/category/32/post/5325/)
[
      hSearch_0=FindFirstFile(addr.c_str(),&FileData_0);](http://www.justwinit.cn/category/32/post/5325/)
[
      while(FindNextFile(hSearch_0,   &FileData_0)){](http://www.justwinit.cn/category/32/post/5325/)
[
        if(FileData_0.cFileName[0]!=a[0]){](http://www.justwinit.cn/category/32/post/5325/)
[
          string addr_0=string("C:\\experiment\\results_stat\\")+string(b)+string("](http://www.justwinit.cn/category/32/post/5325/)\\")+FileData_0.cFileName[;    //输入特定文件的绝对路径名](http://www.justwinit.cn/category/32/post/5325/)
[
          DeleteFile(addr_0.c_str());      //清空特定文件夹中的子文件夹中的所有文件](http://www.justwinit.cn/category/32/post/5325/)
[
        }](http://www.justwinit.cn/category/32/post/5325/)
[
      }](http://www.justwinit.cn/category/32/post/5325/)
[
    }](http://www.justwinit.cn/category/32/post/5325/)
[
    if(!FindNextFile(hSearch,&FileData))](http://www.justwinit.cn/category/32/post/5325/)
[
    {](http://www.justwinit.cn/category/32/post/5325/)
[
      if(GetLastError()==ERROR_NO_MORE_FILES)](http://www.justwinit.cn/category/32/post/5325/)
[
      {](http://www.justwinit.cn/category/32/post/5325/)
[
        fFinished=TRUE;](http://www.justwinit.cn/category/32/post/5325/)
[
      }](http://www.justwinit.cn/category/32/post/5325/)
[
      else](http://www.justwinit.cn/category/32/post/5325/)
[
      {](http://www.justwinit.cn/category/32/post/5325/)
[
        printf("Couldn't   find   next   file.");](http://www.justwinit.cn/category/32/post/5325/)
[
        return;](http://www.justwinit.cn/category/32/post/5325/)
[
      }](http://www.justwinit.cn/category/32/post/5325/)
[
    }](http://www.justwinit.cn/category/32/post/5325/)
[
  }](http://www.justwinit.cn/category/32/post/5325/)
[
  FindClose(hSearch);](http://www.justwinit.cn/category/32/post/5325/)
[
  FindClose(hSearch_0);](http://www.justwinit.cn/category/32/post/5325/)
[
}](http://www.justwinit.cn/category/32/post/5325/)


