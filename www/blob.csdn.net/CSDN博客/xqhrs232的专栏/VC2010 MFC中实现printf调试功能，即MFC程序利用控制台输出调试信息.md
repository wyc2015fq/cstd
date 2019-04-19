# VC2010 MFC中实现printf调试功能，即MFC程序利用控制台输出调试信息 - xqhrs232的专栏 - CSDN博客
2017年07月06日 11:26:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：335
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.chinaunix.net/uid-20672257-id-2973396.html](http://blog.chinaunix.net/uid-20672257-id-2973396.html)

1、在项目自动生成的stdafx.h文件中添加下面头文件
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
2、把下面的函数加到你初始化的地方，然后你就可以使用printf函数了
void InitConsoleWindow()
{
    int nCrt = 0;
    FILE* fp;
    AllocConsole();
    nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nCrt, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}
以下红色部分是我初始化函数中添加的
BOOL CSerialPortptestDlg::OnInitDialog()
{
 CDialogEx::OnInitDialog();
  CString str;
 int    Index;
 HKEY   hKey;
 LONG   ret;   
  OSVERSIONINFO     osvi;   
  BOOL   bOsVersionInfoEx;   
 char   keyinfo[100],comm_name[200],ValueName[200];   
 int   i;   
 DWORD   sType,Reserved,cbData,cbValueName;
 // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
 //  执行此操作
 SetIcon(m_hIcon, TRUE);   // 设置大图标
 SetIcon(m_hIcon, FALSE);  // 设置小图标
InitConsoleWindow();
 printf( "str   =   %s\n ",   "debug");
调用此函数后会弹出一个CONSOLE,然后printf的东西就会出现在上面。
[](http://blog.chinaunix.net/uid-20672257-id-2973396.html#)[](http://blog.chinaunix.net/uid-20672257-id-2973396.html#)[](http://blog.chinaunix.net/uid-20672257-id-2973396.html#)[](http://blog.chinaunix.net/uid-20672257-id-2973396.html#)[](http://blog.chinaunix.net/uid-20672257-id-2973396.html#)

