# 如何打开Nib文件 - xqhrs232的专栏 - CSDN博客
2019年04月10日 17:23:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：25
原文地址::[https://blog.csdn.net/ani_di/article/details/9186979](https://blog.csdn.net/ani_di/article/details/9186979)
相关文章
1、iOS中nib文件分析----[https://www.jianshu.com/p/eb53ccf28680](https://www.jianshu.com/p/eb53ccf28680)
2、iOS 使用nib文件（一）----[https://blog.csdn.net/u011367651/article/details/47292981?utm_source=blogxgwz1](https://blog.csdn.net/u011367651/article/details/47292981?utm_source=blogxgwz1)
3、IOS 使用XIB 自定义View----[https://blog.csdn.net/li6185377/article/details/8131042](https://blog.csdn.net/li6185377/article/details/8131042)
如何打开Nib文件
有段时间没有灌水，今天分享一个小技巧。
nib文件是Cocoa App的界面资源，代码中一般用xib代替了nib，因为这样方便版本管理。发布出去的仍然是nib，而且是编译后的nib，这种文件直接用xcode是不能被打开的。
可以手工修改一下nib就可以打开了。修改方法：
下载Compiled Nib Opener.nib.zip，解压
右键Compiled Nib Opener.nib -> “Show package content"
用需要打开的nib替换keyedobjects.nib
返回，双击打开Compiled Nib Opener.nib
嫌麻烦用这个工具
