# VFW 介绍 - 深之JohnChen的专栏 - CSDN博客

2009年07月16日 10:48:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3821



### 一种实现视频捕捉的简单方法

沈旭

（湛江师范学院 信息科学与技术学院，广东  湛江  524048）

**   摘　要**  当前，随着视频监控、可视电话、电视会议等多媒体应用技术的迅速发展，对数字视频捕获技术的要求越来越高。实现视频捕获的方法有很多，本文主要介绍了微软公司VFW(Video for Windows)软件包中的AVICap窗口类的成员函数和一些关键宏，以及与编写视频捕捉程序紧密相关的几个结构体。最后，通过一个视频捕捉应用程序，展示了如何具体实现视频捕捉，以及要注意的相关问题。

**关键词** VFW；AVICap窗口类；视频捕捉；捕捉窗口；视频对话框

**0  引言**

    随着数字视频监控、可视电话、电视会议等多媒体技术应用的迅速兴起，越来越多的场合需要对数字视频信号进行捕捉。通常来说，捕捉实时数字视频信号是一个比较复杂的过程，但是，微软公司开发的Video for Windows SDK软件包中AVICap窗口类提供了一条捷径，借助于该窗口类，能够很方便地将视频捕捉的各种功能组合到应用程序中去。

# 1  AVICap窗口类简介

    ACICap支持实时的视频流捕捉和视频单帧捕捉。使用ACICap窗口类可创建具有一些基本功能的窗口，例如视频图像的预览、设置捕捉参数的对话框、音频、视频捕捉的独立控制等。ACICap中的回调函数可使应用程序向用户提供有关捕捉的状态，包括进行的过程指示，以及任何可能产生的错误。开发人员可以设置一个标志用来指示在什么时候采集到音频，什么时候采集到视频。这样，应用程序可以直接使用数据而无需写入AVI文件中。

    AVICap窗口类提供了以下功能：

    ◆  单独控制音频、视频的采集；

    ◆  采用overlay（实时叠加）或preview（预览）方式显示视频图像；

    ◆  与ICM和ACM同时工作，将音频和视频数据直接压缩到应用程序中；

    ◆  将音频、视频流直接压缩入AVI文件而不需要开发人员详细了解AVI文件格式的细节；

    ◆  动态了解视频和音频的输入设备；

    ◆  创建、保存和载入调色板；

    ◆  将图像调色板拷贝到剪切板上；

    ◆  控制MCI设备；

    ◆  捕捉单帧图像并以DIB格式保存。

# 2  AVICap窗口类的主要函数、宏简介

    AVICap提供给开发人员一整套函数，用这些函数可以实现许多视频捕捉程序所需的窗口管理；同时，在整个捕捉过程中仍然保留全部的控制。这些函数形式简单，采用基于消息的接口来获取硬件里的音频和视频信号，同时控制着视频流采集到磁盘的过程。

    ACICap的函数能够使开发人员以很少的投入来创建具有基本捕捉功能的采集程序，这些函数是高级的、经过优化的、为开发人员创建具有自己特性的应用程序留有很大的灵活性。 

    下面是ACICap提供给开发人员编写捕捉程序的几个重要函数和宏。

    ◆ 创建捕捉窗口

HWND VFWAPI capCreateCaptureWindow(

      LPCSTR lpszWindowName，// 捕捉窗口名字

      DWORD dwStyle，// 捕捉窗口的风格

      int x，// 窗口左上角x轴坐标

      int y，// 窗口左上角y轴坐标

      int nWidth，// 窗口的宽度

      int nHeight，// 窗口的高度

      HWND HWnd，// 父窗口句柄

      Int nID// 捕捉窗口的ID号

);

    如果该函数调用成功 则函数返回窗口的句柄 否则函数返回NULL。

    ◆ 捕捉窗口与设备连接

BOOL capDriverConnect(

    hwnd，// 捕捉窗口的句柄

    iIndex// 设备驱动号

);

    如果连接成功，返回TRUE否则函数返回FALSE。

    ◆ 获取视频捕捉设备功能

BOOL capDriverGetCaps(

    hwnd，// 捕捉窗口句柄

    psCaps，// 指向一个用于存储返回值CAPDRIVERCAPS结构的指针

    wSize// CAPDRIVERCAPS结构占用的字节数

);

    如果捕获窗口与捕获驱动连接成功，则返回TURE，否则，返回FALSE。

    ◆ 设置捕捉设备的参数

BOOL capCaptureSetSetup(

   hwnd，// 设置窗口句柄

   psCapParms，// 指向一个用于存储返回值的CAPDRIVERCAPS结构的指针

   wSize// CAPDRIVERCAPS结构占用的字节数

);

    设置成功返回TURE，否则返回FALSE。

◆ 设置处理回调状态

