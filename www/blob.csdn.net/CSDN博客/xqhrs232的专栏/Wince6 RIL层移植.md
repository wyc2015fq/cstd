# Wince6 RIL层移植 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:22:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：736
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.csdn.net/huang5556019/article/details/7794817](http://blog.csdn.net/huang5556019/article/details/7794817)
原文：[http://blog.csdn.net/songjinshi/article/details/7311990](http://blog.csdn.net/songjinshi/article/details/7311990)
##### RIL 移植：
因为不同的模组，支持的AT命令有所不同，或是格式不一样，还有就是返回不一样，我们有必要对不同的模组进行RIL驱动移植。
在Response.cpp,Msg.cpp等 文件中ParseXXX等函数，需要根据不同的AT指令参数来做格式修改。要移植，就是移植这些了。
###### 1. 前期准备
步骤1： 在BSP下建一个文件夹RIL驱动文件夹
例如D:/WINCE600/PLATFORM/SMDK6410/SRC/DRIVERS/RIL
步骤2：把/public/Cellcore/ril/driver把mdd 文件夹拷贝到RIL文件夹中重命名“RILMDD”
步骤3：把D:/WINCE600/PLATFORM/CEPC/SRC/DRIVERS/ 下的RILPDD拷贝到RIL文件夹中
步骤4：把RILPDD下面的ENFORA重命名为我们当前调试的模组名称，例如我们现在使用U6100,这样是为了方便以后要BSP同时支持几个模组的RIL驱动时好辨认，
步骤5：系统加入Cellcore组件，详情参照上面的介绍“添加CellCore/RIL组件”，
这样，就可以在enfora模型的基础上修改出我们的驱动了。
###### 2. 修改编译相关：
修改SMDK6410/SRC/DRIVERS/RIL/RILPDD/U6100下的sources文件
!include $(_TARGETPLATROOT)/SRC/DRIVERS/RIL/RILMDD/sources_common.inc
!include $(_TARGETPLATROOT)/SRC/DRIVERS/RIL/RILMDD/sources_dll.inc
使用BSP下面的MDD，
修改SMDK6410/SRC/DRIVERS/RIL/RILMDD下的sources_dll.inc 文件
“SOURCELIBS= ”改为如下（使用BSP下的MDD编译生成的LIB）
SOURCELIBS=$(_TARGETPLATROOT)/lib/$(_CPUINDPATH)/rilmdd.lib
“DEFFILE=” 改为如下：(使用BSP下MDD的DEF文件)
DEFFILE=$(_TARGETPLATROOT)/SRC/DRIVERS/RIL/RILMDD/rilgsm.def
这是为了方便后面的编译，
VS2005里，解决方案资源管理器，找到BSP下RIL文件夹，鼠标右键编译
![clip_image002](http://hi.csdn.net/attachment/201105/11/0_1305083380QoOV.gif)
RILMDD 文件夹，在BSP下的/lib/ARMV4I/retail 下会找到rilmdd.lib
再编译RILPDD 下面的U6100，
这样就得到了最终的RIL驱动：rilenfora.dll，如果想改名，比如叫U6100RIL.dll可以在/RILPDD/U6100下面的source文件 改为TARGETNAME=U6100RIL
###### 3. 修改串口相关
现在市面上的3G模组，一般都支持又COM口（USB接口，虚拟出来的），一个是modem口，一个是AT命令口，所以我们没有必要使用mux驱动。比如现在我们调试的U6100模组，它的USB驱动，会模拟出两个COM，COM6是用于拨号上网的，COM7是用于语音电话的，也就是AT命令口，发短信什么的都是从这个口进行。
微软写好的MDD层（for Enfora Module），默认是使用COM2来作AT操作口的。COM口初始化是在Comhand.cpp文件OpenDownstreamPort()进行的，现在我们要改为COM7，
改修方法：在platform.reg添加
**[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- [HKEY_LOCAL_MACHINE/Drivers/BuiltIn/RIL]// 如果不想开机自动挂载驱动，可以不放在BuiltIn里
- 
- "Dll"="rilgsm.dll"
- 
- "Prefix"="RIL"
- 
- "ComPort"="COM7:" ;AT command port   
- 
- "ComPortNotifications"="COM7:" ; the Notification COM port name  
- 
- "MaxOpenAttempts"=dword:1000; Get the max number of times we should try to open the COM port  
设置COM参数：
Comhand.cpp 文件InitCommState()函数，进行了再次参数设置，可以在这里进行参数修改 ，（也可以去到CComHandle构造函数里面去修改）
InitCommState：修改如下
###### **[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)- dcb.BaudRate = CBR_115200;//m_dwDownstreamBaudRate;- - dcb.fBinary = TRUE;  - - dcb.fParity = TRUE;  - - dcb.fOutxCtsFlow = TRUE;  - - dcb.fOutxDsrFlow = FALSE;  - - dcb.fDtrControl = DTR_CONTROL_DISABLE ;//DTR_CONTROL_ENABLE; //流控制- - dcb.fDsrSensitivity = FALSE;  - - dcb.fTXContinueOnXoff = TRUE;  - - dcb.fOutX = FALSE;  - - dcb.fInX = FALSE;  - - dcb.fErrorChar = FALSE;  - - dcb.fNull = FALSE;  - - dcb.fRtsControl = RTS_CONTROL_DISABLE ;//RTS_CONTROL_HANDSHAKE; // RTS流控制- - dcb.fAbortOnError = FALSE;  - - dcb.ByteSize = 8;  - - dcb.Parity = NOPARITY; //奇偶校验位- - dcb.StopBits = ONESTOPBIT; //停止位4. AT返回关键字移植
###### 模块返回字符串分析流程：
ParseNotification函数（Response.cpp），
进行MDD层的串口返回值分析，识别后给上层上报消息，比如从AT口收到返回字符串“NO CARRIER”，则RIL驱动应该给上面广播
else if (MatchStringAnywhere(szPointer, " NO CARRIER", szPointer))
{
}
ParseString（）函数，
这个函数主要是取得双引号之间的字符串，
例如+CLIP: "15813707036",128,,," ",0
把它输入ParseString，得出的就是的手机号码。
SetupCallListEvaluation（）函数
在这个函数里，发送“AT+CLCC/r”List current calls of ME，查询当前呼叫状况，比如我们现在有电话呼入，然后响铃，在没有接电话之前，每隔一秒发一次这个命令去查询当前状态。得到当前呼入电话的号码等信息
ParseExtRing（）函数
解析RING情况下的额外关键字，也就是说，比如有电话呼入，3G模组会通过串口发送“RING”这样的字符串给AP，
ParseMessageInSim()函数，
解析收到的短信。
拨打电话：
两条线
1）LineMakeCall(TAPI) à RIL_IOControl(rilmain.cpp) à RILDrv_Dial(callctrl.cpp) à
RIL_CALLTYPE_VOICE à new CNotificationData; à InitFromRealBlob à
IndicateCallActivityToAudioSubsystem(设备音频) à
PDD_IndicateCallActivityToAudioSubsystem à QueueCmd(把要发送的AT命令写入队列)
2）发命令线程：CommandThread à 从队列取出AT命令 à SendRILCmdHandleRsp（发送命令“ATD”，启用专门线程HangupThreadProc来处理挂机的问题。）
言归正传，下面针对具体的返回关键字，进行不同的上报，这也是不同的模块RIL驱动移植的工作之一，
###### 1、RING
(1)Response.cpp文件ParseNotification（）
else if (MatchStringBeginning(szPointer, "+CRING: ", szPointer))
改为
**[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- elseif (MatchStringBeginning(szPointer, "RING", szPointer))  
- 
- {  
- 
- SetupCallListEvaluation ();  
- 
- if (!ParseExtRing(szPointer))  
- 
- {  
- 
- goto Error;  
- 
- }  
- 
- }  
（2）ParseExtRing（）
因为我们在接收RING的时候，是没有其他的关键字信息回来的，要把else改为如下
**[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- …  
- 
- else
- 
- {  
- 
- rri.dwCallType = RIL_CALLTYPE_VOICE; // default value
- 
- if (g_rfExternalCalltypeDetermination)  
- 
- {  
- 
- if (!DetermineRingingCalltype(&rri.dwCallType))  
- 
- {  
- 
- m_fUnsolicited = TRUE;  
- 
- return TRUE;  
- 
- }  
- 
- }  
- 
- }  
注掉下面的：
/* rszPointer -= 1; Masked by auly */
如果不注掉，会出现错误如下
RILDrv : i : Failed to find at end of response
因为指针错位不是以回车换行结尾。
###### 2、CONNECT
针对WCDMA模组U6100，当对方接起电话时，它会返回“CONNECT”字符。
当TAPI发送了IOCTL_RIL_DIAL之后，它马上返回了OK，但这样是会让TAPI认为通话已连接。我们要做的是把这个OK屏蔽了，当收到返回的CONNECT，才返回
RIL_RESULT_OK
ParseOKOrError()函数加入 如下代码
**[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- if (!fRet && fOK)  
- 
- {  
- 
- fRet = MatchStringAnywhere(m_szData, "/r/nOK/r/n", szPointer);  
- 
- }  
- 
- if (fRet && m_ATDres)  
- 
- {  
- 
- m_fUnsolicited = FALSE;  
- 
- m_dwCode = (fOK ? RIL_RESULT_OK : RIL_RESULT_ERROR);  
- 
- rcbNewLength = szPointer - m_szData;  
- 
- if (!fOK)  
- 
- {  
- 
- hrError = E_FAIL;  
- 
- if (!SetBlob((void*)&hrError, sizeof(HRESULT)))  
- 
- {  
- 
- fRet = FALSE;  
- 
- goto Error;  
- 
- }  
- 
- RIL_DEMESSAGE((TEXT("ParseOKOrError ---ERROR /r/n")));  
- 
- }  
- 
- else
- 
- {  
- 
- RIL_DEMESSAGE((TEXT("ParseOKOrError ---OK /r/n")));  
- 
- }  
- 
- }  
- 
- elseif(fRet && (!m_ATDres)) // m_ATDres 是个全局变量
- 
- {  
- 
- m_ATDres = TRUE;  
- 
- rcbNewLength = szPointer - m_szData;  
- 
- fRet = FALSE;  
- 
- m_cbLength = 0; //去掉数据
- 
- }  
2)ParseNotification()函数 加入识别出Dial返回的字符串
**[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- elseif(MatchStringBeginning(szPointer, "ATD", szPointer))  
- 
- {  
- 
- RIL_DEBUGMSG(ZONE_ATCMD, (TEXT("ATD ---AULY /r/n")));  
- 
- szPointer +=12 ;   
- 
- m_ATDres = FALSE; //added by auly to void the firs Ok after ATD
- 
- fExpectCRLF = FALSE;  
- 
- m_cbLength = 0; //丢弃本返回字串。
- 
- goto Error;  
- 
- }  
3）识别出”CONNECT”
在ParseNotificationOEM函数
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">elseif(MatchStringBeginning(szPointer, "CONNECT", szPointer))  
- 
- {  
- 
- m_fUnsolicited = FALSE;  
- 
- m_dwCode = RIL_RESULT_OK;  
- 
- }  
- </span>  
作了以上步骤，就能满足：当拨打电话IOCTL_RIL_DIAL之后,对接听，才返回
RIL_RESULT_OK。
###### 3、BUSY
本地拨打，对方挂机：返回“BUSY”
上报消息：RIL_NOTIFY_DISCONNECT
参数：RIL_DISCINIT_BUSY
ParseNotificationOEM（）作如下修改
if (MatchStringBeginning(szPointer, "BUSY", szPointer)) /*本地拨打，对方挂机：返回“BUSY”*/
{ /*对方拨打，本机未接，返回“BUSY”*/
m_fUnsolicited = TRUE;
m_dwCode = RIL_NOTIFY_DISCONNECT;
dwTemp = RIL_DISCINIT_BUSY;//忙音标志
if (!SetBlob((void*)&dwTemp, sizeof(DWORD)))
{
goto Error;
}
fExpectCRLF = FALSE;
}
###### 4、NORMAL END
atd123456789;
OK
CONNECT
NORMAL END
这个关键字一般是在通话接通后对方挂断时返回的，经RILproxy上报
消息：RIL_NOTIFY_DISCONNECT
参数：lpData指向RIL_DISCINIT_REMOTE
###### **[cpp]**[view plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)- elseif(MatchStringAnywhere(szPointer, "NORMAL END", szPointer))/*RIL_NOTIFY_DISCONNECT*/- - {  - - m_fUnsolicited = TRUE;  - - m_dwCode = RIL_NOTIFY_DISCONNECT;  - - dwTemp = RIL_DISCINIT_REMOTE;//表示是对方挂断- - if (!SetBlob((void*)&dwTemp, sizeof(DWORD)))  - - {  - - goto Error;  - - }  - - fExpectCRLF = FALSE;<span style="font-size:16px;">  - }  - </span>  5、CANNOT CONNECT
主叫但对方关机或不在服务区：CANNOT CONNECT
返回消息： RIL_NOTIFY_DISCONNECT
参数：lpData指向RIL_DISCINIT_NETWORKERROR
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">elseif(MatchStringAnywhere(szPointer, "CANNOT CONNECT", szPointer))  
- 
- {  
- 
- m_fUnsolicited = TRUE;  
- 
- m_dwCode = RIL_NOTIFY_DISCONNECT;  
- 
- dwTemp = RIL_DISCINIT_NETWORKERROR;  
- 
- if (!SetBlob((void*)&dwTemp, sizeof(DWORD)))  
- 
- {  
- 
- goto Error;  
- 
- }  
- 
- fExpectCRLF = FALSE;  
- 
- }</span>  
在 RIL里，可以收得到这个消息
###### 6、NO ANSWER
无人接听 ：NO ANSWER
返回消息： RIL_RESULT_NOANSWER
参数：lpData指向NULL
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- elseif(MatchStringAnywhere(szPointer, "NO ANSWER", szPointer))/*RIL_RESULT_NOANSWER*/
- 
- {  
- 
- if(!ParseV25Response(8))  
- 
- {  
- 
- fRetVal = FALSE;  
- 
- goto Error;  
- 
- }  
- 
- fExpectCRLF = FALSE;  
- 
- }  
7、NO CARRIER
返回消息： RIL_RESULT_NOCARRIER
参数：lpData指向NULL
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- elseif(MatchStringAnywhere(szPointer, "NO CARRIER", szPointer))/*NO CARRIER*/
- 
- {  
- 
- if(!ParseV25Response(3))  
- 
- {  
- 
- fRetVal = FALSE;  
- 
- goto Error;  
- 
- }  
- 
- fExpectCRLF = FALSE;  
- 
- }  
8、+CSQ:
信号强度返回串，
at+csq
+CSQ: 24,66
红色是返回串
ParseNotificationOEM监察到关键字，然后
调用Misc.cpp文件里的ParseSignalQualityData（）进行返回字符串分析，提取有效数据。因为格式和Enfora的有区别，所以这里得修改
ParseUInt（）提取数字，并从字符形式转换为数字格式。
ParseDWord（）会把一串连着的字符串数字提取出来并传为数字格式。“138”à 138
在ParseSignalQualityData里，如果提取出信号强度数据，然后把信号分为5档，如下
//now signal state about nRSSI:
// <=4 zero
// 5-8 one
// 9-12 two
// 13-16 three
// 17-31 four
同时，信号强度记录在注册表 [HKEY_LOCAL_MACHINE/System/State/Phone]
SignalStrength2 注册表项。
###### 9、+CLIP
当电话呼入，3G模块会返回如下字符串，
+CLIP: "15813707036",128,,,"8D5673895E73",0
ParseNotification函数，检测到“+CLIP”关键字，调用ParseRemotePartyInfo提取来电话号码等信息。因为格式有所不同，所以移植时，这里得修改。
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- elseif (MatchStringBeginning(szPointer, "+CLIP: ", szPointer))/*提取来电号码*/
- 
- {  
- 
- if (!ParseRemotePartyInfo(szPointer, TRUE))  
- 
- {  
- 
- goto Error;  
- 
- }  
- 
- }  
ParseRemotePartyInfo 分别为“+CLIP”和“+COIP”服务。
在ParseRemotePartyInfo修改如下：
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- BOOL CResponse::ParseRemotePartyInfo(LPCSTR& rszPointer, BOOL fCLIP)  
- 
- {  
- 
- FUNCTION_TRACE(CResponse::ParseRemotePartyInfo);  
- 
- LPCSTR szDummy;  
- 
- UINT nValue;  
- 
- char szAddress[MAXLENGTH_ADDRESS];  
- 
- char szSubAddress[MAXLENGTH_SUBADDR];  
- 
- RILREMOTEPARTYINFO rrpi ; memset(&rrpi,0,sizeof(rrpi)); // zero struct
- 
- BOOL fAddressPresent = FALSE;  
- 
- BOOL fSubAddressPresent = FALSE;  
- 
- BOOL fRet = FALSE;  
- 
- /*
- 
- // Look for a ""
- 
- if (!FindRspPostfix(rszPointer, szDummy))
- 
- {
- 
- // This isn't a complete Caller ID notification -- no need to parse it
- 
- goto Error;
- 
- }
- 
- */
- 
- rrpi.cbSize = sizeof(RILREMOTEPARTYINFO);// Parse "
- /*get the phone number*/
- if (ParseString(rszPointer, szAddress, MAXLENGTH_ADDRESS, rszPointer)) {   
- fAddressPresent = TRUE;   
- } // Parse "," 
- /*find another ","*/
- if (!MatchStringBeginning(rszPointer, ",", rszPointer)) {   
- goto Error;   
- } // Parse "" 
- if (ParseUIntAndVerifyAbove(rszPointer, FALSE, 0x100, nValue, rszPointer) && fAddressPresent && StringToRILAddress(szAddress, (BYTE)nValue, rrpi.raAddress)) {   
- rrpi.dwParams |= RIL_PARAM_RPI_ADDRESS;   
- } // Parse "," 
- 
- if (MatchStringBeginning(rszPointer, ",", rszPointer)) { // Parse "" 
- 
- if (ParseString(rszPointer, szSubAddress, MAXLENGTH_SUBADDR, rszPointer)) {   
- fSubAddressPresent = TRUE;   
- } // Parse "," 
- 
- if (!MatchStringBeginning(rszPointer, ",", rszPointer)) {   
- goto Error;   
- } // Parse "" 
- if (ParseUIntAndVerifyAbove(rszPointer, FALSE, 0x100, nValue, rszPointer) && fSubAddressPresent && StringToRILSubAddress(szSubAddress, (BYTE)nValue, rrpi.rsaSubAddress)) {   
- rrpi.dwParams |= RIL_PARAM_RPI_SUBADDRESS;   
- } // Parse "," 
- 
- if (MatchStringBeginning(rszPointer, ",", rszPointer)) { // Parse "" 
- 
- if (ParseQuotedEncodedString( g_rppPDDParams->etEncodingTECharset, rszPointer, rrpi.wszDescription, rrpi.wszDescription + MAXLENGTH_DESCRIPTION)) {   
- rrpi.dwParams |= RIL_PARAM_RPI_DESCRIPTION;   
- } // Parse "," 
- if (fCLIP && MatchStringBeginning(rszPointer, ",", rszPointer)) { // Parse "" 
- 
- if (!ParseUIntAndVerifyAbove(rszPointer, FALSE, NUM_VALIDITIES, nValue, rszPointer)) {   
- goto Error;   
- }   
- rrpi.dwValidity = g_rgdwValidities[nValue];   
- 
- rrpi.dwParams |= RIL_PARAM_RPI_VALIDITY;   
- }   
- }   
- }   
- 
- if ((TRUE == fCLIP) && (g_rfExternalCalltypeDetermination)) {   
- 
- EnterCriticalSection(&g_csRingingCallData);   
- 
- if (g_rcdRingingCallData.fDelayRingNotification)   
- { // there is a ring notification waiting, get the calltype 
- g_rcdRingingCallData.dwCalltype = GetCalltypeFromRemotePartyInfo(&rrpi);   
- g_rcdRingingCallData.fCalltypeValid = TRUE;   
- g_rcdRingingCallData.fDelayRingNotification = FALSE;   
- g_rcdRingingCallData.fForceRingNotification = TRUE;   
- }   
- RIL_DEBUGMSG(ZONE_INFO, (TEXT("RILDrv : i : ParseRemotePartyInfo : Ringing Call TypeValid = %d, type = %d, Delay = %d, Force = %d/r/n"), g_rcdRingingCallData.fCalltypeValid, g_rcdRingingCallData.dwCalltype,g_rcdRingingCallData.fDelayRingNotification,g_rcdRingingCallData.fForceRingNotification));   
- LeaveCriticalSection(&g_csRingingCallData);   
- }   
- m_fUnsolicited = TRUE;   
- m_dwCode = (fCLIP ? RIL_NOTIFY_CALLERID : RIL_NOTIFY_DIALEDID);   
- if (!SetBlob((void*)&rrpi, sizeof(RILREMOTEPARTYINFO))) {   
- goto Error;   
- }   
- fRet = TRUE;  
-  Error: return fRet;   
- }   
###### 5. 返回OK或ERROR
Response.cpp 里ParseOKOrError()函数，识别respose串是返回的OK或ERROR
//LPCSTR szCode = (fOK ? "0/r" : "4/r");
//修改为
LPCSTR szCode = (fOK ? "OK/r/n" : "ERROR/r/n");
然后要修一些相应修改，这里不细说，请看代码。
这样才适合我们使有用的模块返回规则。
###### 6. 拨号移植
"ATD"(呼出)有点麻烦，对方接起后才返回"OK"，在此之前可能主叫方挂断， 
也就是"ATD"在有回应之前下"ATH"指令。 
因此comhand.cpp的CComHandle::SendRILCmdHandleRsp中对"ATD"和"ATA"有特殊处理， 
启动专门的线程HangupThreadProc处理"ATH"。 
->HangupThreadProc() 
情况1："ATD"/"ATA"指令还没下给串口 
从g_pCmdQ中移走pCmd 
    pCmd->SendResponse(RIL_RESULT_NOCARRIER,NULL,0); 
情况2：已经下给了串口 
假如g_pCmdQ的下个命令是"ATH"，因为g_pCmdQ是CPriorityQueue，"ATH"会排在最前 
      m_fCancelledDial=1 
写"AT"到串口 
对于上述情况2 
comhand.cpp的CComHandle::SendRILCmdHandleRsp() 
在获得pRsp("AT"的返回值"OK")后，判断m_fCancelledDial==1 
调用pRsp->SetCallAborted()把"OK"的RESULT换成"NOCARRIER"，此后按原流程处理。 
"ATH"的指令未被丢弃，随后会再次发出，来防止对方"ATA"发生在"ATD"被取消的同时。 
假如有模块的"ATD"直接返回"OK"，那就不需要线程HangupThreadProc， 
把RILDrv_Dial()的pnd去掉，不然收到"OK"就会BroadcastNotification(RIL_NOTIFY_CONNECT)。 
另外response.cpp的CResponse::ParseNotification()可能也得加点东西。
龙尚的U6100模块，就是当下发了ATD命令后。就马上返回OK的，所以不需要
HangupThreadProc线程，把RILDrv_Dial()的pnd去掉，不然收到"OK"就会
BroadcastNotification(RIL_NOTIFY_CONNECT)。误以为 已接通
###### 7. 挂机功能移植
原来是发送ATH命令，现要改为发送：AT+CHUP/r/n 命令
Callctrl.cpp
RILDrv_Hangup()函数，改为
else if (!QueueCmd(pHandle, "AT+CHUP/r/n", CMDOPT_HANGUP, APIID_HANGUP, NULL, pnd, hr))
移植中常见问题解决
###### 串口发送问题
问题现象：
串口接收没有问题，但是发送却一直在Comhand.cpp文件Write函数里发送失败，表现为
DOWNST_WRITEFILE(m_hDownstream, pBuf, cb, &dwDownstreamWritten)
dwDownstreamWritten一直是返回0，
解决办法：
因为我们现在使用的串口是3线制，CE提供的RIL MDD驱动，是默认9线的COM。我们在DCD中 把流控制，DTR等等设置关闭。
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">dcb.BaudRate = CBR_115200;//m_dwDownstreamBaudRate;
- 
- dcb.fBinary = TRUE;  
- 
- dcb.fParity = TRUE;  
- 
- dcb.fOutxCtsFlow = TRUE;  
- 
- dcb.fOutxDsrFlow = FALSE;  
- 
- dcb.fDtrControl = DTR_CONTROL_DISABLE;//DTR_CONTROL_ENABLE;
- 
- dcb.fDsrSensitivity = FALSE;  
- 
- dcb.fTXContinueOnXoff = TRUE;  
- 
- dcb.fOutX = FALSE;  
- 
- dcb.fInX = FALSE;  
- 
- dcb.fErrorChar = FALSE;  
- 
- dcb.fNull = FALSE;  
- 
- dcb.fRtsControl = RTS_CONTROL_DISABLE;//RTS_CONTROL_HANDSHAKE;
- 
- dcb.fAbortOnError = FALSE;  
- 
- dcb.ByteSize = 8;  
- 
- dcb.Parity = NOPARITY;// 无奇偶校验位
- 
- dcb.StopBits = ONESTOPBIT;//1 停止位
- 
- 改为：  
- 
- dcb.BaudRate = CBR_115200;//m_dwDownstreamBaudRate;
- 
- dcb.fBinary = TRUE;  
- 
- dcb.fParity = FALSE;  
- 
- dcb.fOutxCtsFlow = FALSE;  
- 
- dcb.fOutxDsrFlow = FALSE;  
- 
- dcb.fDtrControl = DTR_CONTROL_DISABLE;//DTR_CONTROL_ENABLE;
- 
- dcb.fDsrSensitivity = FALSE;  
- 
- dcb.fTXContinueOnXoff = FALSE;  
- 
- dcb.fOutX = FALSE;  
- 
- dcb.fInX = FALSE;  
- 
- dcb.fErrorChar = FALSE;  
- 
- dcb.fNull = FALSE;  
- 
- dcb.fRtsControl = RTS_CONTROL_DISABLE;//RTS_CONTROL_HANDSHAKE;
- 
- dcb.fAbortOnError = FALSE;  
- 
- dcb.ByteSize = 8;  
- 
- dcb.Parity = NOPARITY;// 无奇偶校验位
- 
- dcb.StopBits = ONESTOPBIT;//1 停止位</span>
问题解决!
###### 读串口数据，会有积累问题
问题现象：
3G模块返回的信息，会被RIL驱动中的线程ResponseThread调用Read()读进来，然传入HandleRxData进行分析，有个问题就是，在里面分析返回串的时候，把返回字串传给ParseNotification()函数进行分析，在Parse()函数，打印的调试信息会发现，信息有累积效应，而且，当累积到一定程度，新进来的字符串，会被丢弃掉,如下
前一串返回
串口读到的字串: +CLIP: "15813707036",128,,,"8D5673895E73",0
传给分析函数的字串: network:edge+CREG: 1, 2540, 79BD+CLIP: "15813707036",128,,,"8D5673895E73",0
第二串返回
串口读到的字串: RING
传给分析函数的字串:network:edge+CREG: 1, 2540, 79BD+CLIP: "15813707036",128,,,"8D5673895E73",0RING
//传给分析函数的字串会把串口读到的加在后面
原因分析：
在分析返回字串的过程，它涉及到的几个函数之间（Append,Parse，ParseNotification等），是通过全局指针m_szData来传递的，在函数Append（）里，把读到的数据，放入分析串。
memcpy(m_szData + m_cbLength, szString, cbString);
m_cbLength += cbString;
HandleRxData àAppendReadBytes把串口数据放入缓冲区
à GiveUpReadBytes 循环调用该函数，获得所有的串口数据。读完串口数据，从这里跳出循环。
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">if (!cbAppend)  
- 
- {  
- 
- RIL_DEBUGMSG(1, (TEXT("RILDrv :HandleRxData -----1 /r/n")));  
- 
- // Free memory occupied by szAppend
- 
- delete[] szAppend;  
- 
- szAppend = NULL;  
- 
- break;  
- 
- }</span>  
而在RIL认为没有读完数据前，它会在while GiveUpReadBytes 里面一直循环，把读到的数据不断加在后面。这现象就是前面遇到的问题。本来这不是问题，关键在于，返回的一些字符，RIL驱动判断出错，所以还是RIL移植没做好的工作。
其实，决定在不在while GiveUpReadBytes 里循环获取全部串口数据的，是在
szRemainder指针，它是传入AppendString（）第三个参数。如果Parse返回假，则新旧数据接起来，也就是不清除原来的老数据，把新读回来的字串加在原来串的后面。
如果rszRemainder 不为NULL（Parse返回真），则进行发送response 的操作。重新开始，也就是传给系统分析的字串是刚从串口接收到的。跳出GiveUpReadBytes的询环。
如果rszRemainder 为NULL，则继续等待串口有更多的数据进来。跳出循环。
串口新收到一条返回，它的流程是：ResponseThread线程中，等到了COM口中断，然后调 用read（）去读回字符串，然后调用HandleRxData进行处理,在这函数里面，调用了
AppendReadBytes
ResponseThread线程
àread
àHandleRxData()
àAppendReadBytes() 把串口数据放入缓冲区
àAppend()
àAppend() new 新建缓冲区buffer存放返回串。
｛循环//获得全部的串口数据（针对某一功能的返回值新旧数据加起来）
àGiveUpReadBytes()
循环接收COM读到的字符串，比如有些返回信息是要发几个COM中断发送过来的，
àAppendString()
àAppend()
àAppend()新建buffer存放完整的返回串。该串是给系统分析用的。然后把read()读回来的字串拷贝进去。
à Parse() 分析返回串（注意，此处的字符串有可能是接收了几次串口的新老数据加在一起），
à ParseNotification() //进行MDD层解析
à ParseRspPrefix() //检查返回字符串完整性（是否有回车换行）
然后进行关键字识别，如“+CLIP:”“+COLP:”“+CMT:”等等，
两种情况下返回TRUE：（1）不产生Error,
（2）有关键字匹配，但产生Error，并且m_fPotentialBogusResponse条件成立（过虑有问题的response），同时，在返回串里有"/r"，才会返回TRUE,
返回FALSE：只要是产生了ERROR,并且m_fPotentialBogusResponse不成立，都会返回FALSE
Parse返回值：
TRUE，表示该返回已接收完，清除COM读回来的老数据
FALSE 还没收完，等一个COM中断进来的数据，要加在后面。
｝
解决方法：
这个问题要解决：
AppendString（）函数里，当Parse返回FALSE时，加入：m_cbLength = 0;
还得在Parse函数解决。如果不想要旧数据出现在比较串中，那么这个函数不返回FALSE，就不会累积旧数据了。
###### RING 为什么会
(1) RING 为什么会“RILDrv : i : Failed to find at end of response”
原因：因为在ParseNotification里找到了关键字“RING”，此时已把RING前面的一段给去掉了，只剩下后面的， 然后进入ParseExtRing()，找了一系列关键字都没有找到，最后进后FindRspPostfix（），把最后一个回车换行给去了，所以在后面
ParseRspPostfix（）里，找不到回车换行 ，就出现上面的错误，结果就一直以为没有接收完
###### RING返回问题
当有电话呼入，3G模块U6100会通过COM口传“RING”给AP，所以，RIL驱动检测到这个关键字，要给上层上报RIL_NOTIFY_RING的消息，但是WINCE6.0里微软给写好的RIL驱动MDD层里，原来给Enfora写的检测方式，并不适合我们现在的模块，所以，要修改，这也是植移的工作之一，方法如下
（1）
Response.cpp文件ParseNotification（）
else if (MatchStringBeginning(szPointer, "+CRING: ", szPointer))
改为
else if (MatchStringBeginning(szPointer, "RING", szPointer))
（2）ParseExtRing（）
因为我们在接收RING的时候，是没有其他的关键字信息回来的，要把else改为如下
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">…  
- 
- else
- 
- {  
- 
- rri.dwCallType = RIL_CALLTYPE_VOICE; // default value
- 
- if (g_rfExternalCalltypeDetermination)  
- 
- {  
- 
- if (!DetermineRingingCalltype(&rri.dwCallType))  
- 
- {  
- 
- m_fUnsolicited = TRUE;  
- 
- return TRUE;  
- 
- }  
- 
- }  
- 
- }</span>  
注掉下面的：
/* rszPointer -= 1; Masked by auly */
如果不注掉，会出现错误如下
RILDrv : i : Failed to find at end of response
因为指针错位不是以回车换行结尾。
###### fExpectCRLF
在response.cpp文件ParseNotification（）函数，
fExpectCRLF = TRUE; //处理的是以"/r/n" 开头的返回串，实际就是要去掉两个字节
fExpectCRLF = FALSE; //表示返回串是以一个换行符号开头的"/r"
具体实现是
**[cpp]**[view
 plain](http://blog.csdn.net/songjinshi/article/details/7311990#)[copy](http://blog.csdn.net/songjinshi/article/details/7311990#)
- <span style="font-size:16px;">if (fExpectCRLF)  
- 
- {  
- 
- if (!ParseRspPostfix(szPointer, szPointer))  
- 
- {  
- 
- RETAILMSG(ZONE_ATCMD, (TEXT("RILDrv : i : Failed to find  at end of response/r/n")));  
- 
- goto Error;  
- 
- }  
- 
- }  
- 
- elseif (!MatchStringBeginning(szPointer, "/r", szPointer))  
- 
- {  
- 
- RETAILMSG(ZONE_ATCMD, (TEXT("RILDrv : i : Failed to find  at end of response/r/n")));  
- 
- goto Error;  
- 
- }</span>  
/r仅仅是回车 
/n是换行 ，如果不换行，当下一行有内容时，光标是在这些内容的后面的。
