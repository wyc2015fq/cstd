# 移植ffplay播放器到Windows，可以用VC编译 - 爱写代码的自由人 - CSDN博客





2018年03月03日 12:09:37[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：271








   FFmpeg官方实现了一个很简单小巧的播放器---ffplay，这个播放器调用了自家的解码器来解码，用开源的SDL库来渲染视频和播放音频。ffplay实现了一个很好的播放器框架，非常适合开发员用来学习如何开发播放器。但是ffplay只能用GCC编译，对于VC的程序员在Windows系统上调试不大方便。所以，我就对它做了移植，能直接通过VC编译和调试。工程下载地址：

   http://download.csdn.net/download/zhoubotong2012/10242438



