# ffplay的快捷键以及选项 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月11日 00:03:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：26
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










ffplay是ffmpeg工程中提供的播放器，功能相当的强大，凡是ffmpeg支持的视音频格式它基本上都支持。甚至连VLC不支持的一些流媒体都可以播放（比如说RTMP），但是它的缺点是其不是图形化界面的，必须通过键盘来操作。因此本文介绍一下它的快捷键以及选项。



**快捷键**

播放视音频文件的时候，可以通过下列按键控制视音频的播放
|按键|作用|
|----|----|
|q, ESC|退出|
|f|全屏|
|p, 空格|暂停|
|w|显示音频波形|
|s|逐帧显示|
|左方向键/右方向键|向后10s/向前10s|
|上方向键/下方向键|向后1min/向前1min|
|page down/page up|向后10min/向前10min|
|鼠标点击屏幕|跳转到指定位置（根据鼠标位置相对屏幕的宽度计算）|
|||



**选项**

在播放视频前，可以预设一些参数。

一般播放视频的时候，使用命令：


`ffplay "林俊杰_江南.flv"`


如果我们希望能在播放完成后自动退出，则可以使用命令：


`ffplay -autoexit "林俊杰_江南.flv";`


所有的命令如下列表所示。
|名称|有参数|作用|
|----|----|----|
|x|Y|强制屏幕宽度|
|y|Y|强制屏幕高度|
|s|Y|强制屏幕大小|
|fs|N|全屏|
|an|N|关闭音频|
|vn|N|关闭视频|
|ast|Y|设置想播放的音频流（需要指定流ID）|
|vst|Y|设置想播放的视频流（需要指定流ID）|
|sst|Y|设置想播放的字幕流（需要指定流ID）|
|ss|Y|从指定位置开始播放，单位是秒|
|t|Y|播放指定时长的视频|
|nodisp|N|无显示屏幕|
|f|Y|强制封装格式|
|pix_fmt|Y|指定像素格式|
|stats|N|显示统计信息|
|idct|Y|IDCT算法|
|ec|Y|错误隐藏方法|
|sync|Y|视音频同步方式（type=audio/video/ext）|
|autoexit|N|播放完成自动退出|
|exitonkeydown|N|按下按键退出|
|exitonmousedown|N|按下鼠标退出|
|loop|Y|指定循环次数|
|framedrop|N|CPU不够的时候丢帧|
|window_title|Y|显示窗口的标题|
|rdftspeed|Y|Rdft速度|
|showmode|Y|显示方式(0 = video, 1 = waves, 2 = RDFT)|
|codec|Y|强制解码器|




