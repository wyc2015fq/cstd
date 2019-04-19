# Android仿酷狗动感歌词（支持翻译和音译歌词）显示效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月02日 16:41:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：126

# 引言
该开源依赖库是乐乐音乐播放器里的一个歌词模块功能，现在把该功能模块独立出来进行优化，并弄成了一个开源依赖库，其它音乐播放器项目只要引用该库并调用接口，便可轻松实现与乐乐音乐播放器一样的动感歌词显示效果，其项目地址如下：[乐乐音乐播放器](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzhangliangming%2FHappyPlayer5.git)。
# 实现功能
## 歌词解析和生成
- 
支持krc、ksc、lrc歌词格式
- 
hrc歌词：乐乐音乐播放器自定义的动感歌词格式，如需制作歌词（支持动感歌词、翻译歌词和音译歌词制作），可点击链接查看：[Hrc歌词制作器（乐乐音乐播放器PC版）](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzhangliangming%2FHappyPlayer-PC)
- 
歌词格式可自行扩展和相互转换
## 双行歌词
- 
不支持lrc格式歌词的显示
- 
支持动感歌词、翻译歌词和音译歌词的显示
- 
歌词颜色渐变方面，主要是参考酷狗PC版桌面歌词的颜色【蓝-白-蓝】
- 
支持歌词字体大小、字体包、颜色、歌词换行等功能
## 多行歌词
- 
支持lrc、krc和hrc等动感歌词的显示
- 
支持动感歌词、翻译歌词和音译歌词的显示
- 
歌词颜色透明度渐变、歌词平滑移动、歌词快进
- 
支持歌词字体、颜色、歌词换行等功能
- 
动感歌词的翻译歌词，可分为lrc和动感（默认）两种方式绘画显示
# 预览图
## 主界面
![](https://upload-images.jianshu.io/upload_images/4111431-7b611cc2bb23d4ea..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
Android仿酷狗动感歌词（支持翻译和音译歌词）显示效果
## 歌词文件读取并预览
![](https://upload-images.jianshu.io/upload_images/4111431-f4cc3dcf72910d1d..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image
## 双行歌词-动感歌词
![](https://upload-images.jianshu.io/upload_images/4111431-8c7067fd0250de86..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image
## 双行歌词-音译歌词
![](https://upload-images.jianshu.io/upload_images/4111431-891db56caa9f667b..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image
## 双行歌词-翻译歌词
![](https://upload-images.jianshu.io/upload_images/4111431-115e31f9c28e8fce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
wlWCzSr.png
## 多行歌词-lrc歌词
![](https://upload-images.jianshu.io/upload_images/4111431-1251bc3d74fd6749..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image
## 多行歌词-动感歌词
![](https://upload-images.jianshu.io/upload_images/4111431-d52b6594a57fda53..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image
## 多行歌词-音译歌词
image
## 多行歌词-翻译歌词
image
## 多行歌词-快进
image
# 实现思路
## 歌词解析器和生成器
- 
该项目对支持的krc、ksc、lrc和hrc歌词格式，都封装了两个类，一个类（LyricsFileReader）是用来读取歌词文件并解析歌词内容的，另外一个类（LyricsFileWriter）是用来生成歌词文件，用户可自行扩展歌词格式。
- 
其中，歌词解析和生成只要用了正则表达式和字符串分隔来实现。
- 
关于歌词的格式内容和标签，可点击链接查看，也可以在浏览器上输入关键字“浅谈动感歌词”进行搜索，我之前写了“浅谈动感歌词”系列来简单说明我对动感歌词的一些看法：[浅谈动感歌词-歌词分析篇](https://www.jianshu.com/p/ad00388bd139)、[浅谈动感歌词-歌词生成篇](https://www.jianshu.com/p/85343f5a92ff)、[浅谈动感歌词-歌词解析篇](https://www.jianshu.com/p/1806fa8756fd)、[浅谈动感歌词-歌词显示篇](https://www.jianshu.com/p/7063e5acdc5b)、[浅谈动感歌词-歌词补充篇](https://www.jianshu.com/p/7ab6046202d5)
## 翻译歌词和音译歌词构建
翻译歌词和音译歌词里的字时间，只要从原始的歌词里面的字时间集合里重构，由于翻译歌词的字数和原始歌词不一定对应，所以翻译歌词每个字的渐变时间，为该行歌词时长除于字总数的平均值。具体可参考：[浅谈动感歌词-歌词补充篇](https://www.jianshu.com/p/7ab6046202d5)
## 动感歌词渐变效果
API：canvas.clipRect
## 双行歌词上下颜色渐变
API：LinearGradient，目前设置了上下颜色渐变，颜色是平均分布。
## 歌词刷新优化
- 
注意：从Handler.postDelayed(Runnable, 0)到Runnable里面的执行是有消耗时间的。
- 
旧版思路：乐乐音乐之前是通过线程每隔100ms便通过发送广播将歌曲的播放进度发给歌词类，让它去刷新歌词的内容，从而达到歌词与歌曲对应的效果，不过这几天优化的时候发现，每隔100ms更新并通过handler进行刷新时，其实还需要考虑handler从发送到执行请求时，所消耗的时间已经不止100ms，加上广播过于频繁，这应该就是乐乐音乐的歌词在渐变的时候不太流畅的原因。
- 
现在思路：乐乐音乐播放器加载完成歌词后，需要从player中将当前歌曲的播放进度（playProgress）传给歌词，**之后歌词类便操作这个播放进度值（playProgress）去刷新界面，这里的思路是：每隔1ms，播放进度（playProgress）便要添加1ms。**由于handler每次从发送到执行时，需要消耗时间，这里我直接使用该消耗时间作为播放进度的增加值，对歌词界面进行刷新，从而达到歌词与歌曲对应的效果。
- 
个人感觉，如果想歌词更新流畅，用C来实现，效果应该会比较好
- 
类似的，歌曲的进度条显示也可以使用该种方法。
# 源码
具体的代码和调用Demo，可到源码地址处查看：
[Android仿酷狗动感歌词（支持翻译和音译歌词）显示效果](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzhangliangming%2FHPLyrics.git)
# 传送门
- 
[乐乐音乐播放器-Android](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzhangliangming%2FHappyPlayer5)
- 
[Hrc歌词制作器（乐乐音乐播放器PC版）](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzhangliangming%2FHappyPlayer-PC)
# 扩展
- 
歌词格式转换器
- 
歌词文件生成视频
# 最后
如果有问题，可到github上面留言。
作者：只有一条尾巴的猫
链接：https://www.jianshu.com/p/9e7111db7b41
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
