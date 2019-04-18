# 链接：SIP - 一世豁然的专栏 - CSDN博客





2017年05月18日 11:24:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：418标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









Android提供了支持会话发起协议（SIP）的API。 这样可以为您的应用程序添加基于SIP的互联网电话功能。 Android包括一个完整的SIP协议栈和集成的呼叫管理服务，使应用程序可以轻松地设置拨出和接收语音呼叫，而无需直接管理会话，传输级别通信或音频记录或播放。




以下是可能使用SIP API的应用程序类型的示例：


1、视频会议。

2、即时通讯。







一、要求和限制

以下是开发SIP应用程序的要求：


1、您必须拥有运行Android 2.3或更高版本的移动设备。




2、SIP通过无线数据连接运行，因此您的设备必须具有数据连接（使用移动数据服务或Wi-Fi）。 这意味着您无法在AVD上测试 - 您只能在物理设备上测试。 有关详细信息，请参阅测试SIP应用程序。




3、应用程序通信会话中的每个参与者必须具有SIP帐户。 有许多不同的SIP提供商提供SIP帐户。







二、SIP API类和接口

以下是Android SIP API中包含的类和一个接口（SipRegistrationListener）的摘要：



|Class/Interface|Description|
|----|----|
|`[SipAudioCall](https://developer.android.google.cn/reference/android/net/sip/SipAudioCall.html)`|通过SIP处理互联网音频呼叫。|
|`[SipAudioCall.Listener](https://developer.android.google.cn/reference/android/net/sip/SipAudioCall.Listener.html)`|用于与SIP呼叫有关的事件的侦听器，例如当正在接收呼叫（“在振铃”）或呼叫呼出（“在呼叫”）时。|
|`[SipErrorCode](https://developer.android.google.cn/reference/android/net/sip/SipErrorCode.html)`|定义SIP操作期间返回的错误代码。|
|`[SipManager](https://developer.android.google.cn/reference/android/net/sip/SipManager.html)`|为SIP任务提供API，例如发起SIP连接，并提供对相关SIP服务的访问。|
|`[SipProfile](https://developer.android.google.cn/reference/android/net/sip/SipProfile.html)`|定义SIP配置文件，包括SIP帐户，域和服务器信息。|
|`[SipProfile.Builder](https://developer.android.google.cn/reference/android/net/sip/SipProfile.Builder.html)`|用于创建SipProfile的Helper类。|
|`[SipSession](https://developer.android.google.cn/reference/android/net/sip/SipSession.html)`|表示与SIP对话或不在对话框内的独立事务相关联的SIP会话。|
|`[SipSession.Listener](https://developer.android.google.cn/reference/android/net/sip/SipSession.Listener.html)`|用于与SIP会话有关的事件的侦听器，例如当会话被注册时（“登记”）或呼叫正在发出（“在呼叫”）。|
|`[SipSession.State](https://developer.android.google.cn/reference/android/net/sip/SipSession.State.html)`|定义SIP会话状态，例如“注册”，“呼出”和“在呼叫中”。|
|`[SipRegistrationListener](https://developer.android.google.cn/reference/android/net/sip/SipRegistrationListener.html)`|作为SIP注册事件的侦听器的接口。|





三、创建清单

如果您正在开发使用SIP API的应用程序，请记住该功能仅在Android 2.3（API第9级）和更高版本的平台上受支持。 此外，在运行Android 2.3（API级别9）或更高版本的设备中，并非所有设备都将提供SIP支持。





要使用SIP，请将以下权限添加到应用程序的清单中：



- `android.permission.USE_SIP`
- `android.permission.INTERNET`




要确保您的应用程序只能安装在能够支持SIP的设备上，请将以下内容添加到应用程序的清单中：



`<uses-sdk android:minSdkVersion="9" />`

这表示您的应用程序需要Android 2.3或更高版本。 有关详细信息，请参阅API级别和<uses-sdk>元素的文档。




要控制您的应用程序如何从不支持SIP的设备（例如，在Google Play上）过滤，请将以下内容添加到应用程序的清单中：



`<uses-feature android:name="android.hardware.sip.voip" />`

这表示您的应用程序使用SIP API。 声明应包括一个android：required属性，指示是否要从不提供SIP支持的设备中过滤应用程序。 也可能需要其他<uses-feature>声明，具体取决于您的实现。 有关更多信息，请参阅<uses-feature>元素的文档。




如果您的应用程序旨在接收呼叫，则还必须在应用程序的清单中定义一个接收器（BroadcastReceiver子类）：


`<receiver android:name=".IncomingCallReceiver" android:label="Call Receiver" />`

这里是SipDemo清单的摘录：



