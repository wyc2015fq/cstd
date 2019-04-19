# IOS之成员变量，属性变量，局部变量，实例变量，全局变量 - xqhrs232的专栏 - CSDN博客
2019年01月04日 17:18:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：60
原文地址::[https://blog.csdn.net/TenMios/article/details/51612350](https://blog.csdn.net/TenMios/article/details/51612350)
相关文章
1、viewcontroller的成员变量和属性在viewdidunload中的不同处理方式----[https://blog.csdn.net/wsmrcool/article/details/7916832](https://blog.csdn.net/wsmrcool/article/details/7916832)
2、ViewController 的loadView、viewDidLoad、viewDidUnload分别是什么时候调用的，在自定义ViewCointroller时在这几个函数中应该做什么工作？----[https://blog.csdn.net/yuanyuan1314521/article/details/51455588](https://blog.csdn.net/yuanyuan1314521/article/details/51455588)
3、IOS 成员变量，全局变量，局部变量定义，static与extern的区别----[https://blog.csdn.net/s3590024/article/details/51261399](https://blog.csdn.net/s3590024/article/details/51261399)
4、iOS Category中添加属性和成员变量的区别 评论也值得思考----[https://blog.csdn.net/mumuyinyin/article/details/72854579](https://blog.csdn.net/mumuyinyin/article/details/72854579)
5、IOS总结 静变量static、全局变量extern、局部变量、实例变量----[https://blog.csdn.net/chengyakun11/article/details/16942999](https://blog.csdn.net/chengyakun11/article/details/16942999)
6、解惑——iOS中成员变量和属性区别----[https://blog.csdn.net/u012946824/article/details/51788565](https://blog.csdn.net/u012946824/article/details/51788565)
成员变量：
@interface MyViewController :UIViewControlle 
{ 
UIButton *yourButton; 
int count; 
id data； 
} 
成员变量用于类内部，无需与外界接触的变量。成员变量默认是protected，一般情况下，非子类对象无法访问 
因为成员变量不会生成set、get方法，所以外界无法与成员变量接触 
成员变量是定义在｛｝号中的变量，如果变量的数据类型是一个类则称这个变量为实例变量。 
因为实例变量是成员变量的一种特殊情况，所以实例变量也是类内部使用的，无需与外部接触的变量，这个也就是所谓的类私有变量。
局部变量：
-（void）viewDidLoad 
{ 
// 局部变量 
NSArray *array = [[NSArray alloc] initWithObject:@“123”,nil]; 
} 
局部变量是根据其生存周期定义的，在源文件中的array，其生命周期是在以“{ }”为界限的代码块中，虽然它的名称与成员变量相同，但不是同一个变量。
属性变量： 
@interface MyViewController :UIViewControlle 
@property (nonatomic, strong) UIButton *myButton; 
@end
属性变量是用于与其他对象交互的变量。 
属性变量的好处就是允许让其他对象访问到该变量（因为属性创建过程中自动产生了set 和get方法）。 
当然，你可以设置只读或者可写等，设置方法也可自定义。 
现在大家似乎都不怎么喜欢用成员变量来定义类的变量， 
都喜欢用属性变量来定义类的变量。把需要与外部接触的变量定义在.h文件中，只在本类中使用的变量定义在.m文件中。
实例变量： 
实例变量本质上就是成员变量，只是实例是针对类而言， 
编译器会自动为你生成以下划线开头的实例变量 _myButton，也会自动为你生成setter，getter方法。 
如果.m文件中写了@synthesize myButton，那么生成的实例变量就是myButton；如果没写@synthesize myButton，那么生成的实例变量就是_myButton。
全局变量： 
  定义：在@implementation外定义的变量（在@implementation中定义也是可以但是一般不这么干） 
举例： 
//规范的 
static int hu=3;//全局变量 
NSString*sttr1=@”S1ViewController”;//全局变量 
@implementation S1ViewController 
@end 
