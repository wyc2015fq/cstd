# 前端性能优化-将CSS文件放在顶部

#  

# 简介

在web页面设计中，一般在HTML中不直接写样式，而是通过link标签引用一个CSS文件。在下载页面过程中，document最早开始下载，然后浏览器解析document，下载相关的css、js、图片、字体、视频等资源。css文件放置在head中和放在body底部，对css的下载时间不会有影响，但是对页面的呈现有着非常大的影响，与用户的体验密切相关。

下面我们还是来看看天猫网站上的情况：



![img](https://upload-images.jianshu.io/upload_images/3020614-f727f24aa09c1b45.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

从天猫的首页可以看出5个CSS文件全部都放在了head中。同样，不仅仅只是在天猫如此，在国内技术前沿的大型网站中，都是如此。

## CSS文件放置在顶部的原理

CSS文件放在顶部一方面是因为放置顺序决定了下载的优先级，更关键的是浏览器的渲染机制。最理想的情况，我们希望浏览器逐渐的渲染下载好的CSS，将页面逐渐的展现给用户。但是浏览器为了避免样式变化时重新渲染绘制页面元素，会阻塞内容逐步呈现，浏览器等待所有样式加载完成之后才一次性渲染呈现页面。如此，CSS文件如果放置底部，浏览器阻止内容逐步呈现，浏览器在等待最后一个css文件下载完成的过程中，就出现了“白屏”（新打开连接时为白屏，尔后先出现文字，图片，样式最后出现）。这点非常严重，因为在网速非常慢的情况下，css下载时间比较长，这样就给用户带来“白屏”的时间自然也就很长了，用户体验非常差。

## 实际测试

为了延迟我本地环境的网速，我使用了Fiddler  Web Debugger来模拟网速很慢的情况。打开Rules->Performance-Simulate Modems  Speed就是启用了网速延迟模拟，调节网速打开Rules->Customize  Rules会打开一个js脚本，修改184行和186行调节网速。详细的Fiddler使用可参考这篇博客。



![img](https://upload-images.jianshu.io/upload_images/3020614-04f1993f711d1622.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/980)





![img](https://upload-images.jianshu.io/upload_images/3020614-8d0095748bb0ef39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600)

在我的例子中，顶部一个css文件，底部非常多的js文件，最后一个css文件，如下图所示。bootstrap.css启的作用是将图片水平居中，oneui.min-1.5.css在页面中启的作用仅仅是将图片垂直居中。



![img](https://upload-images.jianshu.io/upload_images/3020614-63ba282204f53a2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

## 测试demo





![img](https://upload-images.jianshu.io/upload_images/3020614-1008c559796ee136.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

放置在底部的情况1

在9.38秒时顶部一个样式文件和图片已经加载完毕（网站一出现内容我马上截图，误差不超过0.5秒，图中也可以看出图片还在渲染ing）。





![img](https://upload-images.jianshu.io/upload_images/3020614-fe36285151b8f4db.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

放在底部的情况2

在15秒不到时，oneui.min-1.5.css已经下载完毕，但却在32秒处，图片才垂直居中（一居中我马上截图，误差不超过0.5秒）。

上面可以看出放置顶部的样式无需等待其他css，js文件下载完才进行渲染。放置底部的样式负责的页面渲染需要等待其他所有js，css下载完毕才进行。

再做两个实验，一个是全部优先放在顶部，一个是全部放在最尾部。





![img](https://upload-images.jianshu.io/upload_images/3020614-57bb70d3e950df56.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

全部放在最顶部的情况

全部css放在顶部的情况，在15秒左右（同样，我在页面一出现时马上截图，误差不超过0.5秒），页面就全部渲染完毕；





![img](https://upload-images.jianshu.io/upload_images/3020614-60e6b0a53d0a1134.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/966)

全部放在尾部在1.07秒时的情况





![img](https://upload-images.jianshu.io/upload_images/3020614-e63b1c5933335789.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

全部放在最尾部的情况

全部css放在尾部的情况，在27秒左右（同样，我在页面一出现时马上截图，误差不超过0.5秒），页面就全部渲染完毕；

上面可以看出css放在顶部，页面渲染的就快一些。放在尾部，就要等待所有css，js加载完成以后才能进行渲染。建议开发人员都参照此CSS文件放置头部的规范。前端的性能优化必须对这些细节非常注意，变量引起质变，如果每个地方你都极尽最佳性能，你开发出来的网站性能将会尤其突出。