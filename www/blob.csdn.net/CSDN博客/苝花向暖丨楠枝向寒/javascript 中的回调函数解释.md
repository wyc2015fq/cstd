# javascript 中的回调函数解释 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年10月27日 10:02:59[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：37标签：[回调函数](https://so.csdn.net/so/search/s.do?q=回调函数&t=blog)
个人分类：[javascrip](https://blog.csdn.net/weixin_40247263/article/category/8269605)



其实回调函数就是 调用匿名函数。但是匿名函数没有方法名该如何调用呢？

```java
function(value){
  alert(value)
}
```

我们可以通过另外一个函数来调用该函数，将匿名函数作为参数，匿名函数需要的参数也作为参数，同时传入另外一个函数当中，如下

```java
function execute(somefunction,value){
  somefunction(value)
}
execute(function(value){
  alert(value)
},'hello,world')
```

还有一种给匿名函数传参的方式是在函数体内，如下

```java
function execute(somefunction){
  var value = 'hello,world';
  somefunction(value)
}
```

我们经常会遇到的forEach()就有回调函数，如下

```java
var s = new Set(['A', 'B', 'C']);
s.forEach(function (element, sameElement, set) {
    console.log(element);
});
```

其中element 和 sameElement 代表每次循环迭代出Set中的元素，set代表Set对象

我们给forEach()传入了一个匿名函数，匿名函数在forEach()中被传参，因此我们使用console.log(element) 可以打印出值

并且java script 的函数参数可以与调用该函数时 传递的参数个数不一致，比如

我一个函数 定义的时候接收1个参数，但是你调用时传递了3个参数，那么就会把传递的第一个参数，作为函数调用时的参数。

如下是 Array对象的回调函数的使用

```java
var a = ['A', 'B', 'C'];
a.forEach(function (element) {
    console.log(element);
});
```

