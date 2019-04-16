# ffmbc——为广播电视以及专业用途量身定制的FFmpeg - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年02月16日 00:28:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：82
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










无意中发现了个为广播电视以及专业用途量身定制的FFmpeg。也是开源的，记录之。

ffmbc 全称是 FFMedia Broadcast，是个改版的FFmpeg，有如下功能：

> 创建可以导入Final Cut Pro, AVID Media Composer, Adobe Premiere中的文件
创建 XDCAM HD422 文件（.mov 或 .mxf）
创建 XDCAM IMX/D-10 文件（.mov 或 .mxf）
创建 AVID DNxHD 文件（.mov）
创建 DVCPROHD 文件（.mov 或 .mxf）
创建 ProRes 422 or 4444 文件（.mov）
转换 V210 文件
转换 HD YUV BT709 到 SD BT601 或者反过来
转换 AVCIntra 50 和AVCIntra 100
转换 MPEG-TS 文件
转换 AVCHD 文件
转换 ProRes 422 和 4444 文件
合并或分离音轨
创建 Quicktime 文件
从 HD 到 SD的颜色转换



等等。。。






项目主页上没有可执行文件，只有一个源代码压缩包。自己拿MinGW编译了一下源代码，发现和FFmpeg编译是一样的，编译后生成ffmbc.exe。

