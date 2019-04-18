# AVPlayer视频播放之 - AVPlayerItem - weixin_33985507的博客 - CSDN博客
2017年11月28日 16:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
 AVPlayerItem：该类主要是用于管理资源对象，提供播放数据源，旨在表示由AVPlayer播放的资产的呈现状态，并允许观察该状态，它控制着视频从创建到销毁的诸多状态。
- 
`+ (instancetype)playerItemWithURL:(NSURL *)URL / - (instancetype)initWithURL:(NSURL *)URL`根据URL初始化对象;
- 
`+ (instancetype)playerItemWithAsset:(AVAsset *)asset / - (instancetype)initWithAsset:(AVAsset *)asset`根据Asset初始化对象，相当于`+ playerItemWithAsset：automatedLoadedAssetKeys :`,传递@ [@“duration”]作为自动调用的关键字值;
- 
`+ (instancetype)playerItemWithAsset:(AVAsset *)asset automaticallyLoadedAssetKeys:(nullable NSArray<NSString *> *)automaticallyLoadedAssetKeys / - (instancetype)initWithAsset:(AVAsset *)asset automaticallyLoadedAssetKeys:(nullable NSArray<NSString *> *)automaticallyLoadedAssetKeys` 参数`automaticallyLoadedAssetKeys` 是NSStrings的NSArray，每个NSString表示由AVAsset定义的属性键,例如`duration`、 `preferredRate`等;
- 
`@property (nonatomic, readonly) AVPlayerItemStatus status;` 这个属性的值是一个AVPlayerItemStatus，它指示接收器是否可以用于播放，一般为可以播放。最重要的需要观察的属性！！当你第一次创建AVPlayerItem时，其状态值为AVPlayerItemStatusUnknown，表示其媒体尚未加载，尚未排入队列进行播放。将AVPlayerItem与 AVPlayer相关联后会立即开始排列该项目的媒体并准备播放，但是在准备好使用之前，需要等到其状态变为AVPlayerItemStatusReadyToPlay;
```
typedef NS_ENUM(NSInteger, AVPlayerItemStatus) {
    AVPlayerItemStatusUnknown,  //表示播放器项目的状态尚未知道，因为它尚未尝试加载新媒体资源进行播放
    AVPlayerItemStatusReadyToPlay, //表示播放器项目已准备好播放
    AVPlayerItemStatusFailed //表示AVPlayerItem由于错误而不能再播放
};
```
- 
`@property (nonatomic, readonly, nullable) NSError *error;`如果接收者的状态是AVPlayerItemStatusFailed，则描述导致失败的错误，否则为nil;
- 
`@property (nonatomic, readonly) AVAsset *asset;`获取初始化期间提供的资产;
- 
`@property (nonatomic, readonly) NSArray<AVPlayerItemTrack *> *tracks;`提供AVPlayerItem音轨数组。 可观察（可在播放期间动态更改）;
- 
`@property (nonatomic, readonly) CMTime duration ;`媒体的持续时间，可观察（可在播放期间动态更改）。直到asset被加载完成前，此属性的值将返回为kCMTimeIndefinite（使用这个常量初始化一个不确定的CMTime ---- 如一个实况广播的持续时间，不要使用[time == kCMTimeIndefinite]对此进行测试）;
 有两种方法使可以确保该值只有在可用时才能被访问：
