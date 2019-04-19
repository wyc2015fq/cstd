# iOS 工程瘦身 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月13日 09:43:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：360
从上个版本开始，工程的Appstore安装包竟然达到了53M,简直到了不能忍的地步，工程瘦身势在必行。
1，删除自定义字体文件
工程中功能引导使用了自定义字体，自定义字体文件足有17M,使用自定义字体固然提高页面美观性，但是直接导致 工程打包后的 ipa文件增加了将近12M,
![](http://upload-images.jianshu.io/upload_images/1299701-f8bc6005bc7463a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1299701-f3833eece3ab519e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2，替换较大的图片资源，删除没有用到的图片资源
终端cd到我们工程目录下面，使用命令  查看工程中 大于500k的资源
![](http://upload-images.jianshu.io/upload_images/1299701-170a50e6b57dfb67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
给 500换成 300以及1000都可以，然后就能找到 较大的图片资源，然后到我们的工程中 image.xcasset 找到相应的图片 查看一下，我这里有好几个 44*44 的图标结果有500多kb，于是我找到我们的UI，更换了小的图片。这分明是 图片没有处理好的问题。
下一步就是删除现在工程中不再使用的图片资源，到github 下载 mac 工程，这是外国一大牛写的：[https://github.com/jeffhodnett/Unused](https://github.com/jeffhodnett/Unused)，运行之后如图
![](http://upload-images.jianshu.io/upload_images/1299701-85655f3b426108a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1299701-4eb932b6c117bf79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
工程较大的话会比较慢
我接下来的做法是到工程中image.xcasset 找到相应的图片删除掉，经过这两部工程就少了2.3M,好吧，2.3M也不小了。。。
![](http://upload-images.jianshu.io/upload_images/1299701-1db4f9cdb15e0f85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文／悟_空（简书作者）
原文链接：http://www.jianshu.com/p/59d8868ee760
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
