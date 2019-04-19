# Aspects源码解读：动态Block调用（不定参数的Block） - 月若无涯的专栏 - CSDN博客
2015年10月13日 17:53:04[月若无涯](https://me.csdn.net/u010124617)阅读数：3622
Aspects是iOS开发中比较著名的AOP开源库，体积小巧而功能强大，其使用的时候有个地方比较有意思：
```
+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                       withOptions:(AspectOptions)options
                        usingBlock:(id)block
                             error:(NSError **)error;
```
对于usingBlock这个参数，其传递的block是根据不同的hook方法而可以不同的，比如下面这三个hook调用，其usingBlock就可以带不同个数的参数：
```
[UIViewController aspect_hookSelector:@selector(viewDidAppear:) 
withOptions:AspectPositionAfter usingBlock:^(){
}];
withOptions:AspectPositionAfter usingBlock:^(id<AspectInfo> info){
}];
[UIView aspect_hookSelector:@selector(initWithFrame:) 
withOptions:AspectPositionAfter usingBlock:^(id<AspectInfo> info, CGRect frame){
}];
```
那么Aspects是如何做到这一点的呢？ 
打开其源码，一路跟着参数block走下去，我们可以看到，block参数在进入方法
```
static NSMethodSignature *aspect_blockMethodSignature(id block, NSError **error)
```
后被进行了转换处理，通过block参数生成了一个NSMethodSignature，这货相信对runtime比较熟悉的人还是知道是什么的。那么这里的原理又是什么呢？ 
我们都知道Block其实是一个对象，但是这个对象究竟是啥呢？得益于runtime的开源，我们可以知道其实他是一个结构体，而Aspects文件也定义了一个叫AspectBlockRef的结构体，源码入下：  
```
typedef struct _AspectBlock {
__unused Class isa;
AspectBlockFlags flags;
__unused int reserved;
void (__unused *invoke)(struct _AspectBlock *block, ...);
struct {
    unsigned long int reserved;
    unsigned long int size;
    // requires AspectBlockFlagsHasCopyDisposeHelpers
    void (*copy)(void *dst, const void *src);
    void (*dispose)(const void *);
    // requires AspectBlockFlagsHasSignature
    const char *signature;
    const char *layout;
    } *descriptor;
// imported variables
} *AspectBlockRef;
```
通过对比，这个是Block的结构体形式相同，可以用来解析block的内部数据。从而生成相应的NSMethodSignature。
后面的事情说起来就比较简单了，对于用于触发block的调用方法：
```
- (BOOL)invokeWithInfo:(id<AspectInfo>)info
```
其内部做了几件事：
- 通过block的NSMethodSignature生成对应的NSInvocation
- 把待替换原始方法的参数，赋值给block的invocation
- 通过invocation触发block的调用 
为了直观的解释，我将Aspects的该方法关键点注释出来：
```
- (BOOL)invokeWithInfo:(id<AspectInfo>)info {
NSInvocation *blockInvocation = [NSInvocation invocationWithMethodSignature:self.blockSignature];
NSInvocation *originalInvocation = info.originalInvocation;
NSUInteger numberOfArguments = self.blockSignature.numberOfArguments;
// 检查block中写了几个参数，如果比原方法个数还多，那么是有问题的
if (numberOfArguments > originalInvocation.methodSignature.numberOfArguments) {
    AspectLogError(@"Block has too many arguments. Not calling %@", info);
    return NO;
}
// 如果block中有参数，那么先把info放到1位置上
if (numberOfArguments > 1) {
    [blockInvocation setArgument:&info atIndex:1];
}
//根据block中的参数个数，从原方法中逐个赋值过来
void *argBuf = NULL;
for (NSUInteger idx = 2; idx < numberOfArguments; idx++) {
    const char *type = [originalInvocation.methodSignature getArgumentTypeAtIndex:idx];
    NSUInteger argSize;
    NSGetSizeAndAlignment(type, &argSize, NULL);
    if (!(argBuf = reallocf(argBuf, argSize))) {//分配内存
        AspectLogError(@"Failed to allocate memory for block invocation.");
        return NO;
    }
    //获取原方法中的参数
    [originalInvocation getArgument:argBuf atIndex:idx];
    //把参数赋值给block调用
    [blockInvocation setArgument:argBuf atIndex:idx];
}
//触发block方法调用
[blockInvocation invokeWithTarget:self.block];
if (argBuf != NULL) {
    free(argBuf);
}
return YES;
}
```
总结下来，要实现这样的可变个数参数的block调用，关键还是在于获得其NSInvocation。
最后，推荐另一篇block调用相关的文章：[《NSInvocation动态调用任意block》](http://mp.weixin.qq.com/s?__biz=MjM5NTIyNTUyMQ==&mid=208927760&idx=1&sn=30b9caecba709553e463d719668454ae#rd)，其原理上与本文解析相似。
