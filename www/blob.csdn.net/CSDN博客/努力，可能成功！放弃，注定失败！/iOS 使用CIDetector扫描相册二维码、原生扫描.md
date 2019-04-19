# iOS 使用CIDetector扫描相册二维码、原生扫描 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月26日 17:19:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1143
## 本文来自简书，原文地址:[http://www.jianshu.com/p/98acd7f943c1](http://www.jianshu.com/p/98acd7f943c1)
## 前言
> 
早上使用微信的时候，突然想到以前在项目中集成扫码功能，当时没有从相册中扫描二维码的需求，加上需要向下兼容，于是选择集成了zbar扫描。今天我们就来看一下如何实现从相册中扫码、以及原生扫描。
## 原生扫描
- 
iOS7之后，AVFoundation让我们终于可以使用原生扫描进行扫码了（二维码与条码皆可）AVFoundation可以让我们从设备中获取到输入流与输出流，从而获取二维码中包含的信息。
- 
实现原生扫描非常简单。
1.先导入AVFoundation框架。
`<AVFoundation/AVFoundation.h>`
2.接着设置代理，实现代理回调方法
AVCaptureMetadataOutputObjectsDelegate
3.然后创建几个类即可:
设备 AVCaptureDevice
捕捉会话 AVCaptureSession
输入流 AVCaptureDeviceInput
输出流 AVCaptureMetadataOutput
预览图层 AVCaptureVideoPreviewLayer
- 
下面是简单的代码实现示例
```
NSError *error = nil;
  AVCaptureDevice *captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];//设备
  AVCaptureSession *session = [[AVCaptureSession alloc] init];//捕捉会话
  [session setSessionPreset:AVCaptureSessionPresetHigh];//设置采集率
  AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];//输入流
  AVCaptureMetadataOutput *output = [[AVCaptureMetadataOutput alloc] init];//输出流
  //添加到捕捉会话
  [session addInput:input];
  [session addOutput:output];
  //扫码类型：需要先将输出流添加到捕捉会话后再进行设置
  //这里只设置了可扫描二维码,有条码需要，在数组中继续添加即可
  [output setMetadataObjectTypes:@[AVMetadataObjectTypeQRCode]];
  //输出流delegate,在主线程刷新UI
  [output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
  AVCaptureVideoPreviewLayer *videoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:session];//预览
  videoPreviewLayer.frame = self.view.bounds;
  [self.view.layer insertSublayer:videoPreviewLayer atIndex:0];//添加预览图层
  //还可以设置扫描范围 output.rectOfInterest  不设置默认为全屏
  //开始扫描
  [session startRunning];
```
- 
接着实现扫码成功的回调方法
```
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection {
    NSString *content = @"";
    AVMetadataMachineReadableCodeObject *metadataObject = metadataObjects.firstObject;
    content = metadataObject.stringValue;//获取到二维码中的信息字符串
    //对此字符串进行处理(音效、网址分析、页面跳转等)
}
```
- 我们还可以添加扫码成功后的声音与振动效果，提高用户体验
```
- (void)playBeep{
  SystemSoundID soundID;
  AudioServicesCreateSystemSoundID((__bridge CFURLRef)[NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"滴-2"ofType:@"mp3"]], &soundID);
  AudioServicesPlaySystemSound(soundID);
  AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
```
## 从相册获取二维码
- iOS8之后，可以使用CIDetector(CIDetector可用于人脸识别)进行图片解析，从而使我们可以便捷的从相册中获取到二维码。
- 1.调用系统相册，从系统相册中选取图片
2.使用探测器(CIDetector)对选取的图片进行处理，取得图片二维码中包含的数据信息。
- 
下面是简单的代码实现示例
```
- (void)choicePhoto{
  //调用相册
  UIImagePickerController *imagePicker = [[UIImagePickerController alloc]init];
  imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
  imagePicker.delegate = self;
  [self presentViewController:imagePicker animated:YES completion:nil];
}
//选中图片的回调
-(void)imagePickerController:(UIImagePickerController*)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
  NSString *content = @"" ;
  //取出选中的图片
  UIImage *pickImage = info[UIImagePickerControllerOriginalImage];
  NSData *imageData = UIImagePNGRepresentation(pickImage);
  CIImage *ciImage = [CIImage imageWithData:imageData];
  //创建探测器
  CIDetector *detector = [CIDetector detectorOfType:CIDetectorTypeQRCode context:nil options:@{CIDetectorAccuracy: CIDetectorAccuracyLow}];
  NSArray *feature = [detector featuresInImage:ciImage];
  //取出探测到的数据
  for (CIQRCodeFeature *result in feature) {
      content = result.messageString;
  }
  //进行处理(音效、网址分析、页面跳转等)
}
```
## 源码
> 
点此下载：[github](https://github.com/cxj3599819/CCNScan/tree/master)源码
## 结语
原生扫描比zbar、zxing效率更高，且这两个库年久失修(zxingOBJC有人在持续维护)还有兼容性问题。
CIDetector是系统为我们提供的非常强大的类库，但是很多公司因为需要向下兼容，所以没有办法使用。
如果项目不需向下兼容多个版本时，建议使用系统原生扫描以及CIDetector进行二维码相关处理。
> 
参考:
[http://www.tuicool.com/articles/ie2aAjv](http://www.tuicool.com/articles/ie2aAjv)
[http://www.jianshu.com/p/cc79c45b4ccf](http://www.jianshu.com/p/cc79c45b4ccf)
