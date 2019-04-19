# 在WinCE下，应用程序直接读/写/擦除flash设备的方法 - xqhrs232的专栏 - CSDN博客
2014年11月13日 16:24:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：707
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/article/details/2196466](http://blog.csdn.net/nanjianhui/article/details/2196466)
相关文章
1、WinCE中如何擦除及读写NandFlash----[http://bbs.csdn.net/topics/320084703](http://bbs.csdn.net/topics/320084703)
2、[漫谈WinCE下的格式化](http://blog.csdn.net/norains/article/details/908554)----[http://blog.csdn.net/norains/article/details/908554](http://blog.csdn.net/norains/article/details/908554)
3、[WinCE Storage Manager 分析](http://blog.csdn.net/gsymichael/article/details/2803975)----[http://blog.csdn.net/gsymichael/article/details/2803975](http://blog.csdn.net/gsymichael/article/details/2803975)
4、[WinCE下SD卡格式化代码](http://blog.itpub.net/16803921/viewspace-608610/) ----[http://blog.itpub.net/16803921/viewspace-608610/](http://blog.itpub.net/16803921/viewspace-608610/)

作者：ARM-WINCE
在网上的很多论坛中都看到有人提问：应用程序如何直接读写Flash的扇区，或者是类似的问题。总之，就是希望应用程序能够直接访问Flash设备，直接读写扇区的数据，或者作其他的操作。这几天没事，就尝试着做了一下，把我的方法介绍给大家。
先做个简单的介绍。WinCE支持Flash设备，一般指Nandflash或者是NORFlash，采用的架构一般是FAL+FMD架构，我们实现FMD相关的接口函数，Flash的驱动就算完成了。当WinCE启动以后，我们能够看到Flash设备的磁盘。我们可以操作磁盘上面的文件，但是不能直接操作flash设备，对Flash设备的操作无非就是：读，写，擦除，读ID。
现在开始介绍实现的方法。我们如果想在应用程序中直接调用FMD中的FMD_ReadSector(..)，FMD_WriteSector(..)，FMD_EraseBlock(..)是不太现实的。这里再补充一下，这三个函数分别是Flash的读扇区，写扇区，擦除块的函数。好像有点罗嗦了。但是我们可以在应用程序中调用到FMD_OEMIoControl(..)函数，这个是可以做到的。所以我们需要改一下Flash设备的驱动程序，也就是改Flash设备驱动中的FMD_OEMIoControl(..)这个函数。我的改动如下：
BOOL  FMD_OEMIoControl(DWORD dwIoControlCode, PBYTE pInBuf, DWORD nInBufSize, PBYTE pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned)
{
    PFMDInterface pInterface = (PFMDInterface)pOutBuf;
    RETAILMSG(1, (TEXT("FMD_OEMIoControl: control code is 0x%x/r/n"), dwIoControlCode));
    switch(dwIoControlCode)
    {
        case IOCTL_FMD_GET_INTERFACE:
            if (!pOutBuf || nOutBufSize < sizeof(FMDInterface))
            {
                 DEBUGMSG(1, (TEXT("FMD_OEMIoControl: IOCTL_FMD_GET_INTERFACE bad parameter(s)./r/n")));
                 return(FALSE);
            }    
            pInterface->cbSize = sizeof(FMDInterface);
            pInterface->pInit = FMD_Init;
            pInterface->pDeInit = FMD_Deinit;
            pInterface->pGetInfo = FMD_GetInfo;        
            pInterface->pGetInfoEx = NULL; //FMD_GetInfoEx;
            pInterface->pGetBlockStatus = FMD_GetBlockStatus;     
            pInterface->pSetBlockStatus = FMD_SetBlockStatus;
            pInterface->pReadSector = FMD_ReadSector;
            pInterface->pWriteSector = FMD_WriteSector;
            pInterface->pEraseBlock = FMD_EraseBlock;
            pInterface->pPowerUp = FMD_PowerUp;
            pInterface->pPowerDown = FMD_PowerDown;
            pInterface->pGetPhysSectorAddr = NULL;            
            pInterface->pOEMIoControl = FMD_OEMIoControl;            
            break;
       case 0xff123456:
            FMD_ReadSector(..);                //调用读Sector函数
            break;
       case 0xff654321:
            FMD_WriteSector(..);                //调用写Sector函数
            break;
       case 0xff123457:
            FMD_EraseBlock(..);               //调用擦除Block函数
            break;
       default:
            DEBUGMSG(1, (TEXT("FMD_OEMIoControl: unrecognized IOCTL (0x%x)./r/n"), dwIoControlCode));
            return(FALSE);
    }
    return(TRUE);
} 
在FMD_OEMIoControl(..)函数里面增加了3个case，这3个case里面调用了读/写/擦除函数。至于Case的值，我是随便定义的。这样Flash设备的驱动部分就改完了。
在改完Flash驱动以后，我下面会提供两种方法，每一种方法都和Flash设备的注册表配置有关：
1. 以Nandflash为例，当然对于NORFlash来说大同小异，注册表配置如下：
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/NANDFlash]
"Dll"="ep94xxnandflash.dll"
"Prefix"="DSK"
"Order"=dword:4
;"Ioctl"=dword:4
"Profile"="NSFlash"
"IClass"="{A4E7EDDA-E575-4252-9D6B-4195D48BB865}"
; Override names in default profile
[HKEY_LOCAL_MACHINE/System/StorageManager/Profiles/NSFlash]
 "Name"="Ep94xx NAND Flash"
 "Folder"="NANDFlash"
 "PartitionDriver"="MSPart.dll"
 "AutoMount"=dword:1
 "AutoPart"=dword:1
 "AutoFormat"=dword:1
[HKEY_LOCAL_MACHINE/System/StorageManager/Profiles/NSFlash/FATFS]
"EnableCache"=dword:1
"CacheSize"=dword:1000
"MountBootable"=dword:1
"Flags"=dword:00000024
"CheckForFormat"=dword:1
然后编写应用程序，主要就是通过CreateFile来打开DSK1:设备，然后通过DeviceIoControl(..)函数来调用FMD_OEMIoControl(..)函数，来达到直接读/写/擦除Flash设备的目的。应用程序代码如下：
HANDLE hFirm;
 hFirm = CreateFile(TEXT("DSK1:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
 if(hFirm == INVALID_HANDLE_VALUE)
 {
  printf("Open Flash Device Failed");
  return 0;
 }
 iRet = DeviceIoControl(hFirm, 0xff123456, para1, para2, para3, para4, para5, para6);  //Read Flash Sector
 iRet = DeviceIoControl(hFirm, 0xff654321, para1, para2, para3, para4, para5, para6);  //Write Flash Sector
 iRet = DeviceIoControl(hFirm, 0xff123457, para1, para2, para3, para4, para5, para6);  //Erase Flash Block
 printf("DeviceIoControl OK/r/n");
 while(1)
  ;
通过上面的应用程序，就能够调用到Flash设备驱动中的FMD_OEMIoControl(..)函数，这样根据不同的case就可以调用读/写/擦除函数了。
2. 以Nandflash为例，当然对于NORFlash来说大同小异，注册表配置如下：
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/NANDFlash]
"Dll"="ep94xxnandflash.dll"
"Prefix"="DSK"
"Order"=dword:4
;"Ioctl"=dword:4
"Profile"="NSFlash"
"IClass"="{A4E7EDDA-E575-4252-9D6B-4195D48BB865}"
; Override names in default profile
[HKEY_LOCAL_MACHINE/System/StorageManager/Profiles/NSFlash]
 "Name"="Ep94xx NAND Flash"
 "Folder"="NANDFlash"
 "PartitionDriver"="MSPart.dll"
 "AutoMount"=dword:1
 "AutoPart"=dword:1
 "AutoFormat"=dword:1
[HKEY_LOCAL_MACHINE/System/StorageManager/Profiles/NSFlash/FATFS]
"EnableCache"=dword:1
"CacheSize"=dword:1000
"MountBootable"=dword:1
"Flags"=dword:00000024
"CheckForFormat"=dword:1
[HKEY_LOCAL_MACHINE/System/StorageManager/AutoLoad/NSFlash]
"DriverPath"="Drivers//BuiltIn//NANDFlash"
"LoadFlags"=dword:0
"BootPhase"=dword:1
然后编写应用程序，主要就是通过OpenStore来打开NSFlash，然后通过DeviceIoControl(..)函数来调用FMD_OEMIoControl(..)函数，来达到直接读/写/擦除Flash设备的目的。应用程序代码如下：
 HANDLE hFirm;
 hFirm = OpenStore(L"NSFlash");
 if(hFirm == INVALID_HANDLE_VALUE)
 {
  printf("Open Flash Device Failed");
  return 0;
 }
 iRet = DeviceIoControl(hFirm, 0xff123456, para1, para2, para3, para4, para5, para6);
 iRet = DeviceIoControl(hFirm, 0xff654321, para1, para2, para3, para4, para5, para6);
 iRet = DeviceIoControl(hFirm, 0xff123457, para1, para2, para3, para4, para5, para6);
 printf("DeviceIoControl OK/r/n");
 while(1)
  ;
通过这种方法，也可以在应用程序中调用到FMD_OEMIoControl(..)函数，从而达到直接访问Flash设备的目的。
总结一下，上面的两种方法大致原理其实是一样的，都是通过DeviceIoControl函数来调用FMD_OEMIoControl函数，然后达到直接访问Flash驱动的目的，这样就可以在应用程序中直接读/写/擦除Flash设备了。
最后需要注意的是：你的Flash驱动里面需要对读/写/擦除等直接操作Flash硬件的函数进行保护，因为Flash设备应该是由WinCE的文件系统来管理的，而现在你的应用程序也可以直接访问它了，所以保险起见，添加互斥量保护避免访问冲突。
上面的所有实现，都是在WinCE6.0上面做得，相信在WinCE5.0上面应该差不多。
//========================================================================================
备注：：
1》我用CreateFile(TEXT("DSK1:")的方式跟用OpenStore(L"NANDFLASH")的方式都可以打开成功，并读取成功，但这2种方式读取同一个地方读到的结果是不一样的！！！很奇怪！！！不过通过**CreateFile(TEXT("DSK1:")方式打开的句柄进行DeviceIoControl操作会失败！！！**
**2》看这篇文章觉得其实**CreateFile(TEXT("DSK1:")的方式跟用OpenStore(L"NANDFLASH")的方式是都可以读写NANDFLASH成功的，所不同的是注册表需要进行对应的配置----但我现在没去改注册表来试！！！****
