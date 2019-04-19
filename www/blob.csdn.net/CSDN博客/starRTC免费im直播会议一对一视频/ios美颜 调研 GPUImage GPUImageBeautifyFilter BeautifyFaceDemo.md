# ios美颜 调研 GPUImage GPUImageBeautifyFilter  BeautifyFaceDemo - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月07日 14:35:52[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2076
最近需要给直播项目中添加美颜的功能，调研了很多SDK和开源代码（视决，涂图，七牛，金山云，videoCore等），综合成本/效果/对项目侵入性，最后决定使用一款基于[GPUImage](https://github.com/BradLarson/GPUImage)实现的 [BeautifyFaceDemo](https://github.com/Guikunzhi/BeautifyFaceDemo)美颜滤镜。
**集成GPUImageBeautifyFilter和GPUImage Framework**
首先需要集成好[GPUImage](https://github.com/BradLarson/GPUImage)，通过观察目前iOS平台，90%以上美颜方案都是基于这个框架来做的。
原来项目中的AVCaptureDevice需要替换成GPUImageVideoCamera，删除诸如AVCaptureSession/AVCaptureDeviceInput/AVCaptureVideoDataOutput这种GPUImage实现了的部分。修改一些生命周期，摄像头切换，横竖屏旋转等相关逻辑，保证前后行为统一。
声明需要的属性
@property
 (nonatomic,
strong) GPUImageVideoCamera *videoCamera;
//屏幕上显示的View@property (nonatomic,
strong) GPUImageView *filterView;
//BeautifyFace美颜滤镜@property (nonatomic,
strong) GPUImageBeautifyFilter *beautifyFilter;
然后初始化
self.sessionPreset
 = AVCaptureSessionPreset1280x720;
self.videoCamera = [[GPUImageVideoCamera alloc] initWithSessionPreset:self.sessionPreset
 cameraPosition:AVCaptureDevicePositionBack];
self.filterView = [[GPUImageView alloc] init]; [self.view
 insertSubview:self.filterView atIndex:1];
//省略frame的相关设置//这里我在GPUImageBeautifyFilter中增加个了初始化方法用来设置美颜程度intensityself.beautifyFilter = [[GPUImageBeautifyFilter alloc] initWithIntensity:0.6];
为filterView增加美颜滤镜
[self.videoCamera
addTarget:self.beautifyFilter];[self.beautifyFilter
addTarget:self.filterView];
然后调用startCameraCapture方法就可以看到效果了
[self.videoCamera startCameraCapture];
到这里，仅仅是屏幕显示的内容带有滤镜效果，而作为直播应用，还需要输出带有美颜效果的视频流：
**输出带有美颜效果的视频流**
刚开始集成的时候碰见一个坑，原本的逻辑是实现AVCaptureVideoDataOutputSampleBufferDelegate方法来获得原始帧
- (void) captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
 fromConnection:(AVCaptureConnection *)connection;
而GPUImageVideoCamera也实现了一个类似的代理：
@protocol GPUImageVideoCameraDelegate
 <NSObject>@optional- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;@end
而替换之后发现输出的流依旧是未经美颜的图像，看了实现后发现果不其然，GPUImageVideoCameraDelegate还是通过AVCaptureVideoDataOutputSampleBufferDelegate直接返回的数据，所以想输出带有滤镜的流这里就得借助GPUImageRawDataOutput了
CGSize outputSize =
 {720,
1280};GPUImageRawDataOutput *rawDataOutput = [[GPUImageRawDataOutput alloc] initWithImageSize:CGSizeMake(outputSize.width,
 outputSize.height) resultsInBGRAFormat:YES];[self.beautifyFilter addTarget:rawDataOutput];
这个GPUImageRawDataOutput其实就是beautifyFilter的输出工具，可在setNewFrameAvailableBlock方法的block中获得带有滤镜效果的数据
__weak
 GPUImageRawDataOutput *weakOutput = rawDataOutput;__weaktypeof(self)
 weakSelf = self;[rawDataOutput setNewFrameAvailableBlock:^{ __strong
 GPUImageRawDataOutput *strongOutput = weakOutput; [strongOutput lockFramebufferForReading];
// 这里就可以获取到添加滤镜的数据了 GLubyte *outputBytes = [strongOutput rawBytesForImage];
NSInteger bytesPerRow = [strongOutput bytesPerRowInOutput]; CVPixelBufferRef pixelBuffer
 = NULL; CVPixelBufferCreateWithBytes(kCFAllocatorDefault, outputSize.width, outputSize.height, kCVPixelFormatType_32BGRA, outputBytes, bytesPerRow, nil, nil, nil, &pixelBuffer);
// 之后可以利用VideoToolBox进行硬编码再结合rtmp协议传输视频流了 [weakSelf encodeWithCVPixelBufferRef:pixelBuffer];
 [strongOutput unlockFramebufferAfterReading]; CFRelease(pixelBuffer);}];
