# _WIN32_WINNT not defined. Defaulting to _WIN32_WINNT_MAXVER (see WinSDKVer.h) - xqhrs232的专栏 - CSDN博客
2013年12月13日 18:02:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1240
原文地址::[http://blog.csdn.net/xiaolongwang2010/article/details/7550505](http://blog.csdn.net/xiaolongwang2010/article/details/7550505)
相关网帖
1、错误找不到WinsdkVer.h----[http://bbs.csdn.net/topics/380128304?page=1#post-396334704](http://bbs.csdn.net/topics/380128304?page=1#post-396334704)
警告报告：
_WIN32_WINNT not defined. Defaulting to _WIN32_WINNT_MAXVER (see WinSDKVer.h）
**处理办法：**
**第一步：**
" #define _WIN32_WINNT 0x0502 "
在StdAfx.h中添加上述宏定义。
备注：必须在stdafx.h中所有#include 文件之前添加此代码。
**第二步：**
在StdAfx.h头文件里面加上#include <SDKDDKVer.h>有关于平台的定义
备注：如果StdAfx.h里面有了就不用加 #include <SDKDDKVer.h>了！
 //Including SDKDDKVer.h defines the highest available Windows platform.
 //If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
 //set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
全文参考：[http://blog.sina.com.cn/s/blog_8d7f7f3c01010s3g.html](http://blog.sina.com.cn/s/blog_8d7f7f3c01010s3g.html)
[http://blog.csdn.net/ayan200112/article/details/6918975](http://blog.csdn.net/ayan200112/article/details/6918975)
//==============================================================================================
备注::
1》WinSDKVer.h+SDKDDKVer.h文件在PC上的存储路径为----C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include
