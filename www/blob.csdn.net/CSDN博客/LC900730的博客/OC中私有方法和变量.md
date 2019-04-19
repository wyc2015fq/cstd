# OC中私有方法和变量 - LC900730的博客 - CSDN博客
2017年06月25日 23:36:34[lc900730](https://me.csdn.net/LC900730)阅读数：298
## OC中私有变量
实例变量既可以在interface中定义，也可以在implementation中定义。 
区别在于：写在implementation默认是私有成员变量，并且和利用@privated的修饰的不太一样。@privated在其他类中可以查看，而@implementation中定义的成员变量中在其他类中无法查看，也无法访问。在@implementation中定义的私有变量只能在本类中访问。
## 私有方法
如果只有方法的实现，没有方法的声明(即只在.m中实现,没有在.h声明)。 
OC中没有真正的私有方法，只不过看起来像私有方法。
## property:setter与getter声明的简化
setter与getter方法啃需要很多，除了变量名称类型不一样，其余都一样，为了简化操作，xcode推出property 
@property是一个编译器指令； 
在Xcode4.4之前，可以使用@property来代替setter与getter方法的声明
```
@property int age;那么生成的就是
setAge;age
@property int _age;那么生成的就是
set_age(); _age()
```
## setter与getter方法的实现简化
@synthesize是一个编译器指令，可以简化我们getter与setter方法的实现(即在声明之后写上{}) 
@synthesize age; 
会生成如下代码： 
-(void)setAge:(int)age{ 
} 
但是无法实现具体赋值 
所以写法如下： 
@synthesizer age=_age;(即将传入的age赋值给_age,调用age的时候返回_age的值）。 
-(void)setAge:(int )age{ 
_age=age; 
}; 
-(int) age{ 
return _age; 
} 
如果synthesis没有传入赋值给谁，系统默认会赋值给synthesize后面写得名称相同的成员变量；
## @property增强
Xcode4.5之后，一个@property就会同时完成setter与getter方法声明和实现。(没有告诉@property要传入的参数赋值给谁，默认@property会将传入的属性赋值给_开头的成员变量)；
```
@property int age;
-(void) setAge:(int)age;
-(int)age;
@end
@implementation Person
-(void) setAge:(int)age{
    _age=age;
};
-(int)age{
    return _age;
};
@end
```
@property有一个弊端：只会生成简单的setter/getter方法，不会对传入的数据进行过滤，如果想对传入的数据进行过滤，就要重写setter/getter方法；如果重写了setter方法，那么property就会生成getter方法；如果重写了getter方法，那么property就会生成setter方法。
如果利用@property，那么可以不写成员变量，系统会自动给我们生成一个*开头的成员变量。(注意@property自动生成的*开头的变量是私有的，即.m中生成的，在其他类中无法查看到)；如果同时重写了setter/getter方法，那么Xcode就不会帮我们自动生成_开通的变量。
## property修饰符
@property(属性修饰符) 数据类型 变量名称； 
@property (readwrite) int age;   //可读写 
@property(readonly) int age;   //只读 
@property(getter=abc)  double height; 
(默认情况下生成的是-(double) height,此时生成的是-(double) abc;） 
@property(setter=tizhong:) double weight; 
-(void )setWeight:(double)weight; 
-(void)tizhong:(double)weight;
## 动态数据类型：id
既然是数据类型，那么可以 
1.定义变量； 
2.作为函数的参数； 
3.作为函数的返回值。
特点：在运行的时候才会确定它的真实类型，并且如果通过动态数据类型定义的变量，如果访问了不属于动态数据类型的属性和方法，编译器不会报错。 
id==NSObject * //万能指针，区别是NSObject *是静态数据类型，(不能直接调用子类特有的方法)，动态数据类型可以调用子类特有的方法，且动态数据类型定义的变量可以调用私有方法。 
弊端：由于可以调用任意方法，所以有可能调用到不属于自己的方法，而编译器不会报错，所以可能导致运行时的错误。 
应用场景：一般用在多态，可以减少代码量，避免调用子类特有方法需要强制转换。
为了避免动态数据类型引发的运行时候错误，一般情况下如果使用动态数据类型定义的一个变量，在调用这个变量的方法之前会进行一次判断，判断当前变量是否能够调用这个方法 
id obj=[Student new]; 
if([obj isKindOfClass :[Student class]]){ 
//判断指定的对象是否是某一个类，或者是某一个类的子类 
//isMemberOfClass:判断指定 的对象是否是当前指定的类的实例 
}
## new的实现原理
new: 
1.开辟存储空间 
2.初始化所以的属性 
3.返回对象的地址； 
(是alloc init的组合) 
alloc做了什么事情：1.开辟存储空间；2.将所有属性设置为0；3.返回当前实例对象地址 
但是需要调用init，虽然init什么都没有做。 
Person *p1=[Person alloc]; 
Person *p2=[p1 init]; 
p1与p2地址一样；
## 构造方法
用于初始化一个对象，让某个对象一创建出来就具有某些属性和值； 
//常规情况下，每次都是创建对象，在设置属性； 
//工作中：重写init方法(必须按照苹果格式来写)
```
//1.必须先初始化父类；
//2.必须判断父类是否初始化成功，只有父类初始化成功才能继续初始化子类
//返回当前对象的地址
-(instancetype)init{
    self=[super init];
    if(self!=nil){
        _age=6;
    }
    return self;
//写法二：
//if(self=[super init]){}   //判断self的值
}
```
## instancetype与id
与id类似，都是万能指针； 
区别： 
如果init方法返回值是instancetype，赋值给其他类型的对象会报警告；如果是在以前，init返回值是id，那么将init返回的对象地址 赋值给其他对象是不会报错的。 
如NSString *str=[[Person  alloc]init]; 
NSUInteger length= [str length];（以前会报错，因为person对象没有length对象） 
而instancetype在编译的时候可以判断真实类型； 
此外：id可以定义变量，可以做形式参数的类型 
如：-(void)test:(id) obj1; 可以做返回值； 
而instancetype只能做返回值； 
以后自定义构造方法，返回值尽量用instancetype，不要使用id
