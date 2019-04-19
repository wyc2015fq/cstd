# WinCE虚拟串口驱动(一) - xqhrs232的专栏 - CSDN博客
2012年06月06日 22:20:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：733标签：[wince																[null																[多线程																[thread																[each](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://blog.csdn.net/norains/article/details/4032257](http://blog.csdn.net/norains/article/details/4032257)
//========================================================================
  //TITLE:
  //    WinCE虚拟串口驱动(一)
  //AUTHOR:
  //    norains
  //DATE:
  //    Saturday 28-March-2009
  //Environment:
  //    WINDOWS CE 5.0
  //========================================================================
  用过串口进行开发的朋友应该都知道，串口驱动是一个典型的独占设备。简单点来说，就是在成功地调用CreateFile打开串口之后，没有通过CloseHandle进行关闭，是无论如何都不能再次调用CreateFile来再次打开相同的串口。
  有的朋友可能会觉得莫名奇妙，为什么微软要在这上面做限制呢？但其实从另一个角度来讲，微软这么做是非常有道理的。以接收数据为例子，在驱动里面会有一定的缓存，用来保留一定量的数据。当通过ReadFile来获取数据时，驱动就会将缓存给清空，然后再继续接收数据。如果串口不是独占设备，可以多次打开，那么在读取数据上面就会有问题：应该什么时候才清空缓存？比方说，其中一个线程通过ReadFile来获得了数据，那么驱动应不应该将缓冲清空？如果清空，那另一个线程也想获得同样的数据进行分析，那就会产生数据丢失；如果不清空，万一之前已经通过ReadFile获取数据的线程再次进行读取，那么它将会得到同样重复的数据。如果想要在这多个进程中维持数据的同步，肯定要额外增加相应的标识，但这样就会加大了驱动的复杂度，并且也无法和别的驱动保持一致。因此，微软对串口实行独占设备的策略，是非常正确的。
  但，正确并不代表放之四海而皆准，在某些特殊的情况下，我们还是需要非独占性质的串口。简单地举个例子，在手持PND GPS设备中，导航软件肯定是必须要能通过串口进行数据获取来定位；可另一方面，我的另一个应用程序又想获得GPS数据进行系统时间的校准。在这情形之下，我们就必须使用一个非独占性质的串口设备。
  为了简化设计，该串口设备的驱动我们约定如下：
  1.同一时间只能有一个进程对外输出数据，其余进程只能在该进程输出完毕之后才能进行。
  2.程序不应该主动调用ReadFile来轮询获取数据。而是通过WaitCommEvent进行检测，当返回的状态中具备EV_RXCHAR时才调用ReadFile。并且该调用必须在一定的时间间隔之内，而且为了不丢失数据，缓冲大小一定要等于或大于READ_BUFFER_LENGTH。
  之所以有如上约束，完全是出于设计简便考虑。
  非独占式串口驱动主要是处理数据的分发，可以和具体的硬件分开，换句话说，该驱动是基于原有的串口驱动之上，实际上并“没有”该设备，因此我们将该非独占式串口称之为“虚拟串口驱动”。这样设计的优势很明显，可以不用理会具体的硬件规格，只要采用的是WinCE系统，并且原来已经具备了完善的串口驱动，那么该虚拟串口驱动就能工作正常。
  接下来我们来看看该虚拟串口的具体实现。
  麻雀虽小，五官俱全，虽然说该驱动是“虚拟”的，但毕竟还是“驱动”，该有的部分我们还是要具备的。
  驱动的前缀为VSP，取自于Virtual Serial Port之意。
  该驱动必须实现如下函数：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- VSP_Close  
- VSP_Deinit  
- VSP_Init  
- VSP_IOControl  
- VSP_Open  
- VSP_PowerDown  
- VSP_PowerUp  
- VSP_Read  
- VSP_Seek  
- VSP_Write         
  因为串口驱动是流设备，又和具体的电源管理五官，故VSP_Seek，VSP_PowerDown，VSP_PowerUp这些函数可以不用处理，直接返回即可。
  现在来看一下VSP_Open函数。
  VSP_Open函数我们大致需要如下流程处理事情：
  1.判断当前的是否已经打开串口，如果已经打开，直接跳到4.
  2.获取需要打开的串口序号，并打开该串口。如果打开失败，直接跳到5.
  3.打开数据监视进程(注：该部分在数据读取部分进行分析)。
  4.标识记数(即g_uiOpenCount)增加1。
  5.函数返回
  流程1：
  全局变量g_uiOpenCount用来保存打开的记数，所以只要判断该数值是否为0即可确定是否应该打开串口： 
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- if(g_uiOpenCount != 0)  
- {         
- goto SET_SUCCEED_FLAG;  
- }  
  流程2：
  为了让程序更具备灵活性，所打开的串口序号我们不直接在驱动中设定，而是通过读取注册表的数值获得：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- if(reg.Open(REG_ROOT_KEY,REG_DEVICE_SUB_KEY) == FALSE)  
- {  
-     RETAILMSG(TRUE,(TEXT("[VSP]:Failed to open the registry/r/n")));  
- goto LEAVE_CRITICAL_SECTION;  
- }  
- 
- //Get the MAP_PORT name 
- reg.GetValueSZ(REG_MAP_PORT_NAME,&vtBuf[0],vtBuf.size());  
  接下来便是打开具体的串口：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- g_hCom = CreateFile(&vtBuf[0],GENERIC_READ | GENERIC_WRITE ,0,NULL,OPEN_EXISTING,0,NULL);  
- if(g_hCom == INVALID_HANDLE_VALUE )  
- {  
-     RETAILMSG(TRUE,(TEXT("[VSP]Failed to map to %s/r/n"),&vtBuf[0]));  
- goto LEAVE_CRITICAL_SECTION;  
- }  
- else
- {  
-     RETAILMSG(TRUE,(TEXT("[VSP]Succeed to map to %s/r/n"),&vtBuf[0]));  
- }     
  流程3：
  创建进程来监视数据：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- InterlockedExchange(reinterpret_cast<LONG *>(&g_bExitMonitorProc),FALSE);  
- CloseHandle(CreateThread(NULL,NULL,MonitorCommEventProc,NULL,NULL,NULL));  
  流程4：
  成功打开记数
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- SET_SUCCEED_FLAG:     
-     g_uiOpenCount ++;  
-     bResult = TRUE;  
  流程5:
  函数返回：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- LEAVE_CRITICAL_SECTION:       
-     LeaveCriticalSection(&g_csOpen);      
- return bResult;   
  和VSP_Open密切对应的是VSP_Close，该函数流程基本和VSP_Open相反处理：
  1.打开记数(g_uiOpenCount)减1。如果g_uiOpenCount为不为0，跳转3。
  2.退出监视数据进程，并且关闭打开的串口。
  3.函数返回。
  流程1和流程2处理如下：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- g_uiOpenCount --;     
- if(g_uiOpenCount == 0)  
- {         
- //Notify the monitor thread to exit.    
-     InterlockedExchange(reinterpret_cast<LONG *>(&g_bExitMonitorProc),TRUE);  
- DWORD dwMask = 0;  
-     GetCommMask(g_hCom,&dwMask);  
-     SetCommMask(g_hCom,dwMask);       
- 
- while(InterlockedExchange(reinterpret_cast<LONG *>(&g_bMonitorProcRunning),TRUE) == TRUE)  
-     {  
-         Sleep(20);  
-     }  
-     InterlockedExchange(reinterpret_cast<LONG *>(&g_bMonitorProcRunning),FALSE);  
- 
-     CloseHandle(g_hCom);  
-     g_hCom = NULL;  
- }  
我们必须确保VSP_Open和VSP_Close中的某一个必须要全部处理完才能再次调用，否则在处理过程中如果又再次调用本函数或相对应的加载或卸载函数，那么一定会引发我们不可预料的情况，所以我们在这两个函数中增加了关键段，以维持处理上的同步： 
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- EnterCriticalSection(&g_csOpen);  
- ...  
- LeaveCriticalSection(&g_csOpen);  
  其余的接口，算起来最简单的是VSP_Write，只要确定同一时间只能有唯一的一个进程进行输出即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- EnterCriticalSection(&g_csWrite);  
- DWORD dwWrite = 0;  
- WriteFile(g_hCom,pBuffer,dwNumBytes,&dwWrite,NULL);  
- LeaveCriticalSection(&g_csWrite);  
    在完成VSP_Read之前，我们先来看另外一个函数：WaitCommEvent。这是串口驱动特有的，目的是有某些时间发生时，能够第一时间激活线程。该函数和驱动的MMD层有关，是MDD层的应用程序级别接口。具体串口的PDD层，WaitCommEvent函数体内也仅仅是调用了COM_IOControl接口，然后传入IOCTL_SERIAL_WAIT_ON_MASK控制码而已。也就是说，调用WaitCommEvent的代码，就相当于如此调用COM_IOControl：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- DeviceIoControl(hCom,  
-                                 IOCTL_SERIAL_WAIT_ON_MASK,  
-                                     NULL,  
-                                     0,  
-                                     pOutBuf,  
-                                     dwOutBufLen,  
-                                     &dwReturn,  
-                                     NULL);  
  换句话说，如果想让虚拟串口驱动支持WaitCommEvent函数，我们只需要在VSP_IOControl处理IOCTL_SERIAL_WAIT_ON_MASK控制码即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- BOOL VSP_IOControl(  
- DWORD dwHandle,  
- DWORD dwIoControlCode,  
- PBYTE pBufIn,  
- DWORD dwBufInSize,  
- PBYTE pBufOut,  
- DWORD dwBufOutSize,  
-    PDWORD pBytesReturned  
-    )  
- {  
-     ...  
- 
- switch(dwIoControlCode)   
-     {  
-         ...  
- 
- case IOCTL_SERIAL_WAIT_ON_MASK:  
- 
-             ...                   
- break;  
- 
-         ...  
-     }  
- }  
- 
  推而广之，像SetCommState，SetCommTimeouts等串口特有的函数，都仅仅只是对COM_IOControl函数进行的一层封装而已。
  我们再回到WaitCommEvent函数。可能有的朋友直接认为，我们只要在IOCTL_SERIAL_WAIT_ON_MASK段直接简单调用原有的WaitCommEvent即可：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- switch(dwIoControlCode)   
- {  
-     ...  
- 
- case IOCTL_SERIAL_WAIT_ON_MASK:  
-     {                 
- //直接调用原生的WaitCommEvent，但实际是错误的
- if(dwBufOutSize < sizeof(DWORD) || WaitCommEvent(g_hCom,reinterpret_cast<DWORD *>(pBufOut),NULL) == FALSE)  
-         {  
-             *pBytesReturned = 0;              
- return FALSE;  
-         }  
- else
-         {  
-             *pBytesReturned = sizeof(DWORD);  
- return TRUE;  
-         }                     
-     }  
- 
-     ...  
- }  
 但实际上这样是不行的。查看文档关于WaitCommEvent函数的描述，注意事项中有这么一条：Only one WaitCommEvent can be used for each open COM port handle. This means that if you have three threads in your application and each thread needs to wait on a specific comm event, each thread needs to open
 the COM port and then use the assigned port handle for their respective WaitCommEvent calls.
  也就是说，WaitCommEvent只能被一个线程调用。如果多线程都同时调用该函数，会发生什么情况呢？经过实际测试，如果多线程都调用相同的WaitCommEvent，那么在某个线程调用WaitCommEvent时，之前已经有其余的线程通过调用该函数进行等待状态的话，那等待的线程立马会唤醒。简单点来说，就是同一时间只能有唯一的一个线程通过WaitCommEvent函数进入等待状态。所以，对于IOCTL_SERIAL_WAIT_ON_MASK控制码，我们不能简单地调用WaitCommEvent函数。
  在这里我们采用这么一种设计，对于IOCTL_SERIAL_WAIT_ON_MASK的处理，我们是通过调用WaitForSingleObject进行线程等待。而虚拟串口驱动，会额外开放一个线程，该线程主要是通过调用WaitCommEvent来获取原生串口的状态，当状态有通知时，再发送event给等待的线程。因此，对于IOCTL_SERIAL_WAIT_ON_MASK控制码的处理可以所作如下：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- switch(dwIoControlCode)   
- {  
-     ...  
- 
- case IOCTL_SERIAL_WAIT_ON_MASK:  
-     {                 
- if(dwBufOutSize < sizeof(DWORD) ||   WaitForSingleObject(g_hEventComm,INFINITE) == WAIT_TIMEOUT)  
-                 {  
-                     *pBytesReturned = 0;              
- return FALSE;  
-                 }  
- else
-                 {  
-                     InterlockedExchange(reinterpret_cast<LONG *>(pBufOut),g_dwEvtMask);  
-                     *pBytesReturned = sizeof(DWORD);                          
- return TRUE;  
-                 }                     
-             }  
- 
-             ...  
-         }  
  驱动额外的等待线程所做如是：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- DWORD MonitorCommEventProc(LPVOID pParam)  
- {             
-     ...  
- 
- while(TRUE)  
-     {     
- DWORD dwEvtMask = 0;  
- BOOL bWaitRes = WaitCommEvent(g_hCom,&dwEvtMask,NULL);                
- 
- if(g_bExitMonitorProc != FALSE)  
-         {  
- break;  
-         }                     
- 
- if(bWaitRes == FALSE)  
-         {  
- continue;  
-         }         
- 
-         ...  
- 
-         InterlockedExchange(reinterpret_cast<LONG *>(&g_dwEvtMask),dwEvtMask);  
-         PulseEvent(g_hEventComm);         
- 
-         ...  
- 
-     }  
- 
-     ...  
- 
- return 0;  
- }  
  现在是到考虑ReadFile实现的时候了。我们需要考虑到，不同进程，在同时读取数据时，应该能获得相同的数据。但对于原生的串口驱动，如果再次调用ReadFile，所获得的数据绝对是不会和之前的一样，否则就乱套了。于是，和IOCTL_SERIAL_WAIT_ON_MASK一样，我们这么也不能粗暴简单地调用原生的ReadFile完事。
  我们转换个思维，对于“不同进程，在同时读取数据时，应该能获得相同的数据”，我们应该是这么理解：“不同进程，相当短的间隔内读取数据，应该能获得相同的数据”。如果要做到这点，我们只需要设置一个读取缓存，当上级程序想要获取数据时，我们只需要简单地将数据返回即可。那么接下来最关键的是，我们应该什么时候读取数据？什么时候该刷新缓存呢？
  分开来说，最简单的方式，就是在监视进程MonitorCommEventProc中读取数据并刷新缓存。因为该线程会调用WaitCommEvent函数进行等待，它能够充分知道什么时候有数据进来。只要有数据进来，我们就进行读取。如果之前的缓存已经被读取过，我们就清空缓存，存入新的数据；否则就在旧缓存之后添加我们新的数据。故此，完善的MonitorCommEventProc实现就应该如此：
**[cpp]**[view
 plain](http://blog.csdn.net/norains/article/details/4032257#)[copy](http://blog.csdn.net/norains/article/details/4032257#)
- DWORD MonitorCommEventProc(LPVOID pParam)  
- {  
-     InterlockedExchange(reinterpret_cast<LONG *>(&g_bMonitorProcRunning),TRUE);  
- 
-     RETAILMSG(TRUE,(TEXT("[VSP]:MonitorCommEventProc Running!/r/n")));  
- 
-     std::vector<BYTE> vtBufRead(g_vtBufRead.size(),0);          
- while(TRUE)  
-     {     
- DWORD dwEvtMask = 0;  
- BOOL bWaitRes = WaitCommEvent(g_hCom,&dwEvtMask,NULL);                
- 
- if(g_bExitMonitorProc != FALSE)  
-         {  
- break;  
-         }                     
- 
- if(bWaitRes == FALSE)  
-         {  
- continue;  
-         }         
- 
- DWORD dwRead = 0;             
- if(dwEvtMask & EV_RXCHAR)  
-         {  
-             EnterCriticalSection(&g_csRead);                      
- 
-             ReadFile(g_hCom,&g_vtBufRead[0],vtBufRead.size(),&dwRead,NULL);       
- if(dwRead == vtBufRead.size() || g_bReaded != FALSE)  
-             {  
-                 g_dwLenReadBuf = dwRead;  
-                 g_vtBufRead.swap(vtBufRead);  
-             }  
- elseif(dwRead != 0)  
-             {  
- if(g_dwLenReadBuf + dwRead <= g_vtBufRead.size())  
-                 {  
-                     g_dwLenReadBuf += dwRead;  
-                     g_vtBufRead.insert(g_vtBufRead.end(),vtBufRead.begin(),vtBufRead.begin() + dwRead);  
-                 }  
- else
-                 {  
- DWORD dwCover = g_dwLenReadBuf + dwRead - g_vtBufRead.size();  
-                     std::copy(g_vtBufRead.begin() + dwCover,g_vtBufRead.begin() + g_dwLenReadBuf,g_vtBufRead.begin());  
-                     std::copy(vtBufRead.begin(),vtBufRead.begin() + dwRead,g_vtBufRead.begin() + (g_dwLenReadBuf - dwCover));  
-                     g_dwLenReadBuf = g_vtBufRead.size();  
-                 }  
-             }  
- 
-             g_bReaded = FALSE;  
- 
-             DEBUGMSG(TRUE,(TEXT("[VSP]:Read data : %d/r/n"),dwRead));     
- 
-             LeaveCriticalSection(&g_csRead);  
-         }  
- 
- if(dwEvtMask == EV_RXCHAR && ((g_dwWaitMask & EV_RXCHAR) == 0 || dwRead == 0))  
-         {  
- //The return event mask is only EV_RXCHAR and there is not EV_RXCHAR in the wait mask.
- continue;  
-         }  
- 
-         InterlockedExchange(reinterpret_cast<LONG *>(&g_dwEvtMask),dwEvtMask);  
-         PulseEvent(g_hEventComm);         
- 
- //Sleep for other thread to respond to the event
-         Sleep(100);  
- 
-         DEBUGMSG(TRUE,(TEXT("[VSP]:PulseEvent! The event-mask is 0x%x/r/n"),dwEvtMask));      
- 
-     }  
- 
-     RETAILMSG(TRUE,(TEXT("[VSP]:Exit the MonitorCommEventProc/r/n")));    
-     InterlockedExchange(reinterpret_cast<LONG *>(&g_bMonitorProcRunning),FALSE);  
- 
- return 0;  
- }  
  正因为读取是如此实现，所以我们才有文章开头的第二点约定：
  程序不应该主动调用ReadFile来轮询获取数据。而是通过WaitCommEvent进行检测，当返回的状态中具备EV_RXCHAR时才调用ReadFile（如果一直采用ReadFile来轮询接收数据，很可能会读取重复的数据）。并且该调用必须在一定的时间间隔之内（如果间隔太久，很可能因为缓存已经刷新，数据丢失），而且为了不丢失数据，缓冲大小一定要等于或大于READ_BUFFER_LENGTH（因为只要读取一次数据，读取的标识就会被设置，当有新数据到达时，会刷新缓存，导致数据丢失）。
  这也同时解释了MonitorCommEventProc进程为何在PulseEvent之后会调用Sleep函数进行短暂的休眠，其作用主要是让驱动的读取进程歇歇，好让上级等待进程能在等待事件返回时有足够的时间来读取获得的数据。

