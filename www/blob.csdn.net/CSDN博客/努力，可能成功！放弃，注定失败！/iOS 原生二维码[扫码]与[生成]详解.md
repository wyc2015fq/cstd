# iOS 原生二维码[扫码]与[生成]详解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月10日 22:36:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1509
## 本文来自简书，原文地址:[http://www.jianshu.com/p/2180e940a589](http://www.jianshu.com/p/2180e940a589)
## 前言
二维码现在很常见, App中有许多都配置了该功能, 网上也有很多对iOS二维码的讲解, 但是对配置扫码范围这个问题好像都没怎么讲清晰. 作者今天就写一下二维码.
## 一 二维码介绍
![](http://upload-images.jianshu.io/upload_images/2368070-28a0c6e0683fa624.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
二维码
三个回形大方块, 是为了给相机定位的; 黑白块, 黑块代表1, 白块代表0, 八个一组, 组成二进制信息. [科普：二维码是什么原理？](http://url.cn/2CiHjdt) 这个小视频, 简单介绍了下二维码.
![](http://upload-images.jianshu.io/upload_images/2368070-e8e04e0c88743be7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原理图
## 二 iOS 扫码二维码
- 
一.工程中General -> Linked Frameworks and Libraries -> 引入 AVFoundation.framework
- 
二. 代码部分解析
1) 头文件
```cpp
```cpp
//引入头文件
#import <AVFoundation/AVFoundation.h>
// 作者自定义的View视图, 继承UIView
#import "ShadowView.h"
#define kWidth [UIScreen mainScreen].bounds.size.width
#define kHeight [UIScreen mainScreen].bounds.size.height
#define customShowSize CGSizeMake(200, 200);
```
```
2) 定义属性
```
// ScanCodeViewController是作者创建的VC , 用Navi推出, 写入协议 (UIImagePickerControllerDelegate, UINavigationControllerDelegate 是为了 可以直接扫码图库中的二维码, 在Navi右上角创建button)
@interface ScanCodeViewController ()<AVCaptureMetadataOutputObjectsDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>
/** 输入数据源 */
@property (nonatomic, strong) AVCaptureDeviceInput *input;
/** 输出数据源 */
@property (nonatomic, strong) AVCaptureMetadataOutput *output;
/** 输入输出的中间桥梁 负责把捕获的音视频数据输出到输出设备中 */
@property (nonatomic, strong) AVCaptureSession *session;
/** 相机拍摄预览图层 */
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *layerView;
/** 预览图层尺寸 */
@property (nonatomic, assign) CGSize layerViewSize;
/** 有效扫码范围 */
@property (nonatomic, assign) CGSize showSize;
/** 作者自定义的View视图 */
@property (nonatomic, strong) ShadowView *shadowView;
@end
```
3) 创建二维码扫码
```
-(void)creatScanQR{
/** 创建输入数据源 */
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];  //获取摄像设备
        self.input = [AVCaptureDeviceInput deviceInputWithDevice:device error:nil];  //创建输出流
/** 创建输出数据源 */
        self.output = [[AVCaptureMetadataOutput alloc] init];
        [self.output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];  //设置代理 在主线程里刷新
/** Session设置 */
        self.session = [[AVCaptureSession alloc] init];
        [self.session setSessionPreset:AVCaptureSessionPresetHigh];   //高质量采集
        [self.session addInput:self.input];
        [self.session addOutput:self.output];
        //设置扫码支持的编码格式
        self.output.metadataObjectTypes = @[AVMetadataObjectTypeQRCode,
                                        AVMetadataObjectTypeEAN13Code,
                                        AVMetadataObjectTypeEAN8Code,
                                        AVMetadataObjectTypeCode128Code];
/** 扫码视图 */
         //扫描框的位置和大小
        self.layerView = [AVCaptureVideoPreviewLayer layerWithSession:self.session];
        self.layerView.videoGravity = AVLayerVideoGravityResizeAspectFill;
        self.layerView.frame = CGRectMake(0, 64, self.view.frame.size.width, self.view.frame.size.height - 64);
        // 将扫描框大小定义为属行, 下面会有调用
        self.layerViewSize = CGSizeMake(_layerView.frame.size.width, _layerView.frame.size.height);
}
#pragma mark - 实现代理方法, 完成二维码扫描
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
    if (metadataObjects.count > 0) {
         // 停止动画, 看完全篇记得打开注释, 不然扫描条会一直有动画效果
        //[self.shadowView stopTimer];
        //停止扫描
        [self.session stopRunning];
        AVMetadataMachineReadableCodeObject * metadataObject = [metadataObjects objectAtIndex : 0 ];
        //输出扫描字符串
        NSLog(@"%@",metadataObject.stringValue);
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"提示" message:[NSString stringWithFormat:@"%@", metadataObject.stringValue] delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alert show];
    }
}
```
4) 调用方法
```
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    //调用
    [self creatScanQR];
    //添加拍摄图层
    [self.view.layer addSublayer:self.layerView];
    //开始二维码
    [self.session startRunning];
    // Do any additional setup after loading the view.
}
```
> 
至此 二维码扫码完成 , 你会发现 整个屏幕都可以扫码二维码, 与微信的二维码扫码 差的太多. 下面开始个性化设置
5) 自定义阴影视图层
```
#import <UIKit/UIKit.h>
@interface ShadowView : UIView
@property (nonatomic, assign) CGSize showSize;
- (void)stopTimer;
@end
```
```
#import "ShadowView.h"
@interface ShadowView ()
@property (nonatomic, strong) UIImageView *lineView;
@property (nonatomic, strong) NSTimer *timer;
@end
@implementation ShadowView
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        // 图片下方附上
        self.lineView  = [[UIImageView alloc] init];
        self.lineView.image = [UIImage imageNamed:@"line"];
        [self addSubview:self.lineView];
    }
    return self;
}
-(void)playAnimation{
    [UIView animateWithDuration:2.4 delay:0 options:UIViewAnimationOptionCurveLinear animations:^{
        self.lineView .frame = CGRectMake((self.frame.size.width - self.showSize.width) / 2, (self.frame.size.height + self.showSize.height) / 2, self.showSize.width, 2);
    } completion:^(BOOL finished) {
        self.lineView .frame = CGRectMake((self.frame.size.width - self.showSize.width) / 2, (self.frame.size.height - self.showSize.height) / 2, self.showSize.width, 2);
    }];
}
- (void)stopTimer
{
    [_timer invalidate];
    _timer = nil;    
}
-(void)layoutSubviews{
    [super layoutSubviews];
    self.lineView .frame = CGRectMake((self.frame.size.width - self.showSize.width) / 2, (self.frame.size.height - self.showSize.height) / 2, self.showSize.width, 2);
    if (!_timer) {
        [self playAnimation];
        /* 自动播放 */
        self.timer = [NSTimer scheduledTimerWithTimeInterval:2.5 target:self selector:@selector(playAnimation) userInfo:nil repeats:YES];
    }
}
-(void)drawRect:(CGRect)rect{
    [super drawRect:rect];
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    // 整体颜色
    CGContextSetRGBFillColor(ctx, 0.15, 0.15, 0.15, 0.6);
    CGContextFillRect(ctx, rect);   //draw the transparent layer
    //中间清空矩形框
    CGRect clearDrawRect = CGRectMake((rect.size.width - self.showSize.width) / 2, (rect.size.height - self.showSize.height) / 2, self.showSize.width, self.showSize.height);
    CGContextClearRect(ctx, clearDrawRect);
    //边框
    CGContextStrokeRect(ctx, clearDrawRect);
    CGContextSetRGBStrokeColor(ctx, 1, 1, 1, 1);  //颜色
    CGContextSetLineWidth(ctx, 0.5);             //线宽
    CGContextAddRect(ctx, clearDrawRect);       //矩形
    CGContextStrokePath(ctx);
    [self addCornerLineWithContext:ctx rect:clearDrawRect];
}
- (void)addCornerLineWithContext:(CGContextRef)ctx rect:(CGRect)rect{
    float cornerWidth = 4.0;
    float cornerLong = 16.0;
    //画四个边角 线宽
    CGContextSetLineWidth(ctx, cornerWidth);
    //颜色
    CGContextSetRGBStrokeColor(ctx, 83 /255.0, 239/255.0, 111/255.0, 1);//绿色
    //左上角
    CGPoint poinsTopLeftA[] = {CGPointMake(rect.origin.x + cornerWidth/2, rect.origin.y),
                               CGPointMake(rect.origin.x + cornerWidth/2, rect.origin.y + cornerLong)};
    CGPoint poinsTopLeftB[] = {CGPointMake(rect.origin.x, rect.origin.y + cornerWidth/2),
                               CGPointMake(rect.origin.x + cornerLong, rect.origin.y + cornerWidth/2)};
    [self addLine:poinsTopLeftA pointB:poinsTopLeftB ctx:ctx];
    //左下角
    CGPoint poinsBottomLeftA[] = {CGPointMake(rect.origin.x + cornerWidth/2, rect.origin.y + rect.size.height - cornerLong),
                                  CGPointMake(rect.origin.x + cornerWidth/2, rect.origin.y + rect.size.height)};
    CGPoint poinsBottomLeftB[] = {CGPointMake(rect.origin.x, rect.origin.y + rect.size.height - cornerWidth/2),
                                  CGPointMake(rect.origin.x + cornerLong, rect.origin.y + rect.size.height - cornerWidth/2)};
    [self addLine:poinsBottomLeftA pointB:poinsBottomLeftB ctx:ctx];
    //右上角
    CGPoint poinsTopRightA[] = {CGPointMake(rect.origin.x+ rect.size.width - cornerLong, rect.origin.y + cornerWidth/2),
                                CGPointMake(rect.origin.x + rect.size.width, rect.origin.y + cornerWidth/2 )};
    CGPoint poinsTopRightB[] = {CGPointMake(rect.origin.x+ rect.size.width - cornerWidth/2, rect.origin.y),
                                CGPointMake(rect.origin.x + rect.size.width- cornerWidth/2, rect.origin.y + cornerLong)};
    [self addLine:poinsTopRightA pointB:poinsTopRightB ctx:ctx];
    //右下角
    CGPoint poinsBottomRightA[] = {CGPointMake(rect.origin.x+ rect.size.width - cornerWidth/2, rect.origin.y+rect.size.height - cornerLong),
                                   CGPointMake(rect.origin.x- cornerWidth/2 + rect.size.width, rect.origin.y +rect.size.height )};
    CGPoint poinsBottomRightB[] = {CGPointMake(rect.origin.x+ rect.size.width - cornerLong, rect.origin.y + rect.size.height - cornerWidth/2),
                                   CGPointMake(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height - cornerWidth/2 )};
    [self addLine:poinsBottomRightA pointB:poinsBottomRightB ctx:ctx];
    CGContextStrokePath(ctx);
}
- (void)addLine:(CGPoint[])pointA pointB:(CGPoint[])pointB ctx:(CGContextRef)ctx {
    CGContextAddLines(ctx, pointA, 2);
    CGContextAddLines(ctx, pointB, 2);
}
@end
```
![](http://upload-images.jianshu.io/upload_images/2368070-6b5c413149f463c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
line.png
注意 : 如果你对应写demo了, 并且真机测试的系统在ios7 以上, 会发现也许扫描线的初始位置会出现问题. 给个小提示 layoutSubviews 被执行了两次. 留个小尾巴. 让大家思考.
传送门 : 上述边角画图方法, 作者引用 [Raul7777](https://github.com/Raul7777/XHScanTool) 并附上链接 , 作者进行了一下小改进 , 并添加注释.
 如果对你有帮助, 请给他点个 Star
补充 : 扫描线动画, 用NSTimer, 偶尔手机处理卡顿下, 就会被坑. 作者看到有用CABasicAnimation 写的, 觉得挺好. 所以 提供下, 该方法
```
-(void)addAnimationAboutScan{
    self.lineView.hidden = NO;
    CABasicAnimation *animation = [ShadowView moveYTime:2.5 fromY:[NSNumber numberWithFloat:0] toY:[NSNumber numberWithFloat:(self.showSize.height-1)] rep:OPEN_MAX];
    [self.lineView.layer addAnimation:animation forKey:@"LineAnimation"];       
}
+ (CABasicAnimation *)moveYTime:(float)time fromY:(NSNumber *)fromY toY:(NSNumber *)toY rep:(int)rep{
    CABasicAnimation *animationMove = [CABasicAnimation animationWithKeyPath:@"transform.translation.y"];
    [animationMove setFromValue:fromY];
    [animationMove setToValue:toY];
    animationMove.duration = time;
    animationMove.delegate = self;
    animationMove.repeatCount  = rep;
    animationMove.fillMode = kCAFillModeForwards;
    animationMove.removedOnCompletion = NO;
    animationMove.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    return animationMove;
}
- (void)removeAnimationAboutScan{
    [self.lineView.layer removeAnimationForKey:@"LineAnimation"];
    self.lineView.hidden = YES;
}
```
6) 回到ScanCodeViewController 配置扫码范围
> 
重点: 这是作者最想讲的 . 附上官方介绍
rectOfInterest Property
A rectangle of interest for limiting the search area for visual metadata.
Discussion
The value of this property is a CGRect value that determines the object’s rectangle of interest for each frame of video.
The rectangle's origin is top left and is relative to the coordinate space of the device providing the metadata.
Specifying a rectangle of interest may improve detection performance for certain types of metadata. Metadata objects whose bounds do not intersect with therectOfInterest will not be returned.
The default value of this property is a rectangle of (0.0, 0.0, 1.0, 1.0).
说明 :
看到矩形的原点是左上角, 但是真正测试 你会发现却是在右上角, 因为扫码默认是横屏, 所以原右上角变成左上角, 原宽变成高, 原高变成宽. 取值是按照 摄像头分辨率 来取的比例 而不是屏幕的宽高比例.
屏幕宽高比例:
iPhone4 : [320 480] ; iPhone5: [320 568 ] ; iPhone 6 : [375 667] ; iPhone 6plus : [414 736].
作者设置 AVCaptureSessionPresetHigh 所以机型分辨率均为 1920×1080. 所以除了iPhone4 基本上 屏幕宽高比 符合 分辨率的比例. 会有些许误差, 但影响不大. 如需支持包含iPhone4的所以机型 需要将 屏幕宽高与分辨率统一. 方法如下. 这样便将 ShadowView 中间清空的矩形框 与 有效扫码范围 对应上了.
```
/** 配置扫码范围 */
-(void)allowScanRect{
    /** 扫描是默认是横屏, 原点在[右上角]
     *  rectOfInterest = CGRectMake(0, 0, 1, 1);
     *  AVCaptureSessionPresetHigh = 1920×1080   摄像头分辨率
     *  需要转换坐标 将屏幕与 分辨率统一
     */
    //剪切出需要的大小位置
    CGRect shearRect = CGRectMake((self.layerViewSize.width - self.showSize.width) / 2,
                                  (self.layerViewSize.height - self.showSize.height) / 2,
                                  self.showSize.height,
                                  self.showSize.height);
    CGFloat deviceProportion = 1920.0 / 1080.0;
    CGFloat screenProportion = self.layerViewSize.height / self.layerViewSize.width;
    //分辨率比> 屏幕比 ( 相当于屏幕的高不够)
    if (deviceProportion > screenProportion) {
        //换算出 分辨率比 对应的 屏幕高
        CGFloat finalHeight = self.layerViewSize.width * deviceProportion;
        // 得到 偏差值
        CGFloat addNum = (finalHeight - self.layerViewSize.height) / 2;
                                              // (对应的实际位置 + 偏差值)  /  换算后的屏幕高
        self.output.rectOfInterest = CGRectMake((shearRect.origin.y + addNum) / finalHeight,
                                                 shearRect.origin.x / self.layerViewSize.width,
                                                 shearRect.size.height/ finalHeight,
                                                 shearRect.size.width/ self.layerViewSize.width);
    }else{
        CGFloat finalWidth = self.layerViewSize.height / deviceProportion;
        CGFloat addNum = (finalWidth - self.layerViewSize.width) / 2;
        self.output.rectOfInterest = CGRectMake(shearRect.origin.y / self.layerViewSize.height,
                                                (shearRect.origin.x + addNum) / finalWidth,
                                                shearRect.size.height / self.layerViewSize.height,
                                                shearRect.size.width / finalWidth);
    }
}
```
7) 读取相册中二维码
> 
注意事项 : ios8 以上系统才开放读取相册二维码功能(CIDetectorTypeQRCode), 所以如iPhone4 该功能实现不了, 需要有判断.
```
#pragma mark - 相册中读取二维码
/* navi按钮实现 */
-(void)takeQRCodeFromPic:(UIBarButtonItem *)leftBar{
    if ([[[UIDevice currentDevice] systemVersion] doubleValue] < 8) {
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"请更新系统至8.0以上!" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alert show];
    }else{
        if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]){
            UIImagePickerController *pickerC = [[UIImagePickerController alloc] init];
            pickerC.delegate = self;
            pickerC.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;  //来自相册
            [self presentViewController:pickerC animated:YES completion:NULL];
        }else{
            UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"设备不支持访问相册，请在设置->隐私->照片中进行设置！" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alert show];
        }
    }
}
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    //1.获取选择的图片
    UIImage *image = info[UIImagePickerControllerEditedImage];
    if (!image) {
        image = info[UIImagePickerControllerOriginalImage];
    }
    //2.初始化一个监测器
    CIDetector*detector = [CIDetector detectorOfType:CIDetectorTypeQRCode context:nil options:@{ CIDetectorAccuracy : CIDetectorAccuracyHigh }];
    [picker dismissViewControllerAnimated:YES completion:^{
        //监测到的结果数组  放置识别完之后的数据
        NSArray *features = [detector featuresInImage:[CIImage imageWithCGImage:image.CGImage]];
        //判断是否有数据（即是否是二维码）
        if (features.count >=1) {
            /**结果对象 */
            CIQRCodeFeature *feature = [features objectAtIndex:0];
            NSString *scannedResult = feature.messageString;
            UIAlertView * alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:scannedResult delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
        }
        else{
            UIAlertView * alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"该图片没有包含二维码！" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
        }
    }];
}
```
8) 重写调用
```
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    //显示范围
     self.showSize = customShowSize;
    //调用
    [self creatScanQR];
    //添加拍摄图层
    [self.view.layer addSublayer:self.layerView];
    //开始二维码
    [self.session startRunning];
    //设置可用扫码范围
    [self allowScanRect];
   //添加上层阴影视图
    self.shadowView = [[ShadowView alloc] initWithFrame:CGRectMake(0, 64, kWidth, kHeight - 64)];
    [self.view addSubview:self.shadowView];
    self.shadowView.showSize = self.showSize;
    //添加扫码相册按钮
     self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"相册中选" style:UIBarButtonItemStylePlain target:self action:@selector(takeQRCodeFromPic:)];
    // Do any additional setup after loading the view.
}
```
8) 权限问题
> 
权限问题 使用相机需要获取相应权限, 如用户未开启, 可以设置提醒, 留个小尾巴, 读者自行设置.
## 三 iOS 二维码生成
[天听云道的 [ iOS ] 生成二维码 系统方法](http://www.jianshu.com/p/5b66a3f28c89) 这篇文章写了, 挺简单, 生成的是高清二维码, 但是 在ios7系统上, 还是不清晰, 也就是说如果 你的程序支持到ios7 该方法有些问题.
[船长_的 iOS的生成二维码（彩色+阴影）](http://www.jianshu.com/p/85e131155b79) 这篇文章写了, 支持ios7 系统, 高清二维码.
所以对二维码生成问题 作者就不赘述了. 对iOS中二维码的讲解完成.
## 四 三方二维码链接
补充下三方传送门 :[https://github.com/MxABC/LBXScan](https://github.com/MxABC/LBXScan)
[https://github.com/TheLevelUp/ZXingObjC](https://github.com/TheLevelUp/ZXingObjC)
