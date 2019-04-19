# alloc、init你弄懂50%了吗？ - game3108的专栏 - CSDN博客
2016年06月27日 11:26:56[game3108](https://me.csdn.net/game3108)阅读数：840
# 链接：http://ios.jobbole.com/86324/
# 前言
这是一篇我记录对alloc、init分析思考的笔记。如果读者想看懂我的第二个思考，可能需要您至少了解内存的分段分页管理，如果您对其一点都不知道，可以先看这篇软文简单了解一下。另外很重要的一点是，请先思考。
# 思考1.对象为什么要alloc，init又是干嘛的？
很多人都知道，初始化一个对象应该这么写:

Objective-C
```
```
|1|MyClass*myObj=[MyClassalloc] init];|
那么有没有思考过为什么呢？其实我这么写也是完全可以的：

Objective-C
``
|12|MyClass*myObj=[MyClassalloc];myObj=[myObjinit];|
我们来看看这干了啥。
> 
alloc allocates a chunk of memory to hold the object, and returns the pointer.
就是说alloc分配了一坨 内存给对象，让它不释放，并且把地址返回给指针。

Objective-C
```
```
|1|MyClass*myObj=[MyClassalloc];|
那么这样过后myobj为什么不能被使用呢？这是因为这片内存还没有被正确的初始化。
> 
举个栗子，万达要修房子，他们第一步一定是要先向政府搞到一块地，第二步才能在这块地上动工修楼。
这里操作系统就是政府，alloc就是去争地，init就是在地上修房子。没有调用init，房子都没有修好，别人怎么买房进去住？所以我们需要用init来初始化这片内存：

Objective-C
``
|1234567|-init{self=[superinit];// 1.if(self){// 2.....}returnself;// 3.}|
第一步需要初始化父类的信息，比如实例变量等等。可以理解成王思聪在修房子前要询问他老爸的意见，他老爸说想娱乐会所，他没有意见的话就会修成娱乐会所，他如果有意见，就可以悄悄的在第二步里面改为修成LOL俱乐部。第三步就不说了。
最后提醒一下，不要这样写:

Objective-C
```
```
|12|MyClass*myObj=[MyClassalloc];myObj=[myObj init];|
因为你可能会忘记在第二行加init，并且代码也会增长。
# 思考2.关于alloc的思考
在思考1中我们说了：alloc分配了一坨 内存给对象，让它不释放，并且把地址返回给指针。这里主要有两个问题：
- 调用alloc后内存是直接映射到堆还是只分配给了虚拟内存？
- 这一坨内存到底是多大？
我们依次来展开。
可能有些读者不明白第一个问题是什么意思，这里需要额外讲一些关于内存的东西，其实这是iOS开发很重要的东西，不管是面试还是学习都可能会用到。
## 额外的东西
iOS里的内存是有分类的，它分成Clean Memory和Dirty Memory。顾名思义，Clean Memory是可以被操作系统回收的，Dirty Memory是不可被操作系统回收的。
- Clean Memory:在闪存中有备份，能再次读取重建。如：- Code(代码段)，framework，memory-mapped files
- Dirty Memory:所有非Clean Memory，如：- 被分配了的堆空间，image cache
举个栗子，在这样的代码中：

Objective-C
``
|1234567891011|-(void)dirtyOrCleanMemory{NSString*str1=[NSString stringWithString:@"Welcome!"];// 1.NSString*str2=@"Welcome";// 2.char*buf=malloc(100*1024*1024);// 3.分配100M内存给buffor(inti=0;i<3*1024*1024;++i){buf[i]=rand();}// 4.buf的前3M内存被赋值}|
对每行分析：
1.Dirty Memory。
因为stringWithString:是在堆上分配内存的，如果我们不回收它的话，系统会一直占用这块内存。
2.Clean Memory。
因为用这样的方法创建的是一个常量字符串，常量字符串是放在只读数据段的，如果这块内存被释放了，而我们又访问它的时候，操作系统可以在只读数据段中把值再读取出来重建这块内存。(ps:所以用这种方法创建的string是没有引用计数的。)
接下来的知识就是引出思考问题1、2比较重要的点了：
3.Clean Memory。
这个时候buf指向的100M内存区域是Clean Memory的，因为操作系统是很懒的，只有当我们要用到这块区域的时候才会映射到物理内存，没有使用的时候只会分配一块虚拟内存给buf。读起来很绕口，上张图：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664805905977.jpg)
可以看到虚拟内存和物理内存没有映射关系，所以是Clean Memory的。
4.Dirty & Clean Memory混合。
前3M是Dirty Memory，后97M是Clean Memory。这句for语句执行完成后，buf的前3M内存被赋值，也就是buf的前3M被使用了，所以这个时候的映射关系是这样的：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664810118315.jpg)
额外的东西Done.
## 回到主线
- 调用alloc后内存是直接映射到堆(物理内存)还是只分配给了虚拟内存？
- 一坨内存的一坨是多大？
这个时候我们的第一个问题读者应该能明白了。那么我们怎么验证alloc是直接映射到堆上还是只分配给虚拟内存呢？这个问题让我想了好些天，最后xo哥想到了一剂良药来验证，那就是用instrument来推反。
## 使用instrument来证反
我们假设的论点是：对象收到alloc消息后只在虚拟内存分配空间。
这里需要一丁点代码。
1.我们随便新建个工程。
2.然后做个model类：

