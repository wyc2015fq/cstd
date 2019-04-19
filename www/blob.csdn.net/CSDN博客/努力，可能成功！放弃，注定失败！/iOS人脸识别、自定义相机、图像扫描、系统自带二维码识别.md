# iOS人脸识别、自定义相机、图像扫描、系统自带二维码识别 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月16日 13:29:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6536
前段时间遇到一个需求，需要实时扫描图像，并且设定摄像头的尺寸为1080x1920，然后从中间截取出512x512的图片传给第三方SDK做进一步业务处理，直到SDK返回正确的处理结果。
一顿Google，发现图像预览、人脸识别、二维码识别这些苹果都帮我们做好了，而且它们都是基于AVFoundation框架实现的。
话不多说，上代码~！
用到的类，主要有这些：
```
//硬件设备
@property (nonatomic, strong) AVCaptureDevice *device;
//输入流
@property (nonatomic, strong) AVCaptureDeviceInput *input;
//协调输入输出流的数据
@property (nonatomic, strong) AVCaptureSession *session;
//预览层
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *previewLayer;
//输出流
@property (nonatomic, strong) AVCaptureStillImageOutput *stillImageOutput;  //用于捕捉静态图片
@property (nonatomic, strong) AVCaptureVideoDataOutput *videoDataOutput;    //原始视频帧，用于获取实时图像以及视频录制
@property (nonatomic, strong) AVCaptureMetadataOutput *metadataOutput;      //用于二维码识别以及人脸识别
```
- 
##### 首先我们需要在手机上把图像显示出来
1.1 获取硬件设备
```
-(AVCaptureDevice *)device{
 if (_device == nil) {
     _device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
     if ([_device lockForConfiguration:nil]) {
         //自动闪光灯
         if ([_device isFlashModeSupported:AVCaptureFlashModeAuto]) {
             [_device setFlashMode:AVCaptureFlashModeAuto];
         }
         //自动白平衡
         if ([_device isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance]) {
             [_device setWhiteBalanceMode:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance];
         }
         //自动对焦
         if ([_device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
             [_device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
         }
         //自动曝光
         if ([_device isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]) {
             [_device setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
         }
         [_device unlockForConfiguration];
     }
 }
 return _device;
}
```
device有很多属性可以调整(*注意调整device属性的时候需要上锁, 调整完再解锁*)：
闪光灯
```
typedef NS_ENUM(NSInteger, AVCaptureFlashMode) {
AVCaptureFlashModeOff  = 0,
AVCaptureFlashModeOn   = 1,
AVCaptureFlashModeAuto = 2
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
前后置摄像头
```
typedef NS_ENUM(NSInteger, AVCaptureDevicePosition) {
AVCaptureDevicePositionUnspecified         = 0,
AVCaptureDevicePositionBack                = 1,
AVCaptureDevicePositionFront               = 2
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
手电筒
```
typedef NS_ENUM(NSInteger, AVCaptureTorchMode) {
AVCaptureTorchModeOff  = 0,
AVCaptureTorchModeOn   = 1,
AVCaptureTorchModeAuto = 2,
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
对焦
```
typedef NS_ENUM(NSInteger, AVCaptureFocusMode) {
AVCaptureFocusModeLocked              = 0,
AVCaptureFocusModeAutoFocus           = 1,
AVCaptureFocusModeContinuousAutoFocus = 2,
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
曝光
```
typedef NS_ENUM(NSInteger, AVCaptureExposureMode) {
AVCaptureExposureModeLocked                            = 0,
AVCaptureExposureModeAutoExpose                        = 1,
AVCaptureExposureModeContinuousAutoExposure            = 2,
AVCaptureExposureModeCustom NS_ENUM_AVAILABLE_IOS(8_0) = 3,
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
白平衡
```
typedef NS_ENUM(NSInteger, AVCaptureWhiteBalanceMode) {
AVCaptureWhiteBalanceModeLocked                      = 0,
AVCaptureWhiteBalanceModeAutoWhiteBalance            = 1,
AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance  = 2,
} NS_AVAILABLE(10_7, 4_0) __TVOS_PROHIBITED;
```
1.2 获取硬件的输入流
```
-(AVCaptureDeviceInput *)input{
 if (_input == nil) {
     _input = [[AVCaptureDeviceInput alloc] initWithDevice:self.device error:nil];
 }
 return _input;
}
```
创建输入流的时候，会弹出alert向用户获取相机权限
1.3 需要一个用来协调输入和输出数据的会话，然后把input添加到会话中
```
-(AVCaptureSession *)session{
 if (_session == nil) {
     _session = [[AVCaptureSession alloc] init];
     if ([_session canAddInput:self.input]) {
         [_session addInput:self.input];
     }
 }
 return _session;
}
```
1.4 然后我们需要一个预览图像的层
```
-(AVCaptureVideoPreviewLayer *)previewLayer{
 if (_previewLayer == nil) {
     _previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:self.session];
     _previewLayer.frame = self.view.layer.bounds;
 }
 return _previewLayer;
}
```
1.5 最后把previewLayer添加到self.view.layer上
`[self.view.layer addSublayer:self.previewLayer];`
1.6 找个合适的位置，让session运行起来，比如viewWillAppear
```
-(void)viewWillAppear:(BOOL)animated{
 [super viewWillAppear:animated];
 [self.session startRunning];
}
```
- 
##### 搞一个按钮用来控制手电筒
```
#pragma mark - 手电筒
-(void)openTorch:(UIButton*)button{
 button.selected = !button.selected;
 Class captureDeviceClass = NSClassFromString(@"AVCaptureDevice");
 if (captureDeviceClass != nil) {
     if ([self.device hasTorch] && [self.device hasFlash]){
         [self.device lockForConfiguration:nil];
         if (button.selected) {
             [self.device setTorchMode:AVCaptureTorchModeOn];
         } else {
             [self.device setTorchMode:AVCaptureTorchModeOff];
         }
         [self.device unlockForConfiguration];
     }
 }
}
```
- 
##### 再搞一个按钮来切换前后置摄像头
```
#pragma mark - 切换前后摄像头
-(void)switchCamera{
 NSUInteger cameraCount = [[AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo] count];
 if (cameraCount > 1) {
     AVCaptureDevice *newCamera = nil;
     AVCaptureDeviceInput *newInput = nil;
     AVCaptureDevicePosition position = [[self.input device] position];
     if (position == AVCaptureDevicePositionFront){
         newCamera = [self cameraWithPosition:AVCaptureDevicePositionBack];
     }else {
         newCamera = [self cameraWithPosition:AVCaptureDevicePositionFront];
     }
     newInput = [AVCaptureDeviceInput deviceInputWithDevice:newCamera error:nil];
     if (newInput != nil) {
         [self.session beginConfiguration];
         [self.session removeInput:self.input];
         if ([self.session canAddInput:newInput]) {
             [self.session addInput:newInput];
             self.input = newInput;
         }else {
             [self.session addInput:self.input];
         }
         [self.session commitConfiguration];
     }
 }
}
```
```
-(AVCaptureDevice *)cameraWithPosition:(AVCaptureDevicePosition)position{
 NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
 for ( AVCaptureDevice *device in devices )
     if ( device.position == position ) return device;
 return nil;
}
```
- 
##### 使用AVCaptureStillImageOutput捕获静态图片
4.1 创建一个AVCaptureStillImageOutput对象
```
-(AVCaptureStillImageOutput *)stillImageOutput{
 if (_stillImageOutput == nil) {
     _stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
 }
 return _stillImageOutput;
}
```
4.2 将stillImageOutput添加到session中
```
if ([_session canAddOutput:self.stillImageOutput]) {
    [_session addOutput:self.stillImageOutput];
}
```
4.3 搞个拍照按钮，截取静态图片
```
//AVCaptureStillImageOutput截取静态图片，会有快门声
-(void)screenshot{
 AVCaptureConnection * videoConnection = [self.stillImageOutput connectionWithMediaType:AVMediaTypeVideo];
 if (!videoConnection) {
     NSLog(@"take photo failed!");
     return;
 }
 [self.stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
     if (imageDataSampleBuffer == NULL) {
         return;
     }
     NSData * imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
     UIImage *image = [UIImage imageWithData:imageData];
     [self saveImageToPhotoAlbum:image];
 }];
}
```
- 
##### 使用AVCaptureVideoOutput实时获取预览图像，这也是楼主的项目需求所在
5.1 创建AVCaptureVideoOutput对象
```
-(AVCaptureVideoDataOutput *)videoDataOutput{
 if (_videoDataOutput == nil) {
     _videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
     [_videoDataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
 }
 return _videoDataOutput;
}
```
5.2 将videoDataOutput添加session中
```
if ([_session canAddOutput:self.videoDataOutput]) {
    [_session addOutput:self.videoDataOutput];
}
```
5.3 遵守AVCaptureVideoDataOutputSampleBufferDelegate协议，并实现它的代理方法
```
#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
//AVCaptureVideoDataOutput获取实时图像，这个代理方法的回调频率很快，几乎与手机屏幕的刷新频率一样快
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
     largeImage = [self imageFromSampleBuffer:sampleBuffer];
}
```
5.4 实现imageFromSampleBuffer:方法，将CMSampleBufferRef转为NSImage
```
//CMSampleBufferRef转NSImage
-(UIImage *)imageFromSampleBuffer:(CMSampleBufferRef)sampleBuffer{
 // 为媒体数据设置一个CMSampleBuffer的Core Video图像缓存对象
 CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
 // 锁定pixel buffer的基地址
 CVPixelBufferLockBaseAddress(imageBuffer, 0);
 // 得到pixel buffer的基地址
 void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);
 // 得到pixel buffer的行字节数
 size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
 // 得到pixel buffer的宽和高
 size_t width = CVPixelBufferGetWidth(imageBuffer);
 size_t height = CVPixelBufferGetHeight(imageBuffer);
 // 创建一个依赖于设备的RGB颜色空间
 CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
 // 用抽样缓存的数据创建一个位图格式的图形上下文（graphics context）对象
 CGContextRef context = CGBitmapContextCreate(baseAddress, width, height, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
 // 根据这个位图context中的像素数据创建一个Quartz image对象
 CGImageRef quartzImage = CGBitmapContextCreateImage(context);
 // 解锁pixel buffer
 CVPixelBufferUnlockBaseAddress(imageBuffer,0);
 // 释放context和颜色空间
 CGContextRelease(context); CGColorSpaceRelease(colorSpace);
 // 用Quartz image创建一个UIImage对象image
 UIImage *image = [UIImage imageWithCGImage:quartzImage];
 // 释放Quartz image对象
 CGImageRelease(quartzImage);
 return (image);
}
```
眼看大功告成，结果一运行，创建core graphic上下文的时候报错：
```cpp
```cpp
CGBitmapContextCreate: invalid data bytes/row: should be at least 7680 for 8 integer bits/component, 3 components, kCGImageAlphaPremultipliedFirst.**
CGBitmapContextCreateImage: invalid context 0x0. If you want to see the backtrace, please set CG_CONTEXT_SHOW_BACKTRACE environmental variable.**
```
```
又是一通Google，发现stackoverflow上很多这种问答，楼主英语不好，又是一通翻译，发现大家都是在说像素组件位数什么的，摸索半天找到解决办法，设置videoDataOutput的像素格式：
`[_videoDataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];`
再运行，还有问题，获取到的图片是颠倒的，尼玛，真是多灾多难，不过这个简单，很快找到解决方法，设置一下视频的方向：
```
#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
//AVCaptureVideoDataOutput获取实时图像，这个代理方法的回调频率很快，几乎与手机屏幕的刷新频率一样快
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
 [connection setVideoOrientation:AVCaptureVideoOrientationPortrait];
 largeImage = [self imageFromSampleBuffer:sampleBuffer];
}
```
5.5 还记得楼主一开始的需求吗，设定摄像头的尺寸为1080x1920，然后从中间截取出512x512的图片传给第三方SDK做进一步业务处理：
`[_session setSessionPreset:AVCaptureSessionPreset1920x1080];`
```cpp
```cpp
smallImage = [largeImage imageCompressTargetSize:CGSizeMake(512.0f, 512.0f)];
```
```
到这里为止，楼主的需求就大功告成啦
- 
##### 使用AVCaptureMetadataOutput识别二维码
6.1 创建AVCaptureMetadataOutput对象
```
-(AVCaptureMetadataOutput *)metadataOutput{
 if (_metadataOutput == nil) {
     _metadataOutput = [[AVCaptureMetadataOutput alloc]init];
     [_metadataOutput setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
     //设置扫描区域
     _metadataOutput.rectOfInterest = self.view.bounds;
 }
 return _metadataOutput;
}
```
6.2 将metadataOutput添加到session中，并且设置扫描类型
```
if ([_session canAddOutput:self.metadataOutput]) {
    [_session addOutput:self.metadataOutput];
    //设置扫码格式
    self.metadataOutput.metadataObjectTypes = @[
                                                AVMetadataObjectTypeQRCode,
                                                AVMetadataObjectTypeEAN13Code,
                                                AVMetadataObjectTypeEAN8Code,
                                                AVMetadataObjectTypeCode128Code
                                                ];
}
```
6.3 遵守AVCaptureMetadataOutputObjectsDelegate协议，并实现其代理方法
```
#pragma mark - AVCaptureMetadataOutputObjectsDelegate
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
 if (metadataObjects.count>0) {
     [self.session stopRunning];
     AVMetadataMachineReadableCodeObject *metadataObject = [metadataObjects objectAtIndex :0];
     NSLog(@"二维码内容 ： %@",metadataObject.stringValue);
 }
}
```
- 
##### 关于人脸识别
人脸识别也是基于AVCaptureMetadataOutput实现的，跟二维码识别的区别在于，扫描类型：
`self.metadataOutput.metadataObjectTypes = @[AVMetadataObjectTypeFace];`
```
#pragma mark - AVCaptureMetadataOutputObjectsDelegate
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
 if (metadataObjects.count>0) {
     [self.session stopRunning];
     AVMetadataMachineReadableCodeObject *metadataObject = [metadataObjects objectAtIndex :0];
     if (metadataObject.type == AVMetadataObjectTypeFace) {
         AVMetadataObject *objec = [self.previewLayer transformedMetadataObjectForMetadataObject:metadataObject];
         NSLog(@"%@",objec);
     }
 }
}
```
至于怎么利用它来实现具体的功能需求，楼主也很方哈，这里有个链接可以参考一下：[基于 OpenCV 的人脸识别](https://objccn.io/issue-21-9/)
好啦，就这么多了，[代码在这里](https://github.com/daiweiping/Scanner-AVCapture)，水平有限，有不对的地方还请多多指教
参考资料：[iOS 上的相机捕捉](https://objccn.io/issue-21-3/)
文／戴尼玛（简书作者）
原文链接：http://www.jianshu.com/p/61ca3a917fe5
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
