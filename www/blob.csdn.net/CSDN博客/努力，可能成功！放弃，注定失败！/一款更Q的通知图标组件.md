# 一款更Q的通知图标组件 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月26日 17:17:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：747标签：[iOS学习资料																[iOS 资源大全																[ios开发																[通知图标组件](https://so.csdn.net/so/search/s.do?q=通知图标组件&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
本文来自简书，原文地址:[http://www.jianshu.com/p/56051eca3235](http://www.jianshu.com/p/56051eca3235)
“一闪一闪亮晶晶，满天都是小星星”，作为一枚ios开发程序猿，最大的兴奋莫过于看着模拟器实现了自己想要的效果。
废话不多说，进入今天的主题，今天给大家带来了一个开发中常用的通知图标组件，相信你会喜欢的。
先看下效果图
![](http://upload-images.jianshu.io/upload_images/1003160-d231e1cbd14b57ba.gif?imageMogr2/auto-orient/strip)
badge.gif
当看到Blink Animation的时候知道为什么开篇一闪一闪亮晶晶了吧！是不是很Q？
为什么说这是个组件呢？先来看下结构然后在谈谈怎么使用吧
首先有个通知图标基类，所有的自定义通知图标类型都继承自它，以后如果以后想扩展其他样式也很方便啦
```
@interface HHBadge : NSObject {
    __weak UIView *_sourceView;
    __weak UIBarButtonItem *_sourceItem;
}
#pragma mark - setup
/// 快速创建实例
+ (instancetype)badge;
- (void)show;
- (void)hide;
/// 源视图
@property (nonatomic, weak, readonly)UIView *sourceView;
/// 源item
@property (nonatomic, weak, readonly)UIBarButtonItem *sourceItem;
@end
```
基类中的sourceView和sourceItem属性将来就是我们要添加通知图标的控件
我主要实现开发中常用的两种通知图标样式，点样式和数字样式
先看下点样式
```
@interface HHDotBadge : HHBadge
#pragma mark adjustment
/// 配置外观样式
- (void)applyStyle:(void (^)(HHDotBadgeStyle *style))maker;
/// 设置位置
- (void)moveByX:(CGFloat)x Y:(CGFloat)y;
- (void)scaleBy:(CGFloat)scale;
#pragma mark animation
- (void)doAnimationWithType:(HHBadgeAnimationType)type;
@end
```
`- (void)applyStyle:(void (^)(HHDotBadgeStyle *style))maker;`通过block中的style参数配置外观样式
`- (void)moveByX:(CGFloat)x Y:(CGFloat)y;`调整通知图标的位置
`- (void)scaleBy:(CGFloat)scale;`调整通知图标的大小，当然也可以通过style.badgeDiameter调整
`- (void)doAnimationWithType:(HHBadgeAnimationType)type;`动画支持
可定制的属性模型HHDotBadgeStyle定义如下
```
@interface HHDotBadgeStyle : NSObject
/// 直径 defalut 15
@property (nonatomic, assign)CGFloat badgeDiameter;
/// 圆圈的背景颜色
@property (nonatomic, strong)UIColor *bgColor;
/// 边框颜色
@property (nonatomic, strong)UIColor *borderColor;
/// 边框宽度
@property (nonatomic, assign)CGFloat borderWidth;
/// 是否支持QQ的粘性拖拽效果 default NO 此功能暂时未实现 后继版本加入
//@property (nonatomic, assign, getter=haveGooEffect)BOOL gooEffect;
@end
```
这里类似QQ那种的粘性拖拽效果暂时还未能支持。
支持的动画类型
```
/// 动画类型
typedef NS_ENUM(NSUInteger, HHBadgeAnimationType) {
    HHBadgeAnimationPop = 0,///< 缩放动画
    HHBadgeAnimationBlink,///< 眨眼动画
    HHBadgeAnimationBump,///< 上下跳动动画
    HHBadgeAnimationNone///< 无
};
```
这些通过注释应该很容易就能看明白了，就不过多解释了
再看下数字通知图标样式
```
@interface HHCountBadge : HHDotBadge
#pragma mark setup
/// 快速创建实例
+ (instancetype)badgeWithCount:(NSInteger)count;
- (instancetype)initWithCount:(NSInteger)count NS_DESIGNATED_INITIALIZER;
#pragma mark adjustment
/// 配置外观样式
- (void)applyStyle:(void (^)(HHCountBadgeStyle *style))maker;
#pragma mark changeCount
- (void)increaseWithAnimationType:(HHBadgeAnimationType)type;
- (void)increaseBy:(NSInteger)count animationWithType:(HHBadgeAnimationType)type;
- (void)decreaseWithAnimationType:(HHBadgeAnimationType)type;
- (void)decreaseBy:(NSInteger)count animationWithType:(HHBadgeAnimationType)type;
/// 数量
@property (nonatomic, assign)NSInteger count;
@end
```
`- (void)increaseWithAnimationType:(HHBadgeAnimationType)type;`默认会增加1，当然你也可以通过方法
```
-
 (void)increaseBy:(NSInteger)count animationWithType:(HHBadgeAnimationType)type;
```
自己设置增加数量，同时可设置动画类型，减少数量同理
可定制的属性模型HHCountBadgeStyle
```
@interface HHCountBadgeStyle : HHDotBadgeStyle
/// 是否自适应大小 default YES
@property (nonatomic, assign)BOOL automaticExpansion;
/// 是否强制显示0 default NO
@property (nonatomic, assign, getter=shouldForcedShowZero)BOOL forceShowZero;
/// 数字的颜色
@property (nonatomic, strong)UIColor *countColor;
/// 数字的字体
@property (nonatomic, strong)UIFont *countFont;
/// 数字的对齐方式
@property (nonatomic, assign)NSTextAlignment countAlignment;
@end
```
最后是UIView和UIBarButtonItem分类
```
@interface UIView (HHBadgeHUD)
@property (nonatomic, strong) __kindof HHBadge *hh_badge;
@end
@interface UIBarButtonItem (HHBadgeHUD)
/// 设置此值的时候确保Item的View属性已被加载
@property (nonatomic, strong) __kindof HHBadge *hh_badge;
@end
```
看到这里相信读者应该知道怎么使用该控件了吧！如果不明白的可以看本文底部的连接demo，就不贴代码了
这里说下`/// 设置此值的时候确保Item的View属性已被加载`这句注释
![](http://upload-images.jianshu.io/upload_images/1003160-a697d364427b3d53.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
这是demo中的例子我们来看下add属性在viewDidLoad中是什么样的
![](http://upload-images.jianshu.io/upload_images/1003160-7480be1c012d55d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
可以看到view属性为空，所以如果把`self.add.hh_badge = [HHCountBadge badgeWithCount:9];`放在viewDidLoad中是不会有效果的
在viewDidAppear:方法里add属性是这样的
![](http://upload-images.jianshu.io/upload_images/1003160-670b5db7c8e1e43f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
可以看到此时view属性是有值的，且是UIButton的子类UINavigationButton，读者也可以通过这样的方式去学习系统到底是怎么实现一些特定类的，以及封装思路，笔者也想过通过kvo监听item的view属性，当初始化完成的时候添加我们的自定义通知图标，这样使用者就可以在任意地方赋值了，但是考虑到实现起来还是有点复杂的，并且影响该组件的轻量级特性，就放弃了，读者有兴趣可以自行尝试，还有一点要说明下，当控制器为navigationController的根控制器时在viewDidLoad中item的view是有值的。
这个自定义通知图标组件就到这里，希望能在开发中帮助到你，有什么问题或建议欢迎留言。
gitHub传送门[https://github.com/theSkyOfJune/HHBadgeHUD](https://github.com/theSkyOfJune/HHBadgeHUD)
