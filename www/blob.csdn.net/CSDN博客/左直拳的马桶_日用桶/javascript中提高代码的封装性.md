# javascript中提高代码的封装性 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月14日 20:04:17[左直拳](https://me.csdn.net/leftfist)阅读数：1757
我出的面试题中，有一条是问如何避免页面引用JS，出现函数、变量重复、冲突的。
从大的方面讲，应该引入javascript的模块化开发，符合AMD规范之类；
从小的方面说，大概就是限定变量和函数的作用域了，这也涉及到一点大家如雷贯耳的闭包概念。
众所周知，javascript没有类，只有函数。其实它的函数也跟类差不多了，函数里面可以定义函数。而对于.net来说，直到近期出现的C#7，才支持这一点。
所以，我们可以将函数和变量，都定义在一个函数里面，这样即与外部隔绝矣：
```java
<html>
<head></head>
<body></body>
</html>
<script>
(function (){
    function f1(){};
    var v1;
    ....
})();
</script>
```
在一个页面中这样书写，等价于：
```xml
<html>
<head></head>
<body></body>
</html>
<script>
    function f1(){};
    var v1;
    ....
</script>
```
但封装性就好了许多，避免出现后者这种平铺直白，将方法和变量完全暴露于外界的隐患。并且`(function(){})();` 这种方式可以应用到子函数，乃至推广到所有的函数，层层封装，没有止境。并且有闭包属性，不必担心全局的绑定访问不到里层的函数：
```
(function(){
    function f1(){}
    function f2(){}
    $("#btn1").click(function(){
        f1();
        f2();
    });
}
```
