# iOS音视频 教程 - weixin_33985507的博客 - CSDN博客
2016年08月18日 17:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
### 1.播放音效
```
#import "ViewController.h"
#import <AVFoundation/AVFoundation.h> //播放音频需要用到的框架
@interface ViewController ()
@property (nonatomic, assign) SystemSoundID soundID;
@end
@implementation ViewController
//音效
//又称“短音频”，通常在程序中的播放时长为1~2秒
//在应用程序中起到点缀效果，提升整体用户体验
- (void)playSound{
    // 1.获得音效文件的全路径
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"buyao.wav" withExtension:nil];
    // 2.加载音效文件, 创建音效ID（SoundID）
    AudioServicesCreateSystemSoundID((__bridge CFURLRef)url, &_soundID);
    // 3.播放音效
    AudioServicesPlaySystemSound(_soundID);
}
@end
```
### 2.播放音乐
`类方法不能访问成员属性,只能访问 static NSMutableDictionary *_musicPlayers; 静态变量`
`ZYXAudioTool.h`
```
#import <Foundation/Foundation.h>
@interface ZYXAudioTool : NSObject
/**
 *  播放音效
 *
 *  @param filename 音效的文件名
 */
+ (void)playSound:(NSString *)filename;
/**
 *  销毁音效
 *
 *  @param filename 音效的文件名
 */
+ (void)disposeSound:(NSString *)filename;
/**
 *  播放音乐
 *
 *  @param filename 音乐的文件名
 */
+ (BOOL)playMusic:(NSString *)filename;
/**
 *  暂停音乐
 *
 *  @param filename 音乐的文件名
 */
+ (void)pauseMusic:(NSString *)filename;
/**
 *  停止音乐
 *
 *  @param filename 音乐的文件名
 */
+ (void)stopMusic:(NSString *)filename;
@end
```
`ZYXAudioTool.m`
```
#import "ZYXAudioTool.h"
#import <AVFoundation/AVFoundation.h>
@implementation ZYXAudioTool
#pragma mark - 存放所有的音效ID
static NSMutableDictionary *_soundIDs;
+ (NSMutableDictionary *)soundIDs
{
    if (!_soundIDs) {
        _soundIDs = [NSMutableDictionary dictionary];
    }
    return _soundIDs;
}
#pragma mark - 播放音效 @param filename 音效的文件名
+ (void)playSound:(NSString *)filename
{
    if (!filename) return;
    
    // 1.取出对应的音效ID
    SystemSoundID soundID = [[self soundIDs][filename] intValue];
    
    // 2.初始化 音效文件只需要加载1次
    if (!soundID) {
        // 1.音频文件的URL
        NSURL *url = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
        if (!url) return;
        
        // 2.加载音效文件，得到对应的音效ID
        AudioServicesCreateSystemSoundID((__bridge CFURLRef)(url), &soundID);
        
        // 存入字典
        [self soundIDs][filename] = @(soundID);
    }
    
    // 3.播放
    AudioServicesPlaySystemSound(soundID);
}
#pragma mark - 销毁音效 @param filename 音效的文件名
+ (void)disposeSound:(NSString *)filename
{
    if (!filename) return;
    
    // 1.取出对应的音效ID
    SystemSoundID soundID = [[self soundIDs][filename] intValue];
    
    // 2.销毁
    if (soundID) {
        // 释放音效资源
        AudioServicesDisposeSystemSoundID(soundID);
        [[self soundIDs] removeObjectForKey:filename];
    }
}
#pragma mark - 存放所有的音乐播放器
static NSMutableDictionary *_musicPlayers;
+ (NSMutableDictionary *)musicPlayers
{
    if (!_musicPlayers) {
        _musicPlayers = [NSMutableDictionary dictionary];
    }
    return _musicPlayers;
}
#pragma mark - 播放音乐 @param filename 音乐的文件名
+ (BOOL)playMusic:(NSString *)filename
{
    if (!filename) return NO;
    
    // 1.取出对应的播放器
    AVAudioPlayer *player = [self musicPlayers][filename];
    
    // 2.播放器没有创建，进行初始化
    if (!player) {
        // 音频文件的URL
        NSURL *url = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
        if (!url) return NO;
        
        // 创建播放器(一个AVAudioPlayer只能播放一个URL)
        player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
        
        // 缓冲
        if (![player prepareToPlay]) return NO;
        
        // 存入字典
        [self musicPlayers][filename] = player;
    }
    
    // 3.播放
    if (!player.isPlaying) {
         return [player play];
    }
    
    // 正在播放
    return YES;
}
#pragma mark - 暂停音乐 @param filename 音乐的文件名
+ (void)pauseMusic:(NSString *)filename
{
    if (!filename) return;
    
    // 1.取出对应的播放器
    AVAudioPlayer *player = [self musicPlayers][filename];
    
    // 2.暂停
    if (player.isPlaying) {
        [player pause];
    }
}
#pragma mark - 停止音乐 @param filename 音乐的文件名
+ (void)stopMusic:(NSString *)filename
{
    if (!filename) return;
    
    // 1.取出对应的播放器
    AVAudioPlayer *player = [self musicPlayers][filename];
    
    // 2.停止
    [player stop];
    
    // 3.将播放器从字典中移除
    [[self musicPlayers] removeObjectForKey:filename];
}
@end
```
`ViewController.m`
```
#import "ViewController.h"
#import "ZYXAudioTool.h"
@interface ViewController ()
- (IBAction)play;
- (IBAction)pause;
- (IBAction)stop;
- (IBAction)next;
@property (nonatomic, strong) NSArray *songs;
@property (nonatomic, assign) int currentIndex;
@end
@implementation ViewController
- (NSArray *)songs{
    if (!_songs) {
        self.songs = @[@"120125029.mp3", @"309769.mp3", @"235319.mp3"];
    }
    return _songs;
}
- (IBAction)play {
    [ZYXAudioTool playMusic:self.songs[self.currentIndex]];
}
- (IBAction)pause {
    [ZYXAudioTool pauseMusic:self.songs[self.currentIndex]];
}
- (IBAction)stop {
    [ZYXAudioTool stopMusic:self.songs[self.currentIndex]];
}
- (IBAction)next {
    [self stop];
    self.currentIndex++;
    if (self.currentIndex >= self.songs.count) {
        self.currentIndex = 0;
    }
    [self play];
}
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    [ZYXAudioTool playSound:@"buyao.wav"];
}
@end
```
![396193-915ea89151c96d11.png](https://upload-images.jianshu.io/upload_images/396193-915ea89151c96d11.png)
1-项目中的音效和音乐文件.png
### 3.本地和网络流媒体音频视频播放
`AVPlayer` 播放本地/网络 音频 视频
`MPMoviePlayerController` 播放本地/网络 音频 视频
`MPMoviePlayerViewController` 播放本地/网络 音频 视频
`ViewController.m`
```
#import "ViewController.h"
#import <MediaPlayer/MediaPlayer.h>
#import <AVFoundation/AVFoundation.h>
@interface ViewController ()
@property (nonatomic, strong) MPMoviePlayerController *mpc;
@property (nonatomic, strong) AVPlayer *player;
@end
@implementation ViewController
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self networkVideo];
}
-(void)viewDidLoad
{
    [super viewDidLoad];
    
    [self avPlayer];
    [self moviePlayerController];
}
- (void)avPlayer{
    NSURL *url = nil;
    
#pragma mark - local
    url = [[NSBundle mainBundle] URLForResource:@"minion_01.mp4" withExtension:nil];
    
#pragma mark - network
    url = [NSURL URLWithString:@"http://localhost/videos/235319.mp3"];
    url = [NSURL URLWithString:@"http://localhost/videos/minion_15.mp4"];
    
    // 创建播放器
    AVPlayer *player = [AVPlayer playerWithURL:url];
    [player play];
    // 创建播放器图层
    AVPlayerLayer *layer = [AVPlayerLayer layer];
    layer.player = player;
    layer.frame = CGRectMake(10, 20, 300, 200);
    // 添加图层到控制器的view
    [self.view.layer addSublayer:layer];
}
- (void)moviePlayerController{
    // 创建播放器对象
    MPMoviePlayerController *mpc = [[MPMoviePlayerController alloc] init];
    
#pragma mark - local
    mpc.contentURL = [[NSBundle mainBundle] URLForResource:@"minion_01.mp4" withExtension:nil];
    
#pragma mark - network
    mpc.contentURL = [NSURL URLWithString:@"http://localhost/videos/235319.mp3"];
    mpc.contentURL = [NSURL URLWithString:@"http://localhost/videos/minion_15.mp4"];
    
    
    // 添加播放器界面到控制器的view上面
    mpc.view.frame = CGRectMake(10, self.view.frame.size.height-240, 300, 200);
    [self.view addSubview:mpc.view];
    
    UIView *controlView         = [[UIView alloc] init];
    controlView.backgroundColor = [UIColor redColor];
    controlView.frame           = CGRectMake(0, 0, 300, 50);
    [mpc.view addSubview:controlView];
    
    // 隐藏自动自带的控制面板
    mpc.controlStyle = MPMovieControlStyleNone;
    
    // 监听播放器
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(movieStateDidChange)
                                                 name:MPMoviePlayerPlaybackStateDidChangeNotification
                                               object:mpc];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(movieDidFinish)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:mpc];
    [mpc play];
    self.mpc = mpc;
}
- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (void)movieStateDidChange{
    NSLog(@"----播放状态--%ld", (long)self.mpc.playbackState);
}
- (void)movieDidFinish{
    NSLog(@"----播放完毕");
}
// ----播放状态--1
// ----播放状态--2
// ----播放完毕
#pragma mark - network 
- (void)networkVideo{
    // MPMoviePlayerViewController继承自UIViewController，它内部封装了一个MPMoviePlayerController
    // MPMoviePlayerViewController只能全屏播放
    
    // NSURL *url = [NSURL URLWithString:@"http://streams.videolan.org/streams/mp4/Mr_MrsSmith-h264_aac.mp4"];
    
    NSURL *url = [NSURL URLWithString:@"http://localhost/videos/minion_01.mp4"];
    MPMoviePlayerViewController *mpvc = [[MPMoviePlayerViewController alloc] initWithContentURL:url];
    [self presentMoviePlayerViewControllerAnimated:mpvc];
}
@end
```
![396193-0d324281651c4fbe.png](https://upload-images.jianshu.io/upload_images/396193-0d324281651c4fbe.png)
2-AVPlayer-MPMoviePlayerController-MPMoviePlayerViewController.png
### 4.VLC框架本地和网络流媒体音频视频播放
`ViewController.m`
```
#import "ViewController.h"
#import <MobileVLCKit/MobileVLCKit.h>
@interface ViewController ()
@property (nonatomic, strong) VLCMediaPlayer *player;
@end
@implementation ViewController
- (void)vlcPlayer{
    
    self.player = [[VLCMediaPlayer alloc] init];
    
    // 设置需要播放的多媒体文件
    
    // @"http://streams.videolan.org/streams/mp4/Mr_MrsSmith-h264_aac.mp4"
    // @"http://y1.eoews.com/assets/ringtones/2012/5/18/34045/hi4dwfmrxm2citwjcc5841z3tiqaeeoczhbtfoex.mp3"
    
    NSURL *url = nil;
    
#pragma mark - local
    url = [[NSBundle mainBundle] URLForResource:@"minion_01.mp4" withExtension:nil];
    
#pragma mark - network
    url = [NSURL URLWithString:@"http://localhost/videos/235319.mp3"];
    url = [NSURL URLWithString:@"http://localhost/videos/minion_15.mp4"];
   
    self.player.media = [VLCMedia mediaWithURL:url];
    // 设置播放界面的载体
    self.player.drawable = self.view;
    // 播放
    [self.player play];
}
@end
```
### 5.AVAudioRecorder录音
`ViewController.m`
```
#import "ViewController.h"
#import <AVFoundation/AVFoundation.h>
@interface ViewController ()
- (IBAction)startRecord;
- (IBAction)stopRecord;
@property (nonatomic, strong) AVAudioRecorder *recorder;
@property (nonatomic, strong) CADisplayLink *link;
@property (nonatomic, assign) double slientDuration;
@end
@implementation ViewController
- (CADisplayLink *)link
{
    if (!_link) {
        self.link = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    }
    return _link;
}
- (void)update
{
    // 1.更新录音器的测量值
    [self.recorder updateMeters];
    
    // 2.获得平均分贝
    float power = [self.recorder averagePowerForChannel:0];
    
    // 3.如果小于-30, 开始静音
    if (power < -30) {
        if ([self.recorder isRecording]) {
            [self.recorder pause];
        }
        
        self.slientDuration += self.link.duration;
        
        if (self.slientDuration >= 2) { // 沉默至少2秒钟
            [self.recorder stop];
            
            // 停止定时器
            [self.link invalidate];
            self.link = nil;
            NSLog(@"--------停止录音");
        }
    } else {
        if (![self.recorder isRecording]) {
            [self.recorder record];
        }
        self.slientDuration = 0;
        NSLog(@"**********持续说话");
    }
}
- (IBAction)startRecord {
    NSString *path = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject]
                      stringByAppendingPathComponent:@"zyx.caf"];
    NSLog(@"path = %@",path);
    NSURL *url = [NSURL fileURLWithPath:path];
    
    // 1.创建录音器
    NSMutableDictionary *setting = [NSMutableDictionary dictionary];
    // 音频格式
    setting[AVFormatIDKey] = @(kAudioFormatAppleIMA4);
    // 音频采样率
    setting[AVSampleRateKey] = @(8000.0);
    // 音频通道数
    setting[AVNumberOfChannelsKey] = @(1);
    // 线性音频的位深度
    setting[AVLinearPCMBitDepthKey] = @(8);
    AVAudioRecorder *recorder = [[AVAudioRecorder alloc] initWithURL:url settings:setting error:nil];
    
    // 允许测量分贝
    recorder.meteringEnabled = YES;
    
    // 2.缓冲
    [recorder prepareToRecord];
    
    // 3.录音
    [recorder record];
    
    self.recorder = recorder;
    
    // 4.开启定时器
    self.slientDuration = 0;
    [self.link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}
- (IBAction)stopRecord {
    [self.recorder stop];
}
@end
```
### 6.录音和播放代理
`AVAudioRecorderDelegate``AVAudioPlayerDelegate`
`ViewController.m`
```
#import "ViewController.h"
#import <AVFoundation/AVFoundation.h>
#define kMoniterFile    @"monitor.caf"
#define kRecorderFile   @"recorder.caf"
#define kRecorderPower  -25.0
@interface ViewController () <AVAudioRecorderDelegate, AVAudioPlayerDelegate>
{
    AVAudioRecorder     *_monitor;      // 监听器
    AVAudioRecorder     *_recorder;     // 录音机
    AVAudioPlayer       *_player;       // 播放器
    
    NSTimer             *_timer;        // 时钟
    
    NSTimeInterval      _silenceDuration;   // 累计静音时长
    NSDate              *_lastRecorderTime; // 末次录音时间
}
@end
@implementation ViewController
#pragma mark - 私有方法
#pragma mark - 录音设置
- (NSDictionary *)recorderSettings{
    NSMutableDictionary *setting = [[NSMutableDictionary alloc] init];
    // 音频格式
    [setting setValue:[NSNumber numberWithInt:kAudioFormatAppleIMA4] forKey:AVFormatIDKey];
    // 音频采样率
    [setting setValue:[NSNumber numberWithFloat:8000.0] forKey:AVSampleRateKey];
    // 音频通道数
    [setting setValue:[NSNumber numberWithInt:1] forKey:AVNumberOfChannelsKey];
    // 线性音频的位深度
    [setting setValue:[NSNumber numberWithInt:8] forKey:AVLinearPCMBitDepthKey];
    
    return setting;
}
#pragma mark - 创建URL
- (NSURL *)urlWithFileName:(NSString *)fileName{
    NSArray *docs = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [docs[0] stringByAppendingPathComponent:fileName];
    NSLog(@"path = %@",path);
    
    return [NSURL fileURLWithPath:path];
}
#pragma mark - 实例化录音机
- (AVAudioRecorder *)recorderWithFileName:(NSString *)fileName{
    // 1. 建立URL
    NSURL *url = [self urlWithFileName:fileName];
    // 2. 实例化录音机
    NSError *error = nil;
    AVAudioRecorder *recorder = [[AVAudioRecorder alloc] initWithURL:url
                                                            settings:[self recorderSettings]
                                                               error:&error];
    
    if (error) {
        NSLog(@"实例化录音机出错 - %@", error.localizedDescription);
        return nil;
    }
    
    // 3. 准备录音
    [recorder prepareToRecord];
    
    return recorder;
}
#pragma mark - 实例化播放器
- (AVAudioPlayer *)playerWithFileName:(NSString *)fileName{
    // 1. 建立URL
    NSURL *url = [self urlWithFileName:fileName];
    
    // 2. 实例化播放器
    NSError *error = nil;
    AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    
    if (error) {
        NSLog(@"实例化播放器出错 - %@", error.localizedDescription);
        return nil;
    }
    
    // 3. 准备播放
    [player prepareToPlay];
    
    return player;
}
#pragma mark - 启动时钟
- (void)startTimer{
    NSLog(@"启动时钟");
    _timer = [NSTimer scheduledTimerWithTimeInterval:0.1f
                                              target:self
                                            selector:@selector(timerFired:)
                                            userInfo:nil
                                             repeats:YES];
}
#pragma mark - 设置录音机
- (void)setupRecorders{
    // 1. 监听器
    _monitor = [self recorderWithFileName:kMoniterFile];
    // 1.1 设置音量监听
    [_monitor setMeteringEnabled:YES];
    
    // 2. 录音机
    _recorder = [self recorderWithFileName:kRecorderFile];
    // 2.1 设置代理，监听录音完成事件
    [_recorder setDelegate:self];
    
    // 3. 启动监听器
    [_monitor record];
    
    // 4. 时钟随时检测监听器的数值
    [self startTimer];
}
#pragma mark - 时钟触发方法
- (void)timerFired:(NSTimer *)timer{
    // 1. 更新监听器的数值
    [_monitor updateMeters];
    
    // 2. 取出监听器第一个声道的音量
    CGFloat power = [_monitor averagePowerForChannel:0];
    
    // 3. 判断音量大小
    if (power > kRecorderPower) {
        // 要录音，首先判断录音机是否处于录音状态
        if (![_recorder isRecording]) {
            NSLog(@"启动录音");
            
            // 启动录音机
            [_recorder record];
        }
        
        // 使用当前系统时间，作为末次录音时间
        _lastRecorderTime = [NSDate date];
    } else if ([_recorder isRecording]){
        NSLog(@"出现静音...");
        
        // 累计静音时间，如果过长，停止录音
        if (_silenceDuration > 1.0f) {
            NSLog(@"停止录音，准备播放");
            // 停止录音
            [_recorder stop];
        } else {
            NSLog(@"累加静音时长...");
            
            // 累加静音时间
            _silenceDuration += [[timer fireDate] timeIntervalSinceDate:_lastRecorderTime];
        }
    }
    
    // 4. 如果总静音时长超过一定时间，重新启动监听器
    if (_silenceDuration > 10.0f) {
        [_monitor stop];
        [_monitor record];
    }
}
#pragma mark 播放录音
- (void)startPlaying{
    // 1. 实例化播放器
    _player = [self playerWithFileName:kRecorderFile];
    
    // 2. 设置播放器属性
    [_player setNumberOfLoops:0];
    [_player setDelegate:self];
    
    // 3. 设置播放速度
    [_player setEnableRate:YES];
    [_player setRate:1.2f];
    
    // 4. 开始播放
    [_player play];
}
#pragma mark - 代理方法
#pragma mark - 录音完成
- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag{
    // 1. 停止监听器
    [_monitor stop];
    
    // 2. 停止时钟
    [_timer invalidate];
    _timer = nil;
    
    // 3. 播放录音
    [self startPlaying];
}
#pragma mark 播放完成
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    // 1. 重新启动监听器
    [_monitor record];
    
    // 2. 启动时钟
    [self startTimer];
}
- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setupRecorders];
}
@end
```
