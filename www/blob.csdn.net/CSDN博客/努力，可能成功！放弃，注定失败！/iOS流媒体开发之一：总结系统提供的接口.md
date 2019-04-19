# iOS流媒体开发之一：总结系统提供的接口 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月12日 23:01:23[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2572
尊重知识，转发请注明出处：[iOS流媒体开发之一：总结系统提供的接口](http://www.jianshu.com/p/6c6b59a875c1)
本文参考了博文部分内容：[AVPlayer 本地、网络视频播放相关](http://www.jianshu.com/p/de418c21d33c)
最近独立完成了公司的一个电视直播和电台直播的流媒体类项目，目前完成了第一版，已经上线，开发过程中收获很多，准备写一个流媒体系列博客总结和分享我的一些感悟。第一篇很简单，主要是总结下系统提供的常用的音视频开发接口，属于可以百度或者Google到一大堆资料的东西，几个不支持流媒体播放的接口就不说了，一方面很简单，另一方面大多数项目用不到。了解这些技术的小伙伴就忽略吧，我这里只是简单的总结下，没有太多技术含量和价值。
# 预告
后续会分享有关**自定义视频播放器**、**M3U8下载**、**M3U8回看**等比较有难度的技术点，当然也不是随便百度或者Google下就可以找到的东西，我就把这些技术点献给简书吧。这里主要总结M3U8直播类的音视频技术，至于MP4、3GP等等这些点播类的播放源都很简单，无论下载还是播放都很容易实现，就不浪费简书的服务器硬盘了。最近时间很紧，并且有一些技术点还需要继续改进和优化，争取尽快整理出比较靠谱的东西给大家。
# MPMoviePlayerController
- 
简介
MPMoviePlayerController既支持本地音视频播放也支持网络流媒体播放，功能已经十分完善了，流媒体项目常用的需求都可以满足，比如播放、暂停，快进、后退、监听播放器的播放状态、截图等功能，同时MPMoviePlayerController提供了一个简单的全屏播放界面，可以轻松实现简单的流媒体播放需求，如果需要深度自定义一个视频播放器，可以将MPMoviePlayerController的view添加到自定义的控制器中，然后再讲一些自定义的控件添加到相应view即可，代码如下：
```
MPMoviePlayerController *player =  [[MPMoviePlayerController alloc] initWithContentURL: myURL];
  [player prepareToPlay];
  [player.view setFrame: myView.bounds];
  [myView addSubview: player.view];
  [player play];
```
- 
总结
MPMoviePlayerController为音视频开发提供了简洁易用的接口，小伙伴们自行查阅文档即可，这里就不赘述了。需要注意的是在iOS9 以后MPMoviePlayerController被苹果弃用了，取而代之的是AVPlayerViewController，究其原因，iOS9以后iPad支持了画中画功能，即使用画中画播放后，我们退出APP，视频会在一个小的窗口继续播放，用户可以在使用其他APP的同时观看视频，AVPlayerViewController提供了完整的画中画技术支持，具体后面再讲。画中画如下图：
![](http://upload-images.jianshu.io/upload_images/1347742-c6a924eb956d3798.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
画中画.png
# AVPlayer
- 
简介
如果我们只是开发只有音频的流媒体项目，MPMoviePlayerController足够用了，可以满足几乎任何需求，开发一个简单的视频播放器也足够用。但是要深度自定义一个视频播放器 使用AVPlayer就很方便了， AVPlayer自由度很高，可以自定义视频播放器, AVPlayer本身无法显示视频，需要借助AVPlayerLayer显示，初始化以及播放过程代码如下：
```
#import <AVFoundation/AVFoundation.h>
  @interface ViewController ()
  @property (strong, nonatomic) AVPlayer *avPlayer;
  @end
  @implementation ViewController
  - (void)viewDidLoad {
      [super viewDidLoad];
  //这个链接是M3U8的，你看到此博客的时候此链接可能已经失效，请自行找链接测试
 AVPlayerItem *playItem = [[AVPlayerItem alloc] initWithURL:[NSURL URLWithString:@"http://cctv1.vtime.cntv.cloudcdn.net/cache/12_/seg0/index.m3u8?AUTH=KDumCPYYPzSTcmtewPt/u78MdD6mwSpceXl98vdwcN8RIWA7hZqDK8s3RWkdW3PymV7TkVLHQ5UJp1gHXtkWGg=="]];
  //初始化AVPlayer
  self.avPlayer = [[AVPlayer alloc] initWithPlayerItem:playItem];
  //设置AVPlayer关联
  AVPlayerLayer *playerLayer = [AVPlayerLayer playerLayerWithPlayer:self.avPlayer];
  //设置视频模式
  playerLayer.videoGravity = AVLayerVideoGravityResize;
  playerLayer.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.width * 9.0 / 16.0);
  //创建一个UIView与AVPlayerLayer关联
  UIView *playerView = [[UIView alloc] initWithFrame:CGRectMake(0, 20, CGRectGetWidth(playerLayer.frame), CGRectGetHeight(playerLayer.frame))];
  playerView.backgroundColor = [UIColor blackColor];
  [playerView.layer addSublayer:playerLayer]; 
  [self.view addSubview:playerView];
  //开始播放(请在真机上运行)
  [self.avPlayer play];
  }
```
真机运行图如下：
![](http://upload-images.jianshu.io/upload_images/1347742-5a8683d1148245bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
AVPlayer播放M3U8效果.png
- 
总结
上面我们已经可以把展示我们视频的view随意处置了，可以为视频播放增加常用的改变视频比例、手势改变音量、亮度等常用的功能。
**注意**
*1、获取AVPlayer的缓冲进度*
通过监听loadedTimeRanges、status可以获取缓冲进度和播放进度
代码如下：
首先添加监听
```
[playItem addObserver:self forKeyPath:@"loadedTimeRanges" options:NSKeyValueObservingOptionNew context:nil];
  [playItem addObserver:self forKeyPath:@"status" options:NSKeyValueObservingOptionNew context:nil];
```
监听回调
```
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {
  AVPlayerItem *playerItem = (AVPlayerItem *)object;
  if ([keyPath isEqualToString:@"loadedTimeRanges"]){
  //获取缓冲进度
  NSArray *loadedTimeRanges = [playerItem loadedTimeRanges];
  // 获取缓冲区域
  CMTimeRange timeRange = [loadedTimeRanges.firstObject CMTimeRangeValue];
  //开始的时间
  NSTimeInterval startSeconds = CMTimeGetSeconds(timeRange.start);
  //表示已经缓冲的时间
  NSTimeInterval durationSeconds = CMTimeGetSeconds(timeRange.duration);
  // 计算缓冲总时间
  NSTimeInterval result = startSeconds + durationSeconds;
  NSLog(@"开始:%f,持续:%f,总时间:%f", startSeconds, durationSeconds, result);
  NSLog(@"视频的加载进度是:%%%f", durationSeconds / self.total * 100);
      }else if ([keyPath isEqualToString:@"status"]){
  //获取播放状态
  if (playerItem.status == AVPlayerItemStatusReadyToPlay){
      NSLog(@"准备播放");
      //获取视频的总播放时长
      [self.avPlayer play];
      self.total = CMTimeGetSeconds(self.avPlayer.currentItem.duration);
          } else{
      NSLog(@"播放失败");
      }
    }
   }
```
控制台输出结果
`开始:0.000000,持续:0.325000,总时间:0.325000视频的加载进度是:%inf准备播放开始:0.000000,持续:18.203000,总时间:18.203000视频的加载进度是:%12.227365开始:0.000000,持续:35.108000,总时间:35.108000视频的加载进度是:%23.582833开始:0.000000,持续:43.119000,总时间:43.119000视频的加载进度是:%28.964002开始:0.000000,持续:46.068000,总时间:46.068000视频的加载进度是:%30.944912开始:0.000000,持续:62.160000,总时间:62.160000视频的加载进度是:%41.754270开始:0.000000,持续:73.283000,总时间:73.283000视频的加载进度是:%49.225840开始:0.000000,持续:80.064000,总时间:80.064000视频的加载进度是:%53.780790开始:0.000000,持续:94.298000,总时间:94.298000视频的加载进度是:%63.342088开始:0.000000,持续:101.682000,总时间:101.682000视频的加载进度是:%68.302087开始:0.000000,持续:106.977000,总时间:106.977000视频的加载进度是:%71.858858开始:0.000000,持续:117.775000,总时间:117.775000视频的加载进度是:%79.112117开始:0.000000,持续:132.172000,总时间:132.172000视频的加载进度是:%88.782906开始:0.000000,持续:148.871000,总时间:148.871000视频的加载进度是:%100.000000`
从控制台输出的结果可以很清楚的看到视频加载的进度，这里有几点需要注意
`1、只有播放状态变成AVPlayerItemStatusReadyToPlay时才可以获取视频播放的总时间，提前获取无效;2、我们判断缓冲进度是靠比较视频总时长self.total和已经缓冲的总时间durationSeconds作比较，如果二者相等即达到100%视频则加载完成，这里需要注意，有时这2个值在浮点数下不一定相同，有可能出现99.990836%这样的情况，但是视频实际上已经加载完成，如果我们硬性的凭借100%判断会出现有的视频永远加载不完的假象，因此在判断的时候应该设置一个误差值，比如缓冲进度>99.95%就认为是加载完成了，具体数值可以根据项目自行设定一个合理的误差；3、这里显示的所有数据，比如视频的总时长、进度都是点播类播放源，非M3U8，M3U8是直播无法获取总时长数据，开始时间和缓冲时间等数据也没有参考价值，项目中的直播视频也不会涉及到这些。`
*2、获取AVPlayer当前的播放进度*
```
CMTime ctime = self.avPlayer.currentTime;
    CGFloat currentTimeSec = ctime.value / ctime.timescale;
```
有了当前的播放进度，视频的进度条功能就可以完成，很简单，这里就不赘述了。
*3、关于AVPlayer播放卡顿时如何获取此时的状态也是我遇到的难题，因为AVPlayer并没有给出这种状态，有人说根据AVPlayer的rate是1还是0判断，经测试这个方法不靠谱，即使卡顿时rate有时还是1，文章开头连接的博文给出了一种解决办法，可以参考下，这里大家有什么好的方法欢迎留言，感激不尽。*
```
self.link = [CADisplayLink displayLinkWithTarget:self selector:@selector(upadte)];
    [self.link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
```
实现update方法
```
- (void)upadte
{
NSTimeInterval current = CMTimeGetSeconds(self.avPlayer.currentTime);
if (current!=self.lastTime) {
    //没有卡顿
    NSLog(@"没有卡顿");
}else{
    //卡顿了
    NSLog(@"卡顿了");
}
self.lastTime = current;
}
```
# AVPlayerViewController
- 简介
AVPlayerViewController是iOS9以后推出的新接口，这个接口在我看来主要就是为了iPad的画中画，通过AVPlayerViewControllerDelegate实现，没有太多可以讲的，AVPlayerViewController的使用与AVPlayer基本一样，这里给简单的使用实例看下就可以了，其余的用法参考AVPlayer。
[AVPlayerViewController示例](http://www.jianshu.com/p/eefbe9e5ef14)
# 尾巴
这里只总结了MPMoviePlayerController、AVPlayer、AVPlayerViewController3个开发流媒体常用的接口，在开发中如何选择呢，我总结了一些经验，仅供参考。
1、如果是纯音频的流媒体项目，并且最低支持的版本在iOS9以下，使用MPMoviePlayerController最合适，音频不需要考虑MPMoviePlayerController本身自定义的界面，只需要播放声音，界面我们自己布局就可以了，并且获取播放的各种状态也要比其余两种方便。
2、有视频需求的流媒体项目，如果只是简单的视频需求可以使用MPMoviePlayerController，如果要深度自定义视频播放器，建议使用AVPlayerViewController，用法和AVPlayer基本是一样的，但是AVPlayerViewController的好处是可以在后续方便的实现画中画功能，后续苹果在更新时接口时也会为AVPlayerViewController提供更多有用方便的方法和属性，方便开发者使用。
文／张云龙（简书作者）
原文链接：http://www.jianshu.com/p/6c6b59a875c1
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
