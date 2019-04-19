# EasyDarwin云平台之EasyCMS中心管理服务消息处理优化 - xcyl的口袋 - CSDN博客
2016年07月03日 00:46:56[xcyl](https://me.csdn.net/cai6811376)阅读数：2019
**问题**
之前EasyCMS的消息处理一直是EasyCMS的性能瓶颈。原因是当客户端通过RESTful接口访问EasyCMS时，EasyCMS构建报文转发给设备时与出现等待问题。这就造成了EasyCMS在等待设备响应时出现了资源浪费的情况。如图所示
![CMS](https://img-blog.csdn.net/20160703003022896)
CSHTTPSession在访问RESTful接口后再自己做循环超时等待SCHTTPSession返回回应信息。
```
//HTTPSession.cpp ExecNetMsgCSGetStreamReqRESTful
fInfo.uTimeoutNum++;
if (fInfo.uTimeoutNum > CliStartStreamTimeout / 100)//超时了
{
    fInfo.bWaitingState = false;//恢复状态
    return httpRequestTimeout;
}
else//没有超时，继续等待
{
    fInfo.uWaitingTime = 100;//以100ms为周期循环等待，这样不占用CPU
    return QTSS_NoErr;
}
```
**更改方法**
仿照sip协议中的From/To/Via将发送方、转发方、接收方的ID携带在报文中，当响应报文发送回来之后可以有效的索引报文的发送方和接收方，这样可以更好的将发送/接收的报文使用对应的HTTPSession进行处理。避免了循环等待的低效操作。
```cpp
//HTTPSession.cpp ExecNetMsgDSPushStreamAck
string strFrom = req.GetBodyValue(EASY_TAG_FROM);
string strTo = req.GetBodyValue(EASY_TAG_TO);
string strVia = req.GetBodyValue(EASY_TAG_VIA);
OSRefTableEx* sessionMap = QTSServerInterface::GetServer()->GetHTTPSessionMap();
OSRefTableEx::OSRefEx* sessionRef = sessionMap->Resolve(strTo);
if (sessionRef == NULL)
    return EASY_ERROR_SESSION_NOT_FOUND;
OSRefReleaserEx releaser(sessionMap, strTo);
HTTPSession* httpSession = static_cast<HTTPSession *>(sessionRef->GetObjectPtr());
if (httpSession->IsLiveSession())
{
    //do something
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
