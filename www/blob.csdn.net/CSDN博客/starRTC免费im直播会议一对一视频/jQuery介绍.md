# jQuery介绍 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月14日 10:16:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：107
JavaScript世界中使用最广泛的一个库。
jQuery能帮我们干这些事情：
- 消除浏览器差异：你不需要自己写冗长的代码来针对不同的浏览器来绑定事件，编写AJAX等代码；
- 简洁的操作DOM的方法：写$('#test')肯定比document.getElementById('test')来得简洁；
- 轻松实现动画、修改CSS等各种操作。
目前jQuery有1.x和2.x两个主要版本，区别在于2.x移除了对古老的IE
 6、7、8的支持，因此2.x的代码更精简。选择哪个版本主要取决于你是否想支持IE 6~8。
jQuery只是一个jquery-xxx.js文件，但你会看到有compressed（已压缩）和uncompressed（未压缩）两种版本，使用时完全一样，但如果你想深入研究jQuery源码，那就用uncompressed版本。
使用jQuery只需要在页面的<head>引入jQuery文件即可：
<html><head><script
src="//code.jquery.com/jquery-1.11.3.min.js"></script>
 ...</head>
**极大地简化了 JavaScript 编程**