经过和其他产品对比，GPUImageBeautifyFilter磨皮效果和花椒最为类似。这里采用双边滤波， 花椒应该用了高斯模糊实现。同印客对比，美白效果一般。
**关于性能的问题：**
1 调用setNewFrameAvailableBlock后很多机型只能跑到不多不少15fps
2 在6s这代机型上温度很高，帧率可到30fps但不稳定
关于性能问题，最近把项目中集成的美颜滤镜(BeautifyFace)里用到的 GPUImageCannyEdgeDetectionFilter 替换为 GPUImageSobelEdgeDetectionFilter 会有很大改善，而且效果几乎一致，6s经过长时间测试没有再次出现高温警告了。(替换也十分简单，直接改俩处类名/变量名就可以了)
最近发现当开启美颜的时候，关闭直播内存竟然没有释放。分析得出GPUImageRawDataOutput的setNewFrameAvailableBlock方法的block参数仍然保持着self，解决思路就是将GPUImageRawDataOutput移除。
先附上之前的相关release代码：
[self.videoCamera stopCameraCapture];
 [self.videoCamera removeInputsAndOutputs]; [self.videoCamera removeAllTargets];
开始以为camera调用removeAllTargets会把camera上面的filter，以及filter的output一同释放，但实际camera并不会'帮忙'移除filter的target，所以需要添加：
[self.beautifyFilter
 removeAllTargets]; //修复开启美颜内存无法释放的问题
