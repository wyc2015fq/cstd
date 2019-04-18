# VFW编程实例 - 深之JohnChen的专栏 - CSDN博客

2009年07月16日 10:38:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4790


 VFW(Video for Windows)是Microsoft推出的关于数字视频的一个软件开发包，VFW的核心是AVI文件标准。AVI(Audio Video Interleave)文件中的音、视频数据帧交错存放。围绕AVI文件，VFW推出了一整套完整的视频采集、压缩、解压缩、回放和编辑的应用程序接口(API)。由于AVI文件格式推出较早且在数字视频技术中有广泛的应用，所以VFW仍然有很大的实用价值，而且进一步发展的趋势。

　　在VC++开发环境中调用VFW和使用其它开发包没有什么不同，只是需要将VFW32.lib文件加入工程中，但在开放视频捕捉与压缩管理程序时需要其它软件硬件设置。VFW为AVI文件提供了丰富的处理函数和宏定义，AVI文件的特点在于它是典型的数据流文件，它由视频流、音频流、文本流组成。所以对AVI文件的处理主要是处理文件流。

编程实例
为了能够捕获视频帧，要启动一个捕获帧回调函数VideoStreamCallBack。
捕获一个视频流或当前设备状态时分别使用以下函数： 
//捕获一个视频流 
CapSetCallbackOnVideoStream;  
//得到一个设备错误 
CapSetCallbackOnError;  
//得到一个设备状态 
CapSetCallbackOnStatus  
} 自定义的函数1 //定义一个帧捕获回调函数
CapSetCallbackOnFrame (ghCapWnd,LongInt(@VideoStreamCallBack));  
//将一个捕获窗口与一个设备驱程相关联，第二个参数是个序号，当系统中装有多个显视驱动程序时，其值分别依次为0到总个数 
CapDriverConnect(ghCapWnd, 0);  
//设置设备属性的结构变量 
CapParms.dwRequestMicroSecPerFrame:=40000; 
CapParms.fLimitEnabled := FALSE; 
CapParms.fCaptureAudio := FALSE; // NO Audio 
CapParms.fMCIControl := FALSE; 
CapParms.fYield := TRUE; 
CapParms.vKeyAbort := VK_ESCAPE; 
CapParms.fAbortLeftMouse := FALSE; 
CapParms.fAbortRightMouse := FALSE; 
//使设置生效 
CapCaptureSetSetup(ghCapWnd,LongInt(@CapParms),sizeof(TCAPTUREPARMS));  
//设置预览时的比例 
CapPreviewScale(ghCapWnd, 1); 
//设置预览时的帧频率 
CapPreviewRate(ghCapWnd,66); 
//如果要捕获视频流，则要使用函数指定不生成文件。否则将会自动生成AVI文件 
CapCaptureSequenceNoFile(ghCapWnd); 
//指定是否使用叠加模式，使用为1，否则为0 
CapOverlay(ghCapWnd, 1); 
//打开预览 
CapPreview(ghCapWnd, 1); 
//停止捕获 
capCaptureAbort(ghCapWnd);  
//将捕获窗同驱动器断开 
capDriverDisconnect(ghCapWnd); 

自定义的函数2 //定义捕获帧回调函数： 
function FrameCallBack(hWnd:HWND; lpVHdr:LongInt) :LongInt; stdcall; 
var  
DataPoint:^byte;  
DibLen,RectWidth,RectHeight:integer; 
begin  
//转换从回调函数中得到的指针 
VideoStr:=LPVIDEOHDR(lpVHdr); 
//得到返回的数据大小  
DibLen:=VideoStr^.dwBufferLength;  
GetMem(DataPoint,64000); 
//将帧数据COPY到一个内存中，注意DATAPOINT要先分配空间 
CopyMemory(DataPoint,VideoStr^.lpData,Diblen);  
//一些其他处理 
…… 
end; 
灵活地使用AVICap窗口类的回调函数可以满足各种不同的需求，但要注意从视频卡中捕获的视频数据的格式和图像的长宽要参考视频卡的参数。而且有些视频卡通过设置可支持多种的格式和图像长宽，所以在还原图像时要注意参考所用的视频卡的参数。

与视频捕获相关的编程。
1、定义全局变量：
HWND ghWndCap ; //捕获窗的句柄
CAPDRIVERCAPS gCapDriverCaps ; //视频驱动器的能力

