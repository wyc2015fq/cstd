# iOS开发探索-二维码 - weixin_33985507的博客 - CSDN博客
2016年02月24日 11:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
iOS7之前，开发者进行扫码编程时，一般会借助第三方库。扫描二维码 OC 的开源库有 ZBar和ZXing，iOS7之后，系统的AVMetadataObject类中，为我们提供了解析二维码的接口。经过测试，使用原生API扫描和处理的效率非常高，远远高于第三方库。
### 二维码扫描
在开始之前最重要的一步是要先导入原生的音视频框架即 AVFoundation.framework 。
以下是笔者封装的二维码扫描视图QRView
> 
QRView.h 头文件
```
#import <UIKit/UIKit.h>
@class QRView;
@protocol QRViewDelegate <NSObject>
/**
 *  代理回调扫描结果
 *
 *  @param view   扫一扫视图
 *  @param result 扫描结果
 */
- (void)qrView:(QRView*)view ScanResult:(NSString*)result;
@end
@interface QRView : UIView
@property(nonatomic,assign)id<QRViewDelegate> delegate;
@property(nonatomic,assign,readonly)CGRect scanViewFrame;
- (void)startScan;
- (void)stopScan;
@end
```
> 
QRView.m 实现文件
```
#import "QRView.h"
#import <AVFoundation/AVFoundation.h>
@interface QRView ()<AVCaptureMetadataOutputObjectsDelegate>
@end
@implementation QRView
{
    AVCaptureSession *_session;
    UIImageView *_scanView;
    UIImageView *_lineView;
}
- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        [self initView];
    }
    return self;
}
- (void)initView
{
    UIImage *scanImage = [UIImage imageNamed:@"scanscanBg"];
    CGFloat width = CGRectGetWidth(self.frame);
    CGFloat height = CGRectGetHeight(self.frame);
    CGFloat scanW = 200;
    CGRect scanFrame = CGRectMake(width/2.-100, height/2.-100, scanW, scanW);
    _scanViewFrame = scanFrame;
    _scanView = [[UIImageView alloc]initWithImage:scanImage];
    _scanView.backgroundColor = [UIColor clearColor];
    _scanView.frame = scanFrame;
    [self addSubview:_scanView];
    //获取摄像设备
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    //闪光灯
    if ([device hasFlash] && [device hasTorch]) {
        [device lockForConfiguration:nil];
        [device setFlashMode:AVCaptureFlashModeAuto];
        [device setTorchMode:AVCaptureTorchModeAuto];
        [device unlockForConfiguration];
    }
    //创建输入流
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:nil];
    //创建输出流
    AVCaptureMetadataOutput *output = [[AVCaptureMetadataOutput alloc]init];
    //设置代理 刷新线程
    [output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
    output.rectOfInterest = [self rectOfInterestByScanViewRect:_scanView.frame];
    //初始化连接对象
    _session = [[AVCaptureSession alloc]init];
    //采集率
    _session.sessionPreset = AVCaptureSessionPresetHigh;
    
   if (input) {
        [_session addInput:input];
    }
    if (output) {
        [_session addOutput:output];
        //设置扫码支持的编码格式
    
        NSMutableArray *array = [[NSMutableArray alloc]initWithCapacity:0];
    
        if ([output.availableMetadataObjectTypes containsObject:AVMetadataObjectTypeQRCode]) {
            [array addObject:AVMetadataObjectTypeQRCode];
        }
        if ([output.availableMetadataObjectTypes containsObject:AVMetadataObjectTypeEAN13Code]) {
            [array addObject:AVMetadataObjectTypeEAN13Code];
        }
        if ([output.availableMetadataObjectTypes containsObject:AVMetadataObjectTypeEAN8Code]) {
            [array addObject:AVMetadataObjectTypeEAN8Code];
        }
        if ([output.availableMetadataObjectTypes containsObject:AVMetadataObjectTypeCode128Code]) {
            [array addObject:AVMetadataObjectTypeCode128Code];
        }
        output.metadataObjectTypes = array;
    
    }
    AVCaptureVideoPreviewLayer *layer = [AVCaptureVideoPreviewLayer layerWithSession:_session];
    layer.videoGravity = AVLayerVideoGravityResizeAspectFill;
    layer.frame = self.bounds;
    [self.layer insertSublayer:layer above:0];
    [self bringSubviewToFront:_scanView];
   [self setOverView];
    [_session startRunning];
    [self loopDrawLine];
}
//设置扫描区域
- (CGRect)rectOfInterestByScanViewRect:(CGRect)rect{
    CGFloat width = CGRectGetWidth(self.frame);
    CGFloat height = CGRectGetHeight(self.frame);
    CGFloat x = (height - CGRectGetHeight(rect))/2/height;
    CGFloat y = (width - CGRectGetWidth(rect))/2/width;
    CGFloat w = CGRectGetHeight(rect)/height;
    CGFloat h = CGRectGetWidth(rect)/width;
    return CGRectMake(x, y, w, h);
}
#pragma mark - 添加模糊效果
- (void)setOverView
{
    CGFloat width = CGRectGetWidth(self.frame);
    CGFloat height = CGRectGetHeight(self.frame);
    CGFloat x = CGRectGetMinX(_scanView.frame);
    CGFloat y = CGRectGetMinY(_scanView.frame);
    CGFloat w = CGRectGetWidth(_scanView.frame);
    CGFloat h = CGRectGetHeight(_scanView.frame);
    [self creatView:CGRectMake(0, 0, width, y)];
    [self creatView:CGRectMake(0, y + h, width, height - y)];
    [self creatView:CGRectMake(0, y, x, h)];
    [self creatView:CGRectMake(x + w, y, x, h)];
}
- (void)creatView:(CGRect)rect{
    CGFloat alpha = 0.5;
    UIView *view = [[UIView alloc]initWithFrame:rect];
    view.backgroundColor = [UIColor grayColor];
    view.alpha = alpha;
    [self addSubview:view];
}
#pragma mark - 动画
- (void)loopDrawLine {
    UIImage *lineImage = [UIImage imageNamed:@"scanLine"];
    CGFloat x = CGRectGetMinX(_scanView.frame);
    CGFloat y = CGRectGetMinY(_scanView.frame);
    CGFloat w = CGRectGetWidth(_scanView.frame);
    CGFloat h = CGRectGetHeight(_scanView.frame);
    CGRect start = CGRectMake(x, y, w, 2);
    CGRect end = CGRectMake(x, y + h - 2, w, 2);
    if (!_lineView) {
        _lineView = [[UIImageView alloc]initWithImage:lineImage];
        _lineView.frame = start;
        [self addSubview:_lineView];
    }else{
        _lineView.frame = start;
    }
    __weak typeof(self) weakSelf = self;
    [UIView animateWithDuration:2 animations:^{
        _lineView.frame = end;
    } completion:^(BOOL finished) {
        [weakSelf loopDrawLine];
    }];
}
#pragma mark - AVCaptureMetadataOutputObjectsDelegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection
{
    if (metadataObjects.count>0) {
        AVMetadataMachineReadableCodeObject *metadataObject = [metadataObjects firstObject];
        if ([_delegate respondsToSelector:@selector(qrView:ScanResult:)]) {
            [_delegate qrView:self ScanResult:metadataObject.stringValue];
        }
    }
}
- (void)startScan{
    _lineView.hidden = NO;
    [_session startRunning];
}
- (void)stopScan{
    _lineView.hidden = YES;
    [_session stopRunning];
}
@end
```
> 
**调用示例:**
- (void)viewDidLoad {
[super viewDidLoad];
self.view.backgroundColor = [UIColor whiteColor];
QRView *qrView = [[QRView alloc]initWithFrame:self.view.bounds];
qrView.delegate = self;
[self.view addSubview:qrView];
}
> 
```
//代理回调扫描结果
- (void)qrView:(QRView *)view ScanResult:(NSString *)result
{
    [view stopScan];
    NSLog(@"扫描结果:%@",result);
}
```
![1485695-7be6980e89453f5f.jpg](https://upload-images.jianshu.io/upload_images/1485695-7be6980e89453f5f.jpg)
扫一扫示例图片
### 生成二维码
生成二维码UIImage+QRCode
> 
UIImage+QRCode.h 文件
```
#import <UIKit/UIKit.h>
@interface UIImage (QRCode)
+ (UIImage *)qrImageByContent:(NSString *)content;
//pre
+ (UIImage *)qrImageWithContent:(NSString *)content size:(CGFloat)size;
/**
 *   色值 0~255
 */
+ (UIImage *)qrImageWithContent:(NSString *)content size:(CGFloat)size red:(NSInteger)red green:(NSInteger)green blue:(NSInteger)blue;
+ (UIImage *)qrImageWithContent:(NSString *)content logo:(UIImage *)logo size:(CGFloat)size red:(NSInteger)red green:(NSInteger)green blue:(NSInteger)blue;
@end
```
> 
UIImage+QRCode.m 文件
```
#import "UIImage+QRCode.h"
#import <AVFoundation/AVFoundation.h>
@implementation UIImage (QRCode)
void ProviderReleaseData (void *info, const void *data, size_t size){
    free((void*)data);
}
+ (CIImage *)qrByContent:(NSString *)content {
    CIFilter *qrFilter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
    NSData *stringData = [content dataUsingEncoding:NSUTF8StringEncoding];
    [qrFilter setValue:stringData forKey:@"inputMessage"];
    [qrFilter setValue:@"H" forKey:@"inputCorrectionLevel"];
    CIImage *image = qrFilter.outputImage;
    return image;
}
+ (UIImage *)qrImageWithContent:(NSString *)content size:(CGFloat)size  {
    CIImage *image = [UIImage qrByContent:content];
    CGRect extent = CGRectIntegral(image.extent);
    CGFloat scale = MIN(size/CGRectGetWidth(extent), size/CGRectGetHeight(extent));
    // 创建bitmap;
    size_t width = CGRectGetWidth(extent) * scale;
    size_t height = CGRectGetHeight(extent) * scale;
    CGColorSpaceRef cs = CGColorSpaceCreateDeviceGray();
    CGContextRef bitmapRef = CGBitmapContextCreate(nil, width, height, 8, 0, cs, (CGBitmapInfo)kCGImageAlphaNone);
    CIContext *context = [CIContext contextWithOptions:nil];
    CGImageRef bitmapImage = [context createCGImage:image fromRect:extent];
    CGContextSetInterpolationQuality(bitmapRef, kCGInterpolationNone);
    CGContextScaleCTM(bitmapRef, scale, scale);
    CGContextDrawImage(bitmapRef, extent, bitmapImage);
    // 保存bitmap到图片
   CGImageRef scaledImage = CGBitmapContextCreateImage(bitmapRef);
    CGContextRelease(bitmapRef);
    CGImageRelease(bitmapImage);
    return [UIImage imageWithCGImage:scaledImage];
}
+ (UIImage *)qrImageWithContent:(NSString *)content size:(CGFloat)size red:(NSInteger)red green:(NSInteger)green blue:(NSInteger)blue {
    UIImage *image = [UIImage qrImageWithContent:content size:size];
    const int imageWidth = image.size.width;
    const int imageHeight = image.size.height;
    size_t      bytesPerRow = imageWidth * 4;
    uint32_t* rgbImageBuf = (uint32_t*)malloc(bytesPerRow * imageHeight);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(rgbImageBuf, imageWidth, imageHeight, 8, bytesPerRow, colorSpace,  kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipLast);
    CGContextDrawImage(context, CGRectMake(0, 0, imageWidth, imageHeight), image.CGImage);
    // 遍历像素
    int pixelNum = imageWidth * imageHeight;
    uint32_t* pCurPtr = rgbImageBuf;
    for (int i = 0; i < pixelNum; i++, pCurPtr++){
        if ((*pCurPtr & 0xFFFFFF00) < 0x99999900)    // 将白色变成透明
            {
            // 改成下面的代码，会将图片转成想要的颜色
            uint8_t* ptr = (uint8_t*)pCurPtr;
            ptr[3] = red; //0~255
            ptr[2] = green;
            ptr[1] = blue;
        }
        else
        {
            uint8_t* ptr = (uint8_t*)pCurPtr;
            ptr[0] = 0;
        }
    }    
    // 输出图片
    CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, rgbImageBuf, bytesPerRow * imageHeight, ProviderReleaseData);
    CGImageRef imageRef = CGImageCreate(imageWidth, imageHeight, 8, 32, bytesPerRow, colorSpace,
                                    kCGImageAlphaLast | kCGBitmapByteOrder32Little, dataProvider,
                                    NULL, true, kCGRenderingIntentDefault);
    CGDataProviderRelease(dataProvider);
    UIImage* resultUIImage = [UIImage imageWithCGImage:imageRef];
   // 清理空间
    CGImageRelease(imageRef);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    return resultUIImage;
}
+ (UIImage *)qrImageWithContent:(NSString *)content logo:(UIImage *)logo size:(CGFloat)size red:(NSInteger)red green:(NSInteger)green blue:(NSInteger)blue {
    UIImage* resultUIImage = [self qrImageWithContent:content size:size red:red green:green blue:blue];
    // 添加logo
    CGFloat logoW = resultUIImage.size.width / 5.;
    CGRect logoFrame = CGRectMake(logoW * 2, logoW * 2, logoW, logoW);
    UIGraphicsBeginImageContext(resultUIImage.size);
    [resultUIImage drawInRect:CGRectMake(0, 0, resultUIImage.size.width, resultUIImage.size.height)];
    [logo drawInRect:logoFrame];
    UIImage *kk = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return kk;
}
+ (UIImage *)qrImageByContent:(NSString *)content {
    NSData *stringData = [content dataUsingEncoding:NSUTF8StringEncoding];
    CIFilter *qrFilter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
    [qrFilter setValue:stringData forKey:@"inputMessage"];
    [qrFilter setValue:@"M" forKey:@"inputCorrectionLevel"];
  
    CIImage *qrImage = qrFilter.outputImage;
    CGSize size = CGSizeMake(300, 300);
    CGImageRef cgImage = [[CIContext contextWithOptions:nil] createCGImage:qrImage fromRect:qrImage.extent];
    UIGraphicsBeginImageContext(size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetInterpolationQuality(context, kCGInterpolationNone);
    CGContextScaleCTM(context, 1, -1);
    CGContextDrawImage(context, CGContextGetClipBoundingBox(context), cgImage);
    UIImage *codeImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    CGImageRelease(cgImage);
   return codeImage;
}
@end
```
> 
**调用示例:**
```
UIImageView *imgView = [[UIImageView alloc] initWithFrame:CGRectZero];
imgView.frame = CGRectMake(0, 0, 200, 200);
imgView.center = self.view.center;
imgView.layer.shadowColor = [UIColor blackColor].CGColor;
imgView.layer.shadowOffset = CGSizeMake(1, 2);
imgView.layer.shadowRadius = 1;
imgView.layer.shadowOpacity = 0.5;
[self.view addSubview:imgView];
```
> 
```
imgView.image = [UIImage qrImageByContent:@"www.baidu.com"];
```
![1485695-a7562582df28814b.jpg](https://upload-images.jianshu.io/upload_images/1485695-a7562582df28814b.jpg)
生成的二维码示例
项目地址:[https://github.com/524429264/QRCodeDemo](https://link.jianshu.com?t=https://github.com/524429264/QRCodeDemo)
### 扫描下方二维码关注我###
![1485695-3eca430266100927.png](https://upload-images.jianshu.io/upload_images/1485695-3eca430266100927.png)
零距离仰望星空
##### 在此感谢各位读者的来访,您的关注是我写作分享的最大动力。
