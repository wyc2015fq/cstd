# EasyNVR接入EasyDSS云视频平台快照上传实现 - xcyl的口袋 - CSDN博客
2017年09月22日 10:54:55[xcyl](https://me.csdn.net/cai6811376)阅读数：363
EasyNVR拥有接入EasyDSS云平台的功能
接入EasyDSS云平台会定时向云平台上传快照数据，这个快照数据用于云平台向客户端提供快照展示
**遇到的问题**
由于快照上传的间隔提供认为修改的功能，则我们限定最短时间为1分钟
通道的数量会直接影响快照上传
考虑到压力情况，1分钟之内上传上千通道的快照
快照数据经过Base64编码，会比快照真实数据扩大1.33333倍，数据量大
EasyNVR的固定通道设计
**解决方案**
将所有通道快照上传分散在快照间隔时间内
若遇到通道未启用、通道不在线则跳过，并且减少通道数量，增大间隔时间
**实现**
```cpp
virtual SInt64 Run()
{
    EventFlags event = this->GetEvents();
    if (event & Task::kIdleEvent || event & Task::kUpdateEvent)
    {
           if (!channels_)
               return 0;
        bool allUploaded = true;
        for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
        {
            if (snapCtrlArray_[i])
            {
                allUploaded = false;
                break;
            }
        }
        if (allUploaded)
        {
            return 0;
        }
        if (snapCtrlArray_[curSnapIndex_])
        {
               snapCtrlArray_[curSnapIndex_] = false;
               int curChannel = curSnapIndex_ + 1;
               if (++curSnapIndex_ > MAX_CHANNEL_COUNT - 1)
               {
                   curSnapIndex_ = 0;
               }
               CameraInfo* cameraInfo = (*channels_)[curChannel]->GetChannelInfo();
               if (!cameraInfo->enable || !cameraInfo->online)
               {
                   --postSnapCount_;
                   return 1;
               }
            postSnap(curChannel);
        }
        //return (std::min)(timeout_ * 60 / MAX_CHANNEL_COUNT, 3) * 1000;
        return timeout_ * 60 * 1000 / postSnapCount_;
    }
    if (event & Task::kTimeoutEvent)
    {
           channels_ = NULL;
           QTSS_RoleParams params;
           params.easyNVRChannelsConfigParams.inChannels = NULL;
           EasyNVRUtil::CallDispatch(Easy_NVRGetChannelsConfig_Role, QTSSModule::kGetChannelsConfigRole, params);
           channels_ = static_cast<std::map<int, EasyNVRChannel*>*>(params.easyNVRChannelsConfigParams.inChannels);
           postSnapCount_ = MAX_CHANNEL_COUNT;
           for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
           {
               snapCtrlArray_[i] = true;
           }
           this->Signal(Task::kUpdateEvent);
        timeoutTask_.RefreshTimeout();
    }
    return 0;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
