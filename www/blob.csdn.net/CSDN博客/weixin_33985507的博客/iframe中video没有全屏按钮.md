# iframe中video没有全屏按钮 - weixin_33985507的博客 - CSDN博客
2018年01月03日 21:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：35
#### [HTML内联框架元素 <iframe>](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/iframe)
iframe默认不允许全屏, 如果内嵌了video那么控制条上将不显示全屏按钮, 通过添加allowfullscreen属性可以开启全屏功能.
mozallowfullscreen
使用allowfullscreen属性代替。在Gecko9.9及更高版本上，如果允许通过调用element.mozRequestFullScreen() 方法，设置frame为全屏模式，该属性被设置为true。如果该属性未被设置，iframe元素不能被设置为全屏模式。
webkitallowfullscreen
使用 allowfullscreen 属性代替。在Chrome 17及更高版本上（或者更早），如果允许通过调用element.webkitRequestFullScreen() 方法，设置iframe为全屏模式，该属性被设置为true，如果该属性未被设置，iframe元素不能被设置为全屏模式。
![213419-20180103215128878-917898188.png](https://images2017.cnblogs.com/blog/213419/201801/213419-20180103215128878-917898188.png)