BOOL capSetCallbackOnStatus(

   hwnd，// 捕捉窗口句柄

   fpProc// 指向状态回调函数的指针

);

◆ 设置错误处理

BOOL capSetCallbackOnError(

   hwnd，// 捕捉窗口句柄

   fpProc// 指向错误回调函数的指针

);

    ◆ 设置数据文件名

BOOL capFileSetCaptureFile(

   hwnd，// 捕捉窗口句柄

   szName// 指向字符串的指针，字符串内容为捕获文件名

);

   设置一个由szName指向的字符串作为文件名，用于存储从捕捉窗口hWnd采集的视频图像数据，成功，返回TRUE，否则返回FALSE。

    ◆ 开始捕捉视频

BOOL capCaptureSequence(

       hwnd

);

   触发程序开始捕捉视频图像并将其保存到数据文件。

    ◆ 视频源设置对话框

    BOOL capDlgVideoSource( hwnd ); // hwnd：捕捉窗口句柄

    视频源设置对话框对于每一个捕捉驱动程序来说，是唯一的。而且，有些驱动程序不一定支持这一功能。应用程序可以通过检测CAPDRIVERCAPS结构的成员变量fHasDlgVideoSource来判断驱动程序是否支持这一功能。

    ◆ 视频格式设置对话框

    BOOL capDlgVideoFormat( hwnd ); // hwnd：捕捉窗口句柄

视频格式设置对话框对于每一个捕捉驱动程序来说，是唯一的。而且，有些驱动程序不一定支持这一功能。应用程序可以通过检测CAPDRIVERCAPS结构的成员变量fHasDlgVideoFormat来判断驱动程序是否支持这一功能。

     ◆ 视频显示方式设置对话框

    BOOL capDlgVideoDisplay( hwnd ); // hwnd：捕捉窗口句柄

视频格式设置对话框对于每一个捕捉驱动程序来说，是唯一的。而且，有些驱动程序不一定支持这一功能。应用程序可以通过检测CAPDRIVERCAPS结构的成员变量fHasDlgVideoDisplay来判断驱动程序是否支持这一功能。

    ◆ 视频压缩设置对话框

    BOOL capDlgVideoDisplay( hwnd ); // hwnd：捕捉窗口句柄

    视频格式设置对话框允许用户在视频捕获期间选择不同的压缩器。

# 3  几个重要结构

    编写视频捕捉程序往往要用到以下与视频捕捉相关的结构

    （1）CAPTUREPARMS：包括控制视频流捕捉过程的参数，这一结构被用来得到和设置影响捕捉速率、捕捉时的缓冲区数目、以及捕捉如何结束时的参数。

    （2）CAPSTATUS：定义了捕捉窗口的当前状态，如：以象素为单位表示图像的高、宽、预览和重叠方式的标志量，尺寸缩放的标志量等。

因为捕捉窗口的状态随各种各样的消息而改变，所以当应用程序需要功能菜单项，决定捕捉窗口的真实状态或者调用视频格式对话框时，都应该更新这一结构中的信息。

    （3）CAPDRIVERCAPS：定义了视频捕捉驱动程序的功能，如：驱动程序的数目索引是否支持视频叠加功能等。当应用程序将捕捉窗口与视频捕捉驱动程序相连接时，应该发送消息WM_CAP_DREVER_GET_CAPS或者调用宏capDriverGetCaps将驱动程序的功能拷贝一份到该结构中。

    （4）VIDEOHDR：定义了视频数据块的头信息，其数据成员lpData（指向数据缓存的指针），和dwBufferLenth（数据缓存的大小）经常用到。

# 4  视频捕捉举例

    这是一个视频捕捉应用程序，实现了视频捕捉的基本功能，并且可以设置视频的来源、视频的格式等。

#include "Vfw.h"

#include <windowsx.h>

#pragma comment( lib， "Vfw32.lib" )   // 使用Vfw32.lib库

LRESULT CALLBACK capVideoStreamCallback(HWND hWnd，LPVIDEOHDR lpVHdr);

HWND hMyWnd;

LPBITMAPINFO lpbi;   // 视频格式

void CWin32VideoCaptureDlg::OnBnClickedOk()

