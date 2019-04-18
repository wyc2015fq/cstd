# Windows 8 提示.Net Framework 3.5的解决办法 - weixin_33985507的博客 - CSDN博客
2012年11月08日 08:50:08[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
 Windows 8 默认集成 .Net Framework 4.5，因此运行一些基于3.5或以前版本的程序时会弹出这个提示。
![未命名.jpg](http://static.pb.pbcdn.com/data/attachment/forum/201203/02/1524304pmk0gvjossb2o4s.jpg.thumb.jpg)
2012-3-2 15:24 上传
[**下载附件**(23.91 KB)](http://bbs.pcbeta.com/forum.php?mod=attachment&aid=MjcxMDMyOHxiN2UwM2I0OXwxMzUyMzM1NjI1fDB8OTg0NjEx&nothumb=yes)
这时点安装此功能，可能会很长时间没有反应。
到微软官网下载.Net Framework 3.5离线安装包安装，也弹出上述提示。
最简单的解决办法：
用虚拟光驱加载Windows 8安装盘，记住虚拟光盘的盘符，然后以管理员身份运行CMD，将以下命令行复制进去运行：
如果你的Win8目录为C盘，CMD位于C:\Windows\system32\
**dism.exe /online /enable-feature /featurename:NetFX3 /Source:X:\sources\sxs**
此命令行中的X为Windows 8 虚拟光盘盘符，应该替换成你虚拟的目录。
运行后将会持续以下状态几分钟的时间，请耐心等待。
![1.jpg](http://static.pb.pbcdn.com/data/attachment/forum/201203/02/15242851qpz5s05uuyos55.jpg.thumb.jpg)
2012-3-2 15:24 上传
[**下载附件**(47.71 KB)](http://bbs.pcbeta.com/forum.php?mod=attachment&aid=MjcxMDMyNnxhNThjM2FhZXwxMzUyMzM1NjI1fDB8OTg0NjEx&nothumb=yes)
出现以下提示时，恭喜你已经安装成功。
![2.jpg](http://static.pb.pbcdn.com/data/attachment/forum/201203/02/152429ueg2ehfk5fdq2h8t.jpg.thumb.jpg)
