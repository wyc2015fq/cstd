# [转载]使用EasyRTMP推送RTMP流flash播放器无法正常播放的问题解决 - xcyl的口袋 - CSDN博客
2016年10月29日 14:08:16[xcyl](https://me.csdn.net/cai6811376)阅读数：811
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52960750](http://blog.csdn.net/jinlong0603/article/details/52960750)
## 问题简介
> 
EasyRTMP是EasyDarwin团队开发的一套简单易用的RTMP推送SDK。本文想讲述下开发过程中遇到的一个问题。问题的现象是使用EasyRTMP推送音视频流到自己搭建的nginx-rtmp服务器，使用ffplay和vlc播放器都能够正常播放，但是使用网页flash播放时，总是会卡住，无法正常播放。
## 问题解决
> 
开始时有些疑惑，因为ffplay在播放时都没有提示任务警告等异常信息，尝试了多个网页在线播放都是一样的问题，认为问题肯定是在RTMP推送端。 
  首先想到的是屏蔽掉音频，只推送视频数据，再进行测试。结果让我庆幸，屏蔽掉音频后，果然一切正常。问题就出在音频上，于是开始对AAC相关的部分进行一系列的排查。 
  网上搜索了一堆资料，分析了一开始的AAC header，以及ADTS头部数据，都是正常的。具体分析过程可网上搜索，博主参考了[http://niulei20012001.blog.163.com/blog/static/7514721120130694144813/](http://niulei20012001.blog.163.com/blog/static/7514721120130694144813/)这篇。本人中提到需要去掉ADTS头部再进行推送，于是又查了些资料，果然在发送过AAC header之后，后面的AAC数据帧中需求去掉ADTS头部。
于是加上了个代码判断上层传下来的数据是否已经包含ADTS，如果包含就去掉。
```
if (((data[0] & 0xFF) == 0xFF) && ((data[1] & 0xF0) == 0xF0) && (size > 7))
{
    data += 7;
    size -= 7;
}
```
> 
去掉ADTS头部后再次同时推送音视频，果然所有播放器都可以正常播放，声音也正常。
至此可以确定RTMP推送AAC数据时是需要去掉ADTS头部的。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
