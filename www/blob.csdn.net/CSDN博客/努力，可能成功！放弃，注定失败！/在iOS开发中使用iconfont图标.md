# 在iOS开发中使用iconfont图标 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月01日 09:21:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1100标签：[iOS 资源大全																[iOS学习资料																[iOS																[在iOS开发中使用iconfont图标](https://so.csdn.net/so/search/s.do?q=在iOS开发中使用iconfont图标&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
在开发iOS项目时，不可避免的会用到图标，而为了适配不同分辨率的设备，我们通常会需要@2x，@3x两套格式的图片，最明显的就是底部tabBar的图标使用。而对于那些有换肤需求的APP来说，还需要多套图来匹配不同的主题。通过切图的方式制作图标，一方面加大了开发者和设计者的工作量，另一方面也会增大APP的体积。而使用iconfont的可以达到以下目的
1.减小应用体积，字体文件比图片要小；
2.图标保真缩放，解决2x/3x乃至将来nx图问题；
3.方便更改图标颜色大小，图片复用。
所以为了给开发者、设计者稍微减少点工作量，给APP“减重，我们可以将iconfont应用到自己的项目中。那么，iconfont是怎么用的呢？iconfont，从字面上就能理解它就是字体，让开发者像使用字体一样使用图标。
由于我是做开发的，所以对于iconfont的制作并不太熟悉，都是设计师做好了图标给我，如果你想学习iconfont的制作的话，可以去阿里巴巴的iconfont平台去看看，上面有比较全的资料。制作好的iconfont图标是一种.ttf格式的字体，如图：
![](http://upload-images.jianshu.io/upload_images/972702-f968cbc50c87c570.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iconfont图标
iconfont中的图标是这样的：
![](http://upload-images.jianshu.io/upload_images/972702-cdce2d146d92fd7a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iconfont中的图标
而我们需要的是将.ttf格式的文件引入到自己的工程中
接下来我们借助淘点点科技写的一个关于iconfont封装，方便我们使用iconfont。iconfont的封装包括
![](http://upload-images.jianshu.io/upload_images/972702-99b9bc03f202f78f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iconfont
1.TBCityIconInfo.h的实现
```
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@interface TBCityIconInfo : NSObject
@property (nonatomic, strong) NSString *text;
@property (nonatomic, assign) NSInteger size;
@property (nonatomic, strong) UIColor *color;
- (instancetype)initWithText:(NSString *)text size:(NSInteger)size color:(UIColor *)color;
+ (instancetype)iconInfoWithText:(NSString *)text size:(NSInteger)size color:(UIColor *)color;
@end
```
2.TBCityFontImageInfo.m的实现
```
#import "TBCityIconInfo.h"
@implementation TBCityIconInfo
- (instancetype)initWithText:(NSString *)text size:(NSInteger)size color:(UIColor *)color {
    if (self = [super init]) {
        self.text = text;
        self.size = size;
        self.color = color;
    }
    return self;
}
+ (instancetype)iconInfoWithText:(NSString *)text size:(NSInteger)size color:(UIColor *)color {
    return [[TBCityIconInfo alloc] initWithText:text size:size color:color];
}
@end
```
3.TBCityIconFont.h的实现
```
#import "UIImage+TBCityIconFont.h"
#import "TBCityIconInfo.h"
#define TBCityIconInfoMake(text, imageSize, imageColor) [TBCityIconInfo iconInfoWithText:text size:imageSize color:imageColor]
@interface TBCityIconFont : NSObject
+ (UIFont *)fontWithSize: (CGFloat)size;
+ (void)setFontName:(NSString *)fontName;
@end
```
4.TBCityIconFont.m的实现
```
#import "TBCityIconFont.h"
#import <CoreText/CoreText.h>
@implementation TBCityIconFont
static NSString *_fontName;
+ (void)registerFontWithURL:(NSURL *)url {
    NSAssert([[NSFileManager defaultManager] fileExistsAtPath:[url path]], @"Font file doesn't exist");
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithURL((__bridge CFURLRef)url);
    CGFontRef newFont = CGFontCreateWithDataProvider(fontDataProvider);
    CGDataProviderRelease(fontDataProvider);
    CTFontManagerRegisterGraphicsFont(newFont, nil);
    CGFontRelease(newFont);
}
+ (UIFont *)fontWithSize:(CGFloat)size {
    UIFont *font = [UIFont fontWithName:[self fontName] size:size];
    if (font == nil) {
        NSURL *fontFileUrl = [[NSBundle mainBundle] URLForResource:[self fontName] withExtension:@"ttf"];
        [self registerFontWithURL: fontFileUrl];
        font = [UIFont fontWithName:[self fontName] size:size];
        NSAssert(font, @"UIFont object should not be nil, check if the font file is added to the application bundle and you're using the correct font name.");
    }
    return font;
}
+ (void)setFontName:(NSString *)fontName {
    _fontName = fontName;
}
+ (NSString *)fontName {
    return _fontName ? : @"iconfont";
}
@end
```
5.UIImage+TBCityIconFont.h的实现
```
#import <UIKit/UIKit.h>
#import "TBCityIconInfo.h"
@interface UIImage (TBCityIconFont)
+ (UIImage *)iconWithInfo:(TBCityIconInfo *)info;
@end
```
6.UIImage+TBCityIconFont.m
```
#import "UIImage+TBCityIconFont.h"
#import "TBCityIconFont.h"
#import <CoreText/CoreText.h>
@implementation UIImage (TBCityIconFont)
+ (UIImage *)iconWithInfo:(TBCityIconInfo *)info {
    CGFloat size = info.size;
    CGFloat scale = [UIScreen mainScreen].scale;
    CGFloat realSize = size * scale;
    UIFont *font = [TBCityIconFont fontWithSize:realSize];
    UIGraphicsBeginImageContext(CGSizeMake(realSize, realSize));
    CGContextRef context = UIGraphicsGetCurrentContext();
    if ([info.text respondsToSelector:@selector(drawAtPoint:withAttributes:)]) {
        /**
         * 如果这里抛出异常，请打开断点列表，右击All Exceptions -> Edit Breakpoint -> All修改为Objective-C
         * See: http://stackoverflow.com/questions/1163981/how-to-add-a-breakpoint-to-objc-exception-throw/14767076#14767076
         */
        [info.text drawAtPoint:CGPointZero withAttributes:@{NSFontAttributeName:font, NSForegroundColorAttributeName: info.color}];
    } else {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        CGContextSetFillColorWithColor(context, info.color.CGColor);
        [info.text drawAtPoint:CGPointMake(0, 0) withFont:font];
#pragma clang pop
    }
    UIImage *image = [UIImage imageWithCGImage:UIGraphicsGetImageFromCurrentImageContext().CGImage scale:scale orientation:UIImageOrientationUp];
    UIGraphicsEndImageContext();
    return image;
}
@end
```
7.在AppDelegate.m中，初始化我们的iconfont
```
#import "AppDelegate.h"
#import "TBCityIconFont.h"
#import "ViewController.h"
@interface AppDelegate ()
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
   //iconfont图标
    [TBCityIconFont setFontName:@"iconfont"];
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:[ViewController new]];
    _window.rootViewController = nav;
    // Override point for customization after application launch.
    return YES;
}
```
8.在ViewController.m中实现
```
#import "ViewController.h"
#import "TBCityIconFont.h"
#import "UIImage+TBCityIconFont.h"
@interface ViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationController.navigationBar.translucent = NO;
    UIBarButtonItem *leftBarButton = [[UIBarButtonItem alloc] initWithImage:[ UIImage iconWithInfo:TBCityIconInfoMake(@"\U0000e602",22,[UIColor colorWithRed:0.55 green:0.55 blue:0.55 alpha:1])] style:UIBarButtonItemStylePlain target:self action:@selector(leftButtonAction)];
    self.navigationItem.leftBarButtonItem = leftBarButton;
    self.navigationItem.leftBarButtonItem.tintColor = [UIColor colorWithRed:0.55 green:0.55 blue:0.55 alpha:1];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithImage:[UIImage iconWithInfo:TBCityIconInfoMake(@"\U0000e60d",25, [UIColor colorWithRed:0.14 green:0.61 blue:0.83 alpha:1.00])] style:UIBarButtonItemStylePlain target:self action:@selector(rightButtonAction)];
     self.navigationItem.rightBarButtonItem.tintColor = [UIColor colorWithRed:0.14 green:0.61 blue:0.83 alpha:1.00];
    // Do any additional setup after loading the view, typically from a nib.
}
-(void)loadView{
    [super loadView];
//    imageView
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(100, 50, 30, 30)];
    [self.view addSubview:imageView];
//图标编码是，需要转成\U0000e603
    imageView.image = [UIImage iconWithInfo:TBCityIconInfoMake(@"\U0000e603", 30, [UIColor redColor])];
//    button
    UIButton *button = [UIButton buttonWithType:UIButtonTypeSystem];
    button.frame = CGRectMake(100, 100, 40, 40);
    [self.view addSubview:button];
    [button setImage:[UIImage iconWithInfo:TBCityIconInfoMake(@"\U0000e60c", 40, [UIColor redColor])] forState:UIControlStateNormal];
//    label,label可以将文字与图标结合一起，直接用label的text属性将图标显示出来
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(50, 160, 280, 40)];
    [self.view addSubview:label];
    label.font = [UIFont fontWithName:@"iconfont" size:15];//设置label的字体
    label.text = @"这是用label显示的iconfont  \U0000e60c";
}
-(void)leftButtonAction{
}
-(void)rightButtonAction{
}
```
9.运行得到的结果如图：
![](http://upload-images.jianshu.io/upload_images/972702-2933282dacc2ff7e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iconfont图标示例
这样，我们就可以很方便的使用iconfont图标了。这里要注意的是，图标是用的iconfont中的图标用的是unicode编码，我们在自己的工程中时需要将&#xXXXX格式转换成\UXXXXXXXX格式。
文／若锦（简书作者）
原文链接：http://www.jianshu.com/p/3b10bb95b332
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
