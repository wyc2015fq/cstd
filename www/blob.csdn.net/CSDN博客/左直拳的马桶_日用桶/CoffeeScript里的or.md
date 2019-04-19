# CoffeeScript里的or - 左直拳的马桶_日用桶 - CSDN博客
2014年11月21日 08:54:45[左直拳](https://me.csdn.net/leftfist)阅读数：1690
个人分类：[JavaScript																[node.js](https://blog.csdn.net/leftfist/article/category/2626561)](https://blog.csdn.net/leftfist/article/category/94500)
CoffeeScript里的or，其实会被编译为 ||
这并没有什么令人惊奇之处。我惊讶的是类似这样一个表达式：
```python
word = null
hi = word or "Hello World!"
alert hi
```
最终翻译成javascript是：
```java
var hi, word;
word = null;
hi = word || "Hello World!";
alert(hi);
```
hi = word || "Hello World!"
这句话怎么理解呢？要么等于word，要么等于 “Hello World!”？究竟等于那一个？或者是随机等于一个，哪个都可以？
猜测是优先等于前面这个，如果前面这个为false，那么等于后面那个。事实也是如此。
那么，这是node.js解释成这样的吗？不是的，放在浏览器里执行，也是同样的结果，说明这是javascript里的固有属性。
再次表明，我对javascript掌握得并不好。醉了。
```
word1 = null
word2 = null
hi = word1 or word2 or "Hello World!"
alert hi
```
执行结果一样的。
