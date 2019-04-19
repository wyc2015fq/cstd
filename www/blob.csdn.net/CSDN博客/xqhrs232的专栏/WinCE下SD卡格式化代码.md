# WinCE下SD卡格式化代码 - xqhrs232的专栏 - CSDN博客
2014年08月15日 16:48:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1044
原文地址::[http://blog.itpub.net/16803921/viewspace-608610/](http://blog.itpub.net/16803921/viewspace-608610/)
相关文章
1、WinCE下SD卡格式化代码----[http://www.rosoo.net/a/201008/10009.html](http://www.rosoo.net/a/201008/10009.html)
注意：需要用到[微软](http://blog.itpub.net/16803921/viewspace-608610/..:;)的[API](http://blog.itpub.net/16803921/viewspace-608610/..:;)库"Storeapi.lib"和“storemgr.lib”。
#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <Storemgr.h>
#include <formatdisk.h>
#include <fatutil.h>
#include <Bootpart.h>
//#include <diskio.h>
//#include <pcireg.h>
int wmain()
{
    STOREINFO StoreInfo = {0};
    PARTINFO PartInfo = {0};
    HANDLE hFirstStore,hStore,hPart;
    HINSTANCE hFatUtil = NULL;
    BOOL hDismount = FALSE;//, CPart,hPartition,
    BOOL hMount = FALSE;
    FORMAT_PARAMS fp = {0};
    FORMAT_OPTIONS pfo = {0};
    DWORD dwClusSize = 0;
    DWORD dwFatVersion = 16;
    typedef DWORD ( *PFN_MY_FORMATVOLUME)
        (HANDLE hVolume,PDISK_INFO pdi, PFORMAT_OPTIONS pfo, PFN_PROGRESS pfnProgress,PFN_MESSAGE pfnMessage);
    StoreInfo.cbSize = sizeof(StoreInfo);
    hFirstStore = FindFirstStore( &StoreInfo );
    if(!(hFirstStore ))
    {
        RETAILMSG(1, (TEXT("Error FindFirstStore\r\n")));
    }
    FindNextStore(hFirstStore,&StoreInfo);
    FindNextStore(hFirstStore,&StoreInfo);
    FindNextStore(hFirstStore,&StoreInfo);
    hFatUtil = LoadLibrary(L"fatutil.dll");
    PFN_MY_FORMATVOLUME pfnFormatVolume = NULL ;
    pfnFormatVolume = (PFN_MY_FORMATVOLUME)GetProcAddress(hFatUtil, TEXT( "FormatVolume"));
    if (!pfnFormatVolume )
    {
        RETAILMSG(1, (TEXT("Error pfnFormatVolumeEx\r\n")));
    }
    hStore = OpenStore(StoreInfo.szDeviceName);
    hPart = OpenPartition(hStore,L"Part00");
    hDismount = DismountPartition(hPart);
    if(!hDismount)
    {
        RETAILMSG(1, (TEXT("Error DismountStore\r\n")));
    }
    if(ERROR_SUCCESS != pfnFormatVolume(hPart, NULL, NULL, NULL, NULL))
    {
        RETAILMSG(1, (TEXT("Error pfnFormatVolumeEx\r\n")));
    }
    PartInfo.cbSize = (DWORD)StoreInfo.snBiggestPartCreatable;
    //PartInfo.szPartitionName = L"Part00";
    hDismount = CreatePartition(hStore, L"Part00", PartInfo.cbSize);
    if(!hDismount)
    {
        RETAILMSG(1, (TEXT("Error CreatePartitionEx\r\n")));
    }
    hMount = MountPartition(hPart);
    if(!( hMount ))
    {
        RETAILMSG(1, (TEXT("Error OpenStore\r\n")));
    }
return 0;
}
