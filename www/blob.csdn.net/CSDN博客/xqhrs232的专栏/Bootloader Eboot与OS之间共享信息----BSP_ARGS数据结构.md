# Bootloader/Eboot与OS之间共享信息----BSP_ARGS数据结构 - xqhrs232的专栏 - CSDN博客
2013年10月24日 14:50:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1290
原文地址::[http://dev.10086.cn/cmdn/bbs/thread-19880-1-1.html](http://dev.10086.cn/cmdn/bbs/thread-19880-1-1.html)
相关文章
1、WinCE EBOOT中的Boot Args与Driver Globals 
 ----[http://hgh123.blog.163.com/blog/static/59804221200932310524791/](http://hgh123.blog.163.com/blog/static/59804221200932310524791/)
2、**Boot Args与Driver Globals—WinCE EBOOT----[http://bbs.dzsc.com/space/viewspacepost.aspx?postid=104661](http://bbs.dzsc.com/space/viewspacepost.aspx?postid=104661)**
# BOOTLOADER与OS之间共享信息
=============================================================
标题：BOOTLOADER与OS之间共享信息
摘要：BSP_ARGS数据结构
备注： Windows Embedded CE 6.0
日期：2010.4.23
姓名：朱铭雷
=============================================================
BootLoader获取的数据信息，有些要传递给Windows CE系统使用。途径是在RAM中开辟一块存储区域，通过一个结构体来保存这些信息。
EBOOT下有一个loader.h文件，其中有如下定义：
// Driver globals pointer (parameter sharing memory used by bootloader and OS).
#define
pBSPArgs
((BSP_ARGS *) IMAGE_SHARE_ARGS_UA_START)
从注释可以看出，pBSPArgs指向一块内存，用来保存bootloader和OS之间的共享参数。pBSPArgs是一个结构体（(BSP_ARGS）类型的指针。BSP_ARGS结构体的成员中，就保存了这些共享参数。
IMAGE_SHARE_ARGS_UA_START宏在image_cfg.h文件中定义：
#define IMAGE_SHARE_ARGS_UA_START
0xA0020000
#define IMAGE_SHARE_ARGS_CA_START
0x80020000
#define IMAGE_SHARE_ARGS_SIZE
0x00000800
实际上0xA0020000和0x80020000虚拟地址都映射到0x30020000物理地址，大小为0x00000800，也就是2KB。在Config.bib文件中，也有相关定义：
MEMORY
ARGS
80020000
00000800
RESERVED
BSP_ARGS结构体的定义在\INC\args.h文件中：
———————————————————————————————————————
typedef struct {
OAL_ARGS_HEADER header;
UINT8 deviceId[16];
// Device identification
OAL_KITL_ARGS kitl;
// CAUTION:
The DeviceEmulator contains hard-coded knowledge of the addresses and contents of these
//
three fields.
UINT32 ScreenSignature;
// Set to BSP_SCREEN_SIGNATURE if the 
//DeviceEmulator specifies screen size
UINT16 ScreenWidth;
// May be set by the DeviceEmulator, or zero
UINT16 ScreenHeight;
// May be set by the DeviceEmulator, or zero
UINT16 ScreenBitsPerPixel;
// May be set by the DeviceEmulator, or zero
union {
struct {
UINT16 fSoftReset:1;
}bit;
UINT16 u16Value;
} EmulatorFlags;
// May be set by the DeviceEmulator, or zero
UINT8
ScreenOrientation;
// May be set by the DeviceEmulator, or zero
UINT8
Pad[15];
// May be set by the DeviceEmulator, or zero
BOOL
fUpdateMode;
// Is the device in update mode?
BOOL
fUpdateMode;
// Is folder sharing enabled?
// record the size and location of the RAM FMD if present
DWORD dwExtensionRAMFMDSize;
PVOID pvExtensionRAMFMDBaseAddr;
} BSP_ARGS;
header是信息头，用来指示pBSPArgs所指内存是否包含有效信息。
deviceId是设备ID，用来标识KITL使用的端口外设。
kitl用于存储KITL端口的相关配置信息。
ScreenSignature是Screen的数字签名。
ScreenWidth和ScreenHeight是屏幕的宽和高。
ScreenBitsPerPixel是控制每个像素的位数。
EmulatorFlags用于记录系统启动的方式是硬启动还是软启动（从省电模式下恢复）。
ScreenOrientation是屏幕显示为横向还是纵向。
Pad用于32位字对齐。
fUpdateMode和fUpdateMode保留未用。
BootLoader和OS之间共享数据信息也不是必须的，如果没有这个需要，也可以不做这方面的工作。
