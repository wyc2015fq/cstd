# Appium基础(四)查找app的appActivity与appPackage - _天枢 - 博客园
## [Appium基础(四)查找app的appActivity与appPackage](https://www.cnblogs.com/yhleng/p/7519660.html)
2017-09-14 11:14 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7519660)
**要查看appActivity需要借助日志：adb logcat>D:/log.log **
**前提是已经装了Android SDK**
**在目录D:\Program Files (x86)\android\android-sdk\platform-tools**
**下，我们会看到adb.exe**
** adb即 Android Debug Bridge,Android调试桥，开发会经常用到**
**1、命令行模式下，进入到此目录，执行adb logcat>D:/log.log**
**最好是将这个目录，设置系统环境变量path中，这样在哪个目录都可以执行adb命令**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170914111027203-457888385.png)
**设备已打开，会出现如上图，如果没打开会提示等待设备 **
**2、启动Android模拟器，打开app应用**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170914111133860-147569601.png)
**3、停止logcat，查看日志，找到我们需要的appActivity；appPackage**
**ctrl+c停止日志捕获**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170914111057828-1857516932.png)
** 查看log文件：我们存执行命令**adb logcat>D:/log.log **时****，指定的路径**
**搜索  android.intent.category.LAUNCHER  这是启动应用程序标志**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170914110718844-966740838.png)
**从日志中我们找到I/ActivityManager：com.android.calculator2/.Calculator**
****.Calculator**就是我们要找的appActivity**
**com.android.calculator2就是appPackage**
**以上两个，在做python开发配置的时候是需要的。**
