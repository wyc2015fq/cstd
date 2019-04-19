# iOS夜间模式，换肤，换字体样式 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月15日 14:03:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1202标签：[iOS 资源大全																[iOS学习资料																[iOS常用方法																[iOS夜间模式																[换肤](https://so.csdn.net/so/search/s.do?q=换肤&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS夜间模式&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
APP换肤...
很简单的功能...
但是做起来很复杂...
网上的方法很多...
我觉得很复杂...
其实是我自己没看懂...
hahhahahahahhahahha....
git上也有资料...
要引入的文件一堆...
感觉不够简洁....
所以 自己去设计了一个...
**原理：**
使用通知，需要变肤的控件注册这个监听，收到变化的通知之后改变做出相应的处理。
虽然笨了点，但是简单粗暴。
试过，将近百个的视图一起监听变化，性能还ok。
**实现:**
### 1.定义一个NSObject的类别Skin....
### 2.定义一个比如换字体颜色或者字体样式的类...
我是这样子的
`SkinFontStyleModule.h`
```
SkinFontStyleModule.h
@interface SkinFontStyleModule : NSObject
@property (weak, nonatomic) id targetObj;
@property (assign, nonatomic) NSInteger fontNormal;
- (instancetype)initWithTarget:(id)targetObj andNormalFontSize:(NSInteger)normalSize;
@end
```
`SkinFontStyleModule.m`
```
SkinFontStyleModule.m
@implementation SkinFontStyleModule
-(instancetype)initWithTarget:(id)targetObj andNormalFontSize:(NSInteger)normalSize
{
    self = [super init];
    if(self)
    {
        self.targetObj = targetObj;
        self.fontNormal = normalSize;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(skinFontStyleChange) name:SKIN_FONT_STYLE_NOTIFICATION object:nil];
        [self skinFontStyleChange];
    }
    return self;
}
- (void)skinFontStyleChange
{
    if([self.targetObj isKindOfClass:[UILabel class]])
    {
        UILabel * lab = (UILabel *)self.targetObj;
        lab.font = [UIFont bbxSystemFont:self.fontNormal];
    }
    else if ([self.targetObj isKindOfClass:[UIButton class]])
    {
        UIButton * btn = (UIButton *)self.targetObj;
        btn.titleLabel.font = [UIFont bbxSystemFont:self.fontNormal];    
    }
    else if ([self.targetObj isKindOfClass:[UINavigationBar class]])
    {
        UINavigationBar * barV = (UINavigationBar *)self.targetObj;
        [barV setTitleTextAttributes:@{NSFontAttributeName:[UIFont bbxSystemFont:self.fontNormal],NSForegroundColorAttributeName:[UIColor whiteColor]}];
    }
}
-(void)dealloc
{
    self.targetObj = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self name:SKIN_FONT_STYLE_NOTIFICATION object:nil];
}
@end
```
主要在skinFontStyleChange中，收到变化通知的时候，对targetobj不同类型做判断然后设置他们相应的属性变化...
自己根据需求扩展...
### 3.NSObject+Skin的实现
`NSObject+Skin.h`
```
#define SKIN_FONT_STYLE_NOTIFICATION      @"SKIN_FONT_STYLE_NOTIFICATION"
@class SkinFontStyleModule;
@interface NSObject (Skin)
@property (retain, nonatomic) SkinFontStyleModule * fontStyle;
- (void)postskinFontStyleNotifition;
@end
NSObject+Skin.m
@implementation NSObject (Skin)
-(SkinFontStyleModule *)fontStyle
{
    return objc_getAssociatedObject(self, @selector(fontStyle));
}
-(void)setFontStyle:(SkinFontStyleModule *)fontStyle
{
    objc_setAssociatedObject(self, @selector(fontStyle), fontStyle, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}
- (void)postskinFontStyleNotifition
{
    [[NSNotificationCenter defaultCenter] postNotificationName:SKIN_FONT_STYLE_NOTIFICATION object:nil];
}
@end
```
### 4.使用的时候,只需引入 NSObject+Skin.h
设置需要变化的控件视图：
```
self.cellLab.fontStyle = [[SkinFontStyleModule alloc] initWithTarget:self.cellLab andNormalFontSize:14];
要变化肤色的时候只要执行
[self postskinFontStyleNotifition];
```
肤色也同理...
个人觉得这样子设置比较简单...
如果你有更好的设计方式...
或者问题的...
可以留言大家一起讨论...
已在实际的软件中应用了...
欢迎大家下载使用...
[APP-锁事(加密记事本)](https://itunes.apple.com/cn/app/suo-shi-jia-mi-ji-shi-ben/id1078636442?mt=8)
文／T_aa（简书作者）
原文链接：http://www.jianshu.com/p/c3eda3ec733b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
