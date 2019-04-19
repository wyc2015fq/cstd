# this和闭包 - Arthur的随笔 - CSDN博客
2012年01月01日 23:32:49[largetalk](https://me.csdn.net/largetalk)阅读数：650标签：[javascript																[python																[function																[语言																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[python																[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)](https://blog.csdn.net/largetalk/article/category/715661)
一篇介绍javascript this的好文：[http://www.quirksmode.org/js/this.html](http://www.quirksmode.org/js/this.html)
以前以为js中this和c++中this差不多，原来却不是，
后来又以为this是指js代码块执行时环境，还是不对
现在这篇文章才终于知道this是“owner”的概念。
定义一个js函数，如
```java
function doSomething() {
   this.style.color = '#cc0000';
}
```
这时候的this是window
如果执行：
```java
elem.onclick = doSomething;
```
this就是elem了
如果这样定义：
`<element onclick="doSomething()">`只是表明click的时候去执行doSomething, this还是window
看个例子：
```java
var name=1;
function ff(){
    var name = 2;
    alert(this.name);
}
ff()
```
这时候打印的就是1
============================================================
说说闭包(closure), [http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html](http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html) 这篇文章讲的非常通俗易懂了。
很多语言都有闭包这个特性，当然python2中不支持
```python
def outerFun():
    outerVar = 0
    def innerFun():
        outerVar += 1
    print outerVar
    return innerFun
```
这样干会出错，出错分析有人写了一个很详细的介绍 [http://linluxiang.iteye.com/blog/789946](http://linluxiang.iteye.com/blog/789946)
 python3加了一个nonlocal关键字，来支持闭包，不过还是很不爽。