- 
等待AVPlayerItem的`status`属性值为AVPlayerItemStatusReadyToPlay后使用。
- 
注册属性的键值观察，请求初始值。 如果初始值为kCMTimeIndefinite，则AVPlayerItem一旦知道它的值就会通过键值观察通知你其持续时间的可用性。
> 
关于AVAsset的duration和AVPlayerItem的duration的区别请参考 : [官方文档描述](https://link.jianshu.com?t=https://developer.apple.com/library/content/releasenotes/AudioVideo/RN-AVFoundation-Old/#//apple_ref/doc/uid/TP40011199-CH1-SW4)
- 
`@property (nonatomic, readonly) CGSize presentationSize;`播放器提供的接收器的大小，可观察，该属性可以在任何时候访问，但是在asset已经AVPlayerItemStatusReadyToPlay前会是CGSizeZero;
- 
`@property (nonatomic, readonly, nullable) NSArray<AVMetadataItem *> *timedMetadata;`提供一个AVMetadataItems的NSArray，表示媒体播放时最近遇到的定时元数据，可能是nil;
- 
`@property (nonatomic, readonly) NSArray<NSString *> *automaticallyLoadedAssetKeys`获取在AVPlayerItem到达AVPlayerItemStatusReadyToPlay状态前自动加载AVAsset的keys;
#### 播放信息相关⏬
- 
`@property (nonatomic, readonly) BOOL canPlayFastForward` 7.0前表示播放速度是否可以在1.0-2.0之间播放。7.0后表示播放速度是否可以大于2.0，即使该属性为NO，播放速度也可以在1.0-2.0之间播放;
- 
`@property (nonatomic, readonly) BOOL canPlaySlowForward`是否可以再0.0-1.0之间播放;
- 
`@property (nonatomic, readonly) BOOL canPlayReverse`是否可以以-1.0的速度播放;
- 
`@property (nonatomic, readonly) BOOL canPlaySlowReverse`是否可以在-1.0 - 0.0之间的速度播放;
- 
`@property (nonatomic, readonly) BOOL canPlayFastReverse`是否可以低于-1.0的速度播放;
- 
`@property (nonatomic, readonly) BOOL canStepForward`是否支持快进，一旦AVPlayerItem的status变为AVPlayerItemStatusReadyToPlay，该值将不会再变;
- 
`@property (nonatomic, readonly) BOOL canStepBackward`是否支持快退，一旦AVPlayerItem的status变为AVPlayerItemStatusReadyToPlay，该值将不会再变;
- 
`@property (nonatomic, readonly) NSArray<NSValue *> *loadedTimeRanges`已加载Item的时间范围;
- 
`@property (nonatomic, readonly, getter=isPlaybackLikelyToKeepUp) BOOL playbackLikelyToKeepUp`指示该item是否能无延迟播放，用于监听缓存足够播放的状态，在这里，当属性`playbackBufferFull`指示YES时，可能是`playbackLikelyToKeepUp`指示NO。 在这种情况下，播放缓存已经达到了容量，但是没有统计数据来支持，所以播放可能持续，所以这里需要程序员决定是否继续媒体播放;
- 
`@property (nonatomic, readonly, getter=isPlaybackBufferFull) BOOL playbackBufferFull`缓存区是否已经满了，并且进一步的I / O是否被挂起;
- 
`@property (nonatomic, readonly, getter=isPlaybackBufferEmpty) BOOL playbackBufferEmpty`指示播放是否消耗了所有缓冲媒体，播放将停止或结束;
- 
`@property (nonatomic, assign) BOOL canUseNetworkResourcesForLiveStreamingWhilePausedNS_AVAILABLE(10_11, 9_0)` 指示播放器项目是否可以使用网络资源在暂停时使播放状态保持最新状态，对于直播内容，PlayerItem可能需要使用额外的网络和电源，以使播放状态在暂停时保持最新状态。 例如，当此属性设置为YES时，seekableTimeRanges属性将定期更新以反映实时流的当前状态，iOS 9后默认为NO;
- 
`@property (nonatomic) NSTimeInterval preferredForwardBufferDuration NS_AVAILABLE(10_12, 10_0)`指示播放器在播放头之前缓冲媒体的持续时间，以防止播放中断。该属性定义了首选的前向缓冲区持续时间（秒）。如果设置为0，播放器将为大多数使用情况选择适当的缓冲级别。将此属性设置为较低值会增加播放停顿和重新缓冲的机会，而将其设置为较高值会增加对系统资源的需求;
- 
`@property (nonatomic) double preferredPeakBitRate NS_AVAILABLE(10_10, 8_0)`此item的网络带宽消耗限制（以每秒位数为单位）;
- 
`@property (nonatomic) CGSize preferredMaximumResolution NS_AVAILABLE(10_13, 11_0)`指示播放器下载和呈现的视频的分辨率的首选上限。
默认值为CGSizeZero，表示客户端对视频分辨率没有限制。 其他值表示首选的最大视频分辨率。仅适用于HTTP Live Streaming asset;
#### 时间控制⏬
- 
`- (CMTime)currentTime;`返回AVPlayerItem的当前时间;
- 
`@property (nonatomic) CMTime forwardPlaybackEndTime`指定播放速率为正值时播放结束的时间，默认值为kCMTimeInvalid，表示没有指定前向播放的结束时间。在这种情况下，正向播放的有效结束时间是持续时间。到达结束时间时，接收者将发布`AVPlayerItemDidPlayToEndTimeNotification`通知,速率为负时，此属性的值对播放没有影响;
- 
`@property (nonatomic) CMTime reversePlaybackEndTime`指定播放速度为负数时播放结束的时间。默认值是kCMTimeInvalid，表示没有指定反向播放的结束时间。
在这种情况下，反向播放的有效结束时间是kCMTimeZero。到达结束时间时，接收者将发布`AVPlayerItemDidPlayToEndTimeNotification`通知,速率为正时，此属性的值对播放没有影响;
- 
`@property (nonatomic, readonly) NSArray<NSValue *> *seekableTimeRanges`该属性提供一个AVPlayerItem能找到的时间范围集合，其中元素是CMTimeRange结构体中的NSValue
对象，返回的时间范围可能是不连续的;
- 
`- (void)seekToTime:(CMTime)time completionHandler:(void (^_Nullable)(BOOL finished))completionHandler`使用此方法可以查找项目的指定时间，并在搜索操作已完成或中断时执行指定的块。如果seekTime在seekableTimeRanges属性指示的可寻址时间范围之外，那么查找请求将被取消，完成处理程序将被调用，完成的参数设置为NO，如果查找请求完成而不被中断，则提供的完成处理程序将使用finished参数设置为YES。如果在调用此方法时另一个查找请求已在进行中，则正在进行的查找请求立即调用，并将finished参数设置为NO;
- 
`- (void)seekToTime:(CMTime)time toleranceBefore:(CMTime)toleranceBefore toleranceAfter:(CMTime)toleranceAfter completionHandler:(void (^_Nullable)(BOOL finished))completionHandler`查找的时间将在[`time` -`toleranceBefore`，`time`+ `toleranceAfter`]，将`kCMTimeZero`传递给toleranceBefore和toleranceAfter以请求采样精确查找，这可能会导致额外的解码延迟。全部传递`kCMTimePositiveInfinity`，则与`seekToTime：`方法相同;
- 
`- (void)cancelPendingSeeks`使用此方法取消并释放挂起的搜索的完成处理程序。 完成处理程序的完成参数将被设置为NO;
- 
`- (nullable NSDate *)currentDate`如果currentTime映射到特定的（实时）日期存在，则返回当前播放的日期；如果播放未映射到任何日期，则返回nil;
- 
`- (BOOL)seekToDate:(NSDate *)date completionHandler:(void (^_Nullable)(BOOL finished))completionHandler`与seekToTime相似;
- 
`- (void)stepByCount:(NSInteger)stepCount`将AVPlayerItem的currentTime向前或向后移动指定的步数，正数前进，负数后退。 每个步数的大小取决于AVPlayerItem启用的AVPlayerItemTracks对象;
#### 视频视觉效果及音频设置相关⏬
- 
`@property (nonatomic, copy, nullable) AVVideoComposition *videoComposition`播放期间应用的视频合成设置;
- 
`@property (nonatomic, readonly, nullable) id<AVVideoCompositing> customVideoCompositor`获取自定义的视频合成器，如果没有视频合成器，或者内部视频合成器正在使用，则此属性为nil;
- 
`@property (nonatomic) BOOL seekingWaitsForVideoCompositionRendering`指示在搜索视频作品时，AVPlayerItem的时间是否跟随显示的视频帧。一般设置为NO，此属性对`videoComposition`为nil的项目没有影响;
- 
`@property (nonatomic, copy, nullable) NSArray<AVTextStyleRule *> *textStyleRules`代表文本样式的AVTextStyleRules数组，可应用于字幕和其他易读的媒体，数组中的每个AVTextStyleRule对象中包含的样式信息仅在正在播放的媒体资源没有指定文本样式时使用，此属性仅对媒体子类型为kCMSubtitleFormatType_WebVTT的音轨有效;
- 
`@property (nonatomic, copy) AVAudioTimePitchAlgorithm audioTimePitchAlgorithm`用于管理缩放音频编辑音频音调的处理算法，系统给出四种:
- 
`AVF_EXPORT AVAudioTimePitchAlgorithm const AVAudioTimePitchAlgorithmLowQualityZeroLatency`iOS下默认方式，音频质量低，适合短暂的快进/倒带效果。 速率捕捉到{0.5,0.6666667,0.8,1.0,1.25,1.5,2.0}这六个数值;
- 
`AVF_EXPORT AVAudioTimePitchAlgorithm const AVAudioTimePitchAlgorithmTimeDomain`音频质量适中，适合声音。 可变率从1/32到32;
- 
`AVF_EXPORT AVAudioTimePitchAlgorithm const AVAudioTimePitchAlgorithmVarispeed`音频质量高，没有音调校正，音调随速率而变化。 可变率从1/32到32;
- 
`AVF_EXPORT AVAudioTimePitchAlgorithm const AVAudioTimePitchAlgorithmSpectral`OS X下默认方式，音频质量最高，计算成本最高，适合音乐。 可变率从1/32到32;
- 
`@property (nonatomic, copy, nullable) AVAudioMix *audioMix`播放期间要应用的音频混合参数，音频混合只能用于基于文件的媒体（本地文件），不支持与使用HTTP实时流传输的媒体一起使用，VAudioMix的inputParameters属性中必须具有对应于接收者assetTrack的trackID，否则将被忽略;
#### 媒体选择相关⏬
- 
`- (void)selectMediaOption:(nullable AVMediaSelectionOption *)mediaSelectionOption inMediaSelectionGroup:(AVMediaSelectionGroup *)mediaSelectionGroup`选择指定AVMediaSelectionGroup中指定的AVMediaSelectionOption实例所描述的媒体选项，并取消选择该组中的所有其他选项，如果mediaSelectionOption不是mediaSelectionGroup成员，则不会改变呈现状态，如果AVMediaSelectionGroup的`allowsEmptySelection`属性的值为YES，则可以将nil传递给`mediaSelectionOption`参数以取消选择组中的所有媒体选择选项;
- 
`- (void)selectMediaOptionAutomaticallyInMediaSelectionGroup:(AVMediaSelectionGroup *)mediaSelectionGroup`选择与AVPlayer的自动选择条件最匹配的指定媒体选择组中的媒体选项。除非相关AVPlayer的applyMediaSelectionCriteriaAutomatically属性为YES且已经复写 - [AVPlayerItem selectMediaOption：inMediaSelectionGroup：]方法，否则该方法不起作用;
- 
`@property (nonatomic, readonly) AVMediaSelection *currentMediaSelection NS_AVAILABLE(10_11, 9_0)`获取当前媒体组的媒体选择;
#### 日志相关⏬
- 
`- (nullable AVPlayerItemAccessLog *)accessLog`返回表示网络访问日志快照的对象，可以为nil。新的日志记录信息可用时，将发布`AVPlayerItemNewAccessLogEntryNotification`通知 ;
- 
`- (nullable AVPlayerItemErrorLog *)errorLog`返回表示错误日志快照的对象，可以为nil;
#### 输出相关⏬
- 
`- (void)addOutput:(AVPlayerItemOutput *)output`将指定的AVPlayerItemOutput实例添加到接收方的输出集合中，提供的AVPlayerItemOutput类决定了解码样本的数据结构;
- 
`- (void)removeOutput:(AVPlayerItemOutput *)output`移除指定的的播放器项目输出对象;
- 
`@property (nonatomic, readonly) NSArray<AVPlayerItemOutput *> *outputs`获取与AVPlayerItem相关的输出对象;
#### 输出相关（9.3以后可用）⏬
- 
`- (void)addMediaDataCollector:(AVPlayerItemMediaDataCollector *)collector`将指定的AVPlayerItemMediaDataCollector实例添加到接收者的mediaDataCollectors集合中，这可能会导致额外的I / O异步收集请求的媒体数据;
- 
`- (void)removeMediaDataCollector:(AVPlayerItemMediaDataCollector *)collector`移除指定的AVPlayerItemMediaDataCollector实例;
- 
`@property (nonatomic, readonly) NSArray<AVPlayerItemMediaDataCollector *> *mediaDataCollectors`获取相关联的所有AVPlayerItemMediaDataCollector实例;
#### 以下是涉及的通知：
- 
`AVF_EXPORT NSString *const AVPlayerItemTimeJumpedNotification`AVPlayerItem的`currentTime`发生变化时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemDidPlayToEndTimeNotification`AVPlayerItem播放到结束时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemFailedToPlayToEndTimeNotification`当该物品未能播放到其结束时间就中断时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemPlaybackStalledNotification`暂停时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemNewAccessLogEntryNotification`在新的访问日志被添加条目时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemNewErrorLogEntryNotification`在新的错误日志被添加条目时发布;
- 
`AVF_EXPORT NSString *const AVPlayerItemFailedToPlayToEndTimeErrorKey`该键是从`AVPlayerItemTimeJumpedNotification`通知的用户信息字典中检索错误对象（NSError）的键;
**该类对于视频播放的各种状态监控特别重要，除了按上面的通知外还有几个重要的属性需要使用KVO去进行处理！！！**
