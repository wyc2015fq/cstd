# STM32 VCP PC端安装驱动失败的问题解决----试了解决了问题 - xqhrs232的专栏 - CSDN博客
2018年01月27日 18:11:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2717
原文地址::[http://blog.csdn.net/beyoungbehappy/article/details/53784911](http://blog.csdn.net/beyoungbehappy/article/details/53784911)
相关文章
1、VCP_V1.4.0_Setup.exe安装后，虚拟串口还是感叹号----[http://www.stmcu.org/module/forum/thread-604312-1-1.html](http://www.stmcu.org/module/forum/thread-604312-1-1.html)
2、STM32_USB_VCP驱动问题 （2）----[http://blog.csdn.net/qq_17622851/article/details/52523841](http://blog.csdn.net/qq_17622851/article/details/52523841)
STM32，使用USB　VCP功能，即虚拟串口功能时，选择USB＿OTG_FS Device_Only，如上图。cube自动生成工程，不作修改。将USB插入电脑，WIN10或WIN7系统设备管理器中端口项中都能成功出现STMicroelecronics Virtual COM Port(COM4)，但驱动前显示黄色叹号，表示设备安装存在问题的警告。
问题：
usbd_cdc.c文件中，USBD_CDC_Init函数中存在一个USBD_malloc，可在此打断点调试，pdev->pClassData为NULL。可知malloc并没有申请到空间。
解决方法：
malloc没有申请到空间，并不是USBD_malloc函数有问题，而是系统设置堆空间过小，申请空间大于堆空间，所以无法申请成功。只需要将startup_stm32f429xx.s系统启动文件里的Heap_Size      EQU     0x200 值高大就可以了
//======================================================================
备注::
1>在win10下运行C:\Program Files (x86)\STMicroelectronics\Software\Virtual comport driver\Win8下的dpinst_amd64.exe就可以把驱动安装成功-----显示的串口也就不带感叹号了!!!
