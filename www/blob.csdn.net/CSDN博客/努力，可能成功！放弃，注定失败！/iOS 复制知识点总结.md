# iOS 复制知识点总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年11月06日 09:32:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：127标签：[iOS 复制知识点总结																[iOS 资源大全](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 复制知识点总结&t=blog)
个人分类：[iOS开发																[OC](https://blog.csdn.net/qq_30513483/article/category/6078567)](https://blog.csdn.net/qq_30513483/article/category/6072895)
原文出处：[檀羽冲](http://www.jianshu.com/p/6f635cb5595e)
1、系统对象的复制
不管是集合类对象，还是非集合类对象，接收到`copy`和`mutableCopy`消息时，都遵循以下准则：
- `copy`返回`immutable`对象；所以，如果对copy返回值使用`mutable`对象接口就会crash；
- `mutableCopy`返回`mutable`对象；
下图详细阐述了`NSString`、`NSMutableString`、`NSArray`、`NSMutableArray`、`NSDictionary`、`NSMutableDictionary`分别调用`copy`与`mutableCopy`方法后的结果：
![112030896-5b744f044027a0c1](http://jbcdn2.b0.upaiyun.com/2017/08/9177e800af6fb9b24ca2fececdb0f4e1.png)
## 1.1 非集合类对象的copy与mutableCopy
系统非集合类对象指的是`NSString`，`NSNumber`之类的对象。下面看一下非集合类`NSString`对象拷贝的例子：
### copy


```
```
|1234567|NSString *string=@"origin";NSString *stringCopy=[stringcopy];NSMutableString *stringMCopy=[stringmutableCopy];NSLog(@"%p",string);NSLog(@"%p",stringCopy);NSLog(@"%p",stringMCopy);|

``
|123|2016-03-1017:32:04.479Homework[21715:2353641]0x1000d0ea02016-03-1017:32:04.481Homework[21715:2353641]0x1000d0ea02016-03-1017:32:04.481Homework[21715:2353641]0x17006fe40|
通过查看内存，可以看到`stringCopy`和`string`的地址是一样，进行了指针拷贝；而`stringMCopy`的地址和`string`不一样，进行了内容拷贝。
### mutableCopy


```
```
|12345678910|NSMutableString *string=[NSMutableString stringWithString:@"origin"];//copyNSString *stringCopy=[stringcopy];NSMutableString *mStringCopy=[stringcopy];NSMutableString *stringMCopy=[stringmutableCopy];NSLog(@"%p",string);NSLog(@"%p",stringCopy);NSLog(@"%p",mStringCopy);NSLog(@"%p",stringMCopy);|

``
|1234|2016-03-1017:34:11.486Homework[21728:2354359]0x17426f8002016-03-1017:34:11.487Homework[21728:2354359]0x1742306002016-03-1017:34:11.487Homework[21728:2354359]0x1742306e02016-03-1017:34:11.487Homework[21728:2354359]0x174267240|


```
```
|1234|//change value[mStringCopy appendString:@"mm"];//crash[stringappendString:@" origion!"];[stringMCopy appendString:@"!!"];|
crash的原因就是`copy`返回的对象是`immutable`对象。
## 1.2 集合类对象的copy与mutableCopy
集合类对象是指`NSArray`、`NSDictionary`、`NSSet`之类的对象。下面看一下集合类`NSArray`对象使用`copy`和`mutableCopy`的一个例子：
### copy

``
|1234567|NSArray *array=@[@[@"a",@"b"],@[@"c",@"d"]];NSArray *copyArray=[arraycopy];NSMutableArray *mCopyArray=[arraymutableCopy];NSLog(@"%p",array);NSLog(@"%p",copyArray);NSLog(@"%p",mCopyArray);|


```
```
|123|2016-03-1017:53:40.113Homework[21775:2358227]0x17403f0402016-03-1017:53:40.114Homework[21775:2358227]0x17403f0402016-03-1017:53:40.114Homework[21775:2358227]0x174247e60|
可以看到`copyArray`和`array`的地址是一样的，而`mCopyArray`和`array`的地址是不同的。说明copy操作进行了指针拷贝，mutableCopy进行了内容拷贝。但需要强调的是：此处的内容拷贝，仅仅是拷贝`array`这个对象，`array`集合内部的元素仍然是指针拷贝。这和上面的非集合`immutable`对象的拷贝还是挺相似的，那么`mutable`对象的拷贝会不会类似呢？我们继续往下，
### mutableCopy

``
|1234567|NSMutableArray *array=[NSMutableArrayarrayWithObjects:[NSMutableStringstringWithString:@"a"],@"b",@"c",nil];NSArray *copyArray=[arraycopy];NSMutableArray *mCopyArray=[arraymutableCopy];NSLog(@"%p",array);NSLog(@"%p",copyArray);NSLog(@"%p",mCopyArray);|


```
```
|123|2016-03-1017:54:39.114Homework[21782:2358605]0x170058e402016-03-1017:54:39.115Homework[21782:2358605]0x170058ed02016-03-1017:54:39.115Homework[21782:2358605]0x170058ea0|
查看内存，如我们所料，`copyArray`、`mCopyArray`和`array`的内存地址都不一样，说明`copyArray`、`mCopyArray`都对`array`进行了内容拷贝。
# 2、自定义对象的复制
使用`copy`和`mutableCopy`复制对象的副本使用起来确实方便，那么我们自定义的类是否可调用`copy`与`mutableCopy`方法来复制副本呢？我们先定义一个`Person`类，代码如下:

``
|12345|@interfacePerson:NSObject@property(nonatomic,assign)NSIntegerage;@property(nonatomic,copy)NSString *name;@end|
然后尝试调用`Person`的`copy`方法来复制一个副本:

```
```
|1234|Person *person1=[[Person alloc]init];//创建一个Person对象person1.age=20;person1.name=@"张三";Person *person2=[person1 copy];//复制副本|
运行程序，将会发生崩溃，并输出以下错误信息:

``
|1|[PersoncopyWithZone:]:unrecognizedselectorsenttoinstance0x608000030920|
上面的提示：`Person`找不到`copyWithZone:`方法。我们将复制副本的代码换成如下:

```
```
|1|Person *person2=[person1 mutableCopy];//复制副本|
再次运行程序，程序同样崩溃了，并输出去以下错误信息:

``
|1|[PersonmutableCopyWithZone:]:unrecognizedselectorsenttoinstance0x600000221120|
上面的提示：`Person`找不到`mutableCopyWithZone:`方法。
大家可能会觉得疑惑，程序只是调用了`copy`和`mutableCopy`方法，为什么会提示找不到`copyWithZone:`与`mutableCopyWithZone:`方法呢？其实当程序调用对象的`copy`方法来复制自身时，底层需要调用`copyWithZone:`方法来完成实际的复制工作，`copy`返回实际上就是`copyWithZone:`方法的返回值；`mutableCopy`与`mutableCopyWithZone:`方法也是同样的道理。
那么怎么做才能让自定义的对象进行`copy`与`mutableCopy`呢？需要做以下事情:

```
```
|12|1.让类实现NSCopying/NSMutableCopying协议。2.让类实现copyWithZone:/mutableCopyWithZone:方法|
所以让我们的`Person`类能够复制自身，我们需要让`Person`实现`NSCopying`协议；然后实现`copyWithZone:`方法:

``
|12345|@interfacePerson:NSObject@property(nonatomic,assign)NSIntegerage;@property(nonatomic,copy)NSString *name;@end|


```
```
|1234567891011|#import "Person.h"@implementation Person-(id)copyWithZone:(NSZone *)zone{Person *person=[[[selfclass]allocWithZone:zone]init];person.age=self.age;person.name=self.name;returnperson;}@end|
运行之后发现我们实现了对象的复制:
![122030896-af5a229f764d9214](http://jbcdn2.b0.upaiyun.com/2017/08/2df6b677d4838c5e17772da1ca76cfee.png)
同时需要注意的是如果对象中有其他指针类型的实例变量，且只是简单的赋值操作:`person.obj2 = self.obj2`，其中`obj2`是另一个自定义类，如果我们修改`obj2`中的属性，我们会发现复制后的`person`对象中`obj2`对象中的属性值也变了，因为对于这个对象并没有进行`copy`操作，这样的复制操作不是完全的复制，如果要实现完全的复制，需要将`obj2`对应的类也要实现copy,然后这样赋值:`person.obj2 = [self.obj2 copy]`。如果对象很多或者层级很多，实现起来还是很麻烦的。如果需要实现完全复制同样还有另有一种方法，那就是归档:

``
|1|Person *person2=[NSKeyedUnarchiverunarchiveObjectWithData:[NSKeyedArchiverarchivedDataWithRootObject:person1]];|
这样我们就实现了自定义对象的复制，需要指出的是如果重写`copyWithZone:`方法时，其父类已经实现`NSCopying`协议，并重写过了`copyWithZone:`方法，那么子类重写`copyWithZone:`方法应先调用父类的`copy`方法复制从父类继承得到的成员变量，然后对子类中定义的成员变量进行赋值:

```
```
|123456|-(id)copyWithZone:(NSZone *)zone{id obj=[supercopyWithZone:zone];//对子类定义的成员变量赋值...returnobj;}|
关于`mutableCopy`的实现与`copy`的实现类似，只是实现的是`NSMutableCopying`协议与`mutableCopyWithZone:`方法。对于自定义的对象，在我看来并没有什么可变不可变的概念，因此实现`mutableCopy`其实是没有什么意义的，在此就不详细介绍了。
# 3、定义属性的copy指示符
如下段代码，我们在定义属性的时候使用了`copy`指示符:

``
|1234|#import@interfacePerson:NSObject@property(nonatomic,copy)NSMutableString *name;@end|
使用如下代码来进行测试:

```
```
|123|Person *person1=[[Person alloc]init];//创建一个Person对象person1.name=[NSMutableString stringWithString:@"苏小妖"];[person1.name appendString:@"123"];|
运行程序会崩溃，并且提示以下信息:

``
|1|***Terminatingappduetouncaughtexception'NSInvalidArgumentException',reason:'Attempt to mutate immutable object with appendString:'|
这段错误提示不允许修改`person`的`name`属性，这是因为程序定义`name`属性时使用了`copy`指示符，该指示符置顶调用`setName:`方法时(通过点语法赋值时，实际上是调用对应的`setter`方法)，程序实际上会使用参数的副本对`name`实际变量复制。也就是说，`setName:`方法的代码如下:

```
```
|123|-(void)setName:(NSMutableString *)name{_name=[namecopy];}|
`copy`方法默认是复制该对象的不可变副本，虽然程序传入的`NSMutableString`，但程序调用该参数的`copy`方法得到的是不可变副本。因此，程序赋给`Person`对象的name实例变量的值依然是不可变字符串。
注意:定义合成`getter`、`setter`方法时并没有提供`mutableCopy`指示符。因此即使定义实例变量时使用了可变类型，但只要使用`copy`指示符，实例变量实际得到的值总是不可变对象。
参考文章：
[iOS之对象复制](http://www.jianshu.com/p/ac07c26f467d)
[iOS的深复制与浅复制](http://ibloodline.com/articles/2016/03/10/memory3-copy.html)
