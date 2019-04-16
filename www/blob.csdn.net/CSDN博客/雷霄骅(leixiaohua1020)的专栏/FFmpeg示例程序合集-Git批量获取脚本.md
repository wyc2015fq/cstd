# FFmpeg示例程序合集-Git批量获取脚本 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年02月23日 13:51:58[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：7300
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)









                此前做了一系列有关FFmpeg的示例程序，组成了《[FFmpeg示例程序合集](http://blog.csdn.net/leixiaohua1020/article/details/42658139)》，其中包含了如下项目：
simplest ffmpeg player:                  最简单的基于FFmpeg的视频播放器
simplest ffmpeg audio player:       最简单的基于FFmpeg的音频播放器
simplest ffmpeg pic encoder:        最简单的基于FFmpeg的图像编码器
simplest ffmpeg video encoder:    最简单的基于FFmpeg的视频编码器
simplest ffmpeg audio encoder:   最简单的基于FFmpeg的音频编码器
simplest ffmpeg streamer:             最简单的基于FFmpeg的推流器（推送RTMP）
simplest ffmpeg mem handler:     最简单的基于FFmpeg的内存读写例子
simplest ffmpeg device:                  最简单的基于FFmpeg的AVDevice例子
simplest ffmpeg format:                 最简单的基于FFmpeg的封装格式方面的例子
simplest ffmpeg video filter:          最简单的基于FFmpeg的AVfilter例子（水印叠加）
simplest ffmpeg swscale:             最简单的基于FFmpeg的libswscale的示例
为了方便今后代码的升级，将上述工程的代码统一上传到了Git代码托管平台（SourceForge，Github，开源中国），并且做了几个简单的脚本用于获取代码，如下所列：

> 克隆（simplest_ffmpeg_demos_git_clone_all.sh）：把代码从远端复制到本机（类似于下载）。
更新（simplest_ffmpeg_demos_git_pull_all.sh）：把代码从远端更新到本机（类似于升级）。
重置（simplest_ffmpeg_demos_git_reset_all.sh）：回退本机的代码到上一个版本（类似于还原）。
归档（simplest_ffmpeg_demos_git_archive_all.sh）：将本机的项目代码归档（例如打包为zip）。

这些脚本的下载地址：
【Github】 [https://github.com/leixiaohua1020/leixiaohua1020.github.io/tree/master/batch](https://github.com/leixiaohua1020/leixiaohua1020.github.io/tree/master/batch)



## 克隆

克隆脚本可以“一键下载”（clone命令）源代码至本机，并且设置本地仓库的3个远程仓库（remote）：SourceForge，Github和开源中国。
该脚本有一个originstr参数需要设置。该参数决定了从哪里克隆源代码，经过测试发现开源中国的速度明显高于SourceForge和Github。
simplest_ffmpeg_demos_git_clone_all.sh内容如下所示。

```
echo "============================================="
echo "simplest ffmpeg demos list ---- git clone all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="

originstr="github"
#3 kinds of origins:
githubstr="github"
sourceforgestr="sourceforge"
oscstr="osc"

#Git Clone
#Github
#http://leixiaohua1020.github.com/
if [ "$originstr" = "$githubstr" ]
then
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_player.git simplest_ffmpeg_player
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_audio_player.git simplest_ffmpeg_audio_player
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_picture_encoder.git simplest_ffmpeg_picture_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder.git simplest_ffmpeg_video_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_audio_encoder.git simplest_ffmpeg_audio_encoder
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_streamer.git simplest_ffmpeg_streamer
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_mem_handler.git simplest_ffmpeg_mem_handler
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_device.git simplest_ffmpeg_device
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_format.git simplest_ffmpeg_format
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_video_filter.git simplest_ffmpeg_video_filter
git clone https://github.com/leixiaohua1020/simplest_ffmpeg_swscale.git simplest_ffmpeg_swscale
#SourceForge
#https://sourceforge.net/u/leixiaohua1020/wiki/
elif [ "$originstr" = "$sourceforgestr" ]
then
git clone git://git.code.sf.net/p/simplestffmpegplayer/code simplest_ffmpeg_player
git clone git://git.code.sf.net/p/simplestffmpegaudioplayer/code simplest_ffmpeg_audio_player
git clone git://git.code.sf.net/p/simplestffmpegpictureencoder/code simplest_ffmpeg_picture_encoder
git clone git://git.code.sf.net/p/simplestffmpegvideoencoder/code simplest_ffmpeg_video_encoder
git clone git://git.code.sf.net/p/simplestffmpegaudioencoder/code simplest_ffmpeg_audio_encoder
git clone git://git.code.sf.net/p/simplestffmpegstreamer/code simplest_ffmpeg_streamer
git clone git://git.code.sf.net/p/simplestffmpegmemhandler/code simplest_ffmpeg_mem_handler
git clone git://git.code.sf.net/p/simplestffmpegdevice/code simplest_ffmpeg_device
git clone git://git.code.sf.net/p/simplestffmpegformat/code simplest_ffmpeg_format
git clone git://git.code.sf.net/p/simplestffmpegvideofilter/code simplest_ffmpeg_video_filter
git clone git://git.code.sf.net/p/simplestffmpegswscale/code simplest_ffmpeg_swscale
#OSC
#http://git.oschina.net/leixiaohua1020
else
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_player.git simplest_ffmpeg_player
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_player.git simplest_ffmpeg_audio_player
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_picture_encoder.git simplest_ffmpeg_picture_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_encoder.git simplest_ffmpeg_video_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_audio_encoder.git simplest_ffmpeg_audio_encoder
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_streamer.git simplest_ffmpeg_streamer
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mem_handler.git simplest_ffmpeg_mem_handler
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_device.git simplest_ffmpeg_device
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_format.git simplest_ffmpeg_format
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_video_filter.git simplest_ffmpeg_video_filter
git clone https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_swscale.git simplest_ffmpeg_swscale
fi
```




## 更新

更新脚本可以把本地库的源代码更新到最新版本。随着FFmpeg的API的变更以及一些程序中Bug的修复，项目的源代码需要与时俱进地做一些微调。可以使用pull命令将本地库的源代码“升级”到最新版本。
simplest_ffmpeg_demos_git_pull_all.sh内容如下所示。

```
echo "============================================="
echo "simplest ffmpeg demos list ---- git pull all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="

#remotestr="github"
remotestr="osc master"

#Git Pull
cd simplest_ffmpeg_player
git pull $remotestr
cd ..

cd simplest_ffmpeg_audio_player
git pull $remotestr
cd ..

cd simplest_ffmpeg_picture_encoder
git pull $remotestr
cd ..

cd simplest_ffmpeg_video_encoder
git pull $remotestr
cd ..

cd simplest_ffmpeg_audio_encoder
git pull $remotestr
cd ..

cd simplest_ffmpeg_streamer
git pull $remotestr
cd ..

cd simplest_ffmpeg_mem_handler
git pull $remotestr
cd ..

cd simplest_ffmpeg_device
git pull $remotestr
cd ..

cd simplest_ffmpeg_format
git pull $remotestr
cd ..

cd simplest_ffmpeg_video_filter
git pull $remotestr
cd ..

cd simplest_ffmpeg_swscale
git pull $remotestr
cd ..
```




## 重置

重置脚本用于将源代码回退到上一个版本。重置命令（reset）在两种情况会使用：
一种情况是改代码的时候改着改着忽然发现代码编译不过了，可是又难以恢复到原来可以编译通过的代码，这时候可以使用重置命令；
另一种情况是改完代码之后，使用更新命令（pull）却提示错误，这时候可以先使用重置命令，然后再使用更新命令。
simplest_ffmpeg_demos_git_reset_all.sh内容如下所示。

```
echo "============================================="
echo "simplest ffmpeg demos list ---- git reset all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="


#Git Pull
cd simplest_ffmpeg_player
git reset --hard
cd ..

cd simplest_ffmpeg_audio_player
git reset --hard
cd ..

cd simplest_ffmpeg_picture_encoder
git reset --hard
cd ..

cd simplest_ffmpeg_video_encoder
git reset --hard
cd ..

cd simplest_ffmpeg_audio_encoder
git reset --hard
cd ..

cd simplest_ffmpeg_streamer
git reset --hard
cd ..

cd simplest_ffmpeg_mem_handler
git reset --hard
cd ..

cd simplest_ffmpeg_device
git reset --hard
cd ..

cd simplest_ffmpeg_format
git reset --hard
cd ..

cd simplest_ffmpeg_video_filter
git reset --hard
cd ..

cd simplest_ffmpeg_swscale
git reset --hard
cd ..
```




## 归档

归档脚本用于将本地的代码库打包为压缩包（*.zip，*.tar.gz）。我个人感觉Git归档和直接压缩源代码文件夹最大的不同在于Git归档使用了“.gitignore”文件，从而忽略掉一些无关紧要的文件。
该脚本运行之后会生成一个名称为“simplest_ffmpeg_demos_list_{日期}”的文件夹，并且将归档后的文件保存在其中，归档后文件的命名方式为“simplest_ffmpeg_player_{日期}.zip”。
simplest_ffmpeg_demos_git_archive_all.sh内容如下所示。

```
echo "============================================="
echo "simplest ffmpeg demos list ---- git archive all"
echo "Lei Xiaohua"
echo "Communication University of China / Digital TV Technology"
echo "leixiaohua1020@126.com"
echo "http://blog.csdn.net/leixiaohua1020"
echo "============================================="



#save in a folder named with date
#file suffix
#fsuffix="latest"
fsuffix=$(date +%y%m%d)
#folder to save files
archivefolder="simplest_ffmpeg_demos_list_"${fsuffix}
echo "Archive Folder : "${archivefolder}

#if folder not exist, create it
if [ ! -d ${archivefolder} ]; then
  mkdir ${archivefolder}
fi

#Git Archive
echo Archive simplest_ffmpeg_player...
cd simplest_ffmpeg_player
git archive -o "../"${archivefolder}"/simplest_ffmpeg_player_"${fsuffix}".zip" HEAD
cd ..


echo Archive simplest_ffmpeg_audio_player...
cd simplest_ffmpeg_audio_player
git archive -o "../"${archivefolder}"/simplest_ffmpeg_audio_player_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_picture_encoder...
cd simplest_ffmpeg_picture_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_picture_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_video_encoder...
cd simplest_ffmpeg_video_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_video_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_audio_encoder...
cd simplest_ffmpeg_audio_encoder
git archive -o "../"${archivefolder}"/simplest_ffmpeg_audio_encoder_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_streamer...
cd simplest_ffmpeg_streamer
git archive -o "../"${archivefolder}"/simplest_ffmpeg_streamer_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_mem_handler...
cd simplest_ffmpeg_mem_handler
git archive -o "../"${archivefolder}"/simplest_ffmpeg_mem_handler_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_device...
cd simplest_ffmpeg_device
git archive -o "../"${archivefolder}"/simplest_ffmpeg_device_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_format...
cd simplest_ffmpeg_format
git archive -o "../"${archivefolder}"/simplest_ffmpeg_format_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_video_filter...
cd simplest_ffmpeg_video_filter
git archive -o "../"${archivefolder}"/simplest_ffmpeg_video_filter_"${fsuffix}".zip" HEAD
cd ..

echo Archive simplest_ffmpeg_swscale...
cd simplest_ffmpeg_swscale
git archive -o "../"${archivefolder}"/simplest_ffmpeg_swscale_"${fsuffix}".zip" HEAD
cd ..
```






