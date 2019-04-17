# onvif开发实战2--总结框架搭建 - DoubleLi - 博客园






完成框架搭建后，编写自己的主函数起onvif服务

编写makefile

objs = onvif.o onvif_func.o duration.o soapC.o soapServer.o stdsoap2.o


onvif:$(objs)
    gcc -o onvif $(objs)


.PHONY:clean
clean:
    #-rm onvif
    rm *[!C.o].o



发现提示好多函数没有定义，在头文件soapStub.h中定义的，直接把没有定义的函数声明拷贝到一个onvif_func.c中

暂时用ue等工具实现一个空函数，

将；替换为^p{^p^treturn SOAP_OK；^p}

就可以实现素有函数的空实现，然后编译通过就可以了。。。



下面就是启动onvif服务端代码的具体实现了。。



## 组播setsockopt:no such device问题的解决方法





route add -net 224.0.0.0 netmask 224.0.0.0 eth0



然后



## 4、ProbeMatches代码


这样就创建了基本的服务端和客户端的代码了，下面需要添加具体的代码了。





其中包括：

(1)创建组播用的udp socket，绑定组播地址为239.255.255.250，端口为3702，因为ws-discovery的组播地址和端口就是为239.255.255.250和3702

(2)在产生的Probe函数中添加ProbeMatches代码
首先是udp socket





**[cpp]**[view plain](http://blog.csdn.net/ghostyu/article/details/8182516#)[copy](http://blog.csdn.net/ghostyu/article/details/8182516#)





- int bind_server_udp1(int server_s)  
- {  
- struct sockaddr_in local_addr;  
-     memset(&local_addr,0,sizeof(local_addr));  
-     local_addr.sin_family = AF_INET;  
-     local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
-     local_addr.sin_port = htons(3702);  
- return bind(server_s,(struct sockaddr*)&local_addr,sizeof(local_addr));  
- 
- }  
- static int create_server_socket_udp(void)  
- {  
- int server_udp;  
-     unsigned char one = 1;  
- int sock_opt = 1;  
- 
- //server_udp = socket(PF_INET, SOCK_DGRAM, 0);  
-     server_udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);  
- if (server_udp == -1) {  
-         printf("unable to create socket\n");  
-     }  
- 
- /* reuse socket addr */  
- if ((setsockopt(server_udp, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,  
- sizeof (sock_opt))) == -1) {  
-         printf("setsockopt\n");  
-     }  
- if ((setsockopt(server_udp, IPPROTO_IP, IP_MULTICAST_LOOP,  
-                        &one, sizeof (unsigned char))) == -1) {  
-         printf("setsockopt\n");  
-     }  
- 
- struct ip_mreq mreq;  
-     mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");  
-     mreq.imr_interface.s_addr = htonl(INADDR_ANY);  
- 
- if(setsockopt(server_udp,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))==-1){  
-         perror("memberchip error\n");  
-     }  
- 
- return server_udp;  
- }  



需要注意几点：1/设置socket属性SO_REUSEADDR，2、设置socket属性IP_ADD_MEMBERSHIP，目的是让3702的端口能够重复绑定，一家加入组播组。



其次是添加ProbeMatches代码
(1)首先复制client的soap_send___wsdd__ProbeMatches函数到服务端来，因为soap_send___wsdd__ProbeMatches已经写好了用于响应Probe消息的框架了，不用白不用啊。
(2)编写__wsdd__Probe函数，添加如下内容





**[cpp]**[view plain](http://blog.csdn.net/ghostyu/article/details/8182516#)[copy](http://blog.csdn.net/ghostyu/article/details/8182516#)





- int  __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe)  
- {  
-     DBG("__wsdd__Probe\n");  
- char macaddr[6];  
- char _IPAddr[INFO_LENGTH];  
- char _HwId[1024];  
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
-     macaddr[0]=0x01;macaddr[1]=0x01;macaddr[2]=0x01;macaddr[3]=0x01;macaddr[4]=0x01;macaddr[5]=0x01;  
-     sprintf(_HwId,"urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X",macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);  
- 
-     sprintf(_IPAddr, "http://%03d.%03d.%1d.%03d/onvif/device_service", 192, 168, 1, 233);  
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
-     DBG("wsdd__Probe->Types=%s\n",wsdd__Probe->Types);  
-     ProbeMatches.ProbeMatch->MetadataVersion = 1;  
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
- /*注释的部分为可选，注释掉onvif test也能发现ws-d*/  
- //soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";  
- //soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";  
-     soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));  
- //it's here  
-     soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;  
-     soap->header->wsa__RelatesTo->RelationshipType = NULL;  
-     soap->header->wsa__RelatesTo->__anyAttribute = NULL;  
- 
-     soap->header->wsa__MessageID =(char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);  
-     strcpy(soap->header->wsa__MessageID,_HwId+4);  
- 
- /* send over current socket as HTTP OK response: */  
- /*测试过，第二参数必须http，action随意*/  
-     soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches);  
- return SOAP_OK;  
- 
- }  



