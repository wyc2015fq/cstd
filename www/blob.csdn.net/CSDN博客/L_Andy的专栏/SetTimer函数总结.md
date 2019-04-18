# SetTimer函数总结 - L_Andy的专栏 - CSDN博客

2013年11月26日 11:24:07[卡哥](https://me.csdn.net/L_Andy)阅读数：851
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



                
在编程时，会经常使用到定时器。使用定时器的方法比较简单，通常告诉Windows一个时间间隔，然后WINDOWS以此时间间隔周期性触发程序。通常有两种方法来实现：发送WM_TIMER消息和调用应用程序定义的回调函数。

1.1 用WM_TIMER来设置定时器

先请看SetTimer这个API函数的原型

UINT_PTR SetTimer(

HWND hWnd,              // 窗口句柄

UINT_PTR nIDEvent,      // 定时器ID，多个定时器时，可以通过该ID判断是哪个定时器

UINT uElapse,           // 时间间隔,单位为毫秒

TIMERPROC lpTimerFunc   // 回调函数

);

例如 

SetTimer(m_hWnd,1,1000,NULL); //一个1秒触发一次的定时器

在MFC程序中SetTimer被封装在CWnd类中，调用就不用指定窗口句柄了，例如:

UINT SetTimer(1,100,NULL);

函数反回值就是第一个参数值1，表示此定时器的ID号。

第二个参数表示要等待100毫秒时间再重新处理一次。第三个参数在这种方法中一般用NULL。

注意：设置第二个参数时要注意，如果设置的等待时间比处理时间短，程序就会出问题了。

1.2 调用回调函数

此方法首先写一个如下格式的回调函数

void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);

然后再用SetTimer(1,100,TimerProc)函数来建一个定时器，第三个参数就是回调函数地址。

二、多个定时器的实现与应用

我们在安装定时器时都为其指定了ID，使用多个定时器时，该ID就发挥作用了。

不使用MFC时，当接收到WM_TIMER消息，WPARAM wParam中的值便是该定时器的ID

使用MFC时就更简单了，我们为其增加WM_TIME的消息处理函数OnTimer即可，请看如下例子 

void CTimerTestDlg::OnTimer(UINT nIDEvent)

{

switch (nIDEvent)

{

case 24: ///处理ID为24的定时器

   Draw1();

   break;

case 25: ///处理ID为25的定时器

   Draw2();

   break;

}

CDialog::OnTimer(nIDEvent);

}

当你用回调函数时，我们可以根据nTimerid的值来判断是哪个定时器，例如:

void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)

{

   switch(nTimerid)

   {

   case 1: ///处理ID为1的定时器

         Do1();

         break;

   case 2: ///处理ID为2的定时器

         Do2();

         break;

   }

}
三、取消定时器

不再使用定时器后，我们应该调用KillTimer来取消定时，KillTimer的原型如下

BOOL KillTimer(

HWND hWnd,          // 窗口句柄

UINT_PTR uIDEvent   // ID

);

在MFC程序中我们可以直接调用KillTimer(int nIDEvent)来取消定时器。

如：KillTimer(1);
            

