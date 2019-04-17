# UDP 打洞示例 包含 服务器 客户端 - DoubleLi - 博客园






客户端示例：







**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/40785705#)[copy](http://blog.csdn.net/mtour/article/details/40785705#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include "Net.h"  
- #include "../p2pInfo.h"  
- 
- int main()  
- {  
-     CUdp  udp;  
- if (0!=udp.Open(16888))  
-     {  
-         printf("client udp open failed \n");  
- return -1;  
-     }  
- 
-     P2P_CLIENT_INFO clientInfo;  
-     memset(&clientInfo,0,sizeof(P2P_CLIENT_INFO));  
- 
-     clientInfo.nClientPort=16888;  
-     clientInfo.nID=0;  
-     sprintf(clientInfo.sClientIP,"%s","10.10.2.161");  
- 
-     P2P_CMD_HEAD cmdHead;  
- 
-     cmdHead.nCmd=P2P_CMD_REGISTER_REQ;  
-     cmdHead.nPayloadLen=sizeof(P2P_CLIENT_INFO);  
-     memcpy(cmdHead.sPayLoad,&clientInfo,sizeof(P2P_CLIENT_INFO));  
- 
- long lDestIP=0;  
-     CBSocket::ConvertStringToIP((int*)&lDestIP,"114.247.165.37");  
- int nRet=0;  
-     nRet=udp.Send((unsigned char *)&cmdHead,CMD_HEAD_LEN+cmdHead.nPayloadLen,lDestIP,P2P_SERVER_PORT);  
-     printf("send register len %d \n",nRet);  
-     nRet=udp.Recv();  
- if (nRet>0)  
-     {  
-         printf("recv data \n");  
-     }  
- else  
-     {  
-         printf("recv P2P_CMD_REGISTER_ACK time out \n");  
- return 0;  
-     }     
- 
- 
-     P2P_CLIENT_INFO remoteClientInfo;  
-     memset(&remoteClientInfo,0,sizeof(P2P_CLIENT_INFO));  
-     P2P_CMD_HEAD recvHead;  
-     memset(&recvHead,0,sizeof(P2P_CMD_HEAD));  
- 
- while (1)  
-     {  
-         cmdHead.nCmd=P2P_CMD_COMUNICATION_REQ;  
-         cmdHead.nValue=1;    // remote id  
-         cmdHead.nPayloadLen=sizeof(P2P_CLIENT_INFO);  
- 
-         nRet=udp.Send((unsigned char *)&cmdHead,CMD_HEAD_LEN+cmdHead.nPayloadLen,lDestIP,P2P_SERVER_PORT);  
-         printf("send commuication len %d \n",nRet);  
-         nRet=udp.Recv();          
- 
- if (nRet>0)  
-         {  
-             memcpy(&recvHead,udp.GetBuffer(),nRet);  
- if (0==recvHead.nValue)  
-             {             
-                 memcpy(&remoteClientInfo,recvHead.sPayLoad,sizeof(P2P_CLIENT_INFO));  
-                 printf("recv P2P_CMD_COMUNICATION_ACK remote ip:%s port:%d\n",  
-                     remoteClientInfo.sClientPublicIP,remoteClientInfo.nClientPublicPort);  
- break;  
-             }  
-             usleep(100*1000);  
-         }  
- else  
-         {  
-             printf("recv P2P_CMD_COMUNICATION_ACK time out \n");              
-         }  
-     }  
- 
- 
- 
- // send udp hole to remote ip  
-     CBSocket::ConvertStringToIP((int*)&lDestIP,remoteClientInfo.sClientPublicIP);  
-     cmdHead.nCmd=P2P_CMD_UDP_HOLE_REQ;  
-     cmdHead.nPayloadLen=0;  
-     udp.Send((unsigned char *)&cmdHead,CMD_HEAD_LEN,lDestIP,remoteClientInfo.nClientPublicPort);  
-     usleep(1000*1000);  
-     udp.Send((unsigned char *)&cmdHead,CMD_HEAD_LEN,lDestIP,remoteClientInfo.nClientPublicPort);  
-     usleep(1000*1000);  
-     udp.Send((unsigned char *)&cmdHead,CMD_HEAD_LEN,lDestIP,remoteClientInfo.nClientPublicPort);  
- 
- 
-     nRet=udp.Recv();  
- if (nRet>0)  
-     {  
-         memcpy(&recvHead,udp.GetBuffer(),nRet);  
- 
- if (recvHead.nCmd==P2P_CMD_UDP_HOLE_ACK || recvHead.nCmd==P2P_CMD_UDP_HOLE_REQ)  
-         {  
-             printf("recv P2P_CMD_UDP_HOLE  udp hole success\n");  
-         }  
-     }  
- else  
-     {  
-         printf("P2P_CMD_UDP_HOLE recv out \n");  
- return 0;  
-     }     
- 
- 
- return 0;  
- }  






服务器端示例: 





