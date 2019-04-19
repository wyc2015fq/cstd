# DELETE_FAILED_INTERNAL_ERROR Error while Installing APK - =朝晖= - 博客园
# [DELETE_FAILED_INTERNAL_ERROR Error while Installing APK](https://www.cnblogs.com/dhcn/p/7130630.html)
      真是Android2.3的特殊版本问题，问题原因是android2.3的instant run的[测试](http://lib.csdn.net/base/softwaretest)版安装方式有所特别，解决办法有2：
     1、手动adb install 安装包
     2、把Instant run关闭掉。
参考资料：
http://stackoverflow.com/questions/42687607/application-installation-failed-in-[Android](http://lib.csdn.net/base/android)-studio
http://stackoverflow.com/questions/38892270/delete-failed-internal-error-error-while-installing-apk