Objective-C
```
```
|123456789|#import <Foundation/Foundation.h>@interface XOModel : NSObject{inta1;NSString*a2;}@end|
3.在controller里给view加一个点击事件：

Objective-C
``
|123456|-(void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event{for(inti=0;i<100000;++i){XOModel*model=[XOModelalloc];// 注意这句只有alloc[self.array addObject:model];}}|
4.打开instrument的alloction，运行触发一下点击事件，查看如下：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664828087961.jpg)
(图解：Persistent bytes表示所有的这类东西在堆里的大小，Persistent表示所有的这类东西的个数.)
我们发现发现在Persistent bytes(堆)里实实在在地分配给了XOModel 3.05MB的空间。
我们再触发一下点击事件：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664830375943.jpg)
发现堆分配给了XOModel的大小空间变成了原来的两倍6.10MB。
结论过渡：如果对象收到alloc消息只在虚拟内存分配空间，那么persistent bytes(堆)里是不会分配给XOModel大小的，也就是说这里的persistent bytes大小应该是0。所以问题1的结论如下：
结论：alloc不只分配在虚拟内存，同时会在物理内存建立映射。
## 对象的内存分配
最后剩下我们的最后一个问题：类对象收到alloc消息后，操作系统会分配出来的一坨内存是多大？
3.05M的大小是100000个XOModel对象的总和，那么一个XOModel的实例对象，操作系统会给他分配多大的空间呢？很简单嘛，3.05M/100000就得到了，等等，难道你真准备这样去算？好吧，其实一开始我真这样想过，但是这肯定是算不出准确答案的，关键是你要去思考。
这里有两种办法，我采用的第二种办法。
### 第一种验证方法还是instrument
我们可以修改一下触发的代码，然后重新刷新instrument查看XOModel大小，具体操作同上，不重复了：

Objective-C
```
```
|123456|-(void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event{for(inti=0;i<1;++i){// 修改处XOModel*model=[XOModelalloc];// 注意这句只有alloc[self.array addObject:model];}}|
### 第二种验证办法借用runtime
我们可以借助runtime来查看一个类对象所需要的内存大小。值得一提的是我最开始用的方法是class_getInstanceSize，原型如下：

Objective-C
``
|123456789|/*** Returns the size of instances of a class.** @param cls A class object.** @return The size in bytes of instances of the class \e cls, or \c 0 if \e cls is \c Nil.*/OBJC_EXPORTsize_tclass_getInstanceSize(Classcls)__OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);|
貌似是我们需要的函数，但是我发现这个方法有bug，返回的size和instruments的值不同，后来又发现有人遇到同样的[问题](http://stackoverflow.com/questions/14016877/does-class-getinstancesize-have-a-known-bug-about-returning-incorrect-sizes)，所以[摘抄](http://stackoverflow.com/questions/761969/checking-the-size-of-an-object-in-objective-c)了另一种方法，代码如下：

Objective-C
```
```
|123456789|#import <objc/runtime.h>#import <malloc/malloc.h>...-(void)touchesBegan:(NSSet<UITouch*>*)touches withEvent:(UIEvent*)event{XOModel*model=[XOModelalloc];[self.array addObject:model];NSLog(@"Size of %<a href="http://www.jobbole.com/members/q697158886">@:</a> %zd", NSStringFromClass([XOModel class]), malloc_size((__bridge const void *) model));}|
再贴下XOModel代码：

Objective-C
``
|123456789|#import <Foundation/Foundation.h>@interface XOModel : NSObject{inta1;NSString*a2;}@end|
在iPhone 6(或其他64位)的机子上运行，输出如下：

Objective-C
```
```
|1|AllocTest[38470:2551068] SizeofXOModel:32|
“啊咧，我一个int，一个指针你分给我32个字节，操作系统你是脑子进屎了吗？”
我们再修改一下XOModel代码，不要实例变量：

Objective-C
``
|1234567|#import <Foundation/Foundation.h>@interface XOModel : NSObject{}@end|
输出如下：

Objective-C
```
```
|1|AllocTest[38630:2562602] SizeofXOModel:16|
“我靠，我什么东西都没有，操作系统你还要分给我16个字节，是不是傻？”
智慧的操作系统这样做当然是有它自己的原因滴，这里我们需要知道三个东西：
- 任何类对象都有一个isa指针，需要分配内存。
- 32位机子上指针大小为4字节，64位机子为8字节。
- 字节对齐。
第一点就不说了，不知道的话您多半也没耐心看到现在了。
第二点贴个文档图，iOS7过后部分苹果机就开始从32位操作系统转到64位了，所以部分数据类型的大小也有变化，这里我们主要关注例子中的指针：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664933866025.jpg)
现在，基于这两点对样例分析。
- 第一个样例(类对象中一个int，一个NSString指针，一个isa指针)，64位操作系统上应该是4+8+8=20，然而输出是32，不对。
- 第二个样例(类对象中只有一个isa指针)，64位操作系统上应该是8，然而输出是16，还是不对。why？
#### 字节对齐
字节对齐我了解得也不是太多，简单点讲目的就是为了提高存取效率，概念就不展开了，可以看[这个](http://baike.baidu.com/view/1523557.htm)，我这里就直接讲原理了。先贴一份苹果的[文档](https://developer.apple.com/library/mac/documentation/Performance/Conceptual/ManagingMemory/Articles/MemoryAlloc.html):
> 
When allocating any small blocks of memory, remember that the granularity for blocks allocated by the malloc library is 16 bytes. Thus, the smallest block of memory you can allocate is 16 bytes and any blocks larger than that are a multiple of 16. For example,
 if you call malloc and ask for 4 bytes, it returns a block whose size is 16 bytes; if you request 24 bytes, it returns a block whose size is 32 bytes. Because of this granularity, you should design your data structures carefully and try to make them multiples
 of 16 bytes whenever possible.
有点长，简单的意思就是：
> 
当我们分配一块内存的时候，假设需要的内存小于16个字节，操作系统会直接分配16个字节；加入需要的内存大于16个字节，操作系统会分配a*16个字节。举个栗子，如果你调用malloc并且需要4个字节，系统会给你一块16个字节的内存块；如果你调用malloc并且需要24个字节，系统会给你一块32个字节的内存块。
现在再看我们的栗子，就可以直接上图了：
第一个例子不对齐应该是20字节，对齐就是32字节。
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664984968407.jpg)
第二个例子不对其应该是8字节，对齐就是16字节：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-21-14664986780926.jpg)
ps：在32位机器上可能会有不一样的结果，因为指针大小不同，但是32位的苹果机也是16字节对齐的。
至此我们对alloc的探究就结束了。
# 结语
这次的探究算是比较彻底了，过程当中也学到了很多东西，在这个浮躁的社会，学贵在道，术次之，打好基础、保持思考才不会磨灭掉你对它最初的兴趣。
参考链接：
- [iOS内存管理及优化-腾讯庄延军](http://www.imooc.com/video/11075)
- [Checking
 the size of an object in Objective-C – Stack Overflow](http://stackoverflow.com/questions/761969/checking-the-size-of-an-object-in-objective-c)
- [Does
 class_getInstanceSize have a known bug about returning incorrect sizes? – Stack Overflow](http://stackoverflow.com/questions/14016877/does-class-getinstancesize-have-a-known-bug-about-returning-incorrect-sizes)
- [Memory
 Usage Performance Guidelines – 苹果文档](https://developer.apple.com/library/mac/documentation/Performance/Conceptual/ManagingMemory/Articles/MemoryAlloc.html)
- [字节对齐-百度百科](http://baike.baidu.com/view/1523557.htm)
## Done
