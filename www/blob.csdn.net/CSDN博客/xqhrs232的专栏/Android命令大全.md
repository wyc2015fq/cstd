# Android命令大全 - xqhrs232的专栏 - CSDN博客
2011年04月02日 16:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：813标签：[android																[shell																[list																[delete																[action																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/mayingcai1987/archive/2011/02/23/6203545.aspx](http://blog.csdn.net/mayingcai1987/archive/2011/02/23/6203545.aspx)
相关网址::[http://blog.sina.com.cn/s/blog_5f30b9410100pcvw.html](http://blog.sina.com.cn/s/blog_5f30b9410100pcvw.html)
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
    Linux(~/.android/avd)      Windows(C:/Documents and Settings/Administrator/.android/avd)
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
      源码/system/core/toolbox目录和源码/frameworks/base/cmds目录。
20. 删除系统应用：
      adb remount （重新挂载系统分区，使系统分区重新可写）。
      adb shell
      cd system/app
      rm *.apk
21. 获取管理员权限：
      adb root
22. 启动Activity：
      adb shell am start -n 包名/包名＋类名（-n 类名,-a action,-d date,-m MIME-TYPE,-c category,-e 扩展数据,等）。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/mayingcai1987/archive/2011/02/23/6203545.aspx](http://blog.csdn.net/mayingcai1987/archive/2011/02/23/6203545.aspx)

