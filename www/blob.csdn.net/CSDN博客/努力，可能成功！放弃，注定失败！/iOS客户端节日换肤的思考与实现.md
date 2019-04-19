# iOS客户端节日换肤的思考与实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月21日 14:38:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：37
最近单位的APP来了新的需求，市场说他们要在圣诞节把APP里的图标都换了，还要换背景图片，还要给部分view添加一个遮盖。对，就是换肤。以前没有搞过换肤，所以考虑了一天，感觉大概就是这么个思路，感觉有问题的大家可以一块讨论。
总体来说主要分为两个部分：
- 第一部分主要是服务器资源的远程加载：
	
![](https://upload-images.jianshu.io/upload_images/265821-9832fa7b12113b73.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/697/format/webp)
资源加载.png
	先说一下我们的要求是，一般在活动的前一天或前几天会提前把资源准备好放在服务器，然后后台开启换肤的接口，也就是说争取在活动开始前确保大多数用户已经把节日需要的资源下载到了本地，然后服务器接口返回会有此次活动的开始时间以及结束时间。然后在活动期间用户打开APP的话动态更换资源。暂时不考虑极限情况，例如用户在活动开始之前始终没有打开过APP，那么该用户当天打开APP第一次是不会看到最新的皮肤。
	流程图是刚开始考虑的时候按着当时的思路画的，具体实现的时候有些许偏差，不过总起来说就是这个逻辑。在APP初次打开时配置关于皮肤的一些参数信息`比如说我们默认把起始时间都定为1970年1月1日，把活动id定为0，以后递增`反正自我发挥，只要能确保与之后服务器的数据做对比，之后再次打开时只是去读取之前存好的值，在这里我会有一个换肤服务的一个单例，提供一个只读属性来标示现在是否需要更换资源。
```
@interface ChangeSkinService : NSObject
// 是否允许更换资源
@property (nonatomic, assign, readonly, getter=isShouldChangeSkin) BOOL shouldChangeSkin;
+ (instancetype)sharedInstance;
- (void)configService;
- (NSString *)resourceFile;
@end
```
如果服务器的版本高于本地的版本，则证明可能有换肤活动，这时需要判断服务器活动的结束时间，如果这个时间早于当前时间说明这次活动可能已经过期，那么客户端也就没有下载资源的必要了。如果这个时间晚于当前时间，则需要下载资源，这里可以给一个资源状态的标示，用来以防本地的版本已经跟服务器的同步而资源并没有下载成功的意外情况。事实上，我们可以一直等到资源下载并解压完成后再更改本地的所有设置，中间如果发生不可预料的错误，也可以避免客户端本地产生脏数据。
- 第二部分主要就是图片资源的加载了：
	
![](https://upload-images.jianshu.io/upload_images/265821-c40edbef1098dbd4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/719/format/webp)
图片加载.png
	这部分基本上没什么操作难点，无非就是运行时更改`+ (nullable UIImage *)imageNamed:(NSString *)name;`方法，重写该方法，在ChangeSkinService单例允许更换资源的情况下，去本地沙盒的Document文件夹中读取需要的图片。具体细节可以根据各自业务的不太来制定，大致思路是不变的，比如说你客户端有的地方的图片可能会有拉伸之类的需求，但这样其实是不影响的，因为图片拉伸的操作底层依然需要调用imageNamed:方法来找到该图片。
	所以，基本也就这样。如果你有问题也可以留言我们一起讨论。
> 
注：此文章首发在[简书](https://www.jianshu.com)转载请说明出处。
如果你想看到完整的代码，可以去[这里](https://link.jianshu.com?t=https://github.com/agenric/ChangeSkin)。
作者：Agenric
链接：https://www.jianshu.com/p/0651cfcd53dd
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
