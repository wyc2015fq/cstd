# 使用Onvif协议进行设备RTSP地址获取 - xcyl的口袋 - CSDN博客
2017年08月12日 10:48:56[xcyl](https://me.csdn.net/cai6811376)阅读数：1366
上一篇我们说到如何发现网内Onvif设备，还有如何探测特定设备的信息[EasyNVR-Onvif(一)使用Onvif协议进行设备发现以及指定设备信息探测](http://blog.csdn.net/cai6811376/article/details/77104704)
这里我们说如何进行很重要的信息获取，就是设备的流地址，也就是设备的RTSP地址
## 流程
要获取设备的RTSP地址，我们首先需要获取到设备的Device Service Address，也就是上一篇得到的
如果需要获取更进一步的信息，我们需要知道此设备的用户名密码，不知道是无法进行下去的
**获取设备能力Capabilities**
获取设备的能力，并且可以获取到Media和PTZ的URL，使用这两个URL可以进行流和PTZ操作
```
_tds__GetCapabilities* capabilities = soap_new__tds__GetCapabilities(proxyDevice, -1);
tt__CapabilityCategory* category = new tt__CapabilityCategory;
capabilities->Category = category;
*(capabilities->Category) = tt__CapabilityCategory__All;
_tds__GetCapabilitiesResponse* response = soap_new__tds__GetCapabilitiesResponse(proxyDevice, -1);
if (SOAP_OK == proxyDevice->GetCapabilities(capabilities, response))
{
    if (response->Capabilities)
    {
        //Media URI: response->Capabilities->Media->XAddr;
        //PTZ URI: response->Capabilities->PTZ->XAddr;
    }
}
```
**获取Media Profile**
使用上一步获取到的Media URI获取设备的Media Profile
```
_trt__GetProfiles* getProfiles = soap_new__trt__GetProfiles(proxyMedia, -1);
_trt__GetProfilesResponse* response = soap_new__trt__GetProfilesResponse(proxyMedia, -1);
if (SOAP_OK == proxyMedia->GetProfiles(getProfiles, response))
{
    int size = response->__sizeProfiles;
    for (int i = 0; i < size; ++i)
    {
        if (response->Profiles[i])
        {
            //Media Profile Token: response->Profiles[i]->token;
            //一般设备包含多个Profile Token，包含主码流、子码流、第三码流
        }
    }
}
```
**获取Media Uri**
使用获取的Media URI和Media Profile Token获取对应的Media Uri，也就是RTSP地址
```
MediaBindingProxy* proxyMedia = new MediaBindingProxy;
proxyMedia->soap_endpoint = mediaUrl.c_str();
_trt__GetStreamUri* getStreamUri = soap_new__trt__GetStreamUri(proxyMedia, -1);
tt__StreamSetup* streamSetup = soap_new_tt__StreamSetup(proxyMedia, -1);
getStreamUri->StreamSetup = streamSetup;
getStreamUri->StreamSetup->Stream = tt__StreamType__RTP_Unicast;
tt__Transport* transport = soap_new_tt__Transport(proxyMedia, -1);
getStreamUri->StreamSetup->Transport = transport;
getStreamUri->StreamSetup->Transport->Protocol = tt__TransportProtocol__RTSP;
_trt__GetStreamUriResponse* response = soap_new__trt__GetStreamUriResponse(proxyMedia, -1);
if (!profile.empty())
{
    getStreamUri->ProfileToken = const_cast<char*>(profile.c_str());
    if (security(proxyMedia, username, password))
    {
        if (proxyMedia->GetStreamUri(getStreamUri, response) == SOAP_OK)
        {
            //RTSP地址: response->MediaUri->Uri
        }
    }
}
```
获取到RTSP地址就可以进行拉流操作了
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
