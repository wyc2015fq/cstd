# 使用setsockopt（）接口，设置TCP的接收与发送超时,Invalid argument错误问题 - maopig的专栏 - CSDN博客
2013年04月21日 11:59:52[maopig](https://me.csdn.net/maopig)阅读数：2649
**使用TCP套接字时，当无网络连接时，还会继续send，继续recv阻塞，知道TCP自己协议机制判断断开连接时才会停止发送和接收，时间需要几分钟之久。**
**解决的办法是，自己设置接收超时时间，当超时后重新发送，或者从新初始化socket然后在重新创建socket重新connect**。
代码如下：  
      int iSendTimeout, iRecvTimeOut;
        iClientSock = socket(AF_INET, SOCK_STREAM, 0);
        if(iClientSock < 0)
        {
                printf("[%s:%d]client init error\r\n", __FILE__, __LINE__);
                return ERROR;
        }
        iRecvTimeOut = 3000;        
        iRet1 = setsockopt(iClientSock, SOL_SOCKET, SO_RCVTIMEO, &iRecvTimeOut, sizeof(int));
        printf("%s\r\n", strerror(errno));
        iSendTimeout = 3000;        
        iRet2 = setsockopt(iClientSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&iSendTimeout, sizeof(int));
        printf("%s\r\n", strerror(errno));
但是编译会出错，perror显示参数错误([Invalid argument](http://bbs.chinaunix.net/thread-932833-1-1.html)),看了下内核中的实现，参数确实错了，其中最后一个参数是sizeof（struct
 timeval） 类型的，而非其他类型
改正为：
struct
 timeval tv;    //声明一个struct timeval 类型的变量
然后最后个参数写为:
iRet2=setsockopt(iClientSock, SOL_SOCKET, SO_SNDTIMEO, &iSendTimeout, sizeof(tv));
参数解析:
 int setsockopt(int
 s, int level, int optname,const void *optval, socklen_t optlen);
1)int s :sockfd,即socket文件描述符
2)int level :SOL_SOCKET(通用套接字)
 ,IPPROTO_TCP,等
3)int optname :SO_SNDTIMEO(发送超时时间)|SO_RSVTIMEO(接收超时时间)等,设置选项,
4) const
 void *optval :自定义的超时时间的地址
5) struct
 timeval的大小,即sizeof(struct timecal)
