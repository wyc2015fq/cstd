# iOS开发之原来Runtime的黑魔法这么厉害 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月26日 10:31:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2166
学习iOS以来不间断的会听到Runtime，开始感觉OC的语法已经很完善了根本没有必要学习它背后的运行机制，一直以来对这块知识点置若罔闻的我，今天竟然认认真真的读了一篇Runtime的文章，然后就被它深深地吸引住了^_^
![](http://upload-images.jianshu.io/upload_images/1728983-f4ed788f5e19a693.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
runtime.png
今天在这里小菜鸟不会罗列Runtime的语法，但会通过一个简单的Runtime实例的认真讲解让大家伙领会Runtime的魅力所在。
**1、实例内容：**通过Runtime来阻止数组越界
**2、应用场景：**开发程序过程中大家多多少少都会遇到数组越界的情况
```
NSArray *array=[[NSArray alloc]initWithObjects:@"111",@"222", nil]; 
[array objectAtIndex:3];
```
遇到上述用法直接导致的后果就是我们的程序会崩掉，在开发中遇到我们还可以修改程序，但用户实际使用时崩掉就会闪退，这会使用户体验大大折扣的。
那有没有一种可能在越界的时候，如果我们处于调试过程就让它跳出错误，而当用户使用时越界就不让它崩溃呢。答案是肯定的，那接下来小菜鸟就带领大家一起领略Runtime的黑魔法。
**3、Runtime的黑魔法讲解**
首先我们要明确下需求:
- 1、调试过程中越界提示错误 
- 2、用户实际使用过程中数组越界不影响正常使用 
- 3、如果程序开发中修改，之前使用的objectAtIndex：不受影响
下面是我写的一个NSArray的类别，代码：
```
@implementation NSArray (Boundary)
+(void)load{
  static dispatch_once_t onceToken;
      dispatch_once(&onceToken, ^{
      SEL safeSel=@selector(safeObjectAtIndex:);
      SEL unsafeSel=@selector(objectAtIndex:);
      Class myClass = NSClassFromString(@"__NSArrayI");
      Method safeMethod=class_getInstanceMethod (myClass, safeSel);
      Method unsafeMethod=class_getInstanceMethod (myClass, unsafeSel);
      method_exchangeImplementations(unsafeMethod, safeMethod);
 });
}
```
```
-(id)safeObjectAtIndex:(NSUInteger)index{
    if (index>(self.count-1)) {
        NSAssert(NO, @"beyond the boundary");
        return nil;
    }
    else{
        return [self safeObjectAtIndex:index];
    }
}
@end
```
这些代码什么意思呢，小菜鸟接下来会分段讲解的。
`+(void)load`
这个方法是在创建分类的时候系统会自动调用的方法
```
static dispatch_once_t onceToken;
dispatch_once(&onceToken, ^{
});
```
这句代码我们单利中常常会用到，保证代码只执行一次就不多做解释了。
```
SEL safeSel=@selector(safeObjectAtIndex:);
 SEL unsafeSel=@selector(objectAtIndex:);
```
objc_selector结构体的详细定义没有在头文件中找到。方法的selector用于表示运行时方 法的名字。Objective-C在编译时，会依据每一个方法的名字、参数序列，生成一个唯一的整型标识(Int类型的地址)，这个标识就是SEL。
```
Method safeMethod=class_getInstanceMethod (myClass, safeSel);
 Method unsafeMethod=class_getInstanceMethod (myClass, unsafeSel);
```
使用runtime方法拿到实例中的方法
`method_exchangeImplementations(unsafeMethod, safeMethod);`
交换SEL对应的IMP实现。
总的来说我们就是将objectAtIndex：和safeObjectAtIndex:两个函数互换了，也就是说我们在应用中如果调用objectAtIndex：实际上我们调用的是safeObjectAtIndex：这个方法。
这里还要补充一点的就是在safeObjectAtIndex：中我们写了这么一句return [self safeObjectAtIndex:index];是什么意思呢，难道不会循环调用？当然不会！
当我们外部调用objectAtIndex：时才会调用safeObjectAtIndex：，而return中调用safeObjectAtIndex：实际上调用的就是objectAtIndex：。哈哈感觉有点饶了，其实就是两个函数已经通过Runtime互换了。
`NSAssert(NO, @"beyond the boundary");`
在方法-(id)safeObjectAtIndex:(NSUInteger)index中用到的这个就是OC中的断言，简单讲就是当程序处于debug模式时会给我们跑出错误信息，而处于Release模式时也就是用户使用程序时不会抛出信息而导致闪退。
到这里我们就完成了防止数组越界闪退，内容很简单，有什么不懂得地方小伙伴们可以留言询问。
后续还会更新更多Runtime的实例，喜欢就点喜欢吧^_^

文／逆袭的小菜鸟（简书作者）
原文链接：http://www.jianshu.com/p/d26536d39ab0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
