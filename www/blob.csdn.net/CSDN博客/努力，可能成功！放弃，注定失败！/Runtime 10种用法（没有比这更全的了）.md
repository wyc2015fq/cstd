# Runtime 10种用法（没有比这更全的了） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月04日 10:28:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2713
#### 阅读了多篇运行时的文章,感觉都很不错,从几篇文章里面提取一些个人认为比较重要的,偏实战的知识点的摘录,另外还结合了个人的创造形成此文。再牛逼的技术和实战结合才有意义，本篇介绍技术尽量和实战联系起来，有些没讲明白的，我给的链接里面都有，所以我就不赘述了，如果觉得可以就点个赞吧，哈哈。不需要打赏。
#### 1)替换系统方法,
#### 2)字典转model,
#### 3)归档,
#### 4)万能控制器跳转 
#### 四个偏实战的方法从不同的文章中聚合到这里.没讲明白的麻烦看客去我给的链接里面看看，他们都讲了
对文章的内容我用一张图介绍吧(图只管明了,我喜欢,大家也喜欢)
# runtime 文章收集:
#### [runtime 文章专题](http://www.jianshu.com/collection/dc947eab6af3)   (13+关于runtime的)
### 如下4篇内容有相似的 小白都可以看懂runtime,值得细看
文①  [OC最实用的runtime总结，面试、工作你看我就足够了！](http://www.jianshu.com/p/ab966e8a82e2)
文② [让你快速上手Runtime](http://www.jianshu.com/p/e071206103a4)
文③  [runtime详解](http://www.jianshu.com/p/46dd81402f63)
文④ [详解runtime运行时机制](http://www.jianshu.com/p/1e06bfee99d0)
文⑤ [万能控制器跳转](http://www.cocoachina.com/ios/20150824/13104.html)
**先用一张图对文章做一个介绍吧 (图只管,易懂方便回忆,我喜欢,大家也喜欢) **
![](http://upload-images.jianshu.io/upload_images/1194882-2c8707760366083b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**what(runtime 是什么)**
Runtime基本是用C和汇编写的，可见苹果为了动态系统的高效而作出的努力。你可以在[这里](http://opensource.apple.com//source/objc4/)下到苹果维护的开源代码。苹果和GNU各自维护一个开源的runtime版本，这两个版本之间都在努力的保持一致。Objective-C 从三种不同的层级上与 Runtime 系统进行交互，**分别是通过 Objective-C 源代码**，**通过 Foundation 框架的NSObject类定义的方法**，**通过对 runtime 函数的直接调用**。**大部分情况下你就只管写你的Objc代码就行，runtime 系统自动在幕后辛勤劳作着。**
RunTime简称运行时,**就是系统在运行的时候的一些机制，其中最主要的是消息机制**。
对于C语言，函数的调用在**编译的时候会决定调用哪个函数，编译完成之后直接顺序执行**，无任何二义性。
OC的函数**调用成为消息发送。属于动态调用过程。在编译的时候并不能决定真正调用哪个函数（事实证明，在编 译阶段，OC可以调用任何函数，即使这个函数并未实现，只要申明过就不会报错。而C语言在编译阶段就会报错）**。
只有在真正运行的时候才会根据函数的名称找 到对应的函数来调用。
#### **where(哪些地方使用runtime)**
1.将某些OC代码转为运行时代码，探究底层，比如block的实现原理（上边已讲到）；
2.**拦截系统自带的方法调用（Swizzle 黑魔法）,也可以说成对系统的方法进行替换**，比如拦截imageNamed:、viewDidLoad、alloc；[文／滕先洪（简书作者）
  原文链接：http://www.jianshu.com/p/ab966e8a82e2]()
需求：比如iOS6 升级 iOS7 后需要版本适配，根据不同系统使用不同样式图片（拟物化和扁平化），如何通过不去手动一个个修改每个UIImage的imageNamed：方法就可以实现为该方法中加入版本判断语句？
步骤：
a、为UIImage建一个分类（UIImage+Category）
b、在分类中实现一个自定义方法，方法中写要在系统方法中加入的语句，比如版本判断[参考 ]
> 
+ (UIImage*)xh_imageNamed:(NSString*)name {
doubleversion = [[UIDevicecurrentDevice].systemVersiondoubleValue];
if(version >=7.0) {
// 如果系统版本是7.0以上，使用另外一套文件名结尾是‘_os7’的扁平化图片
name = [name stringByAppendingString:@"_os7"];    }
return[UIImagexh_imageNamed:name];
}
c、分类中重写UIImage的load方法，实现方法的交换（只要能让其执行一次方法交换语句，load再合适不过了）
> 
+ (void)load {
// 获取两个类的类方法
Method  m1 = class_getClassMethod([UIImageclass],@selector(imageNamed:));  
Method m2 = class_getClassMethod([UIImageclass],@selector(xh_imageNamed:));
// 开始交换方法实现
method_exchangeImplementations(m1, m2);
}
注意：自定义方法中最后一定要再调用一下系统的方法，让其有加载图片的功能，但是由于方法交换，系统的方法名已经变成了我们自定义的方法名（有点绕，就是用我们的名字能调用系统的方法，用系统的名字能调用我们的方法），这就实现了系统方法的拦截！
利用以上思路，我们还可以给 NSObject 添加分类，统计创建了多少个对象，给控制器添加分类，统计有创建了多少个控制器，特别是公司需求总变的时候，在一些原有控件或模块上添加一个功能，建议使用该方法！
3.实现分类也可以增加属性；
4.实现NSCoding的自动归档和自动解档；(不用对每个属性edcode和decode了,如果几十个属性一个个的encode和decode真的很麻烦啊,使用运行时可以遍历出每个对象的属性,数组的方式遍历eccode,decode)
**用运行时的归档方法**
![](http://upload-images.jianshu.io/upload_images/1194882-8408feeea0b71f1e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1194882-dca23570316a63ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
不用运行时的归档方法:(还好只有5个属性,如果20个,30个或者后台突然增加了属性,**这么直接写死估计代码就不灵了**)
![](http://upload-images.jianshu.io/upload_images/1194882-9c31409f247709c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
5.实现字典和模型的自动转换(核心就是可以遍历出字典中的每个属性,json解析中大牛框架都用了这个特性,包括MJEXtension,YYModel，jsonModel都是将json转换为字典,再遍历字典中的每个属性来进行modle的转换)。
MJextension 使用运行时转换的json转model的部分代码摘录
![](http://upload-images.jianshu.io/upload_images/1194882-68574138229442dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
YYModel  json转model 核心代码 摘录
![](http://upload-images.jianshu.io/upload_images/1194882-9b0036ba59898803.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
JsonModel json字典转model 摘录
![](http://upload-images.jianshu.io/upload_images/1194882-893286a570409ba0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
获取属性的列表的方法是字典转模型的比较核心的方法,
> 
OBJC_EXPORT Ivar *class_copyIvarList(Class cls, unsigned int *outCount)
__OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);
致此可以下个结论吗?
#### **基本上主流的json 转model 都少不了,使用运行时动态获取属性的属性名的方法,来进行字典转模型替换,字典转模型效率最高的(耗时最短的)的是KVC,其他的字典转模型是在KVC 的key 和Value 做处理,动态的获取json 中的key 和value ,当然转换的过程中,第三方框架需要做一些判空啊,镶嵌的逻辑处理, 再进行KVC 转模型.这句代码 [xx  setValue:value forKey:key];无论JsonModle,YYKIt,MJextension 都少不了[xx  setValue:value forKey:key];这句代码的,不信可以去搜,这是字典转模型的核心方法,**
6)动态增加方法  (动态的为某个类或对象增加一个方法,摘录文章中有详细介绍)
7)动态变量控制  (动态对某个对象的变量的值进行替换,摘录文章有详细介绍)
8)实现万能控制器跳转
产品来一变态需求,推送过来的消息,要跳转到任意控制器.利用runtime动态生成对象、属性、方法这特性，我们可以先跟服务端商量好，定义跳转规则，比如要跳转到A控制器，需要传属性id、type，那么服务端返回字典给我，里面有控制器名，两个属性名跟属性值，客户端就可以根据控制器名生成对象，再用kvc给对象赋值，这样就搞定了
**9)插件开发 **
> 
插件入门
XCode 有个很坑爹的地方，就是它并不官方支持插件开发，官方没有文档，XCode 也没有开源，但由于 XCode 是 Objective-C 写的，OC 动态性太强大，导致在这么封闭的情况下民间还是可以做出各种插件，其核心开发方式就是：
dump 出 Xcode 所有头文件，知道 Xcode 里有哪些类和接口。
通过头文件方法名猜测方法的作用，swizzle 这些方法，插入自己的代码实现插件逻辑。
通过 NSNotificationCenter 监听各种事件的发生。
更详细的开发教程网上有不少文章，有兴趣的自行搜索吧。
**10)Jspath 热更新 也是使用运行时，jspatch 基本上算是黑科技，在线修复版本bug，微信都使用了这个技术，详情百度“JSPatch”，此处省略30000字**
文／hjl_wg（简书作者）
原文链接：http://www.jianshu.com/p/3182646001d1
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
