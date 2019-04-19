# 时间控制组件-TimerTickerManager - game3108的专栏 - CSDN博客
2016年04月16日 20:48:49[game3108](https://me.csdn.net/game3108)阅读数：581标签：[ios																[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[iOS](https://blog.csdn.net/game3108/article/category/2844789)
简书地址：[http://www.jianshu.com/p/8351afd12e3c](http://www.jianshu.com/p/8351afd12e3c)
## 前言
计时器是经常使用的一个组件，比如说登陆操作的时候，点击获取验证码的按钮的时候，需要有一个30秒的倒计时，防止用户误点或者多次点击。 
因此写了个时间控制的组件，用来控制倒计时等操作，具体代码放在了[github](https://github.com/game3108/TimerTickerManager)上，
## 正文
在iOS里，定时操作主要有两个办法 
* NSTimer的`scheduledTimerWithTimeInterval`方法，设置时间间隔，执行相应的函数。 
使用NSTimer的话，要注意在该线程上是否有runloop，以及添加的Runloop Mode，不然容易出现滑动scrollview的时候，倒计时卡住的情况。
- GCD的dispatch source。使用`dispatch_source_set_timer`来设置定时器。
```bash
void
dispatch_source_set_timer(dispatch_source_t source,
 dispatch_time_t start,
 uint64_t interval,
 uint64_t leeway);
```
方法的四个参数，分别是： 
  - source：事件源 
  - start: 开始时间  
  - interval:间隔时间 
  - leeway:精度
配合上 
```
void 
  dispatch_source_set_event_handler(dispatch_source_t source, 
  dispatch_block_t handler);
```
  设置事件源的block回调，就可以完成定时器的设置。
## 代码讲解
#### TimerTicker 时间操作类
```
//
//  TimerTicker.h
//  TimerTickerManager
//
//  Created by game3108 on 16/4/15.
//  Copyright © 2016年 game3108. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
typedef NS_ENUM(NSInteger , TimerTickerType){
    TypeStop = 0,
    TypeOnFire = 1,
    TypePause =2,
    TypeNone = 3
};
@protocol TimerTickerDelegate <NSObject>
- (void) onTimerStop;
- (void) onTimerTicker:(CGFloat) ticker;
@end
@interface TimerTicker : NSObject
@property (nonatomic, assign) CGFloat endNumber;
@property (nonatomic, assign) CGFloat startNumber;
@property (nonatomic, assign) CGFloat tickerGap;
@property (nonatomic, assign) BOOL needStop;
@property (nonatomic, weak) id<TimerTickerDelegate> delegate;
- (dispatch_source_t)startTimeTicker;
- (void)stopTimerTicker;
- (void)pauseTimerTicker;
- (void)resumeTimerTicker;
- (TimerTickerType)getTimerTickerType;
@end
```
```
//
//  TimerTicker.m
//  TimerTickerManager
//
//  Created by game3108 on 16/4/15.
//  Copyright © 2016年 game3108. All rights reserved.
//
#import "TimerTicker.h"
@interface TimerTicker(){
    dispatch_source_t _timer;
    BOOL _onFire;
}
@end
@implementation TimerTicker
- (instancetype)init{
    self = [super init];
    if ( self ){
        _tickerGap = 1;
        _startNumber = 0;
        _endNumber = 0;
        _needStop = NO;
    }
    return self;
}
#pragma mark TimerTicker function
- (dispatch_source_t)startTimeTicker{
    if ( _timer ){
        if ( !_onFire )
            dispatch_resume(_timer);
        dispatch_source_cancel(_timer);
        _timer = nil;
    }
    CGFloat endNumber   = _endNumber;
    CGFloat startNumber = _startNumber;
    BOOL needStop       = _needStop;
    CGFloat tickerGap   = _tickerGap;
    BOOL timerTickerUp  = YES;
    if ( _tickerGap < 0 ){
        tickerGap = - _tickerGap;
        timerTickerUp = NO;
    }
    if ( (endNumber == startNumber) && needStop ){
        [_delegate onTimerStop];
        return nil;
    }
    if ( !_needStop ){
        endNumber = startNumber;
    }
    __block CGFloat timerTicker = (startNumber - endNumber) / tickerGap;
    if ( timerTicker > 0 ){
        timerTickerUp = NO;
    }
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    dispatch_source_set_timer(_timer, dispatch_walltime(NULL, 0), tickerGap*NSEC_PER_SEC, tickerGap*NSEC_PER_SEC*0.1);
    __weak TimerTicker *weakSelf = self;
    dispatch_source_set_event_handler(_timer, ^{
        BOOL judgeStop = [weakSelf judgeStop:timerTicker andTimeTickerUp:timerTickerUp andNeedStop:needStop];
        if(judgeStop) {
            [weakSelf onTimerStop];
        }else{
            [weakSelf onTimertTicker:timerTicker andTickerGap:tickerGap andEndNumber:endNumber];
            if ( timerTickerUp )
                timerTicker++;
            else
                timerTicker--;
            //NSLog(@"!!!!!!!!!%f",timerTicker);
        }
    });
    dispatch_resume(_timer);
    _onFire = YES;
    return _timer;
}
- (void)stopTimerTicker{
    if ( _timer ){
        if ( !_onFire )
            dispatch_resume(_timer);
        dispatch_source_cancel(_timer);
        _timer = nil;
    }
}
- (void)pauseTimerTicker{
    if ( _timer && _onFire ){
        dispatch_suspend(_timer);
        _onFire = NO;
    }
}
- (void)resumeTimerTicker{
    if ( _timer && !_onFire ){
        dispatch_resume(_timer);
        _onFire = YES;
    }
}
- (TimerTickerType)getTimerTickerType{
    if ( !_timer )
        return TypeStop;
    else if ( _onFire )
        return TypeOnFire;
    else
        return TypePause;
}
#pragma mark startTimeTicker related function
- (BOOL) judgeStop:(CGFloat)timerTicker andTimeTickerUp:(BOOL)ifUp andNeedStop:(BOOL)needStop{
    BOOL state1 = needStop && ifUp && timerTicker >=0;
    BOOL state2 = needStop && !ifUp && timerTicker <= 0;
    return state1 || state2;
}
- (void)onTimerStop{
    dispatch_source_cancel(_timer);
    dispatch_sync(dispatch_get_main_queue(), ^{
        [_delegate onTimerStop];
    });
}
- (void)onTimertTicker:(CGFloat)timerTicker andTickerGap:(CGFloat)tickerGap andEndNumber:(CGFloat)endNumber{
    dispatch_sync(dispatch_get_main_queue(), ^{
        [_delegate onTimerTicker:timerTicker*tickerGap+endNumber];
    });
}
#pragma mark dealloc
-(void)dealloc{
    [self stopTimerTicker];
    NSLog(@"timer ticker dealloc");
}
@end
```
TimerTicker类是直接进行时间控制的操作类。 
- startNumber:开始数字 
- endNumber:结束数字 
- tickerGap:时间间隔精度 
- needStop:代表是否停止，设置为YES，则endNumber会无效 
- delegate:回调 
函数调用包括： 
- startTimeTicker：开始计时 
- stopTimerTicker：结束计时 
- pauseTimerTicker： 暂停计时 
- resumeTimerTicker：重新计时 
- getTimerTickerType: 获取当前类型
`startTimeTicker`是主要的开始方法。 
具体逻辑不是那么复杂，应该可以看得懂，但写的时候碰到很多坑，具体的坑会在后面写出来。
#### TimerTickerManager控制类
TimerTickerManager主要提供给外部API，简化使用。
```
//
//  TimerTickerManager.h
//  TimerTickerManager
//
//  Created by game3108 on 16/4/16.
//  Copyright © 2016年 game3108. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "TimerTicker.h"
@interface TimerTickerManager : NSObject
+ (instancetype) sharedInstrance;
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
         withStartNumber:(CGFloat)startNumber
           withEndNumber:(CGFloat)endNumber
           withTickerGap:(CGFloat)tickerGap;
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
         withStartNumber:(CGFloat)startNumber
           withTickerGap:(CGFloat)tickerGap;
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
           withTickerGap:(CGFloat)tickerGap;
- (void) setTimerTickerDelegate:(id<TimerTickerDelegate>) delegate
                        withKey:(NSString *)key;
- (void) removeTimerTickerDelegate:(id<TimerTickerDelegate>) delegate
                           withKey:(NSString *)key;
- (void) removeTimerTickerWithKey:(NSString *)key;
- (void) setTimerTickerStop:(NSString *)key;
- (void) setTimerTickerPause:(NSString *)key;
- (void) setTimerTickerResume:(NSString *)key;
- (BOOL) judgeExistTimerTicker:(NSString *)key;
- (TimerTickerType)getTimerTickerType:(NSString *)key;
@end
```
```
//
//  TimerTickerManager.m
//  TimerTickerManager
//
//  Created by game3108 on 16/4/16.
//  Copyright © 2016年 game3108. All rights reserved.
//
#import "TimerTickerManager.h"
@interface TimerTickerManager(){
    NSMutableDictionary *_timerTickerDic;
}
@end
static TimerTickerManager *instrance = nil;
@implementation TimerTickerManager
+ (void)initialize{
    instrance = [[TimerTickerManager alloc]init];
    instrance->_timerTickerDic = [[NSMutableDictionary alloc]init];
}
+ (instancetype) sharedInstrance{
    return instrance;
}
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
         withStartNumber:(CGFloat)startNumber
           withEndNumber:(CGFloat)endNumber
           withTickerGap:(CGFloat)tickerGap{
    [self startTimerTicker:delegate
                   withKey:key
           withStartNumber:startNumber
             withEndNumber:endNumber
             withTickerGap:tickerGap
              withNeedStop:YES];
}
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
         withStartNumber:(CGFloat)startNumber
           withTickerGap:(CGFloat)tickerGap{
    [self startTimerTicker:delegate
                   withKey:key
           withStartNumber:startNumber
             withEndNumber:startNumber
             withTickerGap:tickerGap
              withNeedStop:NO];
}
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
           withTickerGap:(CGFloat)tickerGap{
    [self startTimerTicker:delegate
                   withKey:key
           withStartNumber:0
             withEndNumber:0
             withTickerGap:tickerGap
              withNeedStop:NO];
}
- (void)startTimerTicker:(id<TimerTickerDelegate>)delegate
                 withKey:(NSString *)key
         withStartNumber:(CGFloat)startNumber
           withEndNumber:(CGFloat)endNumber
           withTickerGap:(CGFloat)tickerGap
            withNeedStop:(BOOL)needStop{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker ){
        objcTimerTicker.tickerGap = tickerGap;
        objcTimerTicker.startNumber = startNumber;
        objcTimerTicker.endNumber = endNumber;
        objcTimerTicker.needStop = needStop;
        objcTimerTicker.delegate = delegate;
        [objcTimerTicker startTimeTicker];
        NSLog(@"timeticker reuse ticker");
    }else{
        TimerTicker *timerTicker = [[TimerTicker alloc]init];
        timerTicker.tickerGap = tickerGap;
        timerTicker.startNumber = startNumber;
        timerTicker.endNumber = endNumber;
        timerTicker.needStop = needStop;
        timerTicker.delegate = delegate;
        [_timerTickerDic setObject:timerTicker forKey:key];
        [timerTicker startTimeTicker];
        NSLog(@"timeticker new ticker");
    }
}
- (void) setTimerTickerDelegate:(id<TimerTickerDelegate>) delegate
                        withKey:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        objcTimerTicker.delegate = delegate;
}
- (void) removeTimerTickerDelegate:(id<TimerTickerDelegate>) delegate
                           withKey:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        objcTimerTicker.delegate = nil;
}
- (void) removeTimerTickerWithKey:(NSString *)key{
    [_timerTickerDic removeObjectForKey:key];
}
- (void) setTimerTickerStop:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        [objcTimerTicker stopTimerTicker];
}
- (void) setTimerTickerPause:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        [objcTimerTicker pauseTimerTicker];
}
- (void) setTimerTickerResume:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        [objcTimerTicker resumeTimerTicker];
}
- (BOOL) judgeExistTimerTicker:(NSString *)key{
    return [[_timerTickerDic allKeys] containsObject:key];;
}
- (TimerTickerType)getTimerTickerType:(NSString *)key{
    TimerTicker *objcTimerTicker = [_timerTickerDic objectForKey:key];
    if ( objcTimerTicker != nil )
        return [objcTimerTicker getTimerTickerType];
    else
        return TypeNone;
}
@end
```
## TimerTickerManger使用注意：
1.设置时间方式 
默认的`startNumber`开始时间为0，`tickerGap`时间间隔为1，使用没有`endNumber`的调用，就不会停止，`tickerGap`大于0为增加，小于0为减少。 
但如果使用有`endNumber`的方法，`startNumber`小于`endNumber`则一定是倒数，大于则一定是正数，与`tickerGap`正负无关。 
`tickerGap`为0则没有效果。 
如
```
[[TimerTickerManager sharedInstrance]startTimerTicker:self
                                                  withKey:@"key1"
                                          withStartNumber:30
                                            withEndNumber:20
                                            withTickerGap:0.5];
```
即从30倒计时到20，间隔为0.5，不停触发delegate回调
2.为什么会有`setTimerTickerStop`还会有`removeTimerTickerDelegate`方法？ 
比如注册界面按获取验证码倒计时30秒操作，如果用户触发完30秒倒计时，退出注册界面，这个时候倒计时应该继续进行，如果用户重新打开注册页面，要从上次的倒计时继续进行，所以会有`setTimerTickerDelegate`,`removeTimerTickerDelegate`,`getTimerTickerType`。
## 一些坑
1.关于循环应用问题 
`dispatch_source_set_event_handler`回调是一个block，所以很容易会出现循环引用问题。
`__weak TimerTicker *weakSelf = self;`
这个weak也主要为了防止循环引用。但写的一开始的版本还是有循环引用问题，后来检查了才发现，原来是在block内引用了实例变量，比如说 
`_startNumber`等。很多时候也许`self.startNumber`的形式更容易让人意识到循环引用问题，但用实例变量`_startNumber`的形式也会有循环引用，block持有self。 
2.关于`dispatch_suspend`与`dispatch_resume`
首先`dispatch_suspend`与`dispatch_resume`在对主线程没有效果。其次，这两个方法对应，拿Apple的注释来说
> 
Calls to dispatch_suspend() must be balanced with calls 
  to dispatch_resume().
你调用了suspend几次，你想resume的话，就必须要remuse几次，才能继续运行。但remuse的状态下，如果再进行一次resume就会crash，所以要注册一个onFire的状态进行记录，防止多次suspend和resume。
除此以外，在suspend的状态下，如果你设置_timer = nil就会crash，所以必须先resume，再cancel再设为nil，如下：
```
- (void)stopTimerTicker{
    if ( _timer ){
        if ( !_onFire )
            dispatch_resume(_timer);
        dispatch_source_cancel(_timer);
        _timer = nil;
    }
}
```
#### 参考资料
1.[GCD介绍（三）: Dispatch Sources](http://www.cnblogs.com/zhw511006/archive/2012/04/06/2434714.html)]([http://www.cnblogs.com/zhw511006/archive/2012/04/06/2434714.html](http://www.cnblogs.com/zhw511006/archive/2012/04/06/2434714.html))
