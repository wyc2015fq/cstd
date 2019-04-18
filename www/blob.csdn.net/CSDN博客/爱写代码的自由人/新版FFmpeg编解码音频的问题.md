# 新版FFmpeg编解码音频的问题 - 爱写代码的自由人 - CSDN博客





2018年02月22日 11:32:11[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：692








      很多人写的老程序还是用FFmpeg的旧版本，但一升级到新版本就遇到很多问题，首先是某些API不能用了，编译不通过，不过这个问题还是算比较容易解决的，遇到最头疼的一个问题是对音频的处理。以前FFmpeg编码和解码接受的音频格式多数是S16（AV_SAMPLE_FMT_S16），是Packed Format格式，就是左右两个声道是连续排列的，而FFmpeg新版本引进了新的排列格式AV_SAMPLE_FMT_S16P，AV_SAMPLE_FMT_FLTP，这些是Plane Format，左右声道是分开两个平面存储的（跟图像将Y，U，V分量分开三个平面存储的方式相似），这样就使处理流程变得复杂了，使原来的代码要改很多东西。不过，好在网上有些前辈提出了相应的解决方案，下面是讨论解决这个问题的一个帖子：

    http://bbs.csdn.net/topics/390700255




