# ffmpeg 常用命令汇总

 



﻿﻿

part 1:

经常用到ffmpeg 做一些视频数据的处理转换等，用来做测试，今天总结了一下，参考了网上部分朋友的经验，一起在这里汇总了一下，有需要的朋友可以收藏测试一下，有问题欢迎在下面回帖交流，谢谢;by ternence.hsu)

## 1、ffmpeg使用语法

```
命令格式：
    ffmpeg -i [输入文件名] [参数选项] -f [格式] [输出文件]
    ffmpeg [[options][`-i' input_file]]... {[options] output_file}...
    1、参数选项：
    (1) -an: 去掉音频
    (2) -acodec: 音频选项， 一般后面加copy表示拷贝
    (3) -vcodec:视频选项，一般后面加copy表示拷贝
    2、格式：
    (1) h264: 表示输出的是h264的视频裸流
    (2) mp4: 表示输出的是mp4的视频
    (3)mpegts: 表示ts视频流
    如果没有输入文件，那么视音频捕捉（只在Linux下有效，因为Linux下把音视频设备当作文件句柄来处理）就会起作用。作为通用的规则，选项一般用于下一个特定的文件。如果你给 –b 64选项，改选会设置下一个视频速率。对于原始输入文件，格式选项可能是需要的。缺省情况下，ffmpeg试图尽可能的无损转换，采用与输入同样的音频视频参数来输出。（by ternence.hsu）
```

 



## 2、视频转换

 

```
H264视频转ts视频流

ffmpeg -i test.h264 -vcodec copy -f mpegts test.ts



H264视频转mp4

ffmpeg -i test.h264 -vcodec copy -f mp4 test.mp4



ts视频转mp4

ffmpeg -i test.ts -acodec copy -vcodec copy -f mp4 test.mp4
mp4视频转flv
ffmpeg -i test.mp4 -acodec copy -vcodec copy -f flv test.flv 


转换文件为3GP格式 

ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 -acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp



转换文件为3GP格式 v2

ffmpeg -y -i test.wmv -ac 1 -acodec libamr_nb -ar 8000 -ab 12200 -s 176x144 -b 128 -r 15 test.3gp



使用 ffmpeg 编码得到高质量的视频

ffmpeg.exe -i "D:\Video\Fearless\Fearless.avi" -target film-dvd -s 720x352 -padtop 64 -padbottom 64 -maxrate 7350000 -b 3700000 -sc_threshold 1000000000 -trellis -cgop -g 12 -bf 2 -qblur 0.3 -qcomp 0.7 -me full -dc 10 -mbd 2 -aspect 16:9 -pass 2 -passlogfile "D:\Video\ffmpegencode" -an -f mpeg2video "D:\Fearless.m2v"



转换指定格式文件到FLV格式

ffmpeg.exe -i test.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 
ffmpeg.exe -i test.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv



转码解密的VOB

ffmpeg -i snatch_1.vob -f avi -vcodec mpeg4 -b 800 -g 300 -bf 2 -acodec mp3 -ab 128 snatch.avi

（上面的命令行将vob的文件转化成avi文件，mpeg4的视频和mp3的音频。注意命令中使用了B帧，所以mpeg4流是divx5兼容的。GOP大小是300意味着29.97帧频下每10秒就有INTRA帧。该映射在音频语言的DVD转码时候尤其有用，同时编码到几种格式并且在输入流和输出流之间建立映射）


转换文件为3GP格式

ffmpeg -i test.avi -y -b 20 -s sqcif -r 10 -acodec amr_wb -ab 23.85 -ac 1 -ar 16000 test.3gp

（如果要转换为3GP格式，则ffmpeg在编译时必须加上–enable-amr_nb –enable-amr_wb，详细内容可参考：转换视频为3GPP格式）


转换文件为MP4格式（支持iPhone/iTouch）

ffmpeg  -y  -i input.wmv  -f mp4 -async 1-s 480x320  -acodec libfaac -vcodec libxvid  -qscale 7 -dts_delta_threshold 1 output.mp4
ffmpeg  -y  -i source_video.avi input -acodec libfaac -ab 128000 -vcodec mpeg4 -b 1200000 -mbd 2 -flags +4mv+trell -aic 2 -cmp 2 -subcmp 2 -s 320x180 -title X final_video.mp4



将一段音频与一段视频混合

ffmpeg -i son.wav -i video_origine.avi video_finale.mpg



将一段视频转换为DVD格式

ffmpeg -i source_video.avi -target pal-dvd -ps 2000000000 -aspect 16:9 finale_video.mpeg

（target pal-dvd : Output format ps 2000000000 maximum size for the output file, in bits (here, 2 Gb) aspect 16:9 : Widescreen）


转换一段视频为DivX格式

ffmpeg -i video_origine.avi -s 320x240 -vcodec msmpeg4v2 video_finale.avi



Turn X images to a video sequence

ffmpeg -f image2 -i image%d.jpg video.mpg

（This command will transform all the images from the current directory (named image1.jpg, image2.jpg, etc...) to a video file named video.mpg.）


Turn a video to X images

ffmpeg -i video.mpg image%d.jpg

（This command will generate the files named image1.jpg, image2.jpg, ... ；The following image formats are also availables : PGM, PPM, PAM, PGMYUV, JPEG, GIF, PNG, TIFF, SGI.）


使用ffmpeg录像屏幕(仅限Linux平台)

ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -vd x11:0,0 -s 1024x768 ~/test.avi

（-vd x11:0,0 指录制所使用的偏移为 x=0 和 y=0，-s 1024×768 指录制视频的大小为 1024×768。录制的视频文件为 test.avi，将保存到用户主目录中；如果你只想录制一个应用程序窗口或者桌面上的一个固定区域，那么可以指定偏移位置和区域大小。使用xwininfo -frame命令可以完成查找上述参数。）


重新调整视频尺寸大小(仅限Linux平台)

ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -i ~/test.avi -s 800×600 ~/test-800-600.avi



把摄像头的实时视频录制下来，存储为文件(仅限Linux平台)

ffmpeg  -f video4linux -s 320*240 -r 10 -i /dev/video0 test.asf



使用ffmpeg压制H.264视频

ffmpeg -threads 4 -i INPUT -r 29.97 -vcodec libx264 -s 480x272 -flags +loop -cmp chroma -deblockalpha 0 -deblockbeta 0 -crf 24 -bt 256k -refs 1 -coder 0 -me umh -me_range 16 -subq 5 -partitions parti4x4+parti8x8+partp8x8 -g 250 -keyint_min 25 -level 30 -qmin 10 -qmax 51 -trellis 2 -sc_threshold 40 -i_qfactor 0.71 -acodec libfaac -ab 128k -ar 48000 -ac 2 OUTPUT

（使用该指令可以压缩出比较清晰，而且文件转小的H.264视频文件）
```


3、网络推送

```
udp视频流的推送
ffmpeg -re  -i 1.ts  -c copy -f mpegts   udp://192.168.0.106:1234
```

 


4、视频拼接

 

```
裸码流的拼接，先拼接裸码流，再做容器的封装
ffmpeg -i "concat:test1.h264|test2.h264" -vcodec copy -f h264 out12.h264
```

5、图像相关

  

```
截取一张352x240尺寸大小的，格式为jpg的图片 
ffmpeg -i test.asf -y -f image2 -t 0.001 -s 352x240 a.jpg

把视频的前30帧转换成一个Animated Gif 
ffmpeg -i test.asf -vframes 30 -y -f gif a.gif

截取指定时间的缩微图,-ss后跟的时间单位为秒 
ffmpeg -i test.avi -y -f image2 -ss 8 -t 0.001 -s 350x240 test.jpg
```


6、音频处理

 

   

```
转换wav到mp2格式

ffmpeg -i /tmp/a.wav -ab 64 /tmp/a.mp2 -ab 128 /tmp/b.mp2 -map 0:0 -map 0:0

（上面的命令行转换一个64Kbits 的a.wav到128kbits的a.mp2 ‘-map file:index’在输出流的顺序上定义了哪一路输入流是用于每一个输出流的。）
```

 

 

 

7、切割ts分片

```
ffmpeg -i input.mp4 -c:v libx264 -c:a aac -strict -2 -f hls -hls_list_size 6 -hls_time 5 output1.m3u8
```

 

============================================================================

part 2:

 

 

一、ffmpeg命令详解

ffmpeg非常强大，轻松几条命令就可以完成你的工作。

```
把darkdoor.[001-100].jpg序列帧和001.mp3音频文件利用mpeg4编码方式合成视频文件darkdoor.avi：
$ ffmpeg -i 001.mp3 -i darkdoor.%3d.jpg -s 1024x768 -author skypp -vcodec mpeg4 darkdoor.avi

ffmpeg还支持mov格式：
$ ffmpeg -i darkdoor.%3d.jpg darkdoor.mov

要查看你的ffmpeg支持哪些格式，可以用如下命令：
$ ffmpeg -formats | less

还可以把视频文件导出成jpg序列帧：
$ ffmpeg -i bc-cinematic-en.avi example.%d.jpg

debian下安装ffmpeg很简单：
＃apt-get install ffmpeg
```

**###################################****###**

**下面是转来的使用说明，慢慢研究吧，嘿嘿**

**################################****###****###**
 

### ffmpeg使用语法

ffmpeg使用语法：

ffmpeg [[options][`-i' input_file]]... {[options] output_file}...

如果没有输入文件，那么视音频捕捉就会起作用。

作为通用的规则，选项一般用于下一个特定的文件。如果你给 –b 64选项，改选会设置下一个视频速率。对于原始输入文件，格式选项可能是需要的。

缺省情况下，ffmpeg试图尽可能的无损转换，采用与输入同样的音频视频参数来输出。

3．选项

a) 通用选项

-L license

-h 帮助

-fromats 显示可用的格式，编解码的，协议的。。。

-f fmt 强迫采用格式fmt

-I filename 输入文件

-y 覆盖输出文件

-t duration 设置纪录时间 hh:mm:ss[.xxx]格式的记录时间也支持

-ss position 搜索到指定的时间 [-]hh:mm:ss[.xxx]的格式也支持

-title string 设置标题

-author string 设置作者

-copyright string 设置版权

-comment string 设置评论

-target type 设置目标文件类型(vcd,svcd,dvd) 所有的格式选项（比特率，编解码以及缓冲区大小）自动设置，只需要输入如下的就可以了：
ffmpeg -i myfile.avi -target vcd /tmp/vcd.mpg

-hq 激活高质量设置

-itsoffset offset 设置以秒为基准的时间偏移，该选项影响所有后面的输入文件。该偏移被加到输入文件的时戳，定义一个正偏移意味着相应的流被延迟了 offset秒。 [-]hh:mm:ss[.xxx]的格式也支持

b) 视频选项

-b bitrate 设置比特率，缺省200kb/s

-r fps 设置帧频 缺省25

-s size 设置帧大小 格式为WXH 缺省160X128.下面的简写也可以直接使用：
Sqcif 128X96 qcif 176X144 cif 252X288 4cif 704X576

-aspect aspect 设置横纵比 4:3 16:9 或 1.3333 1.7777

-croptop size 设置顶部切除带大小 像素单位

-cropbottom size –cropleft size –cropright size

-padtop size 设置顶部补齐的大小 像素单位

-padbottom size –padleft size –padright size –padcolor color 设置补齐条颜色(hex,6个16进制的数，红:绿:兰排列，比如 000000代表黑色)

-vn 不做视频记录

-bt tolerance 设置视频码率容忍度kbit/s

-maxrate bitrate设置最大视频码率容忍度

-minrate bitreate 设置最小视频码率容忍度

-bufsize size 设置码率控制缓冲区大小

-vcodec codec 强制使用codec编解码方式。如果用copy表示原始编解码数据必须被拷贝。

-sameq 使用同样视频质量作为源（VBR）

-pass n 选择处理遍数（1或者2）。两遍编码非常有用。第一遍生成统计信息，第二遍生成精确的请求的码率

-passlogfile file 选择两遍的纪录文件名为file


c)高级视频选项

-g gop_size 设置图像组大小

-intra 仅适用帧内编码

-qscale q 使用固定的视频量化标度(VBR)

-qmin q 最小视频量化标度(VBR)

-qmax q 最大视频量化标度(VBR)

-qdiff q 量化标度间最大偏差 (VBR)

-qblur blur 视频量化标度柔化(VBR)

-qcomp compression 视频量化标度压缩(VBR)

-rc_init_cplx complexity 一遍编码的初始复杂度

-b_qfactor factor 在p和b帧间的qp因子

-i_qfactor factor 在p和i帧间的qp因子

-b_qoffset offset 在p和b帧间的qp偏差

-i_qoffset offset 在p和i帧间的qp偏差

-rc_eq equation 设置码率控制方程 默认tex^qComp

-rc_override override 特定间隔下的速率控制重载

-me method 设置运动估计的方法 可用方法有 zero phods log x1 epzs(缺省) full

-dct_algo algo 设置dct的算法 可用的有 0 FF_DCT_AUTO 缺省的DCT 1 FF_DCT_FASTINT 2 FF_DCT_INT 3 FF_DCT_MMX 4 FF_DCT_MLIB 5 FF_DCT_ALTIVEC

-idct_algo algo 设置idct算法。可用的有 0 FF_IDCT_AUTO 缺省的IDCT 1 FF_IDCT_INT 2 FF_IDCT_SIMPLE 3 FF_IDCT_SIMPLEMMX 4 FF_IDCT_LIBMPEG2MMX 5 FF_IDCT_PS2 6 FF_IDCT_MLIB 7 FF_IDCT_ARM 8 FF_IDCT_ALTIVEC 9 FF_IDCT_SH4 10 FF_IDCT_SIMPLEARM

-er n 设置错误残留为n 1 FF_ER_CAREFULL 缺省 2 FF_ER_COMPLIANT 3 FF_ER_AGGRESSIVE 4 FF_ER_VERY_AGGRESSIVE

-ec bit_mask 设置错误掩蔽为bit_mask,该值为如下值的位掩码 1 FF_EC_GUESS_MVS (default=enabled) 2 FF_EC_DEBLOCK (default=enabled)

-bf frames 使用frames B 帧，支持mpeg1,mpeg2,mpeg4

-mbd mode 宏块决策 0 FF_MB_DECISION_SIMPLE 使用mb_cmp 1 FF_MB_DECISION_BITS 2 FF_MB_DECISION_RD

-4mv 使用4个运动矢量 仅用于mpeg4

-part 使用数据划分 仅用于mpeg4

-bug param 绕过没有被自动监测到编码器的问题

-strict strictness 跟标准的严格性

-aic 使能高级帧内编码 h263+

-umv 使能无限运动矢量 h263+

-deinterlace 不采用交织方法

-interlace 强迫交织法编码仅对mpeg2和mpeg4有效。当你的输入是交织的并且你想要保持交织以最小图像损失的时候采用该选项。可选的方法是不交织，但是损失更大

-psnr 计算压缩帧的psnr

-vstats 输出视频编码统计到vstats_hhmmss.log

-vhook module 插入视频处理模块 module 包括了模块名和参数，用空格分开

D)音频选项

-ab bitrate 设置音频码率

-ar freq 设置音频采样率

-ac channels 设置通道 缺省为1

-an 不使能音频纪录

-acodec codec 使用codec编解码

E)音频/视频捕获选项

-vd device 设置视频捕获设备。比如/dev/video0

-vc channel 设置视频捕获通道 DV1394专用

-tvstd standard 设置电视标准 NTSC PAL(SECAM)

-dv1394 设置DV1394捕获

-av device 设置音频设备 比如/dev/dsp


F)高级选项

-map file:stream 设置输入流映射

-debug 打印特定调试信息

-benchmark 为基准测试加入时间

-hex 倾倒每一个输入包

-bitexact 仅使用位精确算法 用于编解码测试

-ps size 设置包大小，以bits为单位

-re 以本地帧频读数据，主要用于模拟捕获设备

-loop 循环输入流。只工作于图像流，用于ffserver测试

二、Ffmpeg使用语法

Ffmpeg使用语法

```html
ffmpeg [[options][`-i' input_file]]... {[options] output_file}...
```

如果没有输入文件，那么视音频捕捉（只在Linux下有效，因为Linux下把音视频设备当作文件句柄来处理）就会起作用。作为通用的规则，选项一 般用于下一个特定的文件。如果你给 –b 64选项，改选会设置下一个视频速率。对于原始输入文件，格式选项可能是需要的。缺省情况下，ffmpeg试图尽可能的无损转换，采用与输入同样的音频视 频参数来输出。


ffmpeg转换所涉及到的选项较多，可参考[ffmpeg选项详解](http://www.ffmpeg.com.cn/index.php/Ffmpeg%E9%80%89%E9%A1%B9%E8%AF%A6%E8%A7%A3)。


ffmpeg支持多种文件格式和多种音频、视频编码器，可参考[ffmepg格式详解](http://www.ffmpeg.com.cn/index.php/Ffmepg%E6%A0%BC%E5%BC%8F%E8%AF%A6%E8%A7%A3)，（附：[常见视频文件格式详解](http://www.ffmpeg.com.cn/index.php/%E5%B8%B8%E8%A7%81%E8%A7%86%E9%A2%91%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F%E8%AF%A6%E8%A7%A3)）

 

## 视频文件截图

- 截取一张352x240尺寸大小的，格式为jpg的图片

```html
ffmpeg -i test.asf -y -f image2 -t 0.001 -s 352x240 a.jpg
```

- 把视频的前３０帧转换成一个Animated Gif

```html
ffmpeg -i test.asf -vframes 30 -y -f gif a.gif
```

- 截取指定时间的缩微图

```html
ffmpeg -i test.avi -y -f image2 -ss 8 -t 0.001 -s 350x240 test.jpg
```

-ss后跟的时间单位为秒

- 转换文件为3GP格式

```html
ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 



 -acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp
```

或

```html
ffmpeg -y -i test.wmv -ac 1 -acodec libamr_nb -ar 8000 -ab 12200 -s 176x144 -b 128 -r 15 test.3gp
```

 

## 视频格式转换

- 如何使用 ffmpeg 编码得到高质量的视频

```html
ffmpeg.exe -i "D:\Video\Fearless\Fearless.avi" -target film-dvd -s 720x352



 -padtop 64 -padbottom 64 -maxrate 7350000 -b 3700000 -sc_threshold 1000000000 



 -trellis -cgop -g 12 -bf 2 -qblur 0.3 -qcomp 0.7 -me full -dc 10 -mbd 2



 -aspect 16:9 -pass 2 -passlogfile "D:\Video\ffmpegencode" -an -f mpeg2video "D:\Fearless.m2v"
```

- 转换指定格式文件到FLV格式

```html
ffmpeg.exe -i test.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 



