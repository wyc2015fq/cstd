# EasyNVR中使用Onvif协议控制预置位 - xcyl的口袋 - CSDN博客
2017年09月24日 14:54:49[xcyl](https://me.csdn.net/cai6811376)阅读数：769
EasyNVR支持预置位控制，包括转到指定预置位，设置指定预置位，删除指定预置位
预置位在安防领域有较为普遍的应用，可以进行很多既定位置的跳转，很方便
之前我们说过如何用Onvif协议进行设备的发现，探测，以及云台控制等，今天我们来说一下如何使用Onvif进行预置位的操作
## 流程
**获取设备能力Capabilities**
获取设备的能力，并且可以获取到Media和PTZ的URI，使用这两个URL可以进行流和预置位操作，这里如果PTZ URI获取不到意味着这个设备无法支持预置位控制
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
**预置位控制**
```cpp
bool result = false;
   PTZBindingProxy* ptzProxy = new PTZBindingProxy;
   ptzProxy->soap_endpoint = ptzUrl.c_str();
   if (!security(ptzProxy, username, password))
   {
       delete ptzProxy;
       return result;
   }
   char presetToken[8];
   itoa(index, presetToken, 10);
   if (command == PRESET_GOTO)
   {
       _tptz__GotoPreset* preset = soap_new__tptz__GotoPreset(ptzProxy, -1);
       _tptz__GotoPresetResponse* response = soap_new__tptz__GotoPresetResponse(ptzProxy, -1);
       preset->ProfileToken = const_cast<char*>(profile.c_str());
       preset->PresetToken = presetToken;
       if (SOAP_OK == ptzProxy->GotoPreset(preset, response))
           result = true;
       if (response)
           soap_delete__tptz__GotoPresetResponse(ptzProxy, response);
       if (preset)
           soap_delete__tptz__GotoPreset(ptzProxy, preset);
   }
   else if (command == PRESET_SET)
   {
       _tptz__SetPreset* preset = soap_new__tptz__SetPreset(ptzProxy, -1);
       _tptz__SetPresetResponse* response = soap_new__tptz__SetPresetResponse(ptzProxy, -1);
       preset->ProfileToken = const_cast<char*>(profile.c_str());
       preset->PresetToken = presetToken;
       if (SOAP_OK == ptzProxy->SetPreset(preset, response))
           result = true;
       if (response)
           soap_delete__tptz__SetPresetResponse(ptzProxy, response);
       if (preset)
           soap_delete__tptz__SetPreset(ptzProxy, preset);
   }
   else if (command == PRESET_REMOVE)
   {
       _tptz__RemovePreset* preset = soap_new__tptz__RemovePreset(ptzProxy, -1);
       _tptz__RemovePresetResponse* response = soap_new__tptz__RemovePresetResponse(ptzProxy, -1);
       preset->ProfileToken = const_cast<char*>(profile.c_str());
       preset->PresetToken = presetToken;
       if (SOAP_OK == ptzProxy->RemovePreset(preset, response))
           result = true;
       if (response)
           soap_delete__tptz__RemovePresetResponse(ptzProxy, response);
       if (preset)
           soap_delete__tptz__RemovePreset(ptzProxy, preset);
   }
   delete ptzProxy;
   return result;
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
