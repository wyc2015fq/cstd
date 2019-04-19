# WinCE电源管理----驱动增加电源管理属性 - xqhrs232的专栏 - CSDN博客
2013年10月16日 16:31:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：857
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/01/27/1657973.html](http://www.cnblogs.com/we-hjb/archive/2010/01/27/1657973.html)
对于移动设备来说，电源管理是比较重要的。为了让设备有更长的待机和工作时间，实现一个完善的电源管理功能也是必须的。针对移动平台的操作系统WinCE本身包括了一个基本的电源管理子模块"Powe Manager"，可以在Catalog中添加至系统。驱动和应用程序将通过它有效的管理各个设备或者整个系统的电源工作状态。最近，在我们TCC8900的平台上实现了简单的电源管理，积累了一些经验，在这里跟大家分享一下。
      在WinCE系统中实现电源管理主要有三步，添加"Power Manager"组件，在驱动中实现电源管理的接口，在应用程序中通过调用电源管理的API控制各设备和系统的工作状态。
      第一步比较简单，通常只要在Catalog中添加就可以。如果有特殊需求，可以移植C:\WINCE600\PUBLIC\COMMON\OAK\DRIVERS\PM下的代码到BSP，并做相应的修改。
      第二步主要是在现有的驱动上添加电源管理的功能。对于驱动来说，并不要求必须包括电源管理的功能，完全可以不实现，具体情况具体分析。为了让Power Manager知道哪些驱动是支持电源管理功能的，需要在注册表中包含相应的键值(IClass)或者在驱动初始化时调用AdvertiseInterface()。一般来说，流驱动用注册表键值比较方便一些，而显示驱动通常用AdvertiseInterface()来告知。流驱动和显示驱动在实现电源管理的功能上也有所不同。流驱动主要是在XXX_IOControl中，实现IOCTL_POWER_CAPABILITIES、IOCTL_POWER_QUERY、IOCTL_POWER_SET和IOCTL_POWER_GET。具体代码可以参考C:\WINCE600\PLATFORM\DEVICEEMULATOR\SRC\DRIVERS\BACKLIGHT，非常好的一个例子。而显示驱动主要是在DrvEscape中实现以上几个IoControlCode，另外需要注意在QUERYESCSUPPORT添加相应的IoControlCode。这个代码可以参考C:\WINCE600\PLATFORM\COMMON\SRC\SOC\PXA27X_MS_V1\DISPLAY\PXA27X_LCD，也是非常好的一个例子。以上这两个参考代码的框架都基本完善，在做移植时根据具体的情况实现相应的功能即可。如果成功实现了这两步，就可以在控制面板的电源属性中看到相应的设备了。如下图所示。
![clip_image002](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE_14DC1/clip_image002_thumb.jpg)
     第三步主要就是在应用程序中通过调用电源管理的API，协助系统管理各个设备和整个系统的工作状态。
     一般来说，外设的工作状态主要有以下几个Full On 、Low On、Standby、Sleep 和Off，分别对应D0、D1、D2、D3和D4。他们的映射关系和其他一些关于电源管理的配置在注册表[HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Power]中定义。这里需要注意的是应用程序如何修改系统的超时时间设置，键值在[HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Power\Timeouts]下，仅仅修改注册表中的键值并不能实时生效，应用程序通过注册表读写API修改该键下的DWORD值(十六进制以秒为单位)，完成修改后，必须通知PM使用新设置的超时时间，实现的参考代码如下:
```
HANDLE hReloadActivityTimeouts;
    hReloadActivityTimeouts = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("PowerManager/ReloadActivityTimeouts"));
    if(hReloadActivityTimeouts)
    {
        SetEvent(hReloadActivityTimeouts);
        CloseHandle(hReloadActivityTimeouts);
    }
```
       如果实现了HIVE注册表，由于系统启动时PM的启动先于文件系统，所以在PM启动时它还不能读取到文件系统中保存的值，在系统启动完成后也需要通知一下系统重新加载我们设定的值。这个操作可以放在驱动或者应用程序中。
      应用程序中常用的几个电源管理API如下:
- DevicePowerNotify 用于改变设备的电源状态
- SetPowerRequirement 用于请求保持设备的电源状态
- ReleasePowerRequirement 释放已请求设备的电源管理
- SetSystemPowerState 用于设置整个系统的电源状态
- RequestPowerNotifications 注册一个消息队列接收电源状态变化的通知
- StopPowerNotifications 停止接收电源变化的通知
      需要注意的是，驱动不能擅自改变自己的电源状态，而必须请求PM来处理。
      PM中通过Timer进行状态切换的过程如下图所示。
![clip_image004](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE_14DC1/clip_image004_thumb.jpg)
      最后附上M8手机的电源属性图，可以看到好的产品对电源管理做得有多细。
![clip_image006](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE_14DC1/clip_image006_thumb.jpg)
      电源管理是一个非常复杂也非常重要的内容。以上是我在学习过程中的一些经验，抛砖引玉，希望大家能有更多的讨论。
//==================================================
备注::
1>为了让Power Manager知道哪些驱动是支持电源管理功能的，需要在注册表中包含相应的键值(IClass)或者在驱动初始化时调用AdvertiseInterface()。
2>流驱动主要是在XXX_IOControl中，实现IOCTL_POWER_CAPABILITIES、IOCTL_POWER_QUERY、IOCTL_POWER_SET和IOCTL_POWER_GET。具体代码可以参考C:\WINCE600\PLATFORM\DEVICEEMULATOR\SRC\DRIVERS\BACKLIGHT，非常好的一个例子。而显示驱动主要是在DrvEscape中实现以上几个IoControlCode，另外需要注意在QUERYESCSUPPORT添加相应的IoControlCode。这个代码可以参考C:\WINCE600\PLATFORM\COMMON\SRC\SOC\PXA27X_MS_V1\DISPLAY\PXA27X_LCD，也是非常好的一个例子。以上这两个参考代码的框架都基本完善，在做移植时根据具体的情况实现相应的功能即可。
3>IF BSP_NOBACKLIGHT !
[HKEY_LOCAL_MACHINE\Drivers\BuiltIn\Backlight]
    "Prefix"="BKL"
    "Dll"="backlight.dll"
    "Index"=dword:1
    "Order"=dword:1
    "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"
; Backlight tab of Display control panel (timeouts in seconds)
[HKEY_CURRENT_USER\ControlPanel\Backlight]
    "BatteryTimeout"=dword:3c; 60 seconds
    "BacklightOnTap"=dword:1
    "ACTimeout"=dword:258; 600 seconds
    "ACBacklightOnTap"=dword:1
ENDIF BSP_NOBACKLIGHT !
4>extern "C" BOOL BKL_IOControl(
    DWORD hOpenContext,
    DWORD dwCode,
    PBYTE pBufIn,
    DWORD dwLenIn,
    PBYTE pBufOut,
    DWORD dwLenOut,
    PDWORD pdwActualOut )
{
    DWORD dwErr = ERROR_INVALID_PARAMETER;
    RETAILMSG(ZONE_BACKLIGHT,(TEXT("BKL_IOControl IOCTL code  = %d\r\n"), dwCode));
    switch (dwCode) {
    case IOCTL_POWER_CAPABILITIES:  // determines device-specific capabilities
        RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_CAPABILITIES\r\n")));
        if (pBufOut && dwLenOut >= sizeof (POWER_CAPABILITIES) && pdwActualOut) 
        {
            __try 
            {
                PPOWER_CAPABILITIES PowerCaps = (PPOWER_CAPABILITIES)pBufOut;
                // Right now supports D0 (permanently on) and D4(off) only.
                memset(PowerCaps, 0, sizeof(*PowerCaps));
                PowerCaps->DeviceDx = 0x11; //support D0, D4
                *pdwActualOut = sizeof(*PowerCaps);
                dwErr = ERROR_SUCCESS;
            }
            __except(EXCEPTION_EXECUTE_HANDLER) 
            {
                RETAILMSG(ZONE_BACKLIGHT, (TEXT("exception in ioctl\r\n")));
            }
        }
        break;
    case IOCTL_POWER_QUERY: // determines whether changing power state is feasible
            RETAILMSG(ZONE_BACKLIGHT,(TEXT("BKL: Received IOCTL_POWER_QUERY\r\n")));
            if (pBufOut && dwLenOut >= sizeof(CEDEVICE_POWER_STATE)) 
            {
                // Return a good status on any valid query, since we are always ready to
                // change power states (if asked for state we don't support, we move to next highest, eg D3->D4).
                __try 
                {
                    CEDEVICE_POWER_STATE ReqDx = *(PCEDEVICE_POWER_STATE)pBufOut;
                    if (VALID_DX(ReqDx)) 
                    {
                        // This is a valid Dx state so return a good status.
                        dwErr = ERROR_SUCCESS;
                    }
                    RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_QUERY %s\r\n"), dwErr == ERROR_SUCCESS ? (TEXT("succeeded")) : (TEXT("failed")) ));
                }
                __except(EXCEPTION_EXECUTE_HANDLER) 
                {
                    RETAILMSG(ZONE_BACKLIGHT, (TEXT("Exception in ioctl\r\n")));
                }
            }
            break;
        break;
    case IOCTL_POWER_SET: // requests a change from one device power state to another
            RETAILMSG(ZONE_BACKLIGHT,(TEXT("BKL: Received IOCTL_POWER_SET\r\n")));
            if (pBufOut && dwLenOut >= sizeof(CEDEVICE_POWER_STATE)) 
            {
                __try 
                {
                    CEDEVICE_POWER_STATE ReqDx = *(PCEDEVICE_POWER_STATE)pBufOut;
                    if (VALID_DX(ReqDx)) 
                    {
                        if(ReqDx == D1 ||ReqDx == D2 || ReqDx == D3) 
                        {
                            ReqDx = D4;
                        }
                        if (SetBackLightState(D0 == ReqDx ? TRUE : FALSE)) 
                        {
                            *(PCEDEVICE_POWER_STATE) pBufOut = ReqDx;
                            *pdwActualOut = sizeof(CEDEVICE_POWER_STATE);
                            dwErr = ERROR_SUCCESS;
                            RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_SET to D%u \r\n"), ReqDx));
                        }
                        else
                        {
                            dwErr = GetLastError();
                            RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_SET failed to switch to D%u\r\n"), ReqDx));
                        }
                    }
                    else 
                    {
                        RETAILMSG(ZONE_BACKLIGHT, (TEXT("Invalid state request D%u\r\n"), ReqDx));
                    }
                }
                __except(EXCEPTION_EXECUTE_HANDLER) 
                {
                    RETAILMSG(ZONE_BACKLIGHT, (TEXT("Exception in ioctl\r\n")));
                }
            }
            break;
        break;
    case IOCTL_POWER_GET: // gets the current device power state
           RETAILMSG(ZONE_BACKLIGHT,(TEXT("BKL: Received IOCTL_POWER_GET\r\n")));
            if (pBufOut != NULL && dwLenOut >= sizeof(CEDEVICE_POWER_STATE)) 
            {
                __try 
                {
                    if (GetBackLightState((PCEDEVICE_POWER_STATE)pBufOut)) 
                    {
                        dwErr = ERROR_SUCCESS;
                        RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_GET: passing back %u\r\n"), *(PCEDEVICE_POWER_STATE)pBufOut));
                    }
                    else
                    {
                        dwErr = GetLastError();
                        RETAILMSG(ZONE_BACKLIGHT, (TEXT("IOCTL_POWER_GET: failed to get backlight state\r\n")));
                    }
                }
                __except(EXCEPTION_EXECUTE_HANDLER) 
                {
                    RETAILMSG(ZONE_BACKLIGHT, (TEXT("Exception in ioctl\r\n")));
                }
            }
        break;
    default:
        break;
    }
    if (dwErr) 
    {
        RETAILMSG(ZONE_BACKLIGHT, (TEXT("Ioctl failed - err=%d\r\n"), dwErr));
        return FALSE;
    }
    return TRUE;
}
