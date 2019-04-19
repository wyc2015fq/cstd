# Objective-C中@property和@synthesize的用法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月29日 13:48:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1330
在Objective-C中,@是Objective-C的标志,证明您正在使用Objective-C语言!
针对Objective-C语言中的关键词@property与@synthesize的使用进行了一个简要的分析(自己所理解的).
下面是自己编写的一个例子:建立一个Person类,其中省略了方法的声明.
**1)在.h头文件中:**
**@property在头文件中应用于声明:**
> 
例如:
@property NSInteger age;
编译器会自动在.h头文件中声明了2个方法:(setter,getter方法)
> 
- (NSInteger)age;      - (void)setAge:(NSInteger)age;
**2)在.m实现文件中:**
**@sythesize在.m实现文件中应用于实现:**
> 
例如:
@synthesize age = _age; 
编译器会自动在.m文件中实现2个方法:(setter,getter方法)
> 
- (void)setAge:(NSInteger)age
{
           _age  =  age;
}
- (NSInteger)age
{
           return   _age;
}
**请注意:**
在Xcode4.5以后的版本中,可以省略@synthesize,编译器会自动添加getter和setter方法的声明和实现,同时会默认访问_age这个成员变量,如果找不到这个变量,会自动生成_age的私有成员变量.
**3)@property语法的写法:**
> 
例如:
@property(nonatomic, retain) NSString *name;
@property(nonatomic, copy)NSString *gender;
**其中在@property后的()括号中的参数如下:(以及其意义)**
readwrite:可以产生和使用getter和setter方法.
readonly:只产生和使用getter方法,不能产生和使用setter方法.
assign:默认类型,在setter中直接进行赋值使用的,不进行retain操作
retain:setter方法中,对参数进行release释放旧值,再retain新值.释放旧对象,将旧对象的值赋予输入对象,再提高输入对象的引用计数为1.
copy:setter方法进行copy操作,与retain一样, 建立一个引用计数为1的对象,然后释放旧对象
nonatomic:禁止多线程，变量保护，提高性能
atomic:系统默认,这个属性是为了保证程序在多线程情况，编译器会自动生成一些互斥加锁代码，避免该变量的读写不同步问题。
strong:强引用,也是我们通常说的引用,其存亡直接决定了所指向对象的存亡,如果不存在指向一个对象的引用,并且此对象不再显示在列表中,则此对象会被从内存中释放.
weak:弱引用,不决定对象的存亡,即使一个对象被持有无数个弱引用,只要没有强引用指向它,那么还是会被清除
setter=: 对setter方法重新定义一个名字.
getter=: 对getter方法重新定义一个名字.

据上述所说的参数类型中,copy和retain是最为复杂的,下面就这两项进行一个简要的分析:
**retain:**
> 
例如:
@property(nonatomic, retain) NSString *name;
**1)setter方法分析:**
> 
- (void)setName:(NSString *)name
{
    if ( _name != name) {   //判断是否与旧值相同
         [ _name  release ];  //若新值与旧值不同则将旧值释放
        _name = [ name retain ];  //赋予新值,此时的引用计数+1
      }
}
**2)getter方法分析:**
> 
- (NSString *)name
{
return [ [ _name retain ]  autorelease ];
}
**copy:**
> 
例如:
@property(nonatomic, copy)NSString *gender;
**1)setter方法如下:**
> 
- (void)setGender:(NSString *)gender
{
    if ( _gender != gender) { //判断是否与旧值一样
        [ _gender   release ] ;    //释放旧值
        _gender = [ gender copy ];  //进行对原值的拷贝,开辟新的空间,将地址拷贝,并将新的空间的引用计数+1
      }
 }
**2)getter方法如下:**
> 
- (NSString *)gender
{
       return  [ _gender retain ] autorelease ];
}
**注意:**
1)nonatomic:如果使用多线程，有时会出现两个线程互相等待对方导致锁死的情况（具体可以搜下线程方面的注意事项去了解）。在没有(nonatomic)的情况下，即默认(atomic)，会防止这种线程互斥出现，但是会消耗一定的资源。所以如果不是多线程的程序，打上(nonatomic)即可
2)Objective-C语言,系统默认是ARC模式,所以不论是copy或者retain中的setter和getter,通过@synthesize自动生成getter和setter方法.
