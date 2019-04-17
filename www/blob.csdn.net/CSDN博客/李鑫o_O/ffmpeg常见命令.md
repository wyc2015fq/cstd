# ffmpeg常见命令 - 李鑫o_O - CSDN博客





2016年03月14日 11:16:44[hustlx](https://me.csdn.net/HUSTLX)阅读数：868








# 一、安装



       下载ffmpeg，解压之后配置环境变量即为安装

       打开dos界面，进入目标文件夹例如：E:/ cd E:\BaiduYunDownload\ffmpeg\ffmpeg_simple_player\ffmpeg_simple_player\ffmpeg_simple_player1\res

二、命令参数

       命令格式：ffmpeg -i [输入文件名] [参数选项] -f [格式] [输出文件]

       1、参数选项：

       (1) -an: 去掉音频

       (2) -acodec: 音频选项， 一般后面加copy表示拷贝

       (3) -vcodec:视频选项，一般后面加copy表示拷贝

        2、格式：

       (1) h264: 表示输出的是h264的视频裸流

       (2) mp4: 表示输出的是mp4的视频

       (3)mpegts: 表示ts视频流

三、视频格式转换

    h264裸流是没有音频的

      1、H264视频转ts视频流：

    ffmpeg -i test.h264 -vcodec copy -f mpegts test.ts  

      2、H264视频转mp4:

    ffmpeg -i test.h264 -vcodec copy -f mp4 test.mp4  

      3、ts视频转mp4:

    ffmpeg -i test.ts -acodec copy -vcodec copy -f mp4 test.mp4 

      4、MP4转ts

    ffmpeg -i b.mp4 -codec copy -bsf h264_mp4toannexb a.ts

      5、avi转ts

   ffmpeg -i B.avi -acodec aac -vcodec libx264 -b 8000K B.ts

    1.分离视频音频流

    ffmpeg -i input_file -vcodec copy -an output_file_video　　//分离视频流

    ffmpeg -i input_file -acodec copy -vn output_file_audio　　//分离音频流


    ffmpeg -i input_file -vcodec copy -an output_file_video　　//分离视频流

    ffmpeg -i input_file -acodec copy -vn output_file_audio　　//分离音频流

    ffmpeg -i input_file -vn -acodec pcm_s16le output_file_audio.wav 

    2.视频解复用

    ffmpeg –i test.mp4 –vcodec copy –an –f m4v test.264

    ffmpeg –i test.avi –vcodec copy –an –f m4v test.264

    3.视频转码

    ffmpeg –i test.mp4 –vcodec h264 –s 352*278 –an –f m4v test.264              //转码为码流原始文件

    ffmpeg –i test.mp4 –vcodec h264 –bf 0 –g 25 –s 352*278 –an –f m4v test.264  //转码为码流原始文件

    ffmpeg –i test.avi -vcodec mpeg4 –vtag xvid –qsame test_xvid.avi            //转码为封装文件

    //-bf B帧数目控制，-g 关键帧间隔控制，-s 分辨率控制

    4.视频封装

    ffmpeg –i video_file –i audio_file –vcodec copy –acodec copy output_file

    5.视频剪切

    ffmpeg –i test.avi –r 1 –f image2 image-%3d.jpeg        //提取图片

    ffmpeg -ss 0:1:30 -t 0:0:20 -i input.avi -vcodec copy -acodec copy output.avi    //剪切视频

    //-r 提取图像的频率，-ss 开始时间，-t 持续时间

    6.视频录制

    ffmpeg –i rtsp://192.168.3.205:5555/test –vcodec copy out.avi

    7.YUV序列播放

    ffplay -f rawvideo -video_size 1920x1080 input.yuv

    8.YUV序列转AVI

    ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi

四、视频拼接：

      对于视频拼接，个人感觉最好是从视频的裸码流开始拼接，后面再进行视频包装，即格式转换。

      拼接命令：

    ffmpeg -i "concat:test1.h264|test2.h264" -vcodec copy -f h264 out12.h264 

五、音频

     ffmpeg -f s16le -ar 44.1k -ac 2 -i file.pcm file.wav





