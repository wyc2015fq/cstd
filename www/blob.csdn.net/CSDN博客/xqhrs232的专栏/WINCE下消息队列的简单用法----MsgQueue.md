# WINCE下消息队列的简单用法----MsgQueue - xqhrs232的专栏 - CSDN博客
2009年09月29日 20:05:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2847
相关网帖
1、**在WINCE下的点对点消息队列CreateMsgQueue----[http://www.360doc.com/content/12/0115/13/992979_179513334.shtml](http://www.360doc.com/content/12/0115/13/992979_179513334.shtml)**
2、[WinCE下消息队列用法](http://blog.csdn.net/lan120576664/article/details/4576743)----[http://blog.csdn.net/lan120576664/article/details/4576743](http://blog.csdn.net/lan120576664/article/details/4576743)
//============================================================================
HANDLE   hReadMsgQueue;
HANDLE   hWriteMsgQueue;
const TCHAR szevtWriteComMsgQueue[] = TEXT("WriteComMsgQueue");  //写消息队列
const TCHAR szevtReadComMsgQueue[] = TEXT("ReadComMsgQueue");   //读消息对列
//==============================================================================================================================================================
void  InitCommMsgQueue(void) //2009--9--29  xqh  初始化系统用到的消息队列
{
       MSGQUEUEOPTIONS  options  =  {0}; 
      options.dwSize  =  sizeof(MSGQUEUEOPTIONS);   
      options.dwFlags  =  0;   
      //options.dwMaxMessages  =  QUEUE_ENTRIES;   
      //options.cbMaxMessage  =  sizeof(POWER_BROADCAST)+MAX_NAMELEN;   
 options.dwMaxMessages=1;   
 options.cbMaxMessage=10;   
      options.bReadAccess=TRUE;   
     //options.bReadAccess=FALSE;
      hReadMsgQueue=CreateMsgQueue(szevtWriteComMsgQueue,&options);  //2009--9--29  XQH  读写的消息句柄是不相同的！！！
   if(hReadMsgQueue==NULL)
    RETAILMSG(1,(TEXT("[BSP]++++1111::InitCommMsgQueue(  )----in failure/r/n")));
   else
    RETAILMSG(1,(TEXT("[BSP]++++1111::InitCommMsgQueue(  )----in success/r/n")));
  RETAILMSG(1,(TEXT("[BSP]++++1111-1::InitCommMsgQueue(  )----the hReadMsgQueue is 0x%x/r/n"),hReadMsgQueue));
//--------------------------------------------------------------------------------
    //MSGQUEUEOPTIONS  options  =  {0}; 
      options.dwSize  =  sizeof(MSGQUEUEOPTIONS);   
      options.dwFlags  =  0;   
      //options.dwMaxMessages  =  QUEUE_ENTRIES;   
      //options.cbMaxMessage  =  sizeof(POWER_BROADCAST)+MAX_NAMELEN;   
 options.dwMaxMessages=1;   
 options.cbMaxMessage=10;   
     // options.bReadAccess=TRUE;   
     options.bReadAccess=FALSE;
   hWriteMsgQueue=CreateMsgQueue(szevtReadComMsgQueue,&options);//2009--9--29  XQH  读写的消息句柄是不相同的！！！
      if(hWriteMsgQueue==NULL)
          RETAILMSG(1,(TEXT("[BSP]++++2222::InitCommMsgQueue(  )----in failure/r/n")));
   else
    RETAILMSG(1,(TEXT("[BSP]++++2222::InitCommMsgQueue(  )----in success/r/n")));
 RETAILMSG(1,(TEXT("[BSP]++++2222-1::InitCommMsgQueue(  )----the hWriteMsgQueue is 0x%x/r/n"),hWriteMsgQueue));
}
void   WriteMsgQueue(void)
{
int  i;
BYTE   tempByte[10];
for(i=0;i<10;i++)
{
   tempByte[i]=0x30+i;
}
DWORD   dwTimeout=INFINITE;
//DWORD   dwFlags=MSGQUEUE_MSGALERT ;  //警告消息
DWORD   dwFlags=2;
BOOL   bRet;
if(hReadMsgQueue==NULL)
   RETAILMSG(1,(TEXT("[BSP]++++WriteMsgQueue(  )----the hReadMsgQueue is NULL/r/n")));
if(hWriteMsgQueue==NULL)
   RETAILMSG(1,(TEXT("[BSP]++++WriteMsgQueue(  )----the hWriteMsgQueue is NULL/r/n")));
 // bRet=WriteMsgQueue(hReadMsgQueue, tempByte,10,dwTimeout,dwFlags);
 bRet=WriteMsgQueue(hWriteMsgQueue, tempByte,10,dwTimeout,dwFlags);
DWORD  dwError;
dwError=GetLastError( );  //句柄无效？？？
  RETAILMSG(1,(TEXT("[BSP]++++WriteMsgQueue(  )----the dwError is 0x%x/r/n"),dwError));
  if(bRet)
     RETAILMSG(1,(TEXT("[BSP]++++WriteMsgQueue(  )----in success/r/n")));
  else
     RETAILMSG(1,(TEXT("[BSP]++++WriteMsgQueue(  )----in failure/r/n")));
}
DWORD WaitMsgQueueThread(LPVOID lparam)
{
int i;
unsigned long nRead = 0, flags = 0, res = 0; 
BYTE   ByteBuf[10];
 RETAILMSG(1,(TEXT("[BSP]++++WaitMsgQueueThread(  )----Running/r/n")));
  while(1)
  {
    DWORD dwRes = WaitForSingleObject(hReadMsgQueue,INFINITE); 
    //  DWORD dwRes = WaitForSingleObject(hWriteMsgQueue,INFINITE); 
     if(dwRes==WAIT_OBJECT_0) 
     { 
      RETAILMSG(1,(TEXT("[BSP]++++WaitMsgQueueThread(  )----/r/n")));
      memset(&ByteBuf,  0,  10); 
      if (ReadMsgQueue(hReadMsgQueue, &ByteBuf, 10, &nRead, INFINITE, &flags)) 
     { 
        for(i=0;i<nRead;i++)
  {
          RETAILMSG(1,(TEXT("[BSP]++++WaitMsgQueueThread(  )----the index is %d,the data is 0x%x/r/n"),i,ByteBuf[i]));
  }
   }
  }// if(dwRes==WAIT_OBJECT_0) 
  }//while(1)
  return  0;
}
//=========================================================================================================
备注::
1>忘了说明一点，上面的代码只是在BSP里面的代码，要让其成功跑起来还要结合上层的应用程序；在上层的应用程序里面去对应打开读/写的消息队列
2>驱动层的读队列只能读，驱动层的写队列只能写；应用层的读队列只能读，应用层的读队列只能读。驱动层的读消息队列对应应用层的写消息队列，驱动层的写消息队列对应应用层的读消息队列，他们使用同样的名称字符串！！！
3>应用层跟驱动层去创建同一个名称的消息队列，确保了消息队列的一致性。应用层跟驱动层对同一个消息队列的访问只能有一种属性，要不单读，要不单写，不能同时有可读可写的属性！！！

