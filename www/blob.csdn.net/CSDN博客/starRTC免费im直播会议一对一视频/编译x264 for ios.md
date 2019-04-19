# 编译x264 for ios - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月15日 09:56:16[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：678
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
所属专栏：[](https://blog.csdn.net/column/details/.html)[编解码](https://blog.csdn.net/column/details/12721.html)
Tested with:
x264-snapshot-20140914-2245  我用的是x264-snapshot-20150813-2245.tar.bz2
Xcode 7
依赖gas-preprocessor,参见ffmpeg for ios一文。
用root执行
To build everything:
sudo ./build-x264.sh
To build for arm64:
./build-x264.sh arm64
To build fat library for armv7 and x86_64 (64-bit simulator):
./build-x264.sh armv7 x86_64
To build fat library from separately built thin libraries:
./build-x264.sh lipo
问题：
解压之后是一个含snapshot-xxxx的一个文件夹，把它拷贝到随便一个文件夹下，重命名为x264;
把脚本放到跟x264文件夹同级的地方  chmod a+x *.sh
执行时加上sudo
找不到c编译器
需要去掉iphoneos下面的
CFLAGS="-fembed-bitcode -mios-version-min=7.0"
http://blog.sina.com.cn/s/blog_47522f7f0102vcbj.html
其中x264为源码目录，thin_x264为单个平台的。x264-iOS为fat库
如何与FFmpeg集成（编译带x264的ffmpeg包）
拷贝编译好的x264库到build-ffmpeg.sh脚本所在目录下，并重命名x264-iOS为fat-x264，清除build-ffmpeg.sh脚本中X264=`pwd`/fat-x264一行的注释，保存后执行脚本，顺利的话就会跟之前一样生成各个包啦。
build-x264.sh
参考;
https://github.com/kewlbear/x264-ios
            
