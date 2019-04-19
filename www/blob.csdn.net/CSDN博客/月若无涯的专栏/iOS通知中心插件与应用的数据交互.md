# iOS通知中心插件与应用的数据交互 - 月若无涯的专栏 - CSDN博客
2014年12月18日 12:09:27[月若无涯](https://me.csdn.net/u010124617)阅读数：1833
#### 通知中心插件 - 数据传递
> 
名字解释：
Widget：指通知中心的插件
Host：指插件的宿主，即通知中心的Today区域
Container：指插件所依托存在的应用 
##### Widget和Host数据共享
主要是利用协议下的 
-(void)widgetPerformUpdateWithCompletionHandler:(void(^)(NCUpdateResult))completionHandler
方法进行，根据系统的要求，进行相应的视图更新等处理。 
##### Widget和Container数据共享
    Widget和Container之间，数据的共享可以通过AppGroup配置后，对共用的存储空间内进行数据共享。 
主要步骤有 
1.在target中配置AppGroup
![](https://img-blog.csdn.net/20141218120458510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.利用NSUserDefault或者NSFileManager访问AppGroup共享区域的内容，如果是NSUserDefault需要用创建方法：
[[NSUserDefaultalloc]initWithSuiteName:groupID]
如果是NSFileManager需要使用下面的获取路径方法
[[NSFileManagerdefaultManager]containerURLForSecurityApplicationGroupIdentifier:groupID]
##### Widget和Container的实时数据交互
    按照苹果的设计来看，本意是不希望Widget和Container存在直接的实时数据交互的，但是如果我们真的有一些特殊需求，比如：
    当Container开启时，可以实时改变一些Widget中的内容；又或者在Widget中触发某个按钮之类的操作，希望可以实时影响应用的展示... 
    这里我想到一种利用他们之间的AppGroup存储可以共享使用，建立文件监视来变相达到目的的方法。查了一下iOS中目前好像没有相应的FileWatchDog类，因此自己做了一个，丢在github上，[链接FileWatchDog](https://github.com/YueRuo/YRFileWatchDog)
    共享数据的原理是：Widget和Container都对同一个文件，进行文件变动监视，这样另一方有什么要传递的数据或指令，可以通过写文件的方式来提醒另一方，达到变相的实时数据交互。
![](https://img-blog.csdn.net/20141218120836302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
##### PS：
网上看到过有人在Widget中利用NSURLSession来进行对Container的下载结束的提醒，这种也在特定条件下可以做一些事情，因此一并列在这里。
