# ffmpeg快速命令使用 - 深之JohnChen的专栏 - CSDN博客

2014年05月18日 13:14:50[byxdaz](https://me.csdn.net/byxdaz)阅读数：2372


**ffmpeg使用语法**ffmpeg [[options][`-i' input_file]]... {[options] output_file}...

如果没有输入文件，那么视音频捕捉（只在Linux下有效，因为Linux下把音视频设备当作文件句柄来处理）就会起作用。作为通用的规则，选项一 般用于下一个特定的文件。如果你给 –b 64选项，改选会设置下一个视频速率。对于原始输入文件，格式选项可能是需要的。缺省情况下，ffmpeg试图尽可能的无损转换，采用与输入同样的音频视 频参数来输出。

ffmpeg转换所涉及到的选项较多，可参考[ffmpeg选项详解](http://www.cnblogs.com/top5/archive/2009/12/30/1636352.html)。

ffmpeg支持多种文件格式和多种音频、视频编码器，可参考[ffmepg格式详解](http://www.cnblogs.com/top5/archive/2009/12/30/1636353.html)，（附：[常见视频文件格式详解](http://www.cnblogs.com/top5/archive/2009/12/30/1636358.html)）
**视频文件截图**- 截取一张352x240尺寸大小的，格式为jpg的图片 
ffmpeg -i test.asf -y -f image2 -t 0.001 -s 352x240 a.jpg

- 把视频的前３０帧转换成一个Animated Gif 
ffmpeg -i test.asf -vframes 30 -y -f gif a.gif

- 截取指定时间的缩微图 
ffmpeg -i test.avi -y -f image2 -ss 8 -t 0.001 -s 350x240 test.jpg

-ss后跟的时间单位为秒 - 转换文件为3GP格式 
ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 

-acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp

或 ffmpeg -y -i test.wmv -ac 1 -acodec libamr_nb -ar 8000 -ab 12200 -s 176x144 -b 128 -r 15 test.3gp

**视频格式转换**- 如何使用 ffmpeg 编码得到高质量的视频 
ffmpeg.exe -i "D:\Video\Fearless\Fearless.avi" -target film-dvd -s 720x352

-padtop 64 -padbottom 64 -maxrate 7350000 -b 3700000 -sc_threshold 1000000000 

-trellis -cgop -g 12 -bf 2 -qblur 0.3 -qcomp 0.7 -me full -dc 10 -mbd 2

-aspect 16:9 -pass 2 -passlogfile "D:\Video\ffmpegencode" -an -f mpeg2video "D:\Fearless.m2v"

- 转换指定格式文件到FLV格式 
ffmpeg.exe -i test.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 

ffmpeg.exe -i test.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv

- 转码解密的VOB 
ffmpeg -i snatch_1.vob -f avi -vcodec mpeg4 -b 800 -g 300 -bf 2 -acodec mp3 -ab 128 snatch.avi

上面的命令行将vob的文件转化成avi文件，mpeg4的视频和mp3的音频。注意命令中使用了B帧，所以mpeg4流是divx5兼容的。GOP大小是300意味着29.97帧频下每10秒就有INTRA帧。该映射在音频语言的DVD转码时候尤其有用。- 同时编码到几种格式并且在输入流和输出流之间建立映射 
ffmpeg -i /tmp/a.wav -ab 64 /tmp/a.mp2 -ab 128 /tmp/b.mp2 -map 0:0 -map 0:0

上面的命令行转换一个64Kbits 的a.wav到128kbits的a.mp2 ‘-map file:index’在输出流的顺序上定义了哪一路输入流是用于每一个输出流的。- 转换文件为3GP格式 
ffmpeg -i test.avi -y -b 20 -s sqcif -r 10 -acodec amr_wb -ab 23.85 -ac 1 -ar 16000 test.3gp

注：如果要转换为3GP格式，则ffmpeg在编译时必须加上–enable-amr_nb –enable-amr_wb，详细内容可参考：[转换视频为3GPP格式](http://www.cnblogs.com/top5/archive/2009/12/30/1636364.html)- 转换文件为MP4格式（支持iPhone/iTouch） 
ffmpeg -y -i input.wmv -f mp4 -async 1-s 480x320 -acodec libfaac -vcodec libxvid -qscale 7 -dts_delta_threshold 1 output.mp4

ffmpeg -y -i source_video.avi input -acodec libfaac -ab 128000 -vcodec mpeg4 -b 1200000 -mbd 2 -flags +4mv+trell -aic 2 -cmp 2 -subcmp 2 -s 320x180 -title X final_video.mp4

- 将一段音频与一段视频混合 
ffmpeg -i son.wav -i video_origine.avi video_finale.mpg

- 将一段视频转换为DVD格式 
ffmpeg -i source_video.avi -target pal-dvd -ps 2000000000 -aspect 16:9 finale_video.mpeg

注：target pal-dvd : Output format ps 2000000000 maximum size for the output file, in bits (here, 2 Gb) aspect 16:9 : Widescreen- 转换一段视频为DivX格式 
ffmpeg -i video_origine.avi -s 320x240 -vcodec msmpeg4v2 video_finale.avi

- Turn X images to a video sequence 
ffmpeg -f image2 -i image%d.jpg video.mpg

注：This command will transform all the images from the current directory (named image1.jpg, image2.jpg, etc...) to a video file named video.mpg.- Turn a video to X images 
ffmpeg -i video.mpg image%d.jpg

注：This command will generate the files named image1.jpg, image2.jpg, ... The following image formats are also availables : PGM, PPM, PAM, PGMYUV, JPEG, GIF, PNG, TIFF, SGI.**屏幕录制**- 使用ffmpeg录像屏幕 
ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -vd x11:0,0 -s 1024x768 ~/test.avi

：其中，-vd x11:0,0 指录制所使用的偏移为 x=0 和 y=0，-s 1024×768 指录制视频的大小为 1024×768。录制的视频文件为 test.avi，将保存到用户主目录中
如果你只想录制一个应用程序窗口或者桌面上的一个固定区域，那么可以指定偏移位置和区域大小。使用xwininfo -frame命令可以完成查找上述参数。- 重新调整视频尺寸大小 
ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -i ~/test.avi -s 800×600 ~/test-800-600.avi

注：ffmpeg的屏幕录制功能只能在Linux环境下有效。 
**视频录制**

ffmpeg –i rtsp://192.168.3.205:5555/test –vcodec copy out.avi

**视频采集**

- 把摄像头的实时视频录制下来，存储为文件 
ffmpeg -f video4linux -s 320*240 -r 10 -i /dev/video0 test.asf

更多信息可参考ffmpeg工程组论坛的讨论贴：[有关ffmpeg的视频采集](http://bbs.chinavideo.org/viewthread.php?tid=2640&extra=page%3D1)注：ffmpeg的视频采集功能只能在Linux环境下使用 **使用ffmpeg压制H.264视频**ffmpeg -threads 4 -i INPUT -r 29.97 -vcodec libx264 -s 480x272 -flags +loop -cmp +chroma -deblockalpha 0 -deblockbeta 0 -crf 24 

-bt 256k -refs 1 -coder 0 -me umh -me_range 16 -subq 5 -partitions +parti4x4+parti8x8+partp8x8 -g 250 -keyint_min 25 -level 30 

-qmin 10 -qmax 51 -trellis 2 -sc_threshold 40 -i_qfactor 0.71 -acodec libfaac -ab 128k -ar 48000 -ac 2 OUTPUT

注：使用该指令可以压缩出比较清晰，而且文件转小的H.264视频文件
**YUV序列播放**

lay -f rawvideo -video_size 1920x1080 input.yuv

**YUV序列转AVI**

ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi

