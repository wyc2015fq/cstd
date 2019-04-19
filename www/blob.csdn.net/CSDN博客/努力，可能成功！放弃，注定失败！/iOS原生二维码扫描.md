# iOS原生二维码扫描 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月08日 17:30:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2033
现如今几乎每个app基本都有扫描功能，提高了用户体验。所以掌握这项技能必不可少。
在开始写代码之前，我们需要先导入扫描二维码的框架`<AVFoundation/AVFoundation.h>`
```
//需要实现扫描二维码的代理方法 
@interface ViewController ()<AVCaptureMetadataOutputObjectsDelegate>
//在stotyboard中添加一个扫描的视图和开始or暂停的按钮
@property (weak, nonatomic) IBOutlet UIView *scanView;
@property (weak, nonatomic) IBOutlet UIButton *button;
@property (nonatomic ,strong) AVCaptureSession *captureSession;
@property (nonatomic ,strong) AVCaptureVideoPreviewLayer *captureVideoPreviewLayer;
@end
```
![](http://upload-images.jianshu.io/upload_images/1388790-35fa7c5aec29c704.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-04-08 下午4.28.01.png
开始扫描的方法
```
-(BOOL)startScanning{
    [_button setTitle:@"stop" forState:UIControlStateNormal];
    NSError *error = nil;
    AVCaptureDevice *captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    //初始化输入流
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
    if (!input) {
        NSLog(@"%@",error.localizedDescription);
        return NO;
    }
    //创建会话
    _captureSession = [[AVCaptureSession alloc]init];
    //添加输入流
    [_captureSession addInput:input];
    //初始化输出流
    AVCaptureMetadataOutput *captureMetadataOutput = [[AVCaptureMetadataOutput alloc]init];
    //添加输出流
    [_captureSession addOutput:captureMetadataOutput];
    //添加线程
    dispatch_queue_t dispatchQueue;
    dispatchQueue = dispatch_queue_create("ScanQRCodeQueue", NULL);
    //设置代理方法
    [captureMetadataOutput setMetadataObjectsDelegate:self queue:dispatchQueue];
    [captureMetadataOutput setMetadataObjectTypes:@[AVMetadataObjectTypeQRCode]];
    //创建扫描view的layer层
    _captureVideoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc]initWithSession:_captureSession];
    //将图层设置为视频样式
    [_captureVideoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    //设置layer的位置
    [_captureVideoPreviewLayer setFrame:_scanView.frame];
    [self.view.layer addSublayer:_captureVideoPreviewLayer];
    //开始扫描
    [_captureSession startRunning];
    return YES;
}
```
停止扫描的方法
```
-(void)stopScanning{
    //停止扫描
    [_captureSession stopRunning];
    //将会话置为nil
    _captureSession = nil;
    //将button切换回去
    [_button setTitle:@"start" forState:UIControlStateNormal];
}
```
扫描的代理方法，接收扫描结果
```
#pragma mark-----代理方法
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
    //判断扫描是否有数据
    if (metadataObjects != nil && [metadataObjects count] > 0) {
        AVMetadataMachineReadableCodeObject *metadataObject = [metadataObjects objectAtIndex:0];
        NSString *result;
         //判断的扫描的结果是否是二维码
        if ([[metadataObject type] isEqualToString:AVMetadataObjectTypeQRCode]) {
            result = metadataObject.stringValue;
        }else{
            NSLog(@"不是二维码");
        }
        //返回主线程，并将扫描的结果传回主线程
        [self performSelectorOnMainThread:@selector(reportScanResult:) withObject:result waitUntilDone:NO];
    }
}
-(void)reportScanResult:(NSString *)result{
    //扫描完毕，停止扫描
    [self stopScanning];
    //创建对话框，将扫描结果以对话框的形式呈现
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示" message:result preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *action = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
    }];
    [alert addAction:action];
    [self presentViewController:alert animated:YES completion:nil];
}
```
```
- (IBAction)startScan:(UIButton *)sender {
    //点击按钮的时候判断是否是没有在扫描状态，是则开始，不是就暂停
    if ([sender.titleLabel.text isEqualToString:@"start"]) {
        [self startScanning];
    }else{
        [self stopScanning];
    }
}
```
