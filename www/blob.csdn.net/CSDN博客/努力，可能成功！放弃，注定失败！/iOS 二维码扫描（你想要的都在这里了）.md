# iOS 二维码扫描（你想要的都在这里了） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月09日 14:12:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：17221
> 
以前就写过二维码扫描的文章，今天难得抽出来时间重新整理了一下，把所有用都的关于二维码的都写在这了，二维码问题，看了这一篇文章就什么都解决了
# 原生二维码扫描
> 
个人是比较支持用原生二维码扫描的，这里也就仅仅以原生二维码扫面为范例。另也有二维码扫描库[ZBarSDK(点这里)](https://cocoapods.org/pods/ZBarSDK)和[ZXingObjC(点这里)](https://cocoapods.org/pods/ZXingObjC)。
1.原生扫描用到的几个类
```
@property (strong,nonatomic)AVCaptureDevice * device;
@property (strong,nonatomic)AVCaptureDeviceInput * input;
@property (strong,nonatomic)AVCaptureMetadataOutput * output;
@property (strong,nonatomic)AVCaptureSession * session;
@property (strong,nonatomic)AVCaptureVideoPreviewLayer * preview;
```
2.在viewDidLoad里创建它们
```
// Device
_device = [AVCaptureDevicedefaultDeviceWithMediaType:AVMediaTypeVideo];
// Input
_input = [AVCaptureDeviceInputdeviceInputWithDevice:self.deviceerror:nil];
// Output
_output = [[AVCaptureMetadataOutputalloc]init];
[_outputsetMetadataObjectsDelegate:selfqueue:dispatch_get_main_queue()];
// Session
_session = [[AVCaptureSessionalloc]init];
[_sessionsetSessionPreset:AVCaptureSessionPresetHigh];
```
连接输入和输出
```
if ([_sessioncanAddInput:self.input])
{
    [_sessionaddInput:self.input];
}
if ([_sessioncanAddOutput:self.output])
{
    [_sessionaddOutput:self.output];
}
```
设置条码类型
`_output.metadataObjectTypes =@[AVMetadataObjectTypeQRCode];`
添加扫描画面
```
_preview =[AVCaptureVideoPreviewLayerlayerWithSession:_session];
_preview.videoGravity =AVLayerVideoGravityResizeAspectFill;
_preview.frame =self.view.layer.bounds;
[self.view.layerinsertSublayer:_previewatIndex:0];
```
开始扫描
`[_sessionstartRunning];`
最后实现协议AVCaptureMetadataOutputObjectsDelegate
```
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection
{
   NSString *stringValue;
   if ([metadataObjectscount] >0){
   //停止扫描
   [_sessionstopRunning];        
   AVMetadataMachineReadableCodeObject * metadataObject = [metadataObjectsobjectAtIndex:0];
   stringValue = metadataObject.stringValue;        
  }
}
```
到了这一步就可以成功扫描二维码了
> 
还有一个小问题需要说明一下，之前写过一篇限制二维码扫描区域的，在这里稍微提一下。如下图微信扫描，把扫描范围限制在中间的方框内
![](http://upload-images.jianshu.io/upload_images/1951020-9b0ed5f0c4b0cc19.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
微信二维码扫描
要想限制二维码扫描区域，需要设置一个参数rectOfInterest 这个参数有点特别，这个参数的react 与平常设置的坐标系是完全相反的，即X与Y互换、W与H互换。
设置的方法
`  [_output setRectOfInterest:CGRectMake()];`
如果你感到疑惑那么可以看一下我写的这篇文章，里面有详细研究。
> 
###### [iOS 原生二维码扫描（可限制扫描区域）](http://blog.csdn.net/lc_obj/article/details/41549469)
# 生成二维码图片
> 
生成二维码和扫描二维码图片就比较简单了，自己也写了个工具类来处理，这里直接贴出来代码，后面也会有代码可以下载
在这里借鉴的有别人的文章，想看原文[点这里](https://blog.yourtion.com/custom-cifilter-qrcode-generator.html)
##### 首先LCQrcodeUtil.h中
```
/**
 *  生成二维码图片
 *
 *  @param QRString  二维码内容
 *  @param sizeWidth 图片size（正方形）
 *  @param color     填充色
 *
 *  @return  二维码图片
 */
+(UIImage *)createQRimageString:(NSString *)QRString sizeWidth:(CGFloat)sizeWidth fillColor:(UIColor *)color;
```
##### LCQrcodeUtil.m中
```
#pragma mark 生成二维码
/**
 *  生成二维码图片
 *
 *  @param QRString  二维码内容
 *  @param sizeWidth 图片size（正方形）
 *  @param color     填充色
 *
 *  @return  二维码图片
 */
+(UIImage *)createQRimageString:(NSString *)QRString sizeWidth:(CGFloat)sizeWidth fillColor:(UIColor *)color{
CIImage *ciimage = [self createQRForString:QRString];
UIImage *qrcode = [self createNonInterpolatedUIImageFormCIImage:ciimage withSize:sizeWidth];
if (color) {
    CGFloat R, G, B;
    CGColorRef colorRef = [color CGColor];
    long numComponents = CGColorGetNumberOfComponents(colorRef);
    if (numComponents == 4)
    {
        const CGFloat *components = CGColorGetComponents(colorRef);
        R = components[0];
        G = components[1];
        B = components[2];
    }
    UIImage *customQrcode = [self imageBlackToTransparent:qrcode withRed:R andGreen:G andBlue:B];
    return customQrcode;
}
return qrcode;
}
```
##### pragma mark
```
#pragma mark - QRCodeGenerator
+ (CIImage *)createQRForString:(NSString *)qrString {
// Need to convert the string to a UTF-8 encoded NSData object
NSData *stringData = [qrString dataUsingEncoding:NSUTF8StringEncoding];
// Create the filter
CIFilter *qrFilter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
// Set the message content and error-correction level
[qrFilter setValue:stringData forKey:@"inputMessage"];
[qrFilter setValue:@"M" forKey:@"inputCorrectionLevel"];
// Send the image back
return qrFilter.outputImage;
}
```
##### pragma mark
```
#pragma mark - InterpolatedUIImage
+ (UIImage *)createNonInterpolatedUIImageFormCIImage:(CIImage *)image withSize:(CGFloat) size {
CGRect extent = CGRectIntegral(image.extent);
CGFloat scale = MIN(size/CGRectGetWidth(extent), size/CGRectGetHeight(extent));
// create a bitmap image that we'll draw into a bitmap context at the desired size;
size_t width = CGRectGetWidth(extent) * scale;
size_t height = CGRectGetHeight(extent) * scale;
CGColorSpaceRef cs = CGColorSpaceCreateDeviceGray();
CGContextRef bitmapRef = CGBitmapContextCreate(nil, width, height, 8, 0, cs, (CGBitmapInfo)kCGImageAlphaNone);
CIContext *context = [CIContext contextWithOptions:nil];
CGImageRef bitmapImage = [context createCGImage:image fromRect:extent];
CGContextSetInterpolationQuality(bitmapRef, kCGInterpolationNone);
CGContextScaleCTM(bitmapRef, scale, scale);
CGContextDrawImage(bitmapRef, extent, bitmapImage);
// Create an image with the contents of our bitmap
CGImageRef scaledImage = CGBitmapContextCreateImage(bitmapRef);
// Cleanup
CGContextRelease(bitmapRef);
CGImageRelease(bitmapImage);
return [UIImage imageWithCGImage:scaledImage];
}
```
###### pragma mark
```
#pragma mark - imageToTransparent
void ProviderReleaseData (void *info, const void *data, size_t size){
free((void*)data);
}
+ (UIImage*)imageBlackToTransparent:(UIImage*)image withRed:(CGFloat)red andGreen:(CGFloat)green andBlue:(CGFloat)blue{
const int imageWidth = image.size.width;
const int imageHeight = image.size.height;
size_t      bytesPerRow = imageWidth * 4;
uint32_t* rgbImageBuf = (uint32_t*)malloc(bytesPerRow * imageHeight);
// create context
CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
CGContextRef context = CGBitmapContextCreate(rgbImageBuf, imageWidth, imageHeight, 8, bytesPerRow, colorSpace,
                                             kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipLast);
CGContextDrawImage(context, CGRectMake(0, 0, imageWidth, imageHeight), image.CGImage);
// traverse pixe
int pixelNum = imageWidth * imageHeight;
uint32_t* pCurPtr = rgbImageBuf;
for (int i = 0; i < pixelNum; i++, pCurPtr++){
    if ((*pCurPtr & 0xFFFFFF00) < 0x99999900){
        // change color
        uint8_t* ptr = (uint8_t*)pCurPtr;
        ptr[3] = red; //0~255
        ptr[2] = green;
        ptr[1] = blue;
    }else{
        uint8_t* ptr = (uint8_t*)pCurPtr;
        ptr[0] = 0;
    }
}
// context to image
CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, rgbImageBuf, bytesPerRow * imageHeight, ProviderReleaseData);
CGImageRef imageRef = CGImageCreate(imageWidth, imageHeight, 8, 32, bytesPerRow, colorSpace,
                                    kCGImageAlphaLast | kCGBitmapByteOrder32Little, dataProvider,
                                    NULL, true, kCGRenderingIntentDefault);
CGDataProviderRelease(dataProvider);
UIImage* resultUIImage = [UIImage imageWithCGImage:imageRef];
// release
CGImageRelease(imageRef);
CGContextRelease(context);
CGColorSpaceRelease(colorSpace);
return resultUIImage;
}
```
# 解析二维码图片
> 
还是以源码的形式贴出来
这里也有借鉴别人的代码，[点这里](http://www.jianshu.com/p/bfa9e4e24a1a) 查看原文
##### 首先LCQrcodeUtil.h中
```
```
/**
 *  读取图片中二维码信息
 *
 *  @param image 图片
 *
 *  @return 二维码内容
 */
+(NSString *)readQRCodeFromImage:(UIImage *)image;
```
```
##### LCQrcodeUtil.m中
```
#pragma mark 读取图片二维码
/**
 *  读取图片中二维码信息
 *
 *  @param image 图片
 *
 *  @return 二维码内容
 */
+(NSString *)readQRCodeFromImage:(UIImage *)image{
NSData *data = UIImagePNGRepresentation(image);
CIImage *ciimage = [CIImage imageWithData:data];
if (ciimage) {
    CIDetector *qrDetector = [CIDetector detectorOfType:CIDetectorTypeQRCode context:[CIContext contextWithOptions:@{kCIContextUseSoftwareRenderer:@(YES)}] options:@{CIDetectorAccuracy : CIDetectorAccuracyHigh}];
    NSArray *resultArr = [qrDetector featuresInImage:ciimage];
    if (resultArr.count >0) {
        CIFeature *feature = resultArr[0];
        CIQRCodeFeature *qrFeature = (CIQRCodeFeature *)feature;
        NSString *result = qrFeature.messageString;
        return result;
    }else{
        return nil;
    }
}else{
    return nil;
}
```
}
都是直接封装成类方法调用，这个类我也已经上传到git，不想CV的可以到git中下载，地址：
[https://github.com/liutongchao/LCQRCodeUtil](https://github.com/liutongchao/LCQRCodeUtil)
#### 有问题欢迎指正以及互相探讨 -- LC.West
文／就叫West怎么了（简书作者）
原文链接：http://www.jianshu.com/p/1919b240387b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
