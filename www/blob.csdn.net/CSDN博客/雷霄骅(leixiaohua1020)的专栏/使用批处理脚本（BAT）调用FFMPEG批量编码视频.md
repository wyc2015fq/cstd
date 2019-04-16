# 使用批处理脚本（BAT）调用FFMPEG批量编码视频 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 22:02:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：40









使用批处理脚本（BAT）编码视频非常方便，尤其当视频序列非常多的时候，更是省了不少简单重复性劳动。

只要学会批处理里面几个基本的命令就行了，感觉和c/c++差不多。

set：设置变量（注意：变量一般情况下是字符串，而不是整形或者其他类型）

%变量名%：可以取到变量的值

传上来一个做实验的批处理脚本做示范（其中调用了ffmpeg和x264）：



```
@echo off
@rem 使用此脚本生成实验序列
@rem 设置序列名称
set xuliename=src08
@rem 帧率分辨率比特率参数
set resolution1=1920x1080
set resolution2=1280x720
set resolution3=848x480
set resolution4=640x360
set resolution5=480x272
set framerate1=25
set framerate2=20
set framerate3=15
set framerate4=12.5
set framerate5=10
set framerate6=5
set bitrate1=254
set bitrate2=508
set bitrate3=763
set bitrate4=1017
set bitrate5=1526
set bitrate6=2035
set bitrate7=2544
@rem 生成特定分辨率，然后再进行上变换
ffmpeg -s %resolution1% -i %xuliename%_%resolution1%.yuv -s %resolution2% %xuliename%_%resolution2%.yuv
ffmpeg -s %resolution1% -i %xuliename%_%resolution1%.yuv -s %resolution3% %xuliename%_%resolution3%.yuv
ffmpeg -s %resolution1% -i %xuliename%_%resolution1%.yuv -s %resolution4% %xuliename%_%resolution4%.yuv
ffmpeg -s %resolution1% -i %xuliename%_%resolution1%.yuv -s %resolution5% %xuliename%_%resolution5%.yuv
@rem =======================
ffmpeg -s %resolution2% -i %xuliename%_%resolution2%.yuv -s %resolution1% -vcodec rawvideo %xuliename%_reHD_%resolution2%.avi
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -s %resolution1% -vcodec rawvideo %xuliename%_reHD_%resolution3%.avi
ffmpeg -s %resolution4% -i %xuliename%_%resolution4%.yuv -s %resolution1% -vcodec rawvideo %xuliename%_reHD_%resolution4%.avi
ffmpeg -s %resolution5% -i %xuliename%_%resolution5%.yuv -s %resolution1% -vcodec rawvideo %xuliename%_reHD_%resolution5%.avi
@rem 原始分辨率 =======================
ffmpeg -s %resolution1% -i %xuliename%_%resolution1%.yuv -s %resolution1% -vcodec rawvideo %xuliename%_reHD_ori.avi
@rem 生成特定帧率
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate2% -vcodec rawvideo %xuliename%_fps_%framerate2%.avi
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate3% -vcodec rawvideo %xuliename%_fps_%framerate3%.avi
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate4% -vcodec rawvideo %xuliename%_fps_%framerate4%.avi
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate5% -vcodec rawvideo %xuliename%_fps_%framerate5%.avi
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate6% -vcodec rawvideo %xuliename%_fps_%framerate6%.avi
@rem 原始帧率 =======================
ffmpeg -s %resolution3% -i %xuliename%_%resolution3%.yuv -r %framerate2% -vcodec rawvideo %xuliename%_fps_ori.avi
@rem 生成特定比特率
x264 --bitrate %bitrate1% -o %xuliename%_bit_%bitrate1%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate2% -o %xuliename%_bit_%bitrate2%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate3% -o %xuliename%_bit_%bitrate3%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate4% -o %xuliename%_bit_%bitrate4%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate5% -o %xuliename%_bit_%bitrate5%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate6% -o %xuliename%_bit_%bitrate6%.flv %xuliename%_%resolution3%.yuv
x264 --bitrate %bitrate7% -o %xuliename%_bit_%bitrate7%.flv %xuliename%_%resolution3%.yuv
@rem 
ffmpeg -i %xuliename%_bit_%bitrate1%.flv -vcodec rawvideo %xuliename%_bit_%bitrate1%.avi
ffmpeg -i %xuliename%_bit_%bitrate2%.flv -vcodec rawvideo %xuliename%_bit_%bitrate2%.avi
ffmpeg -i %xuliename%_bit_%bitrate3%.flv -vcodec rawvideo %xuliename%_bit_%bitrate3%.avi
ffmpeg -i %xuliename%_bit_%bitrate4%.flv -vcodec rawvideo %xuliename%_bit_%bitrate4%.avi
ffmpeg -i %xuliename%_bit_%bitrate5%.flv -vcodec rawvideo %xuliename%_bit_%bitrate5%.avi
ffmpeg -i %xuliename%_bit_%bitrate6%.flv -vcodec rawvideo %xuliename%_bit_%bitrate6%.avi
ffmpeg -i %xuliename%_bit_%bitrate7%.flv -vcodec rawvideo %xuliename%_bit_%bitrate7%.avi
```










