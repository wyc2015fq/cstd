# Windows下无线局域网设备SSID和BSSID获取方法（C语言） - xqhrs232的专栏 - CSDN博客
2019年02月11日 16:04:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：191
原文地址::[https://blog.csdn.net/qq_37711989/article/details/80878052](https://blog.csdn.net/qq_37711989/article/details/80878052)
相关文章
1、C++程序中使用Windows系统Native Wifi API的基本教程----[https://www.jb51.net/article/81199.htm](https://www.jb51.net/article/81199.htm)
2、wlanapi关于wlanconnect? ----[https://bbs.csdn.net/topics/380213834](https://bbs.csdn.net/topics/380213834)
3、VC++玩转Native Wifi API----[https://blog.csdn.net/lincyang/article/details/34430939/](https://blog.csdn.net/lincyang/article/details/34430939/)
4、native wifi api使用方法----[https://blog.csdn.net/thanklife/article/details/83751653](https://blog.csdn.net/thanklife/article/details/83751653)
5、Native wifi API使用----[https://blog.csdn.net/guoxiang3538/article/details/79266610](https://blog.csdn.net/guoxiang3538/article/details/79266610)
Windows下无线局域网设备SSID和BSSID获取方法
SSID可以简单的理解为无线局域网络的名称 
BSSID可以简单理解为无线局域网络放射设备的MAC地址
SSID获取方法
一、获取步骤 
1、打开WLAN服务 WlanOpenHandle 
2、获取无线网卡接口信息 WlanEnumInterfaces 
3、搜索无线网卡接口上的可用无线网络 WlanGetAvailableNetworkList 
4、提取SSID信息
实例代码如下：
#include <stdio.h>
#include <Windows.h>
#include <wlanapi.h>
int main(int ar,char*aru[])
{
    int id=0;
    char *lang = setlocale(LC_CTYPE,NULL);  
    HANDLE ClientHandle;
    DWORD nv,i,c;
    PWLAN_INTERFACE_INFO_LIST ilist;
    PWLAN_AVAILABLE_NETWORK_LIST nlist;
    static char ssid[36];                   
    if(WlanOpenHandle(1,0,&nv,&ClientHandle)==0)
    {
        if(WlanEnumInterfaces(ClientHandle,0,&ilist)==0)
        {
            for (i = 0; i< ilist->dwNumberOfItems; i++) 
            {
                wprintf(L"\n\n  %s%s\n\n",L"网卡:",ilist->InterfaceInfo[i].strInterfaceDescription);
                if(WlanGetAvailableNetworkList(ClientHandle,&ilist->InterfaceInfo[i].InterfaceGuid,0,0,&nlist)==0)
                {
                    for(c=0;c<nlist->dwNumberOfItems;c++)
                    {
                        memcpy(ssid,nlist->Network[c].dot11Ssid.ucSSID,nlist->Network[c].dot11Ssid.uSSIDLength);
                        ssid[nlist->Network[c].dot11Ssid.uSSIDLength]=0;
                        printf("   %3d. SSID:  %-25s  信号强度¨¨:  %5d\n",id++,ssid,nlist->Network[c].wlanSignalQuality);
                        //可继续获取BSSID
                    }
                    WlanFreeMemory(nlist);
                }
            }
        WlanFreeMemory(ilist);
    }
    system("pause>nul");
    WlanCloseHandle(ClientHandle,0);
    }
    system("pause");
    return true;
}
二、获BSSID方法 
获取到SSID之后可以继续获取BSSID
PWLAN_BSS_LIST ppWlanBssList; //BSS网络列表
dwResult = WlanGetNetworkBssList(
ClientHandle,
&pIfList->InterfaceInfo[i].InterfaceGuid,
&pBssList->Network[j].dot11Ssid,pBssList->Network[j].dot11BssType,
pBssList->Network[j].bSecurityEnabled,
NULL,&ppWlanBssList);
if(ERROR_SUCCESS == dwResult )
{
    for(k=0;k<ppWlanBssList->dwNumberOfItems;k++)
    {
        printf("    %3 MAC:");
        printf("%02X:%02X:%02X:%02X:%02X:%02X", 
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[0],
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[1],
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[2],
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[3],
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[4],
            ppWlanBssList->wlanBssEntries[k].dot11Bssid[5]
            );
        printf("\n");
    }
}
三、相关数据结构和API介绍
//详细参考https://blog.csdn.net/softwave/article/details/41389907
Data Struture
（1）网卡接口信息的阵列
  typedef struct _WLAN_INTERFACE_INFO_LIST {
                DWORDdwNumberOfItems;
                DWORD dwIndex;
                WLAN_INTERFACE_INFOInterfaceInfo[];
 } WLAN_INTERFACE_INFO_LIST,
  *PWLAN_INTERFACE_INFO_LIST;
（2）可用网络（network）的信息的列表
typedef struct_WLAN_AVAILABLE_NETWORK_LIST {
  　　　DWORD dwNumberOfItems;
  　　　DWORD dwIndex;
　　　 WLAN_AVAILABLE_NETWORK Network[1];
 } 　WLAN_AVAILABLE_NETWORK_LIST,
  *PWLAN_AVAILABLE_NETWORK_LIST;
//详细参考https://blog.csdn.net/softwave/article/details/41389907，还可参考WINDOWSAPI开发手册
API
（1） 打开一个与服务器的连接
   DWORD WINAPI WlanOpenHandle( __in DWORD dwClientVersion, PVOID pReserved, __out PDWORD pdwNegotiatedVersion, __out PHANDLE phClientHandle );
（2）枚举处当前系统安装的所有无线网卡的接口信息
DWORD WINAPI WlanEnumInterfaces( __in HANDLE hClientHandle, __in PVOID pReserved, __out PWLAN_INTERFACE_INFO_LIST* ppInterfaceList );  
（3）检索接口上可用的网络
 DWORD WINAPI WlanGetAvailableNetworkList(
      __in          HANDLE hClientHandle,
      __in          const GUID* pInterfaceGuid,
      __in          DWORD dwFlags,
      PVOID pReserved,
      __out         PWLAN_AVAILABLE_NETWORK_LIST* ppAvailableNetworkList
    );
（4）关闭一个与服务器间的连接
DWORD WINAPI WlanCloseHandle(__in  HANDLE hClientHandle, PVOID pReserved )
（5） 释放内存。任何由WLAN API 返回的内存都需要进行释放
  VOID WINAPI WlanFreeMemory( __in   PVOID pMemory);
