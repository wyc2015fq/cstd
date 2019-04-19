# I finally figured out weakSelf and strongSelf - game3108的专栏 - CSDN博客
2016年09月01日 11:42:27[game3108](https://me.csdn.net/game3108)阅读数：321
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
原文地址：https://dhoerl.wordpress.com/2013/04/23/i-finally-figured-out-weakself-and-strongself/
还有另一个链接：http://www.lantean.co/arc-weakself-caveats/ 可以一起参考
其中，最重要的一点可能就是：对象在expression evaluation时被持有。
One problem with using blocks and asynchronous dispatch is that you can get into a retain cycle – the block can retain ‘self’, sometimes in mysterious ways. For instance, if you reference an ivar directly, what appears in the code is ‘theIvar’, the compiler
 generates ‘self->theIvar’. Thus, ‘self’, as a strong variable, is retained, and the queue retains the block, and the object retains the queue.
Apple recommends first assigning ‘self’ into a weak automatic variable, then referencing that in a block (see 1). Since the block captures the variable along with its decorators (i.e. weak qualifier), there is no strong reference to ‘self’, and the object can
 get dealloced, and at that moment the weak captured variable turns to nil.
> 
__weak __typeof__(self) weakSelf = self;
dispatch_group_async(_operationsGroup, _operationsQueue, ^
{
[weakSelf doSomething];
} );
Thinking about this, it occurred to me that ‘weakSelf’ might turn to nil in the middle of ‘doSomething’, but after a few posts on the xcode list, I was given a reference to a clang document that explicitly states that any object within an expression is retained
 for the complete expression, and only released thereafter (see 2). Whew!
But how about:
> 
__weak __typeof__(self) weakSelf = self;
dispatch_group_async(_operationsGroup, _operationsQueue, ^
{
[weakSelf doSomething];
[weakSelf doSomethingElse];
} );
Well, in this case, its possible for ‘weakSelf’ to be non-nil for the first method, but not for the second. Hmmm – the above is a simple example, most real code would get much more complex with other usages of ‘weakSelf’.
Apple calls this second example ‘non-trivial’ (see 1), and does what first seems like a bizarre set of steps: first create the ‘weakSelf’ object, then assign that to a ‘strongSelf’:
> 
__weak __typeof__(self) weakSelf = self;
dispatch_group_async(_operationsGroup, _operationsQueue, ^
{
__typeof__(self) strongSelf = weakSelf;
[strongSelf doSomething];
[strongSelf doSomethingElse];
} );
or in Swift:
> 
dispatch_async(dispatch_get_main_queue()) { [weak self] in
if let strongSelf = self {
//…
}
}
// See “Resolving Strong Reference Cycles for Closures” inThe Swift Programming Language
I looked and looked at this trying to reason it out (guess I’m just slow). Finally, the light bulb lit, and I figured it out! When the block runs, it’s only captured ‘weakSelf’. At the instant the block starts up, ‘weakSelf’ is either ‘self’, or it’s nil. Your
 code (as Apple’s example does) can test to see if ‘strongSelf’ is set, and if so you can use ‘strongSelf->theIvar’ or the more normal ‘strongSelf.someProperty’ (the latter works fine with nil messaging, the former will crash if ‘strongSelf’ is nil).
If ‘weakSelf’ is equal to ‘self’, then ‘strongSelf’ retains it, and it stays retained until the block returns, when its released. It’s all or nothing.
I felt really good finally getting this, and its making my coding of blocks much easier.
NOTE:
If you’re puzzled by the use of __typeof__(self), it’s a non-standard clang macro that turns into the class of the parenthesized object, and was taken from GCC. The nice thing about using it is that you can make this line a Code Snippet (mine is called ‘WeakSelf’),
 and you don’t have to continually adjust the class type.
1) “Programming With ARC Release Notes, search for “For non-trivial cycles, however, you should use”
2) [http://clang.llvm.org/docs/AutomaticReferenceCounting.html](http://clang.llvm.org/docs/AutomaticReferenceCounting.html) :
For __weak objects, the current pointee is retained and then released at the end of the current full-expression. This must execute atomically with respect to assignments and to the final release of the pointee.
