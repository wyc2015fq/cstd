# iOS开发--调试必备 — NSLog - xqhrs232的专栏 - CSDN博客
2019年02月22日 15:51:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：33
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.cnblogs.com/wanghuaijun/p/5391376.html](https://www.cnblogs.com/wanghuaijun/p/5391376.html)
相关文章
1、IOS nslog----[https://blog.csdn.net/wty947825770/article/details/81508940?utm_source=blogxgwz2](https://blog.csdn.net/wty947825770/article/details/81508940?utm_source=blogxgwz2)
2、[iOS项目之NSLog相关](https://www.cnblogs.com/sjxjjx/p/6526626.html)----[https://www.cnblogs.com/sjxjjx/p/6526626.html](https://www.cnblogs.com/sjxjjx/p/6526626.html)
3、ios NSLog 控制台 输出中文----[https://www.jianshu.com/p/d4fe838b04fd](https://www.jianshu.com/p/d4fe838b04fd)
4、[再见了NSLog](https://www.cnblogs.com/sunnyxx/p/3680623.html)----[https://www.cnblogs.com/sunnyxx/p/3680623.html](https://www.cnblogs.com/sunnyxx/p/3680623.html)
> 
对于程序的开发者来说，拥有一手强大的DEBUG能力，那就好比在武侠世界中拥有一种强大的内功心法一样，走到哪里都是大写的牛B。在我们DEBUG的时候，大部分情况都是要查看我们的调试日志的，这些打印日志可以帮我们精确的定位问题的位置。在OC的编程中，我们一般使用NSLog函数来进行一些打印的工作，这大致相当于C语言的printf输出语句。
## 1.基本使用
NSLog定义在NSObjCRuntime.h中，如下所示：
void NSLog(NSString *format, …);
省略号表示可接收多个参数。
NSLog使用起来和printf是很相似的，都是格式化的输出，不同的是printf需要的格式化字符串是char *类型，而NSLog需要的字符串是NSString型。
NSLog中会使用到的格式化占位符：
- %@ 对象
- %d, %i 整数
- %u 无符整形
- %f 浮点/双字
- %x, %X 二进制整数
- %o 八进制整数
- %zu size_t
- %p 指针
- %e 浮点/双字 （科学计算）
- %g 浮点/双字
- %s C字符串
- %.*s Pascal字符串
- %c 字符
- %C unichar
- %lld 64位长整数（long long）
- %llu 无符64位长整数
- %Lf 64位双字
示例：
- // 直接打印字符串
- NSLog(@"this is a string");
- 
- // 打印OC对象
- NSString *string = @"hello world";
- NSLog(@"%@", string);
- 
- // 打印基本数据类型
- NSLog(@"int : %d, float : %f", 2, 3.14);
![QQ20160224-0](http://7xow65.com1.z0.glb.clouddn.com/wp-content/uploads/2016/02/QQ20160224-0.png)
## 2. description 方法
通过上面的例子我们不难发现在NSLog中使用%@占位符来打印对象的功能是非常强大的，它不仅可以打印字符串、字典、数组等OC中已经定义的对象类型，我们自定义的对象类型也可以使用%@来打印。NSLog配合%@使用时，编译器会自动调用被打印对象的description方法，如果打印的是类对象则调用类方法，实例对象则调用实例方法。继承于NSObject类的description方法默认会打印该对象的类名和它在内存中的地址。
新建一个People类，自定义description方法，调试打印。
People.h:
- #import <Foundation/Foundation.h>
- 
- @interface People : NSObject
- 
- @property (nonatomic, copy) NSString *name;
- @property (nonatomic, copy) NSString *sex;
- @property (nonatomic, assign) int age;
- 
- @end
People.m:
- #import "People.h"
- 
- @implementation People
- 
- // 不要尝试在description方法中返回self，因为会出现死循环
- - (NSString *)description {
- /*
- // 错误示范
- return [NSString stringWithFormat:@"%@", self];
- */
- return [NSString stringWithFormat:@"name: %@, sex: %@, age: %d",
- self.name, self.sex, self.age];
- }
- 
- 
- + (NSString *)description {
- return @"People";
- }
- 
- @end
测试代码：
- // 创建并初始化一个People实例p
- People *p = [[People alloc] init];
- p.name = @"jack";
- p.sex = @"man";
- p.age = 18;
- 
- // 打印实例p，调用自己定义的description实例方法
- NSLog(@"%@", p);
- 
- // 打印People类，调用自己定义的description类方法
- NSLog(@"%@", [p class]); // 类对象或者实例对象调用class方法会返回对象的类的所有信息，返回值为Class类型
![QQ20160224-1](http://7xow65.com1.z0.glb.clouddn.com/wp-content/uploads/2016/02/QQ20160224-1.png)
自己实现description方法可以打印我们想要看到的内容，更加便于调试，但是在description方法中不要返回或者打印self。
## 3.自定义打印日志输出内容
从上面的例子我们可以看到使用NSLog打印出来的日志前面带有很长一串的时间戳，可能有时候我们根本用不到，而且还会影响查看的效率。其实对于NSLog的输出格式我们也是可以自定义的，只需要我们写一个宏定义就可以了。
如果只想让NSLog打印我们输出的内容：
- #define NSLog(FORMAT, ...) fprintf(stderr,"%s\n",[[NSString stringWithFormat:FORMAT, ##__VA_ARGS__] UTF8String]);
![QQ20160224-2](http://7xow65.com1.z0.glb.clouddn.com/wp-content/uploads/2016/02/QQ20160224-2.png)
附加输出文件名和打印语句的行号：
- #define NSLog(FORMAT, ...) fprintf(stderr,"%s:%d\t %s\n",[[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String], __LINE__, [[NSString stringWithFormat:FORMAT,##__VA_ARGS__] UTF8String]);
![QQ20160224-3](http://7xow65.com1.z0.glb.clouddn.com/wp-content/uploads/2016/02/QQ20160224-3.png)
由于NSLog的使用**效率比较低**，所以在我们的项目中非调试状态下不应该出现大量的NSLog，所以有些时候我们会在项目的.pch文件中去定义一个宏，让调试打印函数只在调试的时候有用，发布的时候就不能使用。
在Xcode6之前，项目中Supporting files文件夹下有个 “工程名-Prefix.pch”文件,你可以在该文件下声明我们的宏，这样之后工程中的任何一个文件中都可以使用该宏(也就是说可以在.pch中放一些全局的东西)。Xcode6之后，可能是因为大家把大量的头文件和宏定义放到pch里边，导致编译时间过长，苹果就去掉了pch文件，但是我们仍可以手动添加。
实用版本：
- #ifdef DEBUG
- #define NSLog(FORMAT, ...) fprintf(stderr,"%s:%d\t %s\n",[[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String], __LINE__, [[NSString stringWithFormat:FORMAT,##__VA_ARGS__] UTF8String]);
- #else
- #define NSLog(...)
- #endif
