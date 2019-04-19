# 用Ras的API函数创建一个拨号连接 - xqhrs232的专栏 - CSDN博客
2013年01月22日 20:44:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：630
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://www.biancheng521.com/article/draagon101788/7021350.html](http://www.biancheng521.com/article/draagon101788/7021350.html)
   很久以前我们的项目中曾提到过一个需求，因为拨号连接的设置过于繁琐，希望能够在我们的程序中提供输入几个必要的参数，然后自动生成一个拨号连接。当时我对于RAS也是刚刚接触（其实到现在也只是刚刚入了点门），通过查阅资料知道RasSetEntryProperties加上RasSetEntryDialParams可以实现（如果不清楚的，可以查阅MSDN，里面解释的比较清楚），但是这种方法貌似设置不了modem的串口属性、附加拨号命令等一些信息，这样的话还是解决不了我们的问题，因为我们需要的简化的其实就是让用户输入附加拨号命令（就是+CGDCONT=1，"IP"，"CMNET"，0，0，0那么一长串，其实只需要用户输入一个APN就OK了）。
     没办法，深入研究贝~MSDN上关于RasSetEntryProperties函数中的倒数第二个参数的解释说明是说它是一个指向包含设备配置信息的缓冲区的指针，但是要查阅关于这个配置信息的更多详细内容，就要看TAPI相关的lineGetDevConfig函数，说实话，TAPI我那次是第一次看到，从来没见过，根据MSDN的连接，查看了一下lineGetDevConfig函数，结果越看越不明白，光是一个line device我就不知道是什么东东~（现在也还不知道:(），看了半天没明白也懒得浪费时间了，不就是一个指针吗，不就是一个buffer吗？而且还是有另外一个参数标明了大小的，怕什么，把这段buffer的数据打印出来我猜总可以了吧~就这样，我打印一次buffer的数据，再把拨号连接的属性设置改了几个地方，再打印一下，对比来看，最后让我弄出个结构定义：
typedef struct {                                     //Offset
 unsigned short Reserved1;                   // 0x00,       通常为0x20 
 unsigned short WaitForCreditCard;     // 0x02
 unsigned short CancelTimeOut;           // 0x04
 unsigned short Reserved2;                  // 0x06
 unsigned char  Test1;                          // 0x08
 unsigned char  Test2;                          // 0x09
 unsigned short Reserved3;                  // 0x0A
 unsigned int   BaudRate;                     // 0x0C
 unsigned short Terminal;                     // 0x10， 通常为0, 1, 3, 7
 unsigned char  DataBits;                     // 0x12
 unsigned char  StopBits;                     // 0x13
 unsigned char  Parity;                         // 0x14
 unsigned char  FlowControl;               // 0x15
 TCHAR    AtCmd[115];                    // 0x16， 可能后面还有别的结构，但我尝试几次都没有看到后面有有效值，因此笼统定义为附加指令
}   sDevConfig;
    虽然还有些参数没有猜出来，但是这些也够用了。用这个结构在AtCmd中填入附加拨号命令，就可以了（后来发现这个还有一个好处，在PDA上设置附加拨号命令的时候有长度限制，某次用TD的手机设置拨号上网的时候，附加命令特别长，除了上面cgdcont一行还另外加了一句，结果因为长度限制不能够完全输入，但是用这个结构来设置，就可以突破CE系统中的拨号设置中的那个长度限制了）。
    用RasSetEntryProperties设置一个Entry的属性的时候，如果Entry的参数中指定的EntryName已经存在了，那么就更改已有的Entry的设置，如果不存在，则创建了一个新的Entry，这样就实现了创建拨号连接的功能，但是之后你马上会发现，在系统的拨号设置界面中找不到用这个方法创建的拨号连接，但是使用RasEnumEntries却是能够枚举出这个连接的。当时也没研究出这个问题，时间上也不允许，就这么搁置了。反正设置、创建拨号连接的功能实现了，而且我们的软件中使用了RasEnumEntries来枚举拨号连接也能够枚举出自定义创建的拨号连接。
     到今天，差不多时间也过了快一年了。又碰到了拨号设置的需求问题，正好最近我成了“闲人”，这事又落到我肩膀上~今天又把之前的“成果”拿出来看了看，感觉首要解决的就是要把我们自己创建的拨号连接在系统的拨号连接设置界面中显示出来。从什么地方入手呢？当时研究Ras系列的函数的时候，确实没有发现跟这个相关的设置啊~没法了，看看注册表吧。
     我先使用系统设置中的连接设置，在Internet设置中新建了一个名叫“我的连接”的拨号连接，然后用我的小程序创建了另外一个拨号连接（名称随意，只要不叫“我的连接”），完成之后，我就在wince的注册表中搜索“我的连接”，结果还真让我找到一个，在HKCU\comm\RasBook\下面有一个叫我的连接的主键，但是我用程序创建的另外一个连接的名字也在RasBook下面有一个对应的主键，那就只有继续搜索咯~又找到一个，在HKLM\Software\Microsoft\ConnMgr\Providers\{7C4B7A38-5FF7-4bc1-80F6-5DA7870BB1AA}\Connections下面有一个主键叫“我的连接”，但是却没有和我的程序创建的连接同名的主键，那么估计就是这里了，再继续搜索一下也没有匹配的结果了，看来这里的可能性非常大。看看这个主键的内容，它下面有三个DWORD的值，一个String类型值，
RequirePw （DWORD） 1
Enabled (DWORD)            1
EntryType (DWORD)         0
DestId   (String)                 "{ADB0B001-10B5-3F39-27C6-9742E785FCD4}"
明显，这个DestId是一个GUID，而且用过ConnMgr系列API的就应该知道DestId这个东东，可以简单的认为它是一个标示拨号网络类型的GUID，和这个Connections同一级的主键当中有一个叫Destinations的主键，展开，它的子键都是一些设置类型包括：“Internet 设置”，“Wap 网络”，“安全 Wap 网络”等，这里就不全部列举了，有兴趣的朋友可以自己看看。打开“Internet 设置”主键，果然也有一个叫DestId的字符串，且值和“我的连接”的DestId相同，那么显然就是这个了，我试着在“Connections”下面建立了一个和我自己程序创建的连接同名的主键，然后在其下加入一个DestId字符串，值就是Internet的DestId，但是打开系统设置-〉连接-〉Internet设置-管理现有连接
 还是没有看到我自己创建的连接，只好把“我的连接”主键的几个值如法炮制全给我自己的连接主键加上了，再看，嘿~出来了~~~
     那么怎么在程序里实现把自己创建的连接显示出来的思路应该很清晰了吧，写写注册表就OK啦~ ：P
### 评论
# re: 用Ras的API函数创建一个拨号连接 2007-08-01 10:54 NULL
楼主的开源精神值的表演 
这个问题我也找了许久，但网上几乎没有资料 
感谢楼主的奉献  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2007-08-09 11:40 SuperPlayeR
一个朋友研究了在smartphone上建立GPRS拨号连接的情况，贴出一些代码供需要的朋友参考： 
http://cpp.sourceforge.net/?show=39205 
怕这个链接以后会失效，我还是先copy一份过来。 
#include "stdafx.h" 
#include <wchar.h> 
#include <tapi.h> 
#include <tsp.h> 
#include <ras.h> 
#include <raserror.h> 
#include <vector> 
DWORD FindCellLine() 
{ 
LONG lRet = 0; 
DWORD dwDeviceNum = 0; 
DWORD dwDeviceID = -1, dwTapiVersion = TAPI_CURRENT_VERSION; 
HLINEAPP hLineApp = NULL; 
LINEINITIALIZEEXPARAMS initParams = {0}; 
initParams.dwTotalSize = sizeof(initParams); 
initParams.dwOptions = LINEINITIALIZEEXOPTION_USEEVENT; 
std::vector<char> buffer(1024, '\0'); 
LPLINEDEVCAPS devcaps =(LINEDEVCAPS*) &buffer[0]; 
devcaps->dwTotalSize = buffer.size(); 
lRet = lineInitializeEx(&hLineApp, GetModuleHandle(NULL), NULL, 
L"GPRS Device Finder", &dwDeviceNum, &dwTapiVersion, &initParams); 
if (lRet != 0) 
return -1; 
for(DWORD dwDeviceIndex = 0; dwDeviceIndex != dwDeviceNum; dwDeviceIndex++) 
{ 
lineGetDevCaps(hLineApp, dwDeviceIndex, dwTapiVersion, 0, devcaps); 
if (devcaps->dwNeededSize > devcaps->dwTotalSize) 
{ 
buffer.resize(devcaps->dwNeededSize); 
devcaps =(LINEDEVCAPS*) &buffer[0]; 
devcaps->dwTotalSize = buffer.size(); 
dwDeviceIndex--; 
continue; 
} 
if (wcscmp((LPCTSTR) &buffer[devcaps->dwLineNameOffset], 
CELLTSP_LINENAME_STRING) == 0) 
{ 
dwDeviceID = dwDeviceIndex; 
break; 
} 
} 
lineShutdown(hLineApp); 
return dwDeviceID; 
} 
BOOL SetupGprsRasEntry(LPWSTR szEntryName, LPCWSTR szAccessPointName) 
{ 
DWORD dwLineID = 0; 
std::vector<char> buffer(sizeof(VARSTRING) + sizeof(CELLDEVCONFIG), '\0'); 
LPVARSTRING varbuffer =(LPVARSTRING) &buffer[0]; 
varbuffer->dwTotalSize = buffer.size(); 
LPCELLDEVCONFIG cellcfg = NULL; 
{//查找Cell Line的设备ID 
if ((dwLineID = FindCellLine()) == -1) 
return FALSE; 
} 
{//获得默认数据配置 
if (lineGetDevConfig(dwLineID, varbuffer, L"comm/datamodem") != 0) 
return FALSE; 
cellcfg =(LPCELLDEVCONFIG) &buffer[varbuffer->dwStringOffset]; 
} 
{//开始配置GPRS数据 
cellcfg->bGPRSConnectionInfoValid = TRUE; 
memset(&cellcfg->cgciGPRSConnectionInfo, 0x00, 
sizeof(cellcfg->cgciGPRSConnectionInfo)); 
wcscpy(cellcfg->cgciGPRSConnectionInfo.wszAccessPointName, szAccessPointName); 
cellcfg->cgciGPRSConnectionInfo.dwProtocolType = CELLDEVCONFIG_GPRSPROTOCOL_IP; 
cellcfg->cgciGPRSConnectionInfo.dwL2ProtocolType = CELLDEVCONFIG_GPRSL2PROTOCOL_PPP; 
cellcfg->cgciGPRSConnectionInfo.dwDataCompression = CELLDEVCONFIG_GPRSCOMPRESSION_OFF; 
cellcfg->cgciGPRSConnectionInfo.dwHeaderCompression = CELLDEVCONFIG_GPRSCOMPRESSION_OFF; 
} 
{//安装GPRS拨号项 
RASENTRY rsaentry = {0}; 
rsaentry.dwSize = sizeof(rsaentry); 
rsaentry.dwfOptions = RASEO_ProhibitEAP|RASEO_RemoteDefaultGateway; 
rsaentry.dwfNetProtocols = RASNP_Ip; 
rsaentry.dwFramingProtocol = RASFP_Ppp; 
wcscpy(rsaentry.szDeviceName, CELLTSP_LINENAME_STRING); 
wcscpy(rsaentry.szDeviceType, RASDT_Modem); 
wcscpy(rsaentry.szLocalPhoneNumber, L"~GPRS!"); 
if (RasSetEntryProperties(NULL, szEntryName, &rsaentry, sizeof(rsaentry), 
(LPBYTE) cellcfg, sizeof(*cellcfg)) != 0) 
return FALSE; 
} 
return TRUE; 
} 
  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2007-09-10 21:57 scott
非常感谢  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2007-09-11 11:43 SuperPlayeR
后来发现在Mobile5上，注册表结构发生了变化 
HKLM\Software\Microsoft\ConnMgr\Providers\{7C4B7A38-5FF7-4bc1-80F6-5DA7870BB1AA}\Connections 
变成了 
HKLM\Comm\ConnMgr\Providers\{7C4B7A38-5FF7-4bc1-80F6-5DA7870BB1AA}\Connections 
而且连接的键值中多了一个GUID，目前还不知道如何生成或者获取的。  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2008-01-30 14:18 PF775
大大请问一下 : 
下面这些参数是lineGetDevConfig( dwDeviceID, lpDeviceConfig,lpszDeviceClass)的第几个参数项 感谢 
typedef struct { //Offset 
unsigned short Reserved1; // 0x00 
………………………………. 
} sDevConfig; 
  回复  更多评论 
# 用ＡＰＩ设置拨号模式，希望有帮助 2008-03-12 11:37 kiapper
// Set the dialing patterns as we need.
// Unfortunately, there is no documented API for modifying the dialing parameters
// in Windows CE. All of the dialing parameters/patterns are stored in the registry,
// so we have to finish our work by operating registry.
BOOL RasSetDialingPatterns(VOID)
{
HKEY hKey;
DWORD dataType = REG_MULTI_SZ;
TCHAR szKeyName[] = TEXT("0");
LONG retVal = 0;
LONG ConfigurationStringSize = 512;
TCHAR ConfigurationString[512];
memset(ConfigurationString, '\0', 512);
// Build the configuration string.
wcscat(ConfigurationString, TEXT("Work")); // dialing from
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("G")); // local calls
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("G")); // long distance calls
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("G")); // international calls
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("021")); // local area code
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("")); // disable call waiting by dialing
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("86")); // country/region code
wcscat(ConfigurationString, TEXT("$"));
wcscat(ConfigurationString, TEXT("0")); // tone or pulse
wcscat(ConfigurationString, TEXT("$"));
// Format the string correctly, take care please
int ConfigStringLen = wcslen(ConfigurationString);
for (int x = 0; x < ConfigStringLen; x++)
{
if (ConfigurationString[x] == '$')
{
ConfigurationString[x] = '\0';
}
}
// Open the registry key.
retVal = RegOpenKeyEx(HKEY_CURRENT_USER , TEXT("ControlPanel\\Dial\\Locations"), 0, KEY_READ, &hKey);
if (retVal != ERROR_SUCCESS)
{
return FALSE;
}
// Set the value of the key.
retVal = RegSetValueEx(hKey, szKeyName, NULL, dataType, (PBYTE)ConfigurationString, ConfigurationStringSize);
RegCloseKey(hKey);
if (retVal != ERROR_SUCCESS)
{
return FALSE;
}
else
{
return TRUE;
}
}  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2008-03-13 12:32 kiapper
在WinCE中，拨号选项的默认值写在注册表common.reg [HKEY_LOCAL_MACHINE\Drivers\Unimodem] "DevConfig"键下，具体格式如下：
#define DEVMINCFG_VERSION 0x0030
//
// Version 0x0030 changes:
// - increase the size of szDialModifier
//
#define DIAL_MODIFIER_LEN 256
#define MAX_CFG_BLOB 126
#define MAX_NAME_LENGTH 8
typedef struct tagDEVMINICFG {
WORD wVersion;
WORD wWaitBong; // DevCfgHdr
DWORD dwCallSetupFailTimer; // CommConfig.ModemSettings
DWORD dwModemOptions; // CommConfig.ModemSettings
// MDM_BLIND_DIAL MDM_FLOWCONTROL_SOFT
// MDM_CCITT_OVERRIDE MDM_FORCED_EC
// MDM_CELLULAR MDM_SPEED_ADJUST
// MDM_COMPRESSION MDM_TONE_DIAL
// MDM_ERROR_CONTROL MDM_V23_OVERRIDE
// MDM_FLOWCONTROL_HARD
DWORD dwBaudRate; // DCB
WORD fwOptions; // DevCfgHdr
// TERMINAL_PRE TERMINAL_POST
// MANUAL_DIAL
BYTE ByteSize; // DCB
BYTE StopBits; // DCB
BYTE Parity; // DCB
WCHAR szDialModifier[DIAL_MODIFIER_LEN+1]; // Unique to MiniCfg
// Dynamic devices configuration
WCHAR wszDriverName[MAX_NAME_LENGTH+1];
BYTE pConfigBlob[MAX_CFG_BLOB];
HANDLE hPort;
} DEVMINICFG, *PDEVMINICFG; 
格式因版本不同而有所不同。
结构中szDialModifier字段就是博主所说的AtCmd。
注册表的值就是以上结构的二进制数值表示形式。
  回复  更多评论 
# re: 用Ras的API函数创建一个拨号连接 2008-07-03 15:43 Jemini
楼主说的是HKLM\Comm\Rasbook下面各主键下的Devcfg，而楼上说的是Unimodem下的DevConfig，这两个是不一样的。  回复  更多评论 
