# iOS录音简单实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月10日 22:42:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5993
> 
简单实现录音功能
导入`<AVFoundation/AVFoundation.h>` **
```
#import <AVFoundation/AVFoundation.h>
/** 时间显示 */
@property (weak, nonatomic) IBOutlet UILabel      *timeLabel;
/** 播放按钮 */
@property (weak, nonatomic) IBOutlet UIButton     *playButton;
/** 重置按钮 */
@property (weak, nonatomic) IBOutlet UIButton     *resetButton;
/** 录音 */
@property (nonatomic, strong) AVAudioRecorder *recorder;
/** 播放 */
@property (nonatomic, strong) AVAudioPlayer   *player;
/** 定时器 */
@property (nonatomic, strong) NSTimer         *timer;
/** 录音url */
@property (nonatomic, strong) NSString        *playName;
- (void)viewDidLoad {
[super viewDidLoad];
[self setupRecorder];
self.timeLabel.text = @"00:00.00";
}
```
只要走到这就开始录音
```
- (void)setupRecorder {
if ([[[UIDevice currentDevice] systemVersion] compare:@"7.0"] != NSOrderedAscending) {
    //7.0第一次运行会提示，是否允许使用麦克风
    AVAudioSession *session = [AVAudioSession sharedInstance];
    NSError *sessionError;
    //AVAudioSessionCategoryPlayAndRecord用于录音和播放
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&sessionError];
    if(session == nil)
        NSLog(@"Error creating session: %@", [sessionError description]);
    else
        [session setActive:YES error:nil];
}
NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
self.playName = [NSString stringWithFormat:@"%@/play.aac",docDir];
//录音设置
self.recorderSettingsDict =[[NSDictionary alloc] initWithObjectsAndKeys:
                       [NSNumber numberWithInt:kAudioFormatMPEG4AAC],AVFormatIDKey,
                       [NSNumber numberWithInt:44100.0],AVSampleRateKey,
                       [NSNumber numberWithInt:2],AVNumberOfChannelsKey,
                       [NSNumber numberWithInt:8],AVLinearPCMBitDepthKey,
                       [NSNumber numberWithBool:NO],AVLinearPCMIsBigEndianKey,
                       [NSNumber numberWithBool:NO],AVLinearPCMIsFloatKey,
                       nil];
NSError *error = nil;
//必须真机上测试,模拟器上可能会崩溃
self.recordering = [[AVAudioRecorder alloc] initWithURL:[NSURL URLWithString:self.playName]
                                               settings:self.recorderSettingsDict
                                                  error:&error];
if (self.recordering) {
    // 打开音量检测
    self.recordering.meteringEnabled = YES;
    // 创建文件准备录音
    [self.recordering prepareToRecord];
    // 开始录音
    [self.recordering record];
    //启动定时器
    self.timer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(levelTimer:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSRunLoopCommonModes];
    }
}
- (void)levelTimer:(NSTimer *)time {
    // 后面stringFormatWithTimeInterval是自己写的方法用于转换时间格式
    self.timeLabel.text = [NSString stringFormatWithTimeInterval:self.recordering.currentTime];
}
```
下面是结束录音和定时器
```
[self.recordering stop];
self.recordering = nil;
[self.timer invalidate];
self.timer = nil;
```
播放录音的话很简单
```
self.player = nil;
    self.player = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL URLWithString:self.playName] error:&playerError];
```
接下来是recorder的一些基本设置属性
```
//设置录音格式 AVFormatIDKey==kAudioFormatLinearPCM
[recordSetting setValue:[NSNumber numberWithInt:kAudioFormatMPEG4AAC] forKey:AVFormatIDKey];
//设置录音采样率(Hz) 如：AVSampleRateKey==8000/44100/96000（影响音频的质量）
[recordSetting setValue:[NSNumber numberWithFloat:44100] forKey:AVSampleRateKey];
//录音通道数 1 或 2
[recordSetting setValue:[NSNumber numberWithInt:1] forKey:AVNumberOfChannelsKey];
//线性采样位数 8、16、24、32
[recordSetting setValue:[NSNumber numberWithInt:16] forKey:AVLinearPCMBitDepthKey];
//录音的质量
[recordSetting setValue:[NSNumber numberWithInt:AVAudioQualityHigh] forKey:AVEncoderAudioQualityKey];
```
