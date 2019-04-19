# object-c学习：@property （nonatomic,retain)中的nonatom和retain的意思 - xqhrs232的专栏 - CSDN博客
2019年03月14日 16:45:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：23
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/yanfangjin/article/details/7399763](https://blog.csdn.net/yanfangjin/article/details/7399763)
相关文章
1、[属性项目的定义（property）](http://c.gzl.name/archives/85)----[http://c.gzl.name/archives/85](http://c.gzl.name/archives/85)
2、nonatomic, retain,weak,strong用法详解----[https://blog.csdn.net/dong_007_007/article/details/7722196](https://blog.csdn.net/dong_007_007/article/details/7722196)
3、IOS编程学习：property、nonatomic, retain,weak,strong用法----[http://blog.sina.com.cn/s/blog_5da93c8f0101q34c.html](http://blog.sina.com.cn/s/blog_5da93c8f0101q34c.html)
原来简单解释过[属性定义（Property）](http://c.gzl.name/archives/85) ，并且提起了简单的retain，copy，assign的区别。那究竟是有什么区别呢？
assign就不用说了，因为基本上是为简单数据类型准备的，而不是NS对象们。
Retain vs. Copy!!
- copy ： 建立一个索引计数为1的对象，然后释放旧对象
- retain ：释放旧的对象，将旧对象的值赋予输入对象，再提高输入对象的索引计数为1
那上面的是什么该死的意思呢？
Copy其实是建立了一个相同的对象，而retain不是：
比如一个NSString 对象，地址为0×[1111](https://www.baidu.com/s?wd=1111&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd) ，内容为@”STR”
Copy 到另外一个NSString 之后，地址为0×2222 ，内容相同，新的对象retain为1 ，旧有对象没有变化
retain 到另外一个NSString 之后，地址相同（建立一个指针，指针拷贝），内容当然相同，这个对象的retain值+1
也就是说，retain 是指针拷贝，copy 是内容拷贝。哇，比想象的简单多了…
------------------------------------------------------------------------------------------------------
上面这篇文章来自:http://c.gzl.name/archives/339
常在声明一些成员变量时会看到如下声明方式：
@property （参数1，参数2） 类型 名字；
这里我们主要分析在括号中放入的参数，主要有以下三种：
setter/getter方法(assign/retain/copy)
读写属性(readwrite/readonly)
atomicity(nonatomic)
其中各参数说明如下：
assign
默认类型,setter方法直接赋值，而不进行retain操作
retain
setter方法对参数进行release旧值，再retain新值，如下代码：
- -(void) setObj:(ClassX*) value  
- {  
-          if (obj != value)  
-          {  
-                    [obj release];  
-                    obj = [value retain];  
-          }  
- } 
copy
setter方法进行Copy操作，与retain一样
nonatomic
@property （nonatomic,retain)中的nonatom和retain是什么意思
@property是一个属性访问声明，扩号内支持以下几个属性：
1，getter=getterName，setter=setterName，设置setter与getter的方法名
2，readwrite,readonly，设置可供访问级别
2，assign，setter方法直接赋值，不进行任何retain操作，为了解决原类型与环循引用问题
3，retain，setter方法对参数进行release旧值再retain新值，所有实现都是这个顺序(CC上有相关资料)
4，copy，setter方法进行Copy操作，与retain处理流程一样，先旧值release，再Copy出新的对象，retainCount为1。这是为了减少对上下文的依赖而引入的机制。
5，nonatomic，非原子性访问，不加同步，多线程并发访问会提高性能。注意，如果不加此属性，则默认是两个访问方法都为原子型事务访问。锁被加到所属对象实例级。
@synthesize xxx;  为这个心属性自动生成读写函数；
如果你不懂怎么使用他们，那么就这样 ->
- 使用assign: 对基础数据类型 （NSInteger，CGFloat）和C数据类型（int, float, double, char, 等等）
- 使用copy： 对NSString
- 使用retain： 对其他NSObject和其子类
