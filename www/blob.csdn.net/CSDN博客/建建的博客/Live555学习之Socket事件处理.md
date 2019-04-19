# Live555学习之Socket事件处理 - 建建的博客 - CSDN博客
2017年02月21日 09:27:12[纪建](https://me.csdn.net/u013898698)阅读数：573
live555流媒体服务器初始化
- 
RTSPServer* rtspServer;
- 
portNumBits rtspServerPortNum = 554;
- 
//创建rtsp服务器，设置端口号和传入环境
- rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
DynamicRTSPServer* DynamicRTSPServer::createNew(UsageEnvironment& env, Port ourPort,UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
{
  //创建服务端socket
  int ourSocket = setUpOurSocket(env, ourPort);
  //
  if (ourSocket == -1) return NULL;
  //
  return new DynamicRTSPServer(env, ourSocket, ourPort, authDatabase,
  reclamationTestSeconds);
 }
- 
DynamicRTSPServer::DynamicRTSPServer(UsageEnvironment& env, int ourSocket,
- 
Port ourPort,
- 
  UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
- : RTSPServerSupportingHTTPStreaming(env, ourSocket, ourPort, authDatabase,
-   reclamationTestSeconds);
- 
- 
RTSPServerSupportingHTTPStreaming
- 
::RTSPServerSupportingHTTPStreaming(UsageEnvironment& env, int ourSocket, Port
- 
  rtspPort,
- 
UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
- : RTSPServer(env, ourSocket, rtspPort, authDatabase, reclamationTestSeconds) {
- }
- 
RTSPServer::RTSPServer(UsageEnvironment& env,int ourSocket, Port ourPort,
- UserAuthenticationDatabase* authDatabase,
- unsigned reclamationSeconds)
-    : GenericMediaServer(env, ourSocket, ourPort, reclamationSeconds),
-     fHTTPServerSocket(-1), fHTTPServerPort(0),
-     fClientConnectionsForHTTPTunneling(NULL), // will get created if needed
-     fTCPStreamingDatabase(HashTable::create(ONE_WORD_HASH_KEYS)),
-     fPendingRegisterRequests(HashTable::create(ONE_WORD_HASH_KEYS)),
-     fRegisterRequestCounter(0),
-     fAuthDB(authDatabase), fAllowStreamingRTPOverTCP(True) {
-     }
- 
GenericMediaServer
- 
::GenericMediaServer(UsageEnvironment& env, int ourSocket, Port ourPort,
- 
: Medium(env),
- 
- fServerSocket(ourSocket),fServerPort(ourPort),fReclamationSeconds(reclamationSeconds),
- fServerMediaSessions(HashTable::create(STRING_HASH_KEYS)),
- fClientConnections(HashTable::create(ONE_WORD_HASH_KEYS)),
- fClientSessions(HashTable::create(STRING_HASH_KEYS))
-  {
- ignoreSigPipeOnSocket(fServerSocket); // so that clients on the same host that are 
- 
- env.taskScheduler().turnOnBackgroundReadHandling(fServerSocket,
- incomingConnectionHandler, this);
- }
- 
void BasicTaskScheduler
  ::setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData) {
  if (socketNum < 0) return;
#if !defined(__WIN32__) && !defined(_WIN32) && defined(FD_SETSIZE)
  if (socketNum >= (int)(FD_SETSIZE)) return;
#endif
  FD_CLR((unsigned)socketNum, &fReadSet);
  FD_CLR((unsigned)socketNum, &fWriteSet);
  FD_CLR((unsigned)socketNum, &fExceptionSet);
  if (conditionSet == 0) {
    fHandlers->clearHandler(socketNum);
    if (socketNum+1 == fMaxNumSockets) {
      --fMaxNumSockets;
    }
  } else {
- 
    //将server端接受连接的socket加入fHandlers链表，并绑定处理函数incomingConnectionHandler
    fHandlers->assignHandler(socketNum, conditionSet, handlerProc, clientData);
    if (socketNum+1 > fMaxNumSockets) {
      fMaxNumSockets = socketNum+1;
    }
    if (conditionSet&SOCKET_READABLE) FD_SET((unsigned)socketNum, &fReadSet);
    if (conditionSet&SOCKET_WRITABLE) FD_SET((unsigned)socketNum, &fWriteSet);
    if (conditionSet&SOCKET_EXCEPTION) FD_SET((unsigned)socketNum, &fExceptionSet);
  }
}
- 
void GenericMediaServer::incomingConnectionHandler(void* instance, int /*mask*/) {
- 
 GenericMediaServer* server = (GenericMediaServer*)instance;
- 
//处理连接请求的 
- 
server->incomingConnectionHandler();
- }
- 
        //处理连接请求代码，创建新的连接请求会话，创建会话后会申请内存，然后将新创建的socket加入fHanlers
- 
void GenericMediaServer::incomingConnectionHandlerOnSocket(int serverSocket) {
- 
 struct sockaddr_in clientAddr;
- 
SOCKLEN_T clientAddrLen = sizeof clientAddr;
- 
 int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr,
- 
 &clientAddrLen);
- if (clientSocket < 0) {
- int err = envir().getErrno();
-  if (err != EWOULDBLOCK) {
-     envir().setResultErrMsg("accept() failed: ");
-  }
-  return;
-  }
-  ignoreSigPipeOnSocket(clientSocket); // so that clients on the same host that are 
-  makeSocketNonBlocking(clientSocket);
-  increaseSendBufferTo(envir(), clientSocket, 50*1024);
-  (void)createNewClientConnection(clientSocket, clientAddr);
- 
           //检查fHandlers所有的socket  是否有Io时间和触发事件以及延迟事件
- void BasicTaskScheduler::SingleStep(unsigned maxDelayTime) {  
-     fd_set readSet = fReadSet;  
-     fd_set writeSet = fWriteSet;  
-     fd_set exceptionSet = fExceptionSet;  
- 
-     DelayInterval const& timeToDelay = fDelayQueue.timeToNextAlarm();  
- struct timeval tv_timeToDelay;  
- 
-     tv_timeToDelay.tv_sec = timeToDelay.seconds();  
-     tv_timeToDelay.tv_usec = timeToDelay.useconds();  
- 
- /**
-      * 一个太大的tv_sec将导致select函数失败
-      * 因此,确保它不大于一万秒，即11.5天
-      */
- constlong MAX_TV_SEC = MILLION;  
- 
- if (tv_timeToDelay.tv_sec > MAX_TV_SEC) {  
-         tv_timeToDelay.tv_sec = MAX_TV_SEC;  
-     }  
- 
- /**
-      * 检查最大延迟时间是否大于一万秒
-      * 以及微秒数是否大于一百万微秒，即１秒
-      */
- if (maxDelayTime > 0 && (tv_timeToDelay.tv_sec > (long) maxDelayTime / MILLION  
-             || (tv_timeToDelay.tv_sec == (long) maxDelayTime / MILLION  
-                     && tv_timeToDelay.tv_usec > (long) maxDelayTime % MILLION))) {  
-         tv_timeToDelay.tv_sec = maxDelayTime / MILLION;  
-         tv_timeToDelay.tv_usec = maxDelayTime % MILLION;  
-     }  
- 
- int selectResult = select(fMaxNumSockets, &readSet, &writeSet,  
-             &exceptionSet, &tv_timeToDelay);  
- if (selectResult < 0) {  
- if (errno != EINTR && errno != EAGAIN) {  
- /**
-              * 哭了，错误
-              */
-             internalError();  
-         }  
-     }  
- 
- /**
-      * 迭代器
-      */
-     HandlerIterator iter(*fHandlers);  
-     HandlerDescriptor* handler;  
- 
- /**
-      * 找到上次执行后的处理程序队列中的下一个
-      * 这里先找到上次执行时的socket号
-      */
- if (fLastHandledSocketNum >= 0) {  
- while ((handler = iter.next()) != NULL) {  
- if (handler->socketNum == fLastHandledSocketNum)  
- break;  
-         }  
- /**
-          * 没有找到，可能已经被移除，重置延时队列
-          */
- if (handler == NULL) {  
-             fLastHandledSocketNum = -1;  
-             iter.reset();  
-         }  
-     }  
- 
- /**
-      * 从找到的handler开始，执行其下一个，不管其状态是什么，皆执行
-      * 当然，也可能是从队列头开始执行的
-      */
- while ((handler = iter.next()) != NULL) {  
- int sock = handler->socketNum;  
- int resultConditionSet = 0;  
- 
- /**
-          * 检查
-          */
- if (FD_ISSET(sock, &readSet) && FD_ISSET(sock, &fReadSet))  
-             resultConditionSet |= SOCKET_READABLE;  
- 
- if (FD_ISSET(sock, &writeSet) && FD_ISSET(sock, &fWriteSet))  
-             resultConditionSet |= SOCKET_WRITABLE;  
- 
- if (FD_ISSET(sock, &exceptionSet) && FD_ISSET(sock, &fExceptionSet))  
-             resultConditionSet |= SOCKET_EXCEPTION;  
- 
- if ((resultConditionSet & handler->conditionSet) != 0  
-                 && handler->handlerProc != NULL) {  
- /**
-              * 保存sock号，调度程序下次将从该位置继续执行，下同
-              */
-             fLastHandledSocketNum = sock;  
- 
- /**
-              * 调用事件处理函数
-              */
-             (*handler->handlerProc)(handler->clientData, resultConditionSet);  
- break;  
-         }  
-     }  
- 
- /**
-      * 我们没有调用处理程序
-      * 因此，从重再来一次
-      * 造成这样的原因可能是从上一次执行处理程序的位置向后没有找到任何可执行的处理程序了
-      * 于是从头开始寻找处理程序
-      */
- if (handler == NULL && fLastHandledSocketNum >= 0) {  
-         iter.reset();  
- 
- while ((handler = iter.next()) != NULL) {  
- int sock = handler->socketNum;  
- int resultConditionSet = 0;  
- 
- if (FD_ISSET(sock, &readSet) && FD_ISSET(sock, &fReadSet))  
-                 resultConditionSet |= SOCKET_READABLE;  
- 
- if (FD_ISSET(sock, &writeSet) && FD_ISSET(sock, &fWriteSet))  
-                 resultConditionSet |= SOCKET_WRITABLE;  
- 
- if (FD_ISSET(sock, &exceptionSet) && FD_ISSET(sock, &fExceptionSet))  
-                 resultConditionSet |= SOCKET_EXCEPTION;  
- 
- if ((resultConditionSet & handler->conditionSet) != 0  
-                     && handler->handlerProc != NULL) {  
-                 fLastHandledSocketNum = sock;  
- 
-                 (*handler->handlerProc)(handler->clientData, resultConditionSet);  
- break;  
-             }  
-         }  
- 
- /**
-          * 依然没有找到任务何执行的handler
-          * 将其值置为-1
-          * 以告诉处理程序，下次应该从头开始寻找handler
-          */
- if (handler == NULL)  
-             fLastHandledSocketNum = -1;  
-     }  
- 
- /**
-      * 响应新触发的事件
-      */
- if (fTriggersAwaitingHandling != 0) {  
- /**
-          * 首先检查是否只有一个待触发事件
-          */
- if (fTriggersAwaitingHandling == fLastUsedTriggerMask) {  
-             fTriggersAwaitingHandling = 0;  
- if (fTriggeredEventHandlers[fLastUsedTriggerNum] != NULL) {  
- /**
-                  * 执行事件处理函数
-                  */
-                 (*fTriggeredEventHandlers[fLastUsedTriggerNum])(  
-                         fTriggeredEventClientDatas[fLastUsedTriggerNum]);  
-             }  
-         } else {  
- /**
-              * 寻找待执行的触发事件
-              */
-             unsigned i = fLastUsedTriggerNum;  
-             EventTriggerId mask = fLastUsedTriggerMask;  
- 
- do {  
-                 i = (i + 1) % MAX_NUM_EVENT_TRIGGERS;  
-                 mask >>= 1;  
- if (mask == 0)  
-                     mask = 0x80000000;  
- 
- if ((fTriggersAwaitingHandling & mask) != 0) {  
-                     fTriggersAwaitingHandling &= ~mask;  
- if (fTriggeredEventHandlers[i] != NULL) {  
- /**
-                          * 响应事件
-                          */
-                         (*fTriggeredEventHandlers[i])(  
-                                 fTriggeredEventClientDatas[i]);  
-                     }  
- 
-                     fLastUsedTriggerMask = mask;  
-                     fLastUsedTriggerNum = i;  
- break;  
-                 }  
-             } while (i != fLastUsedTriggerNum);  
-         }  
-     }  
- 
- /**
-      * 执行一个最迫切的延迟任务
-      */
-     fDelayQueue.handleAlarm();  
- }  
