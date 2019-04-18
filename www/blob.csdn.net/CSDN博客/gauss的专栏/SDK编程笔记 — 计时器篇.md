# SDK编程笔记 — 计时器篇 - gauss的专栏 - CSDN博客
2011年12月13日 21:35:59[gauss](https://me.csdn.net/mathlmx)阅读数：255标签：[编程																[timer																[windows																[null																[microsoft																[callback](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
**SDK编程笔记 — 计时器篇****两个计时器API的讨论**
　　SetTimer函数用于创建一个计时器，KillTimer函数用于销毁一个计时器。计时器属于系统资源，使用完应及时销毁。
**SetTimer**的函数原型如下：
UINT_PTR SetTimer( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc ) ;
　　其中
　　hWnd是和timer关联的窗口句柄，此窗口必须为调用SetTimer的线程所有；如果hWnd为NULL，没有窗口和timer相关联并且nIDEvent参数被忽略
　　nIDEvent是timer的标识，为非零值；如果hWnd为NULL则被忽略；如果hWnd非NULL而且与timer相关联的窗口已经存在一个为此标识的timer，则此次SetTimer调用将用新的timer代替原来的timer。timer标识和窗口相关，两个不同的窗口可以拥有nIDEvent相同的tiemr
　　uElapse是以毫秒指定的计时间隔值，范围为1毫秒到4,294,967,295毫秒（将近50天），这个值指示Windows每隔多久时间给程序发送WM_TIMER消息。
　　lpTimerFunc是一个回调函数的指针，俗称TimerFunc；如果lpTimerFunc为NULL，系统将向应用程序队列发送WM_TIMER消息；如果lpTimerFunc指定了一个值，DefWindowProc将在处理WM_TIMER消息时调用这个lpTimerFunc所指向的回调函数，因此即使使用TimerProc代替处理WM_TIMER也需要向窗口分发消息。
　　关于SetTimer的返回值：如果hWnd为NULL，返回值为新建立的timer的ID，如果hWnd非NULL，返回一个非0整数，如果SetTimer调用失败则返回0
**KillTimer**的函数原型为：BOOL KillTimer( HWND hWnd, UINT_PTR uIDEvent ) ; 参数意义同SetTimer。
　　关于KillTimer对消息队列中剩余未处理的WM_TIMER消息的影响，MSDN和Programming Windows上的说法完全相反。MSDN的说法很干脆：The KillTimer function does not remove WM_TIMER messages already posted to the message queue. 而petzold则说
The KillTimer call purges the message queue of any pending WM_TIMER messages. Your program will never receive a stray WM_TIMER message following a KillTimer call.(KillTimer消除消息队列中任何未处理的WM_TIMER消息，调用KillTimer后你的程序永远不会收到一条“漂泊游荡”的WM_TIMER消息)
**关于WM_TIMER消息**
　　wParam为计时器的ID；如果需要设定多个计时器，那么对每个计时器都使用不同的计时器ID。wParam的值将随传递到窗口过程中的WM_TIMER消息的不同而不同。
　　lParam为指向TimerProc的指针，如果调用SetTimer时没有指定TimerProc(参数值为NULL)，则lParam为0(即NULL)。 
　　可以通过在窗口过程中提供一个WM_TIMER case处理这个消息，或者，默认窗口过程会调用SetTimer中指定的TimerProc来处理WM_TIMER消息
**使用计时器的三种方法**
　　如果在程序的整个执行过程中使用计时器，一般在处理WM_CREATE消息时或WinMain中消息循环前调用SetTimer,在处理WM_DESTROY消息时或在WinMain中消息循环后return前调用KillTimer。根据SetTimer中的参数不同，有三种方法使用计时器。
　　方法一：调用SetTimer时指定窗口句柄hWnd，nIDEvent中指定计时器ID，将lpTimerFunc置NULL从而不使用TimerProc；在窗口过程中处理WM_TIMER消息。调用KillTimer时，使用SetTimer中指定的hWnd和id。最好使用#define定义timer的id，例如：
#define ID_TIMER 1 
SetTimer(hWnd,ID_TIMER,1000,NULL) ;
KillTimer(hWnd,ID_TIMER) ;
　　方法二：调用SetTimer时指定窗口句柄hWnd，nIDEvent中指定计时器ID，lpTimerFunc参数不为NULL而指定为TimerProc函数的指针。这种方法使用TimerProc函数(名字可自定)处理WM_TIMER消息：
VOID CALLBACK TimerProc ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
　//处理WM_TIMER讯息 
}
　　　TimerProc的参数hwnd是在调用SetTimer时指定的窗口句柄。Windows只把WM_TIMER消息送给TimerProc，因此消息参数总是等于WM_TIMER。iTimerID值是计时器ID，dwTimer值是与从GetTickCount函数的返回值相容的值。这是自Windows启动后所经过的毫秒数。 使用这种方法时，相关函数调用的形式为：
SetTimer(hWnd,ID_TIMER,1000,TimerProc) ;
KillTimer(hWnd,ID_TIMER) ;
　　方法三：调用SetTimer时不指定窗口句柄(为NULL)，iTimerID参数自然被忽略，lpTimerFunc不为NULL而指定为TimerProc的指针。正如上面SetTimer的讨论中所说的，此时SetTimer的返回值正是新建立的计时器的ID，需将这个ID保存以供KillTimer销毁计时器时所用。当然，KillTimer的hWnd参数也置为NULL。这种方法同样用TimerProc处理WM_TIMER消息。
UINT_PTR iTimerID ; 
iTimerID = SetTimer(NULL,0,1000,TimerProc) ;
KillTimer(NULL,iTimerID) ;
　　使用这种方法的好处是不必自己指定计时器ID,这样就不必担心用错ID。
**使用多个计时器**
　　使用多个计时器只要在建立计时器时指定不同的ID。比如用上面所述方法一时的情况：
#define TIMER_SEC 1
#define TIMER_MIN 2
然后使用两个SetTimer来设定两个计时器：
SetTimer (hwnd, TIMER_SEC, 1000, NULL) ;
SetTimer (hwnd, TIMER_MIN, 60000, NULL) ; 
WM_TIMER的处理如下所示：
case WM_TIMER:
　switch (wParam)
　{
　　case TIMER_SEC:
　　　//每秒一次的处理 
　　　break ;
　　case TIMER_MIN:
　　　//每分钟一次的处理
　　　break ;
　}
　return 0 ;
**改变计时器的时间间隔**
　　如果想将一个已经存在的计时器设定为不同的时间间隔，可以简单地用不同的时间值再次调用SetTimer。
**计时器精确吗？**
　　计时器并不精确。有两个原因：
　　原因一：Windows计时器是硬件和ROM BIOS架构下之计时器一种相对简单的扩充。回到Windows以前的MS-DOS程序写作环境下，应用程式能够通过拦截者称为timer tick的BIOS中断来实现时钟或计时器。一些为MS-DOS编写的程序自己拦截这个硬件中断以实现时钟和计时器。这些中断每54.915毫秒产生一次，或者大约每秒18.2次。这是原始的IBM PC的微处理器频率值4.772720 MHz被218所除而得出的结果。在Windows 98中，计时器与其下的PC计时器一样具有55毫秒的解析度。在Microsoft
 Windows NT中，计时器的解析度为10毫秒。Windows应用程式不能以高于这些解析度的频率（在Windows 98下，每秒18.2次，在Windows NT下，每秒大约100次）接收WM_TIMER消息。在SetTimer中指定的时间间隔总是截尾后tick数的整数倍。例如，1000毫秒的间隔除以54.925毫秒，得到18.207个tick，截尾后是18个tick，它实际上是989毫秒。对每个小于55毫秒的间隔，每个tick都会产生一个WM_TIMER消息。
