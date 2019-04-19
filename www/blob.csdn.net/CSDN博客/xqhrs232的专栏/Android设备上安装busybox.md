# Android设备上安装busybox - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:15:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：907
原文地址::[http://blog.csdn.net/andy_android/article/details/7288073](http://blog.csdn.net/andy_android/article/details/7288073)
BusyBox 是一个集成了一百多个最常用linux命令和工具的软件。BusyBox 包含了一些简单的工具，例如ls、 cat 和 echo等等，还包含了一些更大、更复杂的工具，例如 grep、find、mount 以及 telnet。有些人将 BusyBox 称为 Linux 工具里的瑞士军刀。简单的说BusyBox就好像是个大工具箱，它集成压缩了 Linux 的许多工具和命令。也包含了 Android 系统的自带的shell，具体的内容详见[http://en.wikipedia.org/wiki/BusyBox](http://en.wikipedia.org/wiki/BusyBox)
对于习惯使用linux命令的开发者来说，Android自身提供的对Linux命令的支持明显就显得捉襟见肘了，这时候就需要busybox帮助你。[](http://en.wikipedia.org/wiki/BusyBox)
那么怎么安装呢？
1. 下载适用于Android上的busybox可执行文件：
URL: [http://benno.id.au/android/busybox](http://benno.id.au/android/busybox)
2. 修改文件可执行属性；
```java
$ chmod A+X busybox
```
3. 在Android设备或者模拟器data分区上创建busybox的文件夹；
```java
$ adb remount $ adb shell mkdir /data/busybox
```
4. 将busybox push到Android设备或模拟器上；
```java
$ adb push busybox /data/busybox
```
5. 创建支持命令的硬链接；
```java
$ adb shell #cd data/busybox #./busybox --install
```
6. 将busybox的路径加入PATH环境变量；
```java
#export PATH=/data/busybox:$PATH
```
Ok,busybox已经装在了你的设备或模拟器上了，试一试在shell中输入你所知道的linux命令（如uname -a） 是不是很爽呢
