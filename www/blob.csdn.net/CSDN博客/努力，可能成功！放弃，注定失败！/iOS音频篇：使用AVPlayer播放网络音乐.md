# iOS音频篇：使用AVPlayer播放网络音乐 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月24日 09:33:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9045
### 引言
假如你现在打算做一个类似百度音乐、豆瓣电台的在线音乐类APP，你会怎样做？
首先了解一下音频播放的实现级别：
(1) 离线播放：这里并不是指应用不联网，而是指播放本地音频文件，包括先下完完成音频文件再进行播放的情况，这种使用AVFoundation里的AVAudioPlayer可以满足
(2) 在线播放：使用AVFoundation的AVPlayer可以满足
(3) 在线播放同时存储文件：使用
AudioFileStreamer ＋ AudioQueue 可以满足
(4) 在线播放且带有音效处理：使用
AudioFileStreamer ＋ AudioQueue ＋ 音效模块（系统自带或者
自行开发）来满足
本文主要针对第二种级别，介绍如何使用AVPlayer实现网络音乐的播放。
### 什么是AVPlayer
AVPlayer存在于AVFoundation中，其实它是一个视频播放器，但是用它来播放音乐是没问题的，当然播放音乐不需要呈现界面，因此我们不需要实现它的界面。
跟AVPlayer联系密切的名词：
Asset：AVAsset是抽象类，不能直接使用，其子类AVURLAsset可以根据URL生成包含媒体信息的Asset对象。
AVPlayerItem：和媒体资源存在对应关系，管理媒体资源的信息和状态。
### 功能需求
通常音乐播放并展示到界面上需要我们实现的功能如下：
1、（核心）播放器通过一个网络链接播放音乐
2、（基本）播放器的常用操作：暂停、播放、上一首、下一首等等
3、（基本）监听该音乐的播放进度、获取音乐的总时间、当前播放时间
4、（基本）监听改播放器状态：
     (1）媒体加载状态
     (2）数据缓冲状态
     (3）播放完毕状态
5、（可选）Remote Control控制音乐的播放
6、（可选）Now Playing Center展示正在播放的音乐
### 功能实现
#### 1、通过一个网络链接播放音乐
```
NSURL * url  = [NSURL URLWithString:self.currentSong.url];
AVPlayerItem * songItem = [[AVPlayerItem alloc]initWithURL:url];
AVPlayer * player = [[AVPlayer alloc]initWithPlayerItem:songItem];
```
这里是用一个asset来初始化player，当然你也可以直接用URL初始化：
`AVPlayer * player = [[AVPlayer alloc] initWithURL:url];`
需要获取当前播放的item可以这样获取：
`AVPlayerItem * songItem = player.currentItem;`
#### 2、播放器的常用操作
播放：
`[player play];`
需要注意的是初始化完player之后不一定会马上开始播放，需要等待player的状态变为ReadyToPlay才会进行播放。
暂停：
`[player pause];`
上一首、下一首：
这里我们有两种方式可以实现，一种是由你自行控制下一首歌曲的item，将其替换到当前播放的item
`[player replaceCurrentItemWithPlayerItem:songItem];`
另一种是使用AVPlayer的子类AVQueuePlayer来播放多个item，调用advanceToNextItem来播放下一首音乐
```
NSArray * items = @[item1, item2, item3 ....];
AVQueuePlayer * queuePlayer = [[AVQueuePlayer alloc]initWithItems:items];
```
#### 3、监听播放进度
使用`addPeriodicTimeObserverForInterval:queue:usingBlock:`来监听播放器的进度
(1）方法传入一个CMTime结构体，每到一定时间都会回调一次，包括开始和结束播放
(2）如果block里面的操作耗时太长，下次不一定会收到回调，所以尽量减少block的操作耗时
(3）方法会返回一个观察者对象，当播放完毕时需要移除这个观察者
添加观察者：
```
id timeObserve = [self.player addPeriodicTimeObserverForInterval:CMTimeMake(1.0, 1.0) queue:dispatch_get_main_queue() usingBlock:^(CMTime time) {
        float current = CMTimeGetSeconds(time);        
        float total = CMTimeGetSeconds(songItem.duration);        
        if (current) {            
              weakSelf.progress = current / total;            
              weakSelf.playTime = [NSString stringWithFormat:@"%.f",current];            
              weakSelf.playDuration = [NSString stringWithFormat:@"%.2f",total];        }
    }];
```
移除观察者：
```
if (timeObserve) {
        [player removeTimeObserver:_timeObserve];
        timeObserve = nil;
    }
```
#### 4、监听改播放器状态
(1) 媒体加载状态
`[songItem addObserver:self forKeyPath:@"status" options:NSKeyValueObservingOptionNew context:nil];`
然后可以在KVO方法中获取其status的改变
```
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {
    if ([keyPath isEqualToString:@"status"]) {
        switch (self.player.status) {            
            case AVPlayerStatusUnknown:                
                BASE_INFO_FUN(@"KVO：未知状态，此时不能播放");                
                break;            
            case AVPlayerStatusReadyToPlay:                
                self.status = SUPlayStatusReadyToPlay;                    
                BASE_INFO_FUN(@"KVO：准备完毕，可以播放");                
                break;
            case AVPlayerStatusFailed:
                BASE_INFO_FUN(@"KVO：加载失败，网络或者服务器出现问题");
                break;            
            default:                
                break;        
        }
    }
}
```
一般初始化player到播放都会经历
Unknown到ReadyToPlay这个过程，网络情况良好时可能不会出现Unknown状态的提示，网络情况差的时候Unknown的状态可能会持续比较久甚至可能不进入ReadyToPlay状态，针对这种情况我们要做特殊的处理。
播放完成之后需要移除观察者：
`[songItem removeObserver:self forKeyPath:@"status"];`
(2) 数据缓冲状态
`[songItem addObserver:self forKeyPath:@"loadedTimeRanges" options:NSKeyValueObservingOptionNew context:nil];`
然后可以在KVO方法中获取其status的改变
```
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {   
    AVPlayerItem * songItem = object;
    if ([keyPath isEqualToString:@"loadedTimeRanges"]) {
        NSArray * array = songItem.loadedTimeRanges;        
        CMTimeRange timeRange = [array.firstObject CMTimeRangeValue]; //本次缓冲的时间范围
        NSTimeInterval totalBuffer = CMTimeGetSeconds(timeRange.start) + CMTimeGetSeconds(timeRange.duration); //缓冲总长度
        SuLog(@"共缓冲%.2f",totalBuffer);
    }
}
```
如果你需要在进度条展示缓冲的进度，可以增加这个观察者。
播放完成之后需要移除观察者：
`[songItem removeObserver:self forKeyPath:@" loadedTimeRanges"];`
(3) 播放完毕状态
监听AVPlayer播放完成通知
``[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playbackFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:songItem];``
```
```
- (void)playbackFinished:(NSNotification *)notice {    
    BASE_INFO_FUN(@"播放完成");    
    [self playNext];
}
```
```
播放完毕后，一般都会进行播放下一首的操作。
播放下一首前，别忘了移除这个item的观察者：
``[[NSNotificationCenter defaultCenter] removeObserver:self];``
#### 5、Remote Control控制音乐的播放
Remote Control可以让你在不打开APP的情况下控制其播放，最常见的出现于锁屏界面、从屏幕底部上拉和耳机线控三种，可以达到增强用户体验的作用。
我们在AppDelegate里去设置Remote Control：
（1）声明接收Remote Control事件
`[[UIApplication sharedApplication] beginReceivingRemoteControlEvents];`
（2）重写方法，成为第一响应者
```
- (BOOL)canBecomeFirstResponder {    
    return YES;
}
```
（3）对事件进行处理
```
```
- (void)remoteControlReceivedWithEvent:(UIEvent *)event {       
    switch (event.subtype)    {        
        case UIEventSubtypeRemoteControlPlay:
            [self.player startPlay];
            BASE_INFO_FUN(@“remote_播放");
            break;        
        case UIEventSubtypeRemoteControlPause:            
            [self.player pausePlay];
            BASE_INFO_FUN(@"remote_暂停");
            break;        
        case UIEventSubtypeRemoteControlNextTrack:
            [self.player playNextSong];
            BASE_INFO_FUN(@"remote_下一首");
            break;        
        case UIEventSubtypeRemoteControlTogglePlayPause:            
            self.player.isPlaying ? [self.player pausePlay] : [self.player startPlay];           
            BASE_INFO_FUN(@“remote_耳机的播放/暂停");
            break;        
        default:            
            break;    }
}
```
```
#### 6、Now Playing Center
Now Playing Center可以在锁屏界面展示音乐的信息，也达到增强用户体验的作用。
```
- (void)configNowPlayingCenter {    BASE_INFO_FUN(@"配置NowPlayingCenter");
    NSMutableDictionary * info = [NSMutableDictionary dictionary];
    //音乐的标题
    [info setObject:_player.currentSong.title forKey:MPMediaItemPropertyTitle];
     //音乐的艺术家
    [info setObject:_player.currentSong.artist forKey:MPMediaItemPropertyArtist];
     //音乐的播放时间
    [info setObject:@(self.player.playTime.intValue) forKey:MPNowPlayingInfoPropertyElapsedPlaybackTime];
     //音乐的播放速度
    [info setObject:@(1) forKey:MPNowPlayingInfoPropertyPlaybackRate];
     //音乐的总时间
    [info setObject:@(self.player.playDuration.intValue) forKey:MPMediaItemPropertyPlaybackDuration];
     //音乐的封面
    MPMediaItemArtwork * artwork = [[MPMediaItemArtwork alloc] initWithImage:_player.coverImg];
    [info setObject:artwork forKey:MPMediaItemPropertyArtwork];
     //完成设置
    [[MPNowPlayingInfoCenter defaultCenter]setNowPlayingInfo:info];
}
```
Now Playing Center并不需要每一秒都去刷新（设置），它是根据你设置的PlaybackRate来计算进度条展示的进度，比如你PlaybackRate传1，那就是1秒刷新一次进度显示，当然暂停播放的时候它也会自动暂停。
那什么时候设置Now Playing Center比较合适呢？对于播放网络音乐来说，需要刷新的有几个时间点：当前播放的歌曲变化时（如切换到下一首）、当前歌曲信息变化时（如从Unknown到ReadyToPlay）、当前歌曲拖动进度时。
如果有读者是使用百度音乐听歌的话，会发现其带有锁屏歌词，其实它是采用“将歌词和封面合成新的图片设置为Now Playing Center的封面 ＋ 歌词跃进时刷新Now Playing Center”来实现的，有兴趣的筒子可以研究一下。
### 关于总体的播放逻辑
总结一下音乐播放器的播放逻辑：
（1) 初始化播放界面
（2）从接口获取播放列表、选择第一首为当前播放歌曲
（3）根据当前歌曲初始化播放器 、同步歌曲信息到播放界面（此时播放界面应展示歌曲信息，但是播放按钮应不可用且有loading之类的提示表示正在加载歌曲）、同步歌曲信息到Now Playing Center
（4）当播放器的status变为ReadyToPlay时，播放歌曲、同步播放信息到播放界面（播放时间、总时间、进度条等等）、同步播放信息到Now Playing Center
（5）当用户进行暂停操作时，刷新播放界面
（6）当用户进行下一首、上一首操作时，或完成某一首歌曲的播放时，将对应的歌曲设置为当前播放歌曲，重复3-5步骤
（7）由于网络情况不好造成播放器自动暂停播放时，应刷新播放界面
（8）由于网络情况不好造成播放器不能进入播放状态时，应有所处理（比如提示耐心等待或者播放本地离线的歌曲）
### 后记
本文仅以实现基本功能的角度介绍了AVPlayer来播放网络音乐的实现，事实上AVPlayer的功能不仅于此，有兴趣的同学可以深入学习AVFoundation。
如有读者需要类似豆瓣电台APP的项目demo可以到[明仔Su的GitHub]()下载学习。
