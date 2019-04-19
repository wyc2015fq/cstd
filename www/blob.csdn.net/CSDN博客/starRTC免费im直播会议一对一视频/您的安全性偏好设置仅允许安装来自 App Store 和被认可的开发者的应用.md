# 您的安全性偏好设置仅允许安装来自 App Store 和被认可的开发者的应用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月04日 18:54:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：17390
您的安全性偏好设置仅允许安装来自 App Store 和被认可的开发者的应用。
安装macOS Sierra后，会发现系统偏好设置的“安全与隐私”中默认已经去除了允许“任何来源”App的选项，无法运行一些第三方应用。
如果需要恢复允许“任何来源”的选项，即关闭Gatekeeper，请在终端中使用spctl命令：
- sudo spctl --master-disable
