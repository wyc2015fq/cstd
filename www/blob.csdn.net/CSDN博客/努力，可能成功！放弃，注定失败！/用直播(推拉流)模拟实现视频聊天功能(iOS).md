# 用直播(推拉流)模拟实现视频聊天功能(iOS) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月15日 10:26:43[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4613
demo已经写好很久了,懒癌证复发一直没上传~开始进入正题
本文主要是用来练习如何实现直播功能,既推流+拉流,真正的视频聊天并不是这么做的╮(╯▽╰)╭ 咱们的目的是学会如何实现直播功能
> 
说下简单的步骤:搭建本地服务器->推流->拉流->perfect <(￣▽￣)> 哇哈哈…
实现原理:既向一个服务器同时进行推流和拉流,只不过对应的"房间号"不同而已,比如A和B住在同一栋楼(IP地址),A从B的房间拿东西(拉流)并且A向自己的房间放东西(推流);B向A的得房间拿东西(拉流)并且B向自己的房间放东西(推流);此时只要输入正确的房间号就可以实现了
## 服务器
首先你要找到一个测试服务器或者创建本地Nginx服务器,搭建本地服务器请看JJAAIR的文章[Mac搭建nginx+rtmp服务器](http://www.jianshu.com/p/02222073b3f1)
注意注意,搭建服务器配置nginx.conf文件时,application可以随便写,但要记住,后面会用到
![](http://upload-images.jianshu.io/upload_images/2100810-09c4e04e6b1b15b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
nginx.conf
## 概述
现在开始创建xcode文件吧~推流端用的是LFLiveKit框架,拉流用IJKPlayer,先看下整个文件目录
![](http://upload-images.jianshu.io/upload_images/2100810-520abf0e39554ae9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文件目录
> 
是的,没有看错,几个文件就能完成整个推拉流的过程╮(╯▽╰)╭ 主要实现是HBVideoChatViewController文件
```
//
//  HBVideoChatViewController.m
//  视频聊天
@interface HBVideoChatViewController ()<LFLiveSessionDelegate>
//当前区域网所在IP地址
@property (nonatomic,copy) NSString *ipAddress;
//我的房间号
@property (nonatomic,copy) NSString *myRoom;
//别人的房间号
@property (nonatomic,copy) NSString *othersRoom;
//ip后缀(如果用本地服务器,则为在nginx.conf文件中写的rtmplive)
@property (nonatomic, copy) NSString *suffix;
//大视图
@property (nonatomic,weak) UIView *bigView;
//小视图
@property (nonatomic,weak) UIView *smallView;
//播放器
@property (nonatomic,strong) IJKFFMoviePlayerController *player;
//推流会话
@property (nonatomic,strong) LFLiveSession *session;
@end
```
## 推流
LFLiveKit这个推流框架的关键类是LFLiveSession,也是依靠着个类来实现推流的,底层的实现则是对ffmpeg的封装,有兴趣的童鞋可以去研究研究,废话少说上代码~
首先,创建session并进行一些配置
```
- (LFLiveSession *)session{
    if (_session == nil) {
        //初始化session要传入音频配置和视频配置
        //音频的默认配置为:采样率44.1 双声道
        //视频默认分辨率为360 * 640
        _session = [[LFLiveSession alloc] initWithAudioConfiguration:[LFLiveAudioConfiguration defaultConfiguration] videoConfiguration:[LFLiveVideoConfiguration defaultConfigurationForQuality:LFLiveVideoQuality_Low1] ];
        //设置返回的视频显示在指定的view上
        _session.preView = self.smallView;
        _session.delegate = self;
        //是否输出调试信息
        _session.showDebugInfo = NO;
    }
    return _session;
}
```
LFLiveAudioConfiguration和LFLiveVideoConfiguration都可以进行定制,配置的高低影响传输的速度和质量,具体的文档都有中文注释写得很清楚
接着向系统请求设备授权
```
/**
 *  请求摄像头资源
 */
- (void)requesetAccessForVideo{
    __weak typeof(self) weakSelf = self;
    //判断授权状态
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    switch (status) {
        case AVAuthorizationStatusNotDetermined:{
            //发起授权请求
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
                if (granted) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        //运行会话
                        [weakSelf.session setRunning:YES];
                    });
                }
            }];
            break;
        }
        case AVAuthorizationStatusAuthorized:{
            //已授权则继续
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.session setRunning:YES];
            });
            break;
        }
        default:
            break;
    }
}
/**
 *  请求音频资源
 */
- (void)requesetAccessForMedio{
    __weak typeof(self) weakSelf = self;
    //判断授权状态
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    switch (status) {
        case AVAuthorizationStatusNotDetermined:{
            //发起授权请求
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
                if (granted) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        //运行会话
                        [weakSelf.session setRunning:YES];
                    });
                }
            }];
            break;
        }
        case AVAuthorizationStatusAuthorized:{
            //已授权则继续
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.session setRunning:YES];
            });
            break;
        }
        default:
            break;
    }
}
```
通过代理方法来处理连接异常
```
//连接错误回调
- (void)liveSession:(nullable LFLiveSession *)session errorCode:(LFLiveSocketErrorCode)errorCode{
//弹出警告
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Warning" message:@"连接错误,请检查IP地址后重试" preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *sure = [UIAlertAction actionWithTitle:@"sure" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self.navigationController popViewControllerAnimated:YES];
    }];
    [alert addAction:sure];
    [self presentViewController:alert animated:YES completion:nil];
}
```
全部设置好就可以开始推流啦~
```
- (void)viewDidLoad{
    ...
//    推流端
    [self requesetAccessForVideo];
    [self requesetAccessForMedio];
    [self startLive];
    ...
}
- (void)startLive{
    //RTMP要设置推流地址
    LFLiveStreamInfo *streamInfo = [LFLiveStreamInfo new];
    streamInfo.url = [NSString stringWithFormat:@"rtmp://%@:1935/%@/%@",self.ipAddress,self.suffix,self.myRoom];
    [self.session startLive:streamInfo];
}
- (void)stopLive{
    [self.session stopLive];
}
```
## 拉流
用IJKPlayer进行拉流,具体的编译和集成步骤可以看[iOS中集成ijkplayer视频直播框架](http://www.jianshu.com/p/1f06b27b3ac0),也可以直接将我编译好的IJK拖到项目中即可,在文章最后会给出下载地址
对播放器进行初始化
```
-(IJKFFMoviePlayerController *)player{
    if (_player == nil) {
        IJKFFOptions *options = [IJKFFOptions optionsByDefault];
        _player = [[IJKFFMoviePlayerController alloc] initWithContentURLString:[NSString stringWithFormat:@"rtmp://%@:1935/%@/%@",self.ipAddress,self.suffix,self.othersRoom] withOptions:options];
        //设置填充模式
        _player.scalingMode = IJKMPMovieScalingModeAspectFill;
        //设置播放视图
        _player.view.frame = self.bigView.bounds;
        [self.bigView addSubview:_player.view];
        //设置自动播放
        _player.shouldAutoplay = YES;
        [_player prepareToPlay];
    }
    return _player;
}
```
设置播放器播放通知监听
```
- (void)initPlayerObserver{
    //监听网络状态改变
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadStateDidChange:) name:IJKMPMoviePlayerLoadStateDidChangeNotification object:self.player];
    //监听播放网络状态改变
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playStateDidChange:) name:IJKMPMoviePlayerPlaybackStateDidChangeNotification object:self.player];
}
//网络状态改变通知响应
- (void)loadStateDidChange:(NSNotification *)notification{
    IJKMPMovieLoadState loadState = self.player.loadState;
    if ((loadState & IJKMPMovieLoadStatePlaythroughOK) != 0) {
        NSLog(@"LoadStateDidChange: 可以开始播放的状态: %d\n",(int)loadState);
    }else if ((loadState & IJKMPMovieLoadStateStalled) != 0) {
        NSLog(@"loadStateDidChange: IJKMPMovieLoadStateStalled: %d\n", (int)loadState);
    } else {
        NSLog(@"loadStateDidChange: ???: %d\n", (int)loadState);
    }
}
//播放状态改变通知响应
- (void)playStateDidChange:(NSNotification *)notification{
    switch (_player.playbackState) {
        case IJKMPMoviePlaybackStateStopped:
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: stoped", (int)_player.playbackState);
            break;
        case IJKMPMoviePlaybackStatePlaying:
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: playing", (int)_player.playbackState);
            break;
        case IJKMPMoviePlaybackStatePaused:
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: paused", (int)_player.playbackState);
            break;
        case IJKMPMoviePlaybackStateInterrupted:
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: interrupted", (int)_player.playbackState);
            break;
        case IJKMPMoviePlaybackStateSeekingForward:
        case IJKMPMoviePlaybackStateSeekingBackward: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: seeking", (int)_player.playbackState);
            break;
        }
        default: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: unknown", (int)_player.playbackState);
            break;
        }
    }
}
```
接着在viewDidLoad中调用方法并开始播放
```
- (void)viewDidLoad {
    [super viewDidLoad];
    ...
    //    播放端
    [self initPlayerObserver];
    [self.player play];
}
```
大功告成,现在只要传入正确的参数就能实现视频聊天啦╰(￣▽￣)╮
```
//
//  HBVideoChatViewController.h
//  视频聊天
//
//  Created by apple on 16/8/9.
//  Copyright © 2016年 yhb. All rights reserved.
//
#import <UIKit/UIKit.h>
@interface HBVideoChatViewController : UIViewController
/**
 *  创建视频聊天播放器
 *
 *  @param IPAddress  两个人共同所在的区域网
 *  @param myRoom     我的推流后缀地址(随便写,只要与别人的othersRoom相同即可)
 *  @param othersRoom 别人的推流地址
 *
 */
- (instancetype)initWithIPAddress:(NSString *)ipAddress MyRoom:(NSString *)myRoom othersRoom:(NSString *)othersRoom;
@end
```
## 辅助文件
在MainViewController中导入刚写的文件并设置一个alertView来传入参数
```
//
//  TestViewController.m
//  VideoChat
//
//  Created by apple on 16/8/10.
//  Copyright © 2016年 yhb. All rights reserved.
//
#import "MainViewController.h"
#import "HBVideoChatViewController.h"
@interface MainViewController ()
@property (nonatomic,copy) NSString *ipAddress;
@property (nonatomic,copy) NSString *myRoom;
@property (nonatomic, copy) NSString *othersRoom;
@end
@implementation MainViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self setButton];
}
- (void)setButton{
    UIButton *button = [UIButton buttonWithType:UIButtonTypeSystem];
    [button setTitle:@"跳转到视频聊天界面" forState:UIControlStateNormal];
    button.frame = CGRectMake(0, 0, 200, 50);
    button.center = self.view.center;
    [button addTarget:self action:@selector(buttonClick) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
}
- (void)buttonClick{
    //弹出输入框
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Info" message:@"请输入详细信息" preferredStyle:UIAlertControllerStyleAlert];
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"请输入区域网IP";
    }];
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"请输入你的房间号";
    }];
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"请输入对方的房间号";
    }];
    //点击确定按钮跳转界面
    UIAlertAction *sure = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        //                        @"192.168.15.32"
        //取到文本数据传值
        HBVideoChatViewController *viewController = [[HBVideoChatViewController alloc] initWithIPAddress:[alert.textFields[0] text] MyRoom:[alert.textFields[1] text] othersRoom:[alert.textFields[2] text]];
        [self.navigationController pushViewController:viewController animated:YES];
    }];
    //取消按钮
    UIAlertAction *cancel = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    [alert addAction:sure];
    [alert addAction:cancel];
    [self presentViewController:alert animated:YES completion:nil];
}
@end
```
现在用终端推下桌面测试下,模拟器没摄像头所以就没画面了╮(╯_╰)╭
在终端输入
`ffmpeg -f avfoundation -i "1" -vcodec libx264 -preset ultrafast -acodec libfaac -f flv rtmp://localhost:1935/rtmplive/home`
将自己的桌面推送到服务器上,然后运行模拟器,输入对应IP地址,效果如下
rtmp://localhost:1935/rtmplive/home是我本地的服务器,对应的是自己的IP,我的是192.168.15.30,见下图
> 
注意:用真机测试时,要确保手机wifi连接到所搭建服务器的区域网
![](http://upload-images.jianshu.io/upload_images/2100810-1f4ebadbae9402cf.gif?imageMogr2/auto-orient/strip)
视频聊天.gif
测试了下大概有3~5秒的延迟,现在在同一区域网下输入对方的房间号就可以实现视频聊天啦~
> 
完整项目:[Github](https://github.com/KaelYHB/VideoChat)
网络服务器(不一定可用):rtmp://60.174.36.89:1935/live/xxx
打包好的IJKPlayer:[https://pan.baidu.com/s/1o7Frs06](https://pan.baidu.com/s/1o7Frs06)
下载解压后直接拖进项目即可
文／SimonDeCft（简书作者）
原文链接：http://www.jianshu.com/p/1b57c02cf9e0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
