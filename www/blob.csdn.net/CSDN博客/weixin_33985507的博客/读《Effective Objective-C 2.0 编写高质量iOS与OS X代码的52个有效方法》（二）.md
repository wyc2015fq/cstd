# 读《Effective Objective-C 2.0 编写高质量iOS与OS X代码的52个有效方法》（二） - weixin_33985507的博客 - CSDN博客
2018年06月05日 22:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
  本文包含**第三章：接口与API设计**、**第四章：协议与分类**、**第五章：内存管理**。
# 用前缀避免命名空间问题
  因为OC不支持命名空间，所以在定义类、方法等时应尽量加入自己的公司前缀或者名字前缀什么的，最好是三个字母的，比如XYZViewController，为什么不是两位数呢？因为苹果宣称保留使用“两字母前缀”的权利...比如UI、NS等等...
# 实现description方法
  为自己创建的类定义description方法非常有用，这样在使用NSLog打印当前对象的时候，就可以自定义打印一些对象相关的信息了，like this：
```
// 可以包含类名、地址名、以及打印一些属性的值等，看自己需求来扩充
- (NSString *)description {
    return [NSString stringWithFormat:@"<%@: %p, \"%@ %@\">",
            [self class], self, _name, _sex];
}
// 调用和打印的时候可以这样用
ZHCaluVC *vc = [[ZHCaluVC alloc] init];
vc.name = @"小明";
vc.sex = @"男";
NSLog(@"%@", vc);
// 打印如下
<ZHCaluVC: 0x133d06ff0, "小明 男">
```
  另外如果是在控制台用po的方式，查看某个对象的调试信息，NSObject还提供了一个debugDescription方法用于在控制台调试用：
```
// 可以打印一些简单的属性
- (NSString *)description {
    return [NSString stringWithFormat:@"%@ %@", _name, _sex];
}
// 可以打印一些需要调试用的信息
- (NSString *)debugDescription {
    return [NSString stringWithFormat:@"<%@: %p> %@ %@", [self class], self, _name, _sex];
}
// 创建一个对象
ZHCaluVC *vc = [[ZHCaluVC alloc] init];
vc.name = @"小明";
vc.sex = @"男";
```
  在控制台中直接po vc的话，可以看到调用了debugDescription方法，当然如果不重写debugDescription，则在po的时候会直接调用description方法：
