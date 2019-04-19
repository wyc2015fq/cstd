# iOS播放远程网络音乐的核心技术点 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月07日 17:32:27[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1280
![](http://upload-images.jianshu.io/upload_images/2756501-050160b9b0f2aa53.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
播放远程网络音乐.jpg
**一、前言**
这两天做了个小项目涉及到了远程音乐播放，因为第一次做这种音乐项目，边查资料边做，其中涉及到主要技术点有：
- 如何播放远程网络音乐
- 如何切换当前正在播放中的音乐资源
- 如何监听音乐播放的各种状态（播放器状态、播放的进度、缓冲的进度，播放完成）
- 如何手动操控播放进度
- 如何在后台模式或者锁屏情况下正常播放音乐
- 如何在锁屏模式下显示音乐播放信息和远程操控音乐
如果您对一块技术点有兴趣或者正在寻找相关资料，那么本篇或许能提供一些参考或启发。
**二、 网络音乐播放的核心技术点**
根据自己的经验和查了一些音乐播放的相关资料，最简单和最易上手的的技术方案我想应该是采用ios系统自带的AVFoundation框架。
我们知道AVFoundation框架是苹果专门为多媒体打造的一个库，这个库非常强大，专门用来处理音视频等复杂的多媒体技术，而本篇要讲的所有技术点就是基于AVFoundation框架中的一个类——AVPlayer。
那么AVPlayer是什么？
你可以把他看成是一个已经封装好的播放器，它的作用是用来播放远程的或本地的视频和音频。因为本地的音视频的播放比较简单，这里就不做讲述，本编主要是讲远程音乐播放，因为都是基于AVPlayer同一套API，所以掌握远程音乐播放其实就是相当于掌握远程视频播放。好了废话就不多说了，下面开始上菜。
**1、导入AVFoundation框架，创建AVPlayer播放器**
```
-(AVPlayer *)player
{
    if (_player == nil) {
         // AVPlayerItem是一个包装音乐资源的类，初始化时可以传入一个音乐的url
        AVPlayerItem *item = [[AVPlayerItem alloc] initWithURL:[NSURL URLWithString:@"http://xxxxxxxx"]];
        //通过AVPlayerItem初始化player
        _player = [[AVPlayer alloc] initWithPlayerItem:item];
    }
    return _player;
}
```
此处懒加载创建，让播放器成为控制器的全局属性，注意需要强引用，否则回收释放掉了就无法播放。
**2、播放或停止音乐**
```
//开始播放
    [self.player play];
     //停止播放
    [self.player pause];
```
这个没什么好讲的，只要调用AVPlayer的两个实例方法
**3、切换当前正在播放中的音乐资源**
```
//创建需要播放的AVPlayerItem
 AVPlayerItem *item = [[AVPlayerItem alloc] initWithURL:[NSURL URLWithString:model.url]];
 //替换当前音乐资源  
 [self.player replaceCurrentItemWithPlayerItem:item];
```
这个可以用于歌曲的切换，如上一首、下一首。
**4、通过KVO监听播放器的状态**
` [self.player.currentItem addObserver:self forKeyPath:@"status" options:NSKeyValueObservingOptionNew context:nil];`
拿到播放器的`currentItem`，注册当前对象为观察者，监听它的`status`属性。`status`属性是`AVPlayerItemStatus`类型，它是一个枚举类型，如下：
```
typedef NS_ENUM(NSInteger, AVPlayerItemStatus) {
    AVPlayerItemStatusUnknown,//未知状态
    AVPlayerItemStatusReadyToPlay,//准备播放
    AVPlayerItemStatusFailed//加载失败
};
```
当status属性值发生改变时，就会触发观察者方法的回调，如下：
```
//观察者回调
-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context
{
   //注意这里查看的是self.player.status属性
    if ([keyPath isEqualToString:@"status"]) {
        switch (self.player.status) {
            case AVPlayerStatusUnknown:
            {
                NSLog(@"未知转态");
            }
                break;
            case AVPlayerStatusReadyToPlay:
            {
                NSLog(@"准备播放");
            }
                break;
            case AVPlayerStatusFailed:
            {
                NSLog(@"加载失败");
            }
                break;
                 default:
                break；
        }
   }
}
```
当 `self.player.status == AVPlayerStatusReadyToPlay`时，音乐就会开始正常播放，另外两种状态音乐是无法播放的，可以在上面方法相应状态里给出提示。**这里需要特别强调一点的是观察者监听的对象是`self.player.currentItem`，而不是`self.player`，而当监听的属性发生改变时，观察者回调的方法里需要查看的是`self.player.status`。当然，你也可以不这么干，但是我尝试过好几次，不这么干的后果是无法监听到`self.player.status`属性的改变。**
当音乐播放完成，或者切换下一首歌曲时，请务必记得移除观察者，否则会crash。操作如下：
```
//移除观察者
 [self.player.currentItem removeObserver:self forKeyPath:@"status"];
```
**5、监听音乐的缓冲进度**
这个也是通过KVO监听播放器当前播放的音乐资源`AVPlayerItem`的`loadedTimeRanges`属性。我们先看监听，如下：
```
//KVO监听音乐缓冲状态
[self.player.currentItem addObserver:self forKeyPath:@"loadedTimeRanges" options:NSKeyValueObservingOptionNew context:nil];
```
当`loadedTimeRanges`属性发生改变时，回调如下：
```
-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"loadedTimeRanges"]) {
        NSArray * timeRanges = self.player.currentItem.loadedTimeRanges;
        //本次缓冲的时间范围
        CMTimeRange timeRange = [timeRanges.firstObject CMTimeRangeValue];
        //缓冲总长度
        NSTimeInterval totalLoadTime = CMTimeGetSeconds(timeRange.start) + CMTimeGetSeconds(timeRange.duration);
        //音乐的总时间
        NSTimeInterval duration = CMTimeGetSeconds(self.player.currentItem.duration);
        //计算缓冲百分比例
        NSTimeInterval scale = totalLoadTime/duration;
        //更新缓冲进度条
       self.loadTimeProgress.progress = scale;
    }
}
```
loadedTimeRanges这个属性是一个数组，里面装的是本次缓冲的时间范围，这个范围是用一个结构体`CMTimeRange`表示，当然在oc中结构体是不能直接存放数组的，所以它被包装成了oc对象`NSValue`。
我们来看下这个结构体：
```
typedef struct
{
    CMTime            start;        
    CMTime            duration;    
} CMTimeRange;
```
start表示本次缓冲时间的起点，duratin表示本次缓冲持续的时间范围，具体详细的计算方法可以看上面方法的实现。
当音乐播放完成，或者切换下一首歌曲时，请务必记得移除观察者，否则会crash。操作如下：
`[self.player.currentItem addObserver:self forKeyPath:@"loadedTimeRanges" options:NSKeyValueObservingOptionNew context:nil];`
**6、监听音乐播放的进度**
这个不是通过KVO了，AVPlayer专门提供了下面这个api用来监听播放的进度：
```php
```php
/**
 监听音乐播放进度
 @param interval 监听的时间间隔，用来设置多长时间回调一次
 @param queue    队列，一般传主队列
 @param block    回调的block，会把当前的播放时间传递过来
 @return 监听的对象
 */
- (id)addPeriodicTimeObserverForInterval:(CMTime)interval queue:(nullable dispatch_queue_t)queue usingBlock:(void (^)(CMTime time))block;
```
```
操作如下：
```
__weak typeof(self) weakSelf = self;
    self.timeObserver =  [self.player addPeriodicTimeObserverForInterval:CMTimeMake(1.0, 1.0) queue:dispatch_get_main_queue() usingBlock:^(CMTime time) {
        //当前播放的时间
        float current = CMTimeGetSeconds(time);
        //总时间
        float total = CMTimeGetSeconds(item.duration);
        if (current) {
            float progress = current / total;
            //更新播放进度条
           weakSelf.playSlider.value = progress;
            weakSelf.currentTime.text = [weakSelf timeFormatted:current];
        }
    }];
```
我们可以这个block里面拿到当前播放时间，根据总时间计算出当前播放所占的时间比例，最后更新播放进度条。这里又涉及到了一个数据类类型CMTime，它也是一个结构体，用来作为时间的格式，定义如下：
```
typedef struct
     CMTimeValue    value;        
     CMTimeScale    timescale;    
     CMTimeFlags    flags;        
     CMTimeEpoch    epoch;        
   } CMTime;
```
CMTime是以分数的形式表示时间，value表示分子，timescale表示分母，flags是位掩码，表示时间的指定状态。所以我们要获得时间的秒数需要分子除以分母。当然你还可以用下面这个函数来获取时间的秒数：
`Float64 CMTimeGetSeconds(CMTime time)`
最后，当音乐播放完成或者切换音乐时，依然需要移除监听：
```
if (self.timeObserver) {
        [self.player removeTimeObserver:self.timeObserver];
        self.timeObserver = nil;
    }
```
**7、手动超控（移动滑块）播放进度**
这是一个播放音视频很常见的功能，所以强大的AVPlayer理所当然的提供了几个api，下面只讲述其中最简单的一个：
```
```
/**
 定位播放时间
 @param time 指定的播放时间
 */
- (void)seekToTime:(CMTime)time;
```
```
具体使用如下：
```
//移动滑块调整播放进度
- (IBAction)playSliderValueChange:(UISlider *)sender
{
    //根据值计算时间
    float time = sender.value * CMTimeGetSeconds(self.player.currentItem.duration);
    //跳转到当前指定时间
    [self.player seekToTime:CMTimeMake(time, 1)];
}
```
**8、监听音乐播放完成**
一般音视频播放完成时我们或多或少的都要处理一些业务，比如循环播放，播完退出界面等等。下面看下如何监听AVPlayer的播放完成。
```
//给AVPlayerItem添加播放完成通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:_player.currentItem];
```
这里是采用注册监听`AVPlayerItemDidPlayToEndTimeNotification`通知，当AVPlayer一播放完成时，便会发出这个通知，我们收到通知后进行处理即可
**9、设置音乐后台播放**
我们知道运行在ios系统下的程序一旦进入后台就会处于休眠状态，程序停止运行了，也就播放不了什么音乐了。但是有一些特定功能的app还是处于可以后台运行的，比如音乐类型的app正处于这个范畴。但是，并不是说你在应用中播放音乐就能后台高枕无忧的运行了，你依然需要做如下几步操作：
（1）开启后台模式
target ->capabilities-> Background modes ->打开开关 ->勾选第一个选项
![](http://upload-images.jianshu.io/upload_images/2756501-d937b7fde84dfe72.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置后台播放模式
（2）程序启动时设置音频会话
```
//一般在方法：application: didFinishLaunchingWithOptions:设置
   //获取音频会话
    AVAudioSession *session = [AVAudioSession sharedInstance];
    //设置类型是播放。
    [session setCategory:AVAudioSessionCategoryPlayback error:nil];
    //激活音频会话。
    [session setActive:YES error:nil];
```
以上两步设置无误，程序进入后台模式，便可以进行音乐播放
**10、如何设置音乐锁频信息**
我们看百度音乐锁频时，也依然能在屏幕上展示歌曲的信息，以及切换歌曲等。下面看看这个功能是如何实现的：
```
//音乐锁屏信息展示
- (void)setupLockScreenInfo
{
    // 1.获取锁屏中心
    MPNowPlayingInfoCenter *playingInfoCenter = [MPNowPlayingInfoCenter defaultCenter];
   //初始化一个存放音乐信息的字典
    NSMutableDictionary *playingInfoDict = [NSMutableDictionary dictionary];
    // 2、设置歌曲名
    if (self.currentModel.name) {
        [playingInfoDict setObject:self.currentModel.name forKey:MPMediaItemPropertyAlbumTitle];
    }
    // 设置歌手名
    if (self.currentModel.artist) {
        [playingInfoDict setObject:self.currentModel.artist forKey:MPMediaItemPropertyArtist];
    }
    // 3设置封面的图片
    UIImage *image = [self getMusicImageWithMusicId:self.currentModel];
    if (image) {
        MPMediaItemArtwork *artwork = [[MPMediaItemArtwork alloc] initWithImage:image];
        [playingInfoDict setObject:artwork forKey:MPMediaItemPropertyArtwork];
    }
    // 4设置歌曲的总时长
    [playingInfoDict setObject:self.currentModel.detailDuration forKey:MPMediaItemPropertyPlaybackDuration];
    //音乐信息赋值给获取锁屏中心的nowPlayingInfo属性
    playingInfoCenter.nowPlayingInfo = playingInfoDict;
    // 5.开启远程交互，只有开启这个才能进行远程操控
    [[UIApplication sharedApplication] beginReceivingRemoteControlEvents];
}
```
这里设置图片时需要注意下，异步加载网络图片后再设置是无效的，所以图片信息最好是先请求下来后再进行设置。
远程超控的回调如下：
```
//监听远程交互方法
- (void)remoteControlReceivedWithEvent:(UIEvent *)event
{
    switch (event.subtype) {
        //播放
        case UIEventSubtypeRemoteControlPlay:{
            [self.player play];
                    }
            break;
        //停止
        case UIEventSubtypeRemoteControlPause:{
            [self.player pause];
                   }
            break;
        //下一首
        case UIEventSubtypeRemoteControlNextTrack:
            [self nextBtnAction:nil];
            break;
        //上一首
        case UIEventSubtypeRemoteControlPreviousTrack:
            [self lastBtnAction:nil];
            break;
        default:
            break;
    }
}
```
**三、总结**
最后，画了一张图总结下播放远程网络音乐的流程：
![](http://upload-images.jianshu.io/upload_images/2756501-a1b1a700cfe0d02b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
远程音乐播放流程.png
根据QQ音乐的界面做了个小demo，下面是demo的真机前台和后台播放的运行效果：
![](http://upload-images.jianshu.io/upload_images/2756501-8ffbf6db2094ba13.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
前台播放
![](http://upload-images.jianshu.io/upload_images/2756501-5950833ed28f43e6.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
后台播放
附上github下载地址:[https://github.com/yedexiong/MsuicPlayDemo.git](https://github.com/yedexiong/MsuicPlayDemo.git)
**四、结束语**
播放远程网络音乐的核心技术点基本上已经写完，当然AVPlayer还有很多强大的功能没有写出来，有兴趣的可以进一步挖掘。写到这里已经疲倦至极,如果喜欢的可以点赞和关注，后续会持续更新一些精彩的技术点。
文／夜_阑珊（简书作者）
原文链接：http://www.jianshu.com/p/31644a7f581c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
