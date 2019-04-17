# Appium自动获取 Android 设备 id 和包名等信息（python） - zhusongziye的博客 - CSDN博客





2017年09月05日 21:01:50[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3487











使用 Appium，每次换设备换 APP 测试时要在脚本修改很多东西，很麻烦，要自动获取可以通过指令获得的信息，使用该起来也方便。



最终效果是每次只需要修改测试的包的路径和包名就可以自动删除旧的安装包，并获得 Android 设备 id 和包名等 Appium 需要的信息。



适用于：Python + Android



使用到了 aapt，该工具在 SDK/tools 目录下，如果你没有设置环境变量，那需要进入该目录下才能运行 aapt 命令，如果设置了环境变量则可以在任何目录下运行 aapt 命令了。配置 aapt 环境变量的方法，请自行百度。


下面上代码：



```python
# -*- coding: utf-8 -*-

from appium import webdriver

# 使用正则表达式筛选设备 id
import re

# 使用time.sleep(xx)函数进行等待
import time

# 使用 os 模块调用命令
import os

# 测试的包的路径和包名
appLocation = " /Users/Downloads/app.apk "

# 读取设备 id
readDeviceId = list(os.popen('adb devices').readlines())

# 正则表达式匹配出 id 信息
deviceId = re.findall(r'^\w*\b', readDeviceId[1])[0]

# 读取设备系统版本号
deviceAndroidVersion = list(os.popen('adb shell getprop ro.build.version.release').readlines())
deviceVersion = re.findall(r'^\w*\b', deviceAndroidVersion[0])[0]

# 读取 APK 的 package 信息
appPackageAdb = list(os.popen('aapt dump badging ' + appLocation ).readlines())
appPackage = re.findall(r'\'com\w*.*?\'', appPackageAdb[0])[0]

# 删除以前的安装包
os.system('adb uninstall ' + appPackage)


desired_caps = {
    'platformName': 'Android',
    'platformVersion': deviceVersion,
    'deviceName': deviceId,
    'appPackage': appPackage,
    'appWaitPackage': appPackage,
    'app': appLocation,
    'appActivity': appPackage + ".PageSplash",


command_executor = "http://localhost:4723/wd/hub"
```