CAPSTATUS gCapStatus ; //捕获窗的状态
2、处理WM_CREATE消息：
//创建捕获窗,其中hWnd为主窗口句柄
ghWndCap = capCreateCaptureWindow（（LPSTR）"Capture Window",WS_CHILD | WS_VISIBLE, 0, 0, 300,240, （HWND） hWnd, （int） 0）;
//登记三个回调函数，它们应被提前申明
capSetCallbackOnError（ghWndCap, （FARPROC）ErrorCallbackProc）; capSetCallbackOnStatus（ghWndCap, （FARPROC）StatusCallbackProc）; capSetCallbackOnFrame（ghWndCap, （FARPROC）FrameCallbackProc）;

capDriverConnect（ghWndCap,0）; // 将捕获窗同驱动器连接
//获得驱动器的能力,相关的信息放在结构变量gCapDriverCaps中
capDriverGetCaps（ghWndCap,&gCapDriverCaps,sizeof（CAPDRIVERCAPS）） ;
3、处理WM_CLOSE消息:
//取消所登记的三个回调函数
capSetCallbackOnStatus（ghWndCap, NULL）;
capSetCallbackOnError（ghWndCap, NULL）;
capSetCallbackOnFrame（ghWndCap, NULL）;
capCaptureAbort（ghWndCap）;//停止捕获
capDriverDisconnect（ghWndCap）; //将捕获窗同驱动器断开

4、处理菜单项Preview:
capPreviewRate（ghWndCap, 66）; // 设置Preview模式的显示速率
capPreview（ghWndCap, TRUE）; //启动Preview模式
5、处理菜单项Overlay:
if（gCapDriverCaps.fHasOverlay） //检查驱动器是否有叠加能力
capOverlay（ghWndCap,TRUE）; //启动Overlay模式
6、处理菜单项Exit:
SendMessage（hWnd,WM_CLOSE,wParam,lParam）;
7、分别处理Setting下的三个菜单项，它们可分别控制视频源、视频格式及显示：
if （gCapDriverCaps.fHasDlgVideoSource）
capDlgVideoSource（ghWndCap）; //Video source 对话框
if （gapDriverCaps.fHasDlgVideoFormat）
capDlgVideoFormat（ghWndCap）; // Video format 对话框
if （CapDriverCaps.fHasDlgVideoDisplay）
capDlgVideoDisplay（ghWndCap）; // Video display 对话框
8、处理Video Stream菜单项,它捕获视频流到一个.AVI文件:
char szCaptureFile[] = "MYCAP.AVI";
capFileSetCaptureFile（ ghWndCap, szCaptureFile）; //指定捕获文件名
capFileAlloc（ ghWndCap, （1024L * 1024L * 5））; //为捕获文件分配存储空间
capCaptureSequence（ghWndCap）; //开始捕获视频序列
9、处理Single Frame菜单项:
capGrabFrame（ghWndCap）; //捕获单帧图像
10、定义三个回调函数：
LRESULT CALLBACK StatusCallbackProc（HWND hWnd, int nID, LPSTR lpStatusText）
{
if （!ghWndCap） return FALSE;
//获得捕获窗的状态
capGetStatus（ghWndCap, &gCapStatus, sizeof （CAPSTATUS））;
//更新捕获窗的大小
SetWindowPos（ghWndCap, NULL, 0, 0, gCapStatus.uiImageWidth,
gCapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE）;
if （nID == 0） { // 清除旧的状态信息
SetWindowText（ghWndCap, （LPSTR） gachAppName）;
return （LRESULT） TRUE;
}
// 显示状态 ID 和状态文本
wsprintf（gachBuffer, "Status# %d: %s", nID, lpStatusText）;
SetWindowText（ghWndCap, （LPSTR）gachBuffer）;
return （LRESULT） TRUE;
}
LRESULT CALLBACK ErrorCallbackProc（HWND hWnd, int nErrID,LPSTR lpErrorText）
{
if （!ghWndCap）
return FALSE;
if （nErrID == 0）
return TRUE;// 清除旧的错误
wsprintf（gachBuffer, "Error# %d", nErrID）; //显示错误标识和文本
MessageBox（hWnd, lpErrorText, gachBuffer,MB_OK | MB_ICONEXCLAMATION）;
return （LRESULT） TRUE;
}
LRESULT CALLBACK FrameCallbackProc（HWND hWnd, LPVIDEOHDR lpVHdr）