关闭美颜output是直接加在camera上，camera直接removeAllTargets就可以；
开启美颜output加在filter上，camera和filter都需要removeAllTargets。
[http://www.jianshu.com/p/dde412cab8db](http://www.jianshu.com/p/dde412cab8db)
**基于GPUImage的实时美颜滤镜**
实现美颜滤镜的原理和思路。
[GPUImage](https://github.com/BradLarson/GPUImage) 是一个开源的基于GPU的图片或视频的处理框架，其本身内置了多达120多种常见的滤镜效果。有了它，添加实时的滤镜只需要简单地添加几行代码。下面的例子是以摄像头的数据为源，对其实时地进行反色的操作(类似相片底片的效果)：
利用GPUImage对摄像头数据添加滤镜的示例2.1
美颜也是一样，如果有这么一个美颜的滤镜（姑且叫做GPUImageBeautifyFilter)，那么只需要把示例2.1中的GPUImageColorInvertFilter替换成GPUImageBeautifyFilter即可。我们只需要做一个GPUImageBeautifyFilter就能实现实时美颜了，问题来了，到底什么算是美颜呢？我的理解是，大家对于美颜比较常见的需求就是磨皮、美白。当然提高饱和度、提亮之类的就根据需求而定。本文将着重介绍磨皮的实现（实际上GPUImageBeautifyFilter也实现了美白、提亮等效果）。
**磨皮**
     磨皮的本质实际上是模糊。而在图像处理领域，模糊就是将像素点的取值与周边的像素点取值相关联。而我们常见的[高斯模糊](https://zh.wikipedia.org/wiki/%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A) ，它的像素点取值则是由周边像素点求加权平均所得，而权重系数则是像素间的距离的高斯函数，大致关系是距离越小、权重系数越大。下图3.1是高斯模糊效果的示例:
高斯模糊效果示例3.1
 如果单单使用高斯模糊来磨皮，得到的效果是不尽人意的。原因在于，高斯模糊只考虑了像素间的距离关系，没有考虑到像素值本身之间的差异。举个例子来讲，头发与人脸分界处（颜色差异很大，黑色与人皮肤的颜色），如果采用高斯模糊则这个边缘也会模糊掉，这显然不是我们希望看到的。而[双边滤波(Bilateral
 Filter)](https://en.wikipedia.org/wiki/Bilateral_filter) 则考虑到了颜色的差异，它的像素点取值也是周边像素点的加权平均，而且权重也是高斯函数。不同的是，这个权重不仅与像素间距离有关，还与像素值本身的差异有关，具体讲是，像素值差异越小，权重越大，也是这个特性让它具有了保持边缘的特性，因此它是一个很好的磨皮工具。下图3.2是双边滤波的效果示例：
双边滤波效果示例3.2
     对比3.1和3.2，双边滤波效果确实在人脸细节部分保留得更好，因此我采用了双边滤波作为磨皮的基础算法。双边滤波在GPUImage中也有实现，是GPUImageBilateralFilter。
    根据图3.2，可以看到图中仍有部分人脸的细节保护得不够，还有我们并不希望将人的头发也模糊掉（我们只需要对皮肤进行处理）。由此延伸出来的改进思路是结合双边滤波，边缘检测以及肤色检测。整体逻辑如下：
磨皮处理逻辑图3.3
     Combination  Filter是我们自己定义的三输入的滤波器。三个输入分别是原图像A(x,
 y),双边滤波后的图像B(x, y），边缘图像C(x, y)。其中A,B,C可以看成是图像矩阵，(x,y)可以看成其中某一像素的坐标。Combination  Filter的处理逻辑如下图：
Combination Filter逻辑图3.3
     下面是主要的shader代码：
combination filter的shader代码3.4
     Combination Filter通过肤色检测和边缘检测，只对皮肤和非边缘部分进行处理。下面是采用这种方式进行磨皮之后的效果图:
最终磨皮效果图3.5
     对比3.5与3.2，可以看到3.5对人脸细节的保护更好，同时对于面部磨皮效果也很好，给人感觉更加真实。
所采用的磨皮算法是基于双边滤波的，主要是考虑到它同时结合了像素间空间距离以及像素值本身的差异。当然也不一定要采用双边滤波，也有通过改进高斯模糊（结合像素值差异）来实现磨皮的，甚至能取得更好的效果。另外GPUImageBeautifyFilter不仅仅具有磨皮功能，也实现了log曲线调色，亮度、饱和度的调整，具体详情可以参见[demo](https://github.com/Guikunzhi/BeautifyFaceDemo) 。
Faceu贴纸效果
Faceu贴纸效果也是基于GPUImage实现的，demo我放在了[GitHub](https://github.com/Guikunzhi/YLFaceuDemo)上。
**原理**
       Faceu贴纸效果其实就是在人脸上贴一些图片，同时这些图片是跟随着人脸的位置改变的。如果我们不强调贴图的位置，这就是一个简单的水印需求。
Faceu原理.png
       根据人脸检测的结果动态调整水印贴纸的位置即可实现简单的Faceu效果。
**水印**
       在GPUImage的官方demo中就已经有文字水印的实现：
GPUImageFilter *filter = [[GPUImageFilter alloc] init];
 [self.videoCamera addTarget:filter]; GPUImageAlphaBlendFilter *blendFilter
= [[GPUImageAlphaBlendFilter alloc] init];blendFilter.mix
=
1.0;
 NSDate *startTime = [NSDate date]; UIView *temp = [[UIView alloc] initWithFrame:self.view.frame];
 UILabel *timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(0.0,
0.0,
240.0f,
40.0f)];
 timeLabel.font = [UIFont systemFontOfSize:17.0f];
 timeLabel.text = @"Time: 0.0 s";
 timeLabel.textAlignment = UITextAlignmentCenter; timeLabel.backgroundColor
= [UIColor clearColor];
 timeLabel.textColor = [UIColor whiteColor]; [temp
addSubview:timeLabel]; uiElementInput = [[GPUImageUIElement alloc] initWithView:temp];
 [filter addTarget:blendFilter]; [uiElementInput
addTarget:blendFilter]; [blendFilter
 addTarget:filterView]; __unsafe_unretained GPUImageUIElement *weakUIElementInput = uiElementInput;
 [filter setFrameProcessingCompletionBlock:^(GPUImageOutput * filter, CMTime frameTime){ timeLabel.text = [NSString stringWithFormat:@"Time: %f s",
 -[startTime timeIntervalSinceNow]]; [weakUIElementInput update];
 }];
       要理解它的实现原理，需要搞懂GPUImageUIElement和GPUImageAlphaBlendFilter。
GPUImageUIElement的作用是把一个视图的layer通过CALayer的renderInContext方法把layer转化为image，然后作为OpenGL的纹理传给GPUImageAlphaBlendFilter。
而GPUImageAlphaBlendFilter则是一个两输入的blend
**混合**filter, 它的第一个输入是摄像头数据，第二个输入则是刚刚提到的GPUImageUIElement的数据，GPUImageAlphaBlendFilter将这两个输入做alpha blend，可以简单的理解为将第二个输入叠加到第一个的上面，更多关于[alpha
 blend](https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending)在维基百科上有介绍。下图是整个加水印的过程：
水印.png
**人脸检测**
       利用CIDetector即可简单的实现人脸检测，首先是CIDetector的初始化:
NSDictionary
 *detectorOptions = [[NSDictionary alloc] initWithObjectsAndKeys:CIDetectorAccuracyLow,
CIDetectorAccuracy, nil]; _faceDetector = [CIDetector
 detectorOfType:CIDetectorTypeFace context:nil options:detectorOptions];
       然后通过将摄像头数据CMSampleBufferRef转化为CIImage，对CIImage用CIDetector进行人脸检测:
CVPixelBufferRef pixelBuffer =
CMSampleBufferGetImageBuffer(sampleBuffer);
CFDictionaryRef attachments =
CMCopyDictionaryOfAttachments(kCFAllocatorDefault, sampleBuffer, kCMAttachmentMode_ShouldPropagate);
CIImage *convertedImage = [[CIImage
 alloc] initWithCVPixelBuffer:pixelBuffer options:(__bridge NSDictionary *)attachments];
NSArray *features = [self.faceDetector
 featuresInImage:convertedImage options:imageOptions];
       上面得到的features数组里的每个元素都是CIFaceFeature对象，根据它就能计算出人脸的具体位置，从而调整中水印图像的位置，达到图像跟随人脸动的效果。
for
 ( CIFaceFeature *faceFeature in featureArray) { // find the correct position for the square layer within the previewLayer// the feature box originates in the bottom left of the video frame.// (Bottom right if mirroring is turned on)//Update face bounds for iOS Coordinate System CGRect faceRect = [faceFeature bounds];
// flip preview width and height CGFloat temp = faceRect.size.width;
 faceRect.size.width
 = faceRect.size.height;
 faceRect.size.height
 = temp; temp = faceRect.origin.x; faceRect.origin.x = faceRect.origin.y; faceRect.origin.y = temp;
// scale coordinates so they fit in the preview box, which may be scaled CGFloat widthScaleBy
 = previewBox.size.width
 / clap.size.height;
 CGFloat heightScaleBy = previewBox.size.height
 / clap.size.width;
 faceRect.size.width
 *= widthScaleBy; faceRect.size.height
 *= heightScaleBy; faceRect.origin.x *= widthScaleBy; faceRect.origin.y *= heightScaleBy; faceRect = CGRectOffset(faceRect, previewBox.origin.x, previewBox.origin.y);
//mirror CGRect
rect = CGRectMake(previewBox.size.width
 - faceRect.origin.x - faceRect.size.width,
 faceRect.origin.y, faceRect.size.width,
 faceRect.size.height);
if (fabs(rect.origin.x
 - self.faceBounds.origin.x) > 5.0) { self.faceBounds =
rect; } }
       上面则是计算人脸位置faceBounds的方法，我们再根据faceBounds来更新水印图像的位置：
__weak typeof (self) weakSelf = self; [filter
 setFrameProcessingCompletionBlock:^(GPUImageOutput *output, CMTime time) { __strong typeof (self) strongSelf = weakSelf;
// update capImageView's frame CGRect
rect = strongSelf.faceBounds; CGSize
size = strongSelf.capImageView.frame.size;
 strongSelf.capImageView.frame = CGRectMake(rect.origin.x + (rect.size.width
 - size.width)/2,
rect.origin.y -
size.height,
size.width,
size.height);
 [strongSelf.element update]; }];
- 问题1：上面用的人脸检测是基于CIDetector的，实际实验发现，当人脸在摄像头中捕获不全时，有可能检测不出人脸，也就没法更新水印图像的位置。因此，更加精准、快速、细致的人脸检测是很有必要的，后面我会尝试使用一些其他的人脸检测方法。
- 问题2：上面的Faceu贴纸效果是静态图像的贴纸效果，如果要做动态效果的Faceu贴纸该怎么处理呢, Gif? CADisplayLink? 这个有待进一步研究
[http://www.jianshu.com/u/131423baa3c6](http://www.jianshu.com/u/131423baa3c6)
OpenGL ES：（Open
 Graphics Library For Embedded(嵌入的) Systems 开源嵌入式系统图形处理框架），一套图形与硬件接口，用于把处理好的图片显示到屏幕上。
GPUImage:是一个基于OpenGL
 ES 2.0图像和视频处理的开源iOS框架，提供各种各样的图像处理滤镜，并且支持照相机和摄像机的实时滤镜，内置120多种滤镜效果，并且能够自定义图像滤镜。
**GPUImage处理画面原理**
- GPUImage采用链式方式来处理画面,通过addTarget:方法为链条添加每个环节的对象，处理完一个target,就会把上一个环节处理好的图像数据传递下一个target去处理，称为GPUImage处理链。
- 比如：墨镜原理，从外界传来光线，会经过墨镜过滤，在传给我们的眼睛，就能感受到大白天也是乌黑一片。
- 一般的target可分为两类
- 中间环节的target, 一般是各种filter, 是GPUImageFilter或者是子类.
- 最终环节的target,
 GPUImageView：用于显示到屏幕上, 或者GPUImageMovieWriter：写成视频文件。
- GPUImage处理主要分为3个环节
- source(视频、图片源) -> filter（滤镜） -> final target (处理后视频、图片)
- GPUImaged的Source:都继承GPUImageOutput的子类，作为GPUImage的数据源
- GPUImageVideoCamera：用于实时拍摄视频
- GPUImageStillCamera：用于实时拍摄照片
- GPUImagePicture：用于处理已经拍摄好的图片，比如png,jpg图片
- GPUImageMovie：用于处理已经拍摄好的视频,比如mp4文件
- - GPUImage的filter:GPUimageFilter类或者子类，这个类继承自GPUImageOutput,并且遵守GPUImageInput协议，这样既能流进，又能流出，
- GPUImage的final target:GPUImageView,GPUImageMovieWriter
GPUImage处理原理.png
- 美白(GPUImageBrightnessFilter)：本质就是提高亮度。
**GPUImage原生美颜**
- 步骤一：使用Cocoapods导入GPUImage
- 步骤二：创建视频源GPUImageVideoCamera
- 步骤三：创建最终目的源：GPUImageView
- 步骤四：创建滤镜组(GPUImageFilterGroup)，需要组合亮度(GPUImageBrightnessFilter)和双边滤波(GPUImageBilateralFilter)这两个滤镜达到美颜效果.
- 步骤五：设置滤镜组链
- 步骤六：设置GPUImage处理链，从数据源 => 滤镜 => 最终界面效果
- 步骤七：开始采集视频
注意点：
- SessionPreset最好使用AVCaptureSessionPresetHigh，会自动识别，如果用太高分辨率，当前设备不支持会直接报错
- GPUImageVideoCamera必须要强引用，否则会被销毁，不能持续采集视频.
- 必须调用startCameraCapture，底层才会把采集到的视频源，渲染到GPUImageView中，就能显示了。
- GPUImageBilateralFilter的distanceNormalizationFactor值越小，磨皮效果越好,distanceNormalizationFactor取值范围: 大于1。
- (void)viewDidLoad
 { [super viewDidLoad];
// 创建视频源// SessionPreset:屏幕分辨率，AVCaptureSessionPresetHigh会自适应高分辨率// cameraPosition:摄像头方向 GPUImageVideoCamera *videoCamera = [[GPUImageVideoCamera alloc]
initWithSessionPreset:AVCaptureSessionPresetHigh
cameraPosition:AVCaptureDevicePositionFront]; videoCamera.outputImageOrientation = UIInterfaceOrientationPortrait;
 _videoCamera = videoCamera; // 创建最终预览View GPUImageView *captureVideoPreview = [[GPUImageView
 alloc] initWithFrame:self.view.bounds]; [self.view
insertSubview:captureVideoPreview
atIndex:0];
// 创建滤镜：磨皮，美白，组合滤镜 GPUImageFilterGroup *groupFilter = [[GPUImageFilterGroup alloc]
 init]; // 磨皮滤镜 GPUImageBilateralFilter *bilateralFilter = [[GPUImageBilateralFilter
 alloc] init]; [groupFilter addTarget:bilateralFilter]; _bilateralFilter = bilateralFilter;
// 美白滤镜 GPUImageBrightnessFilter *brightnessFilter = [[GPUImageBrightnessFilter alloc]
 init]; [groupFilter addTarget:brightnessFilter]; _brightnessFilter = brightnessFilter;
