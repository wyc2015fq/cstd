# JavaScript全局函数与自定义函数-Functions - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月24日 12:05:43[boonya](https://me.csdn.net/boonya)阅读数：5195
所属专栏：[JavaScript学习提升](https://blog.csdn.net/column/details/jsmanlan.html)









全局属性和函数可用于所有内建的 JavaScript 对象。我们经常会用到一些内置的全局函数如parseInt();parseFloat()等等，另外我们可以根据自己的业务需要编写自己的函数。

# 全局函数

![](https://img-blog.csdn.net/20160824115505651)




## 全局对象描述



全局对象是预定义的对象，作为 JavaScript 的全局函数和全局属性的占位符。通过使用全局对象，可以访问所有其他所有预定义的对象、函数和属性。全局对象不是任何对象的属性，所以它没有名称。

在顶层 JavaScript 代码中，可以用关键字 this 引用全局对象。但通常不必用这种方式引用全局对象，因为全局对象是作用域链的头，这意味着所有非限定性的变量和函数名都会作为该对象的属性来查询。例如，当JavaScript 代码引用 parseInt() 函数时，它引用的是全局对象的 parseInt 属性。全局对象是作用域链的头，还意味着在顶层 JavaScript 代码中声明的所有变量都将成为全局对象的属性。

全局对象只是一个对象，而不是类。既没有构造函数，也无法实例化一个新的全局对象。

在 JavaScript 代码嵌入一个特殊环境中时，全局对象通常具有环境特定的属性。实际上，ECMAScript 标准没有规定全局对象的类型，JavaScript 的实现或嵌入的 JavaScript 都可以把任意类型的对象作为全局对象，只要该对象定义了这里列出的基本属性和函数。例如，在允许通过 LiveConnect 或相关的技术来脚本化 Java 的 JavaScript 实现中，全局对象被赋予了这里列出的 java 和 Package 属性以及 getClass() 方法。而在客户端 JavaScript
 中，全局对象就是 Window 对象，表示允许 JavaScript 代码的 Web 浏览器窗口。

全局对象对于引用它的JSP或者HTML页面是可见的。

```java
<script>

  var variables = "";//全局变量
  var arr=new Array();//全局数组对象
</script>
```

# 自定义函数

**无参函数:function(){  //return; }**

```java
function sum(){

    var a=1;

    var b=2;

    var c=a+b;

    console.log(c);

    //return c; // 若需要返回值打开此行注释即可

}
```



**含参数函数：function (arr0,arr1,...){ //return; }**


```java
function sum(a,b){

    var c=a+b;

    console.log(c);

    //return c; // 若需要返回值打开此行注释即可
}
```

函数返回值可有可无，如果需要返回值使用return yourData;就可以了，根据需要而定。


**变量函数写法:var func=function(){ //return;}**




```java
var sum=function(a,b){

    var c=a+b;

    console.log(c);

    //return c; // 若需要返回值打开此行注释即可
}
```







