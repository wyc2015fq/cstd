# Onvif学习之客户端设备发现 - 建建的博客 - CSDN博客
2017年02月14日 13:49:16[纪建](https://me.csdn.net/u013898698)阅读数：1110
## 前言
```
本文从零基础一步步实现ONVIF协议、RTSP/RTP协议获取IPC实时视频流、FFMPEG解码。开发环境为WIN7 32位 + VS2010。
    最终成功获取浩云、海康、大华的IPC实时视频流。
    如果要了解本文更多细节，或者用本文作设计指导，那最好把文中提到的连接都打开，与本文对照着看。
```
## 前期准备
```
1.准备一个ONVIF服务器
    既然开发的是客户端，那必需要有服务端了。我这里大把的IPC，好几个品牌的，就随便拿了一个。
    如果没有IPC，倒是可以用 VLC media player 搭建一下。或者其他播放器也可以。这个网上很多资料。
2.准备一个ONVIF 测试工具
    这个工具在ONVIF的官网上可以找到：ONVIF Device Test Tool 。
3.准备解码器相关资料及资源
    收到视频流后，需要解码。可以用ffmpeg，也可以用其他解码库。这个是后话了，等ONVIF搞定之后再搞解码也不迟。推荐链接：
    http://wenku.baidu.com/view/f8c94355c281e53a5802ffe4.html?re=view 
    (Windows下使用MinGW编译ffmpeg与x265)
4.准备资料
    ONVIF协议书必看，ONVIF官网自然是不能少的。其他资料推荐几个链接：
http://www.cuplayer.com/player/PlayerCode/camera/2015/1119/2156.html 
http://blog.csdn.net/gubenpeiyuan/article/details/25618177#
http://blog.csdn.net/saloon_yuan/article/details/24901597
http://www.onvif.org/onvif/ver20/util/operationIndex.html
5.准备抓包工具IPAnalyse
    关系到网络通信的有个IP抓包工具能让你省去很多麻烦，也能让你清楚的看到协议的细节。IPAnalyse很容易在网上可以下载。
```
## 测试ONVIF
```
看《ONVIF协议书》估计很多人都会云里雾里，实在搞不懂的话，那就接上IPC，打开 Test Tool，测试一下各项功能。Test Tool里可以看到整个协议的工作细节，每一步做什么，发了什么报文，收了什么报文，都可以看到。
```
![Test Tool](https://img-blog.csdn.net/20161111161538542)
![IPA](https://img-blog.csdn.net/20161111161705481)
```
如果你没有IPC，那用VLC理论上也可以，不过我没测试过。两个VLC(一个作为服务器一个作为客户端)加上IP抓包工具，这个我用过也可以看到协议细节。不过从抓包工具里看到的只是一段段的报文，没有步骤说明，不如Test tool来得明了。
    当然，如果你看懂了ONVIF协议，那就不必搞这些。
```
## Soap及开发框架生成
```
本人一开始并没有听过soap，只好自已查资料去了，这里也不班门弄斧。这个开发框架生成网上大把大把的资料，但系都不好使啊。每个人的开发环境都有一点点差别，以至于很多文章都不能从头至尾的跟着走一次。唯有看比较多的文章再总结一下，才能自己生成一个框架。所以我这里也不多说了，推荐链接：
```
E.[http://blog.csdn.net/saloon_yuan/article/details/24901597](http://blog.csdn.net/saloon_yuan/article/details/24901597)
```
当然也可以不用框架。如你所见，ONVIF的实现最终不过是发送报文和接收报文，用到的功能不多的话完全可以自己手动发报文过去，再解析接收到的报文。
```
后面也会说到不用框架来发现设备。
## ONVIF设备发现、设备搜索
```
设备发现的过程：客户端发送报文到239.255.255.250的3702端口(ONVIF协议规定)，服务器收到报文后再向客户端返回一个报文，客户端收到这个报文然后解析出Xaddrs，这就完成了一次设备发现。
    理论上只要报文能正确收发就可以发现设备。不过，用soap框架搜索设备的时候，多网卡、跨网段等复杂网络会出现搜索不到的问题。这时候就需要路由支持才行(网上说可以加入相关的路由协议)。
    为了解决这个问题，自己又写了一个非SOAP框架的设备发现函数。两个发现函数请看下面代码。
```
```cpp
/*
    SOAP初始化
*/
int UserInitSoap(struct soap *soap,struct SOAP_ENV__Header  *header)
{   
    if(soap == NULL){
        return NULL;
    }
    //命名空间
    soap_set_namespaces( soap, namespaces);
    //参数设置
    int timeout = 5 ;
    soap->recv_timeout      =  timeout ;
    soap->send_timeout      =  timeout ;
    soap->connect_timeout   =  timeout ;
    // 生成GUID , Linux下叫UUID
    char buf[128];
    GUID guid; 
    if(CoCreateGuid(&guid)==S_OK){
        _snprintf_s(buf, sizeof(buf) 
            ,"urn:uuid:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
            , guid.Data1   , guid.Data2   , guid.Data3
            , guid.Data4[0], guid.Data4[1]
            , guid.Data4[2], guid.Data4[3], guid.Data4[4],guid.Data4[5]
            , guid.Data4[6], guid.Data4[7] );
    }
    else{
        _snprintf_s(buf, sizeof(buf),"a5e4fffc-ebb3-4e9e-913e-7eecdf0b05e8");
    }
    //初始化
    soap_default_SOAP_ENV__Header(soap, header);
    //相关参数写入句柄
    header->wsa__MessageID =(char *)soap_malloc(soap,128);
    memset(header->wsa__MessageID, 0, 128);
    memcpy(header->wsa__MessageID, buf, strlen(buf));
    header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"; 
    header->wsa__To     = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
    //写入变量
    soap->header = header;
    return 0 ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
```
SOAP里所有要申请的空间请用带有soap_XXX()的，否则不能通过soap_del(soap);来释放所有空间。这会造成内存泄漏。
```
```
/*
    SOAP释放
*/
int UserReleaseSoap(struct soap *soap)
{
    //soap_free(soap);
    soap_destroy(soap); 
    soap_end(soap); 
    soap_done(soap);
    soap_del(soap);
    /*
    The gSOAP engine uses a memory management method to allocate and deallocate memory. 
    The deallocation is performed with soap_destroy() followed by soap_end(). 
    However, when you compile with -DDEBUG or -DSOAP_MEM_DEBUG then no memory 
    is released until soap_done() is invoked. This ensures that the gSOAP engine 
    can track all malloced data to verify leaks and double frees in debug mode. 
    Use -DSOAP_DEBUG to use the normal debugging facilities without memory debugging. 
    Note that some compilers have DEBUG enabled in the debug configuration, so this behavior 
    should be expected unless you compile in release config
    */
    return 0 ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
```
以下这里有用到类，其中IP为要发现设备IP，XAddrs为设备的端地址。要复制这段代码的，请自行修改。
```
```
int GetVideo::FindONVIFservers()
{
    //清除错误
    this->error = 0 ;
    memset(this->error_info , 0, 1024);
    memset(this->error_info_, 0, 1024);
    int FoundDevNo = 0;     //发现的设备数
    int retval = SOAP_OK;   //返回值
    struct __wsdd__ProbeMatches  resp;
    struct SOAP_ENV__Header  header;
    struct soap*  soap;
    soap = soap_new();
    UserInitSoap(soap,&header);
    ////////////////////////////////////////////
    //send the message broadcast and wait
    //IP Adress and PortNo, broadCast   
    const char *soap_endpoint = "soap.udp://239.255.255.250:3702/"; //从ONVIF协议得来
    //范围相关参数
    wsdd__ProbeType  req;    
    wsdd__ScopesType  sScope;
    soap_default_wsdd__ScopesType(soap, &sScope);
    sScope.__item = "";//"http://www.onvif.org/??"
    soap_default_wsdd__ProbeType(soap, &req);
    req.Scopes = &sScope;
    req.Types = ""; //"dn:NetworkVideoTransmitter";
    ///////////////////////////////////////////////////////////////
    //发送10次，直到成功为止
    int time=10;
    do{
        retval = soap_send___wsdd__Probe(soap, soap_endpoint, NULL, &req);
        Sleep(100);
    }while(retval != SOAP_OK && time--);
    ////////////////////////////////////////////////////////////////
    //一直接收，直到收到当前IP的信息后退出，或收不到当前IP但所有已接收完退出
    while (retval == SOAP_OK)
    {
        retval = soap_recv___wsdd__ProbeMatches(soap, &resp);
        //printf("\nrecv retval = %d \n",retval);
        if (retval == SOAP_OK) {
            if (!soap->error){ 
                FoundDevNo ++;  //找到一个设备
                if (resp.wsdd__ProbeMatches->ProbeMatch != NULL && 
                    resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL
                    ){
                    //判断IP是否是你想要找的IP
                    char *http = strstr(resp.wsdd__ProbeMatches->ProbeMatch->XAddrs, this->IP );
                    if(http!=NULL){
                        //得到XAddrs(这里认为不超过256)
                        //因为有些设备有多个XAddrs，这里要分离出一个
                        memcpy(this->XAddrs,"http://",7);   
                        for(int t=0;t<255-7;t++){   
                            if(http[t]==' ' || http[t]=='\n' || 
                                http[t]=='\r'|| http[t]=='\0' ){
                                    retval = 1234 ; //退出while
                                    break;
                            }
                            this->XAddrs[t+7] = http[t] ;
                        }
                    }//end if(http!=NULL)
                }
            }
            else{
                retval = soap->error;   //退出while
                this->error = soap->error ; //错误代码
                const char *tmp  = *soap_faultcode(soap)   ;    //错误信息
                if(tmp)
                    memcpy(error_info , tmp , strlen(tmp ));    //复制到类
                const char *tmp_ = *soap_faultstring(soap) ;
                if(tmp_)
                    memcpy(error_info_, tmp_, strlen(tmp_));
            }
        }
        else if (soap->error){  //搜索完所有设备
            if (FoundDevNo){
                soap->error = 0 ;
                retval = 0;
            }
            else {
                retval = soap->error; 
            }
            break; //退出while
        }  
        #ifdef DEBUG_INFOPRINT
        if (retval == SOAP_OK) {
            if (!soap->error){ //找到一个设备
                //打印相关信息
                if (resp.wsdd__ProbeMatches->ProbeMatch != NULL && resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL){
                    printf("****** No %d Devices Information ******\n", FoundDevNo );
                    printf("Device Service Address  : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);    
                    printf("Device EP Address       : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);  
                    printf("Device Type             : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);  
                    printf("Device Metadata Version : %d\r\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);  
                    //sleep(1);
                }
            }
            else{
                printf("[%d]: recv soap error :%d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
            }
        }
        else if (soap->error){  
            if (FoundDevNo){
                printf("Search end! Find %d Device! \n", FoundDevNo);
            }
            else {
                printf("No Device found!\n"); 
            }
            break;
        }  
        #endif
    }
    //错误处理
    if(retval!=1234){
        this->error = NO_DEVICE ;
        memcpy(this->error_info_, "ONVIF:  this device NOT found\n", 128);
        char tmp[128];
        sprintf_s(tmp,128,"ONVIF:  found %d devices \n",FoundDevNo);
        memcpy(this->error_info, tmp, 128);
    }
    ////////////////////////////////////////////////
    // 退出
    Sleep(10);
    UserReleaseSoap(soap);
    return retval;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
```
以上代码实现了整个设备发现的过程，最终得到设备的XAddrs。流程为：设置命名空间 >> 超时设置 >> 生成GUID >> 填充header >> 设置搜索范围 >> 发送报文 >> 接收并解析报文 。
```
## 非SOAP框架 设备发现
```
以下代码也实现了事个发现过程，用到几个类变量：this->IP是要发现的设备IP，this->Port是其端口号，this->LocalIP是用来保存与设备IP同一网段的本地IP地址，this->FIND_BASE_UDP_PORT是用于连接设备的本地端口号(如果此端口号被占用，会自动加2查找下一个)。
    关于char *cxml 报文，最直接的方法是从Test tool里复制过来，或者从抓包工具里复制出来，当然也可以复制本文的。
    流程为：输入要发现的设备IP - 判断本地是否添加了此IP网段 - 测试设备是否可以连接 - 绑定本地IP - 准备报文 - 发送报文 - 接收报文 - 解析报文 。
```
```
/*
    获取本机IP
*/ 
bool GetLocalIPs(char ips[][32], int maxCnt, int *cnt)  
{  
    //初始化wsa  
    WSADATA wsaData;  
    int ret=WSAStartup(0x0202,&wsaData);  
    if (ret!=0){  return false; }  
    //获取主机名  
    char hostname[256];  
    ret=gethostname(hostname,sizeof(hostname));  
    if (ret==SOCKET_ERROR){  return false; }  
    //获取主机ip  
    HOSTENT* host=gethostbyname(hostname);  
    if (host==NULL){  return false; }
    //转化为char*并拷贝返回  
    //注意这里，如果你本地IP多于32个就可能出问题了
    *cnt=host->h_length<maxCnt? host->h_length:maxCnt;  
    for (int i=0;i<*cnt;i++)  {  
        in_addr* addr =(in_addr*)*host->h_addr_list;  
        strcpy_s(ips[i], 16, inet_ntoa(addr[i]));  
    }  
    WSACleanup();
    return true;  
}  
/*
    TCP Online Test 
*/
int TcpOnlineTest(char *IP,int Port)
{
    //加载库 
    //启动SOCKET库，版本为2.0 
    WSADATA wsdata;  
    WSAStartup(0x0202,&wsdata);  
    SOCKET PtcpFD=socket(AF_INET,SOCK_STREAM,0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP); //服务器端的地址
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(Port);               //服务器端的端口
    //connect(PtcpFD, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));   //会阻塞很久才反应过来
    //设置为非阻塞模式
    //这样，在connect时，才会立马跳过，不至于等太长时间
    int error = -1;    
    int len = sizeof(int);
    timeval tm;    
    fd_set set;    
    unsigned long ul = 1;    
    ioctlsocket(PtcpFD, FIONBIO, &ul); 
    int err =connect(PtcpFD, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if(err==-1){
        tm.tv_sec  = 3;        
        tm.tv_usec = 0;        
        FD_ZERO(&set);        
        FD_SET(PtcpFD, &set);        
        if( select(PtcpFD+1, NULL, &set, NULL, &tm) > 0){            
            getsockopt(PtcpFD, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len); 
            if(error == 0){ err = 0; }      
            else { err = -1; }   
        }        
        else { err = -1; }
    }//end if(err==-1)
    if(err==0){
        //设置为阻塞模式
        ul = 0;    
        ioctlsocket(PtcpFD, FIONBIO, &ul); 
        char buf[16]={0};
        err = send(PtcpFD, buf, 16, 0); //Online Test 
        if(err!=16){
            err = -1 ;  //此IP不在线，IP或Port有误
        }
        else{ err = 0 ; }
    }
    closesocket(PtcpFD);
    WSACleanup();
    return err;
}
/*
    连接UDP
    返回： 0   -   成功
            -1  -   失败
            -2  -   绑定失败
*/
int UdpConnect(char *IP, int UdpPort, int *UdpFD)
{
    //加载库
    //启动SOCKET库，版本为2.0  
    WSADATA wsdata;  
    WSAStartup(0x0202,&wsdata);  
    //然后赋值  
    sockaddr_in  serv; 
    serv.sin_family     =   AF_INET         ;  
    serv.sin_addr.s_addr=   inet_addr(IP)   ;   //INADDR_ANY ;  
    serv.sin_port       =   htons(UdpPort)  ;  
    //用UDP初始化套接字  
    *UdpFD = socket(AF_INET,SOCK_DGRAM,0); 
    if(!(*UdpFD)){ return -1; }
    // 设置该套接字为广播类型，  
    bool optval =0 ;
    int res =0;
    //res =setsockopt(*UdpFD,SOL_SOCKET,SO_REUSEADDR,(char FAR *)&optval,sizeof(optval)); 
    //时限
    int nNetTimeout=100;//m秒
    //setsockopt(PudpFD，SOL_S0CKET,SO_SNDTIMEO，(char *)&nNetTimeout,sizeof(int));
    res|= setsockopt(*UdpFD,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
    // 缓冲区
    int nBuf=100*1024;//设置为xK
    res|= setsockopt(*UdpFD,SOL_SOCKET,SO_SNDBUF,(const char*)&nBuf,sizeof(int));
    res|= setsockopt(*UdpFD,SOL_SOCKET,SO_RCVBUF,(const char*)&nBuf,sizeof(int));
    if(res){ return -1; }
    // 把该套接字绑定在一个具体的地址上  !!!!!!! 注意这里 !!!!!!!
    // 这是与SOAP框架不同的地方，也是之所以可以跨网段的原因！
    res =bind(*UdpFD,(sockaddr *)&serv,sizeof(sockaddr_in));  
    if(res==-1){
        int errorcode =::GetLastError();
        return -2;
    }
    return 0 ;
}
/*
    关闭UDP
*/
int UdpClose(int UdpFD)
{
    closesocket(UdpFD);
    WSACleanup();
    return 0 ;
}
/*
    UDP SEND
*/
int UdpSend(int UdpFD,char*msg,int len)
{
    int ret =send(UdpFD, msg, len, 0 );
    return ret ;
}
/*
    UDP SEND TO
*/
int UdpSendto(int UdpFD,char *msg, int len, char *toIP, int toPort)
{
    int tolen = sizeof(struct sockaddr_in);  
    sockaddr_in to;
    to.sin_family       =   AF_INET         ;  
    to.sin_addr.s_addr  =   inet_addr(toIP) ;   //INADDR_ANY ;  
    to.sin_port         =   htons(toPort)   ;  
    int ret =sendto(UdpFD, msg, len, 0, (const sockaddr *)&to, tolen);
    return ret ;
}
/*
    UDP RECV from
    返回： -1  -   没有关键字段
            len -   nonSoap_XAddrs 空间不足,返回当前XAddrs长度
*/
int UdpRecvfrom(int UdpFD,char *fromIP,char *buf,int size)
{
    //参数配置
    sockaddr_in  afrom ;
    afrom.sin_family        =   AF_INET         ;  
    afrom.sin_addr.s_addr   =   INADDR_BROADCAST; //inet_addr(fromIP)   ; //INADDR_BROADCAST;  
   // afrom.sin_port            =   htons(fromPort) ;  
    //接收
    int fromlength = sizeof(SOCKADDR);  
    memset(buf, 0, size);
    int res =recvfrom(UdpFD,buf,size,0,(struct sockaddr FAR *)&afrom,(int FAR *)&fromlength);  
    //如果收到的不是指定的IP，将放弃
    //如果你要发现所有设备，这里需要修改
    if( res && 
        (afrom.sin_addr.S_un.S_addr != inet_addr(fromIP)) 
       ){
        //char *afIP =inet_ntoa(afrom.sin_addr); //only for debug
        res = 0 ;
    }
    return res ;
}
/*
    得到参数，解析报文
    返回： 0   -   成功
            -1  -   失败
            >0  -   nonSoap_XAddrs空间不足
*/
int GetDiscoveryParam(char *buf,int size, char *MessageID, char *nonSoap_XAddrs,int xaddr_len)
{
    //规定搜索范围
    const char *type   = "NetworkVideoTransmitter";
    const char *Scopes = "www.onvif.org";
    //是否启用搜索范围
    int isUse_Type   = 0 ;
    int isUse_Scopes = 0 ;
    // type
    if(isUse_Type){
        char *find_type =strstr(buf, type);
        if(find_type==NULL){
            return -1 ; //不是这个类型的不必解析
        }
    }
    // Scopes
    if(isUse_Type){
        char *find_Scopes =strstr(buf, Scopes);
        if(find_Scopes==NULL){
            return -1 ; //不是这个类型的不必解析
        }
    }
    // MessageID
    char *find_uuid =strstr(buf, MessageID);
    if(find_uuid==NULL){
        return -1 ; //MessageID不相等的，不是广播给你的，不必解析
    }
    // 找到XAddrs的开始和结束处
    const char *wsddXAddrs_b = "XAddrs>";//"<wsdd:XAddrs>";
    const char *wsddXAddrs_e = "</";//"</wsdd:XAddrs>";
    char *find_XAddrs_b =strstr(buf, wsddXAddrs_b);
    char *find_XAddrs_e =strstr(find_XAddrs_b, wsddXAddrs_e);
    if(find_XAddrs_b==NULL || wsddXAddrs_e==NULL){
        return -1 ;
    }
    //取出一个XAddrs
    int len_b = strlen(wsddXAddrs_b);               //找到的XAddrs字符串头长度
    int len = find_XAddrs_e -find_XAddrs_b -len_b;  //此区间里可能包含多个XAddrs
    char *tmp = new char[len+1];
    memset(tmp, 0, len+1);
    for(int i=0;i<len;i++){                     //分离出一个XAddrs
        if(find_XAddrs_b[len_b+i]!=' '){        //多个XAddrs用空格隔开
            tmp[i] = find_XAddrs_b[len_b+i] ;   //找到的字符串，除去开头
        }
        else{
            len = i ;   //len重新赋值为单个XAddrs长度
            break;
        }
    }
    // XAddrs 如果比申请空间要长 
    if(len>xaddr_len){
        return len ;
    }
    // 得到 XAddrs 
    memset(nonSoap_XAddrs, 0, xaddr_len);
    memcpy(nonSoap_XAddrs, tmp, len);
    delete tmp ;
    return 0 ;
}
/*
    非SOAP搜索
    返回： -4  -   本地没有添加设备对应的网段
            0   -   成功
*/
int GetVideo::FindONVIFservers_nonSoap()
{
    //清除错误
    this->error = 0 ;
    memset(this->error_info_, '\0',1024);
    memset(this->error_info,  '\0',1024);
    /////////////////////////////////////////////////////////////
    //得到设备网段
    int a,b,c,d;
    char IPD[32];
    sscanf_s(this->IP,"%d.%d.%d.%d",&a,&b,&c,&d);
    sprintf_s(IPD,32,"%d.%d.%d.",a,b,c);
    //本地是否已添加当前网段
    int isAddIPD = 0 ;
    char lips[64][32];
    char *UseIP;
    int cnt = 0 ;
    GetLocalIPs(lips, 64, &cnt);
    for(int i=0;i<cnt;i++){ //历遍所有本地IP
        isAddIPD = 0 ;
        UseIP = strstr(lips[i], IPD);
        if(UseIP!=NULL){    //已添加
            isAddIPD = 1 ;
            memcpy(this->LocalIP, UseIP, 64); //得到本地IP
            break;
        }
    }
    if(isAddIPD==0){
        this->error = NOTADDIP ;
        memcpy(this->error_info_, "没有添加该网段",32);
        memcpy(this->error_info,  "请先添加该网段",32);
        return -4;  //返回没有该网段
    }
    //测试设备IP:Port是否可以连接
    int isConnecting = TcpOnlineTest(this->IP, this->Port);
    if(isConnecting==-1){
        this->error = NOCONNECT ;
        memcpy(this->error_info_, "该IP/Port无法连接，请检查是否填写正确",128);
        memcpy(this->error_info,  "ONVIF:Discovery IP/Port error\n",128);
        return -1;
    }
    //绑定本地IP，连接UDP
    int Dis_UdpFD = 0 ;
    int UsePort = this->FIND_BASE_UDP_PORT ;    //使用预定的端口，向下查找合适端口
    int res =0;
    for(int i=0;i<512;i++){ //查找一定次数
        res =UdpConnect(UseIP, UsePort, &Dis_UdpFD);
        if(res==-1){
            this->error = NOCONNECT ;
            memcpy(this->error_info_, "该IP无法创建UDP连接",64);
            memcpy(this->error_info,  "ONVIF:Discovery UDP SOCKET ERROR(-1)\n",128);
            UdpClose(Dis_UdpFD);
            return -1;
        }
        else if(res==-2){   //bind error
            UsePort+=2;     //UDP PORT 为偶数比较好
            UdpClose(Dis_UdpFD);
            Sleep(10);
        }
        else if(res==0){
            break;
        }
    }
    //如果依然绑定连接失败
    if(res==-2){
        this->error = NOCONNECT ;
        memcpy(this->error_info_, "Can't bind UDP PORT 5340-5596.(used by others now)",128);
        memcpy(this->error_info,  "ONVIF:Discovery UDP bind ERROR\n",128);
        //UdpClose(Dis_UdpFD);  //已经在上面关闭了，此处注释掉
        return -1;
    }
    /////////////////////////////////////////////////////////////////
    //准备报文
    char *cxml = {"<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope x"
                        "mlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" x"
                        "mlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" x"
                        "mlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" x"
                        "mlns:xsd=\"http://www.w3.org/2001/XMLSchema\" x"
                        "mlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" x"
                        "mlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" x"
                        "mlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" x"
                        "mlns:wsa5=\"http://www.w3.org/2005/08/addressing\" x"
                        "mlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" x"
                        "mlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" x"
                        "mlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" x"
                        "mlns:wsc=\"http://schemas.xmlsoap.org/ws/2005/02/sc\" x"
                        "mlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" x"
                        "mlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" x"
                        "mlns:xmime=\"http://tempuri.org/xmime.xsd\" x"
                        "mlns:xop=\"http://www.w3.org/2004/08/xop/include\" x"
                        "mlns:tt=\"http://www.onvif.org/ver10/schema\" x"
                        "mlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" x"
                        "mlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" x"
                        "mlns:wsrfr=\"http://docs.oasis-open.org/wsrf/r-2\" x"
                        "mlns:tad=\"http://www.onvif.org/ver10/analyticsdevice/wsdl\" x"
                        "mlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" x"
                        "mlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" x"
                        "mlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" x"
                        "mlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" x"
                        "mlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" x"
                        "mlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" x"
                        "mlns:tls=\"http://www.onvif.org/ver10/display/wsdl\" x"
                        "mlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" x"
                        "mlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" x"
                        "mlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" x"
                        "mlns:trp=\"http://www.onvif.org/ver10/replay/wsdl\" x"
                        "mlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" x"
                        "mlns:trv=\"http://www.onvif.org/ver10/receiver/wsdl\" x"
                        "mlns:tse=\"http://www.onvif.org/ver10/search/wsdl\">"
                        "<SOAP-ENV:Header><wsa:MessageID>urn:uuid:9D7A37A4-DBFE-4bdd-A79C-74998B7A375D</wsa:MessageID>"
                        "<wsa:To SOAP-ENV:mustUnderstand=\"true\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>"
                        "<wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action>"
                        "</SOAP-ENV:Header><SOAP-ENV:Body><wsdd:Probe><wsdd:Types></wsdd:Types><wsdd:Scopes></wsdd:Scopes>"
                        "</wsdd:Probe></SOAP-ENV:Body></SOAP-ENV:Envelope>" };
    // 生成GUID , Linux下叫UUID
    char MessageID[128];
    GUID guid; 
    if(CoCreateGuid(&guid)==S_OK){
        _snprintf_s(MessageID, sizeof(MessageID) 
            ,"urn:uuid:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
            , guid.Data1   , guid.Data2   , guid.Data3
            , guid.Data4[0], guid.Data4[1]
            , guid.Data4[2], guid.Data4[3], guid.Data4[4],guid.Data4[5]
            , guid.Data4[6], guid.Data4[7] );
    }
    else{
        _snprintf_s(MessageID, sizeof(MessageID),"a5e4fffc-ebb3-4e9e-913e-7eecdf0b05e8");
    }
    //跳转到关键字段
    int xml_len = strlen(cxml);
    char *xml = new char[xml_len+2] ;   //从const char 到 char
    memset(xml, 0, xml_len+2);
    memcpy(xml, cxml, xml_len);
    char *tmp_xml = strstr(xml,"urn:uuid:");
    //写入MessageID
    int idlen = strlen(MessageID);
    for(int i=0;i<idlen;i++){
        tmp_xml[i] = MessageID[i] ;
    }
    /////////////////////////////////////////////////////////////////
    //发送报文 
    res =UdpSendto(Dis_UdpFD, xml, xml_len,"239.255.255.250",3702); 
/*  if(xml_len>1500){
        res  =UdpSendto(Dis_UdpFD,  xml,      1400,         "239.255.255.250",3702);    
        res +=UdpSendto(Dis_UdpFD,  xml+1400, xml_len-1400, "239.255.255.250",3702);    
    }
    else{
        res =UdpSendto(Dis_UdpFD, xml, xml_len,"239.255.255.250",3702); 
    }
*/  if(res!=xml_len){
        this->error = -1 ;
        char tmp[128];
        sprintf_s(tmp,128,"UDP send(%d)!=res(%d) ERROR",xml_len,res);
        memcpy(this->error_info_, tmp,128);
        memcpy(this->error_info,  "ONVIF:Discovery UDP send ERROR\n",128);
        UdpClose(Dis_UdpFD);
        delete xml;
        return -1;
    }
    /////////////////////////////////////////////////////////////////
    //接收报文 
    char buf[4096] ;
    int size = 4096 ;
    //接收10次后依然没有报文，则退出
    for(int j=0,res=0; res==0 && j<100; j++){
        //memset(buf, 0, 4096); //2016年11月11日
        res =UdpRecvfrom(Dis_UdpFD, this->IP, buf+res, size-res);
        Sleep(10);
    }
    //如果没有收到，再次接收
    //if(res==-1){  
    //  for(res=0; res!=0; ){
    //      memset(buf, 0, 4096);
    //      res =UdpRecvfrom(Dis_UdpFD, this->IP, buf, size);
    //      Sleep(10);
    //  }
    //}
    //再次没有收到，认为出错了
    if(res==-1){    
        this->error = -1 ;
        char tmp[128];
        sprintf_s(tmp,128,"UDP recv(%d) ERROR",res);
        memcpy(this->error_info_, tmp,128);
        memcpy(this->error_info,  "ONVIF:Discovery UDP recv ERROR\n",128);
        UdpClose(Dis_UdpFD);
        delete xml;
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////
    //解释报文
    int resGDP =GetDiscoveryParam(buf, res, MessageID, this->XAddrs, 256);
    //XAddrs空间不足
    if(resGDP>0){
        this->error = -1 ;
        char tmp[128];
        sprintf_s(tmp,128,"GetVideo: XAddrs buf overflow(256<%d) ERROR",res);
        memcpy(this->error_info_, tmp,128);
        memcpy(this->error_info,  "GetVideo: 请增大XAddrs空间\n",128);
        UdpClose(Dis_UdpFD);
        delete xml;
        return -1;
    }
    //没有发现此设备
    if(resGDP==-1){
        this->error = NO_DEVICE ;
        memcpy(this->error_info_, "ONVIF:  this device NOT found\n", 128);
        memcpy(this->error_info,  "ONVIF:non Soap Discovery NO Device\n",128);
        UdpClose(Dis_UdpFD);
        delete xml;
        return -1;
    }
    // 关闭
    UdpClose(Dis_UdpFD);
    delete xml;
    return 0 ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
- 316
- 317
- 318
- 319
- 320
- 321
- 322
- 323
- 324
- 325
- 326
- 327
- 328
- 329
- 330
- 331
- 332
- 333
- 334
- 335
- 336
- 337
- 338
- 339
- 340
- 341
- 342
- 343
- 344
- 345
- 346
- 347
- 348
- 349
- 350
- 351
- 352
- 353
- 354
- 355
- 356
- 357
- 358
- 359
- 360
- 361
- 362
- 363
- 364
- 365
- 366
- 367
- 368
- 369
- 370
- 371
- 372
- 373
- 374
- 375
- 376
- 377
- 378
- 379
- 380
- 381
- 382
- 383
- 384
- 385
- 386
- 387
- 388
- 389
- 390
- 391
- 392
- 393
- 394
- 395
- 396
- 397
- 398
- 399
- 400
- 401
- 402
- 403
- 404
- 405
- 406
- 407
- 408
- 409
- 410
- 411
- 412
- 413
- 414
- 415
- 416
- 417
- 418
- 419
- 420
- 421
- 422
- 423
- 424
- 425
- 426
- 427
- 428
- 429
- 430
- 431
- 432
- 433
- 434
- 435
- 436
- 437
- 438
- 439
- 440
- 441
- 442
- 443
- 444
- 445
- 446
- 447
- 448
- 449
- 450
- 451
- 452
- 453
- 454
- 455
- 456
- 457
- 458
- 459
- 460
- 461
- 462
- 463
- 464
- 465
- 466
- 467
- 468
- 469
- 470
- 471
- 472
- 473
- 474
- 475
- 476
- 477
- 478
- 479
- 480
- 481
- 482
- 483
- 484
- 485
- 486
- 487
- 488
- 489
- 490
- 491
- 492
- 493
- 494
- 495
- 496
- 497
- 498
- 499
- 500
- 501
- 502
- 503
- 504
- 505
- 506
- 507
- 508
- 509
- 510
- 511
- 512
- 513
- 514
- 515
- 516
- 517
- 518
- 519
- 520
- 521
- 522
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
- 316
- 317
- 318
- 319
- 320
- 321
- 322
- 323
- 324
- 325
- 326
- 327
- 328
- 329
- 330
- 331
- 332
- 333
- 334
- 335
- 336
- 337
- 338
- 339
- 340
- 341
- 342
- 343
- 344
- 345
- 346
- 347
- 348
- 349
- 350
- 351
- 352
- 353
- 354
- 355
- 356
- 357
- 358
- 359
- 360
- 361
- 362
- 363
- 364
- 365
- 366
- 367
- 368
- 369
- 370
- 371
- 372
- 373
- 374
- 375
- 376
- 377
- 378
- 379
- 380
- 381
- 382
- 383
- 384
- 385
- 386
- 387
- 388
- 389
- 390
- 391
- 392
- 393
- 394
- 395
- 396
- 397
- 398
- 399
- 400
- 401
- 402
- 403
- 404
- 405
- 406
- 407
- 408
- 409
- 410
- 411
- 412
- 413
- 414
- 415
- 416
- 417
- 418
- 419
- 420
- 421
- 422
- 423
- 424
- 425
- 426
- 427
- 428
- 429
- 430
- 431
- 432
- 433
- 434
- 435
- 436
- 437
- 438
- 439
- 440
- 441
- 442
- 443
- 444
- 445
- 446
- 447
- 448
- 449
- 450
- 451
- 452
- 453
- 454
- 455
- 456
- 457
- 458
- 459
- 460
- 461
- 462
- 463
- 464
- 465
- 466
- 467
- 468
- 469
- 470
- 471
- 472
- 473
- 474
- 475
- 476
- 477
- 478
- 479
- 480
- 481
- 482
- 483
- 484
- 485
- 486
- 487
- 488
- 489
- 490
- 491
- 492
- 493
- 494
- 495
- 496
- 497
- 498
- 499
- 500
- 501
- 502
- 503
- 504
- 505
- 506
- 507
- 508
- 509
- 510
- 511
- 512
- 513
- 514
- 515
- 516
- 517
- 518
- 519
- 520
- 521
- 522
```
其实设备发现，就是要得到一个XAddrs。同时看一下设备是否在线。如果你确认设备在线，并已知XAddrs，那不用发现这一步了。直接可以进行下面的操作。
```
## ONVIF获取设备能力
```
先看代码
```
```cpp
/*
    获取能力
*/
void UserGetCapabilities(   struct soap *soap   ,
        struct __wsdd__ProbeMatches *resp,
        class _tds__GetCapabilities *capa_req,
        class _tds__GetCapabilitiesResponse *capa_resp,
        char *ONVIF_USER,
        char *ONVIF_PASSWORD)
{
    //申请空间
    capa_req->Category = (enum tt__CapabilityCategory *)soap_malloc(soap, sizeof(int));
    capa_req->__sizeCategory = 1;
    capa_resp->Capabilities = (class tt__Capabilities*)soap_malloc(soap,sizeof(class tt__Capabilities)) ;
    //选择要获取的设备能力
    *(capa_req->Category) = (enum tt__CapabilityCategory)(tt__CapabilityCategory__All);
    //身份认证，鉴权
    if(soap->error){ return; }
    COnvifDigest cTokenDigest(ONVIF_USER, ONVIF_PASSWORD);
    cTokenDigest.Authentication(soap,TRUE,"user");
    //获取设备能力
    if(soap->error){ return; }
    int result = soap_call___tds__GetCapabilities(soap, resp->wsdd__ProbeMatches->ProbeMatch->XAddrs, NULL, capa_req, capa_resp);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
```
小白都会问：这段代码怎么得来的呢？请看解析。
    设备发现之后是能力获取，这可以从ONVIF官方资料得知。
    能力获取是用GetCapabilities()函数，这可以从ONVIF官网的操作说明看到，也就是上那个D网址。 
    那怎么知道soap的前缀呢，soap_call___tds__XXX也不是乱加的。最保守是在soapClient.cpp里Ctrl+F查找GetCapabilities关键字，就可以找到相应的函数。到你熟悉之后，你会发现tds是GetCapabilities()的命名空间，其他函数有其他的命名空间，代替一下就可以。
    关于函数调用之前，输入输出变量是什么意思，可以看ONVIF官网的函数说明。
```
## ONVIF鉴权
```
鉴权可以用函数：
```
`soap_wsse_add_UsernameTokenDigest(soap,"user", ONVIF_USER, ONVIF_PASSWORD); `- 1
- 1
```
这个函数要用到openssl库，要自己编译加进去，有点麻烦。
    这里的鉴权也就是用户名、密码、随机码/时间码的base64加密，自己也可能实现。所以我选择了不用openssl库的方法。推荐链接：
http://blog.csdn.net/pony_maggie/article/details/8588888
http://blog.csdn.net/qiaowei0/article/details/42024703
    (后面的链接，一开始可以用，后来设备多了情况复杂了就似乎有点问题，我修改过，但忘记改了哪里，代码就不放上来了 so sorry)
```
### ONVIF流程
```
可以实现能力获取这一步，就已经可以举一反三完成所有的ONVIF操作了。下面看看整体流程：
    发现设备 >> 获取能力 >> 获取媒体信息 >> 获取视频编码配置 >> 设置视频编码配置 >> 获取URI >> ONVIF完成 ->  RTSP播放 -> 解码
```
# RTSP/RTP协议实现
#### 增加准备资料，推荐以下链接：
```
http://blog.csdn.net/chenyanxu/article/details/2728427
http://www.cnblogs.com/lidabo/p/3701068.html
http://blog.csdn.net/baby313/article/details/7353605
http://www.faqs.org/rfcs/
http://blog.csdn.net/gavinr/article/details/7035966
http://blog.csdn.net/nkmnkm/article/category/1066093/2
http://blog.csdn.net/cjj198561/article/details/30248963
https://my.oschina.net/u/1431835/blog/393315
都是关于RTSP及RTP包及H264等相关资料
```
## RTSP通信
```
理论自已看上面的链接，不多说，直接上代码。
```
```cpp
int C_RTSP_SendCMD::OPTIONS()
{
    //初始化
    memset(Authorization_Basic, '\0', 128); //用于鉴权
    //得到命令
    char *cmd = new char[1024];
    memset(cmd, 0, 1024);
    sprintf_s(cmd,1024, "OPTIONS %s RTSP/1.0\r\n"
                        "CSeq:1\r\n"
                        "User-Agent:HaoYun"
                        "\r\n"
                        "\r\n", C_RTSP_SendCMD::URI );
    //发送
    int sLenth = strlen(cmd);
    int res = send(tcpFD, cmd, sLenth, 0);
    delete cmd ;
    return res ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
```
这里用到变量是：URI由ONVIF获取的。tcpFD是socket值。由LocalIP与设备建立的一个tcp链接，注意RTSP协议规定端口为554(有些也可能用8554)。Authorization_Basic是关于用户名和密码的一个basic64加密认证。 
    这里要注意的是命令里面不要有多余的空格和多余的转义，否则不行。
    发送完命令后，马上接收服务器返回的数据，然后就可以看到资料里所说的过程了。例如：
        OPTIONS rtsp://192.168.20.136:5000/xxx666  RTSP/1.0 
        CSeq: 1 //每个消息都有序号来标记，第一个包通常是option请求消息 
        User-Agent: HaoYun
    服务器的回应信息包括提供的一些方法,例如:
        RTSP/1.0 200 OK 
        Server: HaoYun IPC
        Cseq: 1 //每个回应消息的cseq数值和请求消息的cseq相对应 
        Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE, GET_PARAMETER   //服务器提供的可用的方法
```
### 再来一个要认证身份的例子
```
看下面代码：
```
```cpp
int C_RTSP_SendCMD::DESCRIBE()
{
    //得到命令
    char *cmd = new char[1024];
    memset(cmd, 0, 1024);
    sprintf_s(cmd,1024, "DESCRIBE %s RTSP/1.0\r\n"
                        "CSeq:2\r\n"
                        "Accept: application/sdp\r\n"
                        "User-Agent:HaoYun"
                        "\r\n"
                        "\r\n", C_RTSP_SendCMD::URI );
    //要认证
    if(this->Authorization_Basic[1]!='\0'){
        char *cmd_tmp = strstr(cmd, "HaoYun");
        sprintf_s(cmd_tmp,512,  "HaoYun\r\n"
                                "Authorization: Basic %s"
                                "\r\n"
                                "\r\n", this->Authorization_Basic );
    }//end if
    //发送
    int sLenth = strlen(cmd);
    int res = send(tcpFD, cmd, sLenth, 0);
    delete cmd ;
    return res ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
```
这里的Authorization_Basic是如下代码算出的。其中base64_bits_to_64()网上好多都有就不复制上来了。SOAP里也有，在那个鉴权函数里面，修改一下soap_malloc()就可以用了。
```
```cpp
int C_RTSP_SendCMD::Calc_Authorization_Basic(char *user, char *passwd)
{
    unsigned char *user_passwd = new unsigned char[600];
    memset(user_passwd, '\0', 600);
    //合并两个
    int u_len = strlen(user);
    int p_len = strlen(passwd);
    memcpy(user_passwd,         user,   u_len);
    *(user_passwd+u_len) = ':';
    memcpy(user_passwd+u_len+1, passwd, p_len);
    //base64转码
    base64_bits_to_64((unsigned char *)this->Authorization_Basic, user_passwd, u_len+p_len+1);
    delete user_passwd;
    return 0 ;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
```
1.接收到DESCRIBE的sdp报文后，就在sdp里解析出 control 和 transport 来。
2.接着就是用上面得到的信息进行SETUP
3.接收到SETUP的返回信息后，解析出Session，每个通话都有一个固定的Session。
4.用Session值发送PLAY命令，成功的话，就有视频流了。这时候的流是RTP包(如果用RTP协议的话)。
5.还有一个关闭的命令TEARDOWN，在关闭的时候调用。
```
## RTP协议实现接收
```
看完上面RTSP的链接后，就可以看下面的代码了。(如果一头雾水，请对照着那两个RTP网址来看，或对照着RTP协议也可以)
    理论不说，分析代码去。
    其中两个结构体是关于RTP头的，GetHeader()就是解析填充这两个头，代码和协议有一一对应的关系。而GetNALUnit_OneFrame()是从视频流中把一个个小RTP包拆解成NAL包，再一个个的合并成一个大帧，合并得到的帧可以送到解码器解码了。有些解码器也许不需要合并帧这一步，直接把NAL流送过去就可以解码的。
```
```
//////////////////////////////////////////////////////////////////////////////////////////  
// 2 class CH264_RTP_UNPACK_2 start  
class CH264_RTP_UNPACK_2
{
#define RTP_VERSION     2  
#define BUF_SIZE        (1024 * 500)  
    typedef struct   
    {  
        //LITTLE_ENDIAN  
        unsigned char   CC;      /* 4 CSRC count                 */  
        unsigned char   X;       /* 1 header extension flag      */  
        unsigned char   P;       /* 1 padding flag               */  
        unsigned char   V;       /* 2 packet type                */  
        unsigned char   PT;      /* 7 payload type               */  
        unsigned char   M;       /* 1 marker bit                 */  
        unsigned short   seq_number;        /* 16 sequence number            */  
        unsigned int     time_stamp;        /* 32 timestamp                  */  
        unsigned int     SSRC;              /* 32 synchronization source     */ 
        unsigned int     size;
    } RTP_Header_t; 
    typedef struct 
    {
        unsigned char   Type;       // 5     
        unsigned char   NRI;        // 2    
        unsigned char   F;          // 1 在 H.264 规范中规定了这一位必须为 0
        unsigned char   FUA_Type;   // 5 
        unsigned char   FUA_R;      // 1
        unsigned char   FUA_E;      // 1 置1 NAL单元的结束
        unsigned char   FUA_S;      // 1 置1 NAL单元的开始
        unsigned char   isMajor578;
        unsigned int    size;
    } NAL_Header_t; 
public:
    RTP_Header_t  m_RTP_Header;
    NAL_Header_t  m_NAL_Header;
    unsigned char *H264_OneFrame;
    int H264_1f_size;
    int pic_size3;
public:
    CH264_RTP_UNPACK_2()
    {
    }
    ~CH264_RTP_UNPACK_2()
    {
        delete H264_OneFrame ;
    }
    void CH264_RTP_UNPACK_2_Init()
    {
        memset(&m_RTP_Header, 0, sizeof(RTP_Header_t));
        memset(&m_NAL_Header, 0, sizeof(NAL_Header_t));
        H264_OneFrame = new unsigned char[pic_size3];
        memset(H264_OneFrame, 0, pic_size3);
        H264_1f_size = 0 ;
    }
    // 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 
    //*-+-+-+-+-+-+-+-+*+-+-+-+-+-+-+-+*+-+-+-+-+-+-+-+*+-+-+-+-+-+-+-+
    //|V=2|P|X|  CC   |M|     PT      |       sequence number         |
    //|                         timestamp                             |
    //|                synchronization source(SSRC)                   |
    //+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    //                      ..........
    int GetHeader(unsigned char *RTP_PackegRawBuf,int size)
    {
        if(size<13){ return -2; }   //包头不会小于13的
        unsigned int ba,bb,bc,bd;   //4个字节中间变量
        m_RTP_Header.V  =  RTP_PackegRawBuf[0] >>6 ;
        m_RTP_Header.P  = (RTP_PackegRawBuf[0] &0x20)? 1:0 ;
        m_RTP_Header.X  = (RTP_PackegRawBuf[0] &0x10)? 1:0 ;
        m_RTP_Header.CC =  RTP_PackegRawBuf[0] &0x0f ;
        m_RTP_Header.M  = RTP_PackegRawBuf[1] >>7 ;
        m_RTP_Header.PT = RTP_PackegRawBuf[1] &0x7f ;
        ba = RTP_PackegRawBuf[2] ;
        bb = RTP_PackegRawBuf[3] ;
        m_RTP_Header.seq_number = (ba<<8) + bb ;
        ba = RTP_PackegRawBuf[4] ;
        bb = RTP_PackegRawBuf[5] ;
        bc = RTP_PackegRawBuf[6] ;
        bd = RTP_PackegRawBuf[7] ;
        m_RTP_Header.time_stamp = (ba<<24) +(bb<<16) +(bc<<8) +bd ;
        ba = RTP_PackegRawBuf[ 8] ;
        bb = RTP_PackegRawBuf[ 9] ;
        bc = RTP_PackegRawBuf[10] ;
        bd = RTP_PackegRawBuf[11] ;
        m_RTP_Header.SSRC = (ba<<24) +(bb<<16) +(bc<<8) +bd ;
        m_RTP_Header.size = 12 + m_RTP_Header.CC ;
        //NAL包
        //RTP版本不同
        if(m_RTP_Header.V!=RTP_VERSION){    
            return -2;
        }
        //存在特殊报头,此处要加入特殊处理,此版本不支持固直接返回
        if(m_RTP_Header.X){
            return -2;
        }
        //《RFC3551 RTP A/V Profile》中定义动态标识区（一般96标识H264）
        //if(m_RTP_Header.PT<96 || m_RTP_Header.PT>127){
        //(!!!!! 此处假定96(0X60)标识H264 并仅对H264进行处理 !!!!!)
        if(m_RTP_Header.PT != 96){
            return -2;
        }
        //提取H264 NAL头
        int nal_h_offset  = 12+m_RTP_Header.CC ;
        m_NAL_Header.F    =  RTP_PackegRawBuf[nal_h_offset] >>7;
        m_NAL_Header.NRI  = (RTP_PackegRawBuf[nal_h_offset] &0x60)>>5;
        m_NAL_Header.Type =  RTP_PackegRawBuf[nal_h_offset] &0x1f;
        //重要类型为 5,7,8,1? (25,27,28,65,67,68,21?...)
        m_NAL_Header.isMajor578 = 0 ;
        if(m_NAL_Header.Type==5 || m_NAL_Header.Type==7 || 
           m_NAL_Header.Type==8 || m_NAL_Header.Type==1 )
        {
            m_NAL_Header.isMajor578 = 1 ; 
        }
        //此处仅处理多片FU-A(28)及单片的情况,其他情况直接返回
        if(m_NAL_Header.Type>23 && m_NAL_Header.Type!=28){
            return -2;
        }
        // 分片 FU-A 
        if(m_NAL_Header.Type==28){
            m_NAL_Header.FUA_S    =  RTP_PackegRawBuf[nal_h_offset+1] >>7 ;
            m_NAL_Header.FUA_E    = (RTP_PackegRawBuf[nal_h_offset+1] &0x40)>>6;
            m_NAL_Header.FUA_R    = 0 ;
            m_NAL_Header.FUA_Type =  RTP_PackegRawBuf[nal_h_offset+1] &0x1f ;
            m_NAL_Header.size     = 2 ;
        }
        else{ //单片
            m_NAL_Header.FUA_S    = 0 ;
            m_NAL_Header.FUA_E    = 0 ;
            m_NAL_Header.FUA_R    = 0 ;
            m_NAL_Header.FUA_Type = 0 ;
            m_NAL_Header.size     = 1 ;
        }
        return 0;
    }
    //此函数未测试 - 2016年11月11日
    int CheckLostMajorPackeg(float *fLossRate)
    {
        bool isLost =0;
        static bool isFrame_loss[256] = {0} ;
        static unsigned char i = 0 ;
        static unsigned short seq_number_old = 0;
        //重要类型为 5,7,8 (25,27,28,65,67,68,...)
        bool isMajor = 0 ;
        unsigned char Type[2];
        Type[0] = m_NAL_Header.Type &0x0f ;
        Type[1] = m_NAL_Header.FUA_Type & 0x0f ;
        //判断是否有5 7 8 
        for(int j=0;j<2 && isMajor==0;j++){
            switch(Type[j]){
                case 5:
                case 7:
                case 8: isMajor = 1 ; break;
                default: isMajor = 0 ;
            }
        }
        if(isMajor==0){ 
            //更新数据
            seq_number_old = m_RTP_Header.seq_number ;
            return 0; 
        }
        //如果是重要帧
        //序列号应该递增
        isLost = 1 ;
        if(m_RTP_Header.seq_number-seq_number_old == 1){
            isLost = 0 ;
        }
        if(seq_number_old==0xffff && m_RTP_Header.seq_number==0x0000){
            isLost = 0 ;
        }
        //计算丢包率
        float lossnum = 0.0 ;
        isFrame_loss[i++] = isLost ;
        for(int j=0;j<256;j++){
            if(isFrame_loss[j])
                lossnum += 1.0 ;
        }
        *fLossRate = lossnum/256.0f ;
        //更新数据
        seq_number_old = m_RTP_Header.seq_number ;
        return (int)isLost;
    }
    int GetNALUnit_OneFrame(unsigned char *RTP_PackegRawBuf,int size)
    {
        int reValue = 0 ;
        float fLossRate =0.0;
        static int isLossPackeg = 0 ;
        //参数断定
        if(RTP_PackegRawBuf==NULL || size<13){return -1;}
        GetHeader(RTP_PackegRawBuf,size);
        /*if(!isLossPackeg && CheckLostMajorPackeg(&fLossRate)){
            isLossPackeg = 1 ;
            printf("loss packeg: LossRate=%f ... \n",fLossRate);
        }
        //如果丢包，丢掉这一帧
        if(isLossPackeg){
            memset(H264_OneFrame, 0, H264_1f_size);
            H264_1f_size = 0 ;
            if(m_RTP_Header.M){ //如果帧结束，清零标记位
                isLossPackeg = 0 ;
            }
            return -1;
        }
        */
        //去壳、去头去尾
        unsigned char *playload = RTP_PackegRawBuf ;
        int playload_size = 0 ;
        int padding_size = 0 ;
        if(m_RTP_Header.P){  //尾部有多余数据
            padding_size = RTP_PackegRawBuf[size-1] ;
        }
        //数据大小及数据地址重新指向有效数据区域
        playload_size = size -(m_RTP_Header.size+m_NAL_Header.size) -padding_size +1;
        playload += m_RTP_Header.size + m_NAL_Header.size -1;
        //单片
        if(m_NAL_Header.size==1 && !m_NAL_Header.isMajor578){
            playload -= 3 ;
            playload_size += 3 ;
            playload[0] =0x00; playload[1] =0x00; //写入头
            playload[2] =0x01; 
        }
        else if(m_NAL_Header.size==1 && m_NAL_Header.isMajor578){
            playload -= 4 ;
            playload_size += 4 ;
            playload[0] =0x00; playload[1] =0x00; 
            playload[2] =0x00; playload[3] =0x01;
        }
        else if(m_NAL_Header.size==2){  //多片
            if(m_NAL_Header.FUA_S){     //分片开始
                playload -= 4 ;
                playload_size += 4 ;
                playload[4] = (playload[3]&0xe0) + (playload[4]&0x1f);//取FU indicator的前三位和FU Header的后五位为NAL类型。
                playload[0] =0x00; playload[1] =0x00; //写入头
                playload[2] =0x00; playload[3] =0x01;
            }
            else{
                playload += 1 ;         //除去 FU Header
                playload_size -= 1 ;
            }
        }
        //合并成一帧
        memcpy(H264_OneFrame+H264_1f_size, playload, playload_size);
        H264_1f_size += playload_size ;
        //帧结束标志
        if(m_RTP_Header.M){
            if(H264_1f_size<pic_size3){ //size大小是否异常
                reValue = 1 ;
            }
            else{   //出错则清零
                memset(H264_OneFrame, 0, pic_size3);
                H264_1f_size = 0 ;
            }
            //以下仅用于文件测试
            //得到的视频文件可以用播放器打开
            //static int i=0;
            //printf("running %d... \n",i++);
/*          FILE *fp = fopen("test.264","ab+");
            int a = fwrite(H264_OneFrame,1,H264_1f_size,fp);
            if(a!=H264_1f_size){
                printf("write file test.264 ERROR ! \n");
            }
            fflush(fp);
            fclose(fp);
*/          
            //memset(H264_OneFrame, 0, H264_1f_size);
            //H264_1f_size = 0 ;
            //以上仅用于测试
        }// end if(m_RTP_Header.M)
        return reValue ;
    }
};
// 2 class CH264_RTP_UNPACK_2 end  
//////////////////////////////////////////////////////////////////////////////////////////
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
## FFMPEG解码实现
```
终于到最后一步了。请编译ffmpeg与x264。网上虽然很多资料，但大多不行，要自己综合各方资料。请看上面的推荐链接。也许编译这一步会有点麻烦，耐心一点总可以解决。
```
### 关于FFMPEG多线程问题：
```
这里注意多线程问题，如果你用FFMPEG解多路视频的话，编译的时候请--enable-w32thread. 否则不好说，网上很多人都说多线程会出现问题，但也有人没有问题。而且就算你enable了多线程，里面有些函数也是不支持多线程的。比如说：
```
```java
/**
 ......
 * @warning This function is not thread safe!
 *
 * @note Always call this function before using decoding routines (such as
 .......
 */
int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
## 关于FFMPGE的例子：
```
官方的资料里有PLAYER的例子，还有test文件夹下的decode例子。推荐一个文件api-h264-test.cpp，这个参考性很高。
    民间例子更多了，比如《最简单的基于FFmpeg的视频播放器 Simplest FFmpeg Player》雷霄骅。
    下面看一个由api-h264-test.cpp更装过来的函数。
```
```
/*
    仅作测试
    功能：打开一个filepath文件并解码
*/
int C_FFMPEG_Decode::onlyforfun_and_test()
{
    AVFormatContext *pFormatCtx;
    int             i, videoindex;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    char filepath[]="test.264";
    //char filepath[]="rtsp://192.168.70.231/majorvideo_and_audio";
    //char filepath[]="rtsp://192.168.70.179/majorvideo_and_audio";
    av_register_all();
    //avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        printf("Couldn't open input stream.（无法打开输入流）\n");
        return -1;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.（无法获取流信息）\n");
        return -1;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++) 
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        printf("Didn't find a video stream.（没有找到视频流）\n");
        return -1;
    }
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL)
    {
        printf("Codec not found.（没有找到解码器）\n");
        return -1;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
    {
        printf("Could not open codec.（无法打开解码器）\n");
        return -1;
    }
    //初始化参数，下面的参数应该由具体的业务决定  
    //pCodec    = avcodec_find_decoder(CODEC_ID_H264);  
    //pCodecCtx = avcodec_alloc_context3(pCodec); 
    //avcodec_open2(pCodecCtx, pCodec, NULL);
    pCodecCtx->time_base.num    = 1         ;  
    pCodecCtx->frame_number     = 1         ; //每包一个视频帧  
    pCodecCtx->codec_type       = AVMEDIA_TYPE_VIDEO;  
    pCodecCtx->bit_rate         = 0         ;  
    pCodecCtx->time_base.den    = 25        ; //帧率  
    pCodecCtx->width            = 384       ; //视频宽  
    pCodecCtx->height           = 288       ; //视频高  
    AVFrame *pFrame,*pFrameYUV;
    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV444P, pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV444P, pCodecCtx->width, pCodecCtx->height);
    int ret, got_picture;
    AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    //输出一下信息-----------------------------
    printf("File Information（文件信息）---------------------\n");
    av_dump_format(pFormatCtx,0,filepath,0);
    printf("-------------------------------------------------\n");
    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV444P, SWS_BICUBIC, NULL, NULL, NULL); 
    //--------------
    //Event Loop
    for (;;) {
        //Wait
        if(1){
            //------------------------------
            Sleep(40);
            if(av_read_frame(pFormatCtx, packet)>=0){
                if(packet->stream_index==videoindex){
                    ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
                    if(ret < 0){
                        printf("Decode Error.（解码错误）\n");
                        return -1;
                    }
                    if(got_picture){
                        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
                        /////
                        int rows = pCodecCtx->height ;
                        int cols = pCodecCtx->width ;
                        if(rows && cols){
                            /* 请加入opencv
/*                          Mat imgBGR = Mat(rows,cols,CV_8UC3);
                            Mat img[3];
                            img[0] = Mat(rows,cols,CV_8UC1);
                            img[1] = Mat(rows,cols,CV_8UC1);
                            img[2] = Mat(rows,cols,CV_8UC1);
                            int size = rows*cols;
                            memcpy(img[0].data, pFrameYUV->data[0], size);
                            memcpy(img[1].data, pFrameYUV->data[1], size);
                            memcpy(img[2].data, pFrameYUV->data[2], size);
                            merge(img,3,imgBGR);
                            cvtColor(imgBGR,imgBGR,CV_YUV2BGR);
                            imshow("opencv",imgBGR);
                            waitKey(1);
*/                      }
                        ///////
                    }
                }
                av_free_packet(packet);
            }else{
                //Exit Thread
                break;
            }
        }
    }
    sws_freeContext(img_convert_ctx);
    //--------------
    av_free(out_buffer);
    av_free(pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
```
这里需要注意的是，视频的宽、高、帧率、输入格式、输出格式等。
    细心者可以看到，这里可以直接打开URL。是的，但是不能打开所有URL。如果自己实现RTSP协议，则可以打开足够多类型的URL。
    如果是打开URL，那接收buffer要足够大，不然会花屏、卡、跳帧等。
```
如果自己实现RTSP/RTP，那么就要自己填充AVPacket了(AVPacket是FFMPEG中一个为数不多的重要结构体！)。参考代码如下：
```
//申请输入码流包空间
    //packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    packet = av_packet_alloc();
    av_init_packet(packet);
    packet->data = (uint8_t *)av_malloc(width*height*3+64); //编码包数据空间(不可能比原图更大了)
    memset(packet->data, 0, width*height*3+64);             //加64是因为有些CPU是每次读32/64位
    packet->size = 0 ;  //未有数据设为0
.....
packet->size = nal_size ;
memcpy(packet->data, packet->size,nal_data);
......
avcodec_decode_video2(...)
.....
memset(packet->data, 0, width*height*3+64);
packet->size = 0 ;
......
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
看看总体效果图 
![这里写图片描述](https://img-blog.csdn.net/20161111170212807)
![这里写图片描述](https://img-blog.csdn.net/20161111170236682)
[转载地址](http://blog.csdn.net/Newyan3651/article/details/53130265)
