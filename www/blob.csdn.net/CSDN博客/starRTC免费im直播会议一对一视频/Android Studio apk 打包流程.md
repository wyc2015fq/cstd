# Android Studio apk 打包流程 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年09月15日 13:55:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：220
Build -> Generate Signed APK
点击Create new，窗口如下
这里只要输入几个必要项
Key store path（生产key文件的保存路径 ）
Key store password（key 存储密码）
Key alias（key别名）
Key password（key密码）
可以输入机构或个人名字
Signature Versions 不能只选择 V2(Full APK
 Signature)，应该选择 V1(Jar Signature)，或者选择 V1 和 V2。
