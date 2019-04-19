# Live555  流媒体服务器启动 - 建建的博客 - CSDN博客
2017年01月18日 21:57:38[纪建](https://me.csdn.net/u013898698)阅读数：196
个人分类：[Live555](https://blog.csdn.net/u013898698/article/category/6686402)
TaskScheduler* scheduler = BasicTaskScheduler::createNew();  创建BasicTaskScheduler对象
BasicTaskScheduler::createNew(unsigned maxSchedulerGranularity)   参数maxSchedulerGranularity的含义是指定我们默认等待时间
返回事件循环以处理非套接字或非基于时间的事件或设置为0，以指定没有这样的最高时间
BasicTaskScheduler 的构造函数
  FD_ZERO(&fReadSet);
  FD_ZERO(&fWriteSet);
  FD_ZERO(&fExceptionSet);
清空了
读集合
写集合
异常集合
 if (maxSchedulerGranularity > 0)
 schedulerTickTask(); // ensures that we handle events frequently
如果如果默认等待时间不是为0 
则调用
schedulerTickTask
调用
scheduleDelayedTask(fMaxSchedulerGranularity, schedulerTickTask, this);
参数最大等待时间，schedulerTickTask，BasicTaskScheduler
  if (microseconds < 0) microseconds = 0;    //
  DelayInterval timeToDelay((long)(microseconds/1000000), (long)(microseconds%1000000));
  AlarmHandler* alarmHandler = new AlarmHandler(proc, clientData, timeToDelay);//将schedulerTickTask和BasicTaskScheduler传入到AlarmHandler
  fDelayQueue.addEntry(alarmHandler);//添加进延迟队列
  return (void*)(alarmHandler->token());
UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);  创建上下文的，打印，输出日志的作用
 portNumBits rtspServerPortNum = 554;
rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);  创建rtsp服务器
  int ourSocket = setUpOurSocket(env, ourPort); //建立TCP socket   
建立socket，基于tcp，主要是为了处理rtsp请求的
 do {
    // The following statement is enabled by default.
    // Don't disable it (by defining ALLOW_RTSP_SERVER_PORT_REUSE) unless you know what you're doing.
#ifndef ALLOW_RTSP_SERVER_PORT_REUSE
    NoReuse dummy(env); // Don't use this socket if there's already a local server using it
#endif
    ourSocket = setupStreamSocket(env, ourPort);
    if (ourSocket < 0) break;
    //设置socket的发送缓存大小，
    // Make sure we have a big send buffer:
    if (!increaseSendBufferTo(env, ourSocket, 50*1024)) break;
    // Allow multiple simultaneous connections:
    if (listen(ourSocket, LISTEN_BACKLOG_SIZE) < 0) {
      env.setResultErrMsg("listen() failed: ");
      break;
    }
    if (ourPort.num() == 0) {
      // bind() will have chosen a port for us; return it also:
      if (!getSourcePort(env, ourSocket, ourPort)) break;
    }
    return ourSocket;
  } while (0);
绑定端口号
setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR,
(const char*)&reuseFlag, sizeof reuseFlag)
设置socket
f (listen(ourSocket, LISTEN_BACKLOG_SIZE) < 0) {
      env.setResultErrMsg("listen() failed: ");
      break;
    }
绑定端口号

  if (ourSocket == -1) return NULL;
  return new DynamicRTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
端口是554
DynamicRTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
env->taskScheduler().doEventLoop(); // does not return  启动事件循环


