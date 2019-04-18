# 使用js对中文进行gbk编码 - z69183787的专栏 - CSDN博客
2013年02月09日 11:29:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13118
最近遇到一个问题，需要通过js来从一个utf-8编码的页面传递一个含中文的参数到另一个gbk编码的页面，直接使用js中的encodeURI等编码函数是不行的，他们都是使用utf-8编码参数，在GBK页面收到参数时就是乱码。在网上查了一下，找到一个用js实现的GBK编码的js组件，很小，挺方便的。在这里记录一下。
下载地址：[http://download.csdn.net/detail/jdluojing/4297634](http://download.csdn.net/detail/jdluojing/4297634)
使用方式：
```java
<script src="gbk.js"></script>
<script>
alert($URL.encode("地方"));
</script>
```
