# 更好用的仿微信、QQ弹框控件 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月26日 17:15:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：476
本文来自简书，原文地址:[http://www.jianshu.com/p/f057a2250c68](http://www.jianshu.com/p/f057a2250c68)
先上效果图
![](http://upload-images.jianshu.io/upload_images/1003160-0f8dcf28ecfea443.gif?imageMogr2/auto-orient/strip)
Untitled.gif
前些时间在项目开发中用到这个带箭头的弹出框功能，为了开发进度，就随便在gitHub上找了个轮子使用了，也能实现我想要的功能，但是就是感觉不是想要的那种封装方式，使用起来比较麻烦，能不能像使用系统框架UIMenuController那样方便的使用这一控件呢？抱着这个想法，闲暇时间开始了HHPopController的探索
首先这个自定义控件使用起来非常简单方便，且可定制性很高，你只需要像下面这样使用
```
HHPopItem *item1 = [HHPopItem itemWithImage:[UIImage imageNamed:@"ic_addvoucher_qrcode"] title:@"创建群聊"];
    HHPopItem *item2 = [HHPopItem itemWithImage:[UIImage imageNamed:@"ic_addvoucher_cinema"] title:@"加好友/群"];
    HHPopItem *item3 = [HHPopItem itemWithImage:[UIImage imageNamed:@"ic_addvoucher_input"] title:@"面对面快传"];
    [HHPopController applyPopStyle:^(HHPopStyle *style) {
        style.borderColor = [UIColor cyanColor];
        style.borderWidth = 1.0f;
        style.itemTextColor = [UIColor whiteColor];
        style.dimColor = [UIColor colorWithWhite:0.1 alpha:0.3];
        style.itemBgColor = [UIColor colorWithWhite:0.3 alpha:1.0];
//        HHPopSupplementaryStyle *header = [HHPopSupplementaryStyle defaultStyle];
//        header.bgColor = style.itemBgColor;
//        header.title = @"header";
//        header.titleColor = [UIColor whiteColor];
//        style.header = header;
//        
//        HHPopSupplementaryStyle *footer = [HHPopSupplementaryStyle defaultStyle];
//        footer.bgColor = style.itemBgColor;
//        footer.title = @"footer";
//        footer.titleColor = [UIColor whiteColor];
//        style.footer = footer;
    }];
    [HHPopController popSourceView:btn popItems:@[item1, item2, item3] selectionHandler:^(NSInteger idx, HHPopItem *item) {
        NSLog(@"%@", item.title);
    }];
```
先来看下类HHPopItem的定义
```
/// 弹出视图数据模型
@interface HHPopItem : NSObject
+ (instancetype)itemWithTitle:(NSString *)title;
+ (instancetype)itemWithImage:(UIImage *)image title:(NSString *)title;
@property (nonatomic, strong)UIImage *image;
@property (nonatomic, strong)NSString *title;
- (void)setHandler:(void (^)(NSInteger idx, HHPopItem *item))handler;
@end
```
这里你可以创建只含有标题或者含有图片和标题两种样式，并且可以通过block配置点击的回调
方法+ (void)applyPopStyle:(void (^)(HHPopStyle *style))maker;通过block传进一个模型来配置该控件所有的样式，可定制的属性如下
```
/// 弹出视图样式模型
@interface HHPopStyle : NSObject
/// 弹出视图的宽度
@property (nonatomic, assign)CGFloat popWidth;
/// 边框颜色
@property (nonatomic, strong)UIColor *borderColor;
/// 边框宽度
@property (nonatomic, assign)CGFloat borderWidth;
/// 行高
@property (nonatomic, assign)CGFloat rowHeight;
/// 圆角尺寸
@property (nonatomic, assign)CGFloat cornerRadius;
/// 蒙版颜色
@property (nonatomic, strong)UIColor *dimColor;
/// 箭头方向
@property (nonatomic, assign)HHArrowStyle arrowStyle;
/// 箭头离矩形边框的距离大小
@property (nonatomic, assign)CGFloat arrowDistanceFromTargetRectBorder;
/// 箭头尺寸, 对应三角形的宽高 default {15, 8}
@property (nonatomic, assign)CGSize arrowSize;
/// 弹框显现的区域限制
@property (nonatomic, assign)UIEdgeInsets popAreaEdgeLimits;
/// 出现的动画时间 default 0.25
@property (nonatomic, assign)CGFloat animationIn;
/// 小时的动画时间 default 0.25
@property (nonatomic, assign)CGFloat animationOut;
/// 是否能滚动 当弹框范围能够容纳的下为NO, 否则为YES
@property (nonatomic, assign)BOOL bounces;
/// 分割线颜色
@property (nonatomic, strong)UIColor *separatorColor;
/// 分割线内边距
@property (nonatomic, assign)UIEdgeInsets separatorInset;
/// 选项的背景颜色
@property (nonatomic, strong)UIColor *itemBgColor;
/// 选项的图片内容模式
@property (nonatomic, assign)UIViewContentMode itemImageContentMode;
/// 选项文字对齐方式
@property (nonatomic, assign)NSTextAlignment itemTextAligment;
/// 选项文字字体
@property (nonatomic, strong)UIFont *itemTextFont;
/// 选项文字颜色
@property (nonatomic, strong)UIColor *itemTextColor;
/// 头部样式
@property (nonatomic, strong)HHPopSupplementaryStyle *header;
/// 尾部样式
@property (nonatomic, strong)HHPopSupplementaryStyle *footer;
+ (instancetype)defaultStyle;
@end
```
看注释应该很明显了，其中这里还可以通过HHPopSupplementaryStyle模型来配置头部数据和尾部数据，其实就是UITableView的header和footer，HHPopSupplementaryStyle的定义如下
```
/// 弹出视图的追加视图模型 (header and footer)
@interface HHPopSupplementaryStyle : NSObject
/// 标题
@property (nonatomic, copy)NSString *title;
/// 标题高度
@property (nonatomic, assign)CGFloat height;
/// 标题文字对齐方式
@property (nonatomic, assign)NSTextAlignment titleAligment;
/// 标题文字颜色
@property (nonatomic, strong)UIColor *titleColor;
/// 标题背景颜色
@property (nonatomic, assign)UIColor *bgColor;
/// 标题文字字体
@property (nonatomic, strong)UIFont *titleFont;
/// 自定义头部视图
@property (nonatomic, strong)UIView *customView;
+ (instancetype)defaultStyle;
@end
```
HHArrowStyle是个枚举类型
```
/// 箭头方式
typedef NS_ENUM(NSInteger, HHArrowStyle) {
    HHArrowStyleVerticalDefault = 0, ///< up or down based on source view location in window
    HHArrowStyleHorizontalDefault, ///< left or right based on source view location in window
    HHArrowStyleUp,///< up force
    HHArrowStyleDown,/// < down force
    HHArrowStyleLeft,///< left force
    HHArrowStyleRight///< right force
};
```
如果不需要显示箭头可以设置arrowSize为CGSizeZero。
最后我们在看下HHPopController的定义
```
/// 弹出视图控制器
@interface HHPopController : NSObject
+ (void)applyReturnPopStyle:(HHPopStyle *(^)(HHPopStyle *style))maker;
+ (void)applyPopStyle:(void (^)(HHPopStyle *style))maker;
+ (void)popSourceView:(UIView *)view popItems:(NSArray<HHPopItem *> *)items selectionHandler:(void (^)(NSInteger idx, HHPopItem *item))handler;
+ (void)popTargetRect:(CGRect)rect soureceView:(UIView *)view popItems:(NSArray<HHPopItem *> *)items selectionHandler:(void (^)(NSInteger idx, HHPopItem *item))handler;
+ (BOOL)isPopVisible;
+ (void)dismiss;
@end
```
配置回调有两种方式，可以通过模型，也可以通过handler参数，使用起来是不是简单明了？既然封装就要封装的彻底点，外界怎么用着舒服就怎么来，嘿嘿，通过模型来配置一些属性，避开了要写一堆的代理方法或者block，但还是有一些缺点的，比如动画支持不是很丰富等
另外还可以通过以下通知名监听对应的通知
```
FOUNDATION_EXTERN NSString *const HHPopControllerWillPopNotification;
FOUNDATION_EXTERN NSString *const HHPopControllerDidPopNotification;
FOUNDATION_EXTERN NSString *const HHPopControllerWillHidenNotification;
FOUNDATION_EXTERN NSString *const HHPopControllerDidHidenNotification;
```
可以通过cocopods安装使用
`pod 'HHPopController'`
好了，功能就这么多，有兴趣的可以看下源码，也很好理解，有什么问题或建议欢迎留言，如果能在开发中帮助到你也算没有白忙活，谢谢大家，喜欢的麻烦给个赞呗！
gitHub传送门[https://github.com/theSkyOfJune/HHPopController](https://github.com/theSkyOfJune/HHPopController)
