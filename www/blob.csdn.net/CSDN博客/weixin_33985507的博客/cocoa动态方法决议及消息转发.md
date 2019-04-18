# cocoa动态方法决议及消息转发 - weixin_33985507的博客 - CSDN博客
2014年11月12日 11:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

假设给一个对象发送不能响应的消息，同一时候又没有进行动态方法决议，又没实现消息转发，那么就会引发以下的crash信息
`2014-07-30 15:47:54.434 MethodNotFind[1719:403] -[Person setName:]: unrecognized selector sent to instance 0x100121db0`下面是測试的demo
先看看Person类的定义。
```
#import <Foundation/Foundation.h>
@class Car;
@interface Person : NSObject
{
    Car* car; //用来处理不能转发的消息
}
@property (copy)NSString* name;
- (void)forwardInvocation:(NSInvocation *)anInvocation;
-(void)print;
@end
```
Person.m的实现部分
```
#import "Person.h"
#import "Car.h"
@implementation Person
@dynamic name; //两个作用:告诉编译器不要创建该属性的get和setter方法.告诉编译器不要创建实现属性所用的实例变量
//@synthesize name=myName; //告诉编译器在Person这个类里面帮我们创建get和set方法,同一时候为我们得Person类创建一个成员变量叫myName
-(id)init
{
    if (self=[super init]) {
        car=[[Car alloc] init];
    }
    return self;
}
-(void)dealloc
{
    if (car)
    {
        [car release];
        car=nil;
    }
    [super dealloc];
}
void dynamicMethodIMP(id self, SEL _cmd)
{
    // implementation ....
    NSLog(@"动态决议方法被调用");
    
    
}
+ (BOOL)resolveInstanceMethod:(SEL)sel
{
    NSLog(@"sel is %@", NSStringFromSelector(sel)); //得到方法名
    if(sel == @selector(setName:)){
       class_addMethod([self class],sel,(IMP)dynamicMethodIMP,"v@:");
        return YES; 
    }
    
    return [super resolveInstanceMethod:sel];
}
// 这种方法触发的前提就是+ (BOOL)resolveInstanceMethod:(SEL)返回NO，否则将不会进行触发。 子类重写这种方法，能够把消息转给一个指定得对象。还有必需要重写- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector这种方法.两个条件缺一不可
//注意：To respond to methods that your object does not itself recognize, you must override methodSignatureForSelector: in addition to forwardInvocation:. The mechanism for forwarding messages uses information obtained from methodSignatureForSelector: to create the NSInvocation object to be forwarded. Your overriding method must provide an appropriate method signature for the given selector, either by pre formulating one or by asking another object for one.
- (void)forwardInvocation:(NSInvocation *)anInvocation
{
   SEL name  =[anInvocation selector];
    
     NSLog(@" >> forwardInvocation for selector %@", NSStringFromSelector(name));
    if ([car respondsToSelector:name])
    {
        [anInvocation invokeWithTarget:car];
    }else
    {
        [self doesNotRecognizeSelector:name];
    }
   
    
 
}
//消息转发的时候这种方法会获取一些信息，去创建NSInvocation对象，给- (void)forwardInvocation:(NSInvocation *)anInvocation使用
- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector {
    return [Car instanceMethodSignatureForSelector:aSelector];
}
-(void)print
{
   
  //  NSLog(@"%@",myName);//成员变量名字，能够由synthesize指定
}
@end
```
Person类不能响应-(void)setName:(NSString*) name方法时，写了一个Car类进行消息转发,
Car.h
```
#import <Foundation/Foundation.h>
@interface Car : NSObject
{
    NSString* myName;
}
-(void)setName:(NSString*) name;
@end
```
Car.m实现
```
#import "Car.h"
@implementation Car
-(void)setName:(NSString *)name
{
    
    NSLog(@"setName 在Car里面被调用");
    if (![name isEqual:myName])
    {
        [myName release];
        myName=name;
    }
}
@end
```
当我们实现了动态方法决议，又实现了消息转发，那么首先会调用动态方法决议，也不须要进行消息转发了。假设在 resolveInstanceMethod 的实现中没有真正为 selector 提供实现，并返回 NO。那么就会进行消息转发。
下面是实现了动态决议方法成功以及实现了消息转发的执行结果：
```
2014-07-30 16:27:55.072 MethodNotFind[1774:403] sel is setName:
2014-07-30 16:27:55.073 MethodNotFind[1774:403] 动态决议方法被调用
```
非常显然动态决议方法被调用了，不会引发crash。
把void dynamicMethodIMP(id self, SEL _cmd)和+ (BOOL)resolveInstanceMethod:(SEL)sel凝视之后。看看执行结果：
```
2014-07-30 16:28:33.343 MethodNotFind[1786:403]  >> forwardInvocation for selector setName:
2014-07-30 16:28:33.343 MethodNotFind[1786:403] setName 在Car里面被调用
```
非常显然实现了消息转发，也没引发crash。
消息发送的过程：
首先在对象的类对象的 cache，method list 以及父类对象的 cache, method list 中依次查找 SEL 相应的 IMP。假设没有找到且实现了动态方法决议机制就会进行决议，假设没有实现动态方法决议机制或决议失败且实现了消息转发机制就会进入消息转发流程。