想要写出上述代码，是一定要了解SOAP格式的，在WS-Discovery中描述了discovery所用的soap格式



**1首先是了解消息头header和ProbeMatches中的内容，非常重要，可以参考这里http://www.w3.org/Submission/ws-addressing/  最好详细的学习一下，里面的内容非常重要。**

**2其次需要理解的是，其实当你看完ws-addressing后你会发现，骨架代码中的结构体和SOAP消息中的内容是一一对应的，例如：**

**结构体osap->header对应SOAP消息的<SOAP-ENV:Header></SOAP-ENV:Header>中的内容，包含在header里的内容当然会包含在SOAP的header内。例如：**

**结构体soap->header->wsa__RelatesTo对应的是<wsa:RelatesTo></wsa:RelatesTo>。**

**3最后需要理解的是，在代码中的"__"双下划线一般对应xml中的命名空间的":"，下划线前是命名空间，后是具体内容。**

**4最后的最后是要详细的阅读ONVIF Core Specification**

下图为响应OnvifTestTool的Probe命令的SOAP消息

![](http://img.my.csdn.net/uploads/201211/14/1352875374_6095.png)

结合上图再分析代码就亲切多了。在ONVIF Core Specification的7.3.2.2  Scopes 一节描述了onvif需要的Scopes，这个是需要在程序里填充，具体填充什么，文档里说的很明确：

![](http://img.my.csdn.net/uploads/201211/14/1352875894_7633.png)

注意点是在太多，随便漏掉一个都可能会导致搜不到设备，下图是非常重要的一个：

![](http://img.my.csdn.net/uploads/201211/14/1352876288_2870.png)

SOAP1.1和SOAP1.2所使用的SOAP-ENV是不同的，ONVIF使用的是SOAP1.1，如果soapcpp2产生的nsmap文件中的SOAP-ENV是SOAP1.2版本的话，那么OnvifTestTool是不会识别设备发出的SOAP消息的。

## 5、该main函数登场了





**[cpp]**[view plain](http://blog.csdn.net/ghostyu/article/details/8182516#)[copy](http://blog.csdn.net/ghostyu/article/details/8182516#)





- int main()  
- {  
- int server_udp;  
- 
- int retval=0;  
- struct soap *soap_udp;  
- int fault_flag = 0;  
- 
-     server_udp = create_server_socket_udp();  
-     bind_server_udp1(server_udp);  
- while(1){  
-         soap_udp=soap_new();  
-         soap_init1(soap_udp, SOAP_IO_UDP);  
-         soap_udp->master = server_udp;  
-         soap_udp->socket = server_udp;  
-         soap_udp->errmode = 0;  
-         soap_udp->bind_flags = 1;  
- if (!soap_valid_socket(soap_bind(soap_udp, NULL, 3702, 100)))  
-         {      
-             soap_print_fault(soap_udp, stderr);  
-         }  
-         fprintf(stderr,"soap_serve starting..\n");  
-         retval = soap_serve(soap_udp); //阻塞在这里  
-         fprintf(stderr,"retval=%d\n",retval);  
- if(retval && !(fault_flag))  
-         {  
-             fault_flag = 1;  
-         }  
- else if(!retval)  
-         {  
-             fault_flag = 0;  
-         }  
-         soap_destroy(soap_udp);  
-         soap_end(soap_udp);  
-         soap_done(soap_udp);  
-         free(soap_udp);  
-     }  
- }  



soap_server函数会一直阻塞，直到接收到SOAP消息，并且该处理是一次性的，所以要将将soap_server放到while里或者独立的线程中。
最后编译运行

问题：哥编译出来的代码，各项都正确啊，就是搜索不到，test工具提示没有返回消息，麻蛋啊。。

自习查看上文才发现，哥的SOAP1.2的，onvif只支持1.1，换了果断可以搜索到。。妹的。。









