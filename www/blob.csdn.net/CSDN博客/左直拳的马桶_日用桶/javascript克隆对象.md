# javascript克隆对象 - 左直拳的马桶_日用桶 - CSDN博客
2018年08月22日 14:57:03[左直拳](https://me.csdn.net/leftfist)阅读数：86
个人分类：[JavaScript																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94500)
出乎我意料，原来javascript中的对象类型（object）也是引用类型。也就是说，这种类型的变量，其实是个指针，指向内存地址。将变量A赋给B，大家是一样的，改了B也就等于改了A。例子
```
var j1 = {name:'zhang3'};
var j2 = j1;
j2.name = "li4";
alert(j1.name);//李四
```
而值类型就不是了
```
var n1 = 10;
var n2 = n1;
n2 = 5;
alert(n1);//显示10
```
既然如此，操作引用类型变量的话，只能克隆了。原生的javascript估计要自己写方法。jquery有，直接拿来用
```java
var j1 = {name:'zhang3'};
var j3 = $.extend(true,{},j1);
j3.name = "wang5";
alert(j1.name);//还是张三
```
经学习，原来javascript的类型也分为值类型（基本类型）和引用类型
1、值类型
```java
Undefined、Null、Boolean、Number、String、Symbol (new in ES 6)
```
2、引用类型
```java
Object 类型、Array 类型、Date 类型、RegExp 类型、Function 类型
```
参考资料 
[JavaScript 深入了解基本类型和引用类型的值](https://segmentfault.com/a/1190000006752076)
