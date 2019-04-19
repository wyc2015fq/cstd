# WebRTC for android ios开发官方指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月06日 15:39:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：475
The WebRTC **native code package** can be found at:
- [https://chromium.googlesource.com/external/webrtc](https://chromium.googlesource.com/external/webrtc)
[https://webrtc.org/faq/](https://webrtc.org/faq/)
**Prerequisite **
**Install depot_tools**
manage checkouts and code reviews.  
The depot_tools package includes gclient, gcl, git-cl, repo,
 and others.
- Confirm git and python are
 installed. git 2.2.1+ recommended. python 2.7+ recommended.
- Fetch depot_tools: 
$ git
 clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
- Add depot_tools to your [PATH](http://www.lmgtfy.com/?q=PATH+environment):
$ export
 PATH=`pwd`/depot_tools:"$PATH"
- Yes, you want to put depot_tools ahead of everything else,
 otherwise gcl will refer to the GNU Common Lisp compiler.
**Example Applications**
src/webrtc/examples and src/talk/examples:
1,Peerconnection :consist of two applications using the [WebRTC
 Native APIs](https://webrtc.org/native-code/native-apis/):peerconnection_server and peerconnection_client  (not currently
 supported on Mac/Android)
***2,Call App***
Target name call (currently
 disabled).
establishes a call using libjingle.
Call uses xmpp (as opposed to SDP used by WebRTC) to allow you to login using your gmail account and make audio/video calls with your gmail friends.
3,relayserver
Can be used with the call application above.
4,stunserver
5,turnserver
安装好java
Android
fetch --nohooks webrtc_android 约16 GB，gclient sync
编译：进入src目录
gn gen out/Debug --args='target_os="android" target_cpu="arm"'
ninja -C out/Debug

AppRTCMobile
is an Android application using WebRTC Native APIs via JNI (JNI wrapper is documented [here](https://chromium.googlesource.com/external/webrtc/+/master/webrtc/sdk/android/README)).
For instructions on how to build and run, see [webrtc/examples/androidapp/README](https://chromium.googlesource.com/external/webrtc/+/master/webrtc/examples/androidapp/README).
可以生成android studio工程：详见官网
**iOS**
A macOS machine is required for iOS development.
fetch --nohooks webrtc_iosgclient sync
gn gen out/ios_64 --args='target_os="ios" target_cpu="arm64"'
ninja -C out/ios_64
target 
To see a list of available targets, run gn ls out/<output
 folder>.
可以生成xcode工程
It’s easiest to deploy to a device using Xcode. Other command line tools exist as well, e.g. [ios-deploy](https://github.com/phonegap/ios-deploy).
**Using WebRTC in your app**
ninja -C out/ios rtc_sdk_framework_objc
本人已经在美国服务器上将android和ios的代码全部下载并同步了。然后打包放在了百度云上，大家只需要继续gn gen和ninja就可以了，android和ios我都只运行到gclient sync！
android打包后有12G,因为里面含有android sdk和ndk.ios大约有6G.
首次同步时间是2017.07.05,大家可以根据指南更新：[https://webrtc.org/native-code/development/](https://webrtc.org/native-code/development/)
 的updating the code章节！
对应的docker镜像如下：
参考：
[https://webrtc.org/native-code](https://webrtc.org/native-code)
[https://webrtc.org/faq/](https://webrtc.org/faq/)
