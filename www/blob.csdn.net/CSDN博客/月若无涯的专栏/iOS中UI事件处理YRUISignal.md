# iOS中UI事件处理YRUISignal - 月若无涯的专栏 - CSDN博客
2016年09月02日 09:32:04[月若无涯](https://me.csdn.net/u010124617)阅读数：638
    遇到一些朋友还在讨论多层级view，事件抛出至VC处理的问题。 
    其实目前解决方案还是有一些的，比如采用ReactiveCocoa、使用通知等等，但是iOS中单纯的UI事件，其实我们还有更简单的选择： 
YRUISignal：[https://github.com/YueRuo/YRUISignal](https://github.com/YueRuo/YRUISignal)
    开发中经常遇到类似的情况，viewController上贴了view1，view1上贴了tableview，tableview上贴了cell，cell上贴了个view3… 
（如下）
viewController 
    —–view1 
        ——tableview 
            ——cell 
                ——view3
    如果在view3上触发了一个事件，需要在view1，或者viewController中处理，按照一般的处理方式，你需要：
- 在view3上做protocol，或者做block回调
- 把事件传给上一个界面（对view3来说就是给到cell）
- cell再重复1-2的工作，直到到达相应的界面为止。
    如果这个view的层次很深。。。。你恐怕就不愿意这么做了。。。
    还好我们还有替代方案：用通知处理，view3上发送通知，在viewController中先注册通知，再处理它。恩恩，比上面那种要简单多了，但是我们继续思考，是否一个小小的UI事件都需要使用全局的通知呢？
    这里我受Touch事件的链式响应处理以及早期BeeFramework中Signal的启发，做了这个小工具类。
    这个工具类的使用很简单。比如说上面提到的view3中触发事件，viewController中处理，只需要两步： 
1.在view3中触发事件时：
`[self sendYRUISignalForKey:@"事件唯一名称"];`
2.在viewController中实现handleYRUISignal方法
```
-(BOOL)handleYRUISignal:(YRUISignal *)signal{
    if ([signal.name isEqualToString:@"事件唯一名称"]) {//能处理这个信号
        return true;//根据需要中断响应链
    }
    return false;//不能处理（继续传递给下一个view）
}
```
    打完收工，是不是非常的简单？
    需要注意的是：这种方式由于采用的是父view链式查找，因此只有view被add到界面上才有效，换句话说，如果这个view被removeFromSubview，则不能正确的传递信号出去。
    希望这里处理方式和思想能对大家有所帮助或者启发。
