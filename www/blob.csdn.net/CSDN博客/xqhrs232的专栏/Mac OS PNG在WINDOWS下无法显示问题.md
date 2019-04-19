# Mac OS PNG在WINDOWS下无法显示问题 - xqhrs232的专栏 - CSDN博客
2019年02月27日 14:51:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：44
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/canglang_123/article/details/39370383](https://blog.csdn.net/canglang_123/article/details/39370383)
相关文章
1、mac下png图片在windows不显示根源分析----[https://www.jianshu.com/p/15e5e6187004](https://www.jianshu.com/p/15e5e6187004)
有的PNG在MAC下可以显示，但拿到WINDOWS下就不能显示。就是因为这些文件是经过xcode编译优化过的，所以无法直接查看。
我看网上说是 "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/pngcrush
\ -revert-iphone-optimizations -q Local.png Local-standard.png" 这个，但我在自己的MAC(4.4)上没找到。
-------------------------------------------------------------------------------------------------------------------------
用下面命令查看 XCode folder 路径信息
命令: xcode-select -print-path
显示: /Applications/Xcode.app/Contents/Developer/
 -------------------------------------------------------------------------------------------------------------------------
最后用下面的命令搞定了
命令: /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/pngcrush -revert-iphone-optimizations /other/bbbbbc/a94@2x.png /other/bbbbbc/test11111.png
 -------------------------------------------------------------------------------------------------------------------------
批处理还原指定的图片(下面就是把/other/new/*.png 还原到 /other/new1 文件夹中)
命令: /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/pngcrush -dir /other/new1  -revert-iphone-optimizations -q /other/new/*.png
