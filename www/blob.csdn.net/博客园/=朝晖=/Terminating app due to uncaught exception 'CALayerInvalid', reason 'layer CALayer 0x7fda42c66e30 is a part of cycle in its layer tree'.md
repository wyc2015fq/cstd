# Terminating app due to uncaught exception 'CALayerInvalid', reason: 'layer <CALayer: 0x7fda42c66e30> is a part of cycle in its layer tree' - =朝晖= - 博客园
# [Terminating app due to uncaught exception 'CALayerInvalid', reason: 'layer <CALayer: 0x7fda42c66e30> is a part of cycle in its layer tree'](https://www.cnblogs.com/dhcn/p/7323519.html)
       iOS App里面所有的View构成一个组件树，这个树里面如果有了闭环就会出现这个报错，最常见的你不小在某UIViewController里面写了这样的代码：
```swift;gutter
someView.addSubView(view)
```
　　因为ViewController默认有一个view作为VC的全局父view，你这样代码就会导致整个view树出现指向闭环。于是出现了 a part of cycyle in Its layer 这样的错误。

