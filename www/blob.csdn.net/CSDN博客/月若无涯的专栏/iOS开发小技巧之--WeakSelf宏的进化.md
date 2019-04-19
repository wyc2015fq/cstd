# iOS开发小技巧之--WeakSelf宏的进化 - 月若无涯的专栏 - CSDN博客
置顶2015年07月03日 11:35:27[月若无涯](https://me.csdn.net/u010124617)阅读数：27006
我们都知道在防止如block的循环引用时，会使用__weak关键字做如下定义：
`__weak typeof(self) weakSelf = self;`
后来，为了方便，不用每次都要写这样一句固定代码，我们定义了宏：
`#define WeakSelf __weak typeof(self) weakSelf = self;`
之后，我们可以比较方便的在需要的地方：
```
WeakSelf;
...
[weakSelf doSomething];
```
再后来，我们发现不止self需要使用weak，可能有部分变量也需要weak，于是我们的宏继续进化，不仅仅只支持self：
`#define WeakObj(o) __weak typeof(o) o##Weak = o;`
这样，后续对需要使用weak的对象，只要写一句`WeakObj(obj)` 即可使用objWeak变量了（PS：发现没有，这里生成的变量名其实是objWeak，并不是weakObj，原因见文章末的注1）
再后来，我们发现了一些小技巧，可以让我们的这个宏看起来更原生一些，我们添加了@符号在前面：
`#define WeakObj(o) autoreleasepool{} __weak typeof(o) o##Weak = o;`
使用上看起来是这样
```
@WeakObj(self);
...
[selfWeak doSomething];
```
是不是感觉挺高大上的？ 
这里是利用了@autoreleasepool{}这个系统的关键字来实现的，其实还可以利用@try{}@finally{}这个也可以达到相同的效果，比如：
`#define WeakObj(o) try{}@finally{} __weak typeof(o) o##Weak = o;`
这部分空的@try或者空的@autoreleasepool会在编译时被优化掉，不必担心性能问题。
至此，我们的宏已经可以用了，但是实际使用中，出现了一个很尴尬的问题，就是代码自动补全，@W并不能自动提示出该宏，所以每次都是很尴尬的先利用提示，写完WeakObj(obj)，然后光标移动到前面去打上一个@符号。 
这种事情怎么能忍受？ 
还好我们还有利器，Xcode的CodeSnippet，任意方法内，写一句代码
`@WeakObj(<#obj#>);`
拖到Xcode的CodeSnippet区域，快捷键设置为@WeakObj即可。 
![这里写图片描述](https://img-blog.csdn.net/20150703110942966)
至此，愉快的使用@W即可自动补全出该宏了。。
另外，还有相应的strong宏，一并晒在这里
`#define StrongObj(o) autoreleasepool{} __strong typeof(o) o = o##Weak;`
用处嘛简单写个例子：
```
@WeakObj(self);
[var setBlock:^{
    @StrongObj(self);
    [self doSomething];
}];
```
关于这么写的原因，请先自行揣摩，之后可以看看`晓月`的这篇文章：[http://blog.csdn.net/uxyheaven/article/details/44226395](http://blog.csdn.net/uxyheaven/article/details/44226395)，也可以在评论中留言。
最后，揭晓为什么该宏生成的变量名是objWeak： 
1. 使用时，如果开发者习惯性的要打出 [self doSomething]时，当他输入self时，自动补全出来的部分能看到还有selfWeak可供选择，算是一种提醒方式。 
2. 如果weak前置，当然也可以，生成的会是weakobj这样的变量名，只需要把宏中`o##Weak` 换成`weak##o`
好了，这篇文章希望对一些人有些启发或帮助。
最后晒出个人在用的宏定义：
```
#define YRWeakObj(o) autoreleasepool{} __weak typeof(o) o##Weak = o;
#define YRStrongObj(o) autoreleasepool{} __strong typeof(o) o = o##Weak;
```
