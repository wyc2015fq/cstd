# WinCE6.0 KITL概要 - xqhrs232的专栏 - CSDN博客
2009年11月21日 23:15:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1490
 原文地址::[http://www.cnblogs.com/we-hjb/archive/2008/10/16/1312236.html](http://www.cnblogs.com/we-hjb/archive/2008/10/16/1312236.html)
KITL（Kernel Independent Transport Layer）即内核独立传输层，它为我们提供了一种调试WinCE的简便方法。KITL将通信服务协议和用于通信的硬件分离开来。所以我们在创建硬件传输层时就省去很多麻烦，否则我们自己必须实现与设备进行数据交互的协议。KITL工作在硬件传输层之上，因此，它无须关心用于通信的具体硬件，我们可以用USB、Serial或者Ethernet作为KITL的调试通道。具体选择哪一个，由硬件平台和软件资源决定。有些设备没有Ethernet和Serial接口，所以只能采用USB，如Mobile设备。如果系统采用了EBOOT，则建议使用Ethernet作为调试通道。这时，配置KITL的代价相对来说也很小。无论如何，KITL相当强大，在BSP的移植过程中，花一些时间来实现KITL的功能是完全值得的。完成KITL之后，你会发现所有的时间都没有白花。由于KITL的实现，后续的调试节省了很多时间。磨刀不误砍柴工！工欲善其事，必先利其器！我深有体会！:-D以前没有认识到KITL的强大，一直没有碰它。最近在实现KITL的功能之后，随即顺利调通了几个顽固的驱动。虽然问题本身不值一提，但没有KITL时，驱动出了状况，内核就挂了，不知道挂在哪里，无从下手，也不好分析。而KITL可以帮助我们定位出现问题的位置。KITL，一用就知道是我想要的。BTW：按启动顺序来说，KITL启动应该在OAL之后，内核之前。所以，必须先完成OAL的移植，才能进一步移植KITL。
闲话少说，接下来介绍WinCE6.0 KITL的基本情况。WinCE6.0中，KITL从OAL中独立出来，单独编译成kitl.dll。在BSP中的目录一般为%_WINCEROOT%"PLATFORM"
 BSPNAME"SRC"kitl。在该目录下有一个kitl.c的文件，这是BSP中有关KITL的主要代码所在。核心代码如下:     
Code
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//------------------------------------------------------------------------------
//
// Platform entry point for KITL.  Called when KITLIoctl (IOCTL_KITL_STARTUP, ![](http://www.cnblogs.com/Images/dot.gif)) is called.
//
BOOL OEMKitlStartup(void)
{
    BOOL rc;
    OAL_KITL_ARGS *pArgs, args;
    CHAR *szDeviceId, buffer[OAL_KITL_ID_SIZE];
    KITL_RETAILMSG(ZONE_KITL_OAL, ("+OEMKitlStartup/r/n"));
// Print banner.  Will remove when KITL-over-ethernet support is dropped
// (in M3)
    KITLOutputDebugString("/n*********************************************/n");
    KITLOutputDebugString("*                                           */n");
    KITLOutputDebugString("*  This image uses KITL-over-ethernet       */n");
    KITLOutputDebugString("*                                           */n");
    KITLOutputDebugString("*  PB Connectivity Options must be set to:  */n");
    KITLOutputDebugString("*  Download:  /"Device Emulator/"             */n");
    KITLOutputDebugString("*  Transport: /"Ethernet/"                    */n");
    KITLOutputDebugString("*                                           */n");
    KITLOutputDebugString("*********************************************/n/n");
// Look for bootargs left by the bootloader or left over from an earlier boot.
//
    pArgs      = (OAL_KITL_ARGS*)OALArgsQuery(OAL_ARGS_QUERY_KITL);
    szDeviceId = (CHAR*)OALArgsQuery(OAL_ARGS_QUERY_DEVID);
// If we don't have bootargs in RAM, look first in NOR flash for the information
// otherwise look on the SmartMedia NAND card (in case we're peRForming a NAND-only) boot.
//
if (pArgs == NULL)
    {
        SectorInfo si;
        UINT8 maccount =0;
// Get MAC address from NAND flash![](http://www.cnblogs.com/Images/dot.gif)
//
if (FMD_Init(NULL, NULL, NULL) == NULL)
        {
            KITL_RETAILMSG(ZONE_ERROR, ("ERROR: Failed to initialize NAND flash controller./r/n"));
return(FALSE);
        }
// If block 0 isn't reserved, we can't trust that the values we read for the MAC address are
// correct.  They may actually be valid logical sector numbers (we're overloading the use
// of the logical sector number field).
//
if (!(FMD_GetBlockStatus(0) & BLOCK_STATUS_RESERVED))
        {
            KITL_RETAILMSG(ZONE_ERROR, ("ERROR: Block 0 isn't reserved - can't trust MAC address values stored in NAND./r/n"));
return(FALSE);
        }
        KITL_RETAILMSG(ZONE_KITL_OAL, ("INFO: Using KITL arguments stored on SmartMedia./r/n"));
        memset(&args, 0, sizeof(args));
        args.flags = OAL_KITL_FLAGS_ENABLED | OAL_KITL_FLAGS_DHCP | OAL_KITL_FLAGS_VMINI;
        args.devLoc.IfcType = Internal;
        args.devLoc.BusNumber =0;
        args.devLoc.LogicalLoc = BSP_BASE_REG_PA_CS8900A_IOBASE;
        args.ipAddress =0;
// We know the first block of NAND flash must be good, so we needn't worry about bad blocks when reading.
//
        maccount =0;
do
        {
if (!FMD_ReadSector(maccount, NULL, &si, 1))
            {
                KITL_RETAILMSG(ZONE_ERROR, ("ERROR: NAND flash read error (sector = 0x%x)./r/n", maccount));
return(FALSE);
            }
            args.mac[maccount] = (UINT16)(si.dwReserved1 &0xFFFF);
        } while(++maccount <3);
        pArgs =&args;
    }        
// If there isn't a device ID from the bootloader create one.
//
if (szDeviceId == NULL)
    {
        OALKitlCreateName(BSP_DEVICE_PREFIX, pArgs->mac, buffer);
        szDeviceId = buffer;
    }
// Finally call KITL library.
//
    rc = OALKitlInit(szDeviceId, pArgs, g_kitlDevices);
    KITL_RETAILMSG(ZONE_KITL_OAL, ("-OEMKitlStartup(rc = %d)/r/n", rc));
return(rc);
}
上面的代码是WinCE6.0中模拟器的基于Ethernet的KITL实现部分，可以看到，OEMKitlStartup()先获取关于KITL的相关信息，然后调用OALKitlInit()初始化KITL的功能。参数g_kitlDevices的定义在文件kitl_cfg.h中，代码如下：     
Code
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->#ifndef __KITL_CFG_H
#define __KITL_CFG_H
//------------------------------------------------------------------------------
OAL_KITL_ETH_DRIVER g_kitlEthCS8900A = OAL_ETHDRV_CS8900A;
OAL_KITL_DEVICE g_kitlDevices[] = {
    { 
        L"CS8900A", Internal, BSP_BASE_REG_PA_CS8900A_IOBASE, 0, OAL_KITL_TYPE_ETH, 
&g_kitlEthCS8900A
    }, {
        NULL, 0, 0, 0, 0, NULL
    }
};    
//------------------------------------------------------------------------------
#endif
     根据硬件平台的不同，设置相应的配置。到这里，代码部分就基本完成了。接下来配置OS Design，如下图所示：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/KITL_SETTINGS.png)
     重新编译OS Design。下载之前，设置Connectivity Options。万事俱备，下载测试。顺利的话会在超级终端打印如下图所示信息:
