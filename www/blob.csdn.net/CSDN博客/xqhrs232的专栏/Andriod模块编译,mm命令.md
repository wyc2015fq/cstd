# Andriod模块编译,mm命令 - xqhrs232的专栏 - CSDN博客
2011年10月31日 22:05:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：640
原文地址::[http://jinguo.iteye.com/blog/691507](http://jinguo.iteye.com/blog/691507)
相关网帖::关于Android中编译单个工程的问题----[http://topic.csdn.net/u/20101123/20/57a817b6-4012-4790-980e-0a4a98c4afad.html](http://topic.csdn.net/u/20101123/20/57a817b6-4012-4790-980e-0a4a98c4afad.html)
如果你只需要修改某一个模块的内容，但是却每次都要执行make, 最后等待很长时间。
使用模块编译，那只需要在你所在的模块的目录或者其子目录，执行mm，便可以编译出一个单独的apk，这样岂不快哉！
具体步骤：
        1）打开~/.baserc文件，加入source ～/I850/build/envsetup.sh. 加入你自己该文件所在的路径，这样就免去了每次启动新的终端执行mm命令之前，需要引用此文件。
        2）完成步骤1之后，就可以在你的模块里面随意执行mm了，要想使用其他快速命令，可以查看envsetup.sh文件，比如cgrep，jgrep，resgrep在不同类型的文件里面进行相应的查询。还有m，mmm等等
        3）还可以使用adb push 将你的apk push到模拟器或者手机终端，也可以在工程根目录通过make －snod生成新的system.img
