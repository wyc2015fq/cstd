# WINCE下使用EnumDisplaySettings获取LCD属性 - xqhrs232的专栏 - CSDN博客
2012年06月26日 13:50:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1171

来自WINCE的MSDN手册
This function retrieves information about one of the graphics modes for a display device. To retrieve information for all the graphics modes of a display device, make a series of calls to this function.
**BOOL EnumDisplaySettings(  LPCTSTR ***[lpszDeviceName]()***,  DWORD ***[iModeNum]()***,  LPDEVMODE ***[lpDevMode]()***);**
#### Parameters
- *lpszDeviceName*
[in] Pointer to a null-terminated string that specifies the display device.
This parameter is either NULL or a **DISPLAY_DEVICE.DeviceName** returned from[EnumDisplayDevices]().
A NULL value specifies the current display device on the system that is running the calling thread.
- *iModeNum*
[in] Specifies the type of information to retrieve.
This value can be a graphics mode index or one of values shown in the following table.
|Value|Description|
|----|----|
|ENUM_CURRENT_SETTINGS|Retrieve the current settings for the display device.|
|ENUM_REGISTRY_SETTINGS|Retrieve the settings for the display devices that are currently stored in the registry.|
For more information, see Remarks. 
- *lpDevMode*
[out] Pointer to a [DEVMODE]() structure that will contain the information about the specified graphics mode.
Before calling **EnumDisplaySettings**, set the **dmSize** member to`sizeof(DEVMODE)`, and set the
**dmDriverExtra** member to indicate the size, in bytes, of the additional space available to receive private driver data.
The **EnumDisplaySettings** function sets values for the following**DEVMODE** members:
- **dmBitsPerPel **
- **dmPelsWidth **
- **dmPelsHeight **
- **dmDisplayFlags **
- **dmDisplayFrequency **
#### Return Values
If the function succeeds, the return value is nonzero.
If the function fails, the return value is zero. 
To get extended error information, call GetLastError. 
#### Remarks
The function fails if *iModeNum* is greater than the index of the display device's last graphics mode.
You can use *iModeNum* to enumerate all of a display device's graphics modes.
Graphics mode indexes start at zero. 
To obtain information for all of a display device's graphics modes, make a series of calls to**EnumDisplaySettings**. To do this, set
*iModeNum* to zero for the first call and then increment*iModeNum* by one for each subsequent call. Continue calling the function until the return value is zero.
When you call **EnumDisplaySettings** with *iModeNum* set to zero, the operating system initializes and caches information about the display device.
When you call **EnumDisplaySettings** with *iModeNum* set to a nonzero value, the function returns the information that was cached the last time the function was called with*iModeNum* set to zero.
#### Requirements 
**OS Versions:** Windows CE .NET 4.2 and later.
**Header:** Windows.h.
**Link Library:** Coredll.lib.
#### See Also
[ChangeDisplaySettingsEx]() | [CreateDC]() | [DEVMODE]() | [DISPLAY_DEVICE]() | [EnumDisplayDevices]() |[GDI Functions]()
Last updated on Wednesday, July 13, 2005
© 2005 Microsoft Corporation. All rights reserved.
