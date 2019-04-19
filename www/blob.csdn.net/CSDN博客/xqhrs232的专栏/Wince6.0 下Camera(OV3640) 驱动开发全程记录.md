# Wince6.0 下Camera(OV3640) 驱动开发全程记录 - xqhrs232的专栏 - CSDN博客
2015年01月05日 11:40:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：939
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/figthter_cui/article/details/6007100](http://blog.csdn.net/figthter_cui/article/details/6007100)
相关文章
1、[Initialization Sequence for Camera Drivers of CE6.0](http://blog.csdn.net/figthter_cui/article/details/6002618)----[http://blog.csdn.net/figthter_cui/article/details/6002618](http://blog.csdn.net/figthter_cui/article/details/6002618)
2、[如何解决多次打开CameraDshowApp.exe系统死机的问题（高分辨率camera）](http://blog.csdn.net/figthter_cui/article/details/6021702)----[http://blog.csdn.net/figthter_cui/article/details/6021702](http://blog.csdn.net/figthter_cui/article/details/6021702)
3、[Wince6.0 下Camera(OV3640) 驱动开发笔记](http://blog.csdn.net/zhengmeifu/article/details/7982524)-----[http://blog.csdn.net/zhengmeifu/article/details/7982524](http://blog.csdn.net/zhengmeifu/article/details/7982524)
4、TE6410 + OV3640的问题----[http://bbs.csdn.net/topics/370186119](http://bbs.csdn.net/topics/370186119)
   从开始搞camera驱动到正式搞好经历了差不多一个月的时间，现在回头来看发现走了很多弯路，耗费了不必要的时间和精力，也许这是菜鸟必经的过程吧。周末没事把调camera驱动的整个过程总结一下，以免以后忘了对不起那一个月的时间和精力，同时也给正在调camera驱动的朋友一点参考。
一.camera硬件调试
     我采用的是camera是300万像素的OV3640，处理器为S5PC100，接口电路如下图所示：
![](http://hi.csdn.net/attachment/201011/13/0_12896317917Zov.gif)
       第一步要干什么呢，当然是要先确保硬件没有了问题了。从上图可以看出OV3640是IIC接口的，也就是它的初始化和各种功能控制都是通过IIC接口写入对应的命令来实现的，因此首要任务是确保IIC驱动能正确读写。我刚开始就卡在这里，三星给的BSP里的IIC1驱动有BUG，我的camera就是连到IIC1了。IIC1驱动的读和写都有问题，而且板子上没有类似EEPRAM等具有IIC接口可读写的芯片，所以验证IIC1成了一个问题。因此在调试IIC1驱动阶段耗费了不少时间。
       怎么确定你的IIC已经能正常工作了？通过读OV3640的ID，ID能正确读出了就说明你CAMAERA的电源接口没有问题，IIC没有问题。在此要提醒一点，OV3640的PWDN在正常工作模式要拉低，否则不能正常工作，从上面的原理图可以看出我搞错了，后来只能跳线解决。
接下来需要验证的是XCLK,也就是cpu输出的时钟信号。从3640的文档上可以知道，需要的标准时钟是24M，通过示波器来验证你的cpu输出的XCLK是否是24M。
        当上述工作都得到了确定，下面就要开始初始化3640了。初始化文件可以向FEA要，自己写就比较纠结了。也许在这里会有些疑问，在哪里对camera进行初始化操作呢？由于目前只是来验证硬件接口是否正常，没有必要一定要在camera驱动程序里进行初始化，可以在任何一个驱动中调用IIC驱动对camera进行初始化。假设你的IIC读写驱动已经正确了，那么此时camera就会数据输出，帧同步，行同步和像素时钟也都会有规律的波形输出。如果没有，那么就要回头检测IIC驱动了。
二.camera驱动分析
     wince6.0 下的camera驱动结构是基于MDD/PDD的分层结构，整体结构如下图所示：
![](http://hi.csdn.net/attachment/201011/13/0_1289634859JVPJ.gif)
   应用程序是基于DirectShow框架的，这一部分这里暂时不谈，到后面再说。这里要分析的重点是camera驱动的MDD/PDD结构，其详细结构图为：
![](http://hi.csdn.net/attachment/201011/13/0_128963535978a9.gif)
        由上图可知，MDD主要分为两大部分CAM（camdriver,camdevice）与PIN（pindriver,pindevice）,其中camdriver与pindriver是流接口；camdevice与pindevice是PDD层函数的封装。具体的函数调用顺序为：
![](http://hi.csdn.net/attachment/201011/13/0_1289635901J16p.gif)
   三.实现camera驱动
   实现camera驱动有两种方法，一种是利用微软提供的camera驱动框架实现，框架代码位于:/WINCE600/PUBLIC/COMMON/OAK/DRIVERS/CAPTURE/CAMERA。这部分代码这是一个框架，没有具体的硬件相关。这种方法难度很难大，一般只有那些OEM厂家才会采用此方法。另一种相对比较简单的方法就是在原有正确的camera驱动上改，我用的三星BSP中有这样的camera驱动。看过camera驱动的人都知道每一种不同的camera被抽象为一个类，因此首先要抽象为你特定类型camera的类。当然这部分也不用从头写，也可以在其他camera模板上改。需要改动主要有以下几处：
1.camera参数宏定义
//========================================================
//  S5K4EAGX default mode
#define S5K4EAGX_MODULE_ITUXXX         CAM_ITU601
#define S5K4EAGX_MODULE_MIPI           (TRUE)
#define S5K4EAGX_MODULE_LANE           (DATA_LANE_1)
#define S5K4EAGX_MODULE_JPEG           (1)
#define S5K4EAGX_MODULE_YUVORDER       CAM_ORDER_CRYCBY
#define S5K4EAGX_MODULE_PREVIEWHSIZE   640
#define S5K4EAGX_MODULE_PREVIEWVSIZE   480
#define S5K4EAGX_MODULE_HSIZE          640
#define S5K4EAGX_MODULE_VSIZE          480
#define S5K4EAGX_MODULE_HOFFSET        0
#define S5K4EAGX_MODULE_VOFFSET        0
#define S5K4EAGX_MODULE_UVOFFSET       CAM_UVOFFSET_0
#define S5K4EAGX_MODULE_CLOCK          24000000
#define S5K4EAGX_MODULE_CODEC          CAM_FORMAT_YCBCR422_1PLANE
#define S5K4EAGX_MODULE_HIGHRST        0        // This is affected by Electic circuit for reset or power control PIN, BB37->0, SMDK->1
#define S5K4EAGX_MODULE_INVPCLK        0
#define S5K4EAGX_MODULE_INVVSYNC       0
#define S5K4EAGX_MODULE_INVHREF        0
//========================================================
这里就要根据你实际采用的摄像头来设置了，提醒一点的是同步时钟的极性要与cpu需要的相匹配。
2.camera初始化函数:InitSensor()
   摄像头的初始化就是在这个函数里完成的，根据你的camera进行改动。
3.camera电源管理函数Power(BOOL bOnOff)
   在这个函数里需要完成打开camera电源或关闭电源的操作，根据具体硬件改写。
4.CCameraPdd::GetVideoFormatList( DWORD dwSensorID )
   该函数比较重要，应用程序就是通过此函数来得到camera支持何种数据数据的信息的，如下所示：
    case SYSLSI_S5K4EAGX:
        // Video Format initialization
        i = 0;
        m_pModeVideoFormat[CAPTURE].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_YV12_640x480_30;
        m_pModeVideoFormat[CAPTURE].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_YV12_1280x720_30;
        m_pModeVideoFormat[CAPTURE].ulAvailFormats          = i;
        // Still Format initialization
        i = 0;
        // With S5K4EAGX Sensor module, IJPG is used for Still shot, and there is no test kit.
        // The user should dump the camera still output pin buffer to check frame data
        if(bForCETK)
        {
            // CETK can test only RGB format.
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_RGB565_640x480_30;
        }
        else
        {
            // Use Default Format
            // YV format also can be used. actually it shows faster performance than RGB format.
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_640x480_30;
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_800x480_30;
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_1280x960_30;
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_1600x1200_30;
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_2048x1536_30;
            m_pModeVideoFormat[STILL].pCsDataRangeVideo[i++]    = &DCAM_StreamMode_IJPG_2592x1944_30;        
        }
        m_pModeVideoFormat[STILL].ulAvailFormats            = i;
        // if preview pins supports
        if( MAX_SUPPORTED_PINS == m_ulCTypes )
        {
            i = 0;
            if(bForCETK)
            {
                // CETK can test only RGB format.
                m_pModeVideoFormat[PREVIEW].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_RGB565_640x480_30;
                m_pModeVideoFormat[PREVIEW].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_RGB565_800x480_30;            
            }
            else
            {
                // YV format also can be used. actually it shows faster performance than RGB format.
                m_pModeVideoFormat[PREVIEW].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_YV12_640x480_30;
                m_pModeVideoFormat[PREVIEW].pCsDataRangeVideo[i++]  = &DCAM_StreamMode_YV12_1280x720_30;            
            }
            m_pModeVideoFormat[PREVIEW].ulAvailFormats          = i;
        }
        break;
     这个就有根据你camera实际输出数据的格式改写了，对应的宏定义（如DCAM_StreamMode_YV12_640x480_30）在sensorformats.h（位于WINCE600/PLATFORM/SMDKC100/SRC/DRIVERS/CAMERAFILTER/CAMERA_PDD），如果其中没有与你camera输出格式匹配的格式，要自己按照它的格式添加上。
      到此，驱动部分基本完成。调用CameraDshowApp.exe测试应该可以看到图像了。今天先写到这里，应用部分改天再写。

