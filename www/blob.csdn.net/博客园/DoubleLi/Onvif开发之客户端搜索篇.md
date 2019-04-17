# Onvif开发之客户端搜索篇 - DoubleLi - 博客园






关于ONVIF的广播，有客户端搜索和服务端发现的区别：客户端向固定的网段和固定的端口发送广播消息，服务端在对应的端口回复广播请求消息
本文首先介绍客户端如何进行广播的已经对广播回复的信息的基本处理。

客户端这里处理相对服务端比较复杂一点，需要注意几个地方：


1 广播的ip和端口号(注意这个端口号和通信的端口是不一样的，通信端口号可以自己随意定一个，但是这个广播的是ONVIF协议定好公用的)

  ip:239:255:255:250   port:3702

2 MessageID ,此ID每次都需要不同，不然每次只有第一次能搜索到设备，第二次就不行了!一般情况下都是区uuid的随机值的，但是我在开发的过程是去的一个随机值加上自己设备的mac地址组合的

3 soap_send___wsdd__Probe函数，发送广播信息，通过它来判断是否发送成功，确认是否继续下一步操作进行接收消息


4 soap_recv___wsdd__ProbeMatches函数，此函数是通过广播后，设备通过onvif协议回复了设备的一些基本信息，客户端通过循 环调用此函数来接多个设备回复的各个设备的一些基本信息包括设备ip以及通信的prot



