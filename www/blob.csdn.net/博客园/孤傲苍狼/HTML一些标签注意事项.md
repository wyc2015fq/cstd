# HTML一些标签注意事项 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [HTML一些标签注意事项](https://www.cnblogs.com/xdp-gacl/p/3495903.html)



最近在IE10下运行一个以前的做web系统发现了两个小问题：

**一、图片上使用"alt"属性来添加一些文字提示信息在IE10下无法正常显示出来**

上网查了一下原因：**原来是现在一些较新的浏览器，比如在IE9、IE10使用"alt"属性时，在图片能够正常显示的情况下，在图片中设置"alt"属性的值将不会显示出来，所以如果需要为图像创建文字提示，可以使用" title" 属性。**

**二、[cursor:hand](http://www.cnblogs.com/hakuci/archive/2011/07/14/2106013.html)在IE10下无法让光标变成手形**

这个问题也比较奇怪，估计也是兼容性的问题，上网找到了一篇比较好的文章，弄明白了[**cursor:hand ****与**** cursor:pointer ****的区别**](http://www.cnblogs.com/hakuci/archive/2011/07/14/2106013.html)

cursor:hand 与 cursor:pointer 的效果是一样的，都像光标指向链接一样，光标变成手行。
cursor:hand ：IE完全支持。但是在firefox是不支持的，没有效果。
cursor:pointer ：是CSS2.0的标准。所以firefox是支持的，但是IE5.0既之前版本不支持。IE6开始支持。

结论：还是用 cursor:pointer 来的方便。

cursor：鼠标

pointer：指针

**附：****cursor****属性收集**


光标类型   CSS
十字准心 cursor: crosshair;
手 cursor: pointer;
cursor: hand;
写两个是为了照顾IE5，它只认hand。
等待/沙漏 cursor: wait;
帮助 cursor: help;
无法释放 cursor: no-drop;
文字/编辑 cursor: text;
可移动对象 cursor: move;
向上改变大小(North)  
cursor: n-resize;
向下改变大小(South)   cursor: s-resize;
向右改变大小(East)   cursor: e-resize;
向左改变大小(West)   cursor: w-resize;
向上右改变大小(North East)   cursor: ne-resize;
向上左改变大小(North West)   cursor: nw-resize;
向下右改变大小(South East)   cursor: se-resize;
向下左改变大小(South West)   cursor: sw-resize;
自动 cursor: auto;
禁止 cursor:not-allowed;
处理中 cursor: progress;
系统默认 cursor: default;









