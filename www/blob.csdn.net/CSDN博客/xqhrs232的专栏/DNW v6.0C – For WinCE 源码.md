# DNW v6.0C – For WinCE 源码 - xqhrs232的专栏 - CSDN博客
2013年05月03日 11:00:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：986
原文地址::[http://www.amobbs.com/forum.php?mod=viewthread&tid=5310114](http://www.amobbs.com/forum.php?mod=viewthread&tid=5310114)
其实 本帖并不是真的“DNW v6.0C – For WinCE 源码”，网上能找到的都是“dnw050A.zip”这个050A版本的PC端源码，可惜只支持之前的USB1.1全速。
这里记录一下，如何让dnw050A的源码能用上USB2.0的高速模式。“dnw050A.zip”源码中的guid.h文件有如下：
DEFINE_GUID(GUID_CLASS_I82930_BULK, 
0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8);
在帖子http://hi.baidu.com/fengniu88/blog/item/650b97d1c49dc33f970a16c1.html
处，看到“根据设备GUID，枚举所有USB　HOST，找匹配的USB设备，然后获取其路径”这样一句话，估计就是这段字符串来控制USB类型的了，姑且尝试一下。
        可是我们并不知道USB2.0的GUID。但既然是字符串，那在编译出来的驱中必定将这段字符串存放在文字池部分。
        先用在secbulk.sys（usb1.1的驱动）中用winhex打开，并以“8e”（0x8e120c45）为关键字查找，
![](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708301LGB1RR.png)
(原文件名:1.1.png)
可以看到红色画线部分与上面的DEFINE_GUID中的内容一致，并且，这里估计，前面6个字节的0和粉色画线的2个字节可以作为识别字段特征。
        在winhex中打开secusb2.sys（usb2.0的驱动），尝试用0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8中的每个字节作为关键字查找，看看是不是有上面估计的那种字段特征，运气比较好，在查到0x49的时候就有了收获：
![](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708302KNCXPV.png)
(原文件名:2.0.png)
将guid.h中的
DEFINE_GUID(GUID_CLASS_I82930_BULK, 
0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8);
改为
//DEFINE_GUID(GUID_CLASS_I82930_BULK, 
//0x8e120c45, 0x4968, 0x4188, 0xba, 0x19, 0x9a, 0x82, 0x36, 0x1c, 0x8f, 0xa8);
DEFINE_GUID(GUID_CLASS_I82930_BULK, 
0x8715c3bf, 0x4949, 0x4260, 0x90, 0xf1, 0xa2, 0xaa, 0x1d, 0xeb, 0xfb, 0x90);
编译MFC工程，发现可用。
至此，源码在手，想怎么改就怎么改咯~~  修改uboot，支持USB自动下载什么的~~~
另附上网上找到的几份资料。
1、win2kddk.exe 需要的头文件什么的都在里面    点击此处下载 [ourdev_708308M4XJCI.rar(文件大小:65.71M)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708308M4XJCI.rar)(原文件名:win2kddk.rar)
2、drivers.rar里面内容为：    点击此处下载 [ourdev_708306EZHN6P.rar(文件大小:14K)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708306EZHN6P.rar)(原文件名:drivers.rar)
     secbulk.inf
     secbulk.sys
     secusb2.inf
     secusb2.sys
3、compile.doc网上某网友的编译dnw的笔记，很好，不过找不到出处了，在此谢过！    点击此处下载 [ourdev_708303DOWIGV.doc(文件大小:22K)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708303DOWIGV.doc)(原文件名:compile.doc)
4、guid.h唯一需要修改的文件    点击此处下载 [ourdev_708305IBP5RX.rar(文件大小:583字节)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708305IBP5RX.rar)(原文件名:guid.rar)
5、dnw操作流程.docx这个是小分析笔记    点击此处下载 [ourdev_708304CMQGEP.doc(文件大小:30K)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708304CMQGEP.doc)(原文件名:dnw操作流程.doc)
6、dnw050A.zip源码在这里    点击此处下载 [ourdev_708307CA9C47.zip(文件大小:3.83M)](http://cache.ourdev.cn/bbs_upload782111/files_49/ourdev_708307CA9C47.zip)(原文件名:dnw050A.zip)

