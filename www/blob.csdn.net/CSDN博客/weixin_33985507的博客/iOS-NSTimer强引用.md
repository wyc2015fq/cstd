# iOS-NSTimer强引用 - weixin_33985507的博客 - CSDN博客
2017年12月15日 13:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
```
@property (nonatomic , strong) NSTimer *animationTimer;
self.animationTimer = [NSTimer timerWithTimeInterval:1 target:self selector:@selector(animationTimerDidFired:) userInfo:nil repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:self.animationTimer forMode:NSRunLoopCommonModes];
```
当使用如上代码创建一个NSTimer之后会出现一个VC-> NSTimer->VC的循环引用。导致界面不能释放。
解决方法就是让其中某一部分变成弱引用。
YYKit中提供了YYWeakProxy这个类来解决NSTimer或CADisplayLink的强引用问题
YYWeakProxy 继承自[NSProxy](https://www.jianshu.com/p/72bcb94f80ed)
![694844-e8198395477b3826.png](https://upload-images.jianshu.io/upload_images/694844-e8198395477b3826.png)
消息转发机制
声明了一个
```
@property (nullable, nonatomic, weak, readonly) id target;
```
手动将传入的参数变成弱引用。
```
#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
@interface YYWeakProxy : NSProxy
@property (nullable, nonatomic, weak, readonly) id target;
- (instancetype)initWithTarget:(id)target;
+ (instancetype)proxyWithTarget:(id)target;
@end
NS_ASSUME_NONNULL_END
```
```
#import "YYWeakProxy.h"
@implementation YYWeakProxy
- (instancetype)initWithTarget:(id)target {
    _target = target;
    return self;
}
+ (instancetype)proxyWithTarget:(id)target {
    return [[YYWeakProxy alloc] initWithTarget:target];
}
// 上图可见当找不到对应SEL的时候执行此方法，手动返回self，开始执行方法
- (id)forwardingTargetForSelector:(SEL)selector {
    return _target;
}
// 对所有的不能处理的都传递给_target了,理论上是不需要写下面两个方法的，但是因为target 是weak引用的，所以还是写了下面两个方法，防止crash。
- (void)forwardInvocation:(NSInvocation *)invocation {
    void *null = NULL;
    [invocation setReturnValue:&null];
}
- (NSMethodSignature *)methodSignatureForSelector:(SEL)selector {
    return [NSObject instanceMethodSignatureForSelector:@selector(init)];
}
- (BOOL)respondsToSelector:(SEL)aSelector {
    return [_target respondsToSelector:aSelector];
}
- (BOOL)isEqual:(id)object {
    return [_target isEqual:object];
}
- (NSUInteger)hash {
    return [_target hash];
}
- (Class)superclass {
    return [_target superclass];
}
- (Class)class {
    return [_target class];
}
- (BOOL)isKindOfClass:(Class)aClass {
    return [_target isKindOfClass:aClass];
}
- (BOOL)isMemberOfClass:(Class)aClass {
    return [_target isMemberOfClass:aClass];
}
- (BOOL)conformsToProtocol:(Protocol *)aProtocol {
    return [_target conformsToProtocol:aProtocol];
}
- (BOOL)isProxy {
    return YES;
}
- (NSString *)description {
    return [_target description];
}
- (NSString *)debugDescription {
    return [_target debugDescription];
}
@end
```
