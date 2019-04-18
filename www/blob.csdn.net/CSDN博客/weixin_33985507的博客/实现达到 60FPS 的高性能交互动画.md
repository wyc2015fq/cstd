# 实现达到 60FPS 的高性能交互动画 - weixin_33985507的博客 - CSDN博客
2017年11月05日 17:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
原文链接 [http://web.jobbole.com/92871/](https://link.jianshu.com?t=http://web.jobbole.com/92871/)
#### 浏览器 101：像素是怎么来的
在深入研究之前，我们要先搞清楚一个很重要的问题：浏览器是怎么把代码转化成为用户可见的像素点呢？
首次加载时，浏览器会下载并解析 HTML，将 HTML 元素转变为一个 DOM 节点的「内容树」(content tree)。除此之外，样式同样会被解析生成「渲染树」 (render tree)。为了提升性能，渲染引擎会分开完成这些工作，**甚至会出现渲染树比 DOM 树更快生成出来。**
#### 布局
渲染树生成后，浏览器会从页面左上角开始迭代地计算出每个元素尺寸和位置，最终生成布局。这个过程可能是一气呵成的，但**也可能由于元素的排列导致反复地绘制**。元素间的位置关系都紧密相关。为了优化必要的任务，浏览器会追踪元素的变化情况，并将这些元素以及它们的子节点标记为 ‘dirty’（脏元素）。但是元素间耦合紧密，**任何布局上的改变代价都是重大的，应该尽量避免**。
#### **绘制**
生成布局后，浏览器将页面绘制到屏幕上。这个环节和「布局」步骤类似，浏览器会追踪脏元素，将它们合并到一个超大的矩形区域中。每一帧内只会发生一次重绘，用于绘制这个被污染区域。**重绘也会消耗大量性能，能免则免**。
#### 复合
最后一步，将所有绘制好的元素进行复合。默认情况下，所有元素将会被绘制到同一个层中；如果将元素分开到不同的复合层中，更新元素对性能友好，**不在同一层的元素不容易受到影响**。CPU 绘制层，GPU 生成层。基础绘图操作在硬件加速合成中完成效率高。层的分离允许非破坏性的改变，正如你所猜测的，**GPU 复合层上的改变代价最小性能消耗最少**。
#### 激发创造力
一般情况下，更改复合层是相对消耗性能较少的一个操作，所以尽量通过改变[opacity](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/CSS/opacity)和[transform](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/CSS/transform)的值触发复合层绘制。看起来好像…我们能做出的效果会很有限，但真的是这样吗？要好好开发自己的创造力哦。
#### 变换
「变换」为元素提供了无限的可能性：位置可以改变 (translateX,translateY, 或translate3d)、大小也可以通过缩放 (scale) 改变、还能旋转、斜切甚至 3D 变换。就是在某些场景下，开发者需要换一种思考方式，通过使用变换减少重排和重绘。 比如给一个元素添加 active 类名后它会向左移动 10px，可以通过改变 left 属性：
![8805811-65bb10d8c9fd1768.png](https://upload-images.jianshu.io/upload_images/8805811-65bb10d8c9fd1768.png)
#### 透明度
可以通过改变opacity的值，实现元素的显示和隐藏（与改变display或者visibility的值达到类似的效果类似，但性能更好）。比如实现菜单的切换效果：菜单展开时，opacity值为1；收起时，opacity值变为 0。要注意的是pointer-events的值也要随之改变，防止用户操作到明明收起的菜单。closed 类名会根据用户点击 ‘open’ 时，closed 类名会被加上；点击 ‘close’ 按钮时，closed 类名会被移除。对应的代码是这样的：
![8805811-7e1831d8ab8ca3d3.png](https://upload-images.jianshu.io/upload_images/8805811-7e1831d8ab8ca3d3.png)
另外，透明度可变意味着开发者可以控制元素的可见程度。多多思考应用透明度的场景 — 比如直接给元素的阴影 (box-shadow) 做动效很可能会造成严重的性能问题：
![8805811-c657b53282fa3a8a.png](https://upload-images.jianshu.io/upload_images/8805811-c657b53282fa3a8a.png)
如果把阴影放到伪元素上，控制伪元素的透明度从而控制阴影，效果一样但性能更好，代码如下：
![8805811-47b3a039109593c1.png](https://upload-images.jianshu.io/upload_images/8805811-47b3a039109593c1.png)
#### 手动优化
还有一个好消息 — 开发者可以选择想要控制的属性，创建复合层，并将元素拖到该层。通过手动优化，确保元素总能被绘制好，这也是通知浏览器准备绘制该元素的最简单方式。需要独立层的场景包括：元素的状态将发生一些变化（比如动画）、改变了很消耗性能的样式（比如position:fixed和overflow:scroll）。可能你也见过了糟糕的性能导致了页面闪烁、震动…或其他不如预期的效果，例如移动端常见的固定在视口顶部的头部，会在页面滚动的时候闪烁。将这**样的元素独立到自己的复合层**，就是常见的解决这类问题的方法。
#### hack 方法
从前，开发者通常是通过backface-visibility:hidden或者trasform: translate3d(0,0,0)触发浏览器生成新的复合层，但这并不是标准的写法，这两种写法也对元素的视觉效果不起作用。
#### 新方法
现在有了[will-change](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/CSS/will-change)，它能够显式地通知浏览器对某一个元素的某个或某些元素做渲染优化。will-change接收各种各样的属性值，比如一个或多个 CSS 属性 (transform,opacity)、contents或者scroll-position。不过最常用值可能就是auto，这个值表示的是浏览器将进行默认的优化：
![8805811-8e9c44af0eff0564.png](https://upload-images.jianshu.io/upload_images/8805811-8e9c44af0eff0564.png)
优化有度，我们总能听到关于「复合层过多反而阻碍渲染」的讨论。因为浏览器已经为优化做了能做的一切，will-change的性能优化方案本身对资源要求很高。如果浏览器持续在执行某个元素的will-change，就意味着浏览器要持续对这个元素的进行优化，性能消耗造成页面卡顿。过多的复合层降低页面性能的现象在移动端很常见。
#### **动画方法**
想要元素动起来可以用 CSS（声明式），也可以使用 JavaScript（命令式），按需选择。
#### 声明式动画
CSS 动画是声明式的（告诉浏览器要做什么），浏览器需要知道动画的起始状态和终止状态，这样它才知道如何优化。CSS 动画不是在主线程中执行，不会妨碍主线程中的任务执行。总的来说，**CSS 动画对性能更友好**。关键帧的动画组合提供了相当丰富的视觉效果，比如下面是一个元素的无限旋转动画：
![8805811-c0f2e1d532607d7b.png](https://upload-images.jianshu.io/upload_images/8805811-c0f2e1d532607d7b.png)
但 CSS 动画缺乏 JS 的表达能力，将两者结合起来效果更好：比如用 JS 监听用户输入，根据动作切换类名。类名对应着不同的动画效果。下面的代码实现的是当元素被点击时切换类名：
![8805811-da969029e45143cb.png](https://upload-images.jianshu.io/upload_images/8805811-da969029e45143cb.png)
![8805811-849545c0dd317688.png](https://upload-images.jianshu.io/upload_images/8805811-849545c0dd317688.png)
值得一提的是，如果你在操作「出血」（注：设计中在画布四边留出的一定区域称为「出血」）时，新的[Web Animation API](https://link.jianshu.com?t=https://www.w3.org/TR/web-animations-1/)会利用 CSS 的性能。通过这个 API，开发者能轻松地在性能友好的基础上处理动画的同步和时间问题。
#### 命令式动画
命令式动画告诉浏览器如何去演绎动画。CSS 动画代码在某些场景下会变得很臃肿，或者需要更多的交互控制，此时 JS 就要介入了。注意！和 CSS 动画不同，JS 动画是在主线程中执行的（也就是说丢帧的可能性大于 CSS 动画的），性能相对差一些。在使用 JS 动画的场景中，考虑范围中的性能之选比较少。
#### requestAnimationFrame
[requestAnimationFrame](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame)对性能友好，你可以将它视作setTimeout的进化版，不过这其实是一个动画执行的 API。理论上调用了这个 API 就能保证 60fps 的帧率，但实践证明**这个函数是请求在下一次可用时绘制动画**，也就是并没有固定的时间间隔。浏览器会把页面上发生的变化组合接着一次绘制，而不会为每一次变化都进行绘制，通过这个方式提升 CPU 的使用率。 RAF 可以**递归地使用**：
![8805811-c2a9e8ce8417e2e9.png](https://upload-images.jianshu.io/upload_images/8805811-c2a9e8ce8417e2e9.png)
另外，类似缩放窗口或页面滚动这样的场景，直接绑定事件是相对消耗性能的，开发者可以考虑在类似情况下用 RAF 提升性能。
#### 滚动
实现性能良好的平滑滚动可是个挑战。幸运的是，最近规范提供一些可配置选项。开发者不再需要通过禁止浏览器默认行为 (preventDefault)，开启[Passive event listeners](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/API/EventTarget/addEventListener#Improving_scrolling_performance_with_passive_listeners)即可提升滚动性能（声明之后，就不需要通过阻止元素的 touch 事件监听和鼠标滚轮事件监听以优化滚动性能）。使用方法仅是在需要的监听器中声明{passive: true}：
![8805811-6797ecf2b2317dd5.png](https://upload-images.jianshu.io/upload_images/8805811-6797ecf2b2317dd5.png)
从 Chrome 56 开始，这个选项将在touchmove和touchstart中默认开启。
新出的[Intersection Observer API](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/API/Intersection_Observer_API)能够告诉开发者某个元素是不是在视口内，或者是不是和其他元素有交互。和通过事件处理这种会阻塞主线程的交互方式相比，Intersection Observer API 可以监听元素，只有当元素交叉路径的时候才会执行相应操作。这个 API 在无限滚动和懒加载的场景都可以使用。
#### 先读后写
不断地读写 DOM 会导致「强制同步布局」(forced synchronous layouts)，不过在技术发展过程中它演变成了更形象的词 — 「布局抖动」(layout thrashing)。前文也有提到，浏览器会追踪「脏元素」，在合适的时候将变换过程储存起来。在读取了特定属性以后，开发者可以强制浏览器提前计算。这样反复的读写会导致重排。幸运的是有一个简单的解决方式：读完再写。
为了模拟上述效果，请看下面这个对读写有严苛要求的例子：
![8805811-9cec612e0d11a9a0.png](https://upload-images.jianshu.io/upload_images/8805811-9cec612e0d11a9a0.png)
将「读」放到forEach外面，而不是和「写」一起在每个迭代里都执行，就能提高性能：
![8805811-f46950d8165ad9eb.png](https://upload-images.jianshu.io/upload_images/8805811-f46950d8165ad9eb.png)
#### **优化的未来**
浏览器在性能优化方面持续投入了越来越多的精力。通过新属性[contain](https://link.jianshu.com?t=https://developer.mozilla.org/en-US/docs/Web/CSS/contain)可以声明一个元素的子树独立于页面的其他元素（目前只有 Chrome 和 Opera 支持该属性）。这就等于告诉了浏览器「这个元素是安全的，它不会影响到其他元素」。contain的属性值根据变化的范围确定，可以是strict、content、size、layout、style或者paint。这确保了子树被更新的时候，不会造成父元素的重排。特别是在引入第三方控件的时候：
![8805811-96c514bbb05295ae.png](https://upload-images.jianshu.io/upload_images/8805811-96c514bbb05295ae.png)
#### 性能测试
知道了如何优化页面性能后，还要做性能测试才行。依我之见，Chrome 开发者工具就是最棒的测试工具。在 ‘More Tools’ 中有一个 ‘Rendering’ 面板，其中包含了一些选项：比如追踪「脏元素」、计算每秒的帧率、高亮每层的边界还有监测滚动性能问题。
![8805811-17e2c7a226c67b13.png](https://upload-images.jianshu.io/upload_images/8805811-17e2c7a226c67b13.png)
‘Performance’ 面板中的 ‘Timeline’ 工具能记录动画过程，开发者可以直接定位到出问题的部分。很简单，红色表示有问题，绿色表示渲染正常。开发者可以直接点击红色区域，看看是哪个函数造成了性能问题的函数。
另一个有趣的工具是在 ‘Caputrue Settings’ 中的 ‘CPU throtting’，开发者可以通过这个选项模拟页面运行在一台非常卡的设备上。开发者在桌面浏览器上测试页面的时候效果可能很好，那是因为 PC 或者 Mac 的本身性能就优于移动设备。这个选项提供了很好的真机模拟。
#### 测试和迭代
动画性能优化最简单的方案就是减少每一帧的工作量。最有效缓解性能压力的方法就是，尽量只更新在复合层中的元素，重新渲染复合层元素不容易影响到页面上其他元素。性能优化往往意味着反复地测试和验证，以及跳出惯性思维找到奇技淫巧实现高性能动画 — 无论怎么样，最终受益的会是用户和开发者。
