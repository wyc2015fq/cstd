# FFmpeg编译：jni not found 的问题 - 夜行过客 - 博客园







# [FFmpeg编译：jni not found 的问题](https://www.cnblogs.com/yongdaimi/p/9772235.html)






进入*Android\Sdk\ndk-bundle\platforms\android-xx\arch-arm\usr*目录查看发现与Google官方下载的NDK相比缺少include目录

此目录下包含各种头文件，下载后完整替换原ndk-bundle下得platforms文件夹重新编译即可。






这个ndk-bundle是用AS上的SDK Manager下载安装的，试了好几次，下载完毕后都没有include目录，但若直接在Google官方手动下载NDK的压缩包就有include目录，不知道为什么。