知道了这几个基本的注意点和[前篇介绍的代码生成框架](http://blog.csdn.net/max_min_go/article/details/17562045)，接下来就是敲代码了。该main函数登场了



**[cpp]**[view plain](http://blog.csdn.net/max_min_go/article/details/17591083#)[copy](http://blog.csdn.net/max_min_go/article/details/17591083#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <span style="font-size:14px;">/* 
-  * ===================================================================================== 
-  * 
-  *       Filename:  main.c 
-  *    Description:  简单例程测试:客户端通过ONVIF协议搜索前端设备, 
-  *        Created:  2013年12月26日 12时17分48秒 
-  *       Compiler:  gcc 
-  *         Author:  max_min_,  
-  * 
-  * ===================================================================================== 
-  */  
- #include "wsdd.h"  
- #include <stdio.h>  
- 
- static struct soap* ONVIF_Initsoap(struct SOAP_ENV__Header *header, const char *was_To, const char *was_Action, int timeout)  
- {  
- struct soap *soap = NULL;   
-     unsigned char macaddr[6];  
- char _HwId[1024];  
-     unsigned int Flagrand;  
-     soap = soap_new();  
- if(soap == NULL)  
-     {     
-         printf("[%d]soap = NULL\n", __LINE__);  
- return NULL;  
-     }     
-      soap_set_namespaces( soap, namespaces);  
- //超过5秒钟没有数据就退出  
- if (timeout > 0)  
-     {     
-         soap->recv_timeout = timeout;  
-         soap->send_timeout = timeout;  
-         soap->connect_timeout = timeout;  
-     }     
- else  
-     {     
- //如果外部接口没有设备默认超时时间的话，我这里给了一个默认值10s  
-         soap->recv_timeout    = 10;   
-         soap->send_timeout    = 10;   
-         soap->connect_timeout = 10;   
-     }     
-    soap_default_SOAP_ENV__Header(soap, header);  
- 
- // 为了保证每次搜索的时候MessageID都是不相同的！因为简单，直接取了随机值  
-     srand((int)time(0));  
-     Flagrand = rand()%9000 + 1000; //保证四位整数  
-     macaddr[0] = 0x1; macaddr[1] = 0x2; macaddr[2] = 0x3; macaddr[3] = 0x4; macaddr[4] = 0x5; macaddr[5] = 0x6;  
-     sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X",  
-             Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);  
-     header->wsa__MessageID =(char *)malloc( 100);  
-     memset(header->wsa__MessageID, 0, 100);  
-     strncpy(header->wsa__MessageID, _HwId, strlen(_HwId));  
- 
- if (was_Action != NULL)  
-     {  
-         header->wsa__Action =(char *)malloc(1024);  
-         memset(header->wsa__Action, '\0', 1024);  
-         strncpy(header->wsa__Action, was_Action, 1024);//"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  
-     }  
- if (was_To != NULL)  
-     {  
-         header->wsa__To =(char *)malloc(1024);  
-         memset(header->wsa__To, '\0', 1024);  
-         strncpy(header->wsa__To,  was_To, 1024);//"urn:schemas-xmlsoap-org:ws:2005:04:discovery";     
-     }  
-     soap->header = header;  
- return soap;  
- }  
- int ONVIF_ClientDiscovery( )  
- {  
- int HasDev = 0;  
- int retval = SOAP_OK;  
-     wsdd__ProbeType req;  
- struct __wsdd__ProbeMatches resp;  
-     wsdd__ScopesType sScope;  
- struct SOAP_ENV__Header header;  
- struct soap* soap;  
- 
- 
- const char *was_To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";  
- const char *was_Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  
- //这个就是传递过去的组播的ip地址和对应的端口发送广播信息      
- const char *soap_endpoint = "soap.udp://239.255.255.250:3702/";  
- 
- //这个接口填充一些信息并new返回一个soap对象，本来可以不用额外接口，  
- // 但是后期会作其他操作，此部分剔除出来后面的操作就相对简单了,只是调用接口就好  
-     soap = ONVIF_Initsoap(&header, was_To, was_Action, 5);  
- 
-     soap_default_SOAP_ENV__Header(soap, &header);  
-     soap->header = &header;  
- 
-     soap_default_wsdd__ScopesType(soap, &sScope);  
-     sScope.__item = "";  
-     soap_default_wsdd__ProbeType(soap, &req);  
-     req.Scopes = &sScope;  
-     req.Types = ""; //"dn:NetworkVideoTransmitter";  
- 
-     retval = soap_send___wsdd__Probe(soap, soap_endpoint, NULL, &req);  
- //发送组播消息成功后，开始循环接收各位设备发送过来的消息  
- while (retval == SOAP_OK)  
-     {  
-         retval = soap_recv___wsdd__ProbeMatches(soap, &resp);  
- if (retval == SOAP_OK)  
-         {  
- if (soap->error)  
-             {  
-                 printf("[%d]: recv error:%d,%s,%s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
-                 retval = soap->error;  
-             }  
- else //成功接收某一个设备的消息  
-             {  
-                 HasDev ++;  
- if (resp.wsdd__ProbeMatches->ProbeMatch != NULL && resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL)  
-                 {  
-                     printf(" ################  recv  %d devices info #### \n", HasDev );  
-                     printf("Target Service Address  : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);  
-                     printf("Target EP Address       : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);  
-                     printf("Target Type             : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);  
-                     printf("Target Metadata Version : %d\r\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);  
-                     sleep(1);  
-                 }  
-             }  
-         }  
- else if (soap->error)  
-         {  
- if (HasDev == 0)  
-             {  
-                 printf("[%s][%d] Thers Device discovery or soap error: %d, %s, %s \n", __func__, __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstrin  
-                 retval = soap->error;  
-             }  
- else  
-             {  
-                 printf(" [%s]-[%d] Search end! It has Searched %d devices! \n", __func__, __LINE__, HasDev);  
-                 retval = 0;  
-             }  
- break;  
-         }  
-     }  
- 
-     soap_destroy(soap);  
-     soap_end(soap);  
-     soap_free(soap);  
- 
- return retval;  
- }  
- int main(void )  
- {  
- //组播接口   
- if (ONVIF_ClientDiscovery() != 0 )  
-     {  
-         printf("discovery failed!\n");  
- return -1;  
-     }  
- 
- return 0;  
- }  
- </span>  



运行测试结果如下图

![](http://img.blog.csdn.net/20131226203901968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




可以看到注意信息就是http://172.18.13.127:9000/onvif/device_service这句了!设备的IP地址以及通信端口都有了，第三个没有端口，也就是使用默认的端口80了，海康和大华的貌似都是用80端口
到这里差不多客户端的搜索就完成了，剩下只是把搜索到的设备信息解析出来，然后保存起来！提供给客户端使用了，我在实际开发的过程中是通过调用外层接口传递一个回调函数指针进去，然后匹配信息出来，给客户端！

搜索使用的源码下载地址：[onvif客户端搜索](http://download.csdn.net/detail/max_min_/6774135)









