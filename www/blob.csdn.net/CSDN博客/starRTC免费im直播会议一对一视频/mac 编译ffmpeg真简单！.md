# mac 编译ffmpeg真简单！ - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月20日 14:28:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：185
brew info ffmpeg 有安装选项，并且会提示依赖库安装状态
安装依赖库
brew install automake fdk-aac git lame libass libtool libvorbis libvpx \opus sdl shtool texi2html theora wget x264 x265 xvid
 yasm
brew install ffmpeg --with-fdk-aac --with-tools --with-freetype --with-libass --with-libvorbis --with-libvpx --with-x265
以上命令，会自动安装上x264的。
