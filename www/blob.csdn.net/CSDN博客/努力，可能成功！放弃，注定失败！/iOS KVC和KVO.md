# iOS KVC和KVO - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月07日 09:53:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：222标签：[iOS开发																[iOS																[KVC																[KVO](https://so.csdn.net/so/search/s.do?q=KVO&t=blog)](https://so.csdn.net/so/search/s.do?q=KVC&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
**写在前面**：
  关于KVC和KVO各种博客多了去了，重新整理下，就当是温习一下吧，也还算是个新手，不对的地方请指教，喜欢的点个喜欢什么也是挺好。
### 一，KVC
  KVC也就是key-value-coding,即键值编码，通常是用来给某一个对象的属性进行赋值，例如有人这么一个类，其对外有两个属性，姓名和年龄,我们在创建了一个人p后可以通过点语法直接给p赋值。
```cpp
```cpp
Person *p = [[Person alloc] init];
p.name = @"张三";
p.age = 20;
```
```
  我们也可以通过kvc给这个人p赋值,代码如下,因为setValue这里的值是id类型的,所以将整数包装成一个对象,
```bash
[p setValue:@"张三" forKey:@"name"];
    [p setValue:@20 forKey:@"age"];
```
  但是我们这样去赋值显得多此一举,可是如果人这个类的属性是没有暴露在外面呢?比如现在给人这个类一个私有的身高的属性,并且对外提供一个输出身高的接口,如下
```
#import "Person.h"
@implementation Person
{
    NSInteger _height;
}
- (void)logHeight
{    
    NSLog(@"%ld",_height);
}
@end
```
  这时候我们是没有办法去给人p直接设置身高的,外面我们访问不到它.但是有了kvc就不一样了。
```bash
[p setValue:@170 forKey:@"height"];
```
我们通过kvc可以直接对私有属性进行赋值,打印如下
![](http://upload-images.jianshu.io/upload_images/877100-9c1d4c73ef459a4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图1.png
  除了`[p setValue:@170 forKey:@"height"]`这个方法外,还有一个方法也是可以对私有属性进行赋值的`[p setValue:@170 forKeyPath:@"height"];`这两个方法对于一个普通的属性是没有区别的,都可以用,但是对于一些特殊的属性就有区别了。
比如说人这个类有个属性是狗,狗又有属性体重。
```bash
p.dog = [[Dog alloc] init];
   [p setValue:@200 forKey:@"dog.weight"];
```
  如果我们直接这样是会报错说找不到dog.weight这个key的,而在storyboard中,我们拖控件连线错误的时候也会报错说找不到什么key,说明storyboard在赋值的时候也是通过kvc来操作的。
  这里如果我们换另外的一个方法,这时候是不会报错的,而且可以打印出狗的体重.
```bash
[p setValue:@200forKeyPath:@"dog.weight"];
```
  说明`forKeyPath`是包含了`forKey`这个方法的功能的,甚至`forKeyPath`方法还有它自己的高级的功能,它会先去找有没有`dog`这个`key`,然后去找有没有`weight`这个属性。所以我们在使用kvc的时候,最好用`forKeyPath`这个方法。
最后还有一点,如下代码
```bash
[p setValue:@170 forKey:@"height"];
```
我们传入的字符串key是`height`,但是定义的属性是`_height`,但是通过kvc还是可以给`_height`属性赋到值。说明对某一个属性进行赋值,可以不用加下划线,而且它的查找规则应该是:先查找和直接写入的字符串相同的成员变量,如果找不到就找以下划线开头的成员变量。
  kvc除了访问私有变量这个用处外,还可以用于字典转模型。在`Person`类对外提供一个接口,将转模型的工作放在模型中进行
```
- (instancetype)initWithDict:(NSDictionary *)dict
{
   if (self = [super init]) {
        [self setValuesForKeysWithDictionary:dict];
    }
    returnself;
}
```
外面可以直接将字典传入,和平常转模型相比,kvc更加方便,减少了代码量。
```
NSDictionary*PersonDict = @{@"name":@"李四",@"age":@"18"};
  Person *p2 = [Person  personWithDict:PersonDict];
  NSLog(@"name = %@,age =%ld",p2.name,p2.age);
```
**所以kvc最常见的两种用法就是:**
> 
1，对私有变量进行赋值
2，字典转模型
但是也有一些需要注意的地方
> 
1，字典转模型的时候,字典中的某一个key一定要在模型中有对应的属性
2，如果一个模型中包含了另外的模型对象,是不能直接转化成功的。
3，通过kvc转化模型中的模型,也是不能直接转化成功的。
  既然可以通过kvc赋值,同样的也可以通过它进行取值。
```
NSLog(@"name=%@",[p2valueForKey:@"name"]);
NSLog(@"dogweight=%@", [p2.dogvalueForKeyPath:@"@p2.dog"]);
```
### 二，KVO
KVO，即key-value-observing,利用一个key来找到某个属性并监听其值得改变。其实这也是一种典型的观察者模式。
简单的说，kvo的用法非常简单。
1，添加观察者
2，在观察者中实现监听方法，`observeValueForKeyPath: ofObject: change: context:`（通过查阅文档可以知道，任何对象都有这个方法，因为这个方法属于NSObject）
3,移除观察者
具体代码如下：
```
//让对象b监听对象a的name属性
//options属性可以选择是哪个
 /*
     NSKeyValueObservingOptionNew =
0x01, 新值
     NSKeyValueObservingOptionOld =
0x02, 旧值
     */
    [a addObserver:b forKeyPath:@"name"options:kNilOptionscontext:nil];
  a.name = @"zzz";
#pragma mark - 实现KVO回调方法
/*
 *  当对象的属性发生改变会调用该方法
 *  @param keyPath 监听的属性
 *  @param object  监听的对象
 *  @param change  新值和旧值
 *  @param context 额外的数据
 */
- (void)observeValueForKeyPath:(NSString *)keyPath
ofObject:(id)object
change:(NSDictionary<NSString *,id>
*)change context:(void *)context
{
  NSLog(@"%@的值改变了,",keyPath);
    NSLog(@"change:%@", change);
}
```
最后不要忘记，和通知一样，要在dealloc方法里面移除监听
```
- (void)dealloc
{
    [a removeObserver:bforKeyPath:@"name"];
}
```
#### KVO的底层实现
  当一个类的属性被观察的时候，系统会通过runtime动态的创建一个该类的派生类，并且会在这个类中重写基类被观察的属性的setter方法，而且系统将这个类的isa指针指向了派生类，从而实现了给监听的属性赋值时调用的是派生类的setter方法。重写的setter方法会在调用原setter方法前后，通知观察对象值得改变。
  具体实现图如下，这里我拿的是iOS程序猿的图，借用一下应该没关系吧？
![](http://upload-images.jianshu.io/upload_images/877100-146caa2385fe8d2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图2.png
**最后**
  貌似有个facebook开源的工具，KVOController ，是一个简单安全的 KVO（Key-value Observing，键-值 观察）工具，好像挺好用的。
