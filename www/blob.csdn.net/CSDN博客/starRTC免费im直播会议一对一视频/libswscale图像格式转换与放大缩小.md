# libswscale图像格式转换与放大缩小 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月16日 18:40:09[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1095
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
缩放，例如640x360拉伸为1280x720
注意：没有裁剪！
========================
有2种使用方法：
**简单的初始化方法**
（1）       sws_getContext()：使用参数初始化SwsContext结构体。
（2）       sws_scale()：转换一帧图像。
（3）       sws_freeContext()：释放SwsContext结构体。
其中sws_getContext()也可以用sws_getCachedContext()取代。
**更灵活的初始化方法，**可以配置更多的参数：
（1）       sws_alloc_context()：为SwsContext结构体分配内存。
（2）       av_opt_set_XXX()：通过av_opt_set_int()，av_opt_set()…等一系列方法设置SwsContext结构体的值。在这里需要注意，SwsContext结构体的定义看不到，所以不能对其中的成员变量直接进行赋值，必须通过av_opt_set()这类的API才能对其进行赋值。
（3）       sws_init_context()：初始化SwsContext结构体。
这种复杂的方法可以配置一些sws_getContext()配置不了的参数。比如说设置图像的YUV像素的取值范围是JPEG标准（Y、U、V取值范围都是0-255）还是MPEG标准（Y取值范围是16-235，U、V的取值范围是16-240）。
可以通过使用av_opt_set()设置“src_range”和“dst_range”来设置输入和输出的YUV的取值范围。如果“dst_range”字段设置为“1”的话，则代表输出的YUV的取值范围遵循“jpeg”标准；如果“dst_range”字段设置为“0”的话，则代表输出的YUV的取值范围遵循“mpeg”标准。

img_convert_ctx =sws_alloc_context();  
av_opt_set_int(img_convert_ctx,"sws_flags",SWS_BICUBIC|SWS_PRINT_INFO,0);  
 av_opt_set_int(img_convert_ctx,"srcw",src_w,0);  
 av_opt_set_int(img_convert_ctx,"srch",src_h,0);  
 av_opt_set_int(img_convert_ctx,"src_format",src_pixfmt,0);  
av_opt_set_int(img_convert_ctx,"src_range",1,0);  
 av_opt_set_int(img_convert_ctx,"dstw",dst_w,0);  
av_opt_set_int(img_convert_ctx,"dsth",dst_h,0);  
av_opt_set_int(img_convert_ctx,"dst_format",dst_pixfmt,0);  
 av_opt_set_int(img_convert_ctx,"dst_range",1,0);  
sws_init_context(img_convert_ctx,NULL,NULL);  
==============================
算法性能测试：
缩小：
SWS_POINT   每秒钟可缩放约427次     效率之高，让我震撼，但效果却不差。
SWS_FAST_BILINEAR   228次
放大
SWS_POINT  112次，边缘有明显锯齿
SWS_FAST_BILINEAR 103次，效果不错
建议，在不明确是放大还是缩小时，直接使用SWS_FAST_BILINEAR算法即可。
如果明确是要缩小并显示，建议使用Point算法
FFmpeg使用不同sws_scale()缩放算法的命令示例
- ffmpeg -s 480x272 -pix_fmt yuv420p -i src01_480x272.yuv -s 1280x720 -sws_flags bilinear -pix_fmt yuv420p src01_bilinear_1280x720.yuv  
- ffmpeg -s 480x272 -pix_fmt yuv420p -i src01_480x272.yuv -s 1280x720 -sws_flags bicubic -pix_fmt yuv420p src01_bicubic_1280x720.yuv  
- ffmpeg -s 480x272 -pix_fmt yuv420p -i src01_480x272.yuv -s 1280x720 -sws_flags neighbor -pix_fmt yuv420p src01_neighbor_1280x720.yuv  
参考
[http://blog.csdn.net/leixiaohua1020/article/details/42134965](http://blog.csdn.net/leixiaohua1020/article/details/42134965)
 有各种yuv的读取与写入方法
[http://blog.csdn.net/leixiaohua1020/article/details/14215391](http://blog.csdn.net/leixiaohua1020/article/details/14215391)
 有保存各种yuv和rgb的方法
[**算法性能测试**](http://blog.csdn.net/leixiaohua1020/article/details/12029505)