```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
          package="com.example.android.sip">
  ...
     <receiver android:name=".IncomingCallReceiver" android:label="Call Receiver" />
  ...
  <uses-sdk android:minSdkVersion="9" />
  <uses-permission android:name="android.permission.USE_SIP" />
  <uses-permission android:name="android.permission.INTERNET" />
  ...
  <uses-feature android:name="android.hardware.sip.voip" android:required="true" />
  <uses-feature android:name="android.hardware.wifi" android:required="true" />
  <uses-feature android:name="android.hardware.microphone" android:required="true" />
</manifest>
```






四、创建SipManager

要使用SIP API，您的应用程序必须创建一个SipManager对象。 SipManager在您的应用程序中处理以下内容：


1、启动SIP会话。




2、启动和接听呼叫。




3、使用SIP提供商注册和取消注册。




4、验证会话连接。




您实例化一个新的SipManager如下：




```java
public SipManager mSipManager = null;
...
if (mSipManager == null) {
    mSipManager = SipManager.newInstance(this);
}
```






五、注册SIP服务器

典型的Android SIP应用程序涉及一个或多个用户，每个用户都有一个SIP帐户。 在Android SIP应用程序中，每个SIP帐户都由SipProfile对象表示。





SipProfile定义SIP配置文件，包括SIP帐户以及域和服务器信息。 与运行应用程序的设备上的SIP帐户关联的配置称为本地配置文件。 会话连接的配置文件称为对等体配置文件。 当您的SIP应用程序使用本地SipProfile登录到SIP服务器时，有效地将设备注册为将SIP呼叫发送到您的SIP地址的位置。





本节介绍如何创建SipProfile，使用SIP服务器注册，并跟踪注册事件。





您创建一个SipProfile对象，如下所示：




```java
public SipProfile mSipProfile = null;
...

SipProfile.Builder builder = new SipProfile.Builder(username, domain);
builder.setPassword(password);
mSipProfile = builder.build();
```


以下代码片段打开用于进行呼叫和/或接收通用SIP呼叫的本地配置文件。 呼叫者可以通过mSipManager.makeAudioCall进行后续呼叫。 这个节选还会设置动作android.SipDemo.INCOMING_CALL，当设备接收到一个呼叫时，它将被意图过滤器使用（请参阅设置意图过滤器来接收呼叫）。 这是注册步骤：



```java
Intent intent = new Intent();
intent.setAction("android.SipDemo.INCOMING_CALL");
PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, intent, Intent.FILL_IN_DATA);
mSipManager.open(mSipProfile, pendingIntent, null);
```


最后，该代码在SipManager上设置一个SipRegistrationListener。 这将跟踪SipProfile是否已成功注册到您的SIP服务提供商：



```java
mSipManager.setRegistrationListener(mSipProfile.getUriString(), new SipRegistrationListener() {

public void onRegistering(String localProfileUri) {
    updateStatus("Registering with SIP Server...");
}

public void onRegistrationDone(String localProfileUri, long expiryTime) {
    updateStatus("Ready");
}

public void onRegistrationFailed(String localProfileUri, int errorCode,
    String errorMessage) {
    updateStatus("Registration failed.  Please check settings.");
}
```


当您的应用程序使用配置文件完成时，应该关闭它，以将关联的对象释放到内存中，并从服务器取消注册设备。 例如：



```java
public void closeLocalProfile() {
    if (mSipManager == null) {
       return;
    }
    try {
       if (mSipProfile != null) {
          mSipManager.close(mSipProfile.getUriString());
       }
     } catch (Exception ee) {
       Log.d("WalkieTalkieActivity/onDestroy", "Failed to close local profile.", ee);
     }
}
```






六、进行音频通话

要进行音频通话，您必须具备以下功能：


1、正在进行呼叫的SipProfile（“本地配置文件”）以及接收呼叫的有效SIP地址（“对等体配置文件”）。

2、SipManager对象。




要进行音频通话，您应该设置一个SipAudioCall.Listener。 客户端与SIP堆栈的大部分交互通过监听器进行。 在这段代码中，您将看到SipAudioCall.Listener在调用建立后如何设置：




```java
SipAudioCall.Listener listener = new SipAudioCall.Listener() {

   @Override
   public void onCallEstablished(SipAudioCall call) {
      call.startAudio();
      call.setSpeakerMode(true);
      call.toggleMute();
         ...
   }

   @Override
   public void onCallEnded(SipAudioCall call) {
      // Do something.
   }
};
```


一旦你设置了SipAudioCall.Listener，你可以打电话。 SipManager方法makeAudioCall采用以下参数：

1、本地SIP配置文件（呼叫者）。

2、对等SIP配置文件（被叫用户）。

3、SipAudioCall.Listener收听来自SipAudioCall的呼叫事件。 这可以是null，但是如上所示，监听器用于在建立呼叫后设置事件。

4、超时值，以秒为单位。




例如：



```java
call = mSipManager.makeAudioCall(mSipProfile.getUriString(), sipAddress, listener, 30);
```



七、接收电话

要接收呼叫，SIP应用程序必须包括BroadcastReceiver的子类，该子类能够响应表示有来电的意图。 因此，您必须在应用程序中执行以下操作：


