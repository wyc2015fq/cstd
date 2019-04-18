# Android中sdcard和adb命令的使用 - Orisun - 博客园







# [Android中sdcard和adb命令的使用](https://www.cnblogs.com/zhangchaoyang/articles/1956952.html)





**先说sdcard**

1.创建sdcard

mksdcard   10M   /home/orisun/sdcard.img

2.带sdcard启动模拟器

emulator  -sdcard  

/home/orisun/sdcard.img  -avd  avd2.2


avd2.2是之前创建好的一个虚拟设备的名称

3.把文件放到sdcard中

另外打开一个终端

adb  push  Picture/1.jpg  /sdcard/1.jpg

注意不论你现在什么目录，模拟器sdcard的目录就是/sdcard，并且不能加.img

4.检查

adb shell

#cd sdcard

#ls

**详细说说adb的功能**

adb(Android Debug Bridge)是Android 提供的一个通用的调试工具

通过adb安装卸载apk包和向sdcard中放入文件前面已经讲过了，就不重复了

1、进入设备或模拟器的shell：

adb shell
通过上面的命令，就可以进入设备或模拟器的shell环境中，在这个Linux Shell中，你可以执行各种Linux 的命令，另外如果只想执行一条shell命令，可以采用以下的方式：
adb shell [command]
如：adb shell dmesg会打印出内核的调试信息。
2、发布端口：
可以设置任意的端口号，做为主机 向模拟器或设备的请求端口。如：
adb forward tcp:5555 tcp:8000
3、复制文件 ：
可向一个设备或从一个设备中复制文件，
复制一个文件或目录到设备或模拟器上：
adb push
如：adb push test.txt /tmp/test.txt
从设备或模拟器上复制一个文件或目录：
adb pull
如：adb pull /addroid/lib/libwebcore.so .
4、搜索模拟器/设备的实例：
取得当前运行的模拟器/设备的实例的列表及每个实例的状态：
adb devices
5、查看bug报告：
adb bugreport
6、记录无线通讯日志：
一般来说，无线通讯的日志非常多，在运行时没必要去记录，但我们还是可以通过命令，设置记录：
adb shell
logcat -b radio
7、获取设备的ID和序列号：
adb get-product
adb get-serialno
8、访问数据库SQLite3
adb shell
sqlite3

9、通过gsm call命令可以像Android 模拟器打电话 ，除了在EclipseADT 的DDMS中通过按钮Dial外，还可以通过DDMS外壳调用gsm call命令直接拨打，我们首先需要启动AndroidEmulator，然后在cmd环境下执行telnet localhost 5554 下面就可以向Android模拟器 拨号，参数为gsmcall < phoneNum> ，比如给10086打电话 为gsm call +10086












