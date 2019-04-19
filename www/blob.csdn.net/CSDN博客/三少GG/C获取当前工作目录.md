# C获取当前工作目录 - 三少GG - CSDN博客
2013年05月26日 18:21:23[三少GG](https://me.csdn.net/scut1135)阅读数：1244
函数原型:     char *getcwd(char *dir,int len)
函数功能:     得到当前路径名称
功 能：获取当前工作目录
参数说明：getcwd()会将当前工作目录的[绝对路径](http://baike.baidu.com/view/25377.htm)复制到参数buffer所指的内存空间中,参数maxlen为buffer的空间大小。
返 回 值：成功则返回当前工作目录，失败返回 **FALSE**。
在某些 Unix 的变种下，如果任何父目录没有设定可读或搜索模式，即使[当前目录](http://baike.baidu.com/view/396351.htm)设定了，**getcwd()**还是会返回 **FALSE**。有关模式与权限的更多信息见 chmod()。
头文件：direct.h（TC2.0下为dir.h）
#include <stdio.h>
#include <direct.h>
int main()
{
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    printf("The current directory is: %s", buffer);
    return 0;
}
获取当前目录对于.net来说很简单，在此不用多说了。C/C++来说挺也简单，但容易掉进MS的陷阱。在此仅作为笔记记录一下方法，以便以后备查，高手请路过。
 若是VC MS 封装了好几个方法
**1、最常用的GetCurrentDirectory，**查一下它的原型：
 DWORD GetCurrentDirectory(
  DWORD nBufferLength,  // size, in characters, of directory buffer
  LPTSTR lpBuffer       // pointer to buffer for current directory
);
很好理解，两个参数一个长度，一个目标指针Buffer，如：可以定义一个char path[MAX_PATH],也可以直接用CString path;
**2、用GetModuleFileName**
 如：
 HMODULE module = GetModuleHandle(0); 
CHAR buf[MAX_PATH]; 
GetModuleFileName(module, buf, sizeof buf); 
不过以上两种目录路径碰到文件夹带有“.”就完蛋了，如当前程序放在 D:\\myproject\\test-1.0\\win-app\\目录下，要想通过GetCurrentDiretory或GetModuleFileName来获取当前目录（想要得到：D:\\myproject\\test-1.0\\win-app）， 永远不正确，每次都是:D:\\myproject\\test-1.0 目录,去掉"1.0"之后，就能正确得到D:\\myproject\\test-1.0\\win-app目录。我不知道这两个函数对于这种情况是BUG还是Ms有意为之。
为了这个问题，我折腾了半天的时间，在开发的时候根本不会想那么多，直接用上上面函数来获取当前程序运行的目录了，结果也都正确，谁知发布的时候我自作聪明在文件夹名称加上版本号，结果死得很惨，上线一用永远得不到正常的结果。。找了半天不知道原因在哪？
最后尝试去文件的"1.0"字样结果就正常了。真是悲剧呀。。。
最后，我决定还是用回标准C++的写法。不相信MS那斯。
其实标准C++的写法更加简单，也有库函数可以调用，_getcwd; 
查一下MSDN
char *_getcwd(char *buffer, int maxlen); //ANSI版本
wchar_t *_wgetcwd(wchar_t *buffer, int maxlen ); //Unicode版本  
参数说明
*buffer:** Storage location for the path.maxlen:Maximum length of the path in characters: char for _getcwd and wchar_t for _wgetcwd.*
Returns a pointer to buffer. 
A NULL return value indicates an error, and errno is set either to ENOMEM, 
indicating that there is insufficient memory to allocate maxlen bytes 
(when a NULL argument is given as buffer), or to ERANGE, indicating that the path is longer than maxlen characters. 
If maxlen is less than or equal to zero, this function invokes an invalid parameter handler, as described in [Parameter Validation](http://msdn.microsoft.com/en-us/library/ksazx244.aspx).
这是MSND上解释，非常好用。。建议大家用这个_getcwd，而不要用MFC那些神话般的函数。 