上传一个自己编译的ffmbc：[http://download.csdn.net/detail/leixiaohua1020/6908055](http://download.csdn.net/detail/leixiaohua1020/6908055)

项目主页：[http://code.google.com/p/ffmbc/](http://code.google.com/p/ffmbc/)





更新记录（2014.7.28）======================





前一段时间受朋友之托，研究了一下ffmbc的使用，最后做成了一个使用说明。自己的感触是，ffmbc最大的改动就在于增加了一个-target选项。



ffmbc使用说明





### 1.编码IMX

编码视频为 IMX 30/50 MXF 或 MOV



#### 1.1.命令行格式

输出Imx30


`ffmbc -i < file > -target imx30 < file.mxf | file.mov >`

输出Imx50


`ffmbc -i < file > -target imx50 < file.mxf | file.mov >`

注意：在MXF中，XDCAM要求至少为4声道。所以如果输入文件的音频流小于4声道，输出文件依然会被标记为4声道。

Target命令支持以下格式："vcd", "svcd", "dvd", "dvcam", "dvcpro", "dvcpro50", "dvcprohd", "imx30", "imx50", "xdcamhd422"




#### 1.2.示例

命令（转码为IMX30，MXF格式）：


`ffmbc -i test.flv -target imx30 test.mxf`

转码前文件格式：

```
概要
完整名称                                     : E:\test\test.flv
文件格式                                     : Flash Video
文件大小                                     : 2.24 MiB
长度                                       : 30秒 0ms
平均混合码率                                   : 627 Kbps


视频
文件格式                                     : Sorenson Spark
编码设置ID                                   : 2
长度                                       : 30秒 0ms
码率                                       : 562 Kbps
画面宽度                                     : 320像素
画面高度                                     : 180像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
位深度                                      : 8位
bits/(pixel*frame)(数据密度)                 : 0.390
大小                                       : 2.01 MiB (90%)


音频
文件格式                                     : MPEG Audio
格式版本                                     : Version 1
格式简介                                     : Layer 3
模式                                       : Joint stereo
扩展模式                                     : MS Stereo
编码设置ID                                   : 2
编码设置ID/提示信息                              : MP3
长度                                       : 29秒 858ms
码率模式                                     : CBR
码率                                       : 32.0 Kbps
声道                                       : 2声道
采样率                                      : 44.1 KHz
压缩模式                                     : 有损压缩
大小                                       : 117 KiB (5%)
编码函数库                                    : LAME3.98
```


转码后MXF文件格式（IMX30）：



```
概要
完整名称                                     : E:\test\test.mxf
文件格式                                     : MXF
Format_Commercial_IfAny                  : IMX 30
格式简介                                     : OP-1a
格式设置                                     : Closed / Complete
文件大小                                     : 152 MiB
长度                                       : 30秒 0ms
平均混合码率                                   : 42.6 Mbps
编码日期                                     : 2014-07-23 02:56:14.000
编码程序                                     : FFmbc OP1a Muxer 53.6.0
编码函数库                                    : FFmbc OP1a Muxer 53.6.0


视频
ID                                       : 2
文件格式                                     : MPEG Video
Format_Commercial_IfAny                  : IMX 30
格式版本                                     : Version 2
格式简介                                     : 4:2:2@Main
格式设置, BVOP                               : 否
格式设置, Matrix                             : 默认
格式设置, GOP                                : N=1
Format_Settings_Wrapping                 : Frame (D-10)
编码设置ID                                   : 0D01030102010501-0401020201020105
长度                                       : 30秒 0ms
码率                                       : 30.3 Mbps
最大码率                                     : 30.0 Mbps
画面宽度                                     : 720像素
画面高度                                     : 576像素
原始高度                                     : 608像素
画面比例                                     : 16:9
帧率                                       : 25.000 fps
标准                                       : Component
色彩空间                                     : YUV
色度抽样                                     : 4:2:2
位深度                                      : 8位
扫描方式                                     : 隔行扫描
扫描顺序                                     : 奇数场(前场TFF)优先
压缩模式                                     : 有损压缩
bits/(pixel*frame)(数据密度)                 : 2.923
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Source                          : Group of pictures header
大小                                       : 108 MiB (71%)
颜色初选                                     : BT.601 PAL
传输特质                                     : BT.709
矩阵系数                                     : BT.601


音频
ID                                       : 3
文件格式                                     : PCM
格式设置, Endianness                         : Little
Format_Settings_Wrapping                 : Frame (D-10)
编码设置ID                                   : 0D01030102010501
长度                                       : 30秒 0ms
码率模式                                     : CBR
码率                                       : 12.3 Mbps
声道                                       : 4声道
采样率                                      : 48.0 KHz
位深度                                      : 16位
大小                                       : 43.9 MiB (29%)


其它 #1
ID                                       : 1
Type                                     : Time code
文件格式                                     : MXF TC
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Settings                        : Striped


其它 #2
Type                                     : Time code
文件格式                                     : SMPTE TC
混合模式                                     : SDTI
TimeCode_FirstFrame                      : 00:00:00:00
```


转码后MOV文件格式（IMX30）：



```
概要
完整名称                                     : E:\test\test.mov
文件格式                                     : MPEG-4
Format_Commercial_IfAny                  : IMX 30
格式简介                                     : QuickTime
编码设置ID                                   : qt  
文件大小                                     : 113 MiB
长度                                       : 30秒 0ms
混合码率模式                                   : CBR
平均混合码率                                   : 31.5 Mbps
编码日期                                     : UTC 2014-07-23 03:03:09
标记日期                                     : UTC 2014-07-23 03:03:09
编码程序                                     : FFmbc 0.7


视频
ID                                       : 1
文件格式                                     : MPEG Video
Format_Commercial_IfAny                  : IMX 30
格式版本                                     : Version 2
格式简介                                     : 4:2:2@Main
格式设置, BVOP                               : 否
格式设置, Matrix                             : 默认
格式设置, GOP                                : N=1
混合模式                                     : MXF
编码设置ID                                   : mx3p
长度                                       : 30秒 0ms
码率模式                                     : CBR
码率                                       : 30.0 Mbps
画面宽度                                     : 720像素
画面高度                                     : 576像素
原始高度                                     : 608像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
标准                                       : Component
色彩空间                                     : YUV
色度抽样                                     : 4:2:2
位深度                                      : 8位
扫描方式                                     : 隔行扫描
扫描顺序                                     : 奇数场(前场TFF)优先
压缩模式                                     : 有损压缩
bits/(pixel*frame)(数据密度)                 : 2.894
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Source                          : Group of pictures header
大小                                       : 107 MiB (95%)
语言                                       : English
编码日期                                     : UTC 2014-07-23 03:03:09
标记日期                                     : UTC 2014-07-23 03:03:09
颜色初选                                     : BT.601 PAL
传输特质                                     : BT.709
矩阵系数                                     : BT.601


音频
ID                                       : 2
文件格式                                     : PCM
格式设置, Endianness                         : Little
格式设置, Sign                               : Signed
编码设置ID                                   : sowt
长度                                       : 29秒 858ms
码率模式                                     : CBR
码率                                       : 1 536 Kbps
声道                                       : 2声道
声道位置                                     : Front: L R
采样率                                      : 48.0 KHz
位深度                                      : 16位
大小                                       : 5.47 MiB (5%)
语言                                       : English
编码日期                                     : UTC 2014-07-23 03:03:09
标记日期                                     : UTC 2014-07-23 03:03:09
```


转码后MOV文件格式（IMX50）：



```
概要
完整名称                                     : E:\test\test.mov
文件格式                                     : MPEG-4
Format_Commercial_IfAny                  : IMX 50
格式简介                                     : QuickTime
编码设置ID                                   : qt  
文件大小                                     : 184 MiB
长度                                       : 30秒 0ms
混合码率模式                                   : CBR
平均混合码率                                   : 51.5 Mbps
编码日期                                     : UTC 2014-07-23 03:06:35
标记日期                                     : UTC 2014-07-23 03:06:35
编码程序                                     : FFmbc 0.7


视频
ID                                       : 1
文件格式                                     : MPEG Video
Format_Commercial_IfAny                  : IMX 50
格式版本                                     : Version 2
格式简介                                     : 4:2:2@Main
格式设置, BVOP                               : 否
格式设置, Matrix                             : 默认
格式设置, GOP                                : N=1
混合模式                                     : MXF
编码设置ID                                   : mx5p
长度                                       : 30秒 0ms
码率模式                                     : CBR
码率                                       : 50.0 Mbps
画面宽度                                     : 720像素
画面高度                                     : 576像素
原始高度                                     : 608像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
标准                                       : Component
色彩空间                                     : YUV
色度抽样                                     : 4:2:2
位深度                                      : 8位
扫描方式                                     : 隔行扫描
扫描顺序                                     : 奇数场(前场TFF)优先
压缩模式                                     : 有损压缩
bits/(pixel*frame)(数据密度)                 : 4.823
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Source                          : Group of pictures header
大小                                       : 179 MiB (97%)
语言                                       : English
编码日期                                     : UTC 2014-07-23 03:06:35
标记日期                                     : UTC 2014-07-23 03:06:35
颜色初选                                     : BT.601 PAL
传输特质                                     : BT.709
矩阵系数                                     : BT.601


音频
ID                                       : 2
文件格式                                     : PCM
格式设置, Endianness                         : Little
格式设置, Sign                               : Signed
编码设置ID                                   : sowt
长度                                       : 29秒 858ms
码率模式                                     : CBR
码率                                       : 1 536 Kbps
声道                                       : 2声道
声道位置                                     : Front: L R
采样率                                      : 48.0 KHz
位深度                                      : 16位
大小                                       : 5.47 MiB (3%)
语言                                       : English
编码日期                                     : UTC 2014-07-23 03:06:35
标记日期                                     : UTC 2014-07-23 03:06:35
```



### 2.编码ProRes


编码视频为ProRes。


码率控制方面，有以下3种选项：

(1)-qscale < value > 或 -cqp < value >

每帧视频设置固定的量化因子。这是可变码率的方法。

(2)-profile < name > ，有以下几个选项：
*proxy
*lt
*std
*hq
如果没有设置码率，则该选项可以自动设置码率。该码率的值将会和apple encoder类似。
(3)-b < bitrate >
设置恒定码率。
输出像素方面，支持444 编码：
命令行中添加 -pix_fmt yuv444p10

#### 2.1.命令行格式
`ffmbc.exe -i < file > -vcodec prores -profile hq -pix_fmt yuv444p10 –acodec copy < file.mxf >`
#### 2.2.示例

命令


`ffmbc.exe -i test.flv -vcodec prores -profile hq -pix_fmt yuv444p10 –acodec copy test _prores.mov`
输入文件格式

```
概要
完整名称                                     : E:\test\test.flv
文件格式                                     : Flash Video
文件大小                                     : 2.24 MiB
长度                                       : 30秒 0ms
平均混合码率                                   : 627 Kbps


视频
文件格式                                     : Sorenson Spark
编码设置ID                                   : 2
长度                                       : 30秒 0ms
码率                                       : 562 Kbps
画面宽度                                     : 320像素
画面高度                                     : 180像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
位深度                                      : 8位
bits/(pixel*frame)(数据密度)                 : 0.390
大小                                       : 2.01 MiB (90%)


音频
文件格式                                     : MPEG Audio
格式版本                                     : Version 1
格式简介                                     : Layer 3
模式                                       : Joint stereo
扩展模式                                     : MS Stereo
编码设置ID                                   : 2
编码设置ID/提示信息                              : MP3
长度                                       : 29秒 858ms
码率模式                                     : CBR
码率                                       : 32.0 Kbps
声道                                       : 2声道
采样率                                      : 44.1 KHz
压缩模式                                     : 有损压缩
大小                                       : 117 KiB (5%)
编码函数库                                    : LAME3.98
```


输出文件格式



```
概要
完整名称                                     : E:\test\test_prores.mov
文件格式                                     : MPEG-4
格式简介                                     : QuickTime
编码设置ID                                   : qt  
文件大小                                     : 46.6 MiB
长度                                       : 30秒 0ms
混合码率模式                                   : VBR
平均混合码率                                   : 13.0 Mbps
编码日期                                     : UTC 2014-07-23 04:25:20
标记日期                                     : UTC 2014-07-23 04:25:20
编码程序                                     : FFmbc 0.7


视频
ID                                       : 1
文件格式                                     : ProRes
格式版本                                     : Version 1
编码设置ID                                   : ap4h
长度                                       : 30秒 0ms
码率模式                                     : VBR
码率                                       : 13.0 Mbps
画面宽度                                     : 320像素
画面高度                                     : 180像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
色度抽样                                     : 4:4:4
扫描方式                                     : 逐行扫描
bits/(pixel*frame)(数据密度)                 : 9.003
大小                                       : 46.4 MiB (99%)
编码函数库                                    : ffm0
语言                                       : English
编码日期                                     : UTC 2014-07-23 04:25:20
标记日期                                     : UTC 2014-07-23 04:25:20


音频
ID                                       : 2
文件格式                                     : MPEG Audio
格式版本                                     : Version 1
格式简介                                     : Layer 3
模式                                       : Joint stereo
扩展模式                                     : MS Stereo
编码设置ID                                   : .mp3
长度                                       : 29秒 859ms
码率模式                                     : CBR
码率                                       : 32.0 Kbps
声道                                       : 2声道
声道位置                                     : Front: L R
采样率                                      : 44.1 KHz
压缩模式                                     : 有损压缩
大小                                       : 219 KiB (0%)
编码函数库                                    : LAME3.98
语言                                       : English
编码日期                                     : UTC 2014-07-23 04:25:20
标记日期                                     : UTC 2014-07-23 04:25:20
```



### 3.添加字幕


向视频文件中添加字幕。支持.srt和 .ass 文件（加字幕这个功能貌似ffmpeg本身也可以）。


注意：需要将libass库编译进FFmbc

选项：

file: set subtitles file
font: set font file
textcolor: set text color
boxcolor: set box background color (format is RRGGBBAA)
box: a box will surround the text
fontsize: set font size
text: set text to be burned
vmargin: set vertical margin


`ffmbc -i < file > -vf sub=file=captions.srt:box=1:boxcolor=0x00000080:font=arial.ttf < out file >`


### 4.编码XDCAMHD422


将视频编译为 XDCAMHD422 MXF或MOV


隔行编码：-target 前面添加-tff 或 -bff


`ffmbc -i < file > -target xdcamhd422 < file.mxf >`

注意：XDCAM要求至少2个单声道。使用-newaudio 和 -map_audio_channel.

举个例子，如下命令将一个立体声的视频转换为两个单声道的视频：
`ffmbc -i < file > -target xdcamhd422 -an < file.mxf > -acodec pcm_s24le -ar 48000 -newaudio -acodec pcm_s24le -ar 48000 -newaudio -map_audio_channel 0:1:0:0:1:0 -map_audio_channel 0:1:1:0:2:0`
#### 3.1.命令行格式
`ffmbc -i < file > -target xdcamhd422 -vtag < tag > < file.mov >`

tag有如下选项：
* "xd54" XDCAM HD422 720p24 CBR
* "xd59" XDCAM HD422 720p60 CBR
* "xd5a" XDCAM HD422 720p50 CBR
* "xd5b" XDCAM HD422 1080i60 CBR
* "xd5c" XDCAM HD422 1080i50 CBR
* "xd5d" XDCAM HD422 1080p24 CBR
* "xd5e" XDCAM HD422 1080p25 CBR
* "xd5f" XDCAM HD422 1080p30 CBR

#### 3.2.示例

命令
`ffmbc -i test.flv -s 1280x720 -ac 1 -target xdcamhd422 test_xdcamhd422.mxf`

注：
1.只支持单声道，如果输入文件为多声道，则会出现如下错误提示：

Error, target xdcamhd422 only supports mono audio tracks

可以通过添加-ac 1，将输入文件转换为单声道。
2.只支持输入为1920x1080或者1280x720格式，其他分辨率不支持。如果输入文件为其他分辨率，则会出现如下错误提示：

Error, target xdcamhd422 only supports 1920x1080 or 1280x720 resolutions

可以通过分辨率变换，将输入文件分辨率转换为1920x1080或者1280x720。


转码前文件格式：

```
概要
完整名称                                     : E:\test\test.flv
文件格式                                     : Flash Video
文件大小                                     : 2.24 MiB
长度                                       : 30秒 0ms
平均混合码率                                   : 627 Kbps


视频
文件格式                                     : Sorenson Spark
编码设置ID                                   : 2
长度                                       : 30秒 0ms
码率                                       : 562 Kbps
画面宽度                                     : 320像素
画面高度                                     : 180像素
画面比例                                     : 16:9
帧率模式                                     : 恒定
帧率                                       : 25.000 fps
位深度                                      : 8位
bits/(pixel*frame)(数据密度)                 : 0.390
大小                                       : 2.01 MiB (90%)


音频
文件格式                                     : MPEG Audio
格式版本                                     : Version 1
格式简介                                     : Layer 3
模式                                       : Joint stereo
扩展模式                                     : MS Stereo
编码设置ID                                   : 2
编码设置ID/提示信息                              : MP3
长度                                       : 29秒 858ms
码率模式                                     : CBR
码率                                       : 32.0 Kbps
声道                                       : 2声道
采样率                                      : 44.1 KHz
压缩模式                                     : 有损压缩
大小                                       : 117 KiB (5%)
编码函数库                                    : LAME3.98
```


转码后文件格式：

```
概要
完整名称                                     : E:\test\test_xdcamhd422.mxf
文件格式                                     : MXF
Format_Commercial_IfAny                  : XDCAM HD422
格式简介                                     : OP-1a
格式设置                                     : Closed / Complete
文件大小                                     : 182 MiB
长度                                       : 30秒 0ms
平均混合码率                                   : 51.0 Mbps
编码日期                                     : 2014-07-23 03:23:12.000
编码程序                                     : FFmbc OP1a Muxer 53.6.0
编码函数库                                    : FFmbc OP1a Muxer 53.6.0


视频
ID                                       : 2
文件格式                                     : MPEG Video
Format_Commercial_IfAny                  : XDCAM HD422
格式版本                                     : Version 2
格式简介                                     : 4:2:2@High
格式设置, BVOP                               : 是
格式设置, Matrix                             : 默认
格式设置, GOP                                : M=3, N=12
Format_Settings_Wrapping                 : Frame
编码设置ID                                   : 0D01030102046001-0401020201040300
长度                                       : 30秒 0ms
码率                                       : 50.0 Mbps
画面宽度                                     : 1 280像素
画面高度                                     : 720像素
画面比例                                     : 16:9
帧率                                       : 25.000 fps
标准                                       : Component
色彩空间                                     : YUV
色度抽样                                     : 4:2:2
位深度                                      : 8位
扫描方式                                     : 逐行扫描
压缩模式                                     : 有损压缩
bits/(pixel*frame)(数据密度)                 : 2.170
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Source                          : Group of pictures header
大小                                       : 179 MiB (98%)
颜色初选                                     : BT.709
传输特质                                     : BT.709
矩阵系数                                     : BT.709


音频
ID                                       : 3
文件格式                                     : PCM
格式设置, Endianness                         : Little
Format_Settings_Wrapping                 : Frame (AES)
编码设置ID                                   : 0D01030102060300
长度                                       : 30秒 0ms
码率模式                                     : CBR
码率                                       : 768 Kbps
声道                                       : 1声道
采样率                                      : 48.0 KHz
位深度                                      : 16位
大小                                       : 2.75 MiB (2%)


其它 #1
ID                                       : 1
Type                                     : Time code
文件格式                                     : MXF TC
TimeCode_FirstFrame                      : 00:00:00:00
TimeCode_Settings                        : Striped


其它 #2
Type                                     : Time code
文件格式                                     : SMPTE TC
混合模式                                     : SDTI
TimeCode_FirstFrame                      : 00:00:00:00
```








