# 使用Onvif协议进行设备发现以及指定设备信息探测 - xcyl的口袋 - CSDN博客
2017年08月12日 01:26:35[xcyl](https://me.csdn.net/cai6811376)阅读数：4366
关于EasyNVR，可以移步[基于EasyNVR实现RTSP/Onvif监控摄像头Web无插件化直播监控](http://blog.csdn.net/xiejiashu/article/details/73401647)
EasyNVR支持使用onvif协议对设备进行设备发现，云台控制，onvif地址获取等功能
这里第一篇我们来说一下关于如何进行设备发现
## 准备
首先，我们要了解Onvif协议，[https://www.onvif.org/](https://www.onvif.org/)
这里我们之探讨如何使用gsoap进行onvif客户端的开发，关于gsoap的onvif框架生成请自行google
Onvif协议的功能有很多，这里我们之探讨安防监控直播常用的几种功能
## 原理
Onvif协议要求实现Onvif协议服务的设备支持设备发现以及探测，也就是discovery和probe
discovery，客户端向网段内的**组播地址为239.255.255.250，端口为3702**发送广播消息，等待网内Onvif协议设备响应
响应消息的设备会返回自己的IP、UUID、EP Address、Type等，最重要的会返回设备的Device Service Address，格式如: [http://192.168.1.2:8080/onvif/devices](http://192.168.1.2:8080/onvif/devices) ，此地址就是为设备提供Onvif服务的服务地址，我们需要使用此地址进行其他的操作
使用gsoap进行discover devices
```cpp
SOAP_ENV__Header header;
soap_default_SOAP_ENV__Header(soap_, &header);
header.wsa__MessageID = const_cast<char*>(soap_wsa_rand_uuid(soap_));
header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
soap_->header = &header;
wsdd__ScopesType scopesType;
soap_default_wsdd__ScopesType(soap_, &scopesType);
scopesType.__item = "";
wsdd__ProbeType probeType;
soap_default_wsdd__ProbeType(soap_, &probeType);
probeType.Scopes = &scopesType;
probeType.Types = "";
__wsdd__ProbeMatches probeMatches;
int result = soap_send___wsdd__Probe(soap_, "soap.udp://239.255.255.250:3702", NULL, &probeType);
if (result == SOAP_OK)
{
    do
    {
        result = soap_recv___wsdd__ProbeMatches(soap_, &probeMatches);
        if (result == SOAP_OK)
        {
            //处理接收到的响应消息，提取设备信息
            //这里我们通常需要IP、Device Service Address
            //IP: soap_->ip
            //Address: probeMatches.wsdd__ProbeMatches->ProbeMatch->XAddrs 
        }
        else
        {
            break;
        }
    } while (true);
}
```
## 想要更多
上面的情况我们可以发现到统一网段内的设备，或者所可以广播到的设备，但是，一旦我们出现了跨网段无法广播到的设备，我们如何进行设备信息获取呢？
首先，我们需要知道这个设备的IP地址，否则还是无从下手，如果你不知道，无力继续…..
在知道设备的IP地址时，同理，我们可以向此设备进行探测，也就是我们说的probe
**目前能查到的说明了Onvif如何获取指定设备信息的，估计也只有我这一篇了**
```cpp
SOAP_ENV__Header header;
soap_default_SOAP_ENV__Header(soap_, &header);
header.wsa__MessageID = const_cast<char*>(soap_wsa_rand_uuid(soap_));
header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
soap_->header = &header;
wsdd__ScopesType scopesType;
soap_default_wsdd__ScopesType(soap_, &scopesType);
scopesType.__item = "";
wsdd__ProbeType probeType;
soap_default_wsdd__ProbeType(soap_, &probeType);
probeType.Scopes = &scopesType;
probeType.Types = "";
__wsdd__ProbeMatches probeMatches;
std::string soapAddress(Format("soap.udp://%s:3702", host));
if (soap_send___wsdd__Probe(soap_, soapAddress.c_str(), NULL, &probeType) == SOAP_OK)
{
    if (soap_recv___wsdd__ProbeMatches(soap_, &probeMatches) == SOAP_OK)
        //Address: probeMatches.wsdd__ProbeMatches->ProbeMatch->XAddrs
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
