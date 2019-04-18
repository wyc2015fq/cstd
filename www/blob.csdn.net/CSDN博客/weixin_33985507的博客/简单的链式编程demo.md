# 简单的链式编程demo - weixin_33985507的博客 - CSDN博客
2018年04月11日 19:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
```
#import <Foundation/Foundation.h>
#import "CalculateManager.h"
@interface NSObject (Calculate)
/*
 方法设计:自己框架,最好添加一个方法前缀
 */
// 把所有的计算代码放在这里--  聚合代码
+ (int)www_makeCalculate:(void(^)(CalculateManager *manager))block;
@end
#import "NSObject+Calculate.h"
@implementation NSObject (Calculate)
+ (int)www_makeCalculate:(void (^)(CalculateManager *manager))block {
    NSLog(@"%s",__func__);
    CalculateManager *mgr = [[CalculateManager alloc] init];
    block(mgr);
    return mgr.result;
}
@end
```
```
#import "CalculateManager.h"
@implementation CalculateManager
- (CalculateManager *(^)(int value))add {
    NSLog(@"%s",__func__);
    __weak typeof(self) weakSelf = self;
    return ^(int value2){
        NSLog(@"value2---%s",__func__);
        weakSelf.result += value2;
        return weakSelf;
    };
}
@end
```
```
#import "ViewController.h"
#import "NSObject+Calculate.h"
@interface ViewController ()
@end
@implementation ViewController
/**
 *  +[NSObject(Calculate) www_makeCalculate:]
 *  [ViewController viewDidLoad]_block_invoke
 *  -[CalculateManager add]
 *  [CalculateManager add]_block_invoke
 *  执行顺序--
    1.www_makeCalculate方法执行后去调用 ViewController中写入该方法中的block
    2.执行www_makeCalculate方法的block
    3.调用add方法（实际就是调用add方法中return的block，该block的传参在ViewController中传值）
    4.执行add方法中的block
 */
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    int result = [NSObject www_makeCalculate:^(CalculateManager *manager) {
        NSLog(@"%s",__func__);
        manager.add(5);
    }];
    NSLog(@"%i",result);
}
```
