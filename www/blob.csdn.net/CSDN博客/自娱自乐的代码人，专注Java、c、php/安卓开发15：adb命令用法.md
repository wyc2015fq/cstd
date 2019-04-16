# 安卓开发15：adb命令用法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月08日 18:54:09[initphp](https://me.csdn.net/initphp)阅读数：2057
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









ADB是一个 客户端-服务器端 程序, 其中客户端是你用来操作的电脑, 服务器端是android设备.



```
//连接设备
adb connect xxxx.xxx.xxx.xx:xxxx
//进入设备
adb shell
//进入安卓app目录下
cd system/app
//将一个apk 推送到app目录下
adb push <本地路径> <远程路径>
//删除apk，需要进入安卓app目录
rm xxx.apk
//查看设备
adb devices
//安装apk
adb install xxx.apk
//卸载apk
adb uninstall <软件名>
adb uninstall -k <软件名>
//下载文件
adb pull <远程路径> <本地路径>
//显示帮助信息
adb help
//手机数据线连接：
adb remount
```


一个表格：




|Category|Command|Description|Comments|
|----|----|----|----|
|Options|-d|仅仅通过USB接口来管理abd|如果不只是用USB接口来管理则返回错误|
|-e|仅仅通过模拟器实例来管理adb|如果不是仅仅通过模拟器实例管理则返回错误| |
|-s <serialNumber>|通过模拟器/设备的允许命令号码来发送命令来管理adb (如 “emulator-5556″)|如果没有指定号码，则会报错| |
|General|devices|查看所有连接模拟器/设备的设施的清单|查看 Querying for Emulator/Device Instances 获取更多相关信息|
|help|查看adb所支持的所有命令|| |
|version|查看adb的版本序列号|| |
|Debug|logcat [<option>] [<filter-specs>]|将日志数据输出到屏幕上||
|bugreport|查看bug的报告，如dumpsys ,dumpstate ,和logcat 信息|| |
|jdwp|查看指定设施的可用的JDWP信息|可以用 forward jdwp:<pid>端口映射信息来连接指定的JDWP进程，例如：adb forward tcp:8000 jdwp:472jdb -attach localhost:8000| |
|Data|install <path-to-apk>|安装Android为（可以模拟器/设施的数据文件.apk指定完整的路径）||
|pull <remote> <local>|将指定的文件从模拟器/设施拷贝到计算机上|| |
|push <local> <remote>|将指定的文件从计算机上拷贝到模拟器/设备中|| |
|Ports and Networking|forward <local> <remote>|用本地指定的端口通过Socket方法远程连接模拟器/设施|端口需要描述下列信息:- tcp:<portnum>- local:<UNIX domain socket name>- dev:<character device name>- jdwp:<pid>|
|ppp <tty> [parm]…|通过USB运行ppp：- <tty> — the tty for PPP stream. For exampledev:/dev/omap_csmi_ttyl.- [parm]… &mdash zero or more PPP/PPPD options, such as defaultroute ,local , notty , etc.需要提醒的不能自动启动PDP连接|| |
|Scripting|get-serialno|查看adb实例的序列号|查看 Querying for Emulator/Device Instances 可以获得更多信息|
|get-state|查看模拟器/设施的当前状态| | |
|wait-for-device|如果设备不联机就不让执行,也就是实例状态是 device 时|可以提前把命令转载在adb的命令器中,在命令器中的命令在模拟器/设备连接之前是不会执行其他命令的，示例如下:adb wait-for-device shell getprop需要提醒的是，这些命令在所有的系统启动起来之前是不会启动adb的，所以在所有的系统启动起来之前也不能执行其他的命令，例如，运用install 的时候就需要Android包，这些包需要系统完全启动，例如：adb wait-for-device install <app>.apk上面的命令只有连接上了模拟器/设备连接上了adb服务才会被执行，而在Android系统完全启动前执行就会有错误发生| |
|Server|start-server|选择服务是否启动adb服务进程||
|kill-server|终止adb服务进程|| |
|Shell|shell|通过远程Shell命令来控制模拟器/设备实例|查看获取更多信息 for more information|
|shell [<shellCommand>]|连接模拟器/设施执行Shell命令，执行完毕后退出远程Shell端l| | |

### 常用用法：




1. 显示系统中全部Android平台： 

    android list targets 

2. 显示系统中全部AVD（模拟器）： 

    android list avd 

3. 创建AVD（模拟器）： 

    android create avd --name 名称 --target 平台编号 

4. 启动模拟器： 

    emulator -avd 名称 -sdcard ~/名称.img (-skin 1280x800) 

5. 删除AVD（模拟器）： 

    android delete avd --name 名称 

6. 创建SDCard： 

    mksdcard 1024M ~/名称.img 

7. AVD(模拟器)所在位置： 

    Linux(~/.android/avd)      Windows(C:\Documents and Settings\Administrator\.android\avd)


8. 启动DDMS： 

    ddms 

9. 显示当前运行的全部模拟器： 

    adb devices 

10. 对某一模拟器执行命令： 

      abd -s 模拟器编号 命令 

11. 安装应用程序： 

      adb install -r 应用程序.apk 

12. 获取模拟器中的文件： 

      adb pull <remote> <local> 

13. 向模拟器中写文件： 

      adb push <local> <remote> 

14. 进入模拟器的shell模式： 

      adb shell 

15. 启动SDK，文档，实例下载管理器： 

      android 

16. 缷载apk包： 

      adb shell 

      cd data/app 

      rm apk包 

      exit 

      adb uninstall apk包的主包名 

      adb install -r apk包 

17. 查看adb命令帮助信息： 

      adb help 

18. 在命令行中查看LOG信息： 

      adb logcat -s 标签名 

19. adb shell后面跟的命令主要来自： 

      源码\system\core\toolbox目录和源码\frameworks\base\cmds目录。 

20. 删除系统应用： 

      adb remount （重新挂载系统分区，使系统分区重新可写）。 

      adb shell 

      cd system/app 

      rm *.apk 

21. 获取管理员权限： 

      adb root 

22. 启动Activity： 

      adb shell am start -n 包名/包名＋类名（-n 类名,-a action,-d date,-m MIME-TYPE,-c category,-e 扩展数据,等）。


23、发布端口： 

    你可以设置任意的端口号，做为主机向模拟器或设备的请求端口。如： 

adb forward tcp:5555 tcp:8000 

24、复制文件： 

    你可向一个设备或从一个设备中复制文件， 

     复制一个文件或目录到设备或模拟器上： 

  adb push <source> <destination></destination></source> 

      如：adb push test.txt /tmp/test.txt 

     从设备或模拟器上复制一个文件或目录： 

     adb pull <source> <destination></destination></source> 

     如：adb pull /addroid/lib/libwebcore.so . 

25、搜索模拟器/设备的实例： 

     取得当前运行的模拟器/设备的实例的列表及每个实例的状态： 

    adb devices 

26、查看bug报告： 

adb bugreport 

27、记录无线通讯日志： 

    一般来说，无线通讯的日志非常多，在运行时没必要去记录，但我们还是可以通过命令，设置记录： 

    adb shell 

    logcat -b radio 

28、获取设备的ID和序列号： 

     adb get-product 

     adb get-serialno 

29、访问数据库SQLite3 

     adb shell 

     sqlite3 

#cd system/sd/data //进入系统内指定文件夹 

#ls //列表显示当前文件夹内容 

#rm -r xxx //删除名字为xxx的文件夹及其里面的所有文件 

#rm xxx //删除文件xxx 

#rmdir xxx //删除xxx的文件夹 







