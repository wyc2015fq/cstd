# 编译webrtc for android库与apk - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月27日 18:35:03[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：381
git clone
[https://chromium.googlesource.com/chromium/tools/depot_tools.git](https://chromium.googlesource.com/chromium/tools/depot_tools.git)
export PATH=`pwd`/depot_tools:"$PATH"
git clone
[https://github.com/pristineio/webrtc-build-scripts.git](https://github.com/pristineio/webrtc-build-scripts.git)
This is only required once：
# Source all the routines
使build.sh里面的一些命令在当前环境中生效source
 android/build.sh# Install any dependencies needed
获取项目部署工具和源码，以最新的源码作当前版本install_dependencies#
 Pull WebRTCget_webrtc
# 直接构建全部的版本出来（不同架构、Debug、Release），时间会很久,不推荐！build_apprtc
或者只编译特定版本的库：
export WEBRTC_ARCH=armv7
#or armv8, x86, or x86_64prepare_gyp_defines
&&
execute_build
注意：android中build_apprtc 命令其实是把全部平台都编译了
最终在：android/webrtc/libjingle_peerconnection_builds
包含了不同的版本的so和jar文件。复制到项目中即可使用了。
you can find the .jar and .so file in $WEBRTC_ROOT under "libjingle_peerconnection_builds".
在WebRTC的范例工程中，有一个Android项目叫AppRTCDemo，可以实现广域网上的视频通话（VoIP）
问题：
libcups2-dev : Depends: libcups2 (= 2.1.3-4) but 2.1.3-4ubuntu0.2 is to be installed
apt-get install libcups2=2.1.3-4
参考：
[https://github.com/pristineio/webrtc-build-scripts](https://github.com/pristineio/webrtc-build-scripts)