ffmpeg.exe -i test.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv
```

- 转码解密的VOB

```html
ffmpeg -i snatch_1.vob -f avi -vcodec mpeg4 -b 800 -g 300 -bf 2 -acodec mp3 -ab 128 snatch.avi
```

上面的命令行将vob的文件转化成avi文件，mpeg4的视频和mp3的音频。注意命令中使用了B帧，所以mpeg4流是divx5兼容的。GOP大小是300意味着29.97帧频下每10秒就有INTRA帧。该映射在音频语言的DVD转码时候尤其有用。

- 同时编码到几种格式并且在输入流和输出流之间建立映射

```html
ffmpeg -i /tmp/a.wav -ab 64 /tmp/a.mp2 -ab 128 /tmp/b.mp2 -map 0:0 -map 0:0
```

上面的命令行转换一个64Kbits 的a.wav到128kbits的a.mp2 ‘-map file:index’在输出流的顺序上定义了哪一路输入流是用于每一个输出流的。

- 转换文件为3GP格式

```html
ffmpeg -i test.avi -y -b 20 -s sqcif -r 10 -acodec amr_wb -ab 23.85 -ac 1 -ar 16000 test.3gp
```

注：如果要转换为3GP格式，则ffmpeg在编译时必须加上–enable-amr_nb –enable-amr_wb，详细内容可参考：[转换视频为3GPP格式](http://www.ffmpeg.com.cn/index.php/%E8%BD%AC%E6%8D%A2%E8%A7%86%E9%A2%91%E4%B8%BA3GPP%E6%A0%BC%E5%BC%8F)

 

## 屏幕录制

- 使用ffmpeg录像屏幕

```html
ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -vd x11:0,0 -s 1024x768 ~/test.avi
```

：其中，-vd x11:0,0 指录制所使用的偏移为 x=0 和 y=0，-s 1024×768 指录制视频的大小为 1024×768。录制的视频文件为 test.avi，将保存到用户主目录中

如果你只想录制一个应用程序窗口或者桌面上的一个固定区域，那么可以指定偏移位置和区域大小。使用xwininfo -frame命令可以完成查找上述参数。

- 重新调整视频尺寸大小

```html
ffmpeg -vcodec mpeg4 -b 1000 -r 10 -g 300 -i ~/test.avi -s 800×600 ~/test-800-600.avi
```

注：ffmpeg的屏幕录制功能只能在Linux环境下有效。

 

## 视频采集

- 把摄像头的实时视频录制下来，存储为文件

```html
ffmpeg  -f video4linux -s 320*240 -r 10 -i /dev/video0  test.asf
三、ffmepg使用
```

各种转换例子见：
[FFmpeg Howto](http://www.itbroadcastanddigitalcinema.com/ffmpeg_howto.html)
程序下载：
[Unofficial FFmpeg Win32 Builds](http://arrozcru.no-ip.org/ffmpeg_builds/)(需要使用代理访问)
或：
[FFMPEG FULL SDK V3.0](http://www.bairuitech.com/html/ruanjianxiazai/20070812/51.html)

The generic syntax is:

```
ffmpeg [[infile options][`-i' infile]]… {[outfile options] outfile}…
```

As a general rule, options are applied to the next specified file. Therefore, order is important, and you can have the same option on the command line multiple times. Each occurrence is then applied to the next input or output file.

\* To set the video bitrate of the output file to 64kbit/s:

```
ffmpeg -i input.avi -b 64k output.avi
```

\* To force the frame rate of the input and output file to 24 fps:

```
ffmpeg -r 24 -i input.avi output.avi
```

\* To force the frame rate of the output file to 24 fps:

```
ffmpeg -i input.avi -r 24 output.avi
```

\* To force the frame rate of input file to 1 fps and the output file to 24 fps:

```
ffmpeg -r 1 -i input.avi -r 24 output.avi
```

The format option may be needed for raw input files.

By default, FFmpeg tries to convert as losslessly as possible: It uses the same audio and video parameters for the outputs as the one specified for the inputs.

转换文件为3GP格式
`ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 -acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp`
或
`ffmpeg -y -i test.mpeg -ac 1 -acodec amr_nb -ar 8000 -s 176x144 -b 128 -r 15 test.3gp`

转换指定格式文件到FLV格式

```
ffmpeg.exe -i test.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv `
`ffmpeg.exe -i test.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv***************************************************************************ffmpeg -i F:\tools\ffmpeg.rev10464\test.avi -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 F:\tools\ffmpeg.rev10464\test.flvffmpeg -i "F:\tools\ffmpeg.rev10464\test.flv" -y -f image2 -ss 8 -t 0.001 -s 350x240 "test.jpg" 
```

 

 

ffmpeg.exe -i F:\闪客之家\闪客之歌.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\11.flv 
ffmpeg -i F:\01.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 
使用-ss参数 作用（time_off set the start time offset），可以从指定时间点开始转换任务。如: 
转换文件格式的同时抓缩微图： 
ffmpeg -i "test.avi" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg' 
对已有flv抓图： 
ffmpeg -i "test.flv" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg' 
-ss后跟的时间单位为秒 
Ffmpeg转换命令 
ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 -acodec aac -ac 2 -ar 22500 
-ab 24 -f 3gp test.3gp 
或者 
ffmpeg -y -i test.mpeg -ac 1 -acodec amr_nb -ar 8000 -s 176x144 -b 128 -r 15 test.3gp 


ffmpeg参数设定解说 
-bitexact 使用标准比特率 
-vcodec xvid 使用xvid压缩 
-s 320x240 指定分辨率 
-r 29.97 桢速率（可以改，确认非标准桢率会导致音画不同步，所以只能设定为15或者29.97） 
画面部分，选其一 
-b <比特率> 指定压缩比特率，似乎ffmpeg是自动VBR的，指定了就大概是平均比特率，比如768，1500这样的 
就是原来默认项目中有的 
-qscale <数值> 以<数值>质量为基础的VBR，取值0.01-255，约小质量越好 
-qmin <数值> 设定最小质量，与-qmax（设定最大质量）共用，比如-qmin 10 -qmax 31 
-sameq 使用和源同样的质量 
声音部分 
-acodec aac 设定声音编码 
-ac <数值> 设定声道数，1就是单声道，2就是立体声，转换单声道的TVrip可以用1（节省一半容量），高品质 
的DVDrip就可以用2 
-ar <采样率> 设定声音采样率，PSP只认24000 
-ab <比特率> 设定声音比特率，前面-ac设为立体声时要以一半比特率来设置，比如192kbps的就设成96，转换 
君默认比特率都较小，要听到较高品质声音的话建议设到160kbps（80）以上 
-vol <百分比> 设定音量，某些DVDrip的AC3轨音量极小，转换时可以用这个提高音量，比如200就是原来的2倍 
这样，要得到一个高画质音质低容量的MP4的话，首先画面最好不要用固定比特率，而用VBR参数让程序自己去 
判断，而音质参数可以在原来的基础上提升一点，听起来要舒服很多，也不会太大（看情况调整 


例子：ffmpeg -y -i "1.avi" -title "Test" -vcodec xvid -s 368x208 -r 29.97 -b 1500 -acodec aac -ac 2 -ar 24000 -ab 128 -vol 200 -f psp -muxvb 768 "1.***" 

解释：以上命令可以在Dos命令行中输入，也可以创建到批处理文件中运行。不过，前提是：要在ffmpeg所在的目录中执行（转换君所在目录下面的cores子目录）。 
参数： 
-y（覆盖输出文件，即如果1.***文件已经存在的话，不经提示就覆盖掉了） 
-i "1.avi"（输入文件是和ffmpeg在同一目录下的1.avi文件，可以自己加路径，改名字） 
-title "Test"（在PSP中显示的影片的标题） 
-vcodec xvid（使用XVID编码压缩视频，不能改的） 
-s 368x208（输出的分辨率为368x208，注意片源一定要是16:9的不然会变形） 
-r 29.97（帧数，一般就用这个吧） 
-b 1500（视频数据流量，用-b xxxx的指令则使用固定码率，数字随便改，1500以上没效果；还可以用动态码率如：-qscale 4和-qscale 6，4的质量比6高） 
-acodec aac（音频编码用AAC） 
-ac 2（声道数1或2） 
-ar 24000（声音的采样频率，好像PSP只能支持24000Hz） 
-ab 128（音频数据流量，一般选择32、64、96、128） 
-vol 200（200%的音量，自己改） 
-f psp（输出psp专用格式） 
-muxvb 768（好像是给PSP机器识别的码率，一般选择384、512和768，我改成1500，PSP就说文件损坏了） 
"1.***"（输出文件名，也可以加路径改文件名） 

机器强劲的话，可以多开几个批处理文件，让它们并行处理。 
E:\ffmpeg.exe -i I:\1.wmv -b 360 -r 25 -s 320x240 -hq -deinterlace -ab 56 -ar 22050 -ac 1 D:\2.flv 
===========================================
ffmpeg.exe -i F:\闪客之家\闪客之歌.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\11.flv ffmpeg -i F:\01.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 使用-ss参数 作用（time_off set the start time offset），可以从指定时间点开始转换任务。如:
转换文件格式的同时抓缩微图：
ffmpeg -i "test.avi" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg'
对已有flv抓图：
ffmpeg -i "test.flv" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg'
-ss后跟的时间单位为秒 Ffmpeg转换命令
ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 -acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp
或者
ffmpeg -y -i test.mpeg -ac 1 -acodec amr_nb -ar 8000 -s 176x144 -b 128 -r 15 test.3gp ffmpeg参数设定解说 
-bitexact 使用标准比特率 
-vcodec xvid 使用xvid压缩 
-s 320x240 指定分辨率 
-r 29.97 桢速率（可以改，确认非标准桢率会导致音画不同步，所以只能设定为15或者29.97） 


画面部分，选其一 
-b <比特率> 指定压缩比特率，似乎ffmpeg是自动VBR的，指定了就大概是平均比特率，比如768，1500这样的就是原来默认项目中有的 
-qscale <数值> 以<数值>质量为基础的VBR，取值0.01-255，约小质量越好 
-qmin <数值> 设定最小质量，与-qmax（设定最大质量）共用，比如-qmin 10 -qmax 31 
-sameq 使用和源同样的质量 声音部分 
-acodec aac 设定声音编码 
-ac <数值> 设定声道数，1就是单声道，2就是立体声，转换单声道的TVrip可以用1（节省一半容量），高品质的DVDrip就可以用2 
-ar <采样率> 设定声音采样率，PSP只认24000 
-ab <比特率> 设定声音比特率，前面-ac设为立体声时要以一半比特率来设置，比如192kbps的就设成96，转换君默认比特率都较小，要听到较高品质声音的话建议设到160kbps（80）以上 
-vol <百分比> 设定音量，某些DVDrip的AC3轨音量极小，转换时可以用这个提高音量，比如200就是原来的2倍 这样，要得到一个高画质音质低容量的MP4的话，首先画面最好不要用固定比特率，而用VBR参数让程序自己去判断，而音质参数可以在原来的基础上提升一点，听起来要舒服很多，也不会太大（看情况调整 例子：ffmpeg -y -i "1.avi" -title "Test" -vcodec xvid -s 368x208 -r 29.97 -b 1500 -acodec aac -ac 2 -ar 24000 -ab 128 -vol 200 -f psp -muxvb 768 "1.***"

解释：以上命令可以在Dos命令行中输入，也可以创建到批处理文件中运行。不过，前提是：要在ffmpeg所在的目录中执行（转换君所在目录下面的cores子目录）。
参数：
-y（覆盖输出文件，即如果1.***文件已经存在的话，不经提示就覆盖掉了）
-i "1.avi"（输入文件是和ffmpeg在同一目录下的1.avi文件，可以自己加路径，改名字）
-title "Test"（在PSP中显示的影片的标题）
-vcodec xvid（使用XVID编码压缩视频，不能改的）
-s 368x208（输出的分辨率为368x208，注意片源一定要是16:9的不然会变形）
-r 29.97（帧数，一般就用这个吧）
-b 1500（视频数据流量，用-b xxxx的指令则使用固定码率，数字随便改，1500以上没效果；还可以用动态码率如：-qscale 4和-qscale 6，4的质量比6高）
-acodec aac（音频编码用AAC）
-ac 2（声道数1或2）
-ar 24000（声音的采样频率，好像PSP只能支持24000Hz）
-ab 128（音频数据流量，一般选择32、64、96、128）
-vol 200（200%的音量，自己改）
-f psp（输出psp专用格式）
-muxvb 768（好像是给PSP机器识别的码率，一般选择384、512和768，我改成1500，PSP就说文件损坏了）
"1.***"（输出文件名，也可以加路径改文件名）

P.S. 版主机器强劲的话，可以多开几个批处理文件，让它们并行处理。 E:\ffmpeg.exe -i I:\1.wmv -b 360 -r 25 -s 320x240 -hq -deinterlace -ab 56 -ar 22050 -ac

============================================================================

part 3:

 

 

 

在cmd中先将目录切换到ffmpeg.exe对应的文件目录： cd D:\study\ffmpeg\ffmpeg-20160910-09317e3-win64-static\bin

**注意：**-命令名；前面要加ffmpeg。eg: 查看版本信息：ffmpeg -version

\1. ffmpeg 是什么？

ffmpeg(命令行工具) 是一个快速的音视频转换工具。

\2. ffmpeg 的使用方式

命令行： ffmpeg [全局选项] {[输入文件选项] -i ‘输入文件’} ... {[输出文件选项] ‘输出文件’} ...

\3. ffmpeg 可使用的选项

3.1 通用选项

ff* 工具的通用选项是共享的。(FFplayFFprobe)

```cs
‘-L’	显示 license



‘-h, -?, -help, --help [arg]’ 打印帮助信息；可以指定一个参数 arg ，如果不指定，只打印基本选项



  可选的 arg 选项：



  ‘long’	除基本选项外，还将打印高级选项



  ‘full’	打印一个完整的选项列表，包含 encoders, decoders, demuxers, muxers, filters 等的共享以及私有选项



  ‘decoder=decoder_name’	打印名称为 “decoder_name” 的解码器的详细信息



  ‘encoder=encoder_name’	打印名称为 “encoder_name” 的编码器的详细信息



  ‘demuxer=demuxer_name’	打印名称为 “demuxer_name” 的 demuxer 的详细信息



  ‘muxer=muxer_name’		打印名称为 “muxer_name” 的 muxer 的详细信息



  ‘filter=filter_name’	  打印名称为 “filter_name” 的过滤器的详细信息



      



‘-version’	 显示版本信息



‘-formats’	 显示有效的格式



‘-codecs’	  显示 libavcodec 已知的所有编解码器



‘-decoders’	显示有效的解码器



‘-encoders’	显示有效的编码器



‘-bsfs’		显示有效的比特流过滤器



‘-protocols’   显示有效的协议



‘-filters’	 显示 libavfilter 有效的过滤器



‘-pix_fmts’	显示有效的像素格式 



‘-sample_fmts’ 显示有效的采样格式



‘-layouts’	 显示通道名称以及标准通道布局



‘-colors’	  显示认可的颜色名称



‘-hide_banner’ 禁止打印欢迎语；也就是禁止默认会显示的版权信息、编译选项以及库版本信息等arg]’ 打印帮助信息；可以指定一个参数 arg ，如果不指定，只打印基本选项
  可选的 arg 选项：
  ‘long’	除基本选项外，还将打印高级选项
  ‘full’	打印一个完整的选项列表，包含 encoders, decoders, demuxers, muxers, filters 等的共享以及私有选项
  ‘decoder=decoder_name’	打印名称为 “decoder_name” 的解码器的详细信息
  ‘encoder=encoder_name’	打印名称为 “encoder_name” 的编码器的详细信息
  ‘demuxer=demuxer_name’	打印名称为 “demuxer_name” 的 demuxer 的详细信息
  ‘muxer=muxer_name’		打印名称为 “muxer_name” 的 muxer 的详细信息
  ‘filter=filter_name’	  打印名称为 “filter_name” 的过滤器的详细信息
      
‘-version’	 显示版本信息
‘-formats’	 显示有效的格式
‘-codecs’	  显示 libavcodec 已知的所有编解码器
‘-decoders’	显示有效的解码器
‘-encoders’	显示有效的编码器
‘-bsfs’		显示有效的比特流过滤器
‘-protocols’   显示有效的协议
‘-filters’	 显示 libavfilter 有效的过滤器
‘-pix_fmts’	显示有效的像素格式 
‘-sample_fmts’ 显示有效的采样格式
‘-layouts’	 显示通道名称以及标准通道布局
‘-colors’	  显示认可的颜色名称
‘-hide_banner’ 禁止打印欢迎语；也就是禁止默认会显示的版权信息、编译选项以及库版本信息等
```

3.2 一些主要选项

```prettyprint
‘-f fmt (input/output)’ 
  强制输入或输出文件格式。通常，输入文件的格式是自动检测的，
  输出文件的格式是通过文件扩展名来进行猜测的，所有该选项大
  多数时候不需要。
‘-i filename (input)’ 
  输入文件名
‘-y (global)’ 
  覆盖输出文件而不询问
‘-n (global)’ 
  不覆盖输出文件，如果一个给定的输出文件已经存在，则立即
  退出
‘-c[:stream_specifier] codec (input/output,per-stream)’
‘-codec[:stream_specifier] codec (input/output,per-stream)’
  为一个或多个流选择一个编码器(当使用在一个输出文件之前时)
  或者一个解码器(当使用在一个输入文件之前时)。codec 是一个
  编码器/解码器名称或者一个特定值“copy”(只适用输出)。
‘-t duration (output)’ 
  当到达 duration 时，停止写输出。
  duration 可以是一个数字(秒)，或者使用hh:mm:ss[.xxx]形式。
  -to 和 -t 是互斥的，-t 优先级更高。
‘-to position (output)’ 
  在 position 处停止写输出。
  duration 可以是一个数字(秒)，或者使用hh:mm:ss[.xxx]形式。
  -to 和 -t 是互斥的，-t 优先级更高。
‘-fs limit_size (output)’
  设置文件大小限制，以字节表示
‘-ss position (input/output)’
  当作为输入选项时(在 -i 之前)，在输入文件中跳转到 position。
  需要注意的是，在大多数格式中，不太可能精确的跳转，因此，
  ffmpeg 将跳转到 position 之前最接近的位置。当进行转码
  并且 ‘-accurate_seek’ 打开时(默认)，位于跳转点和 position 
  之间的额外部分将被解码并且丢弃。当做流拷贝或者当使用
  ‘-noaccurate_seek’时，它将被保留下来。
  当作为输出选项时(在输出文件名前)，解码但是丢弃输入，直到
  时间戳到达 position。
  position 可以是秒或者 hh:mm:ss[.xxx] 形式
‘-itsoffset offset (input)’
  设置输入时间偏移。 offset 将被添加到输入文件的时间戳。指定
  一个正偏移，意味着相应的流将被延时指定时间。
‘-timestamp date (output)’
  在容器中设置录音时间戳
‘-metadata[:metadata_specifier] key=value (output,per-metadata)’
  设置metadata key/value对
‘-target type (output)’
  指定目标文件类型(vcd, svcd, dvd, dv, dv50)。
  type 可以带有 pal-, ntsc- 或 film- 前缀，以使用相应的标准。
  所有的格式选项(bitrate, codecs, buffer sizes)将自动设定。
‘-dframes number (output)’
  设置要录制数据帧的个数。这是 -frames:d 的别名
‘-frames[:stream_specifier] framecount (output,per-stream)’  
  framecount 帧以后，停止写流。
‘-q[:stream_specifier] q (output,per-stream)’
‘-qscale[:stream_specifier] q (output,per-stream)’ 
  使用固定质量范围(VBR)。
‘-filter[:stream_specifier] filtergraph (output,per-stream)’
  创建filtergraph 指定的过滤图，并使用它来过滤流。
‘-filter_script[:stream_specifier] filename (output,per-stream)’
  该选项与‘-filter’相似，唯一的不同是，它的参数是一个存放
  过滤图的文件的名称。
‘-pre[:stream_specifier] preset_name (output,per-stream)’ 
  指定匹配流的预设
‘-stats (global)’
  打印编码进程/统计信息。默认打开，可以使用 -nostats 禁用。
‘-stdin’ 
  开启标准输入交互。默认打开，除非标准输入作为一个输入。
  可以使用 -nostdin 禁止。
‘-debug_ts (global)’
  打印时间戳信息。默认关闭。
‘-attach filename (output)’
  添加一个附件到输出文件中
‘-dump_attachment[:stream_specifier] filename (input,per-stream)’ 
  提取匹配的附件流到filename指定的文件中。fmt (input/output)’ 
  强制输入或输出文件格式。通常，输入文件的格式是自动检测的，
  输出文件的格式是通过文件扩展名来进行猜测的，所有该选项大
  多数时候不需要。
‘-i filename (input)’ 
  输入文件名
‘-y (global)’ 
  覆盖输出文件而不询问
‘-n (global)’ 
  不覆盖输出文件，如果一个给定的输出文件已经存在，则立即
  退出
‘-c[:stream_specifier] codec (input/output,per-stream)’
‘-codec[:stream_specifier] codec (input/output,per-stream)’
  为一个或多个流选择一个编码器(当使用在一个输出文件之前时)
  或者一个解码器(当使用在一个输入文件之前时)。codec 是一个
  编码器/解码器名称或者一个特定值“copy”(只适用输出)。
‘-t duration (output)’ 
  当到达 duration 时，停止写输出。
  duration 可以是一个数字(秒)，或者使用hh:mm:ss[.xxx]形式。
  -to 和 -t 是互斥的，-t 优先级更高。
‘-to position (output)’ 
  在 position 处停止写输出。
  duration 可以是一个数字(秒)，或者使用hh:mm:ss[.xxx]形式。
  -to 和 -t 是互斥的，-t 优先级更高。
‘-fs limit_size (output)’
  设置文件大小限制，以字节表示
‘-ss position (input/output)’
  当作为输入选项时(在 -i 之前)，在输入文件中跳转到 position。
  需要注意的是，在大多数格式中，不太可能精确的跳转，因此，
  ffmpeg 将跳转到 position 之前最接近的位置。当进行转码
  并且 ‘-accurate_seek’ 打开时(默认)，位于跳转点和 position 
  之间的额外部分将被解码并且丢弃。当做流拷贝或者当使用
  ‘-noaccurate_seek’时，它将被保留下来。
  当作为输出选项时(在输出文件名前)，解码但是丢弃输入，直到
  时间戳到达 position。
  position 可以是秒或者 hh:mm:ss[.xxx] 形式
‘-itsoffset offset (input)’
  设置输入时间偏移。 offset 将被添加到输入文件的时间戳。指定
  一个正偏移，意味着相应的流将被延时指定时间。
‘-timestamp date (output)’
  在容器中设置录音时间戳
‘-metadata[:metadata_specifier] key=value (output,per-metadata)’
  设置metadata key/value对
‘-target type (output)’
  指定目标文件类型(vcd, svcd, dvd, dv, dv50)。
  type 可以带有 pal-, ntsc- 或 film- 前缀，以使用相应的标准。
  所有的格式选项(bitrate, codecs, buffer sizes)将自动设定。
‘-dframes number (output)’
  设置要录制数据帧的个数。这是 -frames:d 的别名
‘-frames[:stream_specifier] framecount (output,per-stream)’  
  framecount 帧以后，停止写流。
‘-q[:stream_specifier] q (output,per-stream)’
‘-qscale[:stream_specifier] q (output,per-stream)’ 
  使用固定质量范围(VBR)。
‘-filter[:stream_specifier] filtergraph (output,per-stream)’
  创建filtergraph 指定的过滤图，并使用它来过滤流。
‘-filter_script[:stream_specifier] filename (output,per-stream)’
  该选项与‘-filter’相似，唯一的不同是，它的参数是一个存放
  过滤图的文件的名称。
‘-pre[:stream_specifier] preset_name (output,per-stream)’ 
  指定匹配流的预设
‘-stats (global)’
  打印编码进程/统计信息。默认打开，可以使用 -nostats 禁用。
‘-stdin’ 
  开启标准输入交互。默认打开，除非标准输入作为一个输入。
  可以使用 -nostdin 禁止。
‘-debug_ts (global)’
  打印时间戳信息。默认关闭。
‘-attach filename (output)’
  添加一个附件到输出文件中
‘-dump_attachment[:stream_specifier] filename (input,per-stream)’ 
  提取匹配的附件流到filename指定的文件中。
```

3.3 视频选项

```prettyprint
‘-vframes number (output)’
  设置录制视频帧的个数。这是 -frames:v 的别名
‘-r[:stream_specifier] fps (input/output,per-stream)’
  设置帧率(Hz 值， 分数或缩写)
‘-s[:stream_specifier] size (input/output,per-stream)’
  设置帧大小。格式为 ‘wxh’ (默认与源相同)
‘-aspect[:stream_specifier] aspect (output,per-stream)’
  设置视频显示长宽比
‘-vn (output)’
  禁止视频录制
‘-vcodec codec (output)’
  设置视频 codec。这是 -codec:v 的别名
‘-pass[:stream_specifier] n (output,per-stream)’
  选择pass number (1 or 2)。用来进行双行程视频编码。
‘-passlogfile[:stream_specifier] prefix (output,per-stream)’
  设置 two-pass 日志文件名前缀，默认为“ffmpeg2pass”。
‘-vf filtergraph (output)’
  创建 filtergraph 指定的过滤图，并使用它来过滤流。number (output)’
  设置录制视频帧的个数。这是 -frames:v 的别名
‘-r[:stream_specifier] fps (input/output,per-stream)’
  设置帧率(Hz 值， 分数或缩写)
‘-s[:stream_specifier] size (input/output,per-stream)’
  设置帧大小。格式为 ‘wxh’ (默认与源相同)
‘-aspect[:stream_specifier] aspect (output,per-stream)’
  设置视频显示长宽比
‘-vn (output)’
  禁止视频录制
‘-vcodec codec (output)’
  设置视频 codec。这是 -codec:v 的别名
‘-pass[:stream_specifier] n (output,per-stream)’
  选择pass number (1 or 2)。用来进行双行程视频编码。
‘-passlogfile[:stream_specifier] prefix (output,per-stream)’
  设置 two-pass 日志文件名前缀，默认为“ffmpeg2pass”。
‘-vf filtergraph (output)’
  创建 filtergraph 指定的过滤图，并使用它来过滤流。
```

3.4 高级视频选项

```prettyprint
‘-pix_fmt[:stream_specifier] format (input/output,per-stream)’
  设置像素格式。
‘-sws_flags flags (input/output)’
  设置软缩放标志
‘-vdt n’
  丢弃阈值
‘-psnr’
  计算压缩帧的 PSNR 
‘-vstats’
  复制视频编码统计信息到‘vstats_HHMMSS.log’
‘-vstats_file file’
  复制视频编码统计信息到 file
‘-force_key_frames[:stream_specifier] time[,time...] (output,per-stream)’
‘-force_key_frames[:stream_specifier] expr:expr (output,per-stream)’
  在指定的时间戳强制关键帧
‘-copyinkf[:stream_specifier] (output,per-stream)’
  当进行流拷贝时，同时拷贝开头的非关键帧
‘-hwaccel[:stream_specifier] hwaccel (input,per-stream)’
  使用硬件加速来解码匹配的流
‘-hwaccel_device[:stream_specifier] hwaccel_device (input,per-stream)’
  选择硬件加速所使用的设备。该选项只有‘-hwaccel’同时指定时才有意义。:stream_specifier] format (input/output,per-stream)’
  设置像素格式。
‘-sws_flags flags (input/output)’
  设置软缩放标志
‘-vdt n’
  丢弃阈值
‘-psnr’
  计算压缩帧的 PSNR 
‘-vstats’
  复制视频编码统计信息到‘vstats_HHMMSS.log’
‘-vstats_file file’
  复制视频编码统计信息到 file
‘-force_key_frames[:stream_specifier] time[,time...] (output,per-stream)’
‘-force_key_frames[:stream_specifier] expr:expr (output,per-stream)’
  在指定的时间戳强制关键帧
‘-copyinkf[:stream_specifier] (output,per-stream)’
  当进行流拷贝时，同时拷贝开头的非关键帧
‘-hwaccel[:stream_specifier] hwaccel (input,per-stream)’
  使用硬件加速来解码匹配的流
‘-hwaccel_device[:stream_specifier] hwaccel_device (input,per-stream)’
  选择硬件加速所使用的设备。该选项只有‘-hwaccel’同时指定时才有意义。
```

3.5 音频选项

```perl
‘-aframes number (output)’



  设置录制音频帧的个数。这是 -frames:a 的别名



‘-ar[:stream_specifier] freq (input/output,per-stream)’



  设置音频采样率。



‘-aq q (output)’



  设置音频质量。这是 -q:a 的别名



‘-ac[:stream_specifier] channels (input/output,per-stream)’



  设置音频通道数。



‘-an (output)’



  禁止音频录制



‘-acodec codec (input/output)’



  设置音频codec。这是-codec:a的别名



‘-sample_fmt[:stream_specifier] sample_fmt (output,per-stream)’



  设置音频采样格式



‘-af filtergraph (output)’



  创建filtergraph 所指定的过滤图，并使用它来过滤流number (output)’
  设置录制音频帧的个数。这是 -frames:a 的别名
‘-ar[:stream_specifier] freq (input/output,per-stream)’
  设置音频采样率。
‘-aq q (output)’
  设置音频质量。这是 -q:a 的别名
‘-ac[:stream_specifier] channels (input/output,per-stream)’
  设置音频通道数。
‘-an (output)’
  禁止音频录制
‘-acodec codec (input/output)’
  设置音频codec。这是-codec:a的别名
‘-sample_fmt[:stream_specifier] sample_fmt (output,per-stream)’
  设置音频采样格式
‘-af filtergraph (output)’
  创建filtergraph 所指定的过滤图，并使用它来过滤流
```

3.6 字幕选项

```prettyprint
‘-scodec codec (input/output)’
    设置字幕codec。这是 -codec:s 的别名
‘-sn (output)’
     禁止字幕录制codec (input/output)’
    设置字幕codec。这是 -codec:s 的别名
‘-sn (output)’
     禁止字幕录制
```

3.7 高级选项

```prettyprint
‘-map [-]input_file_id[:stream_specifier][,sync_file_id[:stream_specifier]] | [linklabel] (output)’
  指定一个或多个流作为输出文件的源。
  命令行中的第一个 -map 选项，指定输出流0的源，
  第二个 -map 选项，指定输出流1的源，等等。
‘-map_channel [input_file_id.stream_specifier.channel_id|-1][:output_file_id.stream_specifier]’
  将一个给定输入的音频通道映射到一个输出。
‘-map_metadata[:metadata_spec_out] infile[:metadata_spec_in] (output,per-metadata)’
  设置下一个输出文件的 metadata 信息。
‘-map_chapters input_file_index (output)’
  从索引号为 input_file_index 的输入文件中拷贝章节到下一个输出文件中。
‘-timelimit duration (global)’
  ffmpeg 运行 duration 秒后推出
‘-dump (global)’
  将每一个输入包复制到标准输出
‘-hex (global)’
  复制包时，同时复制负载
‘-re (input)’
  以本地帧率读取数据。主要用来模拟一个采集设备，
  或者实时输入流(例如：当从一个文件读取时).
‘-vsync parameter’
  视频同步方法
‘-async samples_per_second’
  音频同步方法
‘-shortest (output)’
  当最短的输入流结束时，终止编码
‘-muxdelay seconds (input)’
  设置最大解封装-解码延时
‘-muxpreload seconds (input)’
  设置初始解封装-解码延时
‘-streamid output-stream-index:new-value (output)’
  为一个输出流分配一个新的stream-id。
‘-bsf[:stream_specifier] bitstream_filters (output,per-stream)’
  为匹配的流设置比特流过滤器
‘-filter_complex filtergraph (global)’
  定义一个复杂的过滤图
‘-lavfi filtergraph (global)’
  定义一个复杂的过滤图。相当于‘-filter_complex’
‘-filter_complex_script filename (global)’
   该选项类似于‘-filter_complex’，唯一的不同是
   它的参数是一个定义过滤图的文件的文件名
‘-accurate_seek (input)’
   打开或禁止在输入文件中的精确跳转。默认打开。

更详细的高级选项说明，请查阅 ffmpeg 文档input_file_id[:stream_specifier][,sync_file_id[:stream_specifier]] | [linklabel] (output)’
  指定一个或多个流作为输出文件的源。
  命令行中的第一个 -map 选项，指定输出流0的源，
  第二个 -map 选项，指定输出流1的源，等等。
‘-map_channel [input_file_id.stream_specifier.channel_id|-1][:output_file_id.stream_specifier]’
  将一个给定输入的音频通道映射到一个输出。
‘-map_metadata[:metadata_spec_out] infile[:metadata_spec_in] (output,per-metadata)’
  设置下一个输出文件的 metadata 信息。
‘-map_chapters input_file_index (output)’
  从索引号为 input_file_index 的输入文件中拷贝章节到下一个输出文件中。
‘-timelimit duration (global)’
  ffmpeg 运行 duration 秒后推出
‘-dump (global)’
  将每一个输入包复制到标准输出
‘-hex (global)’
  复制包时，同时复制负载
‘-re (input)’
  以本地帧率读取数据。主要用来模拟一个采集设备，
  或者实时输入流(例如：当从一个文件读取时).
‘-vsync parameter’
  视频同步方法
‘-async samples_per_second’
  音频同步方法
‘-shortest (output)’
  当最短的输入流结束时，终止编码
‘-muxdelay seconds (input)’
  设置最大解封装-解码延时
‘-muxpreload seconds (input)’
  设置初始解封装-解码延时
‘-streamid output-stream-index:new-value (output)’
  为一个输出流分配一个新的stream-id。
‘-bsf[:stream_specifier] bitstream_filters (output,per-stream)’
  为匹配的流设置比特流过滤器
‘-filter_complex filtergraph (global)’
  定义一个复杂的过滤图
‘-lavfi filtergraph (global)’
  定义一个复杂的过滤图。相当于‘-filter_complex’
‘-filter_complex_script filename (global)’
   该选项类似于‘-filter_complex’，唯一的不同是
   它的参数是一个定义过滤图的文件的文件名
‘-accurate_seek (input)’
   打开或禁止在输入文件中的精确跳转。默认打开。

更详细的高级选项说明，请查阅 ffmpeg 文档
```

\4. ffmpeg 使用示例

```perl
1) 输出 YUV420P 原始数据



ffmpeg -i test.mp4 test.yuv



 



2) 从视频前10s中提取图像，1s提取一帧



ffmpeg -i test.mp4 -t 10 -r 1 -f image2 pic-%03d.jpeg



 



3) 从视频中提取音频



ffmpeg -i test.mp4 -vn music.mp2



 



4) 转换 wav 格式，同时改变其采样率



ffmpeg -i test.wav -ar 32K test.mp2



 



5) 输出 mp3 原始 PCM 数据



ffmpeg -i test.mp3 -f s16le test.pcm) 输出 YUV420P 原始数据
ffmpeg -i test.mp4 test.yuv

2) 从视频前10s中提取图像，1s提取一帧
ffmpeg -i test.mp4 -t 10 -r 1 -f image2 pic-%03d.jpeg

3) 从视频中提取音频
ffmpeg -i test.mp4 -vn music.mp2

4) 转换 wav 格式，同时改变其采样率
ffmpeg -i test.wav -ar 32K test.mp2

5) 输出 mp3 原始 PCM 数据
ffmpeg -i test.mp3 -f s16le test.pcm
<span style="color:#0000ff">



</span>
================================================================================================
part 4:
 
```

使用网络上的各种转码软件，比如爱剪辑，简直崩溃了！比如，它竟然强行把你要编辑的视频的前面，插入了他的广告！很不爽，决定用ffmpeg。 ffmpeg做IT media的都知道，很强大，只不多对windows用户比较麻烦，用命令行不方便。好吧，只能说，这篇文章估计只有小众朋友看的。呜呜

废话不说，直接贴出来吧。 贴出来之前，先mark一下 **ffmpeg的使用方式**： `ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...`

**常用参数说明： 主要参数：** -i 设定输入流 -f 设定输出格式 -ss 开始时间 **视频参数：** -b 设定视频流量，默认为200Kbit/s -r 设定帧速率，默认为25 -s 设定画面的宽与高 -aspect 设定画面的比例 -vn 不处理视频 -vcodec 设定视频编解码器，未设定时则使用与输入流相同的编解码器 **音频参数：** -ar 设定采样率 -ac 设定声音的Channel数 -acodec 设定声音编解码器，未设定时则使用与输入流相同的编解码器 -an 不处理音频

## 1. 视频格式转换

**（其实格式转换说法不太准确，但大家都这么叫，准确的说，应该是视频容器转换）** 比如一个avi文件，想转为mp4，或者一个mp4想转为ts。 `ffmpeg -i input.avi output.mp4` `ffmpeg -i input.mp4 output.ts` 我目测这个已经能满足很多人的需求了。

## 2. 提取音频

比如我有一个“晓松奇谈”，可是我不想看到他的脸，我只想听声音， 地铁上可以听，咋办？ `ffmpeg -i 晓松奇谈.mp4 -acodec copy -vn output.aac` 上面的命令，默认mp4的audio codec是aac，如果不是会出错，咱可以暴力一点，不管什么音频，都转为最常见的aac。 `ffmpeg -i 晓松奇谈.mp4 -acodec aac -vn output.aac`

## 3. 提取视频

我目测有些IT员工，特别是做嵌入式的，比如机顶盒，想debug一下，没有音频的情况下，播放一个视频几天几夜会不会crash，这时候你需要一个纯视频文件，可以这么干。 `ffmpeg -i input.mp4 -vcodec copy -an output.mp4`

## 4. 视频剪切

经常要测试视频，但是只需要测几秒钟，可是视频却有几个G，咋办？切啊！ 下面的命令，就可以从时间为00:00:15开始，截取5秒钟的视频。 `ffmpeg -ss 00:00:15 -t 00:00:05 -i input.mp4 -vcodec copy -acodec copy output.mp4` -ss表示开始切割的时间，-t表示要切多少。上面就是从开始，切5秒钟出来。

## 5. 码率控制

码率控制对于在线视频比较重要。因为在线视频需要考虑其能提供的带宽。

那么，什么是码率？很简单： bitrate = file size / duration 比如一个文件20.8M，时长1分钟，那么，码率就是： biterate = 20.8M bit/60s = 20.8*1024*1024*8 bit/60s= 2831Kbps 一般音频的码率只有固定几种，比如是128Kbps， 那么，video的就是 video biterate = 2831Kbps -128Kbps = 2703Kbps。

说完背景了。好了，来说ffmpeg如何控制码率。 ffmpg控制码率有3种选择，-minrate -b:v -maxrate -b:v主要是控制平均码率。 比如一个视频源的码率太高了，有10Mbps，文件太大，想把文件弄小一点，但是又不破坏分辨率。 `ffmpeg -i input.mp4 -b:v 2000k output.mp4` 上面把码率从原码率转成2Mbps码率，这样其实也间接让文件变小了。目测接近一半。 不过，ffmpeg官方wiki比较建议，设置b:v时，同时加上 -bufsize -bufsize 用于设置码率控制缓冲器的大小，设置的好处是，让整体的码率更趋近于希望的值，减少波动。（简单来说，比如1 2的平均值是1.5， 1.49 1.51 也是1.5, 当然是第二种比较好） `ffmpeg -i input.mp4 -b:v 2000k -bufsize 2000k output.mp4`

-minrate -maxrate就简单了，在线视频有时候，希望码率波动，不要超过一个阈值，可以设置maxrate。 `ffmpeg -i input.mp4 -b:v 2000k -bufsize 2000k -maxrate 2500k output.mp4`

## 6. 视频编码格式转换

比如一个视频的编码是MPEG4，想用H264编码，咋办？ `ffmpeg -i input.mp4 -vcodec h264 output.mp4` 相反也一样 `ffmpeg -i input.mp4 -vcodec mpeg4 output.mp4`

当然了，如果ffmpeg当时编译时，添加了外部的x265或者X264，那也可以用外部的编码器来编码。（不知道什么是X265，可以Google一下，简单的说，就是她不包含在ffmpeg的源码里，是独立的一个开源代码，用于编码HEVC，ffmpeg编码时可以调用它。当然了，ffmpeg自己也有编码器） `ffmpeg -i input.mp4 -c:v libx265 output.mp4` `ffmpeg -i input.mp4 -c:v libx264 output.mp4`

## 7. 只提取视频ES数据

这个可能做开发的人会用到，顺便提一下吧。 `ffmpeg –i input.mp4 –vcodec copy –an –f m4v output.h264`

## 8. 过滤器的使用

这个我在另一篇博客提到了，这里贴一下吧。

### 8.1 将输入的1920x1080缩小到960x540输出:

`ffmpeg -i input.mp4 -vf scale=960:540 output.mp4` //ps: 如果540不写，写成-1，即scale=960:-1, 那也是可以的，ffmpeg会通知缩放滤镜在输出时保持原始的宽高比。

### 8.2 为视频添加logo

比如，我有这么一个图片 ![iqiyi logo](https://img-blog.csdn.net/20160512155254687) 想要贴到一个视频上，那可以用如下命令： ./ffmpeg -i input.mp4 -i iQIYI_logo.png -filter_complex overlay output.mp4 结果如下所示： ![add logo left](https://img-blog.csdn.net/20160512155411797) 要贴到其他地方？看下面： 右上角： ./ffmpeg -i input.mp4 -i logo.png -filter_complex overlay=W-w output.mp4 左下角： ./ffmpeg -i input.mp4 -i logo.png -filter_complex overlay=0:H-h output.mp4 右下角： ./ffmpeg -i input.mp4 -i logo.png -filter_complex overlay=W-w:H-h output.mp4

### 8.3 去掉视频的logo

有时候，下载了某个网站的视频，但是有logo很烦，咋办？有办法，用ffmpeg的delogo过滤器。 语法：-vf delogo=x:y:w:h[:t[:show]] x:y 离左上角的坐标 w:h logo的宽和高 t: 矩形边缘的厚度默认值4 show：若设置为1有一个绿色的矩形，默认值0。

`ffmpeg -i input.mp4 -vf delogo=0:0:220:90:100:1 output.mp4` 结果如下所示： ![de logo](https://img-blog.csdn.net/20160512155451204)

## 9. 抓取视频的一些帧，存为jpeg图片

比如，一个视频，我想提取一些帧，存为图片，咋办？ `ffmpeg -i input.mp4 -r 1 -q:v 2 -f image2 pic-%03d.jpeg` -r 表示每一秒几帧 -q:v表示存储jpeg的图像质量，一般2是高质量。 如此，ffmpeg会把input.mp4，每隔一秒，存一张图片下来。假设有60s，那会有60张。

60张？什么？这么多？不要不要。。。。。不要咋办？？ 可以设置开始的时间，和你想要截取的时间呀。 `ffmpeg -i input.mp4 -ss 00:00:20 -t 10 -r 1 -q:v 2 -f image2 pic-%03d.jpeg` -ss 表示开始时间 -t表示共要多少时间。 如此，ffmpeg会从input.mp4的第20s时间开始，往下10s，即20~30s这10秒钟之间，每隔1s就抓一帧，总共会抓10帧。

怎么样，好用吧。^^

------

# 其他小众的用法

## 1.输出YUV420原始数据

对于一下做底层编解码的人来说，有时候常要提取视频的YUV原始数据。 怎么坐？很简答： `ffmpeg -i input.mp4 output.yuv` 怎么样，是不是太简单啦？！！！哈哈

如果你想问yuv的数据，如何播放，我不会告诉你，RawPlayer挺好用的！！

**那如果我只想要抽取某一帧YUV呢？** 简单，你先用上面的方法，先抽出jpeg图片，然后把jpeg转为YUV。 比如： 你先抽取10帧图片。 `ffmpeg -i input.mp4 -ss 00:00:20 -t 10 -r 1 -q:v 2 -f image2 pic-%03d.jpeg`结果：

```
-rw-rw-r-- 1 chenxf chenxf    296254  7月 20 16:08 pic-001.jpeg



-rw-rw-r-- 1 chenxf chenxf    300975  7月 20 16:08 pic-002.jpeg



-rw-rw-r-- 1 chenxf chenxf    310130  7月 20 16:08 pic-003.jpeg



-rw-rw-r-- 1 chenxf chenxf    268694  7月 20 16:08 pic-004.jpeg



-rw-rw-r-- 1 chenxf chenxf    301056  7月 20 16:08 pic-005.jpeg



-rw-rw-r-- 1 chenxf chenxf    293927  7月 20 16:08 pic-006.jpeg



-rw-rw-r-- 1 chenxf chenxf    340295  7月 20 16:08 pic-007.jpeg



-rw-rw-r-- 1 chenxf chenxf    430787  7月 20 16:08 pic-008.jpeg



-rw-rw-r-- 1 chenxf chenxf    404552  7月 20 16:08 pic-009.jpeg



-rw-rw-r-- 1 chenxf chenxf    412691  7月 20 16:08 pic-010.jpeg
```

- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10

然后，你就随便挑一张，转为YUV:
`ffmpeg -i pic-001.jpeg -s 1440x1440 -pix_fmt yuv420p xxx3.yuv`
如果-s参数不写，则输出大小与输入一样。

当然了，YUV还有yuv422p啥的，你在-pix_fmt 换成yuv422p就行啦！

## 2. H264编码profile & level控制

### 背景知识

先科普一下profile&level吧，知道的请飘过。（这里讨论最常用的H264）
H.264有四种画质级别,分别是baseline, extended, main, high：
　　1、Baseline Profile：基本画质。支持I/P 帧，只支持无交错（Progressive）和CAVLC；
　　2、Extended profile：进阶画质。支持I/P/B/SP/SI 帧，只支持无交错（Progressive）和CAVLC；(用的少)
　　3、Main profile：主流画质。提供I/P/B 帧，支持无交错（Progressive）和交错（Interlaced），
　　　 也支持CAVLC 和CABAC 的支持；
　　4、High profile：高级画质。在main Profile 的基础上增加了8x8内部预测、自定义量化、 无损视频编码和更多的YUV 格式；
H.264 Baseline profile、Extended profile和Main profile都是针对8位样本数据、4:2:0格式(YUV)的视频序列。在相同配置情况下，High profile（HP）可以比Main profile（MP）降低10%的码率。
根据应用领域的不同，Baseline profile多应用于实时通信领域，Main profile多应用于流媒体领域，High profile则多应用于广电和存储领域。

下图清楚的给出不同的profile&level的性能区别。
**profile**
![这里写图片描述](https://img-blog.csdn.net/20160516164141047)

**level**
![这里写图片描述](https://img-blog.csdn.net/20160516164454535)

### 2.1 ffmpeg如何控制profile&level

举3个例子吧
`ffmpeg -i input.mp4 -profile:v baseline -level 3.0 output.mp4`

```
ffmpeg -i input.mp4 -profile:v main -level 4.2 output.mp4
ffmpeg -i input.mp4 -profile:v high -level 5.1 output.mp4
```

如果ffmpeg编译时加了external的libx264，那就这么写：
`ffmpeg -i input.mp4 -c:v libx264 -x264-params "profile=high:level=3.0" output.mp4`

从压缩比例来说，baseline< main < high，对于带宽比较局限的在线视频，可能会选择high，但有些时候，做个小视频，希望所有的设备基本都能解码（有些低端设备或早期的设备只能解码baseline），那就牺牲文件大小吧，用baseline。自己取舍吧！

苹果的设备对不同profile的支持。
![这里写图片描述](https://img-blog.csdn.net/20160516171746876)

### 2.2. 编码效率和视频质量的取舍(preset, crf)

除了上面提到的，强行配置biterate，或者强行配置profile/level，还有2个参数可以控制编码效率。
一个是preset，一个是crf。
preset也挺粗暴，基本原则就是，如果你觉得编码太快或太慢了，想改改，可以用profile。
preset有如下参数可用：

> ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow and placebo.
> 编码加快，意味着信息丢失越严重，输出图像质量越差。

CRF(Constant Rate Factor): 范围 0-51: 0是编码毫无丢失信息, 23 is 默认, 51 是最差的情况。相对合理的区间是18-28.
值越大，压缩效率越高，但也意味着信息丢失越严重，输出图像质量越差。

举个例子吧。
`ffmpeg -i input -c:v libx264 -profile:v main -preset:v fast -level 3.1 -x264opts crf=18`
(参考自：<https://trac.ffmpeg.org/wiki/Encode/H.264>)

2.3. H265 (HEVC)编码tile&level控制

背景知识

和H264的profile&level一样，为了应对不同应用的需求，HEVC制定了“层级”(tier) 和“等级”(level)。
tier只有main和high。
level有13级，如下所示：
![这里写图片描述](https://img-blog.csdn.net/20160516181155149)

不多说，直接给出怎么用。（supposed你用libx265编码）
`ffmpeg -i input.mp4 -c:v libx265 -x265-params "profile=high:level=3.0" output.mp4`

 

```
===================================================================================
part 5:
```

 

 

 

## 转flv

ffmpeg -i F:\01.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 f:\test.flv 

### 简单的转换：

ffmpeg -i IpCam.ts IpCam.flv

注意：bitrate否则转换后图像模糊：20M 合适

ffmpeg -i IpCam.ts -b 20000000  IpCam.flv

 

 

## ffmpeg 推rtmp 流

ffmpeg -re -i ~/2012.flv -f flv rtmp://192.168.1.102/myapp/test1

ffmpeg -re -i RealStream.fifo -f flv -b 20000000 rtmp://localhost/myapp/test1

 

## 信息查询

ffmpeg -formats
查看所有支持的容器格式

ffmpeg -codecs
查看所有编解码器

ffmpeg -filters
查看所有可用的filter

ffmpeg -pix_fmts
查看所有支持的图片格式


ffmpeg -sample_fmts
查看所有支持的像素格式

ffprobe -i money.mp4
查看媒体信息

 

## 格式转换

ffmpeg -i money.mp4 -c:v mpeg2video -b:v 500k -c:a libmp3lame -f mpegts money.ts
ffmpeg -i money.mp4 -c:v libx264 -minrate:v 500k -maxrate:v 500k -bufsize:v 125k -c:a libmp3lame -f mpegts money.ts
ffmpeg -i money.mp4 -c:v libx264 -x264opts bitrate=500:vbv-maxrate=500:vbv-bufsize=166:nal_hrd=cbr  -c:a libmp3lame -f mpegts money_cbr_500k.ts
-i    输入文件名
-c:v  设置视频编码器
-v:b  设置码率
-c:a  设置音频编码器
-f    设置文件格式（容器）
-minrate:v 500k -maxrate:v 500k -bufsize:v 125k  设置CBR（不太好用）

 

-x264opts bitrate=500:vbv-maxrate=500:vbv-bufsize=166:nal_hrd=cbr  设置CBR（好用）

 

 

选择其中第一个视频流输出，设置码率
ffmpeg -i money.mp4 -map 0:v:0 -c:v libx264 -b:v 500k money_500k.mp4
ffmpeg -i money.mp4 -map 0:v:0 -c:v libx264 -b:v 300k money_300k.mp4
ffmpeg -i money.mp4 -map 0:v:0 -c:v libx264 -b:v 100k -s 336x188 money_100k.mp4
选择其中第一个音频流输出
ffmpeg -i money.mp4 -map 0:a:0 money_audio.mp4

 

使用图片合成翻转的视频：

 

ffmpeg -framerate 30 -pattern_type glob -i '*.jpg' -c:v libx264 -r 30 -vf vflip,hflip out.mp4

====================================================================================

part 6:

 

 

1.分离视频音频流

```html
ffmpeg -i input_file -vcodec copy -an output_file_video　　//分离视频流



ffmpeg -i input_file -acodec copy -vn output_file_audio　　//分离音频流
```

2.视频解复用

```html
ffmpeg –i test.mp4 –vcodec copy –an –f m4v test.264



ffmpeg –i test.avi –vcodec copy –an –f m4v test.264
```

3.视频转码

```html
ffmpeg –i test.mp4 –vcodec h264 –s 352*278 –an –f m4v test.264              //转码为码流原始文件



ffmpeg –i test.mp4 –vcodec h264 –bf 0 –g 25 –s 352*278 –an –f m4v test.264  //转码为码流原始文件



ffmpeg –i test.avi -vcodec mpeg4 –vtag xvid –qsame test_xvid.avi            //转码为封装文件



//-bf B帧数目控制，-g 关键帧间隔控制，-s 分辨率控制
```

4.视频封装

```html
ffmpeg –i video_file –i audio_file –vcodec copy –acodec copy output_file
```

5.视频剪切

```html
ffmpeg –i test.avi –r 1 –f image2 image-%3d.jpeg        //提取图片



ffmpeg -ss 0:1:30 -t 0:0:20 -i input.avi -vcodec copy -acodec copy output.avi    //剪切视频



//-r 提取图像的频率，-ss 开始时间，-t 持续时间
```

6.视频录制

```html
ffmpeg –i rtsp://192.168.3.205:5555/test –vcodec copy out.avi
```

7.YUV序列播放

```html
ffplay -f rawvideo -video_size 1920x1080 input.yuv
```

8.YUV序列转AVI

```html
ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi
```

常用参数说明：

主要参数： -i 设定输入流 -f 设定输出格式 -ss 开始时间 视频参数： -b 设定视频流量，默认为200Kbit/s -r 设定帧速率，默认为25 -s 设定画面的宽与高 -aspect 设定画面的比例 -vn 不处理视频 -vcodec 设定视频编解码器，未设定时则使用与输入流相同的编解码器 音频参数： -ar 设定采样率 -ac 设定声音的Channel数 -acodec 设定声音编解码器，未设定时则使用与输入流相同的编解码器 -an 不处理音频

\------------------------------------------------------------------------

\----------------------------------------------------------------------------------------------------------

\------------------------------------------------------------------------

1、将文件当做直播送至live

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -re -i localFile.mp4 -c copy -f flv rtmp://server/live/streamName  

```go
ffmpeg -re -i localFile.mp4 -c copy -f flv rtmp://server/live/streamName
```

2、将直播媒体保存至本地文件

 

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -i rtmp://server/live/streamName -c copy dump.flv  

```java
ffmpeg -i rtmp://server/live/streamName -c copy dump.flv
```

3、将其中一个直播流，视频改用h264压缩，音频不变，送至另外一个直播服务流

 

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv rtmp://server/live/h264Stream  

```ruby
ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv rtmp://server/live/h264Stream
```

 

4、将其中一个直播流，视频改用h264压缩，音频改用faac压缩，送至另外一个直播服务流

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -i rtmp://server/live/originalStream -c:a libfaac -ar 44100 -ab 48k -c:v libx264 -vpre slow -vpre baseline -f flv rtmp://server/live/h264Stream  

```ruby
ffmpeg -i rtmp://server/live/originalStream -c:a libfaac -ar 44100 -ab 48k -c:v libx264 -vpre slow -vpre baseline -f flv rtmp://server/live/h264Stream
```

5、将其中一个直播流，视频不变，音频改用faac压缩，送至另外一个直播服务流

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -i rtmp://server/live/originalStream -acodec libfaac -ar 44100 -ab 48k -vcodec copy -f flv rtmp://server/live/h264_AAC_Stream  

```ruby
ffmpeg -i rtmp://server/live/originalStream -acodec libfaac -ar 44100 -ab 48k -vcodec copy -f flv rtmp://server/live/h264_AAC_Stream
```

6、将一个高清流，复制为几个不同视频清晰度的流重新发布，其中音频不变

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -re -i rtmp://server/live/high_FMLE_stream -acodec copy -vcodec x264lib -s 640×360 -b 500k -vpre medium -vpre baseline rtmp://server/live/baseline_500k -acodec copy -vcodec x264lib -s 480×272 -b 300k -vpre medium -vpre baseline rtmp://server/live/baseline_300k -acodec copy -vcodec x264lib -s 320×200 -b 150k -vpre medium -vpre baseline rtmp://server/live/baseline_150k -acodec libfaac -vn -ab 48k rtmp://server/live/audio_only_AAC_48k  

```ruby
ffmpeg -re -i rtmp://server/live/high_FMLE_stream -acodec copy -vcodec x264lib -s 640×360 -b 500k -vpre medium -vpre baseline rtmp://server/live/baseline_500k -acodec copy -vcodec x264lib -s 480×272 -b 300k -vpre medium -vpre baseline rtmp://server/live/baseline_300k -acodec copy -vcodec x264lib -s 320×200 -b 150k -vpre medium -vpre baseline rtmp://server/live/baseline_150k -acodec libfaac -vn -ab 48k rtmp://server/live/audio_only_AAC_48k
```

7、功能一样，只是采用-x264opts选项

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -re -i rtmp://server/live/high_FMLE_stream -c:a copy -c:v x264lib -s 640×360 -x264opts bitrate=500:profile=baseline:preset=slow rtmp://server/live/baseline_500k -c:a copy -c:v x264lib -s 480×272 -x264opts bitrate=300:profile=baseline:preset=slow rtmp://server/live/baseline_300k -c:a copy -c:v x264lib -s 320×200 -x264opts bitrate=150:profile=baseline:preset=slow rtmp://server/live/baseline_150k -c:a libfaac -vn -b:a 48k rtmp://server/live/audio_only_AAC_48k  

```ruby
ffmpeg -re -i rtmp://server/live/high_FMLE_stream -c:a copy -c:v x264lib -s 640×360 -x264opts bitrate=500:profile=baseline:preset=slow rtmp://server/live/baseline_500k -c:a copy -c:v x264lib -s 480×272 -x264opts bitrate=300:profile=baseline:preset=slow rtmp://server/live/baseline_300k -c:a copy -c:v x264lib -s 320×200 -x264opts bitrate=150:profile=baseline:preset=slow rtmp://server/live/baseline_150k -c:a libfaac -vn -b:a 48k rtmp://server/live/audio_only_AAC_48k
```

8、将当前摄像头及音频通过DSSHOW采集，视频h264、音频faac压缩后发布

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -r 25 -f dshow -s 640×480 -i video=”video source name”:audio=”audio source name” -vcodec libx264 -b 600k -vpre slow -acodec libfaac -ab 128k -f flv rtmp://server/application/stream_name  

```delphi
ffmpeg -r 25 -f dshow -s 640×480 -i video=”video source name”:audio=”audio source name” -vcodec libx264 -b 600k -vpre slow -acodec libfaac -ab 128k -f flv rtmp://server/application/stream_name
```

9、将一个JPG图片经过h264压缩循环输出为mp4视频

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg.exe -i INPUT.jpg -an -vcodec libx264 -coder 1 -flags +loop -cmp +chroma -subq 10 -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -flags2 +dct8x8 -trellis 2 -partitions +parti8x8+parti4x4 -crf 24 -threads 0 -r 25 -g 25 -y OUTPUT.mp4  

```css
ffmpeg.exe -i INPUT.jpg -an -vcodec libx264 -coder 1 -flags +loop -cmp +chroma -subq 10 -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -flags2 +dct8x8 -trellis 2 -partitions +parti8x8+parti4x4 -crf 24 -threads 0 -r 25 -g 25 -y OUTPUT.mp4
```

10、将普通流视频改用h264压缩，音频不变，送至高清流服务(新版本FMS live=1)

**[plain]** [view plain](http://blog.csdn.net/wh8_2011/article/details/52117932#) [copy](http://blog.csdn.net/wh8_2011/article/details/52117932#) [print](http://blog.csdn.net/wh8_2011/article/details/52117932#)[?](http://blog.csdn.net/wh8_2011/article/details/52117932#)







1. ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv “rtmp://server/live/h264Stream live=1〃<br style="box-sizing: border-box;" /><br style="box-sizing: border-box;" /><br style="box-sizing: border-box;" />  

```xml
ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv “rtmp://server/live/h264Stream live=1〃<br style="box-sizing: border-box;" /><br style="box-sizing: border-box;" /><br style="box-sizing: border-box;" />
```

\------------------------------------------------------------------------

\----------------------------------------------------------------------------------------------------------

\------------------------------------------------------------------------

 

1.采集usb摄像头视频命令：

ffmpeg -t 20 -f vfwcap -i 0 -r 8 -f mp4 cap1111.mp4

 

./ffmpeg -t 10 -f vfwcap -i 0 -r 8 -f mp4 cap.mp4

具体说明如下：我们采集10秒，采集设备为vfwcap类型设备，第0个vfwcap采集设备（如果系统有多个vfw的视频采集设备，可以通过-i num来选择），每秒8帧，输出方式为文件，格式为mp4。

 

2.最简单的抓屏：

ffmpeg -f gdigrab -i desktop out.mpg 

 

3.从屏幕的（10,20）点处开始，抓取640x480的屏幕，设定帧率为5 ：

ffmpeg -f gdigrab -framerate 5 -offset_x 10 -offset_y 20 -video_size 640x480 -i desktop out.mpg 

 

4.ffmpeg从视频中生成gif图片：

ffmpeg -i capx.mp4 -t 10 -s 320x240 -pix_fmt rgb24 jidu1.gif

 

5.ffmpeg将图片转换为视频：

<http://blog.sina.com.cn/s/blog_40d73279010113c2.html>

 

===========================================================================

part 7:

 

 

FFMPEG以其强大的功能而在音视频领域著称，更重要的是它还是开源的！音视频格式转换、裁剪、拼接、提取字幕或某音轨等等，它都能胜任，可谓一把瑞士军刀，小巧但功能强大，是音视频研究的必备利器之一。

 

### 示例

 

**1. 转换成H264编解码**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file -vcodec h264 output_file  

```undefined
ffmpeg -i input_file -vcodec h264 output_file
```

其中 -i 表示输入文件， -vcodec  h264 表示视频编解码方式为 H264。

**2. 转换成H265编解码**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file -vcode hevc output_file  

```undefined
ffmpeg -i input_file -vcode hevc output_file
```

其中 -i 表示输入文件， -vcodec  hevc 表示视频编解码方式为 H265，注意ffmpeg 中名称为 hevc，不是H265！

**3. 设置输出视频的分辨率**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file -vcodec h264 -s 1280x720 output_file  

```undefined
ffmpeg -i input_file -vcodec h264 -s 1280x720 output_file
```

其中 -s 表示分辨率。

**4. 设置输出文件的音视频比特率**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file  -vcodec h264 -b:v 10M -b:a 128K output_file  

```css
ffmpeg -i input_file  -vcodec h264 -b:v 10M -b:a 128K output_file
```

其中 -b:v 10M 表示视频码率为10Mbps， -b:a 128K 表示音频码率为 128Kbps，注意FFMPEG对于码率控制，有时候不太准确，跟输入源有一定关系。

**5. 剪切某段视频**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -ss 0:05:00 -t 0:1:30 -i input_file -vcodec copy -acodec copy output_file  
2. ffmpeg -ss 300 -t 90  -i input_file -vcodec copy -acodec copy output_file  

```css
ffmpeg -ss 0:05:00 -t 0:1:30 -i input_file -vcodec copy -acodec copy output_file
ffmpeg -ss 300 -t 90  -i input_file -vcodec copy -acodec copy output_file
```

上面的两种方式等价，时间格式支持 HH:MM:SS或者秒数。 -ss 开始时间， -t 持续时间， -vcodec copy 保持原视频编码， -acodec copy 保持原音频编码。

**6. 分离音视频流保存为不同文件**

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file -vcodec copy -an output_file_video　　//提取视频流  
2. ffmpeg -i input_file -acodec copy -vn output_file_audio　　//提取音频流  

```go
ffmpeg -i input_file -vcodec copy -an output_file_video　　//提取视频流
ffmpeg -i input_file -acodec copy -vn output_file_audio　　//提取音频流
```

其中 -an 表示不处理音频， -vn 表示不处理视频。

**7.合并多个音视频文件为一个文件**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg –i video_file –i audio_file –vcodec copy –acodec copy output_file  

```go
ffmpeg –i video_file –i audio_file –vcodec copy –acodec copy output_file
```

 

 

**8. 提取视频图像保存为图片文件**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg –i input_file –r 1 –f image2 image-%3d.jpeg        //提取图片  

```go
ffmpeg –i input_file –r 1 –f image2 image-%3d.jpeg        //提取图片
```

 

其中 -r 表示提取图像的频率，-f 表示输出格式， %3d 表示文件命名方式（也即生成的图片名为 image-001.jpeg, image-002.jpeg, ...., image-999.jpeg）。

 

**9. 转换成YUV原始文件**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i input_file -vcodec rawvideo -an output_file_yuv  

```undefined
ffmpeg -i input_file -vcodec rawvideo -an output_file_yuv
```

 

 

**10. YUV序列转出AVI文件**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi  

```css
ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi
```

 

 

**11. 控制关键帧间隔和B帧**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg –i input_file  –vcodec h264 –bf 0 –g 25 –s 854x480 –an –f m4v output_file  

```undefined
ffmpeg –i input_file  –vcodec h264 –bf 0 –g 25 –s 854x480 –an –f m4v output_file
```

 

其中-bf 控制B帧数目，-g 控制关键帧间隔， -f 控制文件格式（format，注意与codec的区别）。

 

**12. 录制rtsp网络数据流**

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg –i rtsp://192.168.1.100:5555/test_stream –vcodec copy out.avi  

```java
ffmpeg –i rtsp://192.168.1.100:5555/test_stream –vcodec copy out.avi
```

 

将rtsp的网络视频流文件保存为 out.avi 本地文件

 

**13. 在多音轨文件中提取某音轨文件**

得分两步走，第一步，查看源文件，找到对应的音频序号；第二步，将对应的音频序号流提取出来。如下所示：

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. <strong><span style="color:#ff0000;">D:>ffmpeg  -i Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv</span></strong>  
2. ffmpeg version N-66931-gbbd8c85 Copyright (c) 2000-2014 the FFmpeg developers  
3.   built on Oct 17 2014 01:05:12 with gcc 4.9.1 (GCC)  
4.   configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-avisynth --enable-bzlib --enable-fontconfi  
5. g --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --  
6. enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmp3lame --enable-lib  
7. opencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinge  
8. r --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-aacenc --  
9. enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-  
10. libx265 --enable-libxavs --enable-libxvid --enable-zlib  
11.   libavutil      54. 10.100 / 54. 10.100  
12.   libavcodec     56.  8.100 / 56.  8.100  
13.   libavformat    56.  9.100 / 56.  9.100  
14.   libavdevice    56.  1.100 / 56.  1.100  
15.   libavfilter     5.  1.106 /  5.  1.106  
16.   libswscale      3.  1.101 /  3.  1.101  
17.   libswresample   1.  1.100 /  1.  1.100  
18.   libpostproc    53.  3.100 / 53.  3.100  
19. Input #0, matroska,webm, from 'D:\Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv':  
20.   Metadata:  
21. ​    title           : X-MEN__DAYS_OF_FUTURE_PAST.Title800  
22. ​    encoder         : libebml v0.7.8 + libmatroska v0.8.1  
23. ​    creation_time   : 2014-10-07 14:08:43  
24.   Duration: 02:11:35.18, start: 0.000000, bitrate: 28873 kb/s  
25. ​    Chapter #0:0: start 0.000000, end 213.338000  
26. ​    Metadata:  
27. ​      title           : (01)00:00:00:000  
28. ​    Chapter #0:1: start 213.338000, end 480.688000  
29. ​    Metadata:  
30. ​      title           : (02)00:03:33:338  
31. ​    Chapter #0:2: start 480.688000, end 628.628000  
32. ​    Metadata:  
33. ​      title           : (03)00:08:00:688  
34. ​    Chapter #0:3: start 628.628000, end 838.045000  
35. ​    Metadata:  
36. ​      title           : (04)00:10:28:628  
37. ​    Chapter #0:4: start 838.045000, end 1022.021000  
38. ​    Metadata:  
39. ​      title           : (05)00:13:58:045  
40. ​    Chapter #0:5: start 1022.021000, end 1216.089000  
41. ​    Metadata:  
42. ​      title           : (06)00:17:02:021  
43. ​    Chapter #0:6: start 1216.089000, end 1331.330000  
44. ​    Metadata:  
45. ​      title           : (07)00:20:16:089  
46. ​    Chapter #0:7: start 1331.330000, end 1597.887000  
47. ​    Metadata:  
48. ​      title           : (08)00:22:11:330  
49. ​    Chapter #0:8: start 1597.887000, end 1832.038000  
50. ​    Metadata:  
51. ​      title           : (09)00:26:37:887  
52. ​    Chapter #0:9: start 1832.038000, end 2069.776000  
53. ​    Metadata:  
54. ​      title           : (10)00:30:32:038  
55. ​    Chapter #0:10: start 2069.776000, end 2160.992000  
56. ​    Metadata:  
57. ​      title           : (11)00:34:29:776  
58. ​    Chapter #0:11: start 2160.992000, end 2298.087000  
59. ​    Metadata:  
60. ​      title           : (12)00:36:00:992  
61. ​    Chapter #0:12: start 2298.087000, end 2437.101000  
62. ​    Metadata:  
63. ​      title           : (13)00:38:18:087  
64. ​    Chapter #0:13: start 2437.101000, end 2753.792000  
65. ​    Metadata:  
66. ​      title           : (14)00:40:37:101  
67. ​    Chapter #0:14: start 2753.792000, end 2933.847000  
68. ​    Metadata:  
69. ​      title           : (15)00:45:53:792  
70. ​    Chapter #0:15: start 2933.847000, end 3118.156000  
71. ​    Metadata:  
72. ​      title           : (16)00:48:53:847  
73. ​    Chapter #0:16: start 3118.156000, end 3283.113000  
74. ​    Metadata:  
75. ​      title           : (17)00:51:58:156  
76. ​    Chapter #0:17: start 3283.113000, end 3471.426000  
77. ​    Metadata:  
78. ​      title           : (18)00:54:43:113  
79. ​    Chapter #0:18: start 3471.426000, end 3648.186000  
80. ​    Metadata:  
81. ​      title           : (19)00:57:51:426  
82. ​    Chapter #0:19: start 3648.186000, end 3806.594000  
83. ​    Metadata:  
84. ​      title           : (20)01:00:48:186  
85. ​    Chapter #0:20: start 3806.594000, end 4029.567000  
86. ​    Metadata:  
87. ​      title           : (21)01:03:26:594  
88. ​    Chapter #0:21: start 4029.567000, end 4183.596000  
89. ​    Metadata:  
90. ​      title           : (22)01:07:09:567  
91. ​    Chapter #0:22: start 4183.596000, end 4391.470000  
92. ​    Metadata:  
93. ​      title           : (23)01:09:43:596  
94. ​    Chapter #0:23: start 4391.470000, end 4531.860000  
95. ​    Metadata:  
96. ​      title           : (24)01:13:11:470  
97. ​    Chapter #0:24: start 4531.860000, end 4647.726000  
98. ​    Metadata:  
99. ​      title           : (25)01:15:31:860  
100. ​    Chapter #0:25: start 4647.726000, end 4770.098000  
101. ​    Metadata:  
102. ​      title           : (26)01:17:27:726  
103. ​    Chapter #0:26: start 4770.098000, end 5090.710000  
104. ​    Metadata:  
105. ​      title           : (27)01:19:30:098  
106. ​    Chapter #0:27: start 5090.710000, end 5218.254000  
107. ​    Metadata:  
108. ​      title           : (28)01:24:50:710  
109. ​    Chapter #0:28: start 5218.254000, end 5464.250000  
110. ​    Metadata:  
111. ​      title           : (29)01:26:58:254  
112. ​    Chapter #0:29: start 5464.250000, end 5643.679000  
113. ​    Metadata:  
114. ​      title           : (30)01:31:04:250  
115. ​    Chapter #0:30: start 5643.679000, end 5853.180000  
116. ​    Metadata:  
117. ​      title           : (31)01:34:03:679  
118. ​    Chapter #0:31: start 5853.180000, end 6114.817000  
119. ​    Metadata:  
120. ​      title           : (32)01:37:33:180  
121. ​    Chapter #0:32: start 6114.817000, end 6292.995000  
122. ​    Metadata:  
123. ​      title           : (33)01:41:54:817  
124. ​    Chapter #0:33: start 6292.995000, end 6439.808000  
125. ​    Metadata:  
126. ​      title           : (34)01:44:52:995  
127. ​    Chapter #0:34: start 6439.808000, end 6689.766000  
128. ​    Metadata:  
129. ​      title           : (35)01:47:19:808  
130. ​    Chapter #0:35: start 6689.766000, end 7002.745000  
131. ​    Metadata:  
132. ​      title           : (36)01:51:29:766  
133. ​    Chapter #0:36: start 7002.745000, end 7212.413000  
134. ​    Metadata:  
135. ​      title           : (37)01:56:42:745  
136. ​    Chapter #0:37: start 7212.413000, end 7354.972000  
137. ​    Metadata:  
138. ​      title           : (38)02:00:12:413  
139. ​    Chapter #0:38: start 7354.972000, end 7831.907000  
140. ​    Metadata:  
141. ​      title           : (39)02:02:34:972  
142. ​    Chapter #0:39: start 7831.907000, end 7895.095000  
143. ​    Metadata:  
144. ​      title           : (40)02:10:31:907  
145. ​    Chapter #0:40: start 7895.095000, end 7895.178884  
146. ​    Metadata:  
147. ​      title           : (41)02:11:35:095  
148. ​    <strong>Stream #0:0: Video: hevc (Main), yuv420p(tv), 3840x2160, SAR 1:1 DAR 16:9, 23.98 fps, 23.98 tbr, 48003.07 tbn, 48003  
149. .07 tbc (default)  
150. ​    Stream #0:1(eng): Audio: dts (DTS-HD MA), 48000 Hz, 5.1(side), fltp, 1536 kb/s (default)  
151. ​    <span style="color:#33ff33;">Stream #0:2(eng): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s</span>  
152. ​    Stream #0:3(spa): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s  
153. ​    Stream #0:4(fra): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s  
154. ​    Stream #0:5(por): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s  
155. ​    Stream #0:6(rus): Audio: dts (DTS), 48000 Hz, 5.1(side), fltp, 768 kb/s  
156. ​    Stream #0:7(tha): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s  
157. ​    Stream #0:8(ukr): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s</strong>  
158. <span style="color:#ff0000;">At least one output file must be specified</span>  
159.   
160. D:>  

```vbnet
<strong><span style="color:#ff0000;">D:>ffmpeg  -i Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv</span></strong>
ffmpeg version N-66931-gbbd8c85 Copyright (c) 2000-2014 the FFmpeg developers
  built on Oct 17 2014 01:05:12 with gcc 4.9.1 (GCC)
  configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-avisynth --enable-bzlib --enable-fontconfi
g --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --
enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmp3lame --enable-lib
opencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinge
r --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-aacenc --
enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-
libx265 --enable-libxavs --enable-libxvid --enable-zlib
  libavutil      54. 10.100 / 54. 10.100
  libavcodec     56.  8.100 / 56.  8.100
  libavformat    56.  9.100 / 56.  9.100
  libavdevice    56.  1.100 / 56.  1.100
  libavfilter     5.  1.106 /  5.  1.106
  libswscale      3.  1.101 /  3.  1.101
  libswresample   1.  1.100 /  1.  1.100
  libpostproc    53.  3.100 / 53.  3.100
Input #0, matroska,webm, from 'D:\Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv':
  Metadata:
    title           : X-MEN__DAYS_OF_FUTURE_PAST.Title800
    encoder         : libebml v0.7.8 + libmatroska v0.8.1
    creation_time   : 2014-10-07 14:08:43
  Duration: 02:11:35.18, start: 0.000000, bitrate: 28873 kb/s
    Chapter #0:0: start 0.000000, end 213.338000
    Metadata:
      title           : (01)00:00:00:000
    Chapter #0:1: start 213.338000, end 480.688000
    Metadata:
      title           : (02)00:03:33:338
    Chapter #0:2: start 480.688000, end 628.628000
    Metadata:
      title           : (03)00:08:00:688
    Chapter #0:3: start 628.628000, end 838.045000
    Metadata:
      title           : (04)00:10:28:628
    Chapter #0:4: start 838.045000, end 1022.021000
    Metadata:
      title           : (05)00:13:58:045
    Chapter #0:5: start 1022.021000, end 1216.089000
    Metadata:
      title           : (06)00:17:02:021
    Chapter #0:6: start 1216.089000, end 1331.330000
    Metadata:
      title           : (07)00:20:16:089
    Chapter #0:7: start 1331.330000, end 1597.887000
    Metadata:
      title           : (08)00:22:11:330
    Chapter #0:8: start 1597.887000, end 1832.038000
    Metadata:
      title           : (09)00:26:37:887
    Chapter #0:9: start 1832.038000, end 2069.776000
    Metadata:
      title           : (10)00:30:32:038
    Chapter #0:10: start 2069.776000, end 2160.992000
    Metadata:
      title           : (11)00:34:29:776
    Chapter #0:11: start 2160.992000, end 2298.087000
    Metadata:
      title           : (12)00:36:00:992
    Chapter #0:12: start 2298.087000, end 2437.101000
    Metadata:
      title           : (13)00:38:18:087
    Chapter #0:13: start 2437.101000, end 2753.792000
    Metadata:
      title           : (14)00:40:37:101
    Chapter #0:14: start 2753.792000, end 2933.847000
    Metadata:
      title           : (15)00:45:53:792
    Chapter #0:15: start 2933.847000, end 3118.156000
    Metadata:
      title           : (16)00:48:53:847
    Chapter #0:16: start 3118.156000, end 3283.113000
    Metadata:
      title           : (17)00:51:58:156
    Chapter #0:17: start 3283.113000, end 3471.426000
    Metadata:
      title           : (18)00:54:43:113
    Chapter #0:18: start 3471.426000, end 3648.186000
    Metadata:
      title           : (19)00:57:51:426
    Chapter #0:19: start 3648.186000, end 3806.594000
    Metadata:
      title           : (20)01:00:48:186
    Chapter #0:20: start 3806.594000, end 4029.567000
    Metadata:
      title           : (21)01:03:26:594
    Chapter #0:21: start 4029.567000, end 4183.596000
    Metadata:
      title           : (22)01:07:09:567
    Chapter #0:22: start 4183.596000, end 4391.470000
    Metadata:
      title           : (23)01:09:43:596
    Chapter #0:23: start 4391.470000, end 4531.860000
    Metadata:
      title           : (24)01:13:11:470
    Chapter #0:24: start 4531.860000, end 4647.726000
    Metadata:
      title           : (25)01:15:31:860
    Chapter #0:25: start 4647.726000, end 4770.098000
    Metadata:
      title           : (26)01:17:27:726
    Chapter #0:26: start 4770.098000, end 5090.710000
    Metadata:
      title           : (27)01:19:30:098
    Chapter #0:27: start 5090.710000, end 5218.254000
    Metadata:
      title           : (28)01:24:50:710
    Chapter #0:28: start 5218.254000, end 5464.250000
    Metadata:
      title           : (29)01:26:58:254
    Chapter #0:29: start 5464.250000, end 5643.679000
    Metadata:
      title           : (30)01:31:04:250
    Chapter #0:30: start 5643.679000, end 5853.180000
    Metadata:
      title           : (31)01:34:03:679
    Chapter #0:31: start 5853.180000, end 6114.817000
    Metadata:
      title           : (32)01:37:33:180
    Chapter #0:32: start 6114.817000, end 6292.995000
    Metadata:
      title           : (33)01:41:54:817
    Chapter #0:33: start 6292.995000, end 6439.808000
    Metadata:
      title           : (34)01:44:52:995
    Chapter #0:34: start 6439.808000, end 6689.766000
    Metadata:
      title           : (35)01:47:19:808
    Chapter #0:35: start 6689.766000, end 7002.745000
    Metadata:
      title           : (36)01:51:29:766
    Chapter #0:36: start 7002.745000, end 7212.413000
    Metadata:
      title           : (37)01:56:42:745
    Chapter #0:37: start 7212.413000, end 7354.972000
    Metadata:
      title           : (38)02:00:12:413
    Chapter #0:38: start 7354.972000, end 7831.907000
    Metadata:
      title           : (39)02:02:34:972
    Chapter #0:39: start 7831.907000, end 7895.095000
    Metadata:
      title           : (40)02:10:31:907
    Chapter #0:40: start 7895.095000, end 7895.178884
    Metadata:
      title           : (41)02:11:35:095
    <strong>Stream #0:0: Video: hevc (Main), yuv420p(tv), 3840x2160, SAR 1:1 DAR 16:9, 23.98 fps, 23.98 tbr, 48003.07 tbn, 48003
.07 tbc (default)
    Stream #0:1(eng): Audio: dts (DTS-HD MA), 48000 Hz, 5.1(side), fltp, 1536 kb/s (default)
    <span style="color:#33ff33;">Stream #0:2(eng): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s</span>
    Stream #0:3(spa): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s
    Stream #0:4(fra): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s
    Stream #0:5(por): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s
    Stream #0:6(rus): Audio: dts (DTS), 48000 Hz, 5.1(side), fltp, 768 kb/s
    Stream #0:7(tha): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s
    Stream #0:8(ukr): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s</strong>
<span style="color:#ff0000;">At least one output file must be specified</span>

D:>
```

 

执行上面的命令，查看原始文件，发现它有8个音轨！，我们只提取第二个音轨流（上图标识为绿色的那个），命令如下所示：

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg -i Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv -map 0:2 audio_2.wav  

```css
ffmpeg -i Xmen_HEVC_4K_Multi_Audio_28Mbps.mkv -map 0:2 audio_2.wav
```

 

**14. 多个视频文件拼接**

首先创建一个需要拼接的文件，例如 concat.txt，内容如下：

file 'orig_20160616113303_0.avi'
file 'orig_20160616113303_1.avi'

然后执行如下命令 

 

**[plain]** [view plain](http://blog.csdn.net/thomashtq/article/details/44940457#) [copy](http://blog.csdn.net/thomashtq/article/details/44940457#) [print](http://blog.csdn.net/thomashtq/article/details/44940457#)[?](http://blog.csdn.net/thomashtq/article/details/44940457#)







1. ffmpeg  -f concat -i concat.txt  -c copy orig_20160616113303_merge.avi  

```css
 ffmpeg  -f concat -i concat.txt  -c copy orig_20160616113303_merge.avi
```

 

### FFMPEG常用参数说明

**主要参数：**

-i 设定输入流
-f 设定输出格式
-ss 开始时间
-formats  查看所有支持的容器格式
-codecs  查看所有编解码器
-filters      查看所有可用的filter
-pix_fmts 查看所有支持的图片格式
-sample_fmts 查看所有支持的像素格式
-i input_file 查看媒体文件input_file的信息（注意后面不再接其它参数，例如: ffmpeg -i test.mp4，查看 test.mp4视频信息）


**视频参数：**
-b 设定视频流量，默认为200Kbit/s
-r 设定帧速率，默认为25
-s 设定画面的宽与高
-aspect 设定画面的比例
-vn 不处理视频
-vcodec 设定视频编解码器，未设定时则使用与输入流相同的编解码器


**音频参数：**
-ar 设定采样率
-ac 设定声音的Channel数
-acodec 设定声音编解码器，未设定时则使用与输入流相同的编解码器
-an 不处理音频

 

### 小结

ffmpeg功能强大，参数配置众多，最好的方式查看它的命令行帮助文档，各个参数都有详细的说明，唯一的遗憾就是没有例子，对于初入媒体领域的人来说，门槛比较高。不明白的参数，可以多试试，再接合MediaInfo工具查看具体信息，通过VLC或者FFMPEG自带的 ffplay播放器播放，就容易理解了。

===========================================================================

part 8:

 

 

格式转换 (将file.avi 转换成output.flv)

​     ffmpeg -i  file.avi   output.flv

​       -i 表示输入文件 

 :现在有个视频video.avi，有个音频 audio.mp3，将其合并成output.avi

两个命令                     （ video2.avi 是中间文件 ，用完可删）

ffmpeg -i video.avi -vcodec copy -an video2.avi   
ffmpeg -i video2.avi -i audio.mp3 -vcodec copy -acodec copy output.avi

 -i 表示输入文件

  -vcodec copy 表示 force video codec ('copy' to copy stream) 这个不知怎么译 ，估计是直接copy 

  -acodec copy   这个说的应该是音频了   跟上面一样

-an : 表示  disable audio  估计是audio no 之类的缩写   表示去掉video.avi 原有的音频

方法2 好像可以直接指定两个输入文件 ，

```html
ffmpeg -i /tmp/a.wav  -i /tmp/a.avi /tmp/a.avi   两个文件 的顺序很重



 
```

从视频里提取声音（声音与视频的分离）

 ffmpeg  -i 人生若只如初见.flv  -vn r.mp3  从flv 文件 中提取声音并保存为mp3 格式  

​       -vn : 表示忽略视频 估计是video no 之类的缩写

 ffmpeg  -i 人生若只如初见.flv  -an  r.flv   只留视频不留声音 

​    -an : 表示 忽略 声音 估计是audio no 之类的缩写

 

从视频里提取图片（ ）

```html
ffmpeg -i test.avi -y -f image2 -ss 8 -t 0.001 -s 350x240 test.jpg       



 



 -ss 第8秒处 截一图



 
```

 

压缩mp3 文件

如果你觉得mp3 文件 有点大，想变小一点那么可以通过-ab 选项改变音频的比特率 （bitrate）

ffmpeg -i input.mp3 -ab 128 output.mp3    //这里将比特率设为128

你可以用file 命令查看一下源文件 的信息

z.mp3: Audio file with ID3 version 2.3.0, contains: MPEG ADTS, layer III, v1, 192 kbps, 44.1 kHz, Stereo

其中的192 kbps 就是这个东西

mp3中比特率的含义是：在压缩音频文件至mp3时，由压缩软件所确定数码文件在播放时每秒传 送给播放器大小，其单位是：千位/秒；英文的含义是：kbps - = kilobits per second。现在mp3文件的最高数位率是320 kbps。这样的文件体积很大，每分钟的音乐超过两兆字节。如果采用可变比特率（VBR）编码来生成mp3文件，获得与320 kbps相当音质，文件的体积会缩小25~50%。请注意：播放时间相同，而歌曲不同，所获的压缩mp3文件的一般不相同，这是因为VBR编码所生成的 mp3文件的大小不仅仅取决于播放时间的长度，还取决于源音频文件的其它因素。

 

录音（要有可用的麦克风，并且如果用alsa 的话，好像得安alsa-oss，重启） 

 ffmpeg  -f oss -i /dev/dsp   out.avi  (should  hava oss or alsa-oss)

 ffmpeg   -f  alsa -ac 2 -i hw:0, 0  out.avi   (should )

 ffmpeg   -f alsa -ac 2 -i pulse  (should hava PulseAudio)

 

   oss 是linux 下的声音相关的东西，与alsa 一样，不过oss 是商业的， 而/dev/dsp 是oss 用到的麦克的设备吧，可以这样理解

屏幕录像

ffmpeg -f x11grab -s xga -r 10 -i :0.0+0+0 wheer.avi 

ffmpeg -f x11grab  **-s 320x240**  -r 10 -i :0.0+100+200 wheer.avi

:0:0 表示屏幕（个人理解，因为系统变量$DISPLAY值就是:0.0）  而100,表示距左端100象素，200表示距上端200

-s 设置窗口大小

  -r 10 好像是设置频率，不懂

ffmpeg -f x11grab -s xga    -qscale 5    -r 10 -i :0.0+0+0 wheer.avi 

-qscale 8 设定画面质量，值 越小越好

屏幕录像，同时录音

ffmpeg -f oss -i /dev/dsp        -f x11grab -r 30 -s 1024x768 -i :0.0  output.mkv

ffmpeg   -ac 2 -f oss  -i  /dev/dsp   -f x11grab -r 30 -s 1024x768 -i :0.0 -acodec pcm_s16le -vcodec libx264 -vpre lossless_ultrafast -threads 0 output.mkv

看到这，你会发现这个命令有多强大，

如果我屏幕上打开了一个窗口，我只想录这个窗口的内容，如何确定这个窗口的坐标位置呢

可以用另外一个命令

xwininfo 输入这个命令后，用鼠标点选目标窗口，

就会出现目标窗口的坐标，宽高等一系列信息

Absolute upper-left X:  276
  Absolute upper-left Y:  57
  Relative upper-left X:  2
  Relative upper-left Y:  23
  Width: 742
  Height: 499
  Depth: 24
  Visual: 0x21
  Visual Class: TrueColor
  Border width: 0
  Class: InputOutput
  Colormap: 0x20 (installed)
  Bit Gravity State: NorthWestGravity
  Window Gravity State: NorthWestGravity
  Backing Store State: NotUseful
  Save Under State: no
  Map State: IsViewable
  Override Redirect State: no
  Corners:  +276+57  -262+57  -262-244  +276-244
  -geometry 80x24+274+34 看到这一行了没 (）

 

比如根据上面的信息

ffmpeg -f oss  -i  /dev/dsp      -f  x11grab -r 30  -s 1280x752 -i :0.0+0+23  output.avi 
Another thing you can change is the video frame rate (FPS). In the example above we used -r 30 which means capture at 30 FPS. You can change this value to whatever frame rate you want.

这个 -r 30 应该是每秒钟取样几次，估计是一秒截三十次屏，

切头去尾

 ffmpeg -ss 00:00:10 -t 00:01:22 -i 五月天-突然好想你.mp3  output.mp3 

只要 从第10秒开始截取，共截取1：22时长的内容

视频文件的连接，如两个flv 文件 连接成一

 

好像必须先将文件 转成mpg ，dv 等格式的文件后才能进行连接

- 连接复数的AVI影片档之范例（在此范例中须一度暂时将AVI档转换成MPEG-1档(MPEG-1, MPEG-2 PS, DV格式亦可连接)）

```html
ffmpeg -i input1.avi -sameq inputfile_01.mpg -r 20



 



ffmpeg -i input2.avi -sameq inputfile_02.mpg  -r 20



 



cat inputfile_01.mpg inputfile_02.mpg > inputfile_all.mpg



 



ffmpeg -i inputfile_all.mpg -sameq outputfile.avi



 



上面将  input1.avi    input2.avi 合并成outputfile.avi



 



-sameq  表示  相同的质量（可能指的是画面，不太清楚）



 



 -r 指频率



 



 



 



 



 



 



 



=====================================================================下面是wiki 上给的几个例子
```

## 参数

FFmpeg可使用众多参数，参数内容会根据ffmpeg版本而有差异，使用前建议先参考参数及编解码器的叙述。此外，参数明细可用 `ffmpeg -h` 显示；编解码器名称等明细可用 `ffmpeg -formats` 显示。

下列为较常使用的参数。

### [[编辑](http://zh.wikipedia.org/w/index.php?title=FFmpeg&action=edit&section=3) ] 主要参数

- -i 设定输入档名。
- -f 设定输出格式。
- -y 若输出档案已存在时则覆盖档案。
- -fs 超过指定的档案大小时则结束转换。
- -ss 从指定时间开始转换。
- -title 设定标题。
- -timestamp 设定时间戳。
- -vsync 增减Frame使影音同步。

### [[编辑](http://zh.wikipedia.org/w/index.php?title=FFmpeg&action=edit&section=4) ] 影像参数

- -b 设定影像流量，默认为200Kbit/秒。（ *单位请参照下方注意事项* ）
- -r 设定FrameRate值，默认为25。
- -s 设定画面的宽与高。
- -aspect 设定画面的比例。
- -vn 不处理影像，于仅针对声音做处理时使用。
- -vcodec 设定影像影像编解码器，未设定时则使用与输入档案相同之编解码器。

### [[编辑](http://zh.wikipedia.org/w/index.php?title=FFmpeg&action=edit&section=5) ] 声音参数

- -ab 设定**每Channel** （最近的[SVN](http://zh.wikipedia.org/w/index.php?title=Subversion6&action=edit&redlink=1) 版为所有Channel的总合）的流量。（ *单位 请参照下方注意事项* ）
- -ar 设定采样率。
- -ac 设定声音的Channel数。
- -acodec 设定声音编解码器，未设定时与影像相同，使用与输入档案相同之编解码器。
- -an 不处理声音，于仅针对影像做处理时使用。
- -vol 设定音量大小，256为标准音量。(要设定成两倍音量时则输入512，依此类推。)

### [[编辑](http://zh.wikipedia.org/w/index.php?title=FFmpeg&action=edit&section=6) ] 注意事项

- 以-b及ab参数设定流量时，根据使用的ffmpeg版本，须注意单位会有kbits/sec与bits/sec的不同。（可用ffmpeg -h显示说明来确认单位。）

例如，单位为bits/sec的情况时，欲指定流量64kbps时需输入‘ -ab 64k ’；单位为kbits/sec的情况时则需输入‘ -ab 64 ’。

- 以-acodec及-vcodec所指定的编解码器名称，会根据使用的ffmpeg版本而有所不同。例如使用AAC编解码器时，会有输入aac与 libfaac的情况。此外，编解码器有分为仅供解码时使用与仅供编码时使用，因此一定要利用`ffmpeg -formats` 确 认输入的编解码器是否能运作。

## [[编辑](http://zh.wikipedia.org/w/index.php?title=FFmpeg&action=edit&section=7) ] 范例

- 将MPEG-1影片转换成MPEG-4格式之范例

```html
ffmpeg -i inputfile.mpg -f mp4 -acodec libfaac -vcodec mpeg4 -b 256k -ab 64k outputfile.mp4



 
```

- 将MP3声音转换成MPEG-4格式之范例

```html
ffmpeg -i inputfile.mp3 -f mp4 -acodec libaac -vn -ab 64k outputfile.mp4



 
```

- 将DVD的VOB档转换成VideoCD格式的MPEG-1档之范例

```html
ffmpeg -i inputfile.vob -f mpeg -acodec mp2 -vcodec mpeg1video -s 352x240 -b 1152k -ab 128k outputfile.mpg
```

- 将AVI影片转换成H.264格式的M4V档之范例
- ffmpeg -i inputfile.avi -f mp4　-acodec libfaac -vcodec libx264 -b 512k -ab 320k outputfile.m4v

- 将任何影片转换成东芝REGZA可辨识的MPEG2格式之范例
- ffmpeg -i inputfile -target ntsc-svcd -ab 128k -aspect 4:3 -s 720x480 outputfile.mpg

- 连接复数的AVI影片档之范例（在此范例中须一度暂时将AVI档转换成MPEG-1档(MPEG-1, MPEG-2 PSDV格式亦可连接)、

```html
ffmpeg -i input1.avi -sameq inputfile_01.mpg



 



ffmpeg -i input2.avi -sameq inputfile_02.mpg



 



cat inputfile_01.mpg inputfile_02.mpg > inputfile_all.mpg



 



ffmpeg -i inputfile_all.mpg -sameq outputfile.avi



 



=============================================================================



http://ffmpeg.org/ffmpeg-doc.html



 



http://ubuntuforums.org/showthread.php?t=1392026



同时搞明白的一些问题在alsa 体系中声卡（也可能是麦克风，）叫hw:0,0 而在oss 体系中叫/dev/dsp (用词可能不太专业)  Linux在安装了声卡后，会有一些设备文件生成。采集数字样本的/dev/dsp文件，针对混音器的/dev/mixer文件，用于音序器的/dev/sequencer，/dev/audio文件一个基于兼容性考虑的声音设备文件。只要向dev/audio中输入wav文件就能发出声音。而对/dev/dsp文件读取就能得到WAV文件格式的声音文件。
```

\---------------------------------------------------------------------------------------------------------------------------------------------

part 9:

mpegts 选项

mpegts混合器选项有：

- -mpegts_original_network_id number

  设置`original_network_id` (默认0x0001). 在DVB是一个唯一的网络标识，它用于标识特殊的服务（通过Original_Network_ID和Transport_Stream_ID）

- -mpegts_transport_stream_id number

  设置`transport_stream_id` (默认0x0001).在DVB是一个传输的标识

- -mpegts_service_id number

  设置`service_id` (默认0x0001),在DVB作为程序标识 DVB.

- -mpegts_service_type number

  设置程序`service_type` (默认digital_tv), 参考下面预设值

- -mpegts_pmt_start_pid number

  对PMT设置第一个PID (默认 0x1000,最大0x1f00).

- -mpegts_start_pid number

  对数据包设置第一个PID(默认0x0100,最大0x0f00).

- -mpegts_m2ts_mode number

  如果设置为1则允许`m2ts`模式，默认为-1，表示禁止 value is -1 which disables m2ts mode.

- -muxrate number

  设置内容为混合码率（默认VBR）

- -pcr_period numer

  覆盖默认的PCR重传时间（默认20ms），如果`muxrate`被设置将会被忽略

- -pes_payload_size number

  以单位字节设置最小PES播放加载包大小

- -mpegts_flags flags

  设置一个标志(后面介绍).

- -mpegts_copyts number

  如果设置为1则保留原始时间戳。默认为-1，将从0开始更新时间戳

- -tables_version number

  设置PAT, PMT 和SDT版本 (默认0,范围0-31) 。这个选项允许更新流结构， 以便用户可以检测到更改。比如在打开AVFormatContext （API使用时）或重启FFMPEG来周期性改变`tables_version`时:

  ffmpeg -i source1.ts -codec copy -f mpegts -tables_version 0 udp://1.1.1.1:1111
  ffmpeg -i source2.ts -codec copy -f mpegts -tables_version 1 udp://1.1.1.1:1111
  ... ffmpeg -i source3.ts -codec copy -f mpegts -tables_version 31 udp://1.1.1.1:1111
  ffmpeg -i source1.ts -codec copy -f mpegts -tables_version 0 udp://1.1.1.1:1111
  ffmpeg -i source2.ts -codec copy -f mpegts -tables_version 1 udp://1.1.1.1:1111
  ...

选项`mpegts_service_type`接受如下值:

- hex_value

  一个16进制值，范围0x01到0xff，定义在 ETSI 300 468.

- digital_tv

  数字TV服务

- digital_radio

  数字广播服务

- teletext

  图文电视服务

- advanced_codec_digital_radio

  高级编码数字广播服务

- mpeg2_digital_hdtv

  MPEG2数字HDTV服务

- advanced_codec_digital_sdtv

  高级编码数字SDTV服务

- advanced_codec_digital_hdtv

  高级编码数字HDTV服务

选项`mpegts_flags`可以设置如下标志:

- resend_headers

  写下一个包前反弹PAT/PMT

- latm

  对AAC编码使用LATM打包

mpegts例子

```html
 
```

ffmpeg -i file.mpg -c copy \

-mpegts_original_network_id 0x1122 \

-mpegts_transport_stream_id 0x3344 \

-mpegts_service_id 0x5566 \

-mpegts_pmt_start_pid 0x1500 \

-mpegts_start_pid 0x150 \

-metadata service_provider="Some provider" \

-metadata service_name="Some Channel" \

-y out.ts

\-----------------------------------------------------------------------------------------------------------------------------------------

part 10: (转自 <http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-22>)

[ffmpeg 命令行](http://blog.chinaunix.net/uid-21878516-id-4700012.html)

分类： Windows平台

2014-12-16 22:15:25

 


为一朋友搞个工具,用到ffmpeg . 做个记录如下，

ffmpeg 功能很是强大，视频相关基本权威了。

<http://www.ffmpeg.org/ffmpeg-formats.html#mpegts>
<http://www.ffmpeg.org/ffprobe.html#Stream-specifiers-1>



ffmpeg.exe -i d:\1.m2t -streamid 0:100 -codec copy d:\1-cpy.m2t

修改 stream 0的 id为100  编解码不变codec copy   生成文件1-cpy.m2t

------


ffmpeg -i d:\1.m2t  -codec copy -mpegts_pmt_start_pid 0x250 -mpegts_start_pid 0x251 d:\1-cpy.m2t
修改pmt_pid=0x250 和pcr_pid=0x251

------


ffprobe -show_format -show_programs d:\1.m2t

显示 format program 相关信息

------


ffprobe -show_entries program=program_num,pmt_pid,pcr_pid:stream=index,id,codec_type  -i d:\1.m2t  -hide_banner

显法prorgram 和stream 中的部分字段。hide_banner不显示库包文件相关版本信息。

\-------------------------------------------------------------------------------------------------------------------------------------

part 11:

## 22 混合器（复用器）

复用器是ffmpeg中负责写入多媒体流到文件中分区的可配置组件。

默认编译时自动允许被支持的混合器。你可以使用`--list-muxers`作为参数运行编译配置脚本以了解当前支持的所有混合器。

编译也可以同`--disable-muxers`禁用所有的混合器，或者通过`--enable-muxer=MUXER` / `--disable-muxer=MUXER`打开/关闭指定的混合器

在ff*工具集中附加`-formats`也可以了解到混合器列表。

下面将详细描述有效的混合器直播：

### aiff

Audio Interchange File Format（aif）密码器

aiff选项

接受下面选项：

- write_id3v2

  如果设为1则允许ID3v2标签，否则0禁止（默认）

- id3v2_version

  选择id3v2版本

### crc值

CRC (Cyclic Redundancy Check)测试格式

这个混合器通过所有输入的音频和视频帧计算（混合）Adler-32 CRC。默认音频会被转换为16bit符号原始音频，视频被解压为原始视频再进行这个计算。

输出会有一个形如`CRC=0xCRC`的一行，其中CRC的值是由16进制以0补足的8位数字，它由所有帧解码计算的。

参考[framecrc] 混合器

crc值计算例子

计算一个crc放置到out.crc:

```
 
ffmpeg -i INPUT -f crc out.crc
```

计算crc并直接输出到标准输出设备:

```
 
ffmpeg -i INPUT -f crc -
```

还可以选择对特定音频、视频编码数据计算crc，例如计算输入文件音频转换成PCM 8bit无符号数据格式，视频转换成MPEG-2 的CRC:

```
 
ffmpeg -i INPUT -c:a pcm_u8 -c:v mpeg2video -f crc -
```

### framecrc

每个数据包的CRC（循环冗余校验）测试格式。

它将对每个数据包做Adler-32 CRC计算并输出。默认音频被转换成16bit符号原始音频，视频被转换成原始视频再进行CRC计算。

输出是针对每个音频/视频数据包都有一行如下格式的信息：

```
 
stream_index, packet_dts, packet_pts, packet_duration, packet_size, 0xCRC
```

其中CRC值是16进制，以0补足的8位数字值。

framecrc例子

对INPUT输入作每数据包CRC计算，输出到out.crc:

```
 
ffmpeg -i INPUT -f framecrc out.crc
```

直接把计算结果输出到标准输出设备:

```
 
ffmpeg -i INPUT -f framecrc -
```

通过ffmpeg，还可以选择输出特定音频和视频格式对应的帧CRC值，例如音频转换成PCM8bit无符号编码，视频为mpeg2计算帧CRC校验值:

```
 
ffmpeg -i INPUT -c:a pcm_u8 -c:v mpeg2video -f framecrc -
```

参看[crc]混合器

### framemd5

每个数据包MD5校验值

计算输出每个数据包MD5校验值，默认音频被转换成16bit符号原始音频，视频被转换成原始视频再进行MD5计算

每个数据包计算对应输出一行如下格式数据:

```
 
stream_index, packet_dts, packet_pts, packet_duration, packet_size, MD5
```

其中MD5就是计算出的MD5 哈希值

framemd5例子

计算INPUT输入的帧md5值，其中音频被转换成16bit符号原始音频数据，视频被转换成原始视频数据，输出到out.md5

```
 
ffmpeg -i INPUT -f framemd5 out.md5
```

直接输出到标准输出设备：

```
 
ffmpeg -i INPUT -f framemd5 -
```

参考[md5]混合器部分

### gif

Animated GIF（动画GIF）混合器

它接受如下选项：

- loop

  设置循环次数，-1表示不循环，0表示一直循环（默认值）

- final_delay

  强制最后一帧延迟 (以厘秒为单位——centiseconds) ，默认为1，这是一个对于循环gif的特殊设定，它为最后一帧播放到新开始播放设置一个特殊的值，比如你可能希望有一个停顿的感觉。

  例如像循环10次，每次重新播放前停顿5秒，则：

  ffmpeg -i INPUT -loop 10 -final_delay 500 out.gif

**注意1**如果你想提取帧到指定的GIF文件序列，你可能需要`image2`混合器

```
 
ffmpeg -i INPUT -c:v gif -f image2 "out%d.gif"
```

**注意2**Gif格式有一个非常小的时基：两帧之间的间隔不可小于百分之一秒。

### hls

Apple HTTP 直播流混合器，它根据HTTP直播流（HLS）规范进行MPEG-TS分割

它创建一个播放列表文件，包括1个或者多个分段文件，输出文件为指定的播放列表文件。

默认混合器对每段创建一个文件，这些文件有相同的基于播放列表的文件名，段索引数和.tx扩展名

例如，转一个输入文件：

```
 
ffmpeg -i in.nut out.m3u8
```

这将根据产品播放列表文件out.m3u8产生分段文件:out0.ts out1.ts out2.ts 等等

参考[segment]混合器，它提供了更多可用于HTL分割的常规处理和修正介绍

hls选项

这个混合器支持如下选项

- hls_time seconds

  设置段长度，单位秒，默认为2

- hls_list_size size

  设置播放列表中字段最大数。如果为0，则包含所有分段。默认为5

- hls_ts_options options_list

  设置输出格式选项，使用'-'分割的`key=value`参数对，如果包括特殊字符需要被转义处理

- hls_wrap wrap

  一种循环机制，设置数量后以0-设定数形成一个环依次循环使用作为输出段号.为0表示不限制， 默认为0

  选项可避免磁盘被多个段文件填满，并限制写入磁盘的最大文件数

- start_number number

  设置播放列表中最先播放的索引号，默认 0.

- hls_allow_cache allowcache

  设置客户端是否：可能(1) 或 必须不 (0) 缓冲媒体段

- hls_base_url baseurl

  对每个列表中的记录添加一个基本的URL，一般用于采用相对路径描述的列表

  **注意**列表序号必须是每段独特的，不可分割的文件名和序列号，序列号是可循环的，则可能会引起困惑，例如hls_wrap选项设置了

- hls_segment_filename filename

  设置段文件名。除非`hls_flags` `single_file`被设置，设置这个文件名可以用于段命名格式化（依据段序数）:

  ffmpeg in.nut -hls_segment_filename 'file%03d.ts' out.m3u8

  这个例子中，段文件会输出为: file000.ts, file001.ts, file002.ts, 等等，而不是默认的out0.ts out1.ts out2.ts 等等

- hls_key_info_file key_info_file

  使用key_info_file对段进行加密。`key_info_file` 中的第一行指定一个URI，是写入播放列表的，这个key URL被用于存放播放期访问的加密密钥。第二行指定用于加密过程中的key文件路径。key文件作为一个单一排列的16进制数组以二进制格式数据读入。可选的第三行则指定初始化向量（IV，一个十六进制字符串用于代替部分序列（默认）进行加密）。改变key_info_file将导致段加密采用新的key/IV 以及播放列表中任意条目采用新的 URI/IV

  key_info_file 格式:

  > key URI key file path IV (optional)

  key URIs 例子:

  > <http://server/file.key> /path/to/file.key file.key

  key文件路径例子:

  > file.key /path/to/file.key

  IV例子:

  > 0123456789ABCDEF0123456789ABCDEF

  完整key_info_file 示例:

  > <http://server/file.key> /path/to/file.key 0123456789ABCDEF0123456789ABCDEF

  shell脚本例子:

  > ```
  >  
  > ```
  >
  > `#!/bin/sh`
  >
  > `BASE_URL=${1:-'.'}`
  >
  > `openssl rand 16 > file.key`
  >
  > `echo $BASE_URL/file.key > file.keyinfo`
  >
  > `echo file.key >> file.keyinfo`
  >
  > `echo $(openssl rand -hex 16) >> file.keyinfo`
  >
  > `ffmpeg -f lavfi -re -i testsrc -c:v h264 -hls_flags delete_segments \`
  >
  > `-hls_key_info_file file.keyinfo out.m3u8`

- hls_flags single_file

  如果这个标记被设置，则会把所有段存储到一个MPEG-TS文件中，且在播放列表中使用字节范围。 HLS播放列表在版本4中支持这种方法：

  ffmpeg -i in.nut -hls_flags single_file out.m3u8

  这里所有的输出都放置在out.ts中了

- hls_flags delete_segments

  在播放的段已经过了持续时间后就删除掉对应的文件。

### ico

ICO文件混合器

微软ICON（ICO）文件格式有一些限制需要注意：

- 每个方向不超过256像素

- 仅BMP和PNG图像可以被存储

- 如果是BMP图像，必须有如下像素格式：

  > bmp位深度 ffmpeg像素格式
  > 1bit pal8
  > 4bit pal8
  > 8bit pal8
  > 16bit rgb555le
  > 24bit bgr24
  > 32bit bgra

- 如果是BMP图像，必须有 BITMAPINFOHEADER DIB 头

- 如果是PNG图像，必须是rgba像素格式

### image2

图像文件混合器

它可以把视频帧重新混合为图像文件

输出文件按模板指定，可以设置成为一个序列数文件。模板中的"%d" 或者 "%0Nd"用于指定序列，其中"%0Nd"表示N位数字，以0补齐。如果文件名中有“%”需要以“%%”转义的形式指定。

如果模板中包含了"%d"或者"%0Nd"则文件名从1计数输出序列

模板可以包含一个后缀用来自动确定图像文件格式

例如模板"img-%03d.bmp"将使得输出为img-001.bmp, img-002.bmp, ...,img-010.bm 等等。而模板"img%%-%d.jpg"则生成img%-1.jpg, img%-2.jpg, ..., img%-10.jpg,等等

image2例子

把输入视频图像帧生成为img-001.jpeg, img-002.jpeg, ...,

```
 
ffmpeg -i in.avi -vsync 1 -r 1 -f image2 'img-%03d.jpeg'
```

**注意**ffmpeg如果没有通过`-f`指定输出文件格式，image2混合器将自动被选择，所以前面的等效于

```
 
ffmpeg -i in.avi -vsync 1 -r 1 'img-%03d.jpeg'
```

如果`strfime`选项允许你导出按时间/日期信息命名的文件 "%Y-%m-%d_%H-%M-%S" 模板，在`strftime()`的文档中了解相关语法

例如：

```
 
ffmpeg -f v4l2 -r 1 -i /dev/video0 -f image2 -strftime 1 "%Y-%m-%d_%H-%M-%S.jpg"
```

image2选项

- start_number

  设置开始序列的数字，默认为0

- update

  如果设置为1，文件名直接作为唯一文件名，而没有模板。即相应的文件被不断改写为新的图像。默认为0

- strftime

  如果设置为1，可以让输出文件支持strftime()提供的日期格式，默认为0

这个图像混合器支持.Y.U.V图像文件格式，这种格式将根据每帧输出3个文件，对于每个YUV420P压缩，对于读或者写这种文件格式，只需要指定.Y文件即可，混合器会自动打开需要的.U和.V文件

### matroska

Matroska内容混合器

混合输出matroska和webm内容

matroska元数据

混合器需要指定一些必要元数据

- title

  设置单个轨道的标题名 language

  以Matroska语言字段指定语言

  语言可是3个字符（依ISO-639-2 (ISO 639-2/B)） (例如 "fre" 表示法语——French),或者语言混合国家/地区代码，(like "fre-ca" 表示加拿大法语——Canadian French). stereo_mode

  设置3D视频两个视图在单个视频轨道播放时的布局规则

  允许如下值:

  ‘mono’

  ```
   
  ```

  `video不是双路的`

  ‘left_right’

  ```
   
  ```

  `两路分别一端，即左眼看左视图，右眼看右视图`

  ‘bottom_top’

  ```
   
  ```

  `上下布局，左眼看下视图，右眼看上视图`

  ‘top_bottom’

  ```
   
  ```

  `与上一个相反，左眼看上，右眼看下`

  ‘checkerboard_rl’

  ```
   
  ```

  `根据序列确认，左眼看第一个`

  ‘checkerboard_lr’

  ```
   
  ```

  `根据序列确认，右眼看第一个`

  ‘row_interleaved_rl’

  ```
   
  ```

  `根据行序列确认，右眼看第一行`

  ‘row_interleaved_lr’

  ```
   
  ```

  `根据行序列确认，左眼看第一行，`

  ‘col_interleaved_rl’

  ```
   
  ```

  `列序列确认，右眼第一列`

  ‘col_interleaved_lr’

  ```
   
  ```

  `列序列确认，左眼第一列`

  ‘anaglyph_cyan_red’

  ```
   
  ```

  `都在一副图中，通过颜色滤镜过滤red-cyan filters`

  ‘right_left’

  ```
   
  ```

  `右眼看左图`

  ‘anaglyph_green_magenta’

  ```
   
  ```

  `混合图，通过green-magenta滤镜看`

  ‘block_lr’

  ```
   
  ```

  `间隔块，左眼先看`

  ‘block_rl’

  ```
   
  ```

  `间隔块，右眼先看`

例如，对于3DWebM影片，可以由下面命令建立：

```
 
ffmpeg -i sample_left_right_clip.mpg -an -c:v libvpx -metadata stereo_mode=left_right -y stereo_clip.webm
```

matroska选项

支持如下选项：

- reserve_index_space

  默认对于定位索引（可以被Matoska调用）将写到文件的末尾部分，因为一开始不知道需要多少空间放置索引。但这将导致流式播放时定位特别慢（因为不知道定位索引），这个选项将把索引放置到文件的开始。

  如果这个选项设置为非0值，混合器将预先在头部放置一个用于写入索引的空间，但如果空间无效则将混合失败。一个较安全的值是大约1小时50KB。

  **注意**这些寻址线索仅当输出文件是可寻址且选项设置了有效值时写入。

### md5

MD5检测格式

将计算输出一个MD5值，对于所有的音视频帧。默认音频帧转换为有符号16bit原始音频，视频转换为原始视频来计算。

输出是一个MD5=`MD5` 格式，其中`MD5`就是计算出的值。

例如：

```
 
ffmpeg -i INPUT -f md5 out.md5
```

也可以输出到标准输出设备

```
 
ffmpeg -i INPUT -f md5 -
```

参考[framemd5]混合器

### mov,mp4,ismv

MOV/MP4/ISMV (Smooth Streaming——平滑流)混合器

MOV/MP4/ISMV混合器支持零碎文件（指数据的组织形式）。通常MOV/MP4文件把所有的元数据存储在文件的一个位置中（这是不零碎的数据组织形式，通常在末尾，也可以移动到起始以更好的支持随机定位播放，比如使用`qt-faststart`工具，并添加`movflags`快速启动标志）。这样一个零碎文件包含了很多片段，其中数据包和元数据是存储在一起的。这样零碎数据组织的文件在解码到写中断（普通的MOV/MP4则不能解码了，因为可能缺少元数据）时也能正常解码，而且这种方式要求更少的内存就可以写很大的文件（因为普通形式的MOV/MP4需要收集所有的信息才能最终完成元数据集中存储，则这一过程中这些数据一直需要缓存在内存中，直到编码完成，元数据完成存储），这是一个优势。缺点是这种组织数据的格式不太通用（很多程序不支持）

mov,mp4,ismv选项

零碎形式也支持AVOtions，它可以定义如何切分文件到零碎片段中：

- -moov_size bytes

  在文件开头设置预留空间用于存储moov原子数据（一些元数据），而不是把这些数据存储在文件尾部。如果预设的空间不够，将导致混合失败

- -movflags frag_keyframe

  在每个关键帧都开始一个新的碎片

- -frag_duration duration

  每duration microseconds时长就创建一个碎片

- -frag_size size

  碎片按size字节（这是一个上限）进行划分

- -movflags frag_custom

  允许调用者手动切片，通过调用av_write_frame(ctx, NULL) 在当前位置写入一个片段(它仅能与libavformat库集成，在ffmpeg中不支持)

- -min_frag_duration duration

  如果少于duration microseconds就不单独创建片段

  如果指定了多个条件，当一个条件满足是，片段被切分出来。例外的是`-min_frag_duration`, 它在任何其它条件满足时都使用来进行判断

此外，输出还可以通过一些其他选项进行调整：

- -movflags empty_moov

  写入一个空的moov atom到文件开始,而没有任何样品描述。一般来说，一个mdat/moov在普通MOV/MP4文件开始时写入，只包括了很少的内容，设置了这个选项将没有初始的moov atom，而仅是一个描述了轨道，但没有持续时间的moov atom。

  这个选项在ismv文件中隐式设定

- -movflags separate_moof

  为每个轨道写独立的moof（电影片段）atom。通常，追踪所有分组是写在一个moof atom中，而通过这个选项，混合器将对每个轨道单独写moof/MDAT，以方便轨道间隔离

  这个选项在ismv文件中隐式设定

- -movflags faststart

  再次移动index（moov atom）到文件开始位置。这个选项可以与其他选项一起工作，除了碎片化输出模式。默认情况是不允许

- -movflags rtphint

  添加RTP打标轨道到输出文件中

- -movflags disable_chpl

  禁止Nero章标签(chpl atom)。通常，Nero章标签和QuickTime章标签都被写入到文件中，通过这个选项，可以强制只输出QuickTime标签。Nero章标签可能导致文件在某些程序处理标签时失败，例如 mp3Tag 2.61a 和 iTunes 11.3，可能其他版本也会受到影响

- -movflags omit_tfhd_offset

  在thfd atom（原子数据）中不写入任何绝对`base_data_offset`。这将避免片段文件/流中的绝对定位绑定

- -movflags default_base_moof

  类似`omit_tfhd_offset`，这个标志避免在tfhd atom中写绝对`base_data_offset`，而是用新的`default-base-is-moof`，这个标志定义在14496-12:2012 。它会使片段在某些情况下更容易被解析（避免通过在前一轨道片段基础上隐式进行追踪计算碎片位置）

mov,mp4,ismv例子

平滑流内容可以通过IIS进行发布，例如：

```
 
ffmpeg -re <normal input/transcoding options> -movflags isml+frag_keyframe -f ismv http://server/publishingpoint.isml/Streams(Encoder1)
```

### mp3

MP3混合器通过下面选项写原始的MP3流：

- 一个ID3v2元数据头会写在开始处（默认），支持版本2.3和2.4， `id3v2_version`私有选项可以使用(3或4)，如果设置`id3v2_version`为0表示禁用ID3v2头

  混合器还支持附加图片（APIC帧）到ID3v2头。这个图片以单一分组视频流的形式提供给混合器。可以有任意数量的这种流，每个都是单独的APIC帧。对于APIC帧的描述和图片类型要求，以及流元数据标题及内容提交者 等参考<http://id3.org/id3v2.4.0-frames>。

  **注意**APIC帧必须写在开始的地方，所以混合器会缓冲音频帧直到所有的图片已经获取完成。因此建议尽快提供图片，以避免过度缓冲。

- Xing/LAME帧正确放置在ID3v2头之后(如果提供)。它也是默认的，但仅仅在输出是可定位情况下写入。`write_xing`私有选项可以用来禁用它。这些帧中包括的变量信息通常用于解码器，例如音频持续时间或者编码延迟

- 一个遗留的ID3v1标签放置在文件的末尾（默认禁止），它可以通过`write_id3v1`私有选项来启用，但其意义非常有限，所以不建议采用

一些例子：

- 写一个mp3，有ID3v2.3 头和ID3v1的末尾标签

  ffmpeg -i INPUT -id3v2_version 3 -write_id3v1 1 out.mp3

- 通过`map`附加图片到音频：

  ffmpeg -i input.mp3 -i cover.png -c copy -map 0 -map 1 -metadata:s:v title="Album cover" -metadata:s:v comment="Cover (Front)" out.mp3

- 写入一个"干净"的MP3，而没有额外特性

  ffmpeg -i input.wav -write_xing 0 -id3v2_version 0 out.mp3

### mpegts

MPEG传输流混合器

这个混合器声明在 ISO 13818-1 和ETSI EN 300 468的部分内容中.

对于通用的元数据设置`service_provider`和`service_name`，如果没有特别指明，则默认`service_provider`为"FFmpeg",`service_name`为"Service01"

mpegts 选项

mpegts混合器选项有：

- -mpegts_original_network_id number

  设置`original_network_id` (默认0x0001). 在DVB是一个唯一的网络标识，它用于标识特殊的服务（通过Original_Network_ID和Transport_Stream_ID）

- -mpegts_transport_stream_id number

  设置`transport_stream_id` (默认0x0001).在DVB是一个传输的标识

- -mpegts_service_id number

  设置`service_id` (默认0x0001),在DVB作为程序标识 DVB.

- -mpegts_service_type number

  设置程序`service_type` (默认digital_tv), 参考下面预设值

- -mpegts_pmt_start_pid number

  对PMT设置第一个PID (默认 0x1000,最大0x1f00).

- -mpegts_start_pid number

  对数据包设置第一个PID(默认0x0100,最大0x0f00).

- -mpegts_m2ts_mode number

  如果设置为1则允许`m2ts`模式，默认为-1，表示禁止 value is -1 which disables m2ts mode.

- -muxrate number

  设置内容为混合码率（默认VBR）

- -pcr_period numer

  覆盖默认的PCR重传时间（默认20ms），如果`muxrate`被设置将会被忽略

- -pes_payload_size number

  以单位字节设置最小PES播放加载包大小

- -mpegts_flags flags

  设置一个标志(后面介绍).

- -mpegts_copyts number

  如果设置为1则保留原始时间戳。默认为-1，将从0开始更新时间戳

- -tables_version number

  设置PAT, PMT 和SDT版本 (默认0,范围0-31) 。这个选项允许更新流结构， 以便用户可以检测到更改。比如在打开AVFormatContext （API使用时）或重启FFMPEG来周期性改变`tables_version`时:

  ffmpeg -i source1.ts -codec copy -f mpegts -tables_version 0 udp://1.1.1.1:1111
  ffmpeg -i source2.ts -codec copy -f mpegts -tables_version 1 udp://1.1.1.1:1111
  ... ffmpeg -i source3.ts -codec copy -f mpegts -tables_version 31 udp://1.1.1.1:1111
  ffmpeg -i source1.ts -codec copy -f mpegts -tables_version 0 udp://1.1.1.1:1111
  ffmpeg -i source2.ts -codec copy -f mpegts -tables_version 1 udp://1.1.1.1:1111
  ...

选项`mpegts_service_type`接受如下值:

- hex_value

  一个16进制值，范围0x01到0xff，定义在 ETSI 300 468.

- digital_tv

  数字TV服务

- digital_radio

  数字广播服务

- teletext

  图文电视服务

- advanced_codec_digital_radio

  高级编码数字广播服务

- mpeg2_digital_hdtv

  MPEG2数字HDTV服务

- advanced_codec_digital_sdtv

  高级编码数字SDTV服务

- advanced_codec_digital_hdtv

  高级编码数字HDTV服务

选项`mpegts_flags`可以设置如下标志:

- resend_headers

  写下一个包前反弹PAT/PMT

- latm

  对AAC编码使用LATM打包

mpegts例子

```
 
ffmpeg -i file.mpg -c copy \
-mpegts_original_network_id 0x1122 \
-mpegts_transport_stream_id 0x3344 \
-mpegts_service_id 0x5566 \
-mpegts_pmt_start_pid 0x1500 \
-mpegts_start_pid 0x150 \
-metadata service_provider="Some provider" \
-metadata service_name="Some Channel" \
-y out.ts
```

### null

Null混合器

这个混合器将不产生任何输出文件，通常用于测试和基准检测

例如要检测一个解码器，你可以使用：

```
 
ffmpeg -benchmark -i INPUT -f null out.null
```

**注意**前面的命令行并不读写out.null，仅仅是因为ffmpeg语法要求必须有个输出

等效的，你可以采用：

```
 
ffmpeg -benchmark -i INPUT -f null -
```

### nut

- -syncpoints flags

  利用nut改变同步点：

  - default:默认采用低开销的定位模式。没有不使用同步点的，但可减少开销，只是流是不可定位的。

  - none:一般不建议采用这个选项，因为它导致文件是损坏敏感的（稍微破坏就不能正常解码了），且不可定位。一般同步点开销是很小以至于可以忽略的。**注意**`-write_index 0`可用于禁止所有增长的数据表，允许重复使用有效的内存，而没有这些缺点。

  - timestamped:时间戳字段扩展来与时钟同步。

    `none`和`timestamped`还处于试验阶段

- -write_index bool

  在最后写索引，这是写索引的默认值

  ffmpeg -i INPUT -f_strict experimental -syncpoints none - | processor

### ogg

Ogg内容混合器

- -page_duration duration

  首选页面持续时间（其实是定位点间隔），单位microseconds。混合器将尝试按设定时间创建页面。这允许用户在定位和容器粒度开销间进行平衡。默认1秒。如果设为0， 将填充所有字段，使索引数据很大。在大多数情况下，设为1将使得每个页面1个数据包，且可以有一个很小的定位粒度，但将产生额外的容器开销（文件变大）

- -serial_offset value

  用于设置流序号的一些值。设置来不同且足够大，可以保证产生的ogg文件可以安全的被锁住

### segment, stream_segment, ssegment

基本流分段

混合器将输出流到指定的文件（根据最接近的持续时间分段）。输出文件名模板可以采用类似与[image2]的方式，或者使用`strftime`模板（如果`strftime`选项被允许）

`stream_segment`是用于流式输出格式的混合器变种，例如不需要全局头，并要求诸如MPEG传输流分段输出的情况。`ssegment`是`stream_segment`的别名。

每个片段都开始于所选流的关键帧，这是通过`reference_stream`选项设置的

**注意**如果你想精确分割视频文件，你需要准确输入按关键帧整数倍对应的预期分割器，或者指定混合器按新片段必须是关键帧开始。

分段混合器对于固定帧率的视频有更好的工作表现

或者它可以生成一个创建段的列表，这需要通过`segment_list`选项设置，列表的类型由`segment_list_type`选项指定。在段列表输入一个文件名被默认为相应段文件的基本名称。

参看[hls]混合器，其提供更多关于`HLS`分段的特定实现

segment, stream_segment, ssegment选项

segment混合器器支持如下选项：

- reference_stream specifier

  由字符串指定参考流，如果设置为`auto`将自动选择参考流。否则必须指定一个流（参看 流说明符 章节）作为参考流。默认为`auto`

- segment_format format

  覆盖内容自身格式。默认根据文件扩展名检测（猜测）

- segment_format_options options_list

  使用“：”分隔的`key=value`列表作为选项参数以一次定义多个选项，其中值如果包含“：”等特殊符号需进行转义

- segment_list name

  指定生成文件的名字列表。如果不指定将没有列表文件生成。

- segment_list_flags flags

  设置影响生成段序列的标志

  可以有下面的标志：

  - ‘cache’

    允许缓存（只能用于M3U8列表文件).

  - ‘live’

    允许直播友好文件生成

- segment_list_type type

  选择列格式Select the listing format.

  `flat`使用简单的 flat列表单元
  `hls` 使用类似m3u8的结构

- segment_list_size size

  当列表文件包含了指定个数段后更新文件,如果为0则列表文件会包含所有的段，默认为0

- segment_list_entry_prefix prefix

  对每条记录添加一个前导修饰。常用于生成绝对路径。默认没有前导添加

  下面的值被允许:

  - ‘flat’

    按`flat`列表生成段，每行一个段

  - ‘csv, ext’

    按列表生成段，每行一段，每行按如下格式(逗号进行分割)，:

    segment_filename,segment_start_time,segment_end_time

    `segment_filename`是输出文件名字，混合器根据提供的模板产生输出文件名(参考 RFC4180)

    `segment_start_time`和`segment_end_time`指定段开始和结束时间，单位秒

    文件列表如果以 ".csv" 或 ".ext"作为扩展名，将自动匹配这个列表格式

    ‘ext’是对不喜欢 ‘csv’的替代

  - ‘ffconcat’

    分析ffconcat文件生成段。 file for the created segments. The resulting file can be read using the FFmpeg concat demuxer.

    列表文件以".ffcat"或".ffconcat"作为扩展名时会自动选择这个格式

  - ‘m3u8’

    分析M3U8的文件，版本3, 符合<http://tools.ietf.org/id/draft-pantos-http-live-streaming>

    如果列表文件有".m3u8"扩展名将自动选择这个格式

    如果不指定就从文件扩展名中进行猜测

- segment_time time

  设置段持续时间，这个值必须指定，默认为2，参考`segment_times`选项.

  **注意**划分可能不太精确，除非强制到流中关键帧间隔时间。参考下面的例子

- segment_atclocktime 1|0

  如果设置为"1"，将从00:00开始计时，利用`segment_time`为间隔划分出多个段

  例如如果`segment_time`设置为"900"，这个选项将在12:00、12:15, 12:30等时间点创建 文件

  默认为"0".

- segment_time_delta delta

  指定一个时间作为段开始时间, 其表示为一个时间规范, 默认为"0".

  当`delta`被指定，关键帧将开始一个新的段以使PTS满足如下关系:

  PTS >= start_time - time_delta

  这个选项通常用来划分视频内容，其总是在GOP边界划分，它在指定点前找到一个关键帧来划分

  它可以结合ffmpeg的`force_key_frames`选项，通过`force_key_frames`可以强制指定一个时间点的是关键帧而不是自动计算。因为四舍五入的原因关键帧时间点可能不是很精确，而可能在设置的时间点之前。对于恒定帧率的视频，在实际值和依`force_key_frames`设定值间最坏有1/(2*frame_rate)的差值

- segment_times times

  指定一个划分点的列表。列表是逗号分隔的升序列表，每个是持续时间。也可以参考`segment_time`选项

- segment_frames frames

  指定划分视频帧的序号列表。列表以逗号分隔的升序列表

  这个选项指定一个新段开始于参考流关键帧和序列（从0开始），下个值则需要表明下一个段切分点

- segment_wrap limit

  环形索引限制

- segment_start_number number

  设置片段开始序号，默认为0

- strftime 1|0

  定义是否使用`strftime`功能来产生新段。如果设置了，输出段名需要依模板由`strftime`生成，默认为0.

- break_non_keyframes 1|0

  如果设置为允许，将允许段在非关键帧点切分。这将改善一下关键帧间隔不一致的播放，但会产生很多奇怪的问题。默认为0

- reset_timestamps 1|0

  在每个段都重新开始时间戳。所以每个段都有接近于0的时间戳。这有利于片段的播放，但很多混合器/编码器不支持, 默认为0

- initial_offset offset

  指定时间戳抵消适用于输出包的时间戳。参数必须是一个时间规范,默认为 0.

segment, stream_segment, ssegment例子

- 重新混合输入的in.mkv生成out-000.nut, out-001.nut, ......列表，并且把生成文件的列表写入out.list:

  ffmpeg -i in.mkv -codec copy -map 0 -f segment -segment_list out.list out%03d.nut

- 按输出格式、选项分拆输入:

  ffmpeg -i in.mkv -f segment -segment_time 10 -segment_format_options movflags=+faststart out%03d.mp4

- 按指定时间点分（由`segment_times`进行指定）拆输入文件

  ffmpeg -i in.mkv -codec copy -map 0 -f segment -segment_list out.csv -segment_times 1,2,3,5,8,13,21 out%03d.nut

- 使用`force_key_frames`选项强制关键点进行切分，还指定了`segment_time_delta` 来处理计算机进程。时间点不够精确

  ffmpeg -i in.mkv -force_key_frames 1,2,3,5,8,13,21 -codec:v mpeg4 -codec:a pcm_s16le -map 0 \
  -f segment -segment_list out.csv -segment_times 1,2,3,5,8,13,21 -segment_time_delta 0.05 out%03d.nut

  为了强制关机帧，必须进行转码

- 按帧号进行分段 ，由`segment_frames`选项指定了若干帧号:

  ffmpeg -i in.mkv -codec copy -map 0 -f segment -segment_list out.csv -segment_frames 100,200,300,500,800 out%03d.nut

- 转换in.mkv 为TS段，并且采用了libx264 和 libfaac 编码器:

  ffmpeg -i in.mkv -map 0 -codec:v libx264 -codec:a libfaac -f ssegment -segment_list out.list out%03d.ts

- 对输入分段，创建了M3U8直播列表 (可以作为HLS直播源):

  ffmpeg -re -i in.mkv -codec copy -map 0 -f segment -segment_list playlist.m3u8 \
  -segment_list_flags +live -segment_time 10 out%03d.mkv

### smoothstreaming

平滑流混合器生成一组文件（清单、块），适用于传统web服务器

- window_size

  指定清单中保留的片段数。默认是0，表示保留所有的

- extra_window_size

  从磁盘移除前，保留清单外片段数，默认5

- lookahead_count

  指定先行片段数，默认2

- min_frag_duration

  指定最小片段持续时间（单位microseconds），默认5000000.

- remove_at_exit

  指定完成后是否移除所有片段，默认0，表示不移除

### tee

tee混合器可以用于同时把相同数据写入多个文件，或者任何其他类型的混合器。例如使用它可以同时把视频发布到网络上以及保存到磁盘上。

它不同于在命令行指定多个输出，因为利用tee混合器，音频和视频数据只被编码了一次，而编码是一个非常昂贵的行为。它是很有效的，当利用libavformat的API直接可以把相同的数据包用于多个混合器输出（多种封装格式或者场景）

多个输出文件由’|’分隔，如果参数中包含任意前导或尾随的空格，任何特殊字符都必须经过转义(参考 ffmpeg-utils(1)手册中的中 "Quoting and escaping" 章节).

混合器的选项可以由被“：”分隔的`key=value`列表进行指定。如果这种形式下选项参数值包含特殊字符，例如“：”则必须被转义。**注意**这个第二层次的转义

下列选项被要求:

- f

  指定格式名，通常用于不能由输出名后缀推测格式的情况

- bsfs[/spec]

  指定一个比特流滤镜应用到指定的输出

  它可以为每个流指定一个比特流滤镜，通过"/"添加一个流选择（说明符），有些流必须由说明符进行指定(格式规范见流说明符)。 如果流说明符没有指定，则比特流滤镜适用于所有输出流。

  可以同时指定多个比特流滤镜，用","分隔。

- select

  选择一些流，它们可以映射到一些输出，通过流说明符进行指定。如果没有指定，则默认会选择所有输入流

tee例子

- 同时编码到WebM文件和UDP协议上的MPEG-TS流(流需要明确的被映射):

  ffmpeg -i ... -c:v libx264 -c:a mp2 -f tee -map 0:v -map 0:a

  ```
   
  ```

  `"archive-20121107.mkv|[f=mpegts]udp://10.0.1.255:1234/"`

- 使用ffmpeg编码输入，有3个不同的目标。`dump_extra`比特流滤镜被用来为所有输出的视频关键帧添加额外的信息，其作为MPEG-TS格式的要求。对out.aac附加的选项是为了让它只包含音频。

  ffmpeg -i ... -map 0 -flags +global_header -c:v libx264 -c:a aac -strict experimental

  ```
   
  ```

  `-f tee "[bsfs/v=dump_extra]out.ts|[movflags=+faststart]out.mp4|[select=a]out.aac"`

- 下面，将只选择一个音频流给音频输出。**注意**第二层引号必须经过转义，":"作为特殊字符被用于标识选项

  ffmpeg -i ... -map 0 -flags +global_header -c:v libx264 -c:a aac -strict experimental

  ```
   
  ```

  `-f tee "[bsfs/v=dump_extra]out.ts|[movflags=+faststart]out.mp4|[select=\'a:1\']out.aac"`

  **注意**一些编码器会根据输出格式的不同要求不同的选项，在tee混合器下自动检测可能会失效。主要有`global_header`的例子

### webm_dash_manifest

WebM DASH 清单混合器.

这个混合器实现了按WebM DASH清单规范生成DASH清单XML文件。它还支持生成DASH直播流

更多参考:

- WebM DASH Specification: <https://sites.google.com/a/webmproject.org/wiki/adaptive-streaming/webm-dash-specification>
- ISO DASH Specification: <http://standards.iso.org/ittf/PubliclyAvailableStandards/c065274_ISO_IEC_23009-1_2014.zip>

webm_dash_manifest选项

支持如下选项:

- adaptation_sets

  这个选项参数有如下语法： "id=x,streams=a,b,c id=y,streams=d,e" 这里的x，y都是唯一合适设置的标识符，a,b,c,d和e是相应的音频和视频流的指代。任何合适的数字可以被用于这个选项。

- live

  如果为1表示创建一个直播流DASH，默认为0

- chunk_start_index

  第一个块的索引号，默认为0，它将作为清单中‘SegmentTemplate’元素的 ‘startNumber’ 属性值

- chunk_duration_ms

  每个块的持续时间，单位milliseconds,默认1000，将作为清单中‘SegmentTemplate’元素的‘duration’属性值

- utc_timing_url

  URL将指示从何处获取UTC时间戳（ISO格式的），它作为清单中 ‘UTCTiming’元素的‘value’ 属性值，默认： None.

- time_shift_buffer_depth

  最小时间（单位秒）的移动缓冲区，为保障可用的任意值，作为清单中‘MPD’元素的‘timeShiftBufferDepth’属性值，默认： 60.

- minimum_update_period

  清单最小更新时间 (单位秒)， 清单中‘MPD’元素的 ‘minimumUpdatePeriod’ 属性值，默认: 0.

webm_dash_manifest例子

```
 
ffmpeg -f webm_dash_manifest -i video1.webm \
-f webm_dash_manifest -i video2.webm \
-f webm_dash_manifest -i audio1.webm \
-f webm_dash_manifest -i audio2.webm \
-map 0 -map 1 -map 2 -map 3 \
-c copy \
-f webm_dash_manifest \
-adaptation_sets "id=0,streams=0,1 id=1,streams=2,3" \
manifest.xml
```

### webm_chunk

WebM直播块混合器

这个混合器输出WebM头和块分离文件，通过DASH它可以被支持WebM直播流的客户端处理。

webm_chunk选项

支持如下选项:

- chunk_start_index

  第一个块的序号，默认0

- header

  文件名将写入初始化数据的头

- audio_chunk_duration

  每个音频块时间，单位milliseconds (默认5000).

webm_chunk例子

```
 
ffmpeg -f v4l2 -i /dev/video0 \
-f alsa -i hw:0 \
-map 0:0 \
-c:v libvpx-vp9 \
-s 640x360 -keyint_min 30 -g 30 \
-f webm_chunk \
-header webm_live_video_360.hdr \
-chunk_start_index 1 \
webm_live_video_360_%d.chk \
-map 1:0 \
-c:a libvorbis \
-b:a 128k \
-f webm_chunk \
-header webm_live_audio_128.hdr \
-chunk_start_index 1 \
-audio_chunk_duration 1000 \
webm_live_audio_128_%d.chk
```

 

 

 

 

 

 

 

 

 

```html
 
```