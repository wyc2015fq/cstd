# C++ 中定时器的用法 - 长歌行 - CSDN博客





2012年03月05日 09:51:06[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：9008








转载的地址：

http://blog.163.com/linzuxin@126/blog/static/340740572008101311552948/




1.1 
  用WM_TIMER来设置定时器

先请看SetTimer这个API函数的原型 



```cpp
UINT_PTR   SetTimer( 
HWND   hWnd,   //   窗口句柄 
UINT_PTR   nIDEvent,   //   定时器ID，多个定时器时，可以通过该ID判断是哪个定时器 
UINT   uElapse,   //   时间间隔,单位为毫秒 
TIMERPROC   lpTimerFunc   //   回调函数 
);
```



例如 
SetTimer(m_hWnd,1,1000,NULL);   //一个1秒触发一次的定时器 
在MFC程序中SetTimer被封装在CWnd类中，调用就不用指定窗口句柄了，例如: 

UINT   SetTimer(1,100,NULL); 
函数反回值就是第一个参数值1，表示此定时器的ID号。 

第二个参数表示要等待100毫秒时间再重新处理一次。第三个参数在这种方法中一般用NULL。 
注意：设置第二个参数时要注意，如果设置的等待时间比处理时间短，程序就会出问题了。 

1.2   调用回调函数 

此方法首先写一个如下格式的回调函数 

void   CALLBACK   TimerProc(HWND   hWnd,UINT   nMsg,UINT   nTimerid,DWORD   dwTime); 
然后再用SetTimer(1,100,TimerProc)函数来建一个定时器，第三个参数就是回调函数地址。 

2、多个定时器的实现与应用 


我们在安装定时器时都为其指定了ID，使用多个定时器时，该ID就发挥作用了。 
不使用MFC时，当接收到WM_TIMER消息，WPARAM   wParam中的值便是该定时器的ID 
使用MFC时就更简单了，我们为其增加WM_TIME的消息处理函数OnTimer即可，请看如下例子 

```cpp
void   CTimerTestDlg::OnTimer(UINT   nIDEvent) 
{ 
switch   (nIDEvent) 
{ 
case   24:   ///处理ID为24的定时器 
Draw1(); 
break; 
case   25:   ///处理ID为25的定时器 
Draw2(); 
break; 
} 
CDialog::OnTimer(nIDEvent); 
}
```


当你用回调函数时，我们可以根据nTimerid的值来判断是哪个定时器，例如: 

```cpp
void   CALLBACK   TimerProc(HWND   hWnd,UINT   nMsg,UINT   nTimerid,DWORD   dwTime) 
{ 
switch(nTimerid) 
{ 
case   1:   ///处理ID为1的定时器 
Do1(); 
break; 
case   2:   ///处理ID为2的定时器 
Do2(); 
break; 
} 
}
```


3、取消定时器 

不再使用定时器后，我们应该调用KillTimer来取消定时，KillTimer的原型如下 


```cpp
BOOL   KillTimer( 
HWND   hWnd,   //   窗口句柄 
UINT_PTR   uIDEvent   //   ID 
);
```


在MFC程序中我们可以直接调用KillTimer(int   nIDEvent)来取消定时器。
例子

```cpp
#include   <windows.h> 
#include   <iostream> 
VOID   CALLBACK   TimerProc(HWND   hwnd,UINT   uMsg,UINT   idEvent,DWORD   dwTime); 
VOID   CALLBACK   TimerProc(HWND   hwnd,UINT   uMsg,UINT   idEvent,DWORD   dwTime) 
{ 
std::cout   < <   "hello "   < <   std::endl; 
} 

void   main() 
{ 
int   timer1   =   1; 
HWND   hwndTimer;       
MSG   msg;                     

SetTimer(NULL,timer1,5000,TimerProc); 
int   itemp; 
while ( (itemp = GetMessage(&msg, NULL,NULL,NULL))&& (itemp!=0) &&  (-1 !=  itemp)) 
{   
   if   (msg.message   ==   WM_TIMER)   
   {   
    std::cout   < <   "i   got   the   message "   < <   std::endl; 
    TranslateMessage(&msg);   
    DispatchMessage(&msg);     
    }   
}   
}
```



输出如下： 
i   got   the   message 
hello 
i   got   the   message 
hello 
i   got   the   message 
hello


```cpp
---------------------------------------------------------------------------------------------------------------------------

// timer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include   <windows.h>  
#include   <stdio.h>  
#include   <conio.h>  

unsigned   long   WINAPI   Thread(PVOID   pvoid);  
void   main()  
{  
    DWORD   dwThreadId;  
    printf("use   timer   in   workthread   of   console   application<masterz>\n");  
    HANDLE   hThread   =   CreateThread(    
        NULL,                                                 //   no   security   attributes    
        0,                                                       //   use   default   stack   size      
        Thread,                                     //   thread   function    
        0,                                 //   argument   to   thread   function    
        0,                                                       //   use   default   creation   flags    
        &dwThreadId);    
    DWORD   dwwait=WaitForSingleObject(hThread,1000*30);  
    switch(dwwait)  
    {  
    case   WAIT_ABANDONED:  
        printf("main   thread   WaitForSingleObject   return   WAIT_ABANDONED\n");  
        break;  
    case   WAIT_OBJECT_0:  
        printf("main   thread   WaitForSingleObject   return   WAIT_OBJECT_0\n");  
        break;  
    case   WAIT_TIMEOUT:  
        printf("main   thread   WaitForSingleObject   return   WAIT_TIMEOUT\n");  
        break;  
    }  
    CloseHandle(hThread);  
    _getch();  
}  

unsigned   long   WINAPI   Thread(PVOID   pvoid)  
{  
    MSG   msg;  
    PeekMessage(&msg,   NULL,   WM_USER,   WM_USER,   PM_NOREMOVE);  
    UINT   timerid=SetTimer(NULL,111,3000,NULL);  
    BOOL   bRet;  
    int   count   =0;  
    while(   (bRet   =   GetMessage(   &msg,   NULL,   0,   0   ))   !=   0)  
    {    
        if   (bRet   ==   -1)  
        {  
            //   handle   the   error   and   possibly   exit  
        }  
        else  
            if(msg.message==WM_TIMER)  
            {  
                count++;  
                printf("WM_TIMER   in   work   thread   count=%d\n",count);  
                if(count>4)  
                    break;  
            }  
            else  
            {  
                TranslateMessage(&msg);    
                DispatchMessage(&msg);    
            }  
    }  
    KillTimer(NULL,timerid);  
    printf("thread   end   here\n");  
    return   0;  
}
```





