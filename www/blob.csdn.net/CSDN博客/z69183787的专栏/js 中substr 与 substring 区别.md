# js 中substr 与 substring 区别 - z69183787的专栏 - CSDN博客
2012年10月22日 10:15:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：684标签：[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Javascript-概述](https://blog.csdn.net/z69183787/article/category/2175033)
```
<script>
var c ="abcd";
alert(c.substr(0,2));
alert(c.substring(0,3));
</script>
```
返回为
ab
abc
其中substr 第一个参数为 起始位置 从0 开始，第二个参数为 截取长度
其中substring 第一个参数为 起始位置 从0 开始，第二个参数为 n，截取的值为 从0到n-1 索引的字符串。
