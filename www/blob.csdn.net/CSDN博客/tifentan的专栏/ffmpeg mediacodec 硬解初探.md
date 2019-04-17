# ffmpeg mediacodec 硬解初探 - tifentan的专栏 - CSDN博客





2018年06月07日 10:29:41[露蛇](https://me.csdn.net/tifentan)阅读数：634








ffmpeg mediacodec硬解初探 

1编译： 

ffmpeg自3.1版本加入了android mediacodec硬解支持，解码器如图
硬件加速器如图（还不清楚硬件加速器的功能）

编译带h264_mediacodec版本的库需要开启以下选项 

–enable-jni 

–enable-mediacodec 

–enable-decoder=h264_mediacodec 

–enable-hwaccel=h264_mediacodec（不知道有什么用，还是开了）
2使用： 

首先在JNI_OnLoad函数，或者使用解码器之前调用 

av_jni_set_java_vm(vm, NULL);（位于libavcodec/jni.h） 

来设置java虚拟机（反调mediacodec时会用到）
其次，由于h264_mediacodec解码器和h264解码器id相同所以，软解时，通过 

avcodec_find_decoder（id）来寻找解码器 

而想使用mediacodec硬解时，使用 

avcodec_find_decoder_by_name(“h264_mediacodec”);寻找指定解码器
之后所有步骤和软解完全相同，不需要更改任何内容

3暂时的现象和问题： 

现象：不知道是否和播放器本身架构设计和实现有关，对比h264软解和硬解，cpu使用率并没有明显下降，多数下相差不大，有一部分视频能出现5%-10%的下降 

但较为明显提升的是解码速度，解码640X360分辨率时，已有明显区别（左图软解，右图硬解，数字为每一帧解码耗费时间，单位毫秒，左右两图时间并非一一对应，仅比较量级） 

视频为[http://ips.ifeng.com/video19.ifeng.com/video09/2015/02/28/3019136-102-008-1929.mp4](http://ips.ifeng.com/video19.ifeng.com/video09/2015/02/28/3019136-102-008-1929.mp4)
解码1920X1080分辨率时，区别很大（左图软解，右图硬解，数字为每一帧解码耗费时间，单位毫秒，左右两图时间并非一一对应，仅比较量级） 

视频为[http://gitdown.bravovcloud.com/video/3.mp4](http://gitdown.bravovcloud.com/video/3.mp4)

问题：软解出来的数据格式为YUV420P，直接使用libyuv转换即可；在我这台手机上硬解出来的数据格式为NV12 

据网上博客所说，不同机器解码出来的数据格式不尽相同，这就导致解码出来后的数据，需要根据其类型来做转化，使用ffmpeg自带的转换函数非常简单通用，但是效率有待考察。

libyuv中包含neon加速，但是不包含通用转换函数，需要自行判断类型，而且还不包含ToRGBA的函数，主要包含ToARGB的函数，native 

 window显示时，只支持RGBA8888，RGBX8888和RGB656，这样会在转换上增加了一些复杂度

[https://www.polarxiong.com/archives/Android-MediaCodec%E8%A7%86%E9%A2%91%E6%96%87%E4%BB%B6%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81-%E9%AB%98%E6%95%88%E7%8E%87%E5%BE%97%E5%88%B0YUV%E6%A0%BC%E5%BC%8F%E5%B8%A7-%E5%BF%AB%E9%80%9F%E4%BF%9D%E5%AD%98JPEG%E5%9B%BE%E7%89%87-%E4%B8%8D%E4%BD%BF%E7%94%A8OpenGL.html](https://www.polarxiong.com/archives/Android-MediaCodec%E8%A7%86%E9%A2%91%E6%96%87%E4%BB%B6%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81-%E9%AB%98%E6%95%88%E7%8E%87%E5%BE%97%E5%88%B0YUV%E6%A0%BC%E5%BC%8F%E5%B8%A7-%E5%BF%AB%E9%80%9F%E4%BF%9D%E5%AD%98JPEG%E5%9B%BE%E7%89%87-%E4%B8%8D%E4%BD%BF%E7%94%A8OpenGL.html)

这篇博客讲述了android在api21之后，硬解指定格式的方法 

附： 

在libavcodec中的 mediacodec_common.c里面我找到了这个
注意ffmpeg里面也有jni.h,需要引入avcodec时加上路径，然后mk文件里面不引入到avcodec这一级目录。

感谢同事刘衍的调研