{

hMyWnd=this->m_hWnd;

*//* Step 1. 建立捕获窗口

HWND hWndC=capCreateCaptureWindow(

(LPSTR)"MyCaptureWindow"，

WS_CHILD | WS_VISIBLE，

0，0，160，120，

this->m_hWnd，

0);

*//* Step 2. 与驱动程序建立连接

LRESULT fOK = ::SendMessage (hWndC， WM_CAP_ DRIVER_ CONNECT， 0， 0L);

*//* Step 3. 列举出目前系统所拥有的捕获驱动程序

         char szDeviceName[80];

         char szDeviceVersion[80];

         CString DeviceList;

         for (int wIndex = 0; wIndex < 10; wIndex++)         {

            if (capGetDriverDescription (wIndex，szDeviceName，/

                            sizeof (szDeviceName)， szDeviceVersion， /

                            sizeof (szDeviceVersion)))     {

                            // 列出系统已安装的驱动程序，让用户选择

                            DeviceList.Append(szDeviceName);

                   }

         }

         MessageBox(DeviceList，"系统中 Capture Driver 描述");

*//* Step 4. 获得驱动程序的性能

    //我们据此判断使用者是否可以设定 Video Source 和 Video Format

    CAPDRIVERCAPS CapDriverCaps;

    ::SendMessage (hWndC， WM_CAP_DRIVER_GET _CAPS， /

                   sizeof (CAPDRIVERCAPS)， (LONG) (LPVOID) &CapDriverCaps);

*//* Step 5.选择视频源

         if (CapDriverCaps.fHasDlgVideoSource)

                   capDlgVideoSource(hWndC);

（可以选用不同的摄像头）

*//* Step 6.调整视频格式

       CAPSTATUS CapStatus;

 if (CapDriverCaps. FhasDlg VideoFormat){

                   capDlgVideoFormat(hWndC);

                   capGetStatus(hWndC， &CapStatus， sizeof (CAPSTATUS));

                   }

*//* Step 7.调整视频输出特性(亮度，对比度，色深)

         if (CapDriverCaps.fHasDlgVideoDisplay)

                   capDlgVideoDisplay(hWndC);

*//* Step 8. 获得硬件状态

         BOOL bOK=capGetStatus(hWndC， &CapStatus， sizeof (CAPSTATUS));

*//* Step 9. 调整捕捉窗口大小

         ::SetWindowPos(hWndC， NULL， 0， 0， CapStatus. uiImage Width， /

                    CapStatus.uiImageHeight，

SWP_NOZORDER | SWP_NOMOVE);

*//* Step 10. 设置视频格式

         DWORD dwSize;

         dwSize = capGetVideoFormatSize(hWndC);

         lpbi = (LPBITMAPINFO) GlobalAllocPtr(GHND， dwSize);

         capGetVideoFormat(hWndC， lpbi， dwSize);

*//* Step 11. 视频预览

 capPreviewRate(hWndC， 66);    

         capPreview(hWndC， TRUE);     

// Step 12. 当填满一帧后，调用回调函数处理

BOOL bOk= capSetCallbackOnFrame(hWndC， capVideo StreamCallback);

}

int gdwFrameNum=0;

char *gachBuffer=new char[100];

LRESULT CALLBACK capVideoStreamCallback(HWND hWnd，LPVIDEOHDR lpVHdr){

   if (!hWnd)

        return FALSE;

        wsprintf(gachBuffer， "Preview frame# %ld 一个pixel=%d bits; 宽=%d; 高=%d;

               (RGB)=(%d，%d，%d) 使用长度=%d" ，/

               gdwFrameNum++， lpbi->bmiHeader. biBit Count，/

lpbi->bmiHeader.biWidth， lpbi-> bmiHeader.  biHeight，/

                               (int)*(lpVHdr->lpData)，/

                               (int)*(lpVHdr->lpData+1)，/

                               (int)*(lpVHdr->lpData+2)，/

                               lpVHdr->dwBytesUsed  );

        SetWindowText(hMyWnd， (LPSTR)gachBuffer);

        return (LRESULT) TRUE ;

  }

    这里要指出的一点是，如果在程序Link中出现不能识别函数的错误时，可以采取两种方式：

    ＊在#include<vfw.h>后加入一行预编译代码：#pragma  comment(lib，“vfw32”)

    ＊或者，在Link设置中加入vfw32.lib。

# 5  结束语

    微软公司开发的Video For Windows具有简单易用、开发容易的优点，可以使得不了解组件编程技术的软件编程人员快速开发出适合自己的视频捕获应用程序。但是，它也具有一些不足之处，诸如：不能处理音、视频同步问题；不支持变长帧编码技术等。针对这些不足，微软目前使用DirectShow与WDM Stream Class来解决问题，但是，这些技术比较复杂，实现起来相对困难，因此，可以预见，在今后的一段时间里，AVICap技术仍将是视频捕获实现的重要选项。

# 参考文献

    1  Microsoft Inc. Video for Windows Developed Toolkit Programming Guide. 2001

    2  张基温，贾中宁，李伟. Visual C++ 程序开发基础. 北京：高等教育出版社. 2002

    3 王超龙，陈志华. Visual C++ 6.0 入门与提高. 北京：人民邮电出版社. 2002

**    作者简介：**沈旭（1979-），男，山东单县人，硕士，计算机专业教师，主要研究方向：嵌入式系统、视频水印等，sjrgl@sina.com

