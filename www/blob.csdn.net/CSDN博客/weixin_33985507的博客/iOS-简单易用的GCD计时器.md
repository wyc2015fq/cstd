# iOS-简单易用的GCD计时器 - weixin_33985507的博客 - CSDN博客
2018年11月24日 19:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
# 前言
  好久没更新文章了，还是给自己立一个flag每周至少更新一篇文章，可能文章的质量还不是很如意，希望通过写文章来提高自己文笔，以及记录自己学习中的遇到的问题解决方案。
  在学习iOS过程中，想定大家对于定时器都不陌生，在日常开发中总会碰到需要计时器的功能，常见的定时器有NSTimer、GCD、CADisplayLink。网上也有很多的教程介绍三者的区别，今天主要讲的是GCD这种方式使用以及封装。
### 三者概括区别
||优点|缺点|
|----|----|----|
|NSTimer|使用简单|受Runloop影响会导致计时不精准|
|CADisplayLink|精度高|CPU负载的时候会影响触发事件，且触发事件大于触发间隔会导致掉帧现象。|
|GCD|较精准|代码较多，基本不受其他影响|
总结：NSTimer和CADisplayLink易受影响，而GCD虽然代码多，但是可控性非常强。
## GCD
```
/** 获取一个全局的线程来运行计时器*/
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
/** 创建一个计时器*/
dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
/** 设置计时器, 这里是每10毫秒执行一次*/
dispatch_source_set_timer(timer, dispatch_walltime(nil, 0), 10*NSEC_PER_MSEC, 0);
/** 设置计时器的里操作事件*/
dispatch_source_set_event_handler(timer, ^{
    //do you want....
});
```
##### 开启、继续已暂停的定时器
```
dispatch_resume(timer);
```
##### 暂停定时器
```
/** 挂起的时候注意，多次暂停的操作会导致线程锁的现象，即多少次暂停，
*   对应多少次的继续操作，即dispatch_suspend和dispatch_resume
*   是成对出现的，计时器才会继续工作。
*/
dispatch_suspend(timer);
```
##### 结束定时器
```
dispatch_source_cancel(timer);
```
### 构思封装
###### 写代码之前构思好功能模块以及会遇到的问题的解决方案、代码逻辑，再来下手写代码，会有事半功倍的效果。
- 必然包含开始、暂停、继续、停止、重置功能
- 时间计算过程中因浮点数计算会丢失精度，计算过程应采用NSDecimal
- 时间转换考虑到精度以及便利性，采用系统的时间转换方法，时区置为GMT
- 由于APP进入后台，若未开启后台任务的开关，计时器将会停止，再次进入APP又会继续，故采用监听app状态的方式记录APP进入后台与前台的时间戳，并与截止时间相比，是否继续计时还是结束计时并回调。
- 计时器返回的结果若采用字符串则还需处理，故使用了一个时间类来把结果返回，可以进行自定义操作
- 倒计时的结果返回和结束通知采用闭包形式
### 部分代码
```
/** app进入后台*/
- (void)appDidEnterBackground{
    [self suspend];
    NSDate *date = [[NSDate alloc] init];
    NSDateFormatter *format = [[NSDateFormatter alloc] init];
    format.dateFormat = @"yyyy-MM-dd HH:mm:ss:SSS";
    self.appDidEnterBackgroundTime = [date timeIntervalSince1970];
}
/** app进入前台*/
- (void)appDidEnterForeground{
    NSDate *date = [[NSDate alloc] init];
    NSDateFormatter *format = [[NSDateFormatter alloc] init];
    format.dateFormat = @"yyyy-MM-dd HH:mm:ss";
    self.appDidEnterForegroundTime = [date timeIntervalSince1970];
    [self reCalculateMinder];
}
```
```
/** 不失精度加减乘除计算结果*/
- (NSDecimalNumber *)value: (NSTimeInterval)value 
                byOpration: (OMDecimalOprationType)byOpration       
                 percision:  (NSInteger)percision 
                withValue: (NSTimeInterval)withValue{
    NSDecimalNumber *number = [self numberValueWithString: value];
    NSDecimalNumber *withNumber = [self numberValueWithString: withValue];  
    NSDecimalNumberHandler *handler = [NSDecimalNumberHandler  decimalNumberHandlerWithRoundingMode: NSRoundPlain  scale: percision raiseOnExactness: NO  raiseOnOverflow: NO  raiseOnUnderflow: NO  raiseOnDivideByZero: YES];
switch (byOpration) {
    case  OMDecimalOprationTypeAdd:
      return [number decimalNumberByAdding: withNumber withBehavior:handler];
      break;
    case  OMDecimalOprationTypeSubtract:
      return [number decimalNumberBySubtracting: withNumber withBehavior: handler];
      break;
   case  OMDecimalOprationTypeDivide:
      return [number decimalNumberByDividingBy: withNumber withBehavior: handler];
      break;
  case  OMDecimalOprationTypeMultiple:
      return [number decimalNumberByMultiplyingBy: withNumber withBehavior: handler];
      break;
  default:
      return nil;
      break;
}
```
```
@property (nonatomic, strong) OMTimer *timer;
```
```
self.timer = [[OMTimer alloc] init];
self.timer.timerInterval = 30;
self.timer.precision = 100;
self.timer.isAscend = NO;
self.timer.progressBlock = ^(OMTime *progress) {
     NSLog(@"%@:%@:%@:%@", progress.hour, progress.minute, progress.second, progress.millisecond;
};
self.timer.completion = ^{
    NSLog(@"complete done!");
};
```
### Swift版本
  最近喜欢上了OC，如有小伙伴需要Swift的版本的话可以留言或者私我，可以在写个Swift版本，。
## 结语
  使用简单，只需要把OMTimer.h和OMTimer.m拖入你的工程即可，满足大数的场景，可倒计时亦可增加计时，全部代码已在Github<[https://github.com/oymuzi/OMKit/](https://github.com/oymuzi/OMKit/)>上面，如对你有帮助，希望得到你的一颗小星星✨，谢谢。
  如有疑问，直接留言或者发送邮件给我 [admin@mypup.cn](mailto:admin@mypup.cn)
