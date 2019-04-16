# BREW 应用中的SVG技术 - 我相信...... - CSDN博客





2010年10月20日 10:36:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：966








SVG是二维的向量图像格式，是由W3C和3GPP的提出的标准，实现了内置的交互性，支持跨越显示环境的缩放，文件尺寸比BMP, JPEG,GIF等格式还要小。SVG的编辑工具非常简单易用，基于XML的格式从而容易创建和修改，支持富文本选项。

在BREW 3.1.4开始，IMediaSVG 接口开始支持SVG的播放，暂停，终止，快进和快退等，同时支持按键，笔操作，旋转和缩放等操作。

       希望使用SVG的时候，首先需要使用一些商用的图像编辑工具来创建SVG文件，例如 Beatware Mobile Designer 2.0™， Beatware e-PicturePro 5.0™，Ikivo Animator 2.0 ™，Adobe® Illustrator CS2®，Adobe® GoLive CS2®和Open source Inkscape 0.44.1等等。

       然后，生成BREW中需要的相关内容，例如：

<!-- Button -->

<g id = "Button_Back2" nav-prev = "url(#Button_GetNew)" navnext = "url(#Button_Options5)" focusable="false" >

其中定义了 ‘nav-prev’和 ‘nav-next’两个元素等等，来保证导航功能。一般的情况下，需要禁止聚焦功能，例如<g focusable="false" id="Animation_38">，并且需要使用gzip压缩目标文件。

在SVG的内容生成之后，一般使用web浏览就可以了解SVG的基本显示，并进行一下交互性测试。但需要注意的是，不同浏览器对SVG的支持程度也不相同。SVG中的XML 文档结构可以通过Opera 浏览器来验证，而且该浏览器还能够报告出错误的行号。在安装了SVG的扩展类之后，BREW模拟器就能够播放SVG了。

在BREW应用中，首先要获得IMediaSVG接口：

ISHELL_CreateInstance(pme->m_pIShell,AEECLSID_MEDIASVG,(void**)pme->m_pIMediaSVG);

其次，确定需要播放的文件内容：

AEEMediaData mData;

mData.clsData = MMD_FILE_NAME;

mData.pData = svg_filename;

mData.dwSize = 0;

IMEDIA_SetMediaData(pMe->pIMediaSVG, &mData);

然后，注册回调函数：

IMEDIA_RegisterNotify(pMe->pIMediaSVG, MediaNotifyCB, pMe);

最后，就可以使用IMEDIA_Play(pMe->pIMediaSVG)来播放文件了。其中回调函数MediaNotifyCB负责获取SVG文件中的一个帧，并显示在屏幕上。

void MediaNotifyCB(void * pUser, AEEMediaCmdNotify * pCmdNotify){

APP_CLASS * pMe = (APP_CLASS*) pUser;

switch(pCmdNotify->nStatus)

{

case MM_STATUS_FRAME:

{

AEEDeviceInfo di;

IBitmap* frameBuf;

IMEDIA_GetFrame(pMe->pIMediaSVG, &frameBuf);

ISHELL_GetDeviceInfo(pMe->a.m_pIShell, &di);

IDISPLAY_BitBlt(pme->m_pIDisplay,0,0,di.cxScreen,di.cyScreen, frameBuf,0,O,AEE_RO_C0PY );

IDISPLAY_UpdateEx(pMe->a.m_pIDisplay, FALSE);

break;

}

}

return;

}

对于SVG中的交互事件，在应用中要做相应的消息处理：

case EVT_KEY:

switch(wParam)

{

case AVK_DOWN:

nErr=IMEDIASVG_Focus(pme->pIMediaSVG,MM_SVG_FOCUS_EVENT_NEXT);

break;

case AVK_UP:

nErr=IMEDIASVG_Focus(pme->pIMediaSVG,MM_SVG_FOCUS_EVENT_PREVIOUS);

break;

case AVK_SELECT:

nErr=IMEDIASVG_Focus(pme->pIMediaSVG,MM_SVG_FOCUS_EVENT_ACTIVATE);

break;

case AVK_END:

nErr = IMEDIA_Stop(pMe->pIMediaSVG);

break;

}

在手机上测试支持SVG应用的时候，要注意手机可能不支持SVG的压缩状态。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/07/21/5751903.aspx](http://blog.csdn.net/wireless_com/archive/2010/07/21/5751903.aspx)



