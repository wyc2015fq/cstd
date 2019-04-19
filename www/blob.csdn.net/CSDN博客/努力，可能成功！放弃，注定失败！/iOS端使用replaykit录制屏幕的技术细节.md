# iOS端使用replaykit录制屏幕的技术细节 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:42:49[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：196
![](https://upload-images.jianshu.io/upload_images/2204252-1cb56c0483770db9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
前端两篇文章：
[iOS端屏幕录制（replaykit）调研](https://www.jianshu.com/p/b8ce67fb08e1)
[iOS端屏幕录制Replaykit项目实践](https://www.jianshu.com/p/392777d1995c)
已经对iOS端实现屏幕录制的调研结果和简单实践进行了概述，本篇开始将分别对iOS9，iOS10，iOS11，iOS12系统上具体实践记录一下，便于分享和自己查看。
相比于安卓端，iOS装置的端的屏幕录制发展太慢了，并且对开发者的需求满足总是延迟很大，就像其他功能一样，这也许就是苹果逐渐丧失他的竞争力的原因。本文将对的的iOS端使用replaykit在各个系统版本中实现细节进行描述。
# iOS9：
对于iOS9的replaykit介绍功能参考可以[官方WWDC视频](https://developer.apple.com/videos/play/wwdc2015/605/?time=738)：支持录制音频，视频，还可以增加语音旁白评论等其他额外的定制化东西对于录制的内容，用户可以回访，剪辑或者通过社交媒体软件分享出去。
> 
[ReplayKit记录](https://developer.apple.com/videos/play/wwdc2015-605/?time=792)[正在运行的应用程序](https://developer.apple.com/videos/play/wwdc2015-605/?time=794)[的音频视觉状语从句：效果](https://developer.apple.com/videos/play/wwdc2015-605/?time=792)[。](https://developer.apple.com/videos/play/wwdc2015-605/?time=794)[它还允许您使用它来添加语音评论](https://developer.apple.com/videos/play/wwdc2015-605/?time=797)[，因此可以他们使他们的录音更个性化或只是](https://developer.apple.com/videos/play/wwdc2015-605/?time=799)[为了提供额外的主页背景。](https://developer.apple.com/videos/play/wwdc2015-605/?time=802)[它允许您的用户播放，](https://developer.apple.com/videos/play/wwdc2015-605/?time=804)[擦洗和修剪他们的录音，](https://developer.apple.com/videos/play/wwdc2015-605/?time=806)[并将名单最终他们的录音分享](https://developer.apple.com/videos/play/wwdc2015-605/?time=809)[到他们最喜欢的社交网络](https://developer.apple.com/videos/play/wwdc2015-605/?time=810)[状语从句：视频网站目的地。](https://developer.apple.com/videos/play/wwdc2015-605/?time=813)
启动录制使用接口：
```
[[RPScreenRecorder sharedRecorder] startRecordingWithMicrophoneEnabled:YES handler:^(NSError * _Nullable error) {
        ;
    }];
```
### 注意：
- 使用[RPScreenRecorder sharedRecorder]启动录制，会首先请求用户同意使用摄像头和麦克风，主要考虑用户的隐私和权限，如果用户拒绝了，将无法进行录制。
- 录制的内容不会包含系统的用户界面中，比如上方导航栏;
- 录制的内容会经过音视频编码，而不是原始的YUV或PCM数据;
- 录制的内容无法直接查看，必须通过RPPreviewViewController才能查看预览，或者分享，或者保存到本地相册中。而这个RPPreviewViewController在停止录制的接口回调中才能获取，也就是说，只有停止录制之后才能通过RPPreviewViewController操作录制的音视频。
```
[[RPScreenRecorder sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController *previewViewController, NSError *  error){
        
        [self presentViewController:previewViewController animated:YES completion:^{
            ;
        }];
    }];
```
预览的VC展示出来如下图：图中圈中位置分别提供了预览，保存到相册，分享三个入口。
![](https://upload-images.jianshu.io/upload_images/2204252-71a214b512221684.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/539/format/webp)
# iOS10：
··· 
iOS9已经实现了基本的app内容录制，预览，保存，分享，但是其输出的结果其实是一个已经将音频，视频编码并交织到一起成为一个mp4文件，开发者只能处理这个mp4文件，无法对原始音视频数据进行处理。对于有些应用可能存在诸如分辨率减小，码率减小，音频编辑等各种需求，都需要对原始的YUV，PCM数据进行处理，或者对编码过程进行定制化干预。
考虑到开发者这个需求，苹果在iOS10的replaykit中开放了这部分API，通过扩展形式将录制进程展现给开发者。其实iOS9时录制也是在一个独立于应用程序的进程中进行，只是未开放.iOS10提供了分发相关多个类和api，用户可以通过代理方法获取到屏幕录制的原始数据，做进一步处理。引入时需要通过xcode的文件 - > new - > target找到两个相关扩展：
![](https://upload-images.jianshu.io/upload_images/2204252-12afa0f81207fb3f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/709/format/webp)
### 录制
ios10的replaykit的录制已经跟iOS9差异很大，ios10已经支持录制的原始音视频数据的【实时】获取（iOS9只可以获取到录制停止后编码的MP4），开发者可以自己进行实时分发或者编码后处理。
主要步骤如下：
- 备选启动
界面：iOS10中由于录制作为一个外部的扩展，可以供所有系统中的应用程序使用，所以不能直接启动这个录制的进程需要首先启动支持录制的列表，通过下面接口
```
[RPBroadcastActivityViewController loadBroadcastActivityViewControllerWithHandler:^(RPBroadcastActivityViewController * _Nullable broadcastActivityViewController, NSError * _Nullable error) {
        
        self.broadcastAVC = broadcastActivityViewController;
        self.broadcastAVC.delegate = self;
        [self presentViewController:self.broadcastAVC animated:YES completion:nil];
    }];
```
这里我们设置代理，通过代理方法的回调我们才能启动录制进程。
![](https://upload-images.jianshu.io/upload_images/2204252-b4213f16a607612f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
- 反馈已完成配置
当我们点击了上图产品产品片中我们自己制作的扩展时，系统将会启动我们在创建扩展时其中一个目标对应的进程：xxxSetupUI进程，这个进程通常用于让用户输入一些信息来鉴权，或者自定义其他界面，在启动录制进程之间插入的一个交互的页面，当然也可以为空，但是不插入交互页面时，我们需要在相关进程中反馈信息：
```
#import "BroadcastSetupViewController.h"
@implementation BroadcastSetupViewController
- (void)userDidFinishSetup {
    NSURL *broadcastURL = [NSURL URLWithString:@"http://apple.com/broadcast/streamID"];
    NSDictionary *setupInfo = @{ @"broadcastName" : @"example" };
    // Tell ReplayKit that the extension is finished setting up and can begin broadcasting
    [self.extensionContext completeRequestWithBroadcastURL:broadcastURL setupInfo:setupInfo];
}
- (void)userDidCancelSetup {
    [self.extensionContext cancelRequestWithError:[NSError errorWithDomain:@"YourAppDomain" code:-1 userInfo:nil]];
}
- (void)viewDidLoad
{
}
- (void)viewWillAppear:(BOOL)animated
{
    [self userDidFinishSetup];
}
```
这里的BroadcastSetupViewController就在xxxSetupUI的目标中，是这个目标建立时自动生成的模板VC，我们可以在这里添加自定义方法来建立一个VC，添加视图，用于展示信息，或者用户鉴权，然后根据用户输入情况，决定是否让用户使用录制进程。
如果我们同意用户使用录制进程，这里我们主要需要告知调用的进程我们xxxSetupUI进程已经完成设置，可以开始广播了。其中的viewDidLoad中，viewWillAppear中中两个方法是我后填写的，这里主要是需要调用[self userDidFinishSetup]; 方法来完成通知调用方。
注意：
- 必须调用[self userDidFinishSetup]，调用进程里面的didFinishWithBroadcastController（下一步启动录制时用到）才能回调
- 必须在viewWillAppear中中中才能调用，在viewDidLoad中中中无法生效（都是坑啊......）
- 启动录制：
上一步，xxxSetupUI进程通过self.extensionContext将其扩展进程中的信息反馈回来，我们的RPBroadcastActivityViewController的代理方法将会回调：
```
- (void)broadcastActivityViewController:(RPBroadcastActivityViewController *)broadcastActivityViewController didFinishWithBroadcastController:(RPBroadcastController *)broadcastController error:(NSError *)error
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [broadcastActivityViewController dismissViewControllerAnimated:YES completion:nil];
    });
    
    self.broadcastController = broadcastController;
    [broadcastController startBroadcastWithHandler:^(NSError * _Nullable error) {
    }];
}
```
回调中我们需要首先将表界面解除。然后通过回调回来的broadcastController，调用接口启动录制，这里需要将broadcastController引用下来，用于我们在合适时机使用它结束录制。
- 接收原始音视频数据
上一步启动录制成功后，我们就可以在录制进程中接收到相关回调了，录制进程在目标创建时，模板生成了SampleHandler，其中已经复写了相关录制进行的方法：
```
@implementation SampleHandler
- (void)broadcastStartedWithSetupInfo:(NSDictionary<NSString *,NSObject *> *)setupInfo {
    // User has requested to start the broadcast. Setup info from the UI extension can be supplied but optional. 
}
- (void)broadcastPaused {
    // User has requested to pause the broadcast. Samples will stop being delivered.
}
- (void)broadcastResumed {
    // User has requested to resume the broadcast. Samples delivery will resume.
}
- (void)broadcastFinished {
    // User has requested to finish the broadcast.
}
- (void)processSampleBuffer:(CMSampleBufferRef)sampleBuffer withType:(RPSampleBufferType)sampleBufferType {
    
    switch (sampleBufferType) {
        case RPSampleBufferTypeVideo:
            // Handle video sample buffer
            break;
        case RPSampleBufferTypeAudioApp:
            // Handle audio sample buffer for app audio
            break;
        case RPSampleBufferTypeAudioMic:
            // Handle audio sample buffer for mic audio
            break;
        default:
            break;
    }
}
```
首先会回调到broadcastStartedWithSetupInfo方法，这里我们通常进行了一些初始化，例如进程间通知的监听等。下面的几个方法broadcastPaused，broadcastResumed，broadcastFinished表示了录制的进程变化，通常我们会在其中添加进程通知，通过源应用这些变化。最后的processSampleBuffer方法就是最终采集到的音频，视频原始数据。其中音频未做混音，包括麦克音频PCM和应用音频PCM，而视频输出为YUV数据。
注意：
- iOS10只支持的应用程序内容录制，所以当应用切到后台，录制内容将停止;
- 手机锁屏时，录制进程将停止;
- 这几个方法中的代码不能阻塞（例如写文件等慢操作），否则导致录制进程停止;
# iOS11：
到了iOS11时代，苹果终于开放了对录制内容的升级，从iOS10的应用内升级到整个系统级别的录制。但是对于隐私方面的考虑，苹果还是增加了很多用户使用门槛.iOS11中如果只是录制的应用程序内的内容，直接使用iOS10的方法即可，但是如果录制系统内容，则变化较多：
- 启动录制：
- 对于录制应用内容，iOS11增加了新接口，可以直接启动想要的录制进程，跳过中间列表片在点击选择的过程：
```
+ (void)loadBroadcastActivityViewControllerWithPreferredExtension:(NSString * _Nullable)preferredExtension handler:(nonnull void(^)(RPBroadcastActivityViewController * _Nullable broadcastActivityViewController, NSError * _Nullable error))handler API_AVAILABLE(ios(11.0)) API_UNAVAILABLE(tvos);
```
- 
对于录制系统内容，iOS11不允许开发直接调用api来启动系统界别的录制，必须是用户通过手动启动。启动方法很复杂：
用户点击进入手机设置页面 - >控制中心 - >自定义，找到屏幕录制的功能按钮，将其添加到上方：添加成功后，我们可以在手机上滑唤出控制界面中发现这个启动按钮：
![](https://upload-images.jianshu.io/upload_images/2204252-5792218282104da6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/872/format/webp)
![](https://upload-images.jianshu.io/upload_images/2204252-27502670e0541652.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)
注意：
在上方弹出的列表中，需要选择我们创建目标对应的应用程序图标，才能使用我们的录制进程进行采集。
- 通知启动应用程序：
由于iOS11录制的启动为手动操作，并且开发者启动录制进程的应用也无从知道是否已经启动，所以通常我们会在broadcastStartedWithSetupInfo中发出进程级通知，告知程序，录制已经启动。
- 结束录制：
从iOS11的接口设计上，我们推断结束估计也跟启动录制一样，不开放给开发者，所以起初我以为只能通过用户自己再次点击启动录制按钮，选择停止，才能主动停止录制，开发者无法干预这个过程，使用方法同启动录制类似，弹出列表中，直接点击下面的停止。
但是很明显，这种设计对用户体验影响很大，如果我们的应用程序已经停止了对采集的数据的显示或者分发，但是由于无法干预录制进程，那个进程将持续在工作，最直观体现在手机导航栏上方绿条（与手机通话时同样的机制），直到后来在RPBroadcastSampleHandler的方法里面发现了这个：
```python
```
/*! @abstract Method that should be called when broadcasting can not proceed due to an error. Calling this method will stop the broadcast and deliver the error back to the broadcasting app through RPBroadcastController's delegate.
    @param error NSError object that will be passed back to the broadcasting app through RPBroadcastControllerDelegate's broadcastController:didFinishWithError: method.
 */
- (void)finishBroadcastWithError:(NSError *)error;
```
```
这个方法就藏在上面列出的broadcastStartedWithSetupInfo，broadcastPaused，broadcastResumed，broadcastFinished等方法后面，被我误以为是一个录制状态的回调。那么在启动录制进程的应用程序中怎么使用这个finishBroadcastWithError方法来结束录制呢？
由于是手动启动录制进程，在启动录制进程的应用程序中，我们没有相关回调能获取到这个方法的RPBroadcastSampleHandler实例，所以无法直接启动。只能在录制进程中RPBroadcastSampleHandler实例自己调用，那么我们就可以通过进程通信的方法，前面已经介绍了启动录制时我们先注册进程通知，然后在收到进程通知时，我们调用[self finishBroadcastWithError：nil]; 即可，这里的错误入参，我们可以自定义一个字典，用于将错误信息展示进程结束时弹出的警告窗口中给用户。
# iOS12：
iOS11的复杂操作启动屏幕录制，不知道阻塞了多少用户的继续使用。进入到2018年的iOS12，苹果终于想通了，replaykit也迎来了柳暗花明，开发者企盼的API控制启动录制终于来了！
启动录制：
iOS12还是会考虑用户的感知性，要求开发者必须通过replaykit提供的RPSystemBroadcastPickerView来展示启动的视图，然后通过点击视图上面的按钮才能启动：
```cpp
```
#ifdef IPHONE_OS_VERSION_iOS12
        _broadPickerView = [[RPSystemBroadcastPickerView alloc] initWithFrame:CGRectMake(20, 5, 20, 20)];
        _broadPickerView.preferredExtension = @"com.cmcc.xiaoximeeting.ScreenRecordUpload";
        [self addSubview:_broadPickerView];
#endif
```
```
如上面代码，可以通过属性preferredExtension直接加载我们想要的录制进程。
优化：
虽然我们迎来更多自主控制权，但是悲催的是这里我们还是要等待弹出界面点击启动，才能开始录制。如果我们这个录制只是作为我们本身的应用程序的功能点，如何绕过这个点击操作呢？可以考虑用一些伎俩方式：
- 首先我们将_broadPickerView的帧合理设置，使其隐藏在某个按钮（通常是自定义的启动录制）后面;
- 当我们点击到这个按钮时，响应链会将点击也传递给这个_broadPickerView，那么这时我们可以再把点击传递给_broadPickerView上面的开始按钮：
```
- (void)clickedOnStartRecordButton:(UIButton *)sender
{
#ifdef IPHONE_OS_VERSION_iOS12
    if (sender.tag == TAG_SHARESCREEN)
    {
        for (UIView *view in _broadPickerView.subviews)
        {
            if ([view isKindOfClass:[UIButton class]])
            {
                [(UIButton*)view sendActionsForControlEvents:UIControlEventTouchDown];
            }
        }
    }
    else
    {
#endif
   // 其他逻辑代码
#ifdef IPHONE_OS_VERSION_iOS12
}
#endif
```
注意：
sendActionsForControlEvents：UIControlEventTouchDown传递的参数必须是UIControlEventTouchDown，我之前传的是upinside事件，一直失败，直到尝试了UIControlEventAllTouchEvents，发现可以成功，才发觉事件不对，逐个尝试其他事件后，才定位到是UIControlEventTouchDown。
- 当我们点击上层的按钮时，自动点击系统的_broadPickerView上面的开始录制按钮。
## 总结：
本文主要论述各个的的iOS系统版本使用replaykit实现屏幕的技术细节，其他需要考虑的点暂不详述，还包括：
- 屏幕方向变化，可以考虑使用RPVideoSampleOrientationKey对采集的YUV数据结构解析出来方向;
- 屏幕锁定的通知，虽然进程级通知提供了锁屏的通知，但是苹果商店的不允许使用，可以考虑使用的的appdelegate的代理方法来判断;
- 采集到数据结构中的YUV的缓存空间，不能占用（例如NSData的的的initWithBytesNoCopy方法虽然可以快速生成的NSData的，但是将占用这个缓存），否则将导致进程停止;
- 系统提供录制进程的内存空间约
作者：杭研融合通信的iOS 
链接：HTTPS：//www.jianshu.com/p/401b5b632d5b 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
