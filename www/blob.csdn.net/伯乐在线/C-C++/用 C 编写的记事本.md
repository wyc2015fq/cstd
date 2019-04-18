# 用 C 编写的记事本 - 文章 - 伯乐在线
原文出处： [yueguanghaidao](http://blog.csdn.net/yueguanghaidao/article/details/6949249)
最近想好好研究C，于是便拿记事本这个小程序练练手。
自己从网上找了一个挺不错的，就从这个开始学习吧。。。。
一：代码
先建立一文件夹->方便管理与查看
拿我来说，在d盘根目录下新建note文件夹
1.新建一note.rc资源文件，代码如下。


```
#include "sample.h"  
  
A ICON MOVEABLE PURE LOADONCALL DISCARDABLE "sample.ico"  
  
MAINMENU MENU   
{  
  POPUP "文件(&F)"  
 {  
  MENUITEM "打开(&O)...", CM_FILE_OPEN  
  MENUITEM "另存为(&S)...", CM_FILE_SAVEAS  
  MENUITEM SEPARATOR  
  MENUITEM "关闭", CM_FILE_EXIT  
 }  
  
 POPUP "帮助(&H)"  
 {  
  MENUITEM "关于(&A)", CM_ABOUT  
 }  
}
```
2.新建sample.h,代码如下


```
#define CM_FILE_SAVEAS    9072  
#define CM_FILE_EXIT    9071  
#define CM_FILE_OPEN    9070  
#define CM_ABOUT        9069
```
3.新建源程序note.c ,代码如下


```
#include <windows.h>  
#include "sample.h"  
  
static char g_szClassName[] = "MyWindowClass";  
static HINSTANCE g_hInst = NULL;  
#define IDC_MAIN_TEXT   1001  
  
BOOL LoadFile(HWND hEdit, LPSTR pszFileName)  
{  
   HANDLE hFile;  
   BOOL bSuccess = FALSE;  
  
  
   hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,  
      OPEN_EXISTING, 0, 0);  
   if(hFile != INVALID_HANDLE_VALUE)  
   {  
      DWORD dwFileSize;  
      dwFileSize = GetFileSize(hFile, NULL);  
      if(dwFileSize != 0xFFFFFFFF)  
      {  
         LPSTR pszFileText;  
         pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);  
         if(pszFileText != NULL)  
         {  
            DWORD dwRead;  
            if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))  
            {  
               pszFileText[dwFileSize] = 0;   
               if(SetWindowText(hEdit, pszFileText))  
                  bSuccess = TRUE;   
            }  
            GlobalFree(pszFileText);  
         }  
      }  
      CloseHandle(hFile);  
   }  
   return bSuccess;  
}  
  
  
BOOL SaveFile(HWND hEdit, LPSTR pszFileName)  
{  
   HANDLE hFile;  
   BOOL bSuccess = FALSE;  
   hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0,  
      CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);  
   if(hFile != INVALID_HANDLE_VALUE)  
   {  
      DWORD dwTextLength;  
      dwTextLength = GetWindowTextLength(hEdit);  
      if(dwTextLength > 0)  
      {  
         LPSTR pszText;  
         pszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);  
         if(pszText != NULL)  
         {  
            if(GetWindowText(hEdit, pszText, dwTextLength + 1))  
            {  
               DWORD dwWritten;  
               if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))  
                  bSuccess = TRUE;  
            }  
            GlobalFree(pszText);  
         }  
      }  
      CloseHandle(hFile);  
   }  
   return bSuccess;  
}  
  
  
BOOL DoFileOpenSave(HWND hwnd, BOOL bSave)  
{  
   OPENFILENAME ofn;  
   char szFileName[MAX_PATH];  
  
  
   ZeroMemory(&ofn, sizeof(ofn));  
   szFileName[0] = 0;  
  
  
   ofn.lStructSize = sizeof(ofn);  
   ofn.hwndOwner = hwnd;  
   ofn.lpstrFilter = "文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0\0";  
   ofn.lpstrFile = szFileName;  
   ofn.nMaxFile = MAX_PATH;  
   ofn.lpstrDefExt = "txt";  
  
  
   if(bSave)  
   {  
      ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |  
         OFN_OVERWRITEPROMPT;  
           
      if(GetSaveFileName(&ofn))  
      {  
         if(!SaveFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))  
         {  
            MessageBox(hwnd, "保存文件失败.", "错误信息",  
               MB_OK | MB_ICONEXCLAMATION);  
            return FALSE;  
         }  
      }  
   }  
   else  
   {  
      ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;  
      if(GetOpenFileName(&ofn))  
      {  
         if(!LoadFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))  
         {  
            MessageBox(hwnd, "打开文件失败.", "错误信息",  
               MB_OK | MB_ICONEXCLAMATION);  
            return FALSE;  
         }  
      }  
   }  
   return TRUE;  
}  
  
  
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)  
{  
   switch(Message)  
   {  
      case WM_CREATE:  
         CreateWindow("EDIT", "",  
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE |  
               ES_WANTRETURN,  
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  
            hwnd, (HMENU)IDC_MAIN_TEXT, g_hInst, NULL);  
  
  
         SendDlgItemMessage(hwnd, IDC_MAIN_TEXT, WM_SETFONT,  
            (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));  
      break;  
      case WM_SIZE:  
         if(wParam != SIZE_MINIMIZED)  
            MoveWindow(GetDlgItem(hwnd, IDC_MAIN_TEXT), 0, 0, LOWORD(lParam),  
               HIWORD(lParam), TRUE);  
      break;  
      case WM_SETFOCUS:  
         SetFocus(GetDlgItem(hwnd, IDC_MAIN_TEXT));  
      break;  
      case WM_COMMAND:  
         switch(LOWORD(wParam))  
         {  
            case CM_FILE_OPEN:  
               DoFileOpenSave(hwnd, FALSE);  
            break;  
            case CM_FILE_SAVEAS:  
               DoFileOpenSave(hwnd, TRUE);  
            break;  
            case CM_FILE_EXIT:  
               PostMessage(hwnd, WM_CLOSE, 0, 0);  
            break;  
            case CM_ABOUT:  
               MessageBox (NULL, "一个Windows C 记事本程序" , "关于...", 0);  
         }  
      break;  
      case WM_CLOSE:  
         DestroyWindow(hwnd);  
      break;  
      case WM_DESTROY:  
         PostQuitMessage(0);  
      break;  
      default:  
         return DefWindowProc(hwnd, Message, wParam, lParam);  
   }  
   return 0;  
}  
  
  
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  
   LPSTR lpCmdLine, int nCmdShow)  
{  
   WNDCLASSEX WndClass;  
   HWND hwnd;  
   MSG Msg;  
  
   g_hInst = hInstance;  
  
   WndClass.cbSize        = sizeof(WNDCLASSEX);  
   WndClass.style         = 0;  
   WndClass.lpfnWndProc   = WndProc;  
   WndClass.cbClsExtra    = 0;  
   WndClass.cbWndExtra    = 0;  
   WndClass.hInstance     = g_hInst;  
   WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);  
   WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);  
   WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);  
   WndClass.lpszMenuName  = "MAINMENU";  
   WndClass.lpszClassName = g_szClassName;  
   WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);  
  
  
   if(!RegisterClassEx(&WndClass))  
   {  
      MessageBox(0, "注册窗口失败", "错误信息",  
         MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);  
      return 0;  
   }  
  
   hwnd = CreateWindowEx(  
      WS_EX_CLIENTEDGE,  
      g_szClassName,  
      "一个记事本程序",  
      WS_OVERLAPPEDWINDOW,  
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,  
      NULL, NULL, g_hInst, NULL);  
  
  
   if(hwnd == NULL)  
   {  
      MessageBox(0, "创建窗口失败", "错误信息",  
         MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);  
      return 0;  
   }  
  
  
   ShowWindow(hwnd, nCmdShow);  
   UpdateWindow(hwnd);  
  
  
   while(GetMessage(&Msg, NULL, 0, 0))  
   {  
      TranslateMessage(&Msg);  
      DispatchMessage(&Msg);  
   }  
   return Msg.wParam;  
}
```
二：编译，连接运行
在编译之前需要一个sample.ico图标.
打开运行，输入cmd,回车，进入dos命令行
进入d:note下
1.用dir命令列一下当前目录包含文件。
![](http://jbcdn2.b0.upaiyun.com/2016/11/3cc6c19616aa5c63142288356c581577.jpg)
2.编译源程序（用cl编译器，/c的意思是：只编译，不连接）
![](http://jbcdn2.b0.upaiyun.com/2016/11/dc1a8c7e517d77e4d2c12c444965b328.jpg)
用dir命令查看一下当前目录，是不是多了一个note.obj文件。
3.编译资源文件（用rc编译器编译）
![](http://jbcdn2.b0.upaiyun.com/2016/11/a20e7bf1d70664d0c187240c3912256a.jpg)
用dir命令查看一下当前目录，是不是多了一个note.RES文件.
4.连接
![](http://jbcdn2.b0.upaiyun.com/2016/11/1c81b45273b597baa82c12b815bdea12.jpg)
用dir命令查看一下当前目录，是不是多了一个note.exe可执行查询.
打开看看，是不是很有成就感啊，没有借助任何编译器。
我们不仅要问，为什么要加入那么多的lib文件呢？
在windows编程中，有三个基本的lib文件，即kernel32.lib,user32.lib,gdi32.lib
所以一般就添加这几个，我们连接一下试试。
不幸的是报错了，我们看看
![](http://jbcdn2.b0.upaiyun.com/2016/11/bc2b6aff67556dc9cdcd6379dd23962b.jpg)
这一看就晕了，什么意思啊
稍微有经验的的人就知道这是缺少lib库文件。
GetOpenFileName这个函数用MSDN查看一下，翻到末尾，有没有看到
**Import Library: **Use comdlg32.lib   -》这个提示
所以我们把comdlg32.lib加进去。
这里有一个问题？为什么头文件sample.h没有用到呢？
解答：在编译连接时不需要指定头文件，将头文件放到当前目录下，系统会自动引用。
三：下面的工作就是仔细分析代码了，借助MSDN看懂不是很难。
