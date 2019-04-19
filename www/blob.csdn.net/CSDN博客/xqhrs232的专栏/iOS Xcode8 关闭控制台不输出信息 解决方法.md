# iOS Xcode8 关闭控制台不输出信息~解决方法 - xqhrs232的专栏 - CSDN博客
2019年02月22日 16:43:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
原文地址::[https://blog.csdn.net/shihuboke/article/details/78131042](https://blog.csdn.net/shihuboke/article/details/78131042)
一、控制台展示效果
如图1:
二、解决办法：
1.文字流程
   Xcode8里边 Edit Scheme... -> Run -> Arguments, 在Environment Variables里边添加 OS_ACTIVITY_MODE ＝ disable
2.图行流程
图1:
图2:
图3:
谢谢!!!
--------------------- 