![](http://images.cnblogs.com/cnblogs_com/we-hjb/KITL_INFO.png)
系统启动完成后，会在网络连接里看到VMINI的连接图标，如下图所示。这里补充一点，在启用KITL时必须将原来的网络驱动停掉，否则会与KITL冲突，导致系统启动不了。VMINI应该是一个桥接的网卡设备，它和KITL共享硬件网络接口。这时你可以通过KITL进行调试，也可以通过VMINI1访问Internet。不过，建议不要尝试打开网页，因为一般会造成网络堵塞而死机。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/VMINI.png)
至此，KITL的功能就实现了，接下来开始调试WinCE系统，也是KITL大显身手的时候了。调试截图如下：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/KITL_DEBUG.png)
     可以看到文件ufnmdd.cpp中有一个DEBUGCHK failed了，这里给出了详细的说明，一看就知道问题在哪。在调试WinCE6.0时经常会出现yzkiqfe等字样的目录，这是WinCE6.0的研发代号(yamazaki)，好像是一种日本葡萄酒的名字（WinCE的研发代号似乎一直用葡萄酒的名字来命名）。我们不用管它，在需要定位目录时，浏览到PB6.0的安装目录即可。通过KITL，还可以设置断点，单步运行，查看寄存器等等。总之，KITL在调试OAL和驱动尤其是本地驱动时有不可替代的作用。
    本文简要介绍了WinCE6.0中KITL的相关内容，以Ethernet为例简单分析了KITL的代码。文中有不贴切的地方，欢迎您指正！
