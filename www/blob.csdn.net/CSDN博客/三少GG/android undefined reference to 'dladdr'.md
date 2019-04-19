# android: undefined reference to 'dladdr' - 三少GG - CSDN博客
2013年06月10日 16:11:59[三少GG](https://me.csdn.net/scut1135)阅读数：3539
 XXX error: undefined reference to 'dladdr'
ndk api 8以下不支持libdl xxx 中某些特性的原因，添加
Application.mk
APP_PLATFORM  := android-8
再次编译成功.
