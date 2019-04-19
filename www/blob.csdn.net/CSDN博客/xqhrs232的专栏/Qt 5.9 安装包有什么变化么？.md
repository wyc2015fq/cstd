# Qt 5.9 安装包有什么变化么？ - xqhrs232的专栏 - CSDN博客
2018年09月05日 14:53:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：618
原文地址::[https://www.zhihu.com/question/265157388/answer/303468279](https://www.zhihu.com/question/265157388/answer/303468279)
# Qt 5.9 安装包有什么变化么？
一直用的5.6，好久不用，发现5.9是LTS，打算安装一下，没想到安装包和5.6不太一样，可以解释下5.9 windows 三个包有什么区别
1. [qt-opensource-windows-x86-5.9.3.exe](https://link.zhihu.com/?target=http%3A//download.qt.io/official_releases/qt/5.9/5.9.3/qt-opensource-windows-x86-5.9.3.exe) 包含了 mingw和msvc？
2.uwp.zip desktop.zip 和 exe 有什么关系？
![](https://pic2.zhimg.com/v2-88c53e35842673e1736ba7bee8148ee1_b.jpg)
关注者
**9**
被浏览
**1,446**
关注问题写回答
​添加评论
​分享
​邀请回答
​
收起
[查看全部 5 个回答](https://www.zhihu.com/question/265157388)
![秋城](https://pic2.zhimg.com/v2-aa47bc4f9b90a95271c5d832b000c527_xs.jpg)
[秋城](https://www.zhihu.com/people/qiuchengw)
[http://piaode.ren](https://link.zhihu.com/?target=http%3A//piaode.ren)
2 人赞同了该回答
1，合并了同一平台下各种编译器编译的Qt二进制包。（msvc，mingw等）。但是去掉了调试文件（win下就是pdb文件）。以前的包里面是带有pdb的。
2，看清楚文件名，两个zip都是pdb的打包而已，也就是问题1里面去掉的这部分。如果你不需要调试Qt源代码，则可以不用下载pdb包。建议最好下载，因为有时候还是需要调试进qt的源码里面，看看具体运行信息的。非常有用，也非常有助于学习Qt源码。
