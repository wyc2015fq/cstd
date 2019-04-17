# ffmpeg编码h264只包含I帧P帧的方法 - DoubleLi - 博客园






ffmpeg使用avcodc_encode_video编码，默认产生的h264包含B帧，在安防行业很多地方是不需要用到B帧的。



1.基础知识充电

这就涉及到h264的各种profile格式了，参考

[h264各种profile](http://blog.csdn.net/qiaoliang328/article/details/10153313)

h264分4种等级（profile）：Baseline、Main、Extended、High

Baseline profile:支持I帧P帧

Main profile:支持I/P/B/SP/SI帧

Extended profile:支持I/P/B帧

High profile:。。。。。。



2.实战验证

将h264写成文件，使用H264Visa播放，在SummImfo-》Stream信息中，确实是：Stream Type：High Profile @Level 31



3.锁定问题

现在的问题就是，如何让ffmpeg编码时，编码出来baseline等级？



4.解决办法：

打开编码器之前先设置成baseline



**[cpp]**[view plain](http://blog.csdn.net/zhuxian2009/article/details/42969371#)[copy](http://blog.csdn.net/zhuxian2009/article/details/42969371#)



- AVDictionary *opts = NULL;  
- 
- av_dict_set(&opts,"profile","baseline");  
- 
- if(avcodec_open2(pCtxEnc,pCodecEnc,&opts)<0)  
- 
- {  
- 
- //return;  
- 
- }  










