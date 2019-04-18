# 【iOS基础】block剖析 - weixin_33985507的博客 - CSDN博客
2016年07月19日 18:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
###### 导语
> 
block是一些简短代码片段的封装.本质是函数指针. 虽然block声明的时候不带星,而且默认情况下存放在栈里,但是,他确实是OC对象,往下看会有示例说明
###### block类型
- _NSConcreteGlobalBlock 全局的静态 block，不会访问任何外部变量
- _NSConcreteStackBlock 保存在栈中的 block，当函数返回时会被销毁。但在ARC下,会被复制到堆上.
- _NSConcreteMallocBlock 保存在堆中的 block，当引用计数为 0 时会被销毁。 所以在ARC下,只有_NSConcreteGlobalBlock和_NSConcreteMallocBlock这两种类型
**写个例子来测试一下block类型**
```
/** 在.h中定义和.m中定义对结果没有影响,已验证完毕*/
//.h中定义类型,声明属性htestBlock 
typedef NSString *(^testBlock)(NSString *); 
@property (nonatomic ,copy)testBlock htestBlock; 
//.m中声明属性mtestBlock及变量_ctestBlock 
@property (nonatomic ,copy)testBlock mtestBlock; @implementation ViewController 
{ 
    testBlock _ctestBlock; 
} 
//下面进行测试 
self.htestBlock = ^(NSString *str) 
{ 
    return str; 
};
self.mtestBlock = ^(NSString *str) 
{ 
    return str;
 };
_ctestBlock = ^(NSString *str) 
{ 
    return str; 
}; 
NSLog(@"%@",self.htestBlock(@"h")); 
NSLog(@"%@",self.mtestBlock(@"m"));
NSLog(@"%@",_ctestBlock(@"c"));
```
![1602974-53f41edfdaddca3b.png](https://upload-images.jianshu.io/upload_images/1602974-53f41edfdaddca3b.png)
VC被销毁.png
]([http://upload-images.jianshu.io/upload_images/1602974-b4f8e1d1d0f64b63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240](https://link.jianshu.com?t=http://upload-images.jianshu.io/upload_images/1602974-b4f8e1d1d0f64b63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240))
结果如上图,你会发现,全部都是GlobalBlock额,但传闻GlobalBlock不会访问任何外部变量,所以接着测试
```
NSString *name = @"daqianqian";
    self.htestBlock = ^(NSString *str)
    {
        return name;
    };
    self.mtestBlock = ^(NSString *str)
    {
        return name;
    };
    _ctestBlock = ^(NSString *str)
    {
        return name;
    };
    NSLog(@"%@",self.htestBlock(@"h"));
    NSLog(@"%@",self.mtestBlock(@"m"));
    NSLog(@"%@",_ctestBlock(@"c"));
```
![1602974-5ad3b140bc908a97.png](https://upload-images.jianshu.io/upload_images/1602974-5ad3b140bc908a97.png)
1EB11A80-21E7-4544-AF8D-2DD7270FCDD0.png
这下全部变成MallocBlock了
> 
根据上面的结果,个人猜测,block声明后,就是全局的静态block,因为现在使用的是ARC,所以当你访问外部变量,会自动copy一份到堆上.
若理解不对请大家指正,感谢!
###### __block
再做个小测试
```
//声明block及属性myBlock
typedef NSString * (^Block) (int);
@property(nonatomic, copy)Block myBlock;
```
![1602974-74ecef17c719f68f.png](https://upload-images.jianshu.io/upload_images/1602974-74ecef17c719f68f.png)
40294132-AAB3-41DD-B571-E0F2001E1DDA.png
你想在在内部更改外部变量,你会发现,无法更改!
因为block如果要访问外部变量,他会拷贝进来一份外部变量,并且这个外部变量是只读的
外部变量改变也并不影响block内部拷贝的那一份变量
请接着看下面的例子
![1602974-8699d46fd4edbdbf.png](https://upload-images.jianshu.io/upload_images/1602974-8699d46fd4edbdbf.png)
FC2CF82E-CCE9-4EDC-B3AE-E2B30147D291.png
这个输出结果如下,验证了上面的判断
![1602974-780b64e427238587.png](https://upload-images.jianshu.io/upload_images/1602974-780b64e427238587.png)
199AFC83-591B-4366-8C4F-29B077F3AC3A.png
如果不想让block拷贝变量，而是想让内部使用的变量和外部使用的变量指向同一地址的话,需要在变量前面加上__block关键字,则外部变量不再是只读的，在block内部也可以改变它的值
![1602974-be4074f162c4a63a.png](https://upload-images.jianshu.io/upload_images/1602974-be4074f162c4a63a.png)
![6D53A005-84F6-4A1E-B7B1-E2965AA15305.png](http://upload-images.jianshu.io/upload_images/1602974-dea636bc30dc88c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
###### 定义和使用block
- 独立block,作为类的变量或属性使用
1- 实现界面反向传值
2- 可以实现界面A做完某一操作后,界面B立即实现其他操作
- 内联block,作为方法的参数使用
1-配合disptch_queue,可以方便的实现简单的多线程编程和异步编程
**Demo1 - 使用独立block,实现界面反向传值**
独立block格式:
typedef 返回值类型 (^block名称)(参数列表)
```
/** 在界面2声明block,*/
//自定义格式
typedef void(^CCBlock)(NSString *);
//声明属性
@property (nonatomic, copy)CCBlock myCCBlock;
@implementation TwoViewController
{
    //声明变量
    CCBlock myCCBlock2;
}
//在你需要的地方,写好调用方法
 if(self.myCCBlock)
{
    self.myCCBlock(@"你好");
}
/** 在界面1声明界面2的对象,并实现回调*/
TwoViewController *secondVC = [[TwoViewController alloc] init];
secondController.myCCBlock = ^(NSString *str)
{
     //在此处你可取到传来的"你好",可以做刷新界面等其他操作
};
```
就这样,轻松实现了界面的反向传值
**接上面的Demo1,再做个小测验**
在界面2声明一个数组,我们在界面2初始化的时候就调用block,并将值作为对象存在数组里,打印结果如下:
```
self.myCCBlock(@"lalala");
NSArray *array = [[NSArray alloc] initWithObjects:@"1",self.myCCBlock,@"2",nil];
NSLog(@"myCCBlock内容为%@,数组内容为为%@",self.myCCBlock,array);
```
![1602974-c0cb19c3a08fdaed.png](https://upload-images.jianshu.io/upload_images/1602974-c0cb19c3a08fdaed.png)
AA868EF4-41F9-4B11-A604-74729015616E.png
这说明,block就是OC对象,因为它能存在数组里!
**Demo2 - 使用内联block,实现**
内联block格式:
(返回值 (^)(参数列表))此方法的参数名
```
/** 在界面2声明内联block并调用*/
- (void)insideBlock: (NSString *)name successBlock:(void (^)(NSString *))success faileBlock:(void (^)(NSString *))fails;
- (void)insideBlock: (NSString *)name successBlock:(void (^)(NSString *))success faileBlock:(void (^)(NSString *))fails
{
   //可以在这里做判断,比如有一个BOOL值控制,为True就调用success(name),为False就调用faile(name)
    success(name);
    faile(name);
}
/** 在界面1声明界面2对象,并实现回调*/
    TwoViewController *secondController = [[TwoViewController alloc] init];
    [secondController insideBlock:@"name" successBlock:^(NSString *response)
    {
        NSLog(@"内联函数成功的回调是%@",response);
    }
    faileBlock:^(NSString *response)
    {
        NSLog(@"内联函数失败的回调是%@",response);  
    }];
```
就这样,我们实现了将一段程序块写在函数里面
###### 循环引用
还是做个测验来分享
大家应该知道,navigationController在pop掉界面的时候,这个界面会被销毁,在界面上加上这句话,会发现pop时这句话会打印出来
```
- (void)dealloc
{
    NSLog(@"被销毁了");
}
```
若出现循环引用,会出现什么情况呢,就是A引用B,B引用A,引用计数都为2,调用dealloc的时候释放一次,但结果是AB的引用计数仍为1,无法销毁,占用内存.
```
/** 在界面2.h声明block类型及属性,另外声明一个属性name*/
typedef void (^Block) (NSString *);
@property(nonatomic, copy)Block myBlock;
@property(nonatomic, strong)NSString *name;
/** 在界面2.m给属性name赋值,并在点击按钮返回界面1时,调用block,注意此时没传值*/
- (void)viewDidLoad
{
    [super viewDidLoad];
     self.name = @"lele";
}
- (IBAction)onClick:(id)sender
{
    if (self.myBlock)
    {
        self.myBlock(@"");
    }
    [self .navigationController popViewControllerAnimated:NO];
}
```
```
/** 在界面1声明界面2对象,并在界面1实现"在界面2点击按钮调用block方法"后的回调*/
- (IBAction)onClick:(id)sender
{
    TwoViewController *secondVC = [[TwoViewController alloc] init];
    secondVC.myBlock = ^(NSString * str)
    {
        NSLog(@"%@",secondVC.name);
    };
    [self.navigationController pushViewController:secondVC animated:NO];
}
```
此时运行程序你会发现,点击界面2的按钮后,界面2pop掉,返回界面1,"le'le"被传了过来,但却没有执行delloc,没有打印"被销毁了"
![1602974-7258d4c7e2eba7fd.png](https://upload-images.jianshu.io/upload_images/1602974-7258d4c7e2eba7fd.png)
2F7AF849-28B0-4254-81E2-96E8DA4762CE.png
> 
**原因**
1- 在界面1中,执行此句,VC的引用计数为1
```
TwoViewController *secondVC = [[TwoViewController alloc] init];
```
2- 在界面1中,执行此句,block的引用计数为1
```
secondVC.myBlock = ^(NSString * str)
    {
    };
```
3- 在界面1中,执行此句,VC的引用计数为2
```
NSLog(@"%@",secondVC.name);
```
4- 在界面2中,执行此句,block的引用计数为2
```
self.myBlock(@"");
```
5- 界面pop时,引用计数只会减1,所以造成循环引用
**解决办法**
手动将VC改为弱引用,使用"__weak typeof(原来VC类名) 自己再起个名字"方法.
```
- (IBAction)onClick:(id)sender
{
    TwoViewController *secondVC = [[TwoViewController alloc] init];
    __weak typeof(secondVC) vc;
    secondVC.myBlock = ^(NSString * str)
    {
        NSLog(@"%@",vc.name);
    };
    [self.navigationController pushViewController:secondVC animated:NO];
}
```
打印结果如图,因为被销毁了,所以取不到他的属性值name
![1602974-33c411914676d3f8.png](https://upload-images.jianshu.io/upload_images/1602974-33c411914676d3f8.png)
> 
补充
- 
###### 1-内联block普通
(返回值 (^)(参数列表))此方法的参数名
```
/** 在界面2声明内联block并调用*/
- (void)insideBlock: (NSString *)name successBlock:(void (^)(NSString *))success faileBlock:(void (^)(NSString *))fails;
- (void)insideBlock: (NSString *)name successBlock:(void (^)(NSString *))success faileBlock:(void (^)(NSString *))fails
{
   //可以在这里做判断,比如有一个BOOL值控制,为True就调用success(name),为False就调用faile(name)
    success(name);
    faile(name);
}
/** 在界面1声明界面2对象,并实现回调*/
    TwoViewController *secondController = [[TwoViewController alloc] init];
    [secondController insideBlock:@"name" successBlock:^(NSString *response)
    {
        NSLog(@"内联函数成功的回调是%@",response);
    }
    faileBlock:^(NSString *response)
    {
        NSLog(@"内联函数失败的回调是%@",response);  
    }];
```
1.系统会先调用
TwoViewController *secondController = [[TwoViewController alloc] init];
2.然后调用
[secondController insideBlock:@"name"
successBlock:^(NSString *response)
3.走到这里去调用这个方法内部
```
- (void)insideBlock: (NSString *)name
       successBlock:(void (^)(NSString *))success
         faileBlock:(void (^)(NSString *))fails
{
    success(name);
    fails(name);
}
```
如果这里有success(name);则去回调下图中的第一个方框
如果这里有fails(name);则去回调下图中的第二个方框
![1602974-2f90f4e815a69914.png](https://upload-images.jianshu.io/upload_images/1602974-2f90f4e815a69914.png)
- 
###### 2-内联block嵌套
```
本例子使用了FMDB,需导入<FMDB.h>
// 1.block类_初始化
#import "TwoViewController.h"
#import "objc/runtime.h"
#import <FMDB.h>
static TwoViewController *_vcSingleton = nil;
@interface TwoViewController ()
{
    FMDatabase      *_db;
}
@end
@implementation TwoViewController
#pragma mark - 单例
+(instancetype)sharedDBSingleton
{
    if (_vcSingleton == nil)
    {
        _vcSingleton = [[TwoViewController alloc] init];
        [_vcSingleton initVCSingleton];
    }
    return _vcSingleton;
}
- (void)initVCSingleton
{
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString *filePath = [documentsPath stringByAppendingPathComponent:@"model.sqlite"];
    NSLog(@"地址是%@",filePath);
    _db = [FMDatabase databaseWithPath:filePath];
    NSLog(@"这个db是什么%@",_db);
    [_db open];
    // 初始化数据表
    NSString *personSql = @"CREATE TABLE 'animal' ('animal_name' VARCHAR(255))";
    [_db executeUpdate:personSql];
    [_db close];
}
// 2.block类_block
#pragma mark - Block
- (void)insideBlock1: (NSString *)name successBlock:(BOOL (^)(FMDatabase *db,SInt32 lastVersion))success
{
    SInt32 lastVersion = 5555;
    
    [self inTransaction:^(FMDatabase *db, BOOL *shouldRollback)
     {
         if (success(db, lastVersion))
         {
             NSLog(@"这是什么");
         }
         else
         {
              NSLog(@"这又是什么");
         }
     }];
}
- (void)inTransaction:(void (^)(FMDatabase *db, BOOL *shouldRollback))block
{
    BOOL shouldRollback = NO;
    block(_db, &shouldRollback);
}
//3.其他类_调用block
    TwoViewController *secondController = [TwoViewController sharedDBSingleton];
    [secondController insideBlock1:@"test" successBlock:
    ^BOOL(FMDatabase *db, SInt32 lastVersion)
    {
        NSLog(@"传来的db是什么%@,传来的lastVersion是什么%d",db,(int)lastVersion);
        return YES;
    }];
```
- 
###### 1.先执行
TwoViewController *secondController = [TwoViewController sharedDBSingleton];
在initVCSingleton中初始化了db为<FMDatabase: 0x60000008e920>
- 
###### 2.调用
[secondController insideBlock1:@"test" successBlock:
^BOOL(FMDatabase *db, SInt32 lastVersion)
的时候,是在block类内部调用,先执行
SInt32 lastVersion = 5555;
- 
###### 3.然后执行了
[self inTransaction:^(FMDatabase *db, BOOL *shouldRollback)
方法内部,因为block(_db, &shouldRollback);
所以执行的是下面的回调,传进来的两个参数就是_db和shouldRollback = NO
![1602974-ce28e49f53a39af3.png](https://upload-images.jianshu.io/upload_images/1602974-ce28e49f53a39af3.png)
下面的这个判断,就会去其他类_调用block中调用,并传过去_db和5555
![1602974-911544dd9a823aa1.png](https://upload-images.jianshu.io/upload_images/1602974-911544dd9a823aa1.png)
可以看到下图是返回YES
![1602974-7120bc230a67961b.png](https://upload-images.jianshu.io/upload_images/1602974-7120bc230a67961b.png)
所以执行下面这段
![1602974-61970f7db3c69389.png](https://upload-images.jianshu.io/upload_images/1602974-61970f7db3c69389.png)
