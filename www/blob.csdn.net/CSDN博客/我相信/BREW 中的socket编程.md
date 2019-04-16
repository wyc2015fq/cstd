# BREW 中的socket编程 - 我相信...... - CSDN博客





2010年11月04日 11:37:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1754








对基于UDP的网络通信而言，首先需要创建ISocket接口，

ISocket *mySocket;

mySocket = INETMGR_OpenSocket(pMe->pINetMgr, AEE_SOCK_STREAM);

if(!mySocket) {

                   DBGPRINTF(“Error value: %x”, INETMGR_GetLastError(pMe->pINetMgr));

}

然后，就可以发送数据包了

Void SendPacket(App* pMe){

    retVal = ISOCKET_SendTo(pMe->pISocket, (byte*)"HelloWorld",

                                                   sizeof("HelloWorld"), 0, pMe->inAddr, HTONS(pMe->port));

      switch(retVal) {

         case AEE_NET_WOULDBLOCK:

              ISOCKET_Writeable(pMe->pISocket, (PFNNOTIFY)SendPacket, pMe);

              return;

          // Some sort of network error

         case AEE_NET_ERROR:

              DBGPRINTF("Network Error");

              break;

         default:

             if(retVal == sizeof(“HelloWorld”))

                 //Success

             else

                 // Some other error

      }

}

在接收数据之前，先要绑定客户端的端口。

static void SetUpNetwork(CVocApp* pMe) {

   int retVal;

   // Bind to the correct port and ip address

   retVal = ISOCKET_Bind(pMe->pIRecvSocket, HTONL(AEE_BREW_LOOPBACK), HTONS(PORT));

   // Check the return value of ISOCKET_Bind() and display necessary message

   switch(retVal) {

   // Socket not ready yet

   case AEE_NET_WOULDBLOCK:

      ISOCKET_Writeable(pMe->pIRecvSocket, (PFNNOTIFY)SetUpNetwork, pMe);

      break;

   // Succesfully connected

   case AEE_NET_SUCCESS:

      ISOCKET_Readable(pMe->pIRecvSocket, (PFNNOTIFY)ReadInData, pMe);

      break;

   // Something went wrong, print error to logger

   default:

      DBGPRINTF("retVal %d", ISOCKET_GetLastError(pMe->pIRecvSocket));

      break;

   }

}

然后，就可以通过UDP接收数据了，

static void ReadInData(App* pMe) {

   int ret;

   // When data is ready, receive data

   ret = ISOCKET_RecvFrom(pMe->pIRecvSocket, (void*)&pMe->indata, sizeof(pMe->indata), ISOCKET_FLAG_SENDTO_WAKEUP|ISOCKET_FLAG_SENDTO_URGENT, NULL, NULL);

   // Data integrity checks

   if(ret < 0) {

       DBGPRINTF("error %d", ISOCKET_GetLastError(pMe->pIRecvSocket));

   }

   // Have socket continue to listen

   ISOCKET_Readable(pMe->pIRecvSocket, (PFNNOTIFY)ReadInData, pMe);

}

通信完毕，需要使用ISOCKET_Release()来释放接口对象。

            对基于TCP的网络通信而言，同样，首先需要创建ISocket接口，

                        ISocket *mySocket;

                                mySocket = INETMGR_OpenSocket(pMe->pINetMgr, AEE_SOCK_STREAM);

                                if(!mySocket) {

                                   DBGPRINTF(“Error value: %x”, INETMGR_GetLastError(pMe->pINetMgr));

                                }

然后，建立数据连接，并监测连接的网络状态，如果连接成功则发送数据

ISOCKET_Connect(pMe->pISocket,HTONL(myIP),HTONS(myPort),(PFNCONNECTCB)ConnectionMade, pMe);

                static void ConnectionMade(myApp* pMe, int error) {

         //Check error code

         switch(error) {

         case AEE_NET_ETIMEDOUT:

             // Connection timed out

             break;

          case AEE_NET_SUCCESS:

             // Send Some data

if(AEE_NET_WOULDBLOCK==ISOCKET_Write(pMe->pISocket,(byte*)"helloworld", sizeof("helloworld")) {

                ISOCKET_Writeable(pMe->pISocket, (PFNNOTIFY) SendDataCB, pMe);

             }

             break;

          default:

              // Some other Network error

              break;

          }



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914099.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914099.aspx)



