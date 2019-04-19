# android monkey  app乱点测试 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月19日 13:57:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：175
Monkey是Android中的一个命令行工具
查看包名：查看电脑中某一位置的apk文件的包名：PC打开CMD-进入TMG目录-运行设备--查看包名aapt dump badging *.apk（apk文件的路径，可以直接拖拽）。
adb shell
cd    /data/data    
输入 ls  命令；可以列出data文件夹中所有的app包名
monkey  –p  com.zhihu.android **--monitor-native-crashes ** –vvv  500  > monkeytest.txt  
com.zhihu.android这个就是要测试应用包名。可以看见手机上面知乎app开始自运行了。
 后面的500  进行500 次完全随机的测试。 monkey会随机的生成500次事件。注意不是-v的选项值。-v应该是一个单独的选项，没有值。-v 为 verbose的缩写， -vvv
3个v就是输出等级1至3的所有事件
-p用来指定是哪个包
时间间隔--throttle为600ms，事件数为2000
要停止就直接kill
--throttle <milliseconds>
在事件之间插入固定延迟。通过这个选项可以减缓Monkey的执行速度。如果不指定该选项，Monkey将不会被延迟，事件将尽可能快地被产成。
**--monitor-native-crashes**
**重现bug：monkey日志搜索关键词ANR exception，将之前的事件重新操作，尤其是seed值要一模一样，如monkey -p 包名 -v seed 0 500**
**日志分析：查看是否有crash， **Exception**等关键字，找上下文，进行简单分析将你所能定位的错误信息发给开发。**
在log的最开始都会显示Monkey执行的seed值、执行次数和测试的包名。

参数
[https://blog.csdn.net/jlminghui/article/details/38238443](https://blog.csdn.net/jlminghui/article/details/38238443)
示例
[https://blog.csdn.net/TskyFree/article/details/41868813](https://blog.csdn.net/TskyFree/article/details/41868813)
