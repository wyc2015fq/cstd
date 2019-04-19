# Runtime那些事儿（消息机制） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月02日 13:56:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：312
**一、关于runtime**
之前在项目中有遇到过用runtime解决改变全局字体的问题，所以再一次感受到了runtime黑魔法的强大，趁现在有机会分享一下对runtime的一些理解。在对象调用方法是Objective-C中经常使用的功能，也就是消息的传递，而Objective-C是C的超集，所以和C不同的是，Objective-C使用的是动态绑定，也就是runtime。Objective-C的消息传递和消息机制也就不多说了，今天主要说的是动态方法，也就是函数的调用。
**二、相关的几个函数**
下面一张图详细的概括了每个函数调用的先后以及执行的前提
![571495-5cd12b17d449bab1.png](http://cc.cocimg.com/api/uploads/20160301/1456815016258663.png)
消息传递函数的调用
1.对象在收到无法解读的消息后，首先会调用所属类的
```
`+ (BOOL)resolveInstanceMethod:(SEL)sel`
```
这个方法在运行时，没有找到SEL的IML时就会执行。这个函数是给类利用class_addMethod添加函数的机会。根据文档，如果实现了添加函数代码则返回YES，未实现返回NO。举个例子，新建了一个工程，首先我在ViewController这个类中执行doSomething1这个方法，代码如下
```
`//`
`//  ViewController.m`
`//  RuntimeTest1`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年 www.igancao.com  All rights reserved.`
`//`
`#import "ViewController.h"`
`@interface ViewController ()`
`@end`
`@implementation ViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    ``[self performSelector:@selector(doSomething)];`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`@end`
```
运行结果
```
`**2015-12-24 10:35:37.726 RuntimeTest1[1877:337842] -[ViewController doSomething]: unrecognized selector sent to instance 0x7fe9f3736680**`
`**2015-12-24 10:35:37.729 RuntimeTest1[1877:337842] *** Terminating app due to uncaught exception ``'NSInvalidArgumentException'``, reason: ``'-[ViewController doSomething]: unrecognized selector sent to instance 0x7fe9f3736680'``**`
`***** First ``throw``call stack:**`
```
不出意外，程序崩溃，因为没有找到doSomething这个方法，下面我们在里面实现 + (BOOL)resolveInstanceMethod:(SEL)sel这个方法，并且判断如果SEL 是doSomething那就输出add method here
```
`//`
`//  ViewController.m`
`//  RuntimeTest1`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年 www.igancao.com All rights reserved.`
`//`
`#import "ViewController.h"`
`@interface ViewController ()`
`@end`
`@implementation ViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    ``[self performSelector:@selector(doSomething)];`
`}`
`+ (BOOL)resolveInstanceMethod:(SEL)sel {`
`    ``if``(sel == @selector(doSomething)) {`
`        ``NSLog(@``"add method here"``);`
`        ``return``YES;`
`    ``}`
`    ``return``[``super``resolveInstanceMethod:sel];`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`@end`
```
继续运行，然后看到log
```
`**2015-12-24 10:47:24.687 RuntimeTest1[2007:382077] add method here**`
`**2015-12-24 10:47:24.687 RuntimeTest1[2007:382077] -[ViewController doSomething]: unrecognized selector sent to instance 0x7f9568c331f0**`
`**2015-12-24 10:47:24.690 RuntimeTest1[2007:382077] *** Terminating app due to uncaught exception ``'NSInvalidArgumentException'``, reason: ``'-[ViewController doSomething]: unrecognized selector sent to instance 0x7f9568c331f0'``**`
`***** First ``throw``call stack:**`
```
可以看到程序依然是崩溃了，但是我们可以看到输出了add method here，这说明我们 + (BOOL)resolveInstanceMethod:(SEL)sel这个方法执行了，并进入了判断，所以，在这儿，我们可以做一下操作，使这个方法得到相应，不至于走到最后- (void)doesNotRecognizeSelector:(SEL)aSelector这个方法中而崩掉了，接下来，我么继续操作，如下
```
`//`
`//  ViewController.m`
`//  RuntimeTest1`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年 www.igancao.com All rights reserved.`
`//`
`#import "ViewController.h"`
`#import [objc/runtime.h](因识别问题，此处将尖括号改为方括号)`
`@interface ViewController ()`
`@end`
`@implementation ViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    ``[self performSelector:@selector(doSomething)];`
`}`
`+ (BOOL)resolveInstanceMethod:(SEL)sel {`
`    ``if``(sel == @selector(doSomething)) {`
`        ``NSLog(@``"add method here"``);`
`        ``class_addMethod([self class], sel, (IMP)dynamicMethodIMP, ``"v@:"``);`
`        ``return``YES;`
`    ``}`
`    ``return``[``super``resolveInstanceMethod:sel];`
`}`
`void dynamicMethodIMP (id self, SEL _cmd) {`
`    ``NSLog(@``"doSomething SEL"``);`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`@end`
```
导入了并且在+ (BOOL)resolveInstanceMethod:(SEL)sel中执行了class_addMethod这个方法，然后定义了一个void dynamicMethodIMP (id self, SEL _cmd)这个函数，运行工程，看log
```
`**2015-12-24 11:45:11.934 RuntimeTest1[2284:478571] add method here**`
`**2015-12-24 11:45:11.934 RuntimeTest1[2284:478571] doSomething SEL**`
```
这时候我们发现，程序并没有崩溃，而且还输出了doSomething SEL，这时候就说明我们已经通过runtime成功的向我们这个类中添加了一个方法。关于class_addMethod这个方法，是这样定义的
```
`OBJC_EXPORT BOOL class_addMethod(Class cls, SEL name, IMP imp,  const char *types)`
```
- 
cls   在这个类中添加方法，也就是方法所添加的类
- 
name  方法名，这个可以随便起的
- 
imp   实现这个方法的函数
- 
types 定义该数返回值类型和参数类型的字符串，这里比如"v@:"，其中v就是void，带表返回类型就是空，@代表参数，这里指的是id(self)，这里:指的是方法SEL(_cmd)，比如我再定义一个函数
```
`int newMethod (id self, SEL _cmd, NSString *str) {`
`  ``return``100;`
`}`
```
那么添加这个函数的方法就应该是ass_addMethod([self class], @selector(newMethod), (IMP)newMethod, "i@:@");
**2.如果在+ (BOOL)resolveInstanceMethod:(SEL)sel中没有找到或者添加方法**
消息继续往下传递到- (id)forwardingTargetForSelector:(SEL)aSelector看看是不是有对象可以执行这个方法，我们来重新建个工程，然后新建一个叫SecondViewController的类，里面有一个- (void)secondVCMethod方法，如下
```
`//`
`//  SecondViewController.m`
`//  RuntimeTest2`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年  www.igancao.com All rights reserved.`
`//`
`#import "SecondViewController.h"`
`@interface SecondViewController ()`
`@end`
`@implementation SecondViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    ``// Do any additional setup after loading the view.`
`}`
`- (void)secondVCMethod {`
`    ``NSLog(@``"This is secondVC method !"``);`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`/*`
`#pragma mark - Navigation`
`// In a storyboard-based application, you will often want to do a little preparation before navigation`
`- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {`
`    ``// Get the new view controller using [segue destinationViewController].`
`    ``// Pass the selected object to the new view controller.`
`}`
`*/`
`@end`
```
工程结构应该是这样的
![571495-43e75b645ee60124.png](http://cc.cocimg.com/api/uploads/20160301/1456803821827502.png)
工程目录图
现在我想在ViewController中调用- (void)secondVCMethod这个方法，我们知道ViewController和SecondViewController并无继承关系，按照正常的步骤去做程序肯定会因为在ViewController找不到- (void)secondVCMethod这个方法而直接崩溃的
```
`//`
`//  ViewController.m`
`//  RuntimeTest2`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年 www.igancao.com  All rights reserved.`
`//`
`#import "ViewController.h"`
`#import @interface ViewController ()`
`@end`
`@implementation ViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`     ``[self performSelector:@selector(secondVCMethod)];`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`@end`
```
运行结果
```
`**2015-12-24 13:54:44.314 RuntimeTest2[3164:835814] -[ViewController secondVCMethod]: unrecognized selector sent to instance 0x7fc3a8535c10**`
`**2015-12-24 13:54:44.317 RuntimeTest2[3164:835814] *** Terminating app due to uncaught exception ``'NSInvalidArgumentException'``, reason: ``'-[ViewController secondVCMethod]: unrecognized selector sent to instance 0x7fc3a8535c10'``**`
`***** First ``throw``call stack:**`
```
现在我们来处理一下这个消息，如下
```
`//`
`//  ViewController.m`
`//  RuntimeTest2`
`//`
`//  Created by HenryCheng on 15/12/24.`
`//  Copyright ?（版权符号） 2015年 www.igancao.com All rights reserved.`
`//`
`#import "ViewController.h"`
`#import @interface ViewController ()`
`@end`
`@implementation ViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`     ``[self performSelector:@selector(secondVCMethod)];`
`}`
`- (id)forwardingTargetForSelector:(SEL)aSelector {`
`    ``Class class = NSClassFromString(@``"SecondViewController"``);`
`    ``UIViewController *vc = class.``new``;`
`    ``if``(aSelector == NSSelectorFromString(@``"secondVCMethod"``)) {`
`        ``NSLog(@``"secondVC do this !"``);`
`        ``return``vc;`
`    ``}`
`    `
`    ``return``nil;`
`}`
`+ (BOOL)resolveInstanceMethod:(SEL)sel {`
`    ``return``[``super``resolveInstanceMethod:sel];`
`}`
`- (void)didReceiveMemoryWarning {`
`    ``[``super``didReceiveMemoryWarning];`
`    ``// Dispose of any resources that can be recreated.`
`}`
`@end`
```
运行结果
```
`**2015-12-24 14:00:34.168 RuntimeTest2[3284:870957] secondVC ``do``this``!**`
`**2015-12-24 14:00:34.169 RuntimeTest2[3284:870957] This is secondVC method !**`
```
我们会发现- (void)secondVCMethod这个方法执行了，程序也并没有崩溃，原因就是在这一步
```
`- (id)forwardingTargetForSelector:(SEL)aSelector {`
`    ``Class class = NSClassFromString(@``"SecondViewController"``);`
`    ``UIViewController *vc = class.``new``;`
`    ``if``(aSelector == NSSelectorFromString(@``"secondVCMethod"``)) {`
`        ``NSLog(@``"secondVC do this !"``);`
`        ``return``vc;`
`    ``}`
`    `
`    ``return``nil;`
`}`
```
在没有找到- (void)secondVCMethod这个方法的时候，消息继续传递，直到- (id)forwardingTargetForSelector:(SEL)aSelector，然后我在里面创建了一个SecondViewController的对象，并且判断如过有这个方法，就返回SecondViewController的对象。这个函数就是消息的转发，在这儿我们成功的把消息传给了SecondViewController，然后让它来执行，所以就执行了那个方法。同时，也相当于完成了一个多继承！
**三、最后一点**
当然，还有好几个函数，在上面那张图里面已经清晰的表达了，有兴趣的可以自己试试，看看消息的传递顺序到底是怎么样的。上面提到的这些知识runtime的冰山一角，runtime黑魔法的强大远不止于此，比如方法的调配（Method Swizzling）等，在项目实战中还是很有用的，后面有时间会再介绍.
**参考**
- 
[Objective-C Runtime  Reference](https://developer.apple.com/library/mac/documentation/Cocoa/Reference/ObjCRuntimeRef/index.html)
- 
[Objective-C Runtime Programming
 Guide](https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Introduction/Introduction.html)
