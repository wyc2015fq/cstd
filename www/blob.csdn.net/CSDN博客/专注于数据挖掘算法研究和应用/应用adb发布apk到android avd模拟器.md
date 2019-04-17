# 应用adb发布apk到android avd模拟器 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年06月15日 15:04:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2520标签：[android																[tools																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)





1）复制需要安装的APK文件到Android SDK的Tools文件夹中或platform-tools中（看adb.exe在那个文件夹）；

2）执行cmd命令，切换目录到Android SDK的Tools或platform-tools路径，执行命令：adb installapkName；如要重装，则adb install -r apkName；

3）打开模拟器，寻找该应用启动即可；](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




