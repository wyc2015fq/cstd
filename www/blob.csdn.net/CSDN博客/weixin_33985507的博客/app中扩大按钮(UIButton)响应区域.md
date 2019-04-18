# app中扩大按钮(UIButton)响应区域 - weixin_33985507的博客 - CSDN博客
2016年04月28日 22:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
###### 在你根据原型标注图搭建好界面之后经常会遇到产品经理提出要求:扩大按钮的点击响应范围和灵敏度,你肯定会有种想上去和他干一架的冲动,但你最后还是抑制住了自己默默地坐了下来,思考着解答方案,然后网上搜了一圈
##### 根据设置按钮图片方法可分为两种方式
###### 1.setImage: 设置图片内容显示模式为居中,然后再设置图片的边距和按钮的大小
```
[[button imageView] setContentMode: UIViewContentModeCenter];
[[button setImage:[UIImage imageNamed:@"xxx.png"] forState:UIControlStateNormal];
//根据需求设置图片与按钮周边的边距及按钮的frame
[button setImageEdgeInsets:UIEdgeInsetsMake(10, 10, 10, 10)];
button.frame = CGRectMake(30, 30, 30, 30);
```
###### 2. setBackgroundImage:设置按钮背景图片上述方法就无效了,此时可以通过OC黑魔法来解决. 首先添加一个UIButton的category来重写hitTest,然后通过为分类添加的一个属性来扩大按钮的响应区域
***UIButton+EnlargeHitArea.h***
```
@interface UIButton (Extensions)
@property(nonatomic, assign) UIEdgeInsets hitTestEdgeInsets;
@end
```
***UIButton+EnlargeHitArea.m***
```
#import "UIButton+EnlargeHitArea.h"
#import <objc/runtime.h>
@implementation UIButton (EnlargeHitArea)
@dynamic hitTestEdgeInsets;
static const NSString *KEY_HIT_TEST_EDGE_INSETS = @"HitTestEdgeInsets";
-(void)setHitTestEdgeInsets:(UIEdgeInsets)hitTestEdgeInsets {
    NSValue *value = [NSValue value:&hitTestEdgeInsets withObjCType:@encode(UIEdgeInsets)];
    objc_setAssociatedObject(self, &KEY_HIT_TEST_EDGE_INSETS, value, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}
-(UIEdgeInsets)hitTestEdgeInsets {
    NSValue *value = objc_getAssociatedObject(self, &KEY_HIT_TEST_EDGE_INSETS);
    if(value) {
        UIEdgeInsets edgeInsets; [value getValue:&edgeInsets]; return edgeInsets;
    }else {
        return UIEdgeInsetsZero;
    }
}
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event {
    if(UIEdgeInsetsEqualToEdgeInsets(self.hitTestEdgeInsets, UIEdgeInsetsZero) || !self.enabled || self.hidden) {
        return [super pointInside:point withEvent:event];
    }
    
    CGRect relativeFrame = self.bounds;
    CGRect hitFrame = UIEdgeInsetsInsetRect(relativeFrame, self.hitTestEdgeInsets);
    
    return CGRectContainsPoint(hitFrame, point);
}
@end
```
***通过设置按钮分类的hitTestEdgeInsets属性扩大按钮的响应范围***
```
#import "UIButton+EnlargeHitArea.h"
[button setHitTestEdgeInsets:UIEdgeInsetsMake(-10, -10, -10, -10)];
```
###### 两种按钮图片设置方式都可以使用按钮分类的方法来扩大按钮的响应区域
本文引用来自:[stackoverflow](https://link.jianshu.com?t=http%3A%2F%2Fstackoverflow.com%2Fquestions%2F808503%2Fuibutton-making-the-hit-area-larger-than-the-default-hit-area)
[Hit-Testing in iOS](https://link.jianshu.com?t=http%3A%2F%2Fsmnh.me%2Fhit-testing-in-ios%2F)