// 设置滤镜组链 [bilateralFilter
addTarget:brightnessFilter]; [groupFilter
setInitialFilters:@[bilateralFilter]]; groupFilter.terminalFilter = brightnessFilter;
// 设置GPUImage响应链，从数据源 => 滤镜 => 最终界面效果 [videoCamera
addTarget:groupFilter]; [groupFilter
addTarget:captureVideoPreview];
// 必须调用startCameraCapture，底层才会把采集到的视频源，渲染到GPUImageView中，就能显示了。// 开始采集视频 [videoCamera startCameraCapture];}- (IBAction)brightnessFilter:(UISlider
 *)sender { _brightnessFilter.brightness = sender.value;}- (IBAction)bilateralFilter:(UISlider
 *)sender { // 值越小，磨皮效果越好 CGFloat maxValue =
10; [_bilateralFilter
setDistanceNormalizationFactor:(maxValue - sender.value)];}
**利用美颜滤镜实现**
- 步骤一：使用Cocoapods导入GPUImage
- 步骤二：导入GPUImageBeautifyFilter文件夹
- 步骤三：创建视频源GPUImageVideoCamera
- 步骤四：创建最终目的源：GPUImageView
- 步骤五：创建最终美颜滤镜：GPUImageBeautifyFilter
- 步骤六：设置GPUImage处理链，从数据源 => 滤镜 => 最终界面效果
注意：
- 切换美颜效果原理：移除之前所有处理链，重新设置处理链
- (void)viewDidLoad
 { [super viewDidLoad];
// Do any additional setup after loading the view.// 创建视频源// SessionPreset:屏幕分辨率，AVCaptureSessionPresetHigh会自适应高分辨率// cameraPosition:摄像头方向 GPUImageVideoCamera *videoCamera = [[GPUImageVideoCamera alloc]
initWithSessionPreset:AVCaptureSessionPresetHigh
cameraPosition:AVCaptureDevicePositionFront]; videoCamera.outputImageOrientation = UIInterfaceOrientationPortrait;
 _videoCamera = videoCamera; // 创建最终预览View GPUImageView *captureVideoPreview = [[GPUImageView
 alloc] initWithFrame:self.view.bounds]; [self.view
insertSubview:captureVideoPreview
atIndex:0]; _captureVideoPreview = captureVideoPreview;
// 设置处理链 [_videoCamera
addTarget:_captureVideoPreview];
// 必须调用startCameraCapture，底层才会把采集到的视频源，渲染到GPUImageView中，就能显示了。// 开始采集视频 [videoCamera startCameraCapture];}- (IBAction)openBeautifyFilter:(UISwitch
 *)sender { // 切换美颜效果原理：移除之前所有处理链，重新设置处理链if (sender.on) {
// 移除之前所有处理链 [_videoCamera removeAllTargets];
// 创建美颜滤镜 GPUImageBeautifyFilter *beautifyFilter = [[GPUImageBeautifyFilter alloc]
 init]; // 设置GPUImage处理链，从数据源 => 滤镜 => 最终界面效果 [_videoCamera
