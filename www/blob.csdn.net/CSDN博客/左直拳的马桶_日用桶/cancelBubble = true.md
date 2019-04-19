# cancelBubble = true - 左直拳的马桶_日用桶 - CSDN博客
2011年03月18日 17:34:00[左直拳](https://me.csdn.net/leftfist)阅读数：2273
cancelBubble = true，阻止事件由子元素向父元素冒泡。
什么时候用到呢？
比如，<div onclick="A()"><a href="b.htm">bbbbbb</a><div>
点 bbbbbb 的时候，也会触发 A()。
要避免出现这种情况，可以阻止点<a>的时候，也触发父元素DIV的click事件：
<div onclick="A()"><a href="b.htm" onclick="event.cancelBubble=true;">bbbbbb</a><div>
这个是阻止点击子元素，父元素也受影响的情况。现在假如，有一个链接<a>，同时具备href 属性和 onclick事件，只想onclick起作用，而href仅用于对搜索引擎友好，怎么办呢？
要点在于，onclick事件对应的函数，处理完毕后，应该返回false。
`<a href="http://blog.sina.com.cn/leftfist" onclick="return HitMe()">这是一条链接，搜索引擎，你看见我了吗？</a>`
```java
function onclick HitMe()
{
.....
return false;
}
```
