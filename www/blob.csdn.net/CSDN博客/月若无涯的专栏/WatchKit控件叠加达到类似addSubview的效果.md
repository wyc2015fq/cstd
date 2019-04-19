# WatchKit控件叠加达到类似addSubview的效果 - 月若无涯的专栏 - CSDN博客
2015年07月22日 11:12:26[月若无涯](https://me.csdn.net/u010124617)阅读数：1165标签：[watchkit开发																[子控件添加																[watchkit布局																[效果](https://so.csdn.net/so/search/s.do?q=效果&t=blog)](https://so.csdn.net/so/search/s.do?q=watchkit布局&t=blog)](https://so.csdn.net/so/search/s.do?q=子控件添加&t=blog)](https://so.csdn.net/so/search/s.do?q=watchkit开发&t=blog)
个人分类：[iOS开发																[WatchKit](https://blog.csdn.net/u010124617/article/category/5657757)](https://blog.csdn.net/u010124617/article/category/1393991)
最近在玩Watch，通过研究watch的布局后发现，其InterfaceController原生布局只有横向和竖向两种，且控件之间不能交叠，也无法addSubview那样添加子控件。
但是我在网易新闻中看到如下界面： 
![网易新闻截图](https://img-blog.csdn.net/20150722105510538)
很好奇他的标题是如何做到叠在图片上的，思考许久想到如下方案： 
动态生成相应的图片，直接处理出来。
这是我实现后的效果图： 
![这里写图片描述](https://img-blog.csdn.net/20150722105843432)
可以看到标题已经叠在了图片之上。
处理具体方法步骤为： 
1.在应用（手机中的那个应用）的AppDelegate的
`- (void)application:(UIApplication *)application handleWatchKitExtensionRequest:(NSDictionary *)userInfo reply:(void(^)(NSDictionary *replyInfo))reply`
方法中，创建imageView，并在做好合适的布局，add完相应的view之后的，利用截图功能，把view的截图作为图片发送给watch。这样这张图片看起来就像是添加了子控件的。
2.在watch端的InterfaceController中，拿到相应的图片和其他信息，做watch标准的布局。
```
[WKInterfaceController openParentApplication:userInfo reply:^(NSDictionary *replyInfo, NSError *error){
        //主应用处理完后的回调，返回extension所需的数据
        dispatch_async(dispatch_get_main_queue(), ^{
            NSString *words = [replyInfo objectForKey:@"words"];
            NSData *data = [replyInfo objectForKey:@"image"];
            [_image setImageData:data];
            [_label setText:words];
        });
    }];
```
其他的方案，也许在watch端直接处理也可以，不过性能上我个人偏向在手机端处理完提交过来更好一些，当然我们也要考虑watch和手机端数据传输的速度和电量消耗问题，尽量小一些，图片压缩一些。
写的比较简单，讲明了方法实现，具体的可以实践一下玩一玩，欢迎大家留言交流。
