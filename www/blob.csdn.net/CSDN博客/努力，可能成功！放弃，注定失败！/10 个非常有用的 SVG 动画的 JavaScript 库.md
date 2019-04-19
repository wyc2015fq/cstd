# 10 个非常有用的 SVG 动画的 JavaScript 库 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月05日 10:28:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：484标签：[javascript																[jquery](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/qq_30513483/article/category/6373631)
本文来自简书，原文地址:[http://www.jianshu.com/p/20bfeda0ba3b](http://www.jianshu.com/p/20bfeda0ba3b)
SVG 通常可以用作跨分辨率视频。这意味着在一块高分屏幕上不会降低图片的锐度。此外，你甚至可以让SVG动起来，通过使用一些javascript类库。下面，我们分享一些javascript类库，这些类库会帮助我们将SVG动画提高一个等级。
## 1. Vivus
[Vivus](http://maxwellito.github.io/vivus/)
Vivus 是一个能动画js类库，它能够给SVG图像显示出被画出来的过程。Vivus是没有其他类库依赖的（比如jQuery)。你仅仅需要在页面中加入这个.js文件，然后传入需要被用来动画的SVG部分就行。同时通过指定一些配置，它能够在页面加载后直接显示动画效果。
![](http://upload-images.jianshu.io/upload_images/1637794-2a7c8230ad4e7e50.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 2.Bonsai
[Bonsai](http://bonsaijs.org/)
Bonsai 是一个功能丰富的 JS 类库，你能够用它来画和 animate 动态内容在网站上。这些内容包括了 HTML5video、变化的 Canvas 和 SVG。通过 Bonsai 框架，你能画一个简单的矩形、甚至一段矩形，如果你喜欢甚至可以画一个丰富的多人卡通游戏进去。
![](http://upload-images.jianshu.io/upload_images/1637794-66401cf04159b930.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 3. Velocity
[Velocity](http://julian.com/research/velocity/)
Velocity 是一个 JS 类库，它是用来做频繁动画用的。Velocity 的 js 动画“速度”是非常快的。它比JQuery 快，甚至比 CSS 动画还要快。Velocity 的 API 和 $.fn.animate 很像，都是通过$()来操作。velocity()是另一种方法，相比 $().animate()。总而言之，你应该使用一致的animate效果，包括 fadeIn 和 fadeOut 方法（译者：Velocity 提供了 fadeIn 和 fadeOut 方法）。
![](http://upload-images.jianshu.io/upload_images/1637794-46d928c0ac9d7933.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 4. Raphael
[Raphael](http://raphaeljs.com/)
RaphaelJS 也通常是用来在网页上画SVG图和动画的。它兼容各种windows浏览器一直到IE6，因为如此，Raphael成为了市面上最受信赖的js（svg）类库。有了它，你可以制作分析图表、地图、游戏就像在厨房做饭一样。
![](http://upload-images.jianshu.io/upload_images/1637794-811e66eda2175532.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 5. Snap
[Snap](http://snapsvg.io/)
SnapSVG 是另一个知名 JS 类库，它是由 Dmitry Baranovskiy 开发的（Raphael 同样也是）。同样它也是 Adobe Web Software Group 来维护。和 Raphael 不同的是，它只提供了 ie 最新版支持。这使得 SnapSVG 在体积上小了许多（相比 Raphel）以实现相同的功能（比如 trim）和支持最新的功能。
![](http://upload-images.jianshu.io/upload_images/1637794-1f690bddf612a0a5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 6. Lazy Line Painter
[Lazy Line Painter](http://lazylinepainter.info/)
Laid back Range Painter 是一个 jQuery 插件，通常被用来作绘制图集，有点像 Vivus。通常你会吐槽的是它的也就只有这么一个特殊的功能。让我来解释下，如果你是用Illustrator 或者Inkscape制作的SVG图像，而且SVG图像没有任何颜色上的变化，仅仅是轨迹的变化，可以用它。
![](http://upload-images.jianshu.io/upload_images/1637794-44a41350258e1e86.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 7. SVG.js
[SVG.js](http://svgjs.com/)
SVG.js 是一个轻量级的操作和动画 SVG 类库。你能够操作变化方向、位置和颜色。这还没完，你甚至可以自己实现插件等一些其他功能。这个实例可以attach一些插件，比如svg.filter.js，他可以为你的图片实现 Gaussian blur, desaturase, compare, sepia 等等功能。
![](http://upload-images.jianshu.io/upload_images/1637794-6b1015b4d2686343.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 8. Walkway
[Walkway](https://github.com/ConnorAtherton/walkway)
Walkway 支持3种方式, path, line 和 用polyline来画的svg线。它提供了一个很好的例子，绘制了一个PlayStation 的集合动画。
![](http://upload-images.jianshu.io/upload_images/1637794-029c582487e96dc6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 9. Progressbar.js
[Progressbar.js](http://kimmobrunfeldt.github.io/progressbar.js/)
ProgressBar.js 是一个可爱的和易于接受的增长曲线图用来绘制卡通SVG线条。有了它，各种形状都可以用作增长曲线。它集成了一些实用的形状如Range,Circle和Block，你甚至可以自行开发一个增长图通过Illustrator或者其他的矢量图绘制工具。 ProgressBar.js 是轻量级，MIT许可的而且支持IE9+。你可以通过它修改大型柱状增长图。你还可以改变属性生成动画，比如stroke breadth, load opacity, load coloring等等。
![](http://upload-images.jianshu.io/upload_images/1637794-2b6eed432314ed46.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
