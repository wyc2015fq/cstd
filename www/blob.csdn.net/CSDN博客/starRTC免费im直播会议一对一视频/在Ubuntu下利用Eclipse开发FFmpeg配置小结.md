# 在Ubuntu下利用Eclipse开发FFmpeg配置小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月14日 13:44:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：211
首先需要编译FFmpeg得到头文件和lib文件，参见：[在Ubuntu下编译FFmpeg](http://172.30.24.252/wiki_elesos_com/index.php?title=%E5%9C%A8Ubuntu%E4%B8%8B%E7%BC%96%E8%AF%91FFmpeg&action=edit&redlink=1)
选择File-New-CProject
选择Executable下的Empty Project,右侧选择Linux GCC
选择Project-属性，选择c/c++Build下的Settings.选择GCC C Linker下的Libraries，
在右侧Libraries下添加avformat，avcodec和avutil等库。
