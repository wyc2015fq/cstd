# ffmpeg多种码率控制方式的实现 - tifentan的专栏 - CSDN博客





2018年04月13日 14:53:03[露蛇](https://me.csdn.net/tifentan)阅读数：235








ffmpeg是我们进行视频编解码常用的工具，而对于ffmpeg中编码时对码率的控制方式一直没找合适的教程，无意中在stackoverflow上发现了答案，在此进行总结备忘。

视频编码器常用的码率控制方式包括abr(平均码率)，crf（限制码率），cqp（固定质量），ffmpeg中AVCodecContext显示提供了码率大小的控制参数，但是并没有提供其他的控制方式。ffmpeg中码率控制方式分为以下几种情况：

1.如果设置了AVCodecContext中bit_rate的大小，则采用abr的控制方式；

2.如果没有设置AVCodecContext中的bit_rate，则默认按照crf方式编码，crf默认大小为23（此值类似于qp值，同样表示视频质量）；

3.如果用户想自己设置，则需要借助av_opt_set函数设置AVCodecContext的priv_data参数。下面给出三种控制方式的实现代码：

[cpp] view plain copy 

……. 

int bpsValue;                       //码流控制方式的对应值 

int bpsMode;                        //码流控制方式，0表示平均码率(abr)，1表示固定码率(crf)，2表示固定质量（cqp） 

AVCodecContext* pCodecCtx; 

…….  
//码率控制方式 

string modeValue = int2String(bpsValue); 

switch (bpsMode) { 

case 0: 

    pCodecCtx->bit_rate = bpsValue*1000; 

    break; 

case 1: 

    av_opt_set(pCodecCtx->priv_data,”crf”,modeValue.c_str(),AV_OPT_SEARCH_CHILDREN); 

    break; 

case 2: 

    av_opt_set(pCodecCtx->priv_data,”qp”,modeValue.c_str(),AV_OPT_SEARCH_CHILDREN); 

    break; 

default: 

    pCodecCtx->bit_rate = bpsValue; 

    break; 

}  
同时ffmpeg中采用H264,H265标准编码时，可能需要设置preset、tune和profile，ffmpeg中需要采用额外参数AVDictionary传入avcodec_open2()函数中实现。下面给出参考代码：

[cpp] view plain copy 

//编码器预设 

 AVDictionary *dictParam = 0; 

 if(pCodecCtx->codec_id == AV_CODEC_ID_H264) 

 { 

     av_dict_set(&dictParam,”preset”,”medium”,0); 

     av_dict_set(&dictParam,”tune”,”zerolatency”,0); 

     av_dict_set(&dictParam,”profile”,”main”,0); 

 }  
if(pCodecCtx->codec_id == AV_CODEC_ID_H265) 

 { 

     av_dict_set(&dictParam,”preset”,”ultrafast”,0); 

     av_dict_set(&dictParam,”tune”,”zerolatency”,0); 

     av_dict_set(&dictParam,”profile”,”main”,0); 

 }  
//输出格式信息 

 av_dump_format(pFormatCtx, 0, outputFileName.c_str(), 1);  

pCodec = avcodec_find_encoder(pCodecCtx->codec_id); 

 if (!pCodec) 

 { 

     printf(“没有找到合适的编码器！\n”); 

     return false; 

 } 

 if (avcodec_open2(pCodecCtx, pCodec,&dictParam) < 0) 

 { 

     printf(“编码器打开失败！\n”); 

     return false; 

 }  







