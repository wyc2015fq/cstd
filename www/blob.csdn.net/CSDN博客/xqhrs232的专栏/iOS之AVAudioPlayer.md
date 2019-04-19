# iOS之AVAudioPlayer - xqhrs232的专栏 - CSDN博客
2019年03月13日 16:29:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：11
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/79ce9f9839fa](https://www.jianshu.com/p/79ce9f9839fa)
相关文章
1、[AVAudioPlayer](https://www.cnblogs.com/laolitou-ping/p/6257265.html)----[https://www.cnblogs.com/laolitou-ping/p/6257265.html](https://www.cnblogs.com/laolitou-ping/p/6257265.html)
苹果系统带有一个音频播放器，这就是AVAudioPlayer，要使用这个播放器进行播放音频，首先需要创建这个播放器对象，而这个播放器的创建所需的头文件并不是在foundation的头文件下而是在播放器自己专属的头文件下，如下：
```
#import <AVFoundation/AVFoundation.h>
```
而AV开头的文件里包含了许多的和音视频相关的头文件。
一、创建播放器对象
我们先声明一个播放器的全局变量，而且只能声明称全局变量。
```
AVAudioPlayer *_audioPlayer;
```
此时我们创建播放器对象：
```
_audioPlayer = [[AVAudioPlayer alloc]initWithContentsOfURL:url error:nil];
```
这里有两个参数：
参数1：需要播放的音频的地址，这里可以是本地或者是网页的文件
参数2：错误信息
这里就暂且使用本地资源，需要创建本地资源的路径，然后转为url路径
```
NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:nil];
```
这里有两个参数：
参数1：文件的名字
参数2：文件的类型
注意的是：在参数1中如果包含了名字和类型，在参数2中可以不用谢类型，直接写nil就可以了。
然后将string类型的路径用url来接收
```
NSURL *url = [NSURL fileURLWithPath:path];
```
这里的参数就是之前创建的NSString类型的路径。
完整的代码如下：
```
NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:nil];
NSURL *url = [NSURL fileURLWithPath:path];
_audioPlayer = [[AVAudioPlayer alloc]initWithContentsOfURL:url error:nil];
```
### 二、播放
音频播放之前需要准备播放，这里需要调用一个方法；
```
[_audioPlayer prepareToPlay];
```
就绪后直接播放
```
[_audioPlayer play];
```
以上就是和音频最主要的功能的介绍。
### 三、音频播放器的相关功能
下面是对音频播放器的扩展，音频播放器一般会有的功能如：开始播放、暂停播放、调整音频的音量的大小、调整音频的进度、显示音频文件的内部附有的图片信息及协议代理等。
1.开始和暂停播放音频
首先，为了方便，我们在storyBoard去拖一个按钮控件，主要功能是让音频暂停和播放。
这里我们可以用到自动布局的相关知识，在storyBoard里运用约束条件，使得可以进行适配手机。这里对按钮的左右下和高度进行约束。
让按钮的初始文字显示为`播放`，点击`播放`后,显示为`STOP`,暂停后显示为`play`；随便设个背景颜色用于区分
然后直接在storyBoard里面拖个方法到ViewController.m文件的@implementation里，然后就在这里这个按钮的点击事件里执行音频的播放和暂停
代码如下：
```
#pragma mark - 开始暂停按钮
- (IBAction)button:(UIButton *)sender {
    // 用一个静态变量对按钮的状态进行控制
    static int i = 0;
    if (i == 0) {
        // 开始播放
        [_audioPlayer play];
        // 开始后按钮文字变为stop，按钮的状态为正常
        [sender setTitle:@"stop" forState:UIControlStateNormal];
        i = 1;
    }
    else {
        // 4.暂停播放
        [_audioPlayer pause];
        //[_audioPlayer stop];
        [sender setTitle:@"play" forState:UIControlStateNormal];
        i = 0;
    }
}
```
2.调整音量大小
我们还是可以在storyBoard中拖一个控件出来，这里需要一个步进器，然后把相关的方法拖到文件中去实现。
```
#pragma mark - 改变音量
- (IBAction)volumeChange:(UIStepper *)sender {
    // 设置音量(0 ~ 1);我们设置的步进的音量值为0.1
    _audioPlayer.volume = sender.value;
}
```
3.调整音频播放进度
3.1更新进度
调整进度就需要用到滑块控件，然后向之前的在storyBoard里去拖个滑块的控件，还有相关的属性和方法。
这里我们可能会运用到定时器，所以我们需要创建定时器，而这个定时器的创建应该放到准备播放音频的时候就创建出来。
```
// 添加定时器
[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(updateProgress) userInfo:nil repeats:YES];
```
这里的参数为：
参数1：定时器的监测时间，单位是`S`
参数2：执行的对象
参数3：选择器（需要执行的方法）
参数4：用户的信息，一般为nil
参数5：是否需要重复执行，这里是需要的，为YES
实现选择器中的方法；
```
#pragma mark - 更新进度updateProgress
- (void)updateProgress {
    // 当前时间
    double currentTime = _audioPlayer.currentTime;
    // 总时间
    double totalTime = _audioPlayer.duration;
    // 计算进度
    float progress = currentTime / totalTime;
    // 展示到进度条上
    self.sliderProgress.value = progress;
}
```
3.2更改滑块的进度
这里就需要实现滑块拖出来的方法了
```
#pragma mark - 改变进度条
- (IBAction)sliderProgress:(UISlider *)sender {
    // 当前时间 = 总时间 * 爆发进度
    double currentTime = _audioPlayer.duration * _sliderProgress.value;
    // 是否让进度条实时更新。
    _sliderProgress.continuous = YES;
    // 重新设置播放器的当前时间
    _audioPlayer.currentTime = currentTime;
}
```
4.获取专辑的图片
同样的需要在storyBoard上拖一个imageView的控件，展示出其属性。
```
#pragma mark - 获取专辑图片
- (void)showPlayerImage {
    AVURLAsset *asset = [[AVURLAsset alloc]initWithURL:_audioPlayer.url options:nil];
    NSString *format = [asset availableMetadataFormats].firstObject;
    NSArray *array = [asset metadataForFormat:format];
    for (AVMutableMetadataItem *item in array) {
        // 这里的artwork是需要在音频文件里去得到的。可以吧array打印出来，得到里面的值
        if ([item.commonKey isEqualToString:@"artwork"]) {
            NSData *data = (NSData *)item.value;
            UIImage *image = [UIImage imageWithData:data];
            _imageView.image = image;
        }
    }
}
```
5.协议代理
5.1当前音频播放器播放的音频结束后会自动调用这个方法
```
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag {
    NSLog(@"播放结束");
    // 切换下一首
    [self preparePlayerWithName:@"春天里.mp3"];
    [_audioPlayer play];
}
```
5.2音频播放被中断的时候会调用这个方法
一般有电话的接入和有短信接收的时候，一般会有中断，这需要调用这个方法。我们一般执行的都是暂停音频的播放。
```
- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player {
    NSLog(@"一般这个方法中会暂停音频");
    [player stop];
}
```
5.3中断结束后
一般就在来电结束后会调用这个方法。一般在这个方法中执行的是会继续音频的播放。
```
// 中断结束的时候会调用这个方法
- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player withOptions:(NSUInteger)flags {
    // 一般在这个方法中继续播放音频
    [player play];
    NSLog(@"继续播放");
}
```
5.4解码出错
```
// 解码错误的时候会调用这个方法
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {
    NSLog(@"文件出错");
}
```
总结：
1.这里需要导入的头文件一定是：AVFoundation/AVFoundation.h；
2.音频播放器AVAudioPlayer作为一个对象，要使用必须要先创建对象，创建对象前需要给一个音频文件的路径（一般都是url）；
3.声明对象一定是要全局变量，不然运行可能失败，因为会调用到偏硬件的；
4.就是在storyBoard里拖控件的时候需要注意自动布局的约束条件，为的是适配；
5.在有代理的设置的时候，一定要遵循代理：AVAudioPlayerDelegate。
