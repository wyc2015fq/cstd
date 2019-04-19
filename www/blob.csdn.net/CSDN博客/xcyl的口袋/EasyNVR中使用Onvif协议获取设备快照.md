# EasyNVR中使用Onvif协议获取设备快照 - xcyl的口袋 - CSDN博客
2017年09月24日 15:02:17[xcyl](https://me.csdn.net/cai6811376)阅读数：527
我们知道EasyNVR中可以获取快照信息，之前的文章也说明了EasyNVR是如何进行快照抓取的
这里我们使用另一种方法进行快照的抓取
## 流程
**获取设备能力Capabilities**
获取设备的能力，并且可以获取到Media和PTZ的URI，使用Media的URI，可以进行快照的抓取
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
获取Profile token可以获取对应的快照
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
**获取快照访问地址**
快照访问地址通常为HTTP地址，可以通过这个HTTP地址GET到快照文件
```cpp
MediaBindingProxy* mediaProxy = new MediaBindingProxy;
  mediaProxy->soap_endpoint = mediaUrl.c_str();
  if (!security(mediaProxy, username, password))
  {
      delete mediaProxy;
      return result;
  }
  _trt__GetSnapshotUri* snap = soap_new__trt__GetSnapshotUri(mediaProxy, -1);
  _trt__GetSnapshotUriResponse* response = soap_new__trt__GetSnapshotUriResponse(mediaProxy, -1);
  snap->ProfileToken = const_cast<char*>(profile.c_str());
  if (SOAP_OK == mediaProxy->GetSnapshotUri(snap, response))
  {
      if (response->MediaUri && response->MediaUri->Uri)
      {
          //response->MediaUri->Uri
          //快照地址
      }
  }
  if (response)
      soap_delete__trt__GetSnapshotUriResponse(mediaProxy, response);
  if (snap)
      soap_delete__trt__GetSnapshotUri(mediaProxy, snap);
  return result;
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
