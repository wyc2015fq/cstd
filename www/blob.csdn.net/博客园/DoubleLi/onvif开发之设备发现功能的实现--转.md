# onvif开发之设备发现功能的实现--转 - DoubleLi - 博客园






忙了一个多月，onvif总算告一段落了。这几个星期忙着其他的项目，也没有好好整理一下onvif的东西。接下来得好好整理一下自己的项目思路和项目经验，同时将自己的一些心得写出来，希望对人有所帮助。

        相信大多数兄弟和我一样，onvif开发，最开始做的就是发现功能。这两天登录onvif的官网看才发现，onvif版本在八月份有更新，已经更新到V2.4了，于是下载最新的版本来进行。代码的生成可以详见我的前一篇文章。V2.4版本新增了一个wsdl文件，现在用于生成源码的文件一共有18个。为了保证全功能，最好一次性生成包含所有功能的源码。然后根据最新生成的源码来实现onvif客户端和服务端的发现功能。



1. 创建onvif_test目录。以下这些源码由最新的gsoap（2.8.16）和最新的onvif的wsdl文件（2.4）生成。（截止2013.09.16）

onvif.h

soapClientLib.c

soapServerLib.c

soapC.c

soapClient.c

soapH.h

soapServer.c

soapStub.h



2.以下文件来自gsoap_2.8.16\gsoap-2.8\gsoap

stdsoap2.c

stdsoap2.h



3.以下文件来自gsoap_2.8.16\gsoap-2.8\gsoap\custom

duration.c



4.生成的soapClientLib.c和soapServerLib.c无实际作用，可直接删除。



5.增加自定义文件：

onvif_server.c   onvif服务端实现代码

onvif_client.c   onvif客户端实现代码

onvif_server_interface.c   onvif服务端接口实现

onvif_function.c  onvif实现函数，公用

onvif_function.h  onvif实现函数，公用



onvif_server.c为服务端的实现代码，主要是定义main函数，服务端主要是监听，并处理和应答消息。

Main函数定义如下：



