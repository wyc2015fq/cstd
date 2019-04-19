# iOS多模式&富交互视频播放器TTAVPlayer(附源码) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月02日 17:25:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1213
本文来自简书，原文地址:[http://www.jianshu.com/p/1deb9a590cd6](http://www.jianshu.com/p/1deb9a590cd6)
源码可见:[[直接点击](https://github.com/tangdiforx/TTAVPlayer)] (如果喜欢文章，大家进去点个star支持一下，多谢~)
1.背景
最近开始抓端体验，播放器作为一个基础功能一直为人诟病:交互不友好，如手势调节播放进度，音量，屏幕亮度，以及对网络状态的处理等。同时也不能支持现在业务场景多样化对播放器的需求等等。于是决定在这一期进行“整治”。
首先为了支持业务需求的多样化，视频播放器需要支持四种模式:普通模式，竖屏模式，横屏模式，静音模式。
> 
普通模式:商品内容，文章内容嵌套播放器，H5桥接播放。
静音模式:列表自动播放时需要，如手淘微淘列表。
竖屏模式:浸入式体验播放，常常用于H5桥接播放与静音播放时点击查看详情，如微博，手淘微淘。
横屏模式:最佳播放体验，具有最丰富的交互操作，如快捷调节音量，播放进度，屏幕亮度。
最终实现效果部分如下:
#### 普通模式-全屏模式
![](http://upload-images.jianshu.io/upload_images/5806025-cbc399c1d3836fb8.gif?imageMogr2/auto-orient/strip)
竖屏模式-全屏模式
![](http://upload-images.jianshu.io/upload_images/5806025-63b7548b13c5f0cb.gif?imageMogr2/auto-orient/strip)
# 2.设计概要
对于基础功能，无论是API级别还是Framework级别，我的设计思想都是"保证最小的接入成本同时保证最大的扩展性"。也就是，对于绝大部分的情况，提供简单易用的API让接入方可以非常方便的接入。但是当接入方需要自定义时，我们也要提供强大的自定义能力。
那回归到播放器，在第一小节中提到的四种模式其实已经可以满足绝大部分的场景需求。把这些模式预置进播放器设计中，就已经可以保证尽可能少的接入成本，只要在设计上进行分层，把基础功能和UI部分以及各种模式特有的Feature进行分层，新增自定义模式，只提供播放器View的基础控制接口，如播放，暂停，调整进度，全屏等，然后UI部分部分可以完全交由接入方自定义。
# 3.详细设计
基于第二小节的设计思想，我们可以确定播放器的五种模式:
![](https://img.alicdn.com/tfs/TB1YFjHRpXXXXX7aFXXXXXXXXXX-376-125.png)
为了尽量少的代码引入，我们选择基于苹果自带的AVPlayer进行播放器开发。关于AVPlayer，在此不做赘述。它用来负责视频文件的解码，播放，以及一些基础的播放操作。
于是，我们便基于AVPlayer创建出我们的TTAVPlayer。除此之外，为了让用户看到视频以及一些UI控件用以提示和交互，还需要添加TTAVPlayerView。TTAVPlayer负责给TTAVPlayerView提供视频文件的播放，暂停，进度调整等基本操作，以及播放状态的回调，而TTAVPlayerView是用户可见的部分，并直接操作TTAVPlayer。
基于以上的思想，可画出以下的设计图:
![](http://upload-images.jianshu.io/upload_images/5806025-1df2f53c6709489c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
TTAVPlayer持有一个AVPlayerItem的实例，它提供了我们访问一个视频文件(AVAsset)的接口，如当前播放时间，视频总时间，播放完成，失败的回调等等。在TTAVPlayer这一层，我们基本上可以把所有的播放类的操作进行封装起来，仅仅向外暴露有关视频播放的代理方法，上层只需要去实现这些代理方法就可以得到整个视频播放时需要的信息回调：
![](http://upload-images.jianshu.io/upload_images/5806025-4b792a1d558dd2ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
而TTAVPlayerView则是设计出来面向于真正的上层调用者的。这个调用者可能是预设模式，可能是使用自定义模式的接入方。所以TTAVPlayer提供出来的代理方法对上层调用者应该是透明的，而需要由TTAVPlayer提供出调用者真正想需要的代理接口。这些接口不仅全量包含了TTAVPlayerView的回调，还包括一些跟用户相关的回调，比如播放器全屏的回调，视频被点击的回调，播放暂停的回调，View加载完成的回调等等。
这样的设计保证了TTAVPlayerView代码的纯净性，它与业务代码解耦，只负责了视频播放本身，而不去关心上面的界面渲染以及一些业务操作逻辑。这为自定义模式的扩展性打下了基础。基于此，我们又有了以下设计:
![](http://upload-images.jianshu.io/upload_images/5806025-323f0e384cd58a13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 4.丰富的Feature
为了提供一个高完成度播放器，我们提供了丰富的Feature:
> 
1. 横屏模式下手势识别，控制快进/快退、音量调节、屏幕亮度调节，向流行App看齐。
2. 检测屏幕方向自动切换横竖屏，并且考虑到很多App并未打开横屏开关，采用“假横屏”的方式来模拟，节省了接入成本。
3. 容错提示页面，温馨提示，提升体验。
4. 检测网络切换功能，当网络从WIFI切到数据流量时，自动暂停视频；
5. 静音播放模式，让用户在公共场所看视频不尴尬。
# 5.Tips
> 
1. 如对代码有不解或者发现Bug以及希望增加新的Feature，可直接github提issue，我会尽量解掉。
2. Demo请尽量运行在真机上。
3. 需要探讨可直接联系我，联系方式如下。
知乎:[[直接点击](https://www.zhihu.com/people/tang-di-78)]
GitHub:[[直接点击](https://github.com/tangdiforx/TTAVPlayer)]
简书:[[直接点击](http://www.jianshu.com/p/1deb9a590cd6)]
