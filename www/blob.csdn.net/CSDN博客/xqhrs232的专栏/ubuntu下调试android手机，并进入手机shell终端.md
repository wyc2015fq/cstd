# ubuntu下调试android手机，并进入手机shell终端 - xqhrs232的专栏 - CSDN博客
2012年05月04日 16:06:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：785标签：[手机																[android																[shell																[ubuntu																[服务器																[sqlite](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/hangxin1940/archive/2011/07/10/2101552.html](http://www.cnblogs.com/hangxin1940/archive/2011/07/10/2101552.html)
既然玩android开发，就必须也得深入到系统，模拟器慢得要死，就上真机了。
windows下一切都是那么小白，换到linux下就得自己动手。
查了相关资料 ，终于知道怎么弄了。
我的机型是 moto mb525 (Defy)
首先准备好一切环境
adb，官网有，开发肯定离不开这个android sdk
手机设置为调试模式
能看到这篇文章，你的手机肯定是已经root过了。 
1、首先查看手机的usb信息
```
[]()
hang@CAPF:~$ lsusb 
Bus 002 Device 004: ID 18c3:6255  
Bus 002 Device 002: ID 8087:0020 Intel Corp. Integrated Rate Matching Hub
Bus 002 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 001 Device 005: ID 22b8:41db Motorola PCS Motorola Droid (USB Debug)
Bus 001 Device 004: ID 04d9:a06b Holtek Semiconductor, Inc. 
Bus 001 Device 003: ID 058f:b002 Alcor Micro Corp. 
Bus 001 Device 002: ID 8087:0020 Intel Corp. Integrated Rate Matching Hub
[]()
```
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
lsusb命令是列出当前所有得usb设备，我的defy就是
Bus 001 Device 005: ID 22b8:41db Motorola PCS Motorola Droid (USB Debug)
 设备id是 22b8 ,设备信息描述是  
Motorola PCS Motorola Droid (USB Debug)hang@CAPF:/etc/udev$ cd /etc/udev/rules.d
2、添加udev规则
udev就是一个动态硬件管理服务 
hang@CAPF:/etc/udev/rules.d$ cd /etc/udev/rules.d/
```
hang@CAPF:/etc/udev/rules.d$
 sudo vi 50-android-usb.rules
```
编辑规则文件并保存 
SUBSYSTEM=="usb", SYSFS("Motorola PCS Motorola Droid (USB Debug)")=="22b8",MODE="0666"
 其中，sysfs括号内是自己android手机的实际描述信息，==后面的是id号，mode是读取模式，0666是所有人可以访问，以上的信息都是lsusb查处来的。
3、设置规则文件权限并重启udev
```
hang@CAPF:/etc/udev/rules.d$ sudo chmod a+rx /etc/udev/rules.d/50-android-usb.rules
```
hang@CAPF:/etc/udev/rules.d$ sudo /etc/init.d/udev restart
 会看到udev相关的提示信息
4、设置adb 
 进入sdk得platform-tools目录
```
hang@CAPF:/opt/android-sdk-linux_x86/platform-tools$ sudo ./adb kill-server
hang@CAPF:/opt/android-sdk-linux_x86/platform-tools$ sudo ./adb devices
* daemon not running. starting it now on port 5037 *
* daemon started successfully *
List of devices attached
```
0163C5420200F029    device
大功告成，可以在eclipse下使用真机调试了。
 进入android shell的方法：
hang@CAPF:/opt/android-sdk-linux_x86/platform-tools$ ./adb shell
进去以后，就可以随便折腾了，，最好别乱删乱改
 最后附上adb的相关参数，英文--help一样：
adb(Android Debug Bridge)是Android 提供的一个通用的调试工具，借助这个工具，我们可以很好的调试开发的程序,adb.exe在你安装的android的sdk开发包tools目录下
|命令|描述|
|----|----|
|install <path>|安装应用程序到服务器|
|pull <remote file> <local file>|将远程文件拉出服务器|
|push <local file> <remote file>|将本地文件推进服务器|
|shell|在服务器上打开一个壳环境|
|forward <local port> <remote port>|从一个端口转递流量到另外一个端口（到或者从服务器上）|
|start-server|启动服务器|
|kill-server|停止服务器|
|ppp <tty> <params>|通过USB使用一个ppp连接|
|devices|列出可用的模拟器|
|help|列出adb的命令|
|version|显示adb的版本|
adb使用方法：
adb [-d|-e|-s <serialNumber>] <command>
当你发出一个命令，系统启用Android客户端。客户端并不与模拟器实例相关，所以如果双服务器/设备是运行中的，你需要用-d选项 去为应被控制的命令确定目标实例。关于使用这个选项的更多信息，可以查看模拟器/设备实例术语控制命令。**1、安装应用 到模拟器：**
你可以使用adb从你的开发电脑上复制一个应用程序，并且将其安装在一个模拟器/设备实例。像这样做，使用install命令。这个 install命令要求你必须指定你所要安装的.apk文件的路径:
adb install <path_to_apk>
为 了获取更多的关于怎样创建一个可以安装在模拟器/设备实例上的.apk文件的信息，可参照Android Asset Packaging Tool (aapt).
要注意的是，如果你正在使用Eclipse IDE并且已经安装过ADT插件，那么就不需要直接使用adb（或者aapt）去安装模拟器/设备上的应用程序。否则，ADT插件代你全权处理应用程序的 打包和安装.
如果要删除某个应用，按下面操作，例如应用为test：
adb shell
cd /data/app
rm test.apk
**2、进入设备或模拟器的shell：**
adb shell
通过上面的命令，就可以进入设备或模拟器的shell环 境中，在这个Linux Shell中，你可以执行各种Linux 的命令，另外如果只想执行一条shell命令，可以采用以下的方式：
adb shell [command]
如：adb shell dmesg会打印出内核的调试信息。
**3、发布端口：**
可以设置任意的端口 号，做为主机 向模拟器或设备的请求端口。如：
adb forward tcp:5555 tcp:8000
**4、从模拟器/设备中拷入或 拷出文件：**
可以使用adbpull ,push命令将文件复制到一个模拟器/设备实例的数据文件或是从数据文件中复制。install命令只将一个.apk文件复制到一个特定的位置，与其不 同的是，pull和push命令可令你复制任意的目录和文件到一个模拟器/设备实例的任何位置。
从模拟器或者设备中复制文件或目录，使用(如下 命):
adb pull <remote> <local>
将文件或目录复制到模拟器或者设备，使用（如下命 令）
adb push <local> <remote>
在这些命令中，<local> 和<remote>分别指通向自己的发展机（本地）和模拟器/设备实例（远程）上的目标文件/目录的路径
下面是一个例子：
adb push test.txt /data/dat/test/test.txt、
**5、查询模拟器/设备实例：**
在发布adb命令之前，有必要知道什么样的模拟器/设备实例与adb服务器是相连的。可以通过使用 devices
命令来得到一系列相关联的模拟器/设备：
adb devices
作为回应，adb为每个实例都制定了相应的状态信息:
序列号——由adb创建的一个字符串，这个字符串通过自己的控制端 口<type>-<consolePort>唯一地识别一个模拟器/设备实例。
下面是一个序列号的例子：
emulator-5554
实 例的连接状态有三种状态: 
offline — 此实例没有与adb相连接或者无法响应.
device — 此实例正与adb服务器连接。注意这个状态并不能百分之百地表示在运行和操作Android系统，因此这个实例是当系统正在运行的时候与adb连接的。然 而，在系统启动之后，就是一个模拟器/设备状态的正常运行状态了. 
每个实例的输出都有如下固定的格式:
[serialNumber] [state]
下面是一个展示devices命令和输出的例子 :
$ adb devices
List of devices attached
emulator-5554  device
emulator-5556  device
emulator-5558  device
如 果当前没有模拟器/设备运行，adb则返回 no device .
**6、查看bug报告：**
adb bugreport
**7、记录无线通讯日志：**
一般来说，无线通讯的日志非常多，在运行时没必要去记录，但我们还是可以通过命令，设置记录：
adb shell
logcat -b radio
**8、获取设备的ID和序列号：**
adb get-product
adb get-serialno
**9、 访问数据库SQLite3**
adb shell
sqlite3
如要打开已存在数据库：
sqlite3 <路径>/dbname.db
