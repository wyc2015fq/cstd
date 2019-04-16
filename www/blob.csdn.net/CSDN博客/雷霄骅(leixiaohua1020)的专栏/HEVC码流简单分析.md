# HEVC码流简单分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 13:03:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：48









初步分析了一下HEVC的码流。

使用了Elecard HEVC Analyzer打开了一个《阿凡达>720P的视频片段

P帧得到如下结果：

宏块划分

![](https://img-blog.csdn.net/20130920125516906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

运动矢量：

![](https://img-blog.csdn.net/20130920125531656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

CU的大小：

![](https://img-blog.csdn.net/20130920125633953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

QP：

![](https://img-blog.csdn.net/20130920125704484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：分析过程中发现QP值是恒定的

B帧得到如下结果：

宏块划分：

![](https://img-blog.csdn.net/20130920125828000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

运动矢量：

![](https://img-blog.csdn.net/20130920125859000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



自己上传了一个Elecard HEVC Analyzer：[http://download.csdn.net/detail/leixiaohua1020/6371925](http://download.csdn.net/detail/leixiaohua1020/6371925)





========补充==============

HEVC测试序列合集：[http://download.csdn.net/detail/leixiaohua1020/6843803](http://download.csdn.net/detail/leixiaohua1020/6843803)