**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/40785705#)[copy](http://blog.csdn.net/mtour/article/details/40785705#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include "Net.h"  
- #include "../p2pInfo.h"  
- 
- #define MAX_CLIENT_COUNT 16  
- 
- P2P_CLIENT_INFO* pClientList[MAX_CLIENT_COUNT];  
- 
- void AddClient(P2P_CLIENT_INFO* pClientInfo)  
- {  
- for(int i=0;i<MAX_CLIENT_COUNT;i++)  
-     {  
- if (!pClientList[i])  
-         {  
-             P2P_CLIENT_INFO* pClient=new P2P_CLIENT_INFO;  
-             memcpy(pClient,pClientInfo,sizeof(P2P_CLIENT_INFO));  
-             pClientList[i]=pClient;  
- return;  
-         }  
-     }  
- }  
- 
- P2P_CLIENT_INFO* FindClient(int nID)  
- {  
- for(int i=0;i<MAX_CLIENT_COUNT;i++)  
-     {  
- if (pClientList[i])  
-         {  
- if (nID==pClientList[i]->nID)  
-             {  
- return pClientList[i];  
-             }  
-         }  
-     }  
- return NULL;  
- }  
- 
- P2P_CLIENT_INFO* FindClient(long ip, unsigned short nPort)  
- {  
- for(int i=0;i<MAX_CLIENT_COUNT;i++)  
-     {  
- if (pClientList[i])  
-         {  
- int nIp=0;  
-             CBSocket::ConvertStringToIP(&nIp,pClientList[i]->sClientPublicIP);  
- if (nPort==pClientList[i]->nClientPublicPort && ip==nIp)  
-             {  
- return pClientList[i];  
-             }  
-         }  
-     }  
- return NULL;  
- }  
- 
- 
- int main()  
- {  
-     CUdp udp;  
-     udp.Open(P2P_SERVER_PORT);  
- int nRet=0;  
-     P2P_CMD_HEAD* pCmdHead=new P2P_CMD_HEAD;  
-     P2P_CMD_HEAD* pSendCmdHead=new P2P_CMD_HEAD;  
-     memset(pCmdHead,0,sizeof(P2P_CMD_HEAD));  
-     memset(pSendCmdHead,0,sizeof(P2P_CMD_HEAD));  
- 
- long lRemoteIP=0;  
-     unsigned short nRemotePort=0;  
- 
- 
- int i=0;  
- for (i = 0; i <16; i++)  
-     {  
-         pClientList[i]=NULL;  
-     }  
- 
- 
- while (1)  
-     {  
-         nRet=udp.Recv(lRemoteIP,nRemotePort);  
- if (nRet>0)  
-         {  
-             memcpy(pCmdHead,udp.GetBuffer(),nRet);  
- 
- switch (pCmdHead->nCmd)  
-             {  
- case P2P_CMD_REGISTER_REQ:  
-                     {  
-                         printf("recv register req \n");  
-                         P2P_CLIENT_INFO* pClient=(P2P_CLIENT_INFO*)pCmdHead->sPayLoad;  
-                         CBSocket::ConvertIPToString(lRemoteIP,pClient->sClientPublicIP);  
-                         pClient->nClientPublicPort=nRemotePort;  
-                         AddClient(pClient);  
- 
-                         printf("recv command P2P_CMD_REGISTER_REQ from ip:%s port:%d\n",pClient->sClientPublicIP,pClient->nClientPublicPort);  
- 
-                         pSendCmdHead->nCmd=P2P_CMD_REGISTER_ACK;  
-                         pSendCmdHead->nPayloadLen=sizeof(P2P_CLIENT_INFO);  
-                         memcpy(pSendCmdHead->sPayLoad,pClient,sizeof(P2P_CLIENT_INFO));  
-                         udp.Send((unsigned char *)pSendCmdHead,CMD_HEAD_LEN+pSendCmdHead->nPayloadLen,lRemoteIP,nRemotePort);  
-                     }  
- break;  
- case P2P_CMD_COMUNICATION_REQ:  
-                     {  
-                         printf("recv command P2P_CMD_COMUNICATION_REQ\n");  
-                         P2P_CLIENT_INFO* pTargetClientInfo=FindClient(pCmdHead->nValue);  
- 
- if (!pTargetClientInfo)  
-                         {  
-                             printf("not find client info id:%d\n",pCmdHead->nValue);  
- continue;  
-                         }  
- 
-                         pSendCmdHead->nCmd=P2P_CMD_COMUNICATION_ACK;  
-                         pSendCmdHead->nPayloadLen=sizeof(P2P_CLIENT_INFO);  
-                         memcpy(pSendCmdHead->sPayLoad,pTargetClientInfo,sizeof(P2P_CLIENT_INFO));  
-                         udp.Send((unsigned char *)pSendCmdHead,CMD_HEAD_LEN+pSendCmdHead->nPayloadLen,lRemoteIP,nRemotePort);                          
- 
- 
- 
- // send to remote client  
- int nIP=0;  
-                         P2P_CLIENT_INFO* pSelfClientInfo=FindClient(lRemoteIP,nRemotePort);  
- if (pSelfClientInfo)  
-                         {  
-                             printf("find self client info ip:%s\n",pSelfClientInfo->sClientPublicIP);  
-                             memcpy(pSendCmdHead->sPayLoad,pSelfClientInfo,sizeof(P2P_CLIENT_INFO));  
-                             CBSocket::ConvertStringToIP(&nIP,pTargetClientInfo->sClientPublicIP);  
-                             udp.Send((unsigned char *)pSendCmdHead,CMD_HEAD_LEN+pSendCmdHead->nPayloadLen,nIP,pTargetClientInfo->nClientPublicPort);  
-                         }         
- else  
-                         {  
-                             printf("not find self client info");  
-                         }  
-                     }  
- break;  
- default:  
- break;  
-             }  
-         }  
- else  
-         {  
-             printf("udp recv time out \n");  
-         }  
-     }  
- return 0;  
- }  








示例下载地址:    [http://download.csdn.net/detail/mtour/8119489](http://download.csdn.net/detail/mtour/8119489)









