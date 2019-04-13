
# @property的属性weak nonatomic strong等介绍（ios） - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月02日 18:06:58[Jlins](https://me.csdn.net/dyllove98)阅读数：9457


学习ios也已经快半个月了，也尝试做简单的应用程序，但是遇到很多问题，总结下来就是：急于求成，总想着做一个东西出来，里面的东西也不懂，只知道把这个复制到代码中就OK了，现在发现基础很重要，所以在此总结一下最基本的经常接触的weak nonatomic strong等功能：
@property @synthesize关键字
注意：这两个关键字是编译器特性，让xcode可以自动生成getter和setter的声明和实现。
@property是在头文件.h中声明一个变量的setter和getter方法。
|1
|2
|3
|4
|5
|6
|@property|int|age;
|//编译时遇到这一行，则自动扩展成下面两句：
|-
 (|void|)setAge:(|int|)age;
|-
 (|int|)age;
|


@synthesize是在m文件中定义setter和getter方法的实现。
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|@synthesize|age=_age;
|//相当于下面的代码：
|-
 （|void|）setAge:(|int|)age
|{
|_age=age;
|}
|-
 (|int|)age
|{
|Return
 _age;
|}
|


atomic与nonatomic
atomic的意思就是setter/getter这个函数是一个原语操作。如果有多个线程同时调用setter的话，不会出现某一个线程执行setter全部语句之前，另一个线程开始执行setter情况，相当于函数头尾加了锁一样。
nonatomic不保证setter/getter的原语行，所以你可能会取到不完整的东西。
比如setter函数里面改变两个成员变量，如果你用nonatomic的话，getter可能会取到只更改了其中一个变量时候的状态，这样取到的东西会有问题。
如果不需要多线程支持的话，当然nonatomic就够用了，另外由于不涉及锁操作，所以它执行相对快点
atomic：默认是有该属性的，这个属性是为了保证程序在多线程情况，编译器会自动生成一些互斥加锁代码，避免该变量的读写不同步问题。
nonatomic：如果该对象无需考虑多线程的情况，请加入这个属性，这样会让编译器少生成一些互斥加锁代码，可以提高效率。
readwrite与readonly
readwrite：这个属性是默认的情况，会自动为你生成存取器。
readonly：只生成getter不会有setter方法。
readwrite、readonly这两个属性的真正价值，不是提供成员变量访问接口，而是控制成员变量的访问权限。

strong与weak
strong：强引用，也是我们通常说的引用，其存亡直接决定了所指向对象的存亡。如果不存在指向一个对象的引用，并且此对象不再显示在列表中，则此对象会被从内存中释放。
weak：弱引用，不决定对象的存亡。即使一个对象被持有无数个弱引用，只要没有强引用指向它，那么还是会被清除。 指针置为nil；
strong与retain功能相似；weak与assign相似，只是当对象消失后weak会自动把指针变为nil;
assign、copy、retain
retain是指针拷贝，copy是内容拷贝。在拷贝之前，都会释放旧的对象。
assign：默认类型，setter方法直接赋值，不进行任何retain操作，不改变引用计数。一般用来处理基本数据类型。
retain：释放旧的对象（release），将旧对象的值赋给新对象，再令新对象引用计数为1。我理解为指针的拷贝，拷贝一份原来的指针，释放原来指针指向的对象的内容，再令指针指向新的对象内容。
copy：与retain处理流程一样，先对旧值release，再copy出新的对象，retainCount为1.为了减少对上下文的依赖而引入的机 制。我理解为内容的拷贝，向内存申请一块空间，把原来的对象内容赋给它，令其引用计数为1。对copy属性要特别注意：被定义有copy属性的对象必须要
 符合NSCopying协议，必须实现- (id)copyWithZone:(NSZone *)zone方法。
也可以直接使用：
使用assign: 对基础数据类型 （NSInteger，CGFloat）和C数据类型（int, float, double, char, 等等）
使用copy： 对NSString
使用retain： 对其他NSObject和其子类
如果这个对象是你alloc或者new出来的，你就需要调用release。如果使用autorelease，那么仅在发生过retain的时候release一次（让retain count始终为1）。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)