![1682338-6eaff19fbd4d6eed.png](https://upload-images.jianshu.io/upload_images/1682338-6eaff19fbd4d6eed.png)
# readonly的运用
  当一个类对外暴露的属性，不希望被其他类修改的时候，应尽量设置为readonly，当然如果属性所在类想修改这个属性，可以在匿名内部类中将该属性设置为readwrite（貌似Swift就没法实现类似的效果了？）：
```
// .h中
@interface ZHCaluVC : UIViewController
// 设置为对外readonly
@property (nonatomic, copy, readonly) NSString *name;
@end
// .m中
@interface ZHCaluVC ()
// 设置为对自身readwrite
@property (nonatomic, copy, readwrite) NSString *name;
@end
```
# 为私有方法名加前缀
  可以为一个类内部要用的私有的方法加上一个前缀，比如p_，代表private，用于跟其他对外的方法做区分：
```
// 私有方法
- (void)p_someMethod {
}
```
  然后我突发奇想，如果一个创建UI的方法，需要创建一整个页面，如果方法过长，需要拆分的话，是不是可以用s_(sub)来表示分支的方法呢？
```
- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self p_createUI];
}
// 总的创建UI方法
- (void)p_createUI {
    [self s_createTopUI];
    [self s_createBottomUI];
    [self s_createBottomUI];
}
// 几个分支的创建UI的方法
- (void)s_createTopUI {
    // 创建顶部UI
}
- (void)s_createMiddleUI {
    // 创建中间UI
}
- (void)s_createBottomUI {
    // 创建底部UI
}
```
# NSException和NSError
  NSException应该用于那种导致App崩溃的严重错误，而NSError可用于一些不严重的错误，而且NSError包含了很多错误相关的信息，可用于处理错误：
```
// 可以设置ErrorDomain、code、userInfo
NSError *error = [NSError errorWithDomain:NSURLErrorDomain code:404 userInfo:nil];
```
# 理解NSCopying协议
  如果一个类可以被copy，就需要实现NSCopying中的- copyWithZone方法（这也是为什么在写单例对象的时候，需要重写该方法）：
```
// 假设当前类有一个属性
@property (nonatomic, copy, readwrite) NSString *name;
// 一个实例变量
{
NSString *_sex;
}
// 需要先遵守NSCopying协议
- (id)copyWithZone:(NSZone *)zone {
    // NSZone不用管，以前内存中是分为不同的zone的，现在只有一个zone了->default zone
// 生成一个当前类的实例，并把当前对象的name属性copy给新的实例，并返回这个实例
// 这里需要注意就是name这个属性应该copy，因为两个对象属于拷贝关系
// 则name这个属性两个对象不应该共享，而是应该各自持有一份
ZHCaluVC *vc = [[ZHCaluVC alloc] init];
// 属性可以直接用点语法set
vc.name = [self.name copy];
// 实例变量需要用"->"
vc->_sex = [_sex copy];
    
return vc;
}
```
  当前类的对象的copy方法实际上就调用了copyWithZone这个方法：
![1682338-6ec9f605d4b70fc7.png](https://upload-images.jianshu.io/upload_images/1682338-6ec9f605d4b70fc7.png)

![1682338-5b3abe8f6b27ca98.png](https://upload-images.jianshu.io/upload_images/1682338-5b3abe8f6b27ca98.png)
  copy和mutableCopy总结：copy的话，应该总是返回一个不可变的实例，mutableCopy应该总是返回一个可变的实例。
  关于深拷贝和浅拷贝：深拷贝就是在拷贝一个对象后，将其底层数据一并拷贝，比如上面的例子，而NSArray、NSDictionary在拷贝的时候都是浅拷贝。而深浅拷贝实际上是没有专门的协议对一个类的深浅拷贝方法进行限制的，一般实现NSCopying中的copy方法的时候，应该设置为浅拷贝，如果想提供一个深拷贝功能，也可以自定义一个`- deepCopy`方法
# 代理模式的性能优化
  核心思想是减少对delegate进行respondsToSelector的操作次数，emmmmm，还是上代码吧，主要就是把[delegate respondsToSelector]缓存到结构体中，存储起来：
```
// .h
@protocol SomeClassDelegate <NSObject>
- (void)didDoSomethingA;
- (void)didDoSomethingB;
- (void)didDoSomethingC;
@end
@interface SomeClass: NSObject
@property (nonatomic, assign) id<SomeClassDelegate> delegate;
@end
```
```
// .m
@interface SomeClass () {
    struct data {
        // 利用“位段”来设置结构体中某个字段所占用的二进制位个数
        // 如下为1，则可以表示0和1，如果是8，则可以表示0-255
        unsigned int didDoSomethingA : 1;
        unsigned int didDoSomethingB : 1;
        unsigned int didDoSomethingC : 1;
    } _delegateFlags;
}
@end
@implementation SomeClass
// 在setDelegate方法中，缓存delegate是否已经实现了代理方法
- (void)setDelegate:(id<SomeClassDelegate>)delegate {
    _delegate = delegate;
    _delegateFlags.didDoSomethingA = [delegate respondsToSelector:@selector(didDoSomethingA)];
    _delegateFlags.didDoSomethingB = [delegate respondsToSelector:@selector(didDoSomethingB)];
    _delegateFlags.didDoSomethingC = [delegate respondsToSelector:@selector(didDoSomethingC)];
}
- (void)doSomeAction {
    // 直接使用缓存好的flag来判断delegate是否实现了某个代理方法
    // 以此减少多次判断respondsToSelector带来的性能问题
    if (_delegateFlags.didDoSomethingA) {
        [self.delegate didDoSomethingA];
    }
}
@end
```
  虽然我觉得对性能的影响并不大...但对于那些需要周期性调用的代理方法，比如每秒刷新UI什么的，还是很有用的。
# 将类的实现代码分散到便于管理的数个分类之中
  可以将一个类中的多个方法，进行归类，然后分散到当前类的多个分类中，这样可以方便管理不同类型的方法，比如处理UI的、处理网络请求的、处理数据包装的等等（当然MVC可以这样，MVVM的话，逻辑就可以放到ViewModel中了）。一些代理的实现，也可以放到分类中去实现，方便管理，比如UITableViewDelegate、UIAlertViewDelegate等：
```
// 定义一个ZHCaluVC类的分类，并遵守UITableViewDelegate
@interface ZHCaluVC (TableView) <UITableViewDelegate>
@end
@implementation ZHCaluVC (TableView)
// 实现UITableViewDelegate的方法
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
}
@end
```
  当然我个人觉得OC的Category和Swift的Extension比，还是有些麻烦的，因为代理也是需要声明@interface的(.h和.m中都可以有)，一个类分散出多个分类，也会导致声明很多@interface，不便于管理，而且分类在新增属性时也比较麻烦，所以以上做法在Swift中使用Extension来做会更理想。
# 给分类添加属性
  分类虽然可以通过@property添加属性，但不会自动添加get/set方法，这样在访问分类中的属性的时候会因为找不到get/set方法而导致崩溃，编译器也告诉了我们这些信息：
![1682338-4aff53dd315b8e3c.png](https://upload-images.jianshu.io/upload_images/1682338-4aff53dd315b8e3c.png)
  不过上面的警告信息已经很明显的告诉了我们解决办法，通过@dynamic+关联对象的方式实现：
```
// .h
@interface SomeClass (SomeCategory)
// 在分类中声明一个属性
@property (nonatomic, copy) NSString *name;
@end
```
```
// .m
static const char *kNamePropertyKey = "kNamePropertyKey";
@implementation SomeClass (SomeCategory)
// 声明自己实现get/set方法
@dynamic name;
// 通过关联对象的方式实现get/set方法
- (NSString *)name {
    return objc_getAssociatedObject(self, kNamePropertyKey);
}
- (void)setName:(NSString *)name {
    objc_setAssociatedObject(self, kNamePropertyKey, name, OBJC_ASSOCIATION_COPY_NONATOMIC);
}
@end
```
  不过还是不建议在分类中新增属性的，毕竟脱离开主接口，这些属性存在意义可能不大。
# 编写“异常安全代码”时留意内存管理问题
  try/catch中的变量，arc是不会自动帮我们release的，也就是下面的代码，如果在try中发生了异常，实际上arr这个变量是不会被释放掉的，也就引发了内存泄漏，当然arc也不允许手动调用[arr release]。
```
@try {
    NSArray *arr = [[NSArray alloc] init];
    arr[0];
} @catch (NSException *exception) {
} @finally {
}
```
  当然如果想让arc帮我们自动释放，需要在项目设置中的“other linker flag”中添加-fobjc-arc-exceptions，这样在编译时arc就会自动帮我们加入try/catch中的内存管理相关代码，当然也会拖慢编译速度。
# 关于unsafe_unretained
  简单理解的话，__weak弱引用一个对象，当对象释放后，指针置空，也就是nil，当调用方法的时候，不会引发崩溃，虽然unsafe_unretained也是弱引用一个对象，但对象释放后，指针指向的还是释放的对象的内存，当调用方法的时候，就会引发崩溃。Swift中有**unowned(无主引用)**的概念跟这个对应。
# 关于自动释放池
  实际上自动释放池这一块，之前在面试的时候，就被问过一次，当时的题目是这样的（貌似是这样的），下面的方法，如何避免内存方面的问题：
```
- (void)dealString {
    for (int i=0; i<10000; i++) {
        NSString *str = [NSString stringWithFormat:@"%d", i];
        NSLog(@"%@", str);
    }
}
```
  当然我答上来了哈哈哈，书中也是这么写的：
```
- (void)dealString {
    for (int i=0; i<10000; i++) {
        
        @autoreleasepool {
            NSString *str = [NSString stringWithFormat:@"%d", i];
            NSLog(@"%@", str);
        }
        
    }
}
```
  正常来说，我们声明的10000个局部变量str，会在方法执行结束进行释放，也就是循环结束，但如果加了自动释放池，局部变量出了自动释放池就释放了，所以局部变量是一边创建一边释放的，会避免局部内存高峰。
# 僵尸对象
  这一节干货太多了，不好总结，参见：[第35条:用“僵尸对象”调试内存管理问题](http://www.cnblogs.com/chmhml/p/7350374.html)。大体说下来，就是如果编辑target的scheme，打开“Enable Zombie Objects”后，在调试阶段，所有对象dealloc时，都会被系统自动替换为NSZombie类，并保留当前对象的一些信息，方便调试阶段定位空指针问题。
