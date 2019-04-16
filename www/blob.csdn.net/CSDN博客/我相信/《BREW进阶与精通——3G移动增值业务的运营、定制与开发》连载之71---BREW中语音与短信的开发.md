# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之71---BREW中语音与短信的开发 - 我相信...... - CSDN博客





2010年08月26日 10:09:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1794








在BREW 3.X 之前的版本，语音和短信的功能主要是通过ITAPI 接口实现的。就语音业务而言，建立语音呼叫通过ITAPI_MakeVoiceCall（）完成，但是无法实现电话的接听。可以用

BREW的

ITAPI_SendSMS()函数来发送中／英文短消息，例如，发送英文短信的方法为：



ITAPI_SendSMS(pITapi,
"8581112222", "Hello World", 0, MyMOSMSNotify, pMe);

　　如果要发送中文短信，需要把中文字符串由

AECHAR*
强制转换成

(const char*),按照英文短信的发送方法发送就可以了。例子如下：

AECHAR w_string[]= { 0x4F60, 0x597D,0x4e2D,0x56FD, ‘/0’};
//你好中国



ITAPI_SendSMS(pITapi,
"13301330133", (const char*)w_string, 0, MyMOSMSNotify, pMe);



ITAPI_SendSms()实际上是一个异步调用，即该函数发消息到系统底层，再由底层完成短消息的发送。短信发送成功与否的状态值需要回调函数返回

,如果程序中需要显示发送成功或失败的结果，则需要用户自己创建一个回调函数例如MyMOSMSNotify；反之，则可以只写一个空函数。这里要注意区分

ITAPI_SendSMS()的返回值和回调函数返回的状态值这两者的区别。前者是表明

ITAPI_SendSMS()函数是否正确执行，即是否发送；由于是异步调用，在回调函数里的状态值是真正的发送结果。

BREW定义了一种特定的短消息格式来启动应用程序。

BREW把这种格式的短消息称为

BREW特定应用消息

(BREW
application-directed message)，即这种特殊的短消息是发送给某一个特定的

BREW应用程序的。为方便起见，统称之为

BREW短信，以区别普通短信。

BREW短信的格式如下：

**//BREW:<ClassID>:<Text Payload>**

其中：



//BREW——消息头，表示本短信为

BREW短信；



Class
ID——表示接收短信的应用程序的

Class ID 号；



Text
Payload——附加的消息文本，

ASCII格式；

例如：

//BREW:01009FF0:test

//BREW:0x01009FF0:Hello world

BREW短信的发送非常简单。和发送普通短信一样，调用

ITAPI_SendSMS()函数发送就可以了。例如：

