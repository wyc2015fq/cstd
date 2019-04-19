# UIButton(干货)--教你如何利用runtime自定义UIButton的titleLabel以及image的frame - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月27日 09:06:27[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：773
本文来自简书，原文地址:[http://www.jianshu.com/p/e5fdc00b8e31](http://www.jianshu.com/p/e5fdc00b8e31)
最近做项目,会用到许多的类似美团首页button的效果,之前做法都是自定义view,view上添加button以及label来做出相同的效果,但是今天我教你极为简便的方法,只需要自定义button就能实现同一个bt上titleLabel与image互换!
想要的效果:
![](http://upload-images.jianshu.io/upload_images/1869411-5b9f7874eb991170.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
例子
实际效果:哈哈,需要各位大佬们自己去美化
![](http://upload-images.jianshu.io/upload_images/1869411-fdf2a85134459210.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自定义bt层级关系
![](http://upload-images.jianshu.io/upload_images/1869411-93e43867ee33f607.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
正面效果
传送门:[https://github.com/JonesCxy/UIButton](https://github.com/JonesCxy/UIButton)
不废话,直接上代码:
category.h
```
#import <UIKit/UIKit.h>
@interface UIButton (Layout)
@property (nonatomic,assign) CGRect titleRect;
@property (nonatomic,assign) CGRect imageRect;
@end
```
category.m
```
#import "UIButton+Layout.h"
#import <objc/runtime.h>
@interface UIButton ()
@end
@implementation UIButton (Layout)
#pragma mark - ************* 通过运行时动态添加关联 ******************
//定义关联的Key
static const char * titleRectKey = "yl_titleRectKey";
- (CGRect)titleRect {
    return [objc_getAssociatedObject(self, titleRectKey) CGRectValue];
}
- (void)setTitleRect:(CGRect)rect {
    objc_setAssociatedObject(self, titleRectKey, [NSValue valueWithCGRect:rect], OBJC_ASSOCIATION_RETAIN);
}
//定义关联的Key
static const char * imageRectKey = "yl_imageRectKey";
- (CGRect)imageRect {
    NSValue * rectValue = objc_getAssociatedObject(self, imageRectKey);
    return [rectValue CGRectValue];
}
- (void)setImageRect:(CGRect)rect {
    objc_setAssociatedObject(self, imageRectKey, [NSValue valueWithCGRect:rect], OBJC_ASSOCIATION_RETAIN);
}
#pragma mark - ************* 通过运行时动态替换方法 ******************
+ (void)load {
    MethodSwizzle(self,@selector(titleRectForContentRect:),@selector(override_titleRectForContentRect:));
    MethodSwizzle(self,@selector(imageRectForContentRect:),@selector(override_imageRectForContentRect:));
}
void MethodSwizzle(Class c,SEL origSEL,SEL overrideSEL)
{
    Method origMethod = class_getInstanceMethod(c, origSEL);
    Method overrideMethod= class_getInstanceMethod(c, overrideSEL);
    //运行时函数class_addMethod 如果发现方法已经存在，会失败返回，也可以用来做检查用:
    if(class_addMethod(c, origSEL, method_getImplementation(overrideMethod),method_getTypeEncoding(overrideMethod)))
    {
        //如果添加成功(在父类中重写的方法)，再把目标类中的方法替换为旧有的实现:
        class_replaceMethod(c,overrideSEL, method_getImplementation(origMethod), method_getTypeEncoding(origMethod));
    }
    else
    {
        //addMethod会让目标类的方法指向新的实现，使用replaceMethod再将新的方法指向原先的实现，这样就完成了交换操作。
        method_exchangeImplementations(origMethod,overrideMethod);
    }
}
- (CGRect)override_titleRectForContentRect:(CGRect)contentRect {
    if (!CGRectIsEmpty(self.titleRect) && !CGRectEqualToRect(self.titleRect, CGRectZero)) {
        return self.titleRect;
    }
    return [self override_titleRectForContentRect:contentRect];
}
- (CGRect)override_imageRectForContentRect:(CGRect)contentRect {
    if (!CGRectIsEmpty(self.imageRect) && !CGRectEqualToRect(self.imageRect, CGRectZero)) {
        return self.imageRect;
    }
    return [self override_imageRectForContentRect:contentRect];
}
- (void)setTitleRect:(CGRect )titleRect ImageRect:(CGRect )imageRect {
    self.titleRect = titleRect;
    self.imageRect = imageRect;
}
@end
```
自定义Bt.h
```
#import <UIKit/UIKit.h>
@interface CxyButton : UIButton
@property(assign,nonatomic)CGRect titleRect;
@property(assign,nonatomic)CGRect imageRect;
@end
```
自定义Bt.m
```
#import "CxyButton.h"
@implementation CxyButton
- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}
-(CGRect)titleRectForContentRect:(CGRect)contentRect{
    if (!CGRectIsEmpty(self.titleRect) && !CGRectEqualToRect(self.titleRect, CGRectZero)) {
        return self.titleRect;
    }
    return [super titleRectForContentRect:contentRect];
}
-(CGRect)imageRectForContentRect:(CGRect)contentRect{
    if (!CGRectIsEmpty(self.imageRect)&&!CGRectEqualToRect(self.imageRect, CGRectZero)) {
        return self.imageRect;
    }
    return [super imageRectForContentRect:contentRect];
}
@end
```
ViewControll.m
```
- (void)viewDidLoad {
    [super viewDidLoad];
    CxyButton *bt = [CxyButton buttonWithType:(UIButtonTypeCustom)];
    bt.imageRect = CGRectMake(0, 0, 20, 20);
    bt.titleRect = CGRectMake(20, 0, 80, 20);
    bt.frame = CGRectMake(100, 100, 100, 20);
    [bt setImage:[UIImage imageNamed:@"heart-1"] forState:(UIControlStateNormal)];
    [bt setTitle:@"我就是我" forState:(UIControlStateNormal)];
    bt.backgroundColor = [UIColor redColor];
    [self.view addSubview:bt];
    CxyButton *bt2 = [CxyButton buttonWithType:(UIButtonTypeCustom)];
    bt2.imageRect = CGRectMake(0, 0, 30, 30);
    bt2.titleRect = CGRectMake(0, 20, 30, 80);
    bt2.frame = CGRectMake(100, 150, 30, 100);
    [bt2 setImage:[UIImage imageNamed:@"heart-2"] forState:(UIControlStateNormal)];
    [bt2 setTitle:@"我" forState:(UIControlStateNormal)];
    bt2.backgroundColor = [UIColor greenColor];
    [self.view addSubview:bt2];
}
```
简介:利用运行时将bt的titleLabel与image位置动态改变,在给bt赋值时,只需要重写title与image的rect即可.