1、在AndroidManifest.xml中，声明一个<receiver>。 在SipDemo中，这是<receiver android：name =“IncomingCallReceiver”android：label =“Call Receiver”/>。

2、实现接收机，它是BroadcastReceiver的一个子类。 在SipDemo中，这是IncomingCallReceiver。

3、初始化本地配置文件（SipProfile），具有在有人呼叫本地配置文件时触发接收器的挂起意图。

4、设置一个意图过滤器，通过代表来电的动作进行过滤。 在SipDemo中，此操作是android.SipDemo.INCOMING_CALL。




一）、Subclassing BroadcastReceiver

要接收呼叫，您的SIP应用程序必须对BroadcastReceiver进行子类化。 Android系统在接收到呼叫时处理传入的SIP呼叫并广播“来电”意图（由应用程序定义）。 以下是SipDemo示例中的子类BroadcastReceiver代码。




```java
/**
 * Listens for incoming SIP calls, intercepts and hands them off to WalkieTalkieActivity.
 */
public class IncomingCallReceiver extends BroadcastReceiver {
    /**
     * Processes the incoming call, answers it, and hands it over to the
     * WalkieTalkieActivity.
     * @param context The context under which the receiver is running.
     * @param intent The intent being received.
     */
    @Override
    public void onReceive(Context context, Intent intent) {
        SipAudioCall incomingCall = null;
        try {
            SipAudioCall.Listener listener = new SipAudioCall.Listener() {
                @Override
                public void onRinging(SipAudioCall call, SipProfile caller) {
                    try {
                        call.answerCall(30);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            };
            WalkieTalkieActivity wtActivity = (WalkieTalkieActivity) context;
            incomingCall = wtActivity.mSipManager.takeAudioCall(intent, listener);
            incomingCall.answerCall(30);
            incomingCall.startAudio();
            incomingCall.setSpeakerMode(true);
            if(incomingCall.isMuted()) {
                incomingCall.toggleMute();
            }
            wtActivity.call = incomingCall;
            wtActivity.updateStatus(incomingCall);
        } catch (Exception e) {
            if (incomingCall != null) {
                incomingCall.close();
            }
        }
    }
}
```


2、设置意图过滤器以接收呼叫

当SIP服务接收到新的呼叫时，它发送一个意图，其中包含应用程序提供的动作字符串。 在SipDemo中，此操作字符串为android.SipDemo.INCOMING_CALL。





来自SipDemo的此代码摘录显示了基于动作字符串android.SipDemo.INCOMING_CALL的SipProfile对象是如何创建的。 当SipProfile接收到一个调用时，PendingIntent对象将执行一个广播：




```java
public SipManager mSipManager = null;
public SipProfile mSipProfile = null;
...

Intent intent = new Intent();
intent.setAction("android.SipDemo.INCOMING_CALL");
PendingIntent pendingIntent = PendingIntent.getBroadcast(this, 0, intent, Intent.FILL_IN_DATA);
mSipManager.open(mSipProfile, pendingIntent, null);
```


广播将被意图过滤器拦截，然后触发接收器（IncomingCallReceiver）。 您可以在应用程序的清单文件中指定意图过滤器，或者按照应用程序的“活动”的SipDemo示例应用程序的onCreate（）方法中的代码进行操作：



```java
public class WalkieTalkieActivity extends Activity implements View.OnTouchListener {
...
    public IncomingCallReceiver callReceiver;
    ...

    @Override
    public void onCreate(Bundle savedInstanceState) {

       IntentFilter filter = new IntentFilter();
       filter.addAction("android.SipDemo.INCOMING_CALL");
       callReceiver = new IncomingCallReceiver();
       this.registerReceiver(callReceiver, filter);
       ...
    }
    ...
}
```






八、测试SIP应用程序

要测试SIP应用程序，您需要以下内容：


1、运行Android 2.3或更高版本的移动设备。 SIP通过无线方式运行，因此您必须在实际的设备上进行测试。 AVD测试将无法正常工作。




2、SIP帐号。 有许多不同的SIP提供商提供SIP帐户。




3、如果您打电话，也必须是有效的SIP帐户。




测试SIP应用程序：


1、在您的设备上，连接到无线（设置>无线和网络> Wi-Fi> Wi-Fi设置）。




2、设置您的移动设备进行测试，如“开发设备”中所述。




3、在您的移动设备上运行应用程序，如“在设备上开发”中所述。




4、如果您使用的是Android Studio，则可以通过打开事件日志控制台（视图>工具Windows>事件日志）查看应用程序日志输出。




5、确保您的应用程序配置为在运行时自动启动Logcat：

 1）、选择**Run > Edit Configurations**。

 2）、在**运行/调试配置**窗口中选择** Miscellaneous**选项卡。

 3）、在**Logcat**下，选择自动显示logcat，然后选择**OK**。