{
if （!ghWndCap）
return FALSE;
//假设fp为一打开的.dat文件指针
fwrite（fp,lpVHdr->lpData,lpVHdr->dwBufferLength,1）;
return （LRESULT） TRUE ;
}
值得注意的是：应在.cpp文件中加入#include 一句,在Link设置中加入vfw32.lib。
上述的回调函数FrameCallbackProc是将视频数据直接从缓冲写入文件，也可利用memcpy函数将视频数据直接拷贝到另一缓存。同理，可 定义VideoStreamCallbackProc。capSetCallbackOnVideoStream的使用比 capSetCallbackOnFrame稍微复杂一些。在捕获过程中，当一个新的视频缓冲可得时，系统就调用它所登记的回调函数。在缺省情况下，捕获 窗在捕获过程中不允许其它应用程序继续运行。为了取消这个限制，可以设置CAPTUREPARMS的成员fYield为TRUE或建立一个Yield回调 函数。为了解决潜在的重入（reentry）问题，可在YieldCallbackProc中用PeekMessage过滤掉一些消息，例如鼠标消息。

常用问题代码

Q:利用网上的代码做了一个视频采集的小程序，现在需要实现如下功能   

  已有如下函数：   
  /*******************************************************************************   
        Function       :   EnablePreviewVideo   
        Arguments     :   Parent   (input)   -   Parent   window   that   will   display   video.   
                                  x   (input)   -   X   Location   in   parent   where   video   will   be   shown.   
                                  y   (input)   -   Y   location   in   parent   where   video   will   be   shown.   
                                  Width   (input)   -   Width   of   preview   window.   
                                  Height   (input)   -   Height   of   preview   window.   
                                  PreviewRate   (input)   -   Rate   of   preview   in   FPS.   
        Return           :   TRUE   Success,   FALSE   Failed.   
        Description:   Enables   preview   video   mode.   
  *******************************************************************************/   
  BOOL   CVFWImageProcessor::EnablePreviewVideo(HWND   Parent,   INT   x,   INT   y,   INT   Width,   INT   Height,   INT   PreviewRate)   
  {   
        //   Reset   any   error   conditions.   
        GetPreviousError(NULL,NULL,TRUE);   

        SetParent(m_hWndVideo,Parent);   
        SetWindowLong(m_hWndVideo,GWL_STYLE,WS_CHILD);   

        SetWindowPos(m_hWndVideo,NULL,x,y,   
                                  Width,   
                                  Height,   
                                  SWP_NOZORDER);   
        ShowWindow(m_hWndVideo,SW_SHOW);   
        capPreviewRate(m_hWndVideo,   PreviewRate);   

        return   capPreview(m_hWndVideo,TRUE);   
  }   

  现在已经通过获取预览窗口CRect   rectVideo的大小，但是视频显示的还是与视频格式设置大小一致，只能在窗口内显示视频左上角的一部分，没有实现视频的缩放，所以显示不全。我现在就想缩放视频预览显示到设定区域，但实际的视频大小不要改变，因为抓图还是要实际大小的。   

A:自动缩放视频到视频窗口大小:   
  在你的capPreviewRate(...)之后加上capPreviewScale(hwnd,TRUE)试试   

  附MSDN：   
  BOOL   capPreviewScale(   hwnd,   f   );   

  Parameters   
  hwnd     
  Handle   of   a   capture   window.     
  f     
  Preview   scaling   flag.   Specify   TRUE   for   this   parameter   to   stretch   preview   frames   to   the   size   of   the   capture   window   or   FALSE   to   display   them   at   their   natural   size.     
  Return   Values   
  Returns   TRUE   if   successful   or   FALSE   otherwise.   

  Remarks   
  Scaling   preview   images   controls   the   immediate   presentation   of   captured   frames   within   the   capture   window.   It   has   no   effect   on   the   size   of   the   frames   saved   to   file.   

  Scaling   has   no   effect   when   using   overlay   to   display   video   in   the   frame   buffer.   

  另：   
  抓图时候设置大小：   
  HWND   m_capwnd=capCreateCaptureWindow(.......);   
  BITMAPINFO   m_bmpinfo;   
  capGetVideoFormat(m_capwnd,&m_bmpinfo,sizeof(m_bmpinfo));   

  m_bmpinfo.bmiHeader.biWidth和m_bmpinfo.bmiHeader.biHeight就是你想要的视频宽度和高度   

  设置大小:   
  m_bmpinfo.bmiHeader.biWidth=176;//320   
  m_bmpinfo.bmiHeader.biHeight=144;//240   
  BOOL   ret=capSetVideoFormat(m_capwnd,&m_bmpinfo,sizeof(m_bmpinfo));   