　　可见，计时器并不能严格按照指定的时间间隔发送WM_TIMER消息，它总要相差那么几毫秒。
　　即使忽略这几个毫秒的差别，计时器仍然不精确。请看原因二：
　　WM_TIMER消息放在正常的消息队列之中，和其他消息排列在一起，因此，如果在SetTimer中指定间隔为1000毫秒，那么不能保证程序每1000毫秒或者989毫秒就会收到一个WM_TIMER消息。如果其他程序的执行事件超过一秒，在此期间内，您的程式将收不到任何WM_TIMER讯息。事实上， Windows对WM_TIMER消息的处理非常类似于对WM_PAINT消息的处理，这两个消息都是低优先级的，程序只有在消息队列中没有其他消息时才接收它们。
　　WM_TIMER还在另一方面和WM_PAINT相似：Windows不能持续向消息队列中放入多个WM_TIMER讯息，而是将多余的WM_TIMER消息组合成一个消息。因此，应用程序不会一次收到多个这样的消息，尽管可能在短时间内得到两个WM_TIMER消息。应用程序不能确定这种处理方式所导致的WM_TIMER消息「遗漏」的数目。
　　可见，WM_TIMER消息并不能及时被应用程序所处理，WM_TIMER在消息队列中的延误可能就不能用毫秒来计算了。
　　由以上两点，你不能通过在处理WM_TIMER时一秒一秒计数的方法来计时。如果要实现一个时钟程序，可以使用系统的时间函数如GetLocalTime ，而在时钟程序中，计时器的作用是定时调用GetLocalTime获得新的时间并刷新时钟画面，当然这个刷新的间隔要等于或小于1秒。
本文转自:http://blog.csdn.net/n5/article/details/14271
