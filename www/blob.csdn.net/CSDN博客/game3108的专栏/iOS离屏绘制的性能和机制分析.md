# iOS离屏绘制的性能和机制分析 - game3108的专栏 - CSDN博客
2016年03月30日 21:07:40[game3108](https://me.csdn.net/game3108)阅读数：607
接上一篇文章，是评论中的内容，对于了解离屏渲染非常有帮助。
链接：http://blog.inico.me/2013/05/18/ios_offscreen_analysis/#.VvvLNeZyZG0
接上一篇文章提到的，评论中有一个Apple UIKit teamer，叫Andy Matuschak的一些补充内容。
---Begin---
```
Hey, cool! I think it's great that you followed up with a post focusing on performance. Your advice as far as the button is good here, but I've got one small correction and some bonus explanation for interested readers.
```
太牛逼了！我觉得你这篇注重性能表现的文章写得很好。到目前为止你的建议对button的分析来说很到位，但是对一些意犹未尽的读者来说我这里还有一些需要补充的部分。
```
I'd like to clarify a few points about offscreen drawing as described in this post. While your list of cases which might elicit offscreen drawing is accurate, there are two grossly different mechanisms being triggered by elements of this list (each with different performance characteristics), and it's possible that a single view will require both. Those two mechanisms have very different performance considerations.
```
关于在你文章中提到的离屏绘制，我这里还有几点需要补充。虽然在你文章中提到关于离屏绘制的例子很犀利，但是我想说在这些例子里涉及到两种截然不同的机制（每一种都有不同性能表现上的特点），并且有可能单个视图两者都涉及到了。在使用的时候也需要考虑到这两种机制不同的性能。
```
In particular, a few (implementing drawRect and doing any CoreGraphics drawing, drawing with CoreText [which is just using CoreGraphics]) are indeed "offscreen drawing," but they're not what we usually mean when we say that. They're very different from the rest of the list. When you implement drawRect or draw with CoreGraphics, you're using the CPU to draw, and that drawing will happen synchronously within your application. You're just calling some function which writes bits in a bitmap buffer, basically.
```
一方面来说，一部分（通过drawRect并且使用任何CoreGraphics来实现的绘制，或使用CoreText[其实就是使用CoreGraphics]绘制）的确涉及到了“离屏绘制”，但是这有区别与我们通常说的那种离屏绘制。这一部分和以上例子中其他的都不一样。当你实现drawRect方法或者通过CoeGraphics绘制的时候，其实你是在使用CPU绘制。并且这个绘制的过程会在你的App内同步地进行。基本上你只是调用了一些向位图缓存内写入一些二进制信息的方法而已。
```
The other forms of offscreen drawing happen on your behalf in the render server (a separate process) and are performed via the GPU (not via the CPU, as suggested in the previous paragraph). When the OpenGL renderer goes to draw each layer, it may have to stop for some subhierarchies and composite them into a single buffer. You'd think the GPU would always be faster than the CPU at this sort of thing, but there are some tricky considerations here. It's expensive for the GPU to switch contexts from on-screen to off-screen drawing (it must flush its pipelines and barrier), so for simple drawing operations, the setup cost may be greater than the total cost of doing the drawing in CPU via e.g. CoreGraphics would have been. So if you're trying to deal with a complex hierarchy and are deciding whether it's better to use -[CALayer setShouldRasterize:] or to draw a hierarchy's contents via CG, the only way to know is to test and measure.
```
而另外一种形式离屏绘制是发生在绘制服务（是独立的处理过程）并且同时通过GPU执行（这里就不是像前面提到的用CPU了）。当OpengGL的绘制程序在绘制每个layer的时候，有可能因为包含多子层级关系而必须停下来把他们合成到一个单独的缓存里。你可能认为GPU应该总是比CPU牛逼一点，但是在这里我们还是需要慎重的考虑一下。因为对GPU来说，从当前屏幕（on-screen）到离屏（off-screen）上下文环境的来回切换（这个过程必须flush管线和光栅，原文是it must flush its pipelines
 and barrier，这里不知道怎么翻译比较合适，回头补补openGL的东西），代价是非常大的。因此对一些简单的绘制过程来说，这个过程有可能用CoreGraphics，全部用CPU来完成反而会比GPU做得更好。所以如果你正在尝试处理一些复杂的层级，并且在犹豫到底用-[CALayer setShouldRasterize:] 还是通过CoreGraphics来绘制层级上的所有内容，唯一的方法就是测试并且进行权衡。
```
You could certainly end up doing two off-screen passes if you draw via CG within your app and display that image in a layer which requires offscreen rendering. For instance, if you take a screenshot via -[CALayer renderInContext:] and then put that screenshot in a layer with a shadow.
```
如果在你的App里通过CoreGraphics来绘制，并且在layer上通过离屏绘制的方式来呈现image，你就可以体会到两种不同的off-screen绘制过程。举个具体的例子来就是说，你通过[CALayer renderInContext:] 得到了一张截图之后再把它放到一个layer中，并且打开shadow属性。
```
Also: the considerations for shouldRasterize are very different from masking, shadows, edge antialiasing, and group opacity. If any of the latter are triggered, there's no caching, and offscreen drawing will happen on every frame; rasterization does indeed require an offscreen drawing pass, but so long as the rasterized layer's sublayers aren't changing, that rasterization will be cached and repeated on each frame. And of course, if you're using drawRect: or drawing yourself via CG, you're probably caching locally. More on this in "Polishing Your Rotation Animations," WWDC 2012.
```
还有需要注意的是，shouldRasterize和masking（遮罩）, shadows（阴影）, edge antialiasing,（抗锯齿），group opacity属性都不一样。如果后面这些提到的属性是开启的，就不会有缓存，并且离屏绘制将会在每一帧都发生；rasterization（光栅化）的确涉及到了离屏绘制，不过如果被光栅化之后的layer的sublayers如果没有发生改变，那么刚才说的光栅化后的内容将会被缓存起来并且在之后frame刷新过程中被复用。当然了，如果你正在使用drawRect或者用CoreGraphics绘制，你有可能正在使用本地的缓存机制。更多的内容可以参照WWDC2012的
 Polishing Your Rotation Animations（不知道是不是[Polishing Your Interface Rotation Animations](http://v.youku.com/v_show/id_XNTQxNjk4NTQw.html)）的内容。
```
Speaking of caching: if you're doing a lot of this kind of drawing all over your application, you may need to implement cache-purging behavior for all these (probably large) images you're going to have sitting around on your application's heap. If you get a low memory warning, and some of these images are not actively being used, it may be best for you to get rid of those stretchable images you drew (and lazily regenerate them when needed). But that may end up just making things worse, so testing is required there too.
```
说到缓存，如果你在App里面正在使用大量的绘制工作，你可能需要为你程序的堆内存中无所事事的images（有可能是大图）实现清空缓存的工作。如果你得到了内存警告，同时这些图片又不不在使用，你最好把之前绘制的时候需要的stretchable的图片全都释放（当需要的时候再通过lazy方式生成）。但是这也有可能会使情况变得更加糟糕，所以说到底，这个也需要经过测试。
