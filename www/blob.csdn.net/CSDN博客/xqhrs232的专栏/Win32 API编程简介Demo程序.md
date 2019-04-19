# Win32 API编程简介Demo程序 - xqhrs232的专栏 - CSDN博客
2013年04月01日 14:59:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：634
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/markmin214/article/details/8735747#comments](http://blog.csdn.net/markmin214/article/details/8735747#comments)
Windows编程的方法主要有两种方法，
                    传统编写法：利用API函数；
                    交互式方法：利用MFC类库。
          为使程序员编写具有Windows风格的应用程序，Windows提供了一个应用程序编程接口(ApplicationProgrammingInterface，简称API)，它是Windows系统与Windows应用程序间的标准程序接口。API可以为应用程序提供windows系统特殊函数及数据结构。
MFC, MicrosoftFoundation ClassLibrary，微软基础类库，约有200个类。MFC封装了大部分API函数，并提供了一个应用程序框架，简化了和标准了windows程序设计。
**开始做第一个****WIn32 API****程序**
 【文件】→【新建】，打开【工程】选项页，选择【Win32Application】，在“位置”中修改工程保存的路径。在“工程名”中写入你为这个工程取的名字。下一步【一个简单的Windows程序】
在WinMain函数中输入：MessageBox(NULL,TEXT("Hello
 world"),TEXT("First"),MB_OK);
编译运行：
![](https://img-my.csdn.net/uploads/201303/29/1364527631_2223.PNG)
事件驱动
Windows程序设计是针对事件或消息的，它的执行顺序取决于事件发生的顺序。在Windows环境下，应用程序启动后，系统等待用户在图形用户界面内的输入选择，如鼠标按键、键盘按键、窗口被创建、关闭、改变大小、移动等，对系统来说，这都是事件，都会产生相应的windows消息。事件驱动编程方法对于编写交互式程序很有用处，它避免了死板的操作模式。
消息是描述事件发生的信息。
Windows应用程序通过消息进行信息交换。
在Windows中，消息用结构体MSG表示，其结构如下：
Typedef  struct tagMSG
{ HWND  hwnd; //窗口句柄
   UNIT   message; //消息号。
   WPARAM wParam;//用于提供消息的附加消息。
   LPARAM  lParam;//用于提供消息的附加消息。
   DWORD  time;//指定消息送至队列的时间
   POINT pt;  //指定消息发送时屏幕光标的位置
}MSG;
句柄，顾名思义，指的是一个windows对象的把柄。
Windows中的句柄都是32位的指针变量，用来指向某个windows对象所占据的内存区。
句柄的使用，极大地方便了Windows管理其内存中的各种windows对象。
典型的Windows程序结构
Windows应用程序的主体由以下两个函数完成：WinMain()函数负责建立窗口和建立消息循环，WinProc()函数负责消息的处理。
![](https://img-my.csdn.net/uploads/201303/29/1364527772_7165.PNG)
WinMain()函数类似C语言中的Main函数，是Win32应用程序入口函数。WinMain()函数主要完成创建主窗口，并产生和处理消息循环。
WinMain()函数的大致结构
```cpp
intPASCAL WinMain(…… )
{
//①定义窗口类：
     WNDCLASS wndclass;
     wndclass.style=CS_HREDRAW|CS_VREDRAW;
     ……                                 十个字段
     wndclass.lpszClassName=”Lei”;
//②注册窗口类：
      RegisterClass(&wndclass);
//③创建窗口：
      HWND hwnd;
      hwnd=CreateWindow();
//④显示及更新窗口：
      ShowWindow()；
      UpdateWindow();
//⑤消息循环：
      MSG msg;
      While(GetMessage(&msg,NULL,0,0))
      {   TranlateMessage(&msg);
            DispatchMessage(&msg)
      }
     return msg.wParam;
}
```
消息处理函数WndProc的大致结构
```cpp
LRESULTCALLBACK WndProc(HWND hMainwnd, UINT message, WPARAM wParam, LPARAMlParam)  //窗口函数的定义
{     switch( message )
       {  case WM_KEYDOWN://击键消息
 {   ……  ;
                break;
  }         
           case ……:
          {   ……;
                   break;
           }
  ……
  case WM_DESTROY://关闭应用程序窗口时发出的消息
  {   PostQuitMessage(0);//发出WM_QUIT消息，结束应用程序
       return 0;
   }
   default : break;
}
  //调用缺省消息处理过程
  return DefWindowProc(hMainwnd, message,wParam, lParam);
}
```
窗口创建DEMO
#include<windows.h>//头文件
LRESULT CALLBACKWndProc(HWND,UINT,WPARAM,LPARAM);
//窗口函数声明，这一函数将处理发往应用程序窗口的各种消息
charszClassName[]="windowclass";//窗口结构体的名称
charszAppTitle[]="API建立窗口实例";//窗口的标题
INT PASCALWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam , INTnCmdShow)
//WinMain()函数的定义
{
HWND hMainWnd;//窗口句柄
MSG msg;//消息结构体
WNDCLASSwinclass;//窗口结构体
if(!hPrevInstance)
{ 
winclass.style=CS_HREDRAW|CS_VREDRAW;//窗口风格
winclass.lpfnWndProc=WndProc;
winclass.cbClsExtra=0;//附加参数
winclass.cbWndExtra=0;//附加参数
winclass.hInstance=hInstance;//当前应用程序实例句柄
winclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);//窗口图标
winclass.hCursor=LoadCursor(NULL,IDC_ARROW);//窗口光标          
winclass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
//窗口背景色为浅灰色
winclass.lpszMenuName=NULL;//窗口菜单
winclass.lpszClassName=szClassName;//给窗口结构体命名，
//CreateWindow()函数将根据窗口结构体的名称来建立窗口
RegisterClass(&winclass);//注册窗口
}
//下面用CreateWindow()函数来建立窗口，并返回所建立窗口的句柄
hMainWnd=CreateWindow(
  szClassName,//窗口结构体的名称
  szAppTitle,//窗口的标题
  WS_OVERLAPPEDWINDOW,//窗口风格为可重叠窗口
  //下面四个参数代表窗口左上角x，y坐标和窗口的宽度与高度，都使
  //用缺省值
  CW_USEDEFAULT,CW_USEDEFAULT,
  CW_USEDEFAULT,CW_USEDEFAULT,
  //下面参数分别为父窗口句柄、窗口菜单句柄、应用程序实例句柄
  //和附加参数
  NULL,NULL,hInstance,NULL);
ShowWindow(hMainWnd,SW_SHOWNORMAL);
//显示最大化窗口
UpdateWindow(hMainWnd);//更新窗口
//下面用While()循环来建立消息循环
while(GetMessage(&msg,NULL,0,0))//获取消息，填充msg结构体
{
TranslateMessage(&msg);//翻译键盘消息
DispatchMessage(&msg);//向窗口函数发送消息，让窗口函数处理
}
return msg.wParam;
}
LRESULT CALLBACKWndProc(HWND hMainwnd, UINT message, WPARAM wParam, LPARAM lParam)  //窗口函数的定义
{        switch(message )
{   case WM_KEYDOWN://击键消息
{  MessageBox(hMainwnd,"键按下了!","Keyboard",MB_OK);
break;
}         
           case WM_RBUTTONDOWN://鼠标消息
   {  MessageBox(hMainwnd,"鼠标右键按下了!","Mouse",MB_OK);
   break;
   }
   case WM_LBUTTONDOWN://鼠标消息
   {   MessageBox(hMainwnd,"鼠标左键按下了!","Mouse",MB_OK);
                     break;
   }
   case WM_DESTROY://关闭应用程序窗口时发出的消息
   {   PostQuitMessage(0);//发出WM_QUIT消息，结束应用程序
   return 0;
   }
   default : break;
}
//调用缺省消息处理过程
returnDefWindowProc(hMainwnd, message, wParam, lParam);
}
   case WM_RBUTTONDOWN:
  {        MessageBox(GetFocus(),messageright,"API建立窗口实例",MB_OK|MB_ICONINFORMATION);
   break;
   }
   case WM_LBUTTONDOWN:
   {         MessageBox(GetFocus(),messageleft,"API建立窗口实例",MB_OK|MB_ICONINFORMATION);
   break;
   }
   case WM_DESTROY://关闭应用程序窗口时发出的消息
  {        PostQuitMessage(0);
   //发出WM_QUIT消息，结束应用程序
   return 0;
   }
   default:        break;
 }
 returnDefWindowProc(hMainwnd,message,wParam,lParam);
 //其它没有处理的消息交给Windows做默认处理
}
对话框demo：
BOOL WINAPIMain_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG,Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND,Main_OnCommand);
HANDLE_MSG(hWnd,WM_CLOSE,Main_OnClose);
    }
    return FALSE;
}
BOOLMain_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    return TRUE;
}
voidMain_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
caseIDC_OK:
{
MessageBox(hwnd,TEXT("欢迎"),TEXT("Hello"),MB_OK);
}
        break;
default:
break;
    }
}
voidMain_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}
int APIENTRYWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
//EnableIPAddress、Calendar.etc
InitCommonControls();
DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN), NULL, Main_Proc);
return0;
}
  觉得还不错就顶一下吧O(∩_∩)O谢谢支持
