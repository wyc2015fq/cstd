# UIDynamicBehavior - weixin_33985507的博客 - CSDN博客
2016年07月13日 16:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
```
//
//  UIDynamicBehavior.h
//  UIKit
//
//  Copyright (c) 2012-2015 Apple Inc. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIGeometry.h>
NS_ASSUME_NONNULL_BEGIN
@class UIDynamicAnimator;     //  
@class UIBezierPath;
// 碰撞界限的枚举类型
typedef NS_ENUM(NSUInteger, UIDynamicItemCollisionBoundsType) {
    UIDynamicItemCollisionBoundsTypeRectangle,   // 矩形
    UIDynamicItemCollisionBoundsTypeEllipse,     // 椭圆 radii will be determined from the items bounds width, height
    UIDynamicItemCollisionBoundsTypePath         // 路径
} NS_ENUM_AVAILABLE_IOS(9_0);
//  UIDynamicItem 协议， 所有的 UI 控件都遵守了这个协议
@protocol UIDynamicItem <NSObject>
@property (nonatomic, readwrite) CGPoint center;
@property (nonatomic, readonly) CGRect bounds;
@property (nonatomic, readwrite) CGAffineTransform transform;
// 可选的
@optional
/**
 The collision type represents how the dynamics system will evaluate collisions with 
 respect to the dynamic item. defaults to UIDynamicItemCollisionBoundsTypeRectangle
 */
@property (nonatomic, readonly) UIDynamicItemCollisionBoundsType collisionBoundsType NS_AVAILABLE_IOS(9_0);
/**
 The path must represent a convex polygon with counter clockwise winding and no self intersection. 
 The point (0,0) in the path corresponds to the dynamic item's center.
 */
@property (nonatomic, readonly) UIBezierPath *collisionBoundingPath NS_AVAILABLE_IOS(9_0);
@end
// item group
NS_CLASS_AVAILABLE_IOS(9_0) @interface UIDynamicItemGroup : NSObject <UIDynamicItem>
// 通过 item 数组进行实例化
- (instancetype)initWithItems:(NSArray<id <UIDynamicItem>> *)items;
// 获取 item 数组
@property (nonatomic, readonly, copy) NSArray<id <UIDynamicItem>> *items;
@end
// 行为类
NS_CLASS_AVAILABLE_IOS(7_0) @interface UIDynamicBehavior : NSObject
// ****** 下面这些方法只能在 自定义的  UIDynamicBehavior 中使用  UI 中定义的 Behavior 是不能使用的 ******
// 添加子行为 到这个行为对象（ 这种方式主要是进行行为自定义）
/*
  behavior 对象如果已经添加到了夫 behavior 中，或者 behavior 为 nil 。
  那么这个方法的调用就被忽略
  这个方法只能在 自定义的 UIDynamicBehavior 子类中调用
*/
- (void)addChildBehavior:(UIDynamicBehavior *)behavior;
// 移除子行为 （只能在自定义的类中使用）
- (void)removeChildBehavior:(UIDynamicBehavior *)behavior;
// 获取所有的子行为
// 这个方法只能在 自定义的 UIDynamicBehavior 子类中调用
@property (nonatomic, readonly, copy) NSArray<__kindof UIDynamicBehavior *> *childBehaviors;
// When running, the dynamic animator calls the action block on every animation step.
// 运行时,动态动画器调用动作块在每个动画的步骤。
@property (nullable, nonatomic,copy) void (^action)(void);
// behavior 被添加或者被移除的时候会调用这个方法
// 这个方法是用来重载的 用来响应 behavior  tree 在 animator 中的改变
- (void)willMoveToAnimator:(nullable UIDynamicAnimator *)dynamicAnimator; // nil when being removed from an animator
// 获取动画管理者
@property (nullable, nonatomic, readonly) UIDynamicAnimator *dynamicAnimator;
@end
NS_ASSUME_NONNULL_END
```
