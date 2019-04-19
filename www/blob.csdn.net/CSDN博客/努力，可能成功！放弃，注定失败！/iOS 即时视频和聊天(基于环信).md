# iOS 即时视频和聊天(基于环信) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月25日 10:06:45[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4016

先上效果图:
![](http://upload-images.jianshu.io/upload_images/271180-95dcd33d9f72fb53.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2015-07-30 下午5.19.46.png
说说需求:开发一个可以进行即时视频聊天软件.
- 
最近比较忙,考完试回到公司就要做这个即时通信demo.本来是打算用xmpp协议来做视频通信的,想了想要搞后台,还要搭建服务器.一开始没明白是怎么样的一种形式.(现在想了想,其实就是自己写个服务器,然后放在服务器上而已了""脑袋被驴踢了).让后问boss服务器是我自己写还是怎样?然后boss让我先做个环信的demo,搞完再搞xmpp.
![](http://upload-images.jianshu.io/upload_images/271180-738447d9a9e2d25a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
服务器的安装包
东西都下好了,打算下个星期再弄,想接触xmpp的同学可以了解一下.
- 
环信,什么鬼?
- 1.集成IOS SDK前的准备工作:
(如果需要推送消息,则要到苹果官网上制作证书,再到环信后台制作推送证书.
详细请看[http://www.easemob.com/docs/ios/IOSSDKPrepare/#registerDeveloper](http://www.easemob.com/docs/ios/IOSSDKPrepare/#registerDeveloper))
注册环信开发者账号并创建后台应用,
登陆地址:[https://console.easemob.com/?comeFrom=easemobHome](https://console.easemob.com/?comeFrom=easemobHome)
注册和登陆就不多说了,只介绍创建应用: 一般我们选择开放注册.
![](http://upload-images.jianshu.io/upload_images/271180-1dddbf42ba6c6d3c.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建应用
![](http://upload-images.jianshu.io/upload_images/271180-cecf5150801dde87.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
得到appkey
- 2.然后开始创建工程:
下载环信Demo及SDK：[http://www.easemob.com/sdk/](http://www.easemob.com/sdk/)
解压缩iOSSDK.zip后会得到以下目录结构：
![](http://upload-images.jianshu.io/upload_images/271180-caab5f8cb92c423b.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS_Example_layout_IOS.png.jpeg
将EaseMobSDK拖入到项目中,并添加SDK依赖库
![](http://upload-images.jianshu.io/upload_images/271180-c9c98270a35e53a5.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS_AddLibs.png.jpeg
添加 以后,在AppDelegate中注册SDK
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary     *)launchOptions
{//registerSDKWithAppKey:为应用标示,apnsCertName为推送证书;(如果没用推送证书,这里可以随便)
    [[EaseMob sharedInstance] registerSDKWithAppKey:@"easemob-demo#chatdemo" apnsCertName:apnsCertName];
    // 需要在注册sdk后写上该方法
    [[EaseMob sharedInstance] application:application didFinishLaunchingWithOptions:launchOptions];
    return YES;
}
```
3.下面我们开始主要界面的详解:
先上图吧.(因为视频通信需要两台真机,而我只能和iPhone对模拟器,所以没有视频图,但demo已经过测试可以视频聊天)
- - 
要进行对话,必须先有用户名,这样才能让对方找到你.(先注册,再登录.退出(用于测试))
![](http://upload-images.jianshu.io/upload_images/271180-a0f0c34d6cdc8d33.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
登录页面
登录页面的代码比较简单.
```
//异步注册账号
   [[EaseMob sharedInstance].chatManager asyncRegisterNewAccount:name.text
                                                       password:password.text
                                                 withCompletion:
   ^(NSString *username, NSString *password, EMError *error) {
       if (!error) {//注册成功,显示马上登录
           UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil
                                                           message:@"注册成功，请登陆"
                                                          delegate:nil
                                                 cancelButtonTitle:@"OK"
                                                 otherButtonTitles:nil];
           [alert show];
           alert = nil;
       }else{
          //输出错误信息.
       }
   } onQueue:nil];
//////////////////////////////////////////////////////////////////////
     // //异步登录账号
      [[EaseMob sharedInstance].chatManager asyncLoginWithUsername:name.text
                                                      password:password.text
                                                    completion:
   ^(NSDictionary *loginInfo, EMError *error) {
       if (loginInfo && !error) {
           //设置是否自动登录
           [[EaseMob sharedInstance].chatManager setIsAutoLoginEnabled:YES];
           // 旧数据转换 (如果您的sdk是由2.1.2版本升级过来的，需要家这句话)
           [[EaseMob sharedInstance].chatManager importDataToNewDatabase];
           //获取数据库中数据
           [[EaseMob sharedInstance].chatManager loadDataFromDatabase];
           //获取群组列表
           [[EaseMob sharedInstance].chatManager asyncFetchMyGroupsList];
       #warning 开始跳转,然后开始聊天
           NSLog(@"登录成功");
           TTAlertNoTitle(@"登录成功");
           [self thisToChatViewController];
           //发送自动登陆状态通知
           //[[NSNotificationCenter defaultCenter] postNotificationName:KNOTIFICATION_LOGINCHANGE object:@YES];
       }
       else
       {//输出错误信息.(太多,所以详见demo)
       }
   } onQueue:nil];
```
- 
2.登录后,有挺多的控件,
- 1.返回键:点击时会调用-(void)dealloc方法,用做登出操作.
- 2.UITextField: 你想要发送对象的名称,例如8080(下面将用8080做例子).
- 3.send:建立一个与8080的会话.
- 4.cheak:用于测试(现在没用了)
- 5.hehe黄色的lable: 当有收到消息的时候,文字内容就会发生改变,显示还有多少条未读信息.
- 6.最下面是UITableview,显示会话联系人.
![](http://upload-images.jianshu.io/upload_images/271180-a5d5ab0f171b12b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
聊天列表
```
首先要将代理设置好
    [[EaseMob sharedInstance].chatManager removeDelegate:self];
    //注册为SDK的ChatManager的delegate
    [[EaseMob sharedInstance].chatManager addDelegate:self delegateQueue:nil];
    [[EaseMob sharedInstance].callManager addDelegate:self delegateQueue:nil];
    //最后一个为即时通讯的代理,(即时视频,即时语音)
    当离开这个页面的时候,要讲代理取消掉,不然会造成别的页面接收不了消息.
    [[EaseMob sharedInstance].chatManager removeDelegate:self];
    [[EaseMob sharedInstance].callManager removeDelegate:self];
    这样以后,就可以使用代理方法来作一些事情了
    1.接收消息
    -(void)didReceiveMessage:(EMMessage *)message
    2. 未读消息数量变化回调
    -(void)didUnreadMessagesCountChanged
    3.实时通话状态发生变化时的回调,(如果没有实现这个函数,视频聊天邀请就会接收不到.)
    - (void)callSessionStatusChanged:(EMCallSession *)callSession changeReason:(EMCallStatusChangedReason)reason error:(EMError *)error
```
- 3.(已经更新)因为是demo,就先用UITextView来显示对话,在UITextField中输入文字,点击send,即可发送,对方发送过来的内容也可以看到.点击视频聊天,大概要等5-10s对方才能收到请求.
![](http://upload-images.jianshu.io/upload_images/271180-95dcd33d9f72fb53.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2015-07-30 下午5.19.46.png
```
发送消息
-(void) send:(UIButton *)sender{
//         conversation= [[EaseMob sharedInstance].chatManager conversationForChatter:@"ozxozx" conversationType:eConversationTypeChat];
    EMChatText *txtChat = [[EMChatText alloc] initWithText:sendContext.text];
    EMTextMessageBody *body = [[EMTextMessageBody alloc] initWithChatObject:txtChat];
    // 生成message
    EMMessage *message = [[EMMessage alloc] initWithReceiver:userName bodies:@[body]];
    message.messageType = eMessageTypeChat;
    EMError *error = nil;
    id <IChatManager> chatManager = [[EaseMob sharedInstance] chatManager];
//    [chatManager asyncResendMessage:message progress:nil];
    [chatManager sendMessage:message progress:nil error:&error];
    if (error) {
        UIAlertView * a = [[UIAlertView alloc] initWithTitle:@"error" message:@"发送失败" delegate:nil cancelButtonTitle:@"好" otherButtonTitles:nil, nil];
        [a show];
    }else {
        textview.text = [NSString stringWithFormat:@"%@\n\t\t\t\t\t我说:%@",textview.text,sendContext.text];
    }
}
```
```
//从会话管理者中获得当前会话.并将会话内容显示到textview中
-(void) addtext{
//1.
    EMConversation *conversation2 =  [[EaseMob sharedInstance].chatManager conversationForChatter:userName conversationType:0] ;
    NSString * context = @"";//用于制作对话框中的内容.(现在还没有分自己发送的还是别人发送的.)
    NSArray * arrcon;
    NSArray * arr;
    long long timestamp = [[NSDate date] timeIntervalSince1970] * 1000 + 1;//制作时间戳
    arr = [conversation2 loadAllMessages]; // 获得内存中所有的会话.
    arrcon = [conversation2 loadNumbersOfMessages:10 before:timestamp]; //根据时间获得5调会话. (时间戳作用:获得timestamp这个时间以前的所有/5会话)
// 2.
    for (EMMessage * hehe in arrcon) {
        id<IEMMessageBody> messageBody = [hehe.messageBodies firstObject];
        NSString *messageStr = nil;
//3.
        messageStr = ((EMTextMessageBody *)messageBody).text;
//        [context stringByAppendingFormat:@"%@",messageStr ];
        if (![hehe.from isEqualToString:userName]) {//如果是自己发送的.
            context = [NSString stringWithFormat:@"%@\n\t\t\t\t\t我说:%@",context,messageStr];
        }else{
            context = [NSString stringWithFormat:@"%@\n%@",context,messageStr];
        }
    }
    textview.text = context;
}
```
1 .EMConversation *conversation2 :会话对象,里面装着当前对话的双方的各种消息(EMMessage).
2 . EMMessage 消息.
```cpp
```cpp
一个message的内容(对方发来的)
{"messageId":"83265683047055820","messageType":0,"from":"ozx8899","bodies":
["{\"type\":\"txt\",\"msg\":\"反对党\"}"]
,"isAcked":true,"to":"ozxozx","timestamp":1436951601011,"requireEncryption":false}
```
```
3.`((EMTextMessageBody *)messageBody)即为"bodies":["{\"type\":\"txt\",\"msg\":\"反对党\"}"]即:消息为文本,信息内容为:反对党`
## 视频聊天
```
-(void)openTheVideo:(UIButton *)btn{
    BOOL isopen = [self canVideo];//判断能否打开摄像头,(太多,详见demo)
    EMError *error = nil;
    EMCallSession *callSession = nil;
    if (!isopen) {
        NSLog(@"不能打开视频");
        return ;
    }
    //这里发送异步视频请求
    callSession = [[EaseMob sharedInstance].callManager asyncMakeVideoCall:userName timeout:50 error:&error];
    //请求完以后,开始做下面的
    if (callSession && !error) {
        [[EaseMob sharedInstance].callManager removeDelegate:self];
        CallViewController *callController = [[CallViewController alloc] initWithSession:callSession isIncoming:NO];
        callController.modalPresentationStyle = UIModalPresentationOverFullScreen;
        [self presentViewController:callController animated:NO completion:nil];
    }
    if (error) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"error", @"error") message:error.description delegate:nil cancelButtonTitle:NSLocalizedString(@"ok", @"OK") otherButtonTitles:nil, nil];
        [alertView show];
        alertView = nil;
    }
}
```
视频聊天的代码主要在CallViewController中.如果需要改变视频通话时的界面,就要改此控制器中的布局内容.
最要值得主要的是在dealloc函数中一定要加下面两句
```
```
[[EaseMob sharedInstance].callManager removeDelegate:self];
[[NSNotificationCenter defaultCenter] postNotificationName:@"callControllerClose" object:nil];
```
```
1.注销callManager的代理 2.通知消息中心,即时对话已经完成了
如果没有第二句,那么你的程序就只能进行一次即时通讯.
并且要在CallViewController将要返回的控制器中加上函数,注册当前控制器为callManager的代理.
```
- (void)callControllerClose:(NSNotification *)notification
{
    [[EaseMob sharedInstance].callManager addDelegate:self delegateQueue:nil];
}
```
如果开发过程中遇到问题,不妨看看是不是两个主要的代理没有设定好:
[EaseMob sharedInstance].chatManager ; //这是会话管理者,获取该对象后, 可以做登录、聊天、加好友等操作
[EaseMob sharedInstance].callManager ;//这是即时通讯(语音聊天和视频聊天)的管理者.
demo可以在:[https://github.com/ouzhenxuan/huanxinDemo](https://github.com/ouzhenxuan/huanxinDemo) 下载
使用前,请到[http://www.easemob.com/downloads](http://www.easemob.com/downloads) 把iOS版的SDK下载到工程中,即可使用
简化版的demo：[https://github.com/ouzhenxuan/shipinchat](https://github.com/ouzhenxuan/shipinchat)
如果感觉对你有帮助,请点个star.最近十分需要.你的star是我工作的支持.
