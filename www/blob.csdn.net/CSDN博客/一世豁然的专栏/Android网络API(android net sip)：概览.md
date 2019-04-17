# Android网络API(android.net.sip)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月11日 07:48:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：751








提供对会话发起协议（SIP）功能的访问，例如使用SIP进行和应答VOIP呼叫。




有关更多信息，请参阅会话启动协议开发人员指南。





要开始，您需要通过调用newInstance（）获取SipManager的一个实例。





使用SipManager，您可以使用makeAudioCall（）和takeAudioCall（）发起SIP音频通话。 两种方法都需要一个SipAudioCall.Listener，当呼叫的状态发生变化时，例如当呼叫响铃，建立或结束时，SipAudioCall.Listener接收回叫。





makeAudioCall（）还需要两个表示本地设备和对等设备的SipProfile对象。 您可以使用SipProfile.Builder子类创建SipProfile。





一旦您有SipAudioCall，您可以使用实例执行SIP音频呼叫操作，例如拨打电话，应答呼叫，静音通话，打开扬声器模式，发送DTMF音调等。





如果要创建通用SIP连接（例如视频通话或其他），可以使用open（）从SipManager创建SIP连接。 如果您只想创建音频SIP呼叫，那么您应该使用SipAudioCall类，如上所述。





注意：并非所有Android设备都支持SIP的VOIP功能。 在执行任何SIP活动之前，您应该调用isVoipSupported（）来验证设备是否支持VOIP呼叫，并且是ApiSupported（）来验证设备是否支持SIP API。 您的应用程序还必须请求INTERNET和USE_SIP权限才能使用SIP API。








一、接口

[SipRegistrationListener](https://developer.android.google.cn/reference/android/net/sip/SipRegistrationListener.html)


SIP注册事件侦听器。








二、类

[SipAudioCall](https://developer.android.google.cn/reference/android/net/sip/SipAudioCall.html)


通过SIP处理互联网音频呼叫。





[SipAudioCall.Listener](https://developer.android.google.cn/reference/android/net/sip/SipAudioCall.Listener.html)


用于与SIP呼叫有关的事件的侦听器，例如当接收到呼叫时（“在振铃”）或呼叫正在发出（“在呼叫”）。





[SipErrorCode](https://developer.android.google.cn/reference/android/net/sip/SipErrorCode.html)


定义在SIP操作期间返回的错误代码。





[SipManager](https://developer.android.google.cn/reference/android/net/sip/SipManager.html)


为SIP任务提供API，例如发起SIP连接，并提供对相关SIP服务的访问。





[SipProfile](https://developer.android.google.cn/reference/android/net/sip/SipProfile.html)


定义SIP配置文件，包括SIP帐户，域和服务器信息。





[SipProfile.Builder](https://developer.android.google.cn/reference/android/net/sip/SipProfile.Builder.html)


用于创建SipProfile的Helper类。





[SipSession](https://developer.android.google.cn/reference/android/net/sip/SipSession.html)


表示与SIP对话或不在对话框内的独立事务相关联的SIP会话。





[SipSession.Listener](https://developer.android.google.cn/reference/android/net/sip/SipSession.Listener.html)


用于与SIP会话有关的事件的侦听器，例如当会话被注册时（“在注册”）或呼叫正在发出（“在呼叫”）。





[SipSession.State](https://developer.android.google.cn/reference/android/net/sip/SipSession.State.html)


定义SIP会话状态，如“注册”，“呼出”和“在通话”。








三、异常

[SipException](https://developer.android.google.cn/reference/android/net/sip/SipException.html)


表示一般的SIP相关异常。




