# iOS 播放音频的几种方式----本人备注！！！ - xqhrs232的专栏 - CSDN博客
2019年03月13日 11:56:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：132
原文地址::[https://blog.csdn.net/u013749108/article/details/78765871](https://blog.csdn.net/u013749108/article/details/78765871)
相关文章
1、ios播放音频文件----[https://blog.csdn.net/yuanya/article/details/13773643](https://blog.csdn.net/yuanya/article/details/13773643)
2、iOS实现音频播放----[https://blog.csdn.net/xiaoxingaiwo/article/details/81611123](https://blog.csdn.net/xiaoxingaiwo/article/details/81611123)
3、iOS 播放音乐----[https://blog.csdn.net/lwjok2007/article/details/50425324](https://blog.csdn.net/lwjok2007/article/details/50425324)
4、iOS 播放音效----[https://blog.csdn.net/lwjok2007/article/details/50419327](https://blog.csdn.net/lwjok2007/article/details/50419327)
5、iOS 录音----[https://blog.csdn.net/lwjok2007/article/details/50432931](https://blog.csdn.net/lwjok2007/article/details/50432931)
6、iOS音频播放的几种方式----[https://www.zybuluo.com/kezhen/note/136614](https://www.zybuluo.com/kezhen/note/136614)
对于比较短促的声音，比如系统的推送声音和短信声音，官方要求不要超过30s。
开始播放
NSString *filePath = [[NSBundle mainBundle] pathForResource:@"done" ofType:@"wav"];
NSURL *fileUrl = [NSURL URLWithString:filePath];
SystemSoundID soundID = 0;
AudioServicesCreateSystemSoundID((__bridge CFURLRef)(fileUrl), &soundID);
   AudioServicesAddSystemSoundCompletion(soundID,NULL,NULL,soundCompleteCallBack,NULL);
AudioServicesPlaySystemSound(soundID);
1
2
3
4
5
6
7
播放完成
void soundCompleteCallBack(SystemSoundID soundID, void *clientData)
{
    NSLog(@"播放完成");
}
1
2
3
4
优点：C语言的底层写法，节省内存。 
缺点：支持的格式有限，音量无法通过音量键控制，而且播放方式单一。
第二种播放方式就是 AVAudioPlayer，这种方式适用于音乐时间比较长，或者对播放的控制性要求比较高的场景。
开始播放
if (!self.musicPlayer) {
        NSString *filePath = [[NSBundle mainBundle] pathForResource:@"happy_birthday" ofType:@"mp3"];
        NSURL *fileUrl = [NSURL URLWithString:filePath];
        self.musicPlayer = [[AVAudioPlayer alloc]initWithContentsOfURL:fileUrl error:nil];
        self.musicPlayer.delegate = self;
    }
if (![self.musicPlayer isPlaying]){
    [self.musicPlayer setVolume:0.6];
    [self.musicPlayer prepareToPlay];
    [self.musicPlayer play];
}
1
2
3
4
5
6
7
8
9
10
11
12
暂停
if ([self.musicPlayer isPlaying]){
    [self.musicPlayer pause];
}
1
2
3
播放完成
-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    [self.musicPlayer stop];
    self.musicPlayer=nil;
    self.musicPlayer.delegate = nil;
}
1
2
3
4
5
6
优点： 抒写效率更高，基本上支持所有的音频格式，对播放的控制，如循环播放，声音大小，暂停等比较方便。 
缺点: 相比上一种，对内存的消耗会多些。不支持流式，即无法播放在线音乐。
还有一种就是 AVPlayer，这个和AVAudioPlayer相似，最大的不同，就是支持流播放，即可以播放在线的音乐。
播放
NSURL * url  = [NSURL URLWithString:@"www.xxxxx.mp3"];
AVPlayerItem * songItem = [[AVPlayerItem alloc]initWithURL:url]; 
self.player = [[AVPlayer alloc]initWithPlayerItem:songItem];
[self.player play];
1
2
3
4
5
暂停
[self.player pause];
1
切换上一首或者下一首
[self.player replaceCurrentItemWithPlayerItem:songItem];
1
另外，也提供了一些KVO方法来监听缓冲进度和播放进度等附件功能。
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//本人备注：：
1》怎么让声音支持后台播放
打开后台模式的音乐播放，或者在info.plist文件中添加`Required Background Modes`键，其值是`App plays audio or streams audio/video using AirPlay`
