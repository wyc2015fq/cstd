# 从AP中获取电池和电源信息变更通知的方法 - xqhrs232的专栏 - CSDN博客
2010年02月24日 16:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：753标签：[wince																[null																[winapi																[input																[object																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
在WinCE的项目开发过程中经常要编写AP来获取电池的电量和电源的来源等信息，由于WinCE底层的电池驱动一般以查询的方式得到电池的状态然后更新到一个结构体中，AP可以调用GetSystemPowerStatusEx2来得到这个结构体的数值，为了实时的更新电池的信息AP必须频繁的调用函数去得到数据更新。
　　其实WinCE的电源管理中已经集成了一种notify机制，会在电池信息发生变化时发出提醒。
　　RequestPowerNotifications函数可以被AP用来请求接收这种提醒服务。
　　AP在调用这个API之前必须创建一个消息队列，可以用CreateMsgQueue来实现。
　　接受提醒的方式是使用WaitForSingleObject来实现，该函数会一直等待直到收到电源管理发来的提醒，然后AP可以去读取消息队列中的数据来判定具体电源系统发生了哪些变化，然后做相关的事情比如更新UI的显示等。
　　参考源代码：
　　//#################################################################
　　#include <Pm.h>
　　#define QUEUE_ENTRIES 3 
　　#define MAX_NAMELEN 200 
　　#define QUEUE_SIZE (QUEUE_ENTRIES * (sizeof(POWER_BROADCAST) + MAX_NAMELEN)) 
　　HANDLE hMsgQ;
　　DWORD WINAPI PowerChangeListen(void * temp_p)
　　{
　　UCHAR buf[QUEUE_SIZE]; 
　　unsigned long nRead = 0, flags = 0, res = 0;
　　while(1)
　　{
　　 DWORD dwRes = WaitForSingleObject(hMsgQ,INFINITE);
　　 if(dwRes==WAIT_OBJECT_0)
　　 {
　　 memset(&buf, 0, QUEUE_SIZE); 
　　 if (ReadMsgQueue(hMsgQ, &buf, QUEUE_SIZE, &nRead, INFINITE, &flags))
　　 {
　　 PPOWER_BROADCAST pB = (PPOWER_BROADCAST)&buf; 
　　 PPOWER_BROADCAST_POWER_INFO ppbpi = (PPOWER_BROADCAST_POWER_INFO) pB->SystemPowerState;
　　 if(pB->Message==PBT_POWERINFOCHANGE)
　　 {
　　 //在这里处理一些电池信息相关数据改变的事情
　　 // MessageBox(NULL,L"Battery info change",NULL,NULL);
　　 NKDbgPrintfW(L"[Fred]Battery info change BatteryLifePercent=%d/r/n",ppbpi->bBatteryLifePercent);
　　 }
　　 if(pB->Message==PBT_POWERSTATUSCHANGE)
　　 {
　　 //在这里处理一些电源输入状态改变 (AC/Battery)的事情
　　 //MessageBox(NULL,L"Power input change",NULL,NULL);
　　 NKDbgPrintfW(L"[Fred]Power input change ACIN=%d/r/n",ppbpi->bACLineStatus);
　　 }
　　 }
　　}
　　}
　　}
　　void Init_PowerNotify()
　　{
　　NKDbgPrintfW(L"[Fred]Init_PowerNotify++/r/n");
　　MSGQUEUEOPTIONS options = {0}; 
　　DWORD dwErr;
　　 options.dwSize = sizeof(MSGQUEUEOPTIONS); 
　　 options.dwFlags = 0; 
　　 options.dwMaxMessages = QUEUE_ENTRIES; 
　　 options.cbMaxMessage = sizeof(POWER_BROADCAST) + MAX_NAMELEN; 
　　 options.bReadAccess = TRUE; 
　　 hMsgQ = CreateMsgQueue(NULL, &options); 
　　if(!hMsgQ)
　　{
　　 dwErr=GetLastError();
　　 NKDbgPrintfW(L"[Fred]CreateMsgQueue failed/r/n");
　　 RETAILMSG(1, (TEXT("[Fred]CreateMessageQueue ERROR:%d/n"), dwErr)); 
　　 return; 
　　}
　　HANDLE hNotifications = RequestPowerNotifications(hMsgQ, POWER_NOTIFY_ALL); // Flags
　　 if (!hNotifications) { 
　　 dwErr = GetLastError(); 
　　 RETAILMSG(1, (TEXT("[Fred]RequestPowerNotifications ERROR:%d/n"), dwErr)); 
　　 StopPowerNotifications(hMsgQ);
　　 return;
　　 } 
　　CreateThread(NULL, 0, PowerChangeListen, NULL, 0, NULL);
　　NKDbgPrintfW(L"[Fred]Init_PowerNotify--/r/n");
　　}
　　//###############################################################################################
　　AP可以把上面的代码全部复制到自己的源码中，然后在初始化的时候调用一次Init_PowerNotify,之后就可以等待消息的发生（中文注释部分）
