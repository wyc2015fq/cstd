# FFMPEG实现H264的解码(从源代码角度) - DoubleLi - 博客园






农历2014年底了，将前段时间工作中研究的FFMPEG解码H264流程在此做一下整理，也算作年终技术总结了！

## H264解码原理：

H264的原理参考另一篇博文 [http://blog.csdn.net/rootusers/article/details/43563133](http://blog.csdn.net/rootusers/article/details/43563133)

H264分为NAL(网络抽象层)和VCL(视频编码层)



解码器的总框架：

![](http://img.blog.csdn.net/20150206102212539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9vdHVzZXJz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解码器的流程为：将NAL数据位流输入到H264的解码器中，熵解码模块解码后输出量化系数X；系数经过反量化和反变换得到残差数据R；解码器使用从码流中解码的头信息生成预测块Pred，然后Pred与残差R求和得到块数据dF；每个块dF通过去除块效应的滤波得到解码图像的重建块F。



## FFMPEG源代码解析：

FFMPEG中对于codec的注册和初始化在另一篇博文中有介绍，在这里不在涉及了：[av_register_all](http://blog.csdn.net/rootusers/article/details/40619809)。

FFMPEG处理h264的模块主要是在h264.c中。



## 解码的主函数为：

主函数中的avcodec_init(),avcodec_register_all()等函数都是直接拿去的FFMPGE的源代码实现的，这个工程对于理解FFMPEG的解码流程有很大的作用。





**[cpp]**[view plain](http://blog.csdn.net/rootusers/article/details/43560913#)[copy](http://blog.csdn.net/rootusers/article/details/43560913#)

[print](http://blog.csdn.net/rootusers/article/details/43560913#)[?](http://blog.csdn.net/rootusers/article/details/43560913#)

- int main(){  
- FILE * inp_file;  
- FILE * out_file;  
- 
- int i;  
- int nalLen;         /*NAL 长度*/  
-     unsigned char* Buf; /*H.264码流*/  
- int  got_picture;       /*是否解码一帧图像*/  
- int  consumed_bytes; /*解码器消耗的码流长度*/  
- int cnt=0;  
- 
-     AVCodec *codec;           /* 编解码CODEC*/  
-     AVCodecContext *c;        /* 编解码CODEC context*/  
-     AVFrame *picture;         /* 解码后的图像*/     
- 
- /*输出和输出的文件*/  
-     inp_file = fopen("test.h264", "rb");  
-     out_file = fopen("test.yuv", "wb");  
- 
-     nalLen = 0;  
- /*分配内存，并初始化为0*/  
-     Buf = (unsigned char*)calloc ( 500*1024, sizeof(char));  
- 
- /*CODEC的初始化，初始化一些常量表*/  
-     avcodec_init();   
- 
- /*注册CODEC*/  
-     avcodec_register_all();   
- 
- /*查找 H264 CODEC*/  
-     codec = avcodec_find_decoder(CODEC_ID_H264);  
- 
- if (!codec)  return 0;   
- 
- /*初始化CODEC的默认参数*/  
-     c = avcodec_alloc_context();   
- 
- if(!c)  return 0;  
- 
- /*1. 打开CODEC，这里初始化H.264解码器，调用decode_init本地函数*/  
- if (avcodec_open(c, codec) < 0)  return 0;    
- 
- /*为AVFrame申请空间，并清零*/  
-         picture   = avcodec_alloc_frame();  
- if(!picture)    return 0;  
- 
- /*循环解码*/  
- while(!feof(inp_file))  {  
- /*从码流中获得一个NAL包*/  
-         nalLen = getNextNal(inp_file, Buf);  
- 
- /*2. NAL解码,调用decode_frame本地函数*/  
-         consumed_bytes= avcodec_decode_video(c, picture, &got_picture, Buf, nalLen);  
- 
-         cnt++;  
- /*输出当前的解码信息*/  
-         printf("No:=%4d, length=%4d\n",cnt,consumed_bytes);  
- 
- /*返回<0 表示解码数据头，返回>0，表示解码一帧图像*/  
- if(consumed_bytes > 0)  
-         {  
- /*从二维空间中提取解码后的图像*/  
- for(i=0; i<c->height; i++)  
-                 fwrite(picture->data[0] + i * picture->linesize[0], 1, c->width, out_file);  
- for(i=0; i<c->height/2; i++)  
-                 fwrite(picture->data[1] + i * picture->linesize[1], 1, c->width/2, out_file);  
- for(i=0; i<c->height/2; i++)  
-                 fwrite(picture->data[2] + i * picture->linesize[2], 1, c->width/2, out_file);  
-         }  
- 
-     }  
- 
- /*关闭文件*/  
- if(inp_file)    fclose(inp_file);  
- if(out_file)    fclose(out_file);  
- 
- /*3. 关闭CODEC，释放资源,调用decode_end本地函数*/  
- if(c) {  
-         avcodec_close(c);   
-         av_free(c);  
-         c = NULL;  
-     }   
- /*释放AVFrame空间*/  
- if(picture) {  
-         av_free(picture);  
-         picture = NULL;  
-     }  
- /*释放内存*/  
- if(Buf) {  
-         free(Buf);  
-         Buf = NULL;  
-     }      
- 
- return 0;  
- }  






本工程是参考的网上的很多资料来改写的，因为开源，我们才会变的更优秀，希望所有的开发者都积极分享自己的音视频开发经验。向开源项目致敬。

源代码下载：

[http://download.csdn.net/detail/rootusers/8428977](http://download.csdn.net/detail/rootusers/8428977)

from:http://blog.csdn.net/rootusers/article/details/43560913









