# Wince系统时间设置 - xqhrs232的专栏 - CSDN博客
2014年05月26日 15:34:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：808
原文地址::[http://blog.csdn.net/xuefeng_baggio/article/details/5494206](http://blog.csdn.net/xuefeng_baggio/article/details/5494206)
相关文章
1、WINCE 系统时间的掉电保存----[http://blog.sina.com.cn/s/blog_5fe796ce0101i1n7.html](http://blog.sina.com.cn/s/blog_5fe796ce0101i1n7.html)
本来以为系统时间设置很简单，只要修改下RTC会记得你设置的时间，但是经过测试发现情况不是这样的。于是研究了bsp启动时系统时间的设置的流程。我一直认为在OAL层加入OEMSetRealTime()函数即可，但是测试了好几次不行，于是动了真格，呵呵之前一直以为这个功能很简单。在OEMInit加入相应的调试信息后，发现在我OEMSetRealTime之后，wince调用了一个函数OALIoCtlHalInitRTC()这个函数太关键了。于是看了这个函数介绍英文，看的够呛，不过意思都懂了，This
 function is called by WinCE OS to initialize the time after boot.
//  Input buffer contains SYSTEMTIME structure with default time value.
//  If hardware has persistent real time clock it will ignore this value
啥意思呀？？也就是说这个函数是wince OS调用的，那什么时候调用的呢？通过调试信息发现是在调用OEMIoControl时开始这个函数的调用的。
  在bsp中有个oal_ioctl_tab.h文件，其中有个{ IOCTL_HAL_INIT_RTC,                       0,  OALIoCtlHalInitRTC          },定义，一切都明白了。但是很奇怪，为什么调用了这个OALIoCtlHalInitRTC后系统时间有变成默认的呢？？分析这个函数吧？
BOOL OALIoCtlHalInitRTC(
    UINT32 code, VOID *pInpBuffer, UINT32 inpSize, VOID *pOutBuffer,
    UINT32 outSize, UINT32 *pOutSize
) {
    BOOL rc = FALSE;
    SYSTEMTIME *pTime = (SYSTEMTIME*)pInpBuffer;
    OALMSG(1, (L"+OALIoCtlHalInitRTC(...)/r/n"));
    // Validate inputs
    if (pInpBuffer == NULL || inpSize < sizeof(SYSTEMTIME)) {
        NKSetLastError(ERROR_INVALID_PARAMETER);
        OALMSG(OAL_ERROR, (
            L"ERROR: OALIoCtlHalInitRTC: Invalid parameter/r/n"
        ));
        goto cleanUp;
    }
    // Add static mapping for RTC alarm
    OALIntrStaticTranslate(SYSINTR_RTC_ALARM, IRQ_RTC);
// Set time fxf 20100416
  //  rc = OEMSetRealTime(pTime);
    rc = OEMGetRealTime(pTime);
cleanUp:
    OALMSG(OAL_IOCTL&&OAL_FUNC, (L"-OALIoCtlHalInitRTC(rc = %d)/r/n", rc));
    return rc;
}
这个函数大部分代码都是废话，也就是没有作为的代码。当我看到  rc = OEMSetRealTime(pTime);时我崩溃了。为什么呢？我先前在OEMInit又被这句话设置成默认的了，于是改成 rc = OEMGetRealTime(pTime);这样在调用OALIoCtlHalInitRTC时只要get一个就可以了。呵呵搞定了！小样！
