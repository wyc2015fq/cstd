# 使用Onvif协议进行设备PTZ云台控制 - xcyl的口袋 - CSDN博客
2017年08月12日 11:24:43[xcyl](https://me.csdn.net/cai6811376)阅读数：9189
接上一篇
使用Onvif协议最重要的应用就是对设备进行PTZ云台控制，PTZ控制包含转动、变焦等，这里我们主要讨论常用的转动和变焦(也就是放大缩小)
## 流程
要进行设备PTZ控制，我们首先需要获取到设备的Device Service Address和此设备的用户名密码
前两个流程和获取流地址一样
**获取设备能力Capabilities**
获取设备的能力，并且可以获取到Media和PTZ的URI，使用这两个URL可以进行流和PTZ操作，这里如果PTZ URI获取不到意味着这个设备无法支持PTZ控制
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
**PTZ控制**
使用获取的Media URI和Media Profile Token进行PTZ控制
PTZ控制，这里只考虑ContinuousMove，需要在停止的时候发送停止命令
```
_tptz__ContinuousMove* continuousMove = soap_new__tptz__ContinuousMove(ptzProxy, -1);
_tptz__ContinuousMoveResponse* response = soap_new__tptz__ContinuousMoveResponse(ptzProxy, -1);
continuousMove->ProfileToken = const_cast<char *>(profile.c_str());
tt__PTZSpeed* velocity = soap_new_tt__PTZSpeed(ptzProxy, -1);
continuousMove->Velocity = velocity;
tt__Vector2D* panTilt = soap_new_tt__Vector2D(ptzProxy, -1);
continuousMove->Velocity->PanTilt = panTilt;
continuousMove->Velocity->PanTilt->space = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace";
tt__Vector1D* zoom = soap_new_tt__Vector1D(ptzProxy, -1);
continuousMove->Velocity->Zoom = zoom;
switch (command)
{
case LEFT:
    continuousMove->Velocity->PanTilt->x = -((float)speed / 10);
    continuousMove->Velocity->PanTilt->y = 0;
    break;
case RIGHT:
    continuousMove->Velocity->PanTilt->x = ((float)speed / 10);
    continuousMove->Velocity->PanTilt->y = 0;
    break;
case UP:
    continuousMove->Velocity->PanTilt->x = 0;
    continuousMove->Velocity->PanTilt->y = ((float)speed / 10);
    break;
case DOWN:
    continuousMove->Velocity->PanTilt->x = 0;
    continuousMove->Velocity->PanTilt->y = -((float)speed / 10);
    break;
case ZOOMIN:
    continuousMove->Velocity->Zoom->x = ((float)speed / 10);
    break;
case ZOOMOUT:
    continuousMove->Velocity->Zoom->x = -((float)speed / 10);
    break;
default:
    break;
}
if (ptzProxy->ContinuousMove(continuousMove, response) == SOAP_OK)
    //转动成功
```
停止PTZ控制
```cpp
_tptz__Stop* stop = soap_new__tptz__Stop(ptzProxy, -1);
_tptz__StopResponse* response = soap_new__tptz__StopResponse(ptzProxy, -1);
stop->ProfileToken = const_cast<char*>(profile.c_str());
bool* pantilt = new bool;
stop->PanTilt = pantilt;
*(stop->PanTilt) = true;
bool* zoom = new bool;
stop->Zoom = zoom;
*(stop->Zoom) = true;
if (SOAP_OK == ptzProxy->Stop(stop, response))
    //停止成功
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