**[cpp]**[view plain](http://blog.csdn.net/love_xjhu/article/details/11821037#)[copy](http://blog.csdn.net/love_xjhu/article/details/11821037#)



- int main(int argc,char ** argv)  
- {  
-     printf("[%s][%d][%s][%s] start \n", __FILE__, __LINE__, __TIME__, __func__);  
- 
- int count = 0;  
- struct soap ServerSoap;  
- struct ip_mreq mcast;  
- 
-     soap_init1(&ServerSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);  
-     soap_set_namespaces(&ServerSoap,  namespaces);  
- 
-     printf("[%s][%d][%s][%s] ServerSoap.version = %d \n", __FILE__, __LINE__, __TIME__, __func__, ServerSoap.version);  
- 
- if(!soap_valid_socket(soap_bind(&ServerSoap, NULL, ONVIF_LISTEN_PORT, 10)))  
-     {  
-         soap_print_fault(&ServerSoap, stderr);  
-         exit(1);  
-     }  
- 
-     mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");  
-     mcast.imr_interface.s_addr = htonl(INADDR_ANY);  
- 
- if(setsockopt(ServerSoap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)  
-     {  
-             printf("setsockopt error! error code = %d,err string = %s\n",errno,strerror(errno));  
- return 0;  
-     }  
- 
- for(;;)  
-     {  
- if(soap_serve(&ServerSoap))  
-         {  
-             soap_print_fault(&ServerSoap, stderr);  
-         }  
- 
-         soap_destroy(&ServerSoap);  
-         soap_end(&ServerSoap);  
- 
- //客户端的IP地址  
-         printf("RECEIVE count %d, connection from IP = %lu.%lu.%lu.%lu socket = %d \r\n", count, ((ServerSoap.ip)>>24)&0xFF, ((ServerSoap.ip)>>16)&0xFF, ((ServerSoap.ip)>>8)&0xFF,(ServerSoap.ip)&0xFF, (ServerSoap.socket));  
-         count++;  
-     }  
- 
- //分离运行时的环境  
-     soap_done(&ServerSoap);  
- 
- return 0;  
- }  


onvif_server_interface.c   此文件用来定义所有服务端需要填充的接口。这里我们填充__wsdd__Probe接口即可，其他赞不支持的接口可以用宏统一处理。

__wsdd__Probe的填充如下：



**[cpp]**[view plain](http://blog.csdn.net/love_xjhu/article/details/11821037#)[copy](http://blog.csdn.net/love_xjhu/article/details/11821037#)



- SOAP_FMAC5 int SOAP_FMAC6  __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe)  
- {  
- 
-     #define MACH_ADDR_LENGTH 6  
-     #define INFO_LENGTH 512  
-     #define LARGE_INFO_LENGTH 1024  
-     #define SMALL_INFO_LENGTH 512  
- 
-     printf("[%d] __wsdd__Probe start !\n", __LINE__);  
- 
-     unsigned char macaddr[6] = {0};  
- char _IPAddr[INFO_LENGTH] = {0};  
- char _HwId[1024] = {0};  
- 
-     wsdd__ProbeMatchesType ProbeMatches;  
-     ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));  
-     ProbeMatches.ProbeMatch->XAddrs = (char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);  
-     ProbeMatches.ProbeMatch->Types = (char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);  
-     ProbeMatches.ProbeMatch->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap,sizeof(struct wsdd__ScopesType));  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap,sizeof(struct wsa__ReferencePropertiesType));  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap,sizeof(struct wsa__ReferenceParametersType));  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap,sizeof(struct wsa__ServiceNameType));  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *) * SMALL_INFO_LENGTH);  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute = (char *)soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.Address = (char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);  
- 
-     netGetMac("eth4", macaddr); //eth0  根据实际情况填充  
-     macaddr[0]=0x01;macaddr[1]=0x01;macaddr[2]=0x01;macaddr[3]=0x01;macaddr[4]=0x01;macaddr[5]=0x01;  
-     sprintf(_HwId,"urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X",macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);  
- 
- 
-     unsigned int localIp = 0;  
-     netGetIp("eth4", &localIp); //eth0 根据实际情况填充  
-     sprintf(_IPAddr, "http://%s/onvif/device_service", inet_ntoa(*((struct in_addr *)&localIp)));  
-     printf("[%d] _IPAddr ==== %s\n", __LINE__, _IPAddr);  
- 
-     ProbeMatches.__sizeProbeMatch = 1;  
-     ProbeMatches.ProbeMatch->Scopes->__item =(char *)soap_malloc(soap, 1024);  
-     memset(ProbeMatches.ProbeMatch->Scopes->__item,0,sizeof(ProbeMatches.ProbeMatch->Scopes->__item));    
- 
- //Scopes MUST BE  
-     strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/type/NetworkVideoTransmitter");  
- 
-     ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;  
-     strcpy(ProbeMatches.ProbeMatch->XAddrs, _IPAddr);  
-     strcpy(ProbeMatches.ProbeMatch->Types, wsdd__Probe->Types);  
-     printf("wsdd__Probe->Types=%s\n",wsdd__Probe->Types);  
-     ProbeMatches.ProbeMatch->MetadataVersion = 1;  
- 
- //ws-discovery规定 为可选项  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;  
- 
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = (char *)soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);  
- //ws-discovery规定 为可选项  
-     strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0], "ttl");  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = (char *)soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);  
-     strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any");  
-     strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute, "Attribute");  
-     ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;  
-     strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.Address, _HwId);  
- 
-     soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";  
-     soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";  
-     soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));  
-     soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;  
-     soap->header->wsa__RelatesTo->RelationshipType = NULL;  
-     soap->header->wsa__RelatesTo->__anyAttribute = NULL;  
- 
-     soap->header->wsa__MessageID =(char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);  
-     strcpy(soap->header->wsa__MessageID,_HwId+4);  
- 
- if (SOAP_OK == soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches))  
-         {  
-         printf("send ProbeMatches success !\n");  
- return SOAP_OK;  
-     }  
- 
-     printf("[%d] soap error: %d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));    
- 
- return soap->error;;  
- 
- }  


onvif_client.c   onvif客户端实现代码，主要是定义客户端的main函数：

Main定义如下：



