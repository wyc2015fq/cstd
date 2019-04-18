# OC链式语法浅析 - weixin_33985507的博客 - CSDN博客
2018年11月12日 18:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
#### 一、什么是链式语法
不知道有没有官方的定义，个人理解的点语法主要包含点语法、小括号调用、连续调用几部分。其中比较有代表性的是[Masonry](https://github.com/SnapKit/Masonry)著名的AutoLayout布局框架，它里面的api就采用了链式语法的调用方式，常见的如下面的代码。
```
[self.muteIM mas_makeConstraints:^(MASConstraintMaker *make) {
        
        make.size.mas_equalTo(CGSizeMake(18, 18));
        make.bottom.right.equalTo(self.userHeadIM);
    }];
```
#### 二、如何在OC中使用链式语法
##### 1、点语法
在OC里面可以使用"."访问常见的有以下两种情况
```
①、类的属性
我们知道当我们给一个类添加属性后，系统会自动生成get、set方法我们就可以使用点语法来访问属性;
②、无参的方法
形如 - (void)func;我们也可以使用点语法来访问obj.func;
```
##### 2、小括号
c函数/block
```
- (void)test
{
    //1、c函数
    testFunc();
    
    //2、block
    void(^testBlock)(void) = ^{};
    testBlock();
}
void testFunc()
{
    
}
```
##### 3、连续访问
结合1、2怎么连续访问就也很简单了
```
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@interface MSetFrame : NSObject
@property (nonatomic, strong) NSMutableArray *handleArray;
@property (nonatomic, weak) UIView *view;
- (MSetFrame *)left;
- (MSetFrame *)right;
- (MSetFrame *)top;
- (MSetFrame *)bottom;
- (MSetFrame *)width;
- (MSetFrame *)height;
- (void(^)(NSArray *))m_equal;
- (MSetFrame *(^)(void))m_blockTest;
@end
```
如上述示例代码所示：
①、点语法连续访问
**MSetFrame**的实例可以调用**left、right**这些方法，这些方法返回的是**MSetFrame**实例又可以调用**left、right**，所以一个**MSetFrame**实例即可有**obj.left.right**的调用方式。
②、小括号
如示例代码中的**m_blockTest**方法因为它返回了一个**返回MSetFrame实例的block**，所以我们可以采用**obj.m_blockTest().left**的调用方式（**因为方法返回的是block所以调用方法后可以接()，又因为block的执行结果返回的是实例本身所以又可以继续调用示例拥有的方法**）。
#### 三、一个简单的实例
1、目标代码
```
- (void)viewDidLoad
{
    [super viewDidLoad];
    UIView *redView = [[UIView alloc] init];
    redView.backgroundColor = [UIColor redColor];
    redView.mset.left.top.width.height.m_equal(@[@10,@20,@200,@100]);
    [self.view addSubview:redView];
    // Do any additional setup after loading the view, typically from a nib.
}
```
使用链式语法实现一个简单的设置视图frame的功能
2、核心逻辑简析
要实现这样一个功能我们主要要解决三个问题
①、语法逻辑
```
@implementation UIView (frame)
- (MSetFrame *)mset
{
    MSetFrame *set = [[MSetFrame alloc] init];
    set.view = self;
    return set;
}
@end
```
新建一个UIView的分类，定义一个无参数的**mset**方法返回**MSetFrame**实例用来设置view的frame;
②、存储要设置的参数（left、right、width、height...）
将要设置的项以NSNumber的形式存储在数组中
```
- (MSetFrame *)left
{
    return [self m_addHandleWithType:MH_LEFT];
}
- (MSetFrame *)m_addHandleWithType:(MHandleType)type
{
    NSNumber *typeNum = [NSNumber numberWithInteger:type];
    [self.handleArray addObject:typeNum];
    return self;
}
```
③、传递参数
定义一个返回带参block的方法，使用block传递一个参数数组，然后根据前面存储的要设置的项来进行设置。
```
- (void (^)(NSArray *))m_equal
{
    //MSetFrame本身并没有持有这个block，所以在block里访问self没有循环引用。
    return ^(NSArray *pa){
        
        #ifdef DEBUG
        NSAssert(pa.count == self.handleArray.count, @"MSetFrame参数个数有问题");
        #endif
        
        if (pa.count == self.handleArray.count)
        {
            //[self sortHandle];
            for (int i = 0; i < pa.count; i++)
            {
                MHandleType type = [self.handleArray[i] integerValue];
                [self m_setValue:pa[i] type:type];
            }
        }
    };
}
```
**注：为什么不直接在UIView分类里添加left、right这些方法来实现，主要有两方面考虑①、UIView是一个常用的类直接在它的分类里实现，会给正常使用带来困扰(误调用)；②、因为要存储设置项，需要创建一个数组如果直接在UIView分类里实现，数组跟view绑定当重复设置一个view时需要一个清理逻辑容易出错；**
[详见MFrameDemo](https://github.com/tiantiankaixin/MBlogDemo)