addTarget:beautifyFilter]; [beautifyFilter
addTarget:_captureVideoPreview]; }
else {
// 移除之前所有处理链 [_videoCamera removeAllTargets]; [_videoCamera
addTarget:_captureVideoPreview]; }}
**GPUImage扩展**
- [GPUImage所有滤镜介绍](http://www.360doc.com/content/15/0907/10/19175681_497418716.shtml)：
已有的一些filter介绍：
#import "GPUImageBrightnessFilter.h"                //亮度
#import "GPUImageExposureFilter.h"                  //曝光
#import "GPUImageContrastFilter.h"                  //对比度
#import "GPUImageSaturationFilter.h"                //饱和度
#import "GPUImageGammaFilter.h"                     //伽马线
#import "GPUImageColorInvertFilter.h"               //反色
#import "GPUImageSepiaFilter.h"                     //褐色（怀旧）
#import "GPUImageLevelsFilter.h"                    //色阶
#import "GPUImageGrayscaleFilter.h"                 //灰度
#import "GPUImageHistogramFilter.h"                 //色彩直方图，显示在图片上
#import "GPUImageHistogramGenerator.h"              //色彩直方图
#import "GPUImageRGBFilter.h"                       //RGB
#import "GPUImageToneCurveFilter.h"                 //色调曲线
#import "GPUImageMonochromeFilter.h"                //单色
#import "GPUImageOpacityFilter.h"                   //不透明度
#import "GPUImageHighlightShadowFilter.h"           //提亮阴影
#import "GPUImageFalseColorFilter.h"                //色彩替换（替换亮部和暗部色彩）
#import "GPUImageHueFilter.h"                       //色度
#import "GPUImageChromaKeyFilter.h"                 //色度键
#import "GPUImageWhiteBalanceFilter.h"              //白平横
#import "GPUImageAverageColor.h"                    //像素平均色值
#import "GPUImageSolidColorGenerator.h"             //纯色
#import "GPUImageLuminosity.h"                      //亮度平均
#import "GPUImageAverageLuminanceThresholdFilter.h" //像素色值亮度平均，图像黑白（有类似漫画效果）
#import "GPUImageLookupFilter.h"                    //lookup 色彩调整
#import "GPUImageAmatorkaFilter.h"                  //Amatorka lookup
#import "GPUImageMissEtikateFilter.h"               //MissEtikate lookup
#import "GPUImageSoftEleganceFilter.h"              //SoftElegance lookup

#pragma mark - 图像处理 Handle Image
#import "GPUImageCrosshairGenerator.h"              //十字
#import "GPUImageLineGenerator.h"                   //线条
#import "GPUImageTransformFilter.h"                 //形状变化
#import "GPUImageCropFilter.h"                      //剪裁
#import "GPUImageSharpenFilter.h"                   //锐化
#import "GPUImageUnsharpMaskFilter.h"               //反遮罩锐化
#import "GPUImageFastBlurFilter.h"                  //模糊
#import "GPUImageGaussianBlurFilter.h"              //高斯模糊
#import "GPUImageGaussianSelectiveBlurFilter.h"     //高斯模糊，选择部分清晰
#import "GPUImageBoxBlurFilter.h"                   //盒状模糊
#import "GPUImageTiltShiftFilter.h"                 //条纹模糊，中间清晰，上下两端模糊
#import "GPUImageMedianFilter.h"                    //中间值，有种稍微模糊边缘的效果
#import "GPUImageBilateralFilter.h"                 //双边模糊
#import "GPUImageErosionFilter.h"                   //侵蚀边缘模糊，变黑白
#import "GPUImageRGBErosionFilter.h"                //RGB侵蚀边缘模糊，有色彩
#import "GPUImageDilationFilter.h"                  //扩展边缘模糊，变黑白
#import "GPUImageRGBDilationFilter.h"               //RGB扩展边缘模糊，有色彩
#import "GPUImageOpeningFilter.h"                   //黑白色调模糊
#import "GPUImageRGBOpeningFilter.h"                //彩色模糊
#import "GPUImageClosingFilter.h"                   //黑白色调模糊，暗色会被提亮
#import "GPUImageRGBClosingFilter.h"                //彩色模糊，暗色会被提亮
#import "GPUImageLanczosResamplingFilter.h"         //Lanczos重取样，模糊效果
#import "GPUImageNonMaximumSuppressionFilter.h"     //非最大抑制，只显示亮度最高的像素，其他为黑
#import "GPUImageThresholdedNonMaximumSuppressionFilter.h" //与上相比，像素丢失更多
#import "GPUImageSobelEdgeDetectionFilter.h"        //Sobel边缘检测算法(白边，黑内容，有点漫画的反色效果)
#import "GPUImageCannyEdgeDetectionFilter.h"        //Canny边缘检测算法（比上更强烈的黑白对比度）
#import "GPUImageThresholdEdgeDetectionFilter.h"    //阈值边缘检测（效果与上差别不大）
#import "GPUImagePrewittEdgeDetectionFilter.h"      //普瑞维特(Prewitt)边缘检测(效果与Sobel差不多，貌似更平滑)
#import "GPUImageXYDerivativeFilter.h"              //XYDerivative边缘检测，画面以蓝色为主，绿色为边缘，带彩色
#import "GPUImageHarrisCornerDetectionFilter.h"     //Harris角点检测，会有绿色小十字显示在图片角点处
#import "GPUImageNobleCornerDetectionFilter.h"      //Noble角点检测，检测点更多
#import "GPUImageShiTomasiFeatureDetectionFilter.h" //ShiTomasi角点检测，与上差别不大
#import "GPUImageMotionDetector.h"                  //动作检测
#import "GPUImageHoughTransformLineDetector.h"      //线条检测
#import "GPUImageParallelCoordinateLineTransformFilter.h" //平行线检测
#import "GPUImageLocalBinaryPatternFilter.h"        //图像黑白化，并有大量噪点
#import "GPUImageLowPassFilter.h"                   //用于图像加亮
#import "GPUImageHighPassFilter.h"                  //图像低于某值时显示为黑
#pragma mark - 视觉效果 Visual Effect
#import "GPUImageSketchFilter.h"                    //素描
#import "GPUImageThresholdSketchFilter.h"           //阀值素描，形成有噪点的素描
#import "GPUImageToonFilter.h"                      //卡通效果（黑色粗线描边）
#import "GPUImageSmoothToonFilter.h"                //相比上面的效果更细腻，上面是粗旷的画风
#import "GPUImageKuwaharaFilter.h"                  //桑原(Kuwahara)滤波,水粉画的模糊效果；处理时间比较长，慎用
#import "GPUImageMosaicFilter.h"                    //黑白马赛克
#import "GPUImagePixellateFilter.h"                 //像素化
#import "GPUImagePolarPixellateFilter.h"            //同心圆像素化
#import "GPUImageCrosshatchFilter.h"                //交叉线阴影，形成黑白网状画面
#import "GPUImageColorPackingFilter.h"              //色彩丢失，模糊（类似监控摄像效果）
#import "GPUImageVignetteFilter.h"                  //晕影，形成黑色圆形边缘，突出中间图像的效果
#import "GPUImageSwirlFilter.h"                     //漩涡，中间形成卷曲的画面
#import "GPUImageBulgeDistortionFilter.h"           //凸起失真，鱼眼效果
#import "GPUImagePinchDistortionFilter.h"           //收缩失真，凹面镜
#import "GPUImageStretchDistortionFilter.h"         //伸展失真，哈哈镜
#import "GPUImageGlassSphereFilter.h"               //水晶球效果
#import "GPUImageSphereRefractionFilter.h"          //球形折射，图形倒立
#import "GPUImagePosterizeFilter.h"                 //色调分离，形成噪点效果
#import "GPUImageCGAColorspaceFilter.h"             //CGA色彩滤镜，形成黑、浅蓝、紫色块的画面
#import "GPUImagePerlinNoiseFilter.h"               //柏林噪点，花边噪点
#import "GPUImage3x3ConvolutionFilter.h"            //3x3卷积，高亮大色块变黑，加亮边缘、线条等
#import "GPUImageEmbossFilter.h"                    //浮雕效果，带有点3d的感觉
#import "GPUImagePolkaDotFilter.h"                  //像素圆点花样
#import "GPUImageHalftoneFilter.h"                  //点染,图像黑白化，由黑点构成原图的大致图形
#pragma mark - 混合模式 Blend
#import "GPUImageMultiplyBlendFilter.h"             //通常用于创建阴影和深度效果
#import "GPUImageNormalBlendFilter.h"               //正常
#import "GPUImageAlphaBlendFilter.h"                //透明混合,通常用于在背景上应用前景的透明度
#import "GPUImageDissolveBlendFilter.h"             //溶解
#import "GPUImageOverlayBlendFilter.h"              //叠加,通常用于创建阴影效果
#import "GPUImageDarkenBlendFilter.h"               //加深混合,通常用于重叠类型
#import "GPUImageLightenBlendFilter.h"              //减淡混合,通常用于重叠类型
#import "GPUImageSourceOverBlendFilter.h"           //源混合
#import "GPUImageColorBurnBlendFilter.h"            //色彩加深混合
#import "GPUImageColorDodgeBlendFilter.h"           //色彩减淡混合
#import "GPUImageScreenBlendFilter.h"               //屏幕包裹,通常用于创建亮点和镜头眩光
#import "GPUImageExclusionBlendFilter.h"            //排除混合
#import "GPUImageDifferenceBlendFilter.h"           //差异混合,通常用于创建更多变动的颜色
#import "GPUImageSubtractBlendFilter.h"             //差值混合,通常用于创建两个图像之间的动画变暗模糊效果
#import "GPUImageHardLightBlendFilter.h"            //强光混合,通常用于创建阴影效果
#import "GPUImageSoftLightBlendFilter.h"            //柔光混合
#import "GPUImageChromaKeyBlendFilter.h"            //色度键混合
#import "GPUImageMaskFilter.h"                      //遮罩混合
#import "GPUImageHazeFilter.h"                      //朦胧加暗
#import "GPUImageLuminanceThresholdFilter.h"        //亮度阈
#import "GPUImageAdaptiveThresholdFilter.h"         //自适应阈值
#import "GPUImageAddBlendFilter.h"                  //通常用于创建两个图像之间的动画变亮模糊效果
#import "GPUImageDivideBlendFilter.h"               //通常用于创建两个图像之间的动画变暗模糊效果
- [美图秀秀滤镜大汇总](http://www.tuicool.com/articles/6bIbQbQ):[http://www.cnblogs.com/salam/p/5125836.html?utm_source=tuicool&utm_medium=referral](http://www.cnblogs.com/salam/p/5125836.html?utm_source=tuicool&utm_medium=referral)
**源码下载**
[源码](https://github.com/iThinkerYZ/GPUImgeDemo)
注意：第一次打开需要 pod
 install
[http://www.jianshu.com/p/4646894245ba](http://www.jianshu.com/p/4646894245ba)
[https://www.v2ex.com/t/260901](https://www.v2ex.com/t/260901)
[http://www.cnblogs.com/doujiangyoutiao/p/6228730.html](http://www.cnblogs.com/doujiangyoutiao/p/6228730.html)
用CocoaPods集成到项目中。
pod 'GPUImage', '~> 0.1.7'
亮度(brightness)取值范围[-1,1],0为正常状态，默认。 (2)平滑因子(distanceNormalizationFactor)值越小，磨皮效果越好，默认为8。我为了演示效果，把最大值设置成100，这样几乎就没有磨皮效果了，平时最好10以内。最好大于0，不然就会变形。
**自定义滤镜**
  如果你感觉GPUImage自带的滤镜不够用的话，也可以自定义滤镜，使用方式和上面的差不多。我以一个别人写的[美颜滤镜](https://github.com/Guikunzhi/BeautifyFaceDemo)为例。
1.[demo下载地址](https://github.com/haichong/LiveAppDemo)。把GPUImageBeautifyFilter文件夹导入你的工程中。
[http://www.iosxxx.com/blog/2017-01-15-iOS%E7%9B%B8%E6%9C%BA%E5%BC%80%E5%8F%91%E7%9A%84%E8%B8%A9%E5%9D%91%E7%AF%87.html](http://www.iosxxx.com/blog/2017-01-15-iOS%E7%9B%B8%E6%9C%BA%E5%BC%80%E5%8F%91%E7%9A%84%E8%B8%A9%E5%9D%91%E7%AF%87.html)

待验证
ios美颜:
[http://www.cnblogs.com/fengmin/p/5518755.html](http://www.cnblogs.com/fengmin/p/5518755.html)

