# ios知识点 - LC900730的博客 - CSDN博客
2017年06月25日 14:06:05[lc900730](https://me.csdn.net/LC900730)阅读数：198
## 获取plist信息
获取文件路径： 
NSString *filePath=[[NSBundle mainBundle] pathForResource：@“Info.plist”ofType:nil]; 
解析： 
NSDictory *dict=[NSDictory dictionarywithContentOfFile:filepath]
NSString *Version=dict[@”CFBundleShortVersionString”]; 
NSLog(@”%@”,Version);
第二种方式： 
Version=[NSBundle mainbundle].infoDictory[“CFBundleShortVersionString”];
## PCH文件
pch原理：pch里面的内容被项目中所有文件共有，但是需要提前编译 
1.存放公用的宏； 
2.存放一些公用的头文件(如果汇编入C文件，会报错) 
实际上是拷贝一份到各个文件 
3.自定义Log 
//…在宏里面表示可变参数 
函数里面表示可变参数是**VA_ARGS**
#define XMGLog(…)  NSLog(**VA__ARGS**) 
在注释的时候，就把NSLog()注释掉；
```
#ifdef DEBUG//系统定义
#define  XMGLog(...)  NSLog(__VA__ARGS__)
#else 
#define XMGLog(...)
#endif
```
注意点：如果有C文件，不需要导入OC语法 
#ifdef **OBJC**
xxx 
#endif
### UIApplication
什么是单例
- UIApplication对象是应用程序的象征；
- 每一个应用都有自己的UIAppication对象，而且是单列；
- 通过[UIApplication sharedApplication]可以获得这个单例对象
一个IOS程序启动后创建的 第一个对象就是UIApplication对象
利用UIApplication可以
每次程序启动，就会把所有的类加载到内存；UIApplication比main还先调用
```
#import "Person.h"
@implementation Person
static Person *_instance;
+(void) load{
    NSLog(@"%s",__func__);
    _instance=[[Person alloc] init];
}
+(instancetype)sharePerson{
    return _instance;
}
+(instancetype) alloc{
    if(_instance){
    //已经分配好了，不允许创建分配内存,创建异常类
    NSException *excp=[NSException exceptionWithName:@"NSIn"
                                              reason:@"there  can be "
    userInfo:nil];
    [excp raise];
    //return [super alloc];
}
    return [super alloc];
}
@end
```
### openUrl
NSURL *url=[NSURL URLWithString:@”[http://www.baidu.com](http://www.baidu.com)“]; 
根据协议头判断
IOS7以后状态栏默认交给控制器来管理了，如果希望给app来管理，则在info.plist配置，点击+，添加一个key，view controller prefer status toolbar为no
### app的代理：delegate
app很容易受到干扰，如来电，锁屏幕 
在app受到干扰的时候，会产生一些系统事件，这时UIApplication会通知它的delegate对象，让delegate代理来处理这些系统事件；
delegate可以处理事件包括
- 应用程序的声明周期事件(如程序的启动和关闭)
- 系统事件
- 内存警告
遵守UIApplicationDelegate协议 
每次新建完项目，都会有AppDelagate字样。
main函数里面调用UIApplicationMain (argc,argv,nil,NSStringFromClass([AppDelegate class]));
@param3:UIApplication类或者子类，如果指定nil，那么默认使用UIApplication，即@“UIApplication” 
@param4:UIApplication的代理名称， 
所以此时等于@“AppDelagate” 
NSStringFromClass:把类名转化成字符串(主要是有提示功能，避免写错)
UIApplication底层实现： 
1.根据principalClassName传递的类名创建UIApplication对象； 
2.创建UIApplication代理对象，给UIApplication对象设置代理； 
3.开启主运行事件循环，处理事件，保存程序一直运行 
4.加载info.plist文件，判断是否指定了main nib file。如果指定了，就会加载。
### main  run loop
主运行循环：
### UIWindow
- 是一种特殊的控件：通常在一个app中至少有一个UIWindow
- ios启动之后，创建的第一个视图控件就是UIWindow，接着创建控制器的view，最后将控制器的view添加到UIWindows上，于是控制器的view就显示在屏幕上
- 一个ios程序之所以能够显示到屏幕上，完全是因为有UIWindow也就是说没有UIWindow，就看不见任何界面
info.plist中定义了main storyboard，如果定义了，那么程序启动之后，ios会帮你执行以下步骤： 
1.实例化一个window提供给你； 
2.加载main storyboard，实例化启动view controller； 
3.将启动view controller赋值给窗口的rootViewController，然后显示这个window 
```
//1.创建窗口对象
    self.window=[[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    //2.设置窗口的根控制器
    //UIViewController *rootVc=[[UIViewController alloc]init];
    self.window.rootViewController=rootVc;
    //3.显示窗口
    [self.window makeKeyAndVisible];
    return YES;
```
注意点： 
1.不能让窗口销毁，需要弄一个强引用； 
self.window; 
2.必须设置窗口的尺寸； 
App  delegate
推荐使用控制器的原因：遵循mvc，对应界面的事情交给对应的控制器去管理
一旦设置窗口根控制器，就会把根控制器的view添加到窗口 
还可以： 
[self.window addSubview:rootVc.view]这样也可以添加根控制器的view添加到窗口 
推荐使用rootViewController:做了旋转功能
默认窗口显示是隐藏的，self.window.hidden=NO;则改成显示
称为应用程序的主窗口。 
[UIApplication ]
### 窗口
状态栏是比较特殊的窗口； 
键盘也是一个窗口； 
application.windows会报错整个应该程序的窗口
UITextField *textF=[[UITextField alloc]init];
category：在现有的类中增加一些方法，只能扩展函数，消息，不能扩展字段，变量等。 
 命名规范： 
 NSString +ReverseString.h 
  NSString +ReverseString.m
## NSString 
```
NSString *str=@"hello world";
用一个字符串初始化另一个字符串
NSString *str2=[[NSString alloc]initWithString :str]
//用C语言方式创建字符串
NSString *str3=[[NSString alloc] initWithUTF8String:"hello world"];
//用C的字符串创建OC的字符串对象
NSString *str4=[[NSString alloc] initWithFormat:@"hello %d %c",5,'A'];
alloc分配的需要release释放
[str2 release]
[str3 release]
[str4 release]
//将C字符串转换成OC对象
NSString *str5=[NSString stringWithUTF8String:"hello world!"];
OC允许我们给参数添加一个标签来说明当前参数的意义
注意标签也是方法名的一部分；
-(int) sendMessagewithNumber:(int)number andContent:(char *)content;
```
## 类方法与对象方法：
- 对象方法可以直接访问自己访问属性(成员变量)；
- 类方法中不可以直接调用访问属性；
- 对象方法和类方法可以相互调用;但是开发中不推荐使用
- 类方法的效率比对象方法效率高
- 类方法中可以调用类的其他方法
- 对象方法中可以直接调用对象方法；可以在对象方法中创建对象来调用(知道可以如此调用)，开发中是对象方法中用self直接调用其他对象方法
- 如果方法中没有用到属性，那么能用类方法就用类方法，执行效率高
## 类方法
```
typedef enum {
KIColorBlack,KIColorwhite,KIColorTuhaojin
}IColor;
```
## 类对象
类对象与实例对象
- 实例对象的第0个属性是isa，是一个指针，占8个字节，会指向创建这个对象的那个类
- 创建对象其实就是通过类对象来创建实例对象
- 类对象是系统创建的 ，里面保存着当前对象的所有方法
## 成员变量
### 局部变量
写在类声明的大括号中的变量，我们称为成员变量(属性，实例变量)； 
//成员变量只能通过对象来访问； 
//成员变量不能离开类，离开类之后就不是成员变量，成员变量不能再定义的同时初始化； 
存储：堆区(当前对象对应的堆的存储空间); 
存储在堆中的数据不会自动释放，只能程序员手动释放
### 全局变量
//写在函数或者大括号外部的 变量，我们称为全局变量 
作用域：从定义的开始，一直到文件末尾； 
局部变量可以先定义再初始化，也可以定义的同时初始化，存储在静态区，程序启动就会分配存储空间，直到程序结束释放；
## 函数和方法
1.函数属于整个文件，方法属于某一个类；方法离开类就不行 
2.函数可以直接调用，方法必须是对象或者类来调用 
3.函数属于整个文件，可以写在类的实现或者类的外部，但是不能写在类的声明中；(即不能写在@interface和@end之间)；
方法的注意点：
- 方法可以没有声明，只有实现
- 方法可以只有声明，没有实现，那么编译不会报错，运行的时候会reason: ‘+[XX XX ] unrecognised selector send to class(如果是实例对象调用方法，那么就是-开头，send to instance)
//外部函数 
extern void sum(){} 
//内部函数： 
static void minus(){}
OC中类的本质是一个结构体，所以我们用一个指向指针的结构体来操作对象，但是对象的属性默认是受保护的，必须把属性变为公开的才可以直接访问。
结构体做函数参数其实是拷贝了一份
