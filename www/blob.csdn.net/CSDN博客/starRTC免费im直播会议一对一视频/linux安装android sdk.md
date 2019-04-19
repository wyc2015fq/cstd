# linux安装android sdk - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月14日 13:49:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：155
[https://dl.google.com/android/repository/sdk-tools-linux-4333796.zip](https://dl.google.com/android/repository/sdk-tools-linux-4333796.zip)
1，先安装java
[https://developer.android.com/studio/](https://developer.android.com/studio/)
下载linux sdk
找到tools 执行 ./android update sdk
或./android update sdk --no-ui
上面的命令已经不建议使用了，用bin/sdkmanager --update
发现没有build-tools
进入bin下，发现sdkmanager
执行./sdkmanager "platforms;android-26"
相关sdkmanager命令可以查看[https://developer.android.com/studio/command-line/sdkmanager](https://developer.android.com/studio/command-line/sdkmanager)
List installed and available packages
sdkmanager --list
Install packages
sdkmanager "platform-tools" "platforms;android-17" 具体值可以在--list中得到
"build-tools;28.0.3"
Update all installed packages
sdkmanager --update
