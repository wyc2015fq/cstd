# MFC SetTimer用法 - 三少GG - CSDN博客
2012年02月22日 22:49:19[三少GG](https://me.csdn.net/scut1135)阅读数：1612标签：[mfc																[timer																[null																[callback																[windows																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
### [MFC SetTimer用法](http://blog.csdn.net/loonghua1314/article/details/6057262)【[http://blog.csdn.net/loonghua1314/article/details/6057262](http://blog.csdn.net/loonghua1314/article/details/6057262)】
### 什么时候我们需要用到SetTimer函数呢？当你需要每个一段时间执行一件事的的时候就需要使用SetTimer函数了。 使用定时器的方法比较简单，通常告诉WINDOWS一个时间间隔，然后WINDOWS以此时间间隔周期性触发程序。通常有两种方法来实现：发送WM_TIMER消息和调用应用程序定义的回调函数。
1.1 用WM_TIMER来设置定时器
先请看SetTimer这个API函数的原型
UINT_PTR SetTimer(
  HWND hWnd,              // 窗口句柄
  UINT_PTR nIDEvent,      // 定时器ID，多个定时器时，可以通过该ID判断是哪个定时器
  UINT uElapse,           // 时间间隔,单位为毫秒
  TIMERPROC lpTimerFunc   // 回调函数
);
例如 
SetTimer(m_hWnd,1,1000,NULL); //一个1秒触发一次的定时器
在MFC程序中SetTimer被封装在CWnd类中，调用就不用指定窗口句柄了
于是SetTimer函数的原型变为：
UINT SetTimer(UINT nIDEvent,UINT nElapse,void(CALLBACK EXPORT *lpfnTimer)(HWND,UINT ,YINT ,DWORD))
当使用SetTimer函数的时候，就会生成一个计时器。函数中nIDEvent指的是计时器的标识，也就是名字。nElapse指的是时间间隔，也就是每隔多长时间触发一次事件。第三个参数是一个回调函数，在这个函数里，放入你想要做的事情的代码，你可以将它设定为NULL，也就是使用系统默认的回调函数，系统默认认的是onTime函数。这个函数怎么生成的呢？你需要在需要计时器的类的生成onTime函数：在ClassWizard里，选择需要计时器的类，添加WM_TIME消息映射，就自动生成onTime函数了。然后在函数里添加代码，让代码实现功能。每隔一段时间就会自动执行一次。
例：
SetTimer(1,1000,NULL);
1:计时器的名称；
1000：时间间隔，单位是毫秒；
NULL:使用onTime函数。
当不需要计时器的时候调用KillTimer(nIDEvent);
例如：KillTimer(1);
1.2 调用回调函数
此方法首先写一个如下格式的回调函数
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);
然后再用SetTimer(1,100,TimerProc)函数来建一个定时器，第三个参数就是回调函数地址。
二. 或许你会问，如果我要加入两个或者两个以上的 timer怎么办？
继续用SetTimer函数吧，上次的timer的ID是1，这次可以是2，3，4。。。。
SetTimer(2,1000,NULL);
SetTimer(3,500,NULL);
嗯，WINDOWS会协调他们的。当然onTimer函数体也要发生变化，要在函数体内添加每一个timer的处理代码：
onTimer(nIDEvent)
{
switch(nIDEvent)
{
case 1:........;
break;
case 2:.......;
break;
case 3:......;
break;
}
}
**分享到：**
