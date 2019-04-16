# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之53---BREW SDK 九大功能之电信服务 - 我相信...... - CSDN博客





2010年04月07日 09:10:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1690








能够直接提供电信服务是BREW技术的一大特色。BREW中支持的电信服务主要是指语音业务，以及附加业务短消息功能，蓝牙通信功能，小区多播通信功能等等。在BREW3.X之前，语音和短信功能都可以通过ITAPI接口来提供，在BREW 3.X及以上版本，这一方法同样有限。

ITAPI 是一个简单的设备通话层接口。它可提供以下服务：获取通话状态，安排语音呼叫，从 SMS 消息中提取 SMS 文本，获取进入或正在呼叫的主叫号码，注册 SMS 消息，发送 SMS 消息等功能。ITAPI 允许应用程序注册以下通知：NMASK_TAPI_STATUS，NMASK_TAPI_SMS_TEXT和NMASK_TAPI_SMS_TS。

应用程序通过ITAPI接口可以接收 SMS 消息：

1. 目标为BREW 应用的定向 SMS 消息，发送形式为 //BREW:<ClassID>:<消息> 。在指定目标应用的ClassID 时，ITAPI_SendSMS() 将消息内部进行格式化操作

2. 所有文本消息

3. 所有特定电信业务 ID 的消息

4. 所有具有特定前缀的文本消息

其中，接收指向应用程序的SMS不需要注册，作为 EVT_APP_MESSAGE 事件接收，dwParam 包含指向短信有效负载的指针，该有效负载可作为 char * pszMsgTxt = (char *)dwParam。

对于访问接收任意文本消息，应用程序必须使用 NMASK_TAPI_SMS_TEXT 掩码注册 TAPI 类，作为 EVT_NOTIFY 事件接收，其中 dwParam 包含指向 AEENotify 的指针。消息可作为 AEENotify * pNotify = (AEENotify*)dwParam; AEESMSTextMsg * pTextMsg = (AEESMSTextMsg*)pNotify->pData;

如果接收特定电信业务消息，应用程序必须使用以下掩码 uint32 mask = (NMASK_TAPI_SMS_TS |(SMS_TELESERVICE_ID << 16)) 进行注册，作为 EVT_NOTIFY 事件接收，其中 dwParam 包含指向 AEENotify 的指针消息可作为 AEENotify * pNotify = (AEENotify*)dwParam; AEESMSMsg *pMsg = (AEESMSMsg *)pNotify->pData; 

如果访问接收具有特定前缀的文本消息：消息格式为 //<SMS_Prefix>:<Message_Text> 应用程序必须注册为 <SMS_Prefix> 的处理程序，作为 EVT_APP_BROWSE_URL 接收。dwParam 包含指向 //<SMS_Prefix>:<Message_Text> 的指针。

通过BREW中的ITAPI可以获知设备状态的更改，通过NMASK_TAPI_STATUS掩码，只要设备的通话状态发生变化，应用程序都可以使用 TAPI 类获得通知。只要状态发生变化，应用程序就会收到 EVT_NOTIFY 事件。此事件的 dwParam 为 AEENotify 类型。此 AEENotify 结构内的 pData 成员属于 TAPIStatus 类型，并包含设备当前通话状态的详细信息。

BREW 3.x专门为短信提供了新的接口ISMSMsg，ISMS，ISMSNotifier，和ISMSStorage新接口完整实现了短信协议TIA-EIA-637-A，而旧接口仅部分实现此协议。就短信发送而言，新接口主要支持下面三种编码格式：

lASCII－单位：字节，最大长度：160

lUNCODE －单位：双字节，最大长度：70

lBINARY－单位：字节，最大长度：140

原有接口仅支持一种编码格式，由AEE_DEVICEITEM_TAPI_SMS_ENCODING指定。就短信接收而言，新接口可以得到更多信息：短信内容/编码格式，发送方号码，短信中心接收到短信的时刻等，而旧接口仅能得到短信内容。ISMSStorage& ISMSStorage2用来存储，读取，删除和更新短信。存储的类型包括RUIM卡和手机自身。

BREW 3.X及以上版本还提供了对一般语音付、蓝牙和WIFI的完整支持，使应用可以获得更广泛的通信能力。



