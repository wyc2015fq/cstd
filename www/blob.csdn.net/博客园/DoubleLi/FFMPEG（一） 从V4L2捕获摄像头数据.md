# FFMPEG（一） 从V4L2捕获摄像头数据 - DoubleLi - 博客园






系列相关博文：



[FFMPEG（一） 从V4L2捕获摄像头数据](http://blog.csdn.net/li_wen01/article/details/62226563)

[FFMPEG（二） v4l2 数据格式装换](http://blog.csdn.net/li_wen01/article/details/67631687)

[FFMPEG（三） v4l2 数据编码H264](http://blog.csdn.net/li_wen01/article/details/67638454)



    最近在学习FFMPEG，发现网上的很多例子都是基于读文件的。直接从摄像头中读取数据的却很少。之前写过通过v4l2采集摄像头数据然后调用x264编码成视频文件的例子。在FFMPEG中，它将很多的V4L2操作函数已经封装好，这里提供一个最简的例程。需要注意的是，我采用的FFMPEG的版本是最新的ffmpeg-3.2.4 版本，不同版本的库，接口函数有有些不一样。





**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/62226563#)[copy](http://blog.csdn.net/li_wen01/article/details/62226563#)

[print](http://blog.csdn.net/li_wen01/article/details/62226563#)[?](http://blog.csdn.net/li_wen01/article/details/62226563#)

- /*============================================================================= 
- #     FileName: read_device.c 
- #         Desc: use ffmpeg read a frame data from v4l2  
- #       Author: licaibiao 
- #   LastChange: 2017-03-15  
- =============================================================================*/  
- #include <stdio.h>  
- #include <string.h>  
- #include <stdlib.h>  
- #include <unistd.h>  
- #include "avformat.h"  
- #include "avcodec.h"  
- #include "avdevice.h"  
- 
- char* input_name= "video4linux2";  
- char* file_name = "/dev/video0";  
- char* out_file  = "test.jpeg";  
- 
- void captureOneFrame(void){  
- AVFormatContext *fmtCtx = NULL;      
- AVPacket *packet;   
- AVInputFormat *inputFmt;  
- FILEFILE *fp;   
- int ret;  
- 
- 
-     inputFmt = av_find_input_format (input_name);      
- 
- if (inputFmt == NULL)    {          
-         printf("can not find_input_format\n");          
- return;      
-     }      
- 
- if (avformat_open_input ( &fmtCtx, file_name, inputFmt, NULL) < 0){  
-         printf("can not open_input_file\n");         return;      
-     }  
- /* print device information*/  
-     av_dump_format(fmtCtx, 0, file_name, 0);  
- 
-     packet = (AVPacket *)av_malloc(sizeof(AVPacket));      
-     av_read_frame(fmtCtx, packet);   
-     printf("data length = %d\n",packet->size);     
- 
-     fp = fopen(out_file, "wb");      
- if (fp < 0)    {          
-         printf("open frame data file failed\n");          
- return ;      
-     }      
- 
-     fwrite(packet->data, 1, packet->size, fp);      
- 
-     fclose(fp);      
-     av_free_packet(packet);      
-     avformat_close_input(&fmtCtx);  
-  }   
- 
- int main(void){      
-     avcodec_register_all();      
-     avdevice_register_all();      
-     captureOneFrame();      
- return 0;  
- }  


    Makefile文件如下：







**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/62226563#)[copy](http://blog.csdn.net/li_wen01/article/details/62226563#)

[print](http://blog.csdn.net/li_wen01/article/details/62226563#)[?](http://blog.csdn.net/li_wen01/article/details/62226563#)

- OUT_APP      = test  
- INCLUDE_PATH = /usr/local/include/  
- INCLUDE = -I$(INCLUDE_PATH)libavutil/ -I$(INCLUDE_PATH)libavdevice/ \  
-             -I$(INCLUDE_PATH)libavcodec/ -I$(INCLUDE_PATH)libswresample \  
-             -I$(INCLUDE_PATH)libavfilter/ -I$(INCLUDE_PATH)libavformat \  
-             -I$(INCLUDE_PATH)libswscale/  
- 
- FFMPEG_LIBS = -lavformat -lavutil -lavdevice -lavcodec -lswresample -lavfilter -lswscale  
- SDL_LIBS    =   
- LIBS        = $(FFMPEG_LIBS)$(SDL_LIBS)  
- 
- COMPILE_OPTS = $(INCLUDE)  
- C            = c  
- OBJ          = o  
- C_COMPILER   = cc  
- C_FLAGS      = $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS)  
- 
- LINK         = cc -o   
- LINK_OPTS    = -lz -lm  -lpthread  
- LINK_OBJ     = read_device.o   
- 
- .$(C).$(OBJ):  
-     $(C_COMPILER) -c -g $(C_FLAGS) $<  
- 
- 
- $(OUT_APP): $(LINK_OBJ)  
-     $(LINK)$@  $(LINK_OBJ)  $(LIBS) $(LINK_OPTS)  
- 
- clean:  
-         -rm -rf *.$(OBJ) $(OUT_APP) core *.core *~  *.jpeg  




    编译运行结果如下：





**[objc]**[view plain](http://blog.csdn.net/li_wen01/article/details/62226563#)[copy](http://blog.csdn.net/li_wen01/article/details/62226563#)

[print](http://blog.csdn.net/li_wen01/article/details/62226563#)[?](http://blog.csdn.net/li_wen01/article/details/62226563#)

- licaibiao@ubuntu:~/test/FFMPEG/device$ ./test   
- [video4linux2,v4l2 @ 0x1d18040] Time per frame unknown  
- Input #0, video4linux2,v4l2, from '/dev/video0':  
-  Duration: N/A, bitrate: N/A  
-     Stream #0:0: Video: mjpeg, none, 640x480, 1000k tbn  
- data length = 27697  
- licaibiao@ubuntu:~/test/FFMPEG/device$ ls  
- Makefile  read_device.c  read_device.o  test  test.jpeg  webcm.c  
- licaibiao@ubuntu:~/test/FFMPEG/device$   


    我使用的摄像头输出的是mjpeg格式数据，将它保存到test.jpeg 文件。打开如下：





![](http://img.blog.csdn.net/20170315155551642)











