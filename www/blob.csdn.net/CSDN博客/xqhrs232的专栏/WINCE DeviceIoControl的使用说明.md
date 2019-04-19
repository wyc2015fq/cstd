# WINCE DeviceIoControl的使用说明 - xqhrs232的专栏 - CSDN博客
2013年08月26日 23:57:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：979
原文地址::[http://blog.163.com/tianjunqiang666@126/blog/static/8725911920121189220576/](http://blog.163.com/tianjunqiang666@126/blog/static/8725911920121189220576/)
相关网帖
1、[CreateEvent，DeviceIoControl的用法](http://blog.csdn.net/qq872318306/article/details/6617884)----[http://blog.csdn.net/qq872318306/article/details/6617884](http://blog.csdn.net/qq872318306/article/details/6617884)

本文来自：
http://blog.csdn.net/lujunql/article/details/2532362
[http://msdn.microsoft.com/en-us/library/ms904001.aspx](http://msdn.microsoft.com/en-us/library/ms904001.aspx)
应用程序和驱动程序的通信过程是：应用程序使用CreateFile函数打开设备，然后用DeviceIoControl与驱动程序进行通信，包括读和写两种操作。还可以用ReadFile读数据用WriteFile写数据。操作完毕时用CloseHandle关闭设备。我们比较常用的就是用DeviceIoControl对设备进行读写操作。先看看DeviceIoControl是怎么定义的：
BOOL DeviceIoControl(   HANDLE hDevice,    DWORD dwIoControlCode,    LPVOID lpInBuffer,    DWORD nInBufferSize,    LPVOID lpOutBuffer,    DWORD nOutBufferSize,    LPDWORD lpBytesReturned,    LPOVERLAPPED lpOverlapped );
#### Parameters（参数）
- *hDevice* （CreateFile返回的设备句柄）
[in] Handle to the device that is to perform the operation. To obtain a device handle, call the CreateFile function.- *dwIoControlCode* （应用程序调用驱动程序的控制命令，就是IOCTL_*XXX* IOCTLs ）
[in] IOCTL for the operation. This value identifies the specific operation to perform and the type of device on which to perform the operation. There are no specific values defined for the *dwIoControlCode *parameter.
 However, you can define custom IOCTL_*XXX* IOCTLs with the [CTL_CODE](http://msdn.microsoft.com/en-us/library/ms904001.aspx) macro.
 You can then advertise these IOCTLs and an application can use these IOCTLs with **DeviceIoControl** to perform the driver-specific functions.- *lpInBuffer* （应用程序传递给驱动程序的数据缓冲区地址）
[in] Long pointer to a buffer that contains the data required to perform the operation. Set to NULL if the *dwIoControlCode* parameter specifies an operation that does not require input data.- *nInBufferSize* （应用程序传递给驱动程序的数据缓冲区大小，字节数）
[in] Size, in bytes, of the buffer pointed to by *lpInBuffer*.- *lpOutBuffer* （驱动程序返回给应用程序的数据缓冲区地址）
[out] Long pointer to a buffer that receives the output data for the operation. Set to NULL if the *dwIoControlCode* parameter specifies an operation that does not produce output data.- *nOutBufferSize* （驱动程序返回给应用程序的数据缓冲区大小，字节数）
[out] Size, in bytes, of the buffer pointed to by *lpOutBuffer*.- *lpBytesReturned* （驱动程序实际返回给应用程序的数据字节数地址）
[out] Long pointer to a variable that receives the size, in bytes, of the data stored in *lpOutBuffer*. The **DeviceIoControl** function may unnecessarily use this parameter. For example,
 if an operation does not produce data for *lpOutBuffer* and *lpOutBuffer* is NULL, the value of *lpBytesReturned* is meaningless.- *lpOverlapped* （重叠操作结构）
[in] Ignored; set to NULL.
#### Return Values（返回值）
Nonzero indicates success. Zero indicates failure. To obtain extended error information, call the [GetLastError](http://msdn.microsoft.com/en-us/library/ms885627.aspx) function.
 （非0成功，0失败）
具体使用我们看看列子：
1，向设备传递数据，我们定义一个函数来实现
bool CDeviceOperDlg::SendKeyData(HANDLE handle, BYTE *bData, int iSize)
{
 ULONG nOutput;
 BYTE bTemp[512];
 //将数据放置到发送数组
 memset(bTemp,0,sizeof(bTemp));
 memcpy(bTemp,&bData[0],iSize);
 //向设备发送
 if (!DeviceIoControl(handle,         
       ATST2004_IOCTL_WRITE,     //根据具体的设备有相关的定义
       bTemp,                                        //向设备传递的数据地址
       iSize,                                            //数据大小，字节数
       NULL,                                          //没有返回的数据，置为NULL
       0,                                                  //没有返回的数据，置为0
       &nOutput,
       NULL)
    )
 {
  return false;
 }
 return true;
}
2，从设备读取数据
bool CDeviceOperDlg::ReviceKeyData(HANDLE handle, BYTE *bData, int iSize)
{
 ULONG nOutput;
 BYTE bTemp[512];
 //数组清零
 memset(bTemp,0,sizeof(bTemp));
 //向设备发送
 if (!DeviceIoControl(handle,
       ATST2004_IOCTL_READ,           //根据具体的设备有相关的定义
       NULL,                                              //没有向设备传递的数据，置为NULL
       0,                                                      //没有向设备传递的数据，置为NULL
       bTemp,                                           //读取设备的数据返回地址
       iSize,                                               //读取数据的字节数
       &nOutput,
       NULL)
    )
 {
  return false;
 }
 //放置到公用数组
 memcpy(&bData[0],&bTemp[0],iSize);
 return true;
}
我们在说DeviceIoControl函数时其第二个参数dwIoControlCode就是由CTL_CODE宏定义的，下边我们可以了解一下CTL_CODE的内容。
CTL_CODE：用于创建一个唯一的32位系统I/O控制代码，这个控制代码包括4部分组成：DeviceType（设备类型，高16位（16-31位）），Access（访问限制，14-15位），Function（功能2-13位），Method（I/O访问内存使用方式）。
This macro creates a unique system I/O control code (IOCTL).
#define CTL_CODE(DeviceType, Function, Method, Access) (   ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )
#### Parameters（参数）
- *DeviceType*
Defines the type of device for the given IOCTL.
This parameter can be no bigger than a WORD value.
The values used by Microsoft are in the range 0-32767; the values 32768-65535 are reserved for use by OEMs and IHVs.
The following device types are defined by the system:
- FILE_DEVICE_BEEP
- FILE_DEVICE_CD_ROM
- FILE_DEVICE_CD_ROM_FILE_SYSTEM
- FILE_DEVICE_CONTROLLER
- FILE_DEVICE_DATALINK
- FILE_DEVICE_DFS
- FILE_DEVICE_DISK
- FILE_DEVICE_DISK_FILE_SYSTEM
- FILE_DEVICE_FILE_SYSTEM
- FILE_DEVICE_INPORT_PORT
- FILE_DEVICE_KEYBOARD
- FILE_DEVICE_MAILSLOT
- FILE_DEVICE_MIDI_IN
- FILE_DEVICE_MIDI_OUT
- FILE_DEVICE_MOUSE
- FILE_DEVICE_MULTI_UNC_PROVIDER
- FILE_DEVICE_NAMED_PIPE
- FILE_DEVICE_NETWORK
- FILE_DEVICE_NETWORK_BROWSER
- FILE_DEVICE_NETWORK_FILE_SYSTEM
- FILE_DEVICE_NULL
- FILE_DEVICE_PARALLEL_PORT
- FILE_DEVICE_PHYSICAL_NETCARD
- FILE_DEVICE_PRINTER
- FILE_DEVICE_SCANNER
- FILE_DEVICE_SERIAL_MOUSE_PORT
- FILE_DEVICE_SERIAL_PORT
- FILE_DEVICE_SCREEN
- FILE_DEVICE_SOUND
- FILE_DEVICE_DEVICE_STREAMS
- FILE_DEVICE_TAPE
- FILE_DEVICE_TAPE_FILE_SYSTEM
- FILE_DEVICE_TRANSPORT
- FILE_DEVICE_UNKNOWN
- FILE_DEVICE_VIDEO
- FILE_DEVICE_VIRTUAL_DISK
- FILE_DEVICE_WAVE_IN
- FILE_DEVICE_WAVE_OUT
- FILE_DEVICE_8042_PORT
- FILE_DEVICE_NETWORK_REDIRECTOR
- FILE_DEVICE_BATTERY
- FILE_DEVICE_BUS_EXTENDER
- FILE_DEVICE_MODEM
- FILE_DEVICE_VDM
- FILE_DEVICE_MASS_STORAGE
- FILE_DEVICE_SMB
- FILE_DEVICE_KS
- FILE_DEVICE_CHANGER
- FILE_DEVICE_SMARTCARD
- FILE_DEVICE_ACPI
- FILE_DEVICE_DVD
- FILE_DEVICE_FULLSCREEN_VIDEO
- FILE_DEVICE_DFS_FILE_SYSTEM
- FILE_DEVICE_DFS_VOLUME
The following device types are specific to Windows CE:
- FILE_DEVICE_HAL
- FILE_DEVICE_CONSOLE
- FILE_DEVICE_PSL
- FILE_DEVICE_SERVICE
- *Function*
Defines an action within the device category.
Function codes 0-2047 are reserved for Microsoft; codes 2048-4095 are reserved for OEMs and IHVs.
A function code can be no larger then 4095.
- *Method*
Defines the method codes for how buffers are passed for I/O and file system controls.
The following values are possible for this parameter:
- METHOD_BUFFERED
- METHOD_IN_DIRECT
- METHOD_OUT_DIRECT
- METHOD_NEITHER
This field is ignored by Windows CE. You should always use the METHOD_BUFFERED value unless compatibility with Windows-based desktop platforms is required using a different *Method* value.
- *Access*
Defines the access check value for any access.
The following table shows the possible flags for this parameter. The FILE_ACCESS_ANY is generally the correct value.
|Flag|Description|
|----|----|
|FILE_ANY_ACCESS|Request all access.|
|FILE_READ_ACCESS|Request read access. Can be used with FILE_WRITE_ACCESS.|
|FILE_WRITE_ACCESS|Request write access. Can be used with FILE_READ_ACCESS.|
#### Return Values（返回值）
None.
#### Remarks（备注）
The macro can be used for defining IOCTL and FSCTL function control codes. All IOCTLs must be defined this way to ensure that values used by Microsoft, OEMs, and IHVs do not overlap.
The following illustration shows the format of the resulting IOCTL.
![](http://msdn.microsoft.com/en-us/library/ms904001.ctl_code(en-us,MSDN.10).gif)
举例说明一下：
我定义两个IOCTL,一个用于对设备的读，一个用于对设备的写
#define ATST2004_IOCTL_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_DATA)
#define ATST2004_IOCTL_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
在VC中使用不需要进行处理，假如我要在VB中使用这两个IOCTL，就需要进行查值计算了，计算后定义如下：
Private Const ATST2004_IOCTL_READ = &H226000
Private Const ATST2004_IOCTL_WRITE = &H22A004
其中CTL_CODE定义中有一个Method域，该域的功能意义是定义用于与在驱动程序中获取应用程序数据缓冲区的地址方式。如果你看了我前边的文章，你可以看到在[DeviceIoControl](http://blog.csdn.net/lujunql/archive/2008/06/10/2532152.aspx)的使用说明中对[DeviceIoControl](http://blog.csdn.net/lujunql/archive/2008/06/10/2532152.aspx)参数进行描述中关于lpInBuffer
 和lpOutBuffer 的描述。简单的说，Method域就是定义了lpInBuffer 和lpOutBuffer 缓冲区在驱动程序中对这两个缓冲区地址的获取和数据的操作方式。我们分别进行描述：
METHOD_BUFFERED
系统分配一个缓冲区用于输入和输出，该缓冲区的字节数应该为应用程序的输入和输出缓冲区中较大的字节数。驱动程序中通过KIrp::IoctlBuffer获得缓冲区的地址。对于输出，驱动程序必须将输出字节数返回给I.Information(),然后由I/O管理器将数据从系统缓冲区复制到应用程序的缓冲区中。
METHOD_IN_DIRECTMETHOD_OUT_DIRECT
在这两种方式下，输入缓冲区数据被复制到一个系统缓冲区中，驱动程序可以用KIrp::IoctlBuffer访问这个缓冲区。输出缓冲区被类KMemory对象映射，驱动程序可以通过KIrp::Mdl来访问这个缓冲区。
METHOD_NEITHER
这个比较特殊，一般不使用。
