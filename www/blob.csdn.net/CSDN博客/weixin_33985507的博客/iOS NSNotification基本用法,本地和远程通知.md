# iOS NSNotification基本用法,本地和远程通知 - weixin_33985507的博客 - CSDN博客
2017年02月19日 00:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
### Notification Center的概念：
它是一个单例对象，允许当事件发生时通知一些对象，让对象做出相应反应。
它允许我们在低程度耦合的情况下，满足控制器与一个任意的对象进行通信的目的。 这种模式的基本特征是为了让其他的对象能够接收到某种事件传递过来的通知，主要使用通知名称来发送和接收通知。
基本上不用考虑其它影响因素，只需要使用同样的通知名称，监听该通知的对象（即观察者）再对通知做出反应即可。
#### 优势：
1.不需要编写多少代码，实现比较简单；
2.对于一个发出的通知，多个对象能够做出反应，简单实现1对多的方式，较之于 Delegate 可以实现更大的跨度的通信机制；
3.能够传递参数（object和userInfo），object和userInfo可以携带发送通知时传递的信息。
#### 缺点：
1.在编译期间不会检查通知是否能够被观察者正确的处理；
2.在释放通知的观察者时，需要在通知中心移除观察者；
3.在调试的时候，通知传递的过程很难控制和跟踪；
4.发送通知和接收通知时需要提前知道通知名称，如果通知名称不一致，会出现不同步的情况；
5.通知发出后，不能从观察者获得任何的反馈信息。
### <一>NSNotification 一般使用情况的的5种使用方式
[本文demo1](https://link.jianshu.com?t=http://git.oschina.net/leiming97/NSNotificationDemo)
#### 1、不传递参数, 最常用的一种
```
/*----------------------- 一 --------- 不传递参数, 最常用的一种 ------------------------------------*/
// 第一步:监听通知
- (void)ListeningToNotification1{
    //监听
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(noti1) name:@"noti1" object:nil];
}
// 第二部:发送通知
-(void)btn1Click{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"noti1" object:nil];
}
//调用方法
-(void)noti1{
    NSLog(@"接收 不带参数的消息");
}
```
#### 2、 使用监听一方的调用方法时 传递消息
//发通知
```
/*--------------- 二 ------ 使用监听一方的调用方法时 传递消息-------------*/
// 第一步:监听通知
- (void)ListeningToNotification2{
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(noti2:) name:@"noti2" object:nil];
}
// 第二部:发送通知(可以跨控制器进行发送)
-(void)btn2Click:(UIButton *)btn
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"noti2" object:[NSString stringWithFormat:@"%@",btn.titleLabel.text]];
}
//调用方法
-(void)noti2:(NSNotification *)noti
{
    //使用object处理消息
    NSString *info = [noti object];
    NSLog(@"接收 object传递的消息：%@",info);
}
```
#### 3、使用发送通知一方的 userInfo 传递消息
```
/*----------- 三 ----------- 使用发送通知一方的userInfo 传递消息  ----------*/
// 第一步:监听通知
-(void)ListeningToNotification3{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(noti3:) name:@"noti3" object:nil];
}
//第二步:发送通知
-(void)btn3Click{
    NSDictionary *dic = [NSDictionary dictionaryWithObject:@"wangLei在这里" forKey:@"param"];
    // 我把方法的object都设为了nil。那么这个参数起到什么作用呢？这个参数有点像二次确认的意思，就是在同一个通知name的情况下还可以通过object再次进行细分通知。就拿上面这个小demo说，如果object为空，接收方会接受所有名字为giveName的通知。但是如果object不为空，接收方就会只接收名字为giveName的而且object正确的通知
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"noti3" object:nil userInfo:dic];
}
//调用方法
-(void)noti3:(NSNotification *)noti
{
    //使用userInfo处理消息
    NSDictionary  *dic = [noti userInfo];
    NSString *info = [dic objectForKey:@"param"];
    NSLog(@"接收 userInfo传递的消息：%@",info);
}
// 最后一步:移除通知
-(void)dealloc
{
    //移除观察者，Observer不能为nil
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
```
#### 4 :使用block接收通知,不用再指定方法了
```
//第一步:接收通知
- (void)ListeningToNotification4{
[[NSNotificationCenter defaultCenter]addObserverForName:@"noti4" object:nil queue:nil usingBlock:^(NSNotification *not){
    //使用userInfo处理消息
    NSDictionary  *dic = [not userInfo];
    NSString *info = [dic objectForKey:@"param"];
    NSLog(@"使用block接收传递的消息：%@",info);
}];
}
//第二步:发送通知
-(void)btn4Click{
    NSDictionary *dic = [NSDictionary dictionaryWithObject:@"wangLei" forKey:@"param"];
    // 我把方法的object都设为了nil。那么这个参数起到什么作用呢？这个参数有点像二次确认的意思，就是在同一个通知name的情况下还可以通过object再次进行细分通知。就拿上面这个小demo说，如果object为空，接收方会接受所有名字为giveName的通知。但是如果object不为空，接收方就会只接收名字为giveName的而且object正确的通知
    [[NSNotificationCenter defaultCenter] postNotificationName:@"noti4" object:nil userInfo:dic];
}
```
#### 5、使用NSNotificationQueue实现通知的异步
// 第一步:注册通知
```
- (void)ListeningToNotification5{
    /**
     *  注册一个通知
     *
     */
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getNotification:) name:@"notificationMethon" object:nil];
    
}
```
//第二部: 发出通知
```
- (void)btn5Click{
    
//    [[NSNotificationCenter defaultCenter] postNotificationName:@"notificationMethon" object:nil userInfo:@{@"key":@"value"}];
    
    NSNotification *notifacation = [[NSNotification alloc]initWithName:@"notificationMethon" object:nil userInfo:@{@"key":@"value1"}];
    [[NSNotificationQueue defaultQueue] enqueueNotification:notifacation postingStyle:NSPostNow];
/*
我们可以通过，NSNotificationQueue的defaultQueue来获取到这个通知队列，然后调用enqueueNotification来发出通知，我们可以看到第二个参数postingStyle,这个参数是一个枚举，他可以是以下三个值：
typedef NS_ENUM(NSUInteger, NSPostingStyle) {
    NSPostWhenIdle = 1,
    NSPostASAP = 2,
    NSPostNow = 3
};
这三个不同的值是有一定区别的。（以下内容摘抄自网络）
尽快发送
以NSPostASAP风格进入队列的通告会在运行循环的当前迭代完成时被发送给通
告中心，如果当前运行循环模式和请求的模式相匹配的话（如果请求的模式和当
前模式不同，则通告在进入请求的模式时被发出）。由于运行循环在每个迭代过
程中可能进行多个调用分支（callout），所以在当前调用分支退出及控制权返回
运行循环时，通告可能被分发，也可能不被分发。其它的调用分支可能先发生
，比如定时器或由其它源触发了事件，或者其它异步的通告被分发了。
您通常可以将NSPostASAP风格用于开销昂贵的资源，比如显示服务器。如果在
运行循环的一个调用分支过程中有很多客户代码在窗口缓冲区中进行描画，在每
次描画之后将缓冲区的内容刷新到显示服务器的开销是很昂贵的。在这种情况
下，每个draw...方法都会将诸如“FlushTheServer” 这样的通告排入队列，并指定
按名称和对象进行聚结，以及使用NSPostASAP风格。结果，在运行循环的最
后，那些通告中只有一个被派发，而窗口缓冲区也只被刷新一次。
空闲时发送
以NSPostWhenIdle风格进入队列的通告只在运行循环处于等待状态时才被发
出。在这种状态下，运行循环的输入通道中没有任何事件，包括定时器和异步事
件。以NSPostWhenIdle风格进入队列的一个典型的例子是当用户键入文本、而
程序的其它地方需要显示文本字节长度的时候。在用户输入每一个字符后都对文
本输入框的尺寸进行更新的开销是很大的（而且不是特别有用），特别是当用户
快速输入的时候。在这种情况下，Cocoa会在每个字符键入之后，将诸
如“ChangeTheDisplayedSize”这样的通告进行排队，同时把聚结开关打开，并使
用NSPostWhenIdle风格。当用户停止输入的时候，队列中只有一
个“ChangeTheDisplayedSize”通告（由于聚结的原因）会在运行循环进入等待状
态时被发出，显示部分也因此被刷新。请注意，运行循环即将退出（当所有的输
入通道都过时的时候，会发生这种情况）时并不处于等待状态，因此也不会发出
通告。
立即发送
以NSPostNow风格进入队列的通告会在聚结之后，立即发送到通告中心。您可以
在不需要异步调用行为的时候 使用NSPostNow风格（或者通过
NSNotificationCenter的postNotification:方法来发送）。在很多编程环境下，我
们不仅允许同步的行为，而且希望使用这种行为：即您希望通告中心在通告派发
之后返回，以便确定观察者对象收到通告并进行了处理。当然，当您希望通过聚
结移除队列中类似的通告时，应该用enqueueNotification...方法，且使用
NSPostNow风格，而不是使用postNotification:方法。
*/
}
//
-(void)getNotification:(NSNotification *)info{
    NSDictionary *dict = info.userInfo;
    NSLog(@"当前执行任务的线程为%@",[NSThread currentThread]);
}
```
// 第三部:我们将调用发出通知的代码放在一个非主队列里面
```
NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [queue addOperationWithBlock:^{
        [self btn5Click];
        [NSThread sleepForTimeInterval:3];
        NSLog(@"1目前的queue所在的线程为%@",[NSThread currentThread]);
    }];
```
运行结果如下:
```
2017-02-20 16:14:53.217 NSNotificationDemo[6438:492325] 这是通知后面的代码,按照通知是同步的道理,应该在通知完成回调之后才会执行
2017-02-20 16:14:53.219 NSNotificationDemo[6438:493599] 这是通知里面执行的代码,这里的代码正常情况下要执行完之后,才能执行通知之后的代码;当前执行任务的线程为<NSThread: 0x600000077880>{number = 3, name = (null)}
2017-02-20 16:14:56.222 NSNotificationDemo[6438:493599] 1目前的queue所在的线程为<NSThread: 0x600000077880>{number = 3, name = (null)}
```
记住下面的话:
上面说到 NSNotificationCenter是一个同步操作，也就是只有当响应的通知的代码执行完毕以后，发出通知的对象的代码才会继续往下执行。
那么 NSNotificationQueue就有一些区别，他有两个非常重要的特点：即通告的聚结和异步发送。聚结是把和刚进入队列的通告相类似的其它通告从队列中移除的过程。如果一个新的通告和已经在队列中的通告相类似，则新的通告不进入队列，而所有类似的通告（除了队列中的第一个通告以外）都被移除。然而，您不应该依赖于这个特殊的聚结行为。
而异步发送则很好理解了，也就是说发出通知以后立刻返回，也就是是继续执行下面的代码，并不管通知发出后的具体情况
**总结NSNotificationCenter和NSNotificationQueue的区别，也许最大的一点就是发出通知时一个是同步一个是异步。**
#### 最后，记得在发送通知消息的页面，在dealloc方法里面移除观察者。
```
-(void)dealloc
{
//移除观察者，Observer不能为nil
[[NSNotificationCenter defaultCenter] removeObserver:self];
}
```
### <二>NSNotification 添加本地通知的使用方式
[demo2下载地址](https://link.jianshu.com?t=http://git.oschina.net/leiming97/nativeNtificationDemo)
<1>、本地通知栏通知
1.1、本地通知的创建和使用
本地通知其实在之前的文章和网上就已经很详细的说了，主要是UILocalNotification的使用，本地通知主要应用在固定时间的通知事件，比如日历、活动提醒等
```
NSLog(@"添加本地通知");
    //ios8.0以上的系统需要注册通知
    if (D_ISHight(8.0)) {
 //注册通知
  [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeSound|UIUserNotificationTypeBadge categories:nil]]; 
    }
    
    UILocalNotification *localNotification = [[UILocalNotification alloc] init];
    //设置固定时间通知
    if (sender.tag == LOCALNOTIFICATION1) {
        NSDateFormatter * forma = [[NSDateFormatter alloc] init];
        [forma setDateFormat:@"HH:mm"];
        localNotification.fireDate = [forma dateFromString:@"13:20"];   //12点提醒
        [localNotification setRepeatInterval:NSCalendarUnitDay];        //每天12点
    }
    //设置重复间隔时间
//    else if (sender.tag == LOCALNOTIFICATION1){
//        NSDate *date =[[NSDate alloc] init];
//        localNotification.fireDate = [date dateByAddingTimeInterval:5];//重复间隔的时长
//        [localNotification setRepeatInterval:NSCalendarUnitSecond];//时间间隔单位，秒
//    }
    
    [localNotification setTimeZone:[NSTimeZone defaultTimeZone]];   //时区
    //ios8.2以上的系统可以设置标题
    if (D_ISHight(8.2)) {
        [localNotification setAlertTitle:@"提醒标题"];
    }
    [localNotification setAlertBody:@"提醒内容：Damon"];
    [localNotification setAlertAction:@"锁屏时显示的动作标题"]; //在锁屏时显示的动作标题(完整测标题:"滑动来" + alertAction)
    [localNotification setApplicationIconBadgeNumber:1];    //设置提醒的软件右上角的小红点
    [localNotification setSoundName:UILocalNotificationDefaultSoundName];//默认声音
    //或者指定文件名localNotification.soundName = @"123.wav";
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];    //调用通知
```
在本地消息调用之后，就会在通知栏显示了，点击通知栏的消息之后，会自动打开这个软件，需要注意的是软件不同的状态，点击通知栏调用的函数也不同。
1、软件如果是在后台运行，并没有退出，调用的是AppDelegate的这个函数
```
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
```
2、软件已经主动退出，或者后台超过时间退出，点击通知栏消息之后就只会调用这个函数
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
```
3、所以不同的状态，调用的函数也不同，那么想获取是哪个通知调用的函数方法也不相同了，如果是后台运行状态下，想知道某个通知，可以通过不同的userInfo这样实现
```
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    NSLog(@"Application did receive local notifications");
    // 在这里写跳转代码
    // 如果是应用程序在前台,依然会收到通知,但是收到通知之后不应该跳转
    if (application.applicationState == UIApplicationStateActive)
    {
        return;
    }
    if (application.applicationState == UIApplicationStateInactive) {
        // 当应用在后台收到本地通知时执行的跳转代码
        //可以通过设置通知时的userinfo过滤某条通知
        if ([notification.userInfo[@"name"] isEqualToString:@"Damon"]) {
            NSLog(@"damon");
        }
        //可以得到所有的通知
        for (UILocalNotification *noti in [[UIApplication sharedApplication] scheduledLocalNotifications]) {
            NSLog(@"%@",noti.fireDate);
        }
        //通知之后可以取消对应的通知
        [[UIApplication sharedApplication] cancelLocalNotification:notification];
    }
    [self jump:1];
}
```
4、而如果app已经退出，那么在启动函数里面获取通知就需要通过launchOptions里面的UIApplicationLaunchOptionsLocalNotificationKey来实现，比如这样：
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    //当应用退出之后，点击通知跳转到应用会走这个函数，而不是didReceiveLocalNotification
    UILocalNotification * local = [launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
    if ([local.userInfo[@"name"] isEqualToString:@"Damon"]) {
        NSLog(@"damon");
        [self jump:2];
    }
    return YES;
}
```
转自:[http://www.hudongdong.com/ios/348.html](https://link.jianshu.com?t=http://www.hudongdong.com/ios/348.html)