**[cpp]**[view plain](http://blog.csdn.net/love_xjhu/article/details/11821037#)[copy](http://blog.csdn.net/love_xjhu/article/details/11821037#)



- int main()    
- {    
-     printf("[%s][%d][%s][%s] start \n", __FILE__, __LINE__, __TIME__, __func__);  
- 
- int result = 0;    
-     wsdd__ProbeType req;  
- struct __wsdd__ProbeMatches resp;  
-     wsdd__ScopesType sScope;  
- struct SOAP_ENV__Header header;    
- 
- struct soap *soap;    
-     soap = soap_new();    
- if(NULL == soap )    
-     {    
-         printf("sopa new error\r\n");    
- return -1;    
-     }    
- 
-     soap->recv_timeout = 10;    
-     soap_set_namespaces(soap, namespaces);    
-     soap_default_SOAP_ENV__Header(soap, &header);    
- 
-     uuid_t uuid;  
- char guid_string[100];  
-     uuid_generate(uuid);  
-     uuid_unparse(uuid, guid_string);  
- 
-     header.wsa__MessageID = guid_string;   
-     header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";    
-     header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";    
-     soap->header = &header;    
- 
-     soap_default_wsdd__ScopesType(soap, &sScope);    
-     sScope.__item = "";    
-     soap_default_wsdd__ProbeType(soap, &req);    
-     req.Scopes = &sScope;    
-     req.Types = ""; //"dn:NetworkVideoTransmitter";    
- 
- int i = 0;        
-        result = soap_send___wsdd__Probe(soap, MULTICAST_ADDRESS, NULL, &req);    
- while(result == SOAP_OK)    
-        {    
-         result = soap_recv___wsdd__ProbeMatches(soap, &resp);    
- if(result == SOAP_OK)    
-         {    
- if(soap->error)    
-             {    
-                 printf("soap error 1: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));    
-                 result = soap->error;    
-             }    
- else    
-             {    
-                 printf("guog *********************************************\r\n");    
- if(soap->header->wsa__MessageID)    
-                 {    
-                     printf("MessageID   : %s\r\n", soap->header->wsa__MessageID);    
-                 }    
- if(soap->header->wsa__RelatesTo && soap->header->wsa__RelatesTo->__item)    
-                 {    
-                     printf("RelatesTo   : %s\r\n", soap->header->wsa__RelatesTo->__item);    
-                 }    
- if(soap->header->wsa__To)    
-                 {    
-                     printf("To          : %s\r\n", soap->header->wsa__To);    
-                 }    
- if(soap->header->wsa__Action)    
-                 {    
-                     printf("Action      : %s\r\n", soap->header->wsa__Action);    
-                 }    
- 
- for(i = 0; i < resp.wsdd__ProbeMatches->__sizeProbeMatch; i++)    
-                 {    
-                     printf("__sizeProbeMatch        : %d\r\n", resp.wsdd__ProbeMatches->__sizeProbeMatch);    
-                     printf("wsa__EndpointReference       : %p\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference);    
-                     printf("Target EP Address       : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);    
-                     printf("Target Type             : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);    
-                     printf("Target Service Address  : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);    
-                     printf("Target Metadata Version : %d\r\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);    
- if(resp.wsdd__ProbeMatches->ProbeMatch->Scopes)    
-                     {    
-                         printf("Target Scopes Address   : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item);    
-                     }    
-                 }  
-             }    
-         }    
- else if (soap->error)    
-         {    
-             printf("[%d] soap error 2: %d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));    
-             result = soap->error;    
-         }    
-        }    
- 
-     soap_destroy(soap);   
-     soap_end(soap);   
-     soap_free(soap);  
- 
-     printf("[%d] guog discover over !\n", __LINE__);  
- 
- return result;    
- }    


6.将wsdd.nsmap改为nsmap.h，并删除其余的*.nsmap(都一样)



7.编写makefile文件。注意在makefile中打开开关DEBUG，以便跟踪日志。



8.tcpdump为gcc环境的抓包工具，调试的时候用。



make编译通过，运行，客户端发现功能ok；

![](http://img.blog.csdn.net/20130918215818078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG92ZV94amh1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    但是服务端的发现功能却不行；别急，这是由于SOAP的版本问题；soap的版本是根据命名空间来自动确定的；在soap结构体的version字段表示soap版本；

以下命名空间表示SOAP1.1版本：

{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},

{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL}, //1.1



以下命名空间表示SOAP1.2版本：

{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},

{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},  //1.2

    注意确定自己当前的命名空间。我们用SOAP1.2版本才能被测试工具发现(ONVIF Device Test Tool version 13.06)。

不清楚可以查看soap_set_namespaces接口和soap_set_local_namespaces接口；

于是将nsmap.h中的命名空间前两行改为：

{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},

{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},  //1.2

再编译运行，发现功能ok。

![](http://img.blog.csdn.net/20130918215733828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG92ZV94amh1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



最后，将所用的工具和源码提供给大家，大家根据需要下载。


onvif测试工具V13.06：[http://download.csdn.net/detail/u011597695/6288593](http://download.csdn.net/detail/u011597695/6288593)


gsoap 2.8.16版本： [http://download.csdn.net/detail/u011597695/6288615](http://download.csdn.net/detail/u011597695/6288615)


onvif v2.4版本的wsdl文件：[http://download.csdn.net/detail/u011597695/6288627](http://download.csdn.net/detail/u011597695/6288627)


onvif v2.4版本的wsdl文件（适于离线生成源码）：[http://download.csdn.net/detail/u011597695/6288647](http://download.csdn.net/detail/u011597695/6288647)


onvif源代码V2.4：[http://download.csdn.net/detail/u011597695/6288663](http://download.csdn.net/detail/u011597695/6288663)



from:http://blog.csdn.net/love_xjhu/article/details/11821037









