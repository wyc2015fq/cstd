# iOS实现无限后台background的方法 - game3108的专栏 - CSDN博客
2015年05月26日 18:20:43[game3108](https://me.csdn.net/game3108)阅读数：333
个人分类：[iOS](https://blog.csdn.net/game3108/article/category/2844789)
项目中有需求，要在后台监控某些参数，进行一些逻辑，那么就涉及到如何让app可以在后台更久的运行。
在ios7以前，后台可以
`[[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil]`
的方式，去在后台存活5-10分钟，而在ios8中，只能存活3分钟。
查询过一些资料以后，个人如果要无限的后台存活的话，可能就要涉及到后台播放音乐时最简单的办法。
首先在Required background modes加上audio，然后在applicationDidEnterBackground
中进行播放音乐的操作。
```
<p class="p1"><span class="s1">- (</span><span class="s2">void</span><span class="s1">)applicationDidEnterBackground:(</span><span class="s3">UIApplication</span><span class="s1"> *)application</span></p>{
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
    _shouldStopBg = NO;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(){
        while ( TRUE ) {
            if ( _shouldStopBg ){
                break;
            }
            //do something
            float remainTime = [[UIApplication sharedApplication] backgroundTimeRemaining];
            cootek_log(@"###!!!BackgroundTimeRemaining: %f",remainTime);
            
            if ( remainTime < 20.0 ){
                cootek_log(@"start play audio!");
                
                NSError *audioSessionError = nil;
                AVAudioSession *audioSession = [AVAudioSession sharedInstance];
                if ( [audioSession setCategory:AVAudioSessionCategoryPlayback withOptions:AVAudioSessionCategoryOptionMixWithOthers error:&(audioSessionError)] ){
                    cootek_log(@"set audio session success!");
                }else{
                    cootek_log(@"set audio session fail!");
                }
                
                NSURL *musicUrl = [[NSURL alloc]initFileURLWithPath:[[NSBundle mainBundle] pathForResource:@"bgSong" ofType:@"mp3"]];
                
                self.audioPlayer = [[AVAudioPlayer alloc]initWithContentsOfURL:musicUrl error:nil];
                self.audioPlayer.numberOfLoops = 0;
                self.audioPlayer.volume = 0;
                [self.audioPlayer play];
                
                [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
            }
            [NSThread sleepForTimeInterval:1.0];
            
        }
    });
}
```
其中需要关注的是，audioplayer在arc的环境中会被release，所以需要持有他，而
`[[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];`需要在程序在前台的时候去在一次触发（如果在后台无法触发），所以使用音乐播放的时候的前台触发才行。
