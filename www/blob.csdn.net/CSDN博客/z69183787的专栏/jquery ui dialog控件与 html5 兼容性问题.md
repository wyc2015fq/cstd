# jquery ui dialog控件与 html5 兼容性问题 - z69183787的专栏 - CSDN博客
2012年12月11日 12:04:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3053
因为项目中 使用了html5 的某些元素，在IE 7 和8 中 不能识别这些元素，所以那些css即无法作用于这些元素。
设计人员 使用了 google 提供的 html5.js 其中包含了 html5的新增元素，可以让IE9一下的浏览器识别。
html5.js下载地址：[http://code.google.com/p/html5shiv/](http://code.google.com/p/html5shiv/)
```java
<!--[if lt IE 9]>
<script src="dist/html5shiv.js"></script>
<![endif]-->
```
如上使用即可。
之前碰到的问题是 html5.js的版本过旧，初步判断是因为，jquery ui dialog 控件中 用到了 button 元素，导致不兼容。
更新html5.js后即可使用 。
