# DIV中的对象错位问题 - 左直拳的马桶_日用桶 - CSDN博客
2012年07月25日 19:15:38[左直拳](https://me.csdn.net/leftfist)阅读数：1467标签：[div																[浏览器																[float																[微软																[ie](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=微软&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=div&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
象以下语句：
```
<div style="width:1000px;">
<div style="float:right;width:490px;">右</div>
<div style="width:490px;">左</div>
</div>
```
有时候，在某些浏览器下（比如微软的IE系列），这个 右 和 左 不在一行，而是发生错位，各自占据一行。但是它们的宽度加起来，并没有这种撑爆的后果啊。
世界上的事情就是这么奇妙。想破头都不搞不定的时候，加上一跳边框，他们就乖乖地回复正常了：
```
<div style="width:1000px;border:solid 1px red;">
<div style="float:right;width:490px;">右</div>
<div style="width:490px;">左</div>
</div>
```
看来浏览器有时候可能也会发生脑子短路的情况，没有画出边框线，它还不知道是框在里面的。
如果嫌边框不好看，可以将边框颜色设为更背景色一样的颜色，这样既看不出来，又不会错位。

