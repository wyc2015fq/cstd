
# adb 启动某个apk - 嵌入式Linux - CSDN博客

2019年02月20日 09:52:26[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：105


有时候需要用apk来启动某个apk
### adb shell am start -n
### com.android.launcher3
### /
### com.android.launcher3.Launcher
具体查看~
/rk3399_7in1/packages/apps/Launcher3/AndroidManifest.xml
![](https://img-blog.csdnimg.cn/20190220095204576.png)
![](https://img-blog.csdnimg.cn/20190220095214686.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)

