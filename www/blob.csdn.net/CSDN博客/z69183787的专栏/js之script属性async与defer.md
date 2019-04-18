# js之script属性async与defer - z69183787的专栏 - CSDN博客
2018年05月09日 21:46:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：95
[https://www.cnblogs.com/tanhehe/p/4236021.html](https://www.cnblogs.com/tanhehe/p/4236021.html)
# 概念
默认情况下js的脚本执行是同步和阻塞的，但是 <script> 标签有 defer 和 async 属性, 这可以改变脚本的执行方式，这些都是布尔类型了，没有值，只需要出现在 <script> 标签里即可。
如：
![](https://images0.cnblogs.com/blog/494203/201501/201339397358672.png)
还要注意一点，html5说这些属性只在和src属性联合使用时才有效。
如果同时指定了两个属性，则会遵从async属性而忽略defer属性。
# 作用
**defer** 属性标注的脚本是延迟脚本，使得浏览器延迟脚本的执行，也就是说，脚本会被异步下载但是不会被执行，直到文档的载入和解析完成，并可以操作，脚本才会被执行。
**async** 属性标注的脚本是异步脚本，即异步下载脚本时，不会阻塞文档解析，但是一旦下载完成后，立即执行，阻塞文档解析。
# 区别
**延迟脚本**会按他们在文档里的出现顺序执行
**异步脚本**在它们载入后执行，但是不能保证执行顺序。
一张图足以说明区别：
![](https://images0.cnblogs.com/blog/494203/201501/201356044381705.jpg)
使用async的意义就在于使得下载脚本时，不会阻塞文档的解析。因为async的脚本执行顺序是没有保证的，因此要确认脚本间没有依赖关系。
现在呢基本上都是在文档的最后写脚本，那么这和 defer 的区别在哪里呢?
第一点当然是异步下载脚本了，第二点就是 使用async或defer任何一个都意味着在脚本里不能出现 document.write。
# 参考
http://peter.sh/experiments/asynchronous-and-deferred-javascript-execution-explained/
http://css-tricks.com/async-attribute-scripts-bottom/
<<JavaScript 权威指南>>