ITAPI_SendSMS(pMe->m_pITAPI,”13331016512”,”//BREW:0x01009FF0:Hello world”, 0,(PFNSMSSTATUS)MySmsNotify, pMe);



BREW短信的接收过程如下：

1．BREW手机的底层软件首先截获短消息，判断消息头是否为 //BREW；

2．如果判断是BREW短信，则启动BREW应用管理器；

3．BREW应用管理器根据收到的Class ID加载对应的BREW应用程序,并完成该应用程序的初始化工作；

4．BREW应用管理器发送EVT_APP_MESSAGE事件给应用程序；

5．应用程序的HandleEvent()函数处理EVT_APP_MESSAGE事件，如果需要启动应用，则调用ISHELL_StartApplet()函数来启动；或者也可以“悄悄地”进行处理而不用启动应用程序。



通过以上的描述我们知道，BREW应用管理器是通过发送EVT_APP_MESSAGE来唤醒应用程序的，所以应用程序必须处理EVT_APP_MESSAGE才能接收BREW短信，才能完成程序的启动。具体例子如下（代码中的 return TRUE 语句是必须要加上的）：

……

case EVT_APP_MESSAGE:

//start up applet

ISHELL_StartApplet(pMe->a.m_pIShell, AEECLSID_HELLOWORLD);

return TRUE;

……

如果应用程序想得到BREW短信的文本信息，可以通过

EVT_APP_MESSAGE事件的

dwParam参数来得到，即参数

dwParm的内容实际上是指向

Text
Payload消息体的指针。具体例子如下：

case
EVT_APP_MESSAGE:

{

char*
textPayload = (char *)dwParam;

return TRUE;

}

中文短消息需要用

Unicode编码格式，但是

BREW短信的消息头必须用

ASCII码发送（包括

Class
ID）

,即字符串**//BREW:<Class ID>:**须用英文发送才能被BREW的底层软件正确识别，如果是UNICODE字符，会被认作是普通短消息而不会启动相应的应用程序。这就产生了一个矛盾：如果用ASCII码，则无法发送中文；如果用UNICODE码，则会被认作普通短消息。

解决办法是采用拼接的办法，即BREW短消息的消息头部分采用ASCII码，后段的消息体部分采用UNICODE码，把这两段拼起来发送出去就可以了。具体步骤如下：

1．定义变量wStrMessage,
类型为（AECHAR*），指向一段UNICODE字符串的缓冲区；

2．定义变量BrewHeader存储BREW短信的消息头字符串，这里假设为//BREW:0x01009FF0:。将这段ASCII字符串拷贝到wStrMessage缓冲区的开始段。可以用STRNCPY函数来完成拷贝，例子如下：

STRNCPY( (char *)(wStrMessage), BrewHeader, STRLEN(BrewHeader)
);

3．在wStringMessage缓冲区中，紧接着BREW短消息头的后面，将中文字符串ChineseTextMsg拷贝过来；

例如：WSTRCPY（&(wstrMessage[STRLEN(BrewHeader)/2]),



ChineseTextMsg);

4．调用

ITAPI_SendSMS()函数发送，注意

UNICODE字符串

wStrMessage要强制转换成（

const
char*），这是

ITAPI_SendSMS()函数要求的。



经过以上的处理，在要发送的

UNICODE字符串的前面实际上是包含了

BREW消息头的

ASCII字符，所以能被

BREW底层正确识别，而

BREW底层对其后面的数据不作任何处理。

在接收方处理中文短信很简单，EVT_APP_MESSAGE事件的

dwParam变成了指向

UNICODE字符串的指针，只要把该段的数据拷贝过来即可。例子如下：

case
EVT_APP_MESSAGE:


{


WSTRCPY( (AECHAR*)wTextPayload, (AECHAR*)dwParam);



return TRUE;



}

采用上述方法，即可以发送中文，也可以发送中英文混排的字符串。需要注意的是由于中文短信最多是70个，扣除掉BREW短信头，大约还能够发送60个中文字符。



BREW
3.X中极大的增强了语言呼叫的处理能力，引入了ICallMgr，ICall和ITelephone 3个新的接口，可以发起和接听呼叫处理，并监测呼叫的状态。但这些操作需要的使用权限较高，开发时一定要引起重视。就短信而言，功能也得到了极大的丰富，相应的引入了ISMSMsg，ISMS，ISMSNotifier和ISMSStorage 这4个接口，使用这些接口的短信接收示例如下：

case EVT_NOTIFY:



if (pNotify && (pNotify->cls == AEECLSID_SMSNOTIFIER)) // 发送方确认



{



uint32

MyNMask =
((AEESMS_TYPE_TEXT << 16) | NMASK_SMS_TYPE);



if ((pNotify->dwMask & MyNMask) == MyNMask) {



uMsgId = (uint32
)pNotify->pData; // 屏蔽位确认

if(ISMS_ReceiveMsg(pMe->m_pISMS,uMsgId,&pRxMsg)==AEE_SUCCESS)


{



SMSMsgOpt TmpOpt;



if(ISMSMSG_GetOpt(pRxMsg,
MSGOPT_FROM_DEVICE_SZ,



&TmpOpt)==AEE_SUCCESS) // 获取发送方号码



{

……

 // 处理

TmpOpt 中包含的电话选项



}

其它

Options 处理



}



}



}



