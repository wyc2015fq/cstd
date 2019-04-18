# 网络公共函数（大小端字节转换、网卡/ip信息获取等。） - 深之JohnChen的专栏 - CSDN博客

2017年11月14日 13:05:10[byxdaz](https://me.csdn.net/byxdaz)阅读数：1026


网络公共函数（大小端字节转换、网卡/ip信息获取等。）

//NetCommmonFunction.h

```cpp
#pragma once

#include <Windows.h>
#include <list>
#include <string>

//ip地址信息
typedef struct tagIpInfo{
	char szIp[128];
	char szMask[128];
	std::list<std::string> listGateway;
}IpInfo;

//网卡信息
typedef struct tagNetAdapter{
	char AdapterName[256];			//网卡名称
	char Description[256];			//描述
	char szMacAddr[20];				//
	std::list<IpInfo> listIp;		//ip列表
}NetAdapter;

#define BigtoLittle16(A)   (( ((unsigned short)(A) & 0xff00) >> 8)    | \
	(( (unsigned short)(A) & 0x00ff) << 8))

#define LittletoBig16(A)   (( ((unsigned short)(A) & 0x00ff) << 8)    | \
	(( (unsigned short)(A) & 0xff00) >> 8))

#define BigtoLittle32(A)   ((( (unsigned int)(A) & 0xff000000) >> 24) | \
	(( (unsigned int)(A) & 0x00ff0000) >> 8)   | \
	(( (unsigned int)(A) & 0x0000ff00) << 8)   | \
	(( (unsigned int)(A) & 0x000000ff) << 24))

#define LittletoBig32(A)   ((( (unsigned int)(A) & 0x000000ffUL) << 24) | \
	(( (unsigned int)(A) & 0x0000ff00UL) << 8)   | \
	(( (unsigned int)(A) & 0x00ff0000UL) >> 8)   | \
	(( (unsigned int)(A) & 0xff000000UL) >> 24))

#define _WS2_32_WINSOCK_SWAP_LONG(l)                \
            ( ( ((l) >> 24) & 0x000000FFL ) |       \
            ( ((l) >>  8) & 0x0000FF00L ) |       \
            ( ((l) <<  8) & 0x00FF0000L ) |       \
            ( ((l) << 24) & 0xFF000000L ) )
#define _WS2_32_WINSOCK_SWAP_LONGLONG(l)           \
            ( ( ((l) >> 56) & 0x00000000000000FFLL ) |       \
            ( ((l) >> 40) & 0x000000000000FF00LL ) |       \
            ( ((l) >> 24) & 0x0000000000FF0000LL ) |       \
            ( ((l) >>  8) & 0x00000000FF000000LL ) |       \
            ( ((l) <<  8) & 0x000000FF00000000LL ) |       \
            ( ((l) << 24) & 0x0000FF0000000000LL ) |       \
          	( ((l) << 40) & 0x00FF000000000000LL ) |       \
          	( ((l) << 56) & 0xFF00000000000000LL ) )

/*功能：当前电脑CPU是否大端模式参数：无返回：大端对齐标志*/
BOOL LocalCpuIsBigEndian();

/*功能：获取大端对齐标志参数：无返回：大端对齐标志*
/BOOL& GetBigEndianFlag();

/*功能：设置大端对齐标志参数：bBigEndian[in]:是否为大端对齐(TRUE:大端,FALSE:小端(默认))返回：无*/
void SetBigEndian(BOOL bBigEndian);

/*功能：unsigned long大小端对齐转换参数：hostlong[in]:转换前数据返回：转换结果*/
unsigned long h2n_conver_ulong(unsigned long hostlong);unsigned long n2h_conver_ulong(unsigned long netlong);

/*功能：unsigned short大小端对齐转换参数：hostshort[in]:转换前数据返回：转换结果*/
unsigned short h2n_convert_ushort(unsigned short hostshort);
unsigned short n2h_convert_ushort(unsigned short netshort);

/*功能：unsigned long long大小端对齐转换参数：hostlonglong[in]:转换前数据返回：转换结果*/
unsigned long long h2n_convert_ulonglong(unsigned long long hostlonglong);
unsigned long long n2h_convert_ulonglong(unsigned long long netlonglong);

unsigned __int64 htonll ( unsigned __int64 Value ); 
unsigned __int64 ntohll ( unsigned __int64 Value );
unsigned __int32 htonf ( float Value );
float ntohf ( unsigned __int32 Value );
unsigned __int64 htond ( double Value );
double ntohd ( unsigned __int64 Value );

/*功能：获取网卡信息参数：listNetAdapter[out]:结果返回：BOOL*/
BOOL GetLocalAdaptersInfo(std::list<NetAdapter> & listNetAdapter);  
 
//获取本机外网IP 
bool GetInternetIPList(std::list<std::string> & listIp)
```

//NetCommmonFunction.cpp

```cpp
#include "NetCommmonFunction.h"
#include <winsock.h>
#include <IPHlpApi.h>
#include <iostream>
#pragma comment(lib,"IPHlpApi.lib")
#pragma comment(lib,"Ws2_32.lib")

BOOL LocalCpuIsBigEndian()
{
union NUM{int a;char b;}num;
num.a = 0x1234;
if( num.b == 0x12 )
{
return TRUE;
}
return FALSE;
}

/*功能：获取大端对齐标志参数：无返回：大端对齐标志*/
BOOL& GetBigEndianFlag()
{
static BOOL gbBigEndian = FALSE;
return gbBigEndian;
}
/*功能：设置大端对齐标志参数：bBigEndian[in]:是否为大端对齐(TRUE:大端,FALSE:小端(默认))返回：无*/
void SetBigEndian(BOOL bBigEndian)
{
GetBigEndianFlag() = bBigEndian;
}
/*功能：大小端对齐转换参数：hostlong[in]:转换前数据返回：转换结果*/
unsigned long h2n_conver_ulong(unsigned long hostlong)
{
if(GetBigEndianFlag())
return htonl(hostlong);
else return hostlong;
}
unsigned long n2h_conver_ulong(unsigned long netlong)
{
if(GetBigEndianFlag())
return ntohl(netlong);
else return netlong;
}
/*功能：大小端对齐转换参数：hostshort[in]:转换前数据返回：转换结果*/
unsigned short h2n_convert_ushort(unsigned short hostshort)
{
if(GetBigEndianFlag())
return htons(hostshort);
else 
return hostshort;
}
unsigned short n2h_convert_ushort(unsigned short netshort)
{
if(GetBigEndianFlag())
return ntohs(netshort);
else 
return netshort;
}
/*功能：unsigned long long大小端对齐转换参数：hostlonglong[in]:转换前数据返回：转换结果*/
unsigned long long h2n_convert_ulonglong(unsigned long long hostlonglong)
{
if(GetBigEndianFlag())
{
unsigned long temp_low = 0,temp_high = 0;
temp_low = htonl((long)hostlonglong);
temp_high = htonl((long)(hostlonglong >> 32));
hostlonglong &= 0;
hostlonglong |= temp_low;
hostlonglong <<= 32;
hostlonglong |= temp_high;
return hostlonglong;
}
else
{
return hostlonglong;
}
}

unsigned long long n2h_convert_ulonglong(unsigned long long netlonglong)
{
if(GetBigEndianFlag())
{
unsigned long temp_low = 0,temp_high = 0;
temp_low = ntohl((long)netlonglong);
temp_high = ntohl((long)(netlonglong >> 32));
netlonglong &= 0;
netlonglong |= temp_low;
netlonglong <<= 32;
netlonglong |= temp_high;
return netlonglong;
}
else
{
return netlonglong;
}
}

unsigned __int64 htonll ( unsigned __int64 Value ) {  const unsigned __int64 Retval = _WS2_32_WINSOCK_SWAP_LONGLONG (Value); return Retval;}
unsigned __int64 ntohll ( unsigned __int64 Value ) {  const unsigned __int64 Retval = _WS2_32_WINSOCK_SWAP_LONGLONG (Value); return Retval;}
unsigned __int32 htonf ( float Value ) {  unsigned __int32 Tempval; unsigned __int32 Retval; Tempval = *(unsigned __int32*)(&Value); Retval = _WS2_32_WINSOCK_SWAP_LONG (Tempval); return Retval;}
float ntohf ( unsigned __int32 Value ) {  const unsigned __int32 Tempval = _WS2_32_WINSOCK_SWAP_LONG (Value); float Retval; *((unsigned __int32*)&Retval) = Tempval; return Retval;}
unsigned __int64 htond ( double Value ) {  unsigned __int64 Tempval; unsigned __int64 Retval; Tempval = *(unsigned __int64*)(&Value); Retval = _WS2_32_WINSOCK_SWAP_LONGLONG (Tempval); return Retval;}
double ntohd ( unsigned __int64 Value ) {  const unsigned __int64 Tempval = _WS2_32_WINSOCK_SWAP_LONGLONG (Value); double Retval; *((unsigned __int64*)&Retval) = Tempval; return Retval;}

/*功能：获取网卡信息参数：listNetAdapter[out]:结果返回：BOOL*/
BOOL GetLocalAdaptersInfo(std::list<NetAdapter> & listNetAdapter)
{
//IP_ADAPTER_INFO结构体
PIP_ADAPTER_INFO pIpAdapterInfo = NULL;
pIpAdapterInfo = new IP_ADAPTER_INFO;
//结构体大小
unsigned long ulSize = sizeof(IP_ADAPTER_INFO);
//获取适配器信息int nRet = GetAdaptersInfo(pIpAdapterInfo,&ulSize);
if (ERROR_BUFFER_OVERFLOW == nRet)
{
//空间不足，删除之前分配的空间
delete []pIpAdapterInfo;
//重新分配大小
pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[ulSize];
//获取适配器信息
nRet = GetAdaptersInfo(pIpAdapterInfo,&ulSize);
//获取失败
if (ERROR_SUCCESS != nRet)
{
if (pIpAdapterInfo != NULL)
{
delete []pIpAdapterInfo;
}
return FALSE;
}
}
NetAdapter naNetAdapter;
//赋值指针
PIP_ADAPTER_INFO pIterater = pIpAdapterInfo;while(pIterater){sprintf_s(naNetAdapter.AdapterName,255,"%s",pIterater->AdapterName);
sprintf_s(naNetAdapter.Description,255,"%s",pIterater->Description);
sprintf_s(naNetAdapter.szMacAddr,19,"%02X-%02X-%02X-%02X-%02X-%02X",pIterater->Address[0],pIterater->Address[1],pIterater->Address[2],pIterater->Address[3],pIterater->Address[4],pIterater->Address[5]);
//IP地址列表
naNetAdapter.listIp.clear();
IpInfo ipIpInfo;
//指向IP地址列表
PIP_ADDR_STRING pIpAddr = &pIterater->IpAddressList;
while (pIpAddr)
{
sprintf_s(ipIpInfo.szIp,127,"%s",pIpAddr->IpAddress.String);
sprintf_s(ipIpInfo.szMask,127,"%s",pIpAddr->IpMask.String);
ipIpInfo.listGateway.clear();
//指向网关列表
PIP_ADDR_STRING pGateAwayList = &pIterater->GatewayList;while(pGateAwayList)
{
//网关ipIpInfo.listGateway.push_back(pGateAwayList->IpAddress.String);
pGateAwayList = pGateAwayList->Next;
}
pIpAddr = pIpAddr->Next;
}
//插入naNetAdapter.listIp.push_back(ipIpInfo);
pIterater = pIterater->Next;
//插入
listNetAdapter.push_back(naNetAdapter);}
//清理
if (pIpAdapterInfo)
{
delete []pIpAdapterInfo;
}
return TRUE;
}

//获取本机外网IP
bool GetInternetIPList(std::list<std::string> & listIp)
{
	//两个查询网站供用户切换
#if(1)
	std::string checkWeb = "http://only-997227-218-13-34-109.nstool.netease.com/";
	std::string tagStr = "您的IP地址信息: ";
	std::string endChar = " ";
#else
	std::string checkWeb = "http://www.ip138.com/ips1388.asp";
	std::string tagStr = "您的IP地址是：[";
	std::string endChar = "]";
#endif
	DeleteFile("ipFile.txt");
	HRESULT hr = URLDownloadToFileA(0, checkWeb.c_str(), "ipFile.txt", 0, NULL);
	if (hr != S_OK) {
		return false;
	}

	FILE * fpIp = fopen("ipFile.txt", "r");
	if (fpIp == NULL)
	{
		return false;
	}
	ULONGLONG len = 0;
	fseek(fpIp, 0, SEEK_END);
	len = ftell(fpIp);
	fseek(fpIp, 0, SEEK_SET);
	char* buf = new char[len + 1];
	ZeroMemory(buf, len + 1);
	fread(buf, sizeof(char), len, fpIp);
	buf[len] = '\0';
	char szIP[128] = { 0 };
	char *p = NULL, *q = NULL;
	p = strstr(buf, (char *)tagStr.c_str());
	if (p != NULL)
	{
		len = tagStr.size();
		q = strstr(p + len,endChar.c_str());
		if (q != NULL)
		{
			memcpy(szIP, p + len, q - p - len);
		}
	}
	if (buf != NULL)
	{
		delete buf;
		buf = NULL;
	}
	fclose(fpIp);
	DeleteFile("ipFile.txt");
	if (strlen(szIP) > 0)
	{
		listIp.push_back(szIP);
	}

	if (strlen(szIP) == 0)
	{
		return false;
	}

	return true;
}
```

