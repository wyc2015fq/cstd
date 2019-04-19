# 在Windows下编译WebRTC - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月06日 14:05:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：63

web端用webRTC实现的一对一视频，互动直播和会议。https://github.com/starrtc/android-demo
安装depot_tools
chromium的webrtc下载的时候，没有下载它的一些资源文件
src\third_party\webrtc\resources
fetch --nohooks webrtc //还可以加--no-history gclient sync
gn gen out/Default
执行下面的命令生成VS工程文件
$ gn gen --ide=vs out\vs
找到 all.sln 这个工程文件
上面2个gn gen命令有什么不同？
就多了一个
如果不想生成vs就用第一条命令。
如果想生成vs的，只需要运行第2条命令就可以了，它包括了第一条命令的结果
在第一条命令的基础上，多生成了些IDE工程文件
2个gn gen运行一个就可以了。
然后编译：
ninja -C out/vs
参考：
[https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md](https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md)
[https://www.jianshu.com/p/475886261388](https://www.jianshu.com/p/475886261388)
