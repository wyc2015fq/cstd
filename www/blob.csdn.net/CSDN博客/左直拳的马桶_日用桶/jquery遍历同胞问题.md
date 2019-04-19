# jquery遍历同胞问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月10日 19:05:37[左直拳](https://me.csdn.net/leftfist)阅读数：303
jquery中，获得某元素的同胞方法如下：
`$("#div1").siblings();`
但是这个方法似乎并不是返回一个数组。反正我想当然地这样写，结果行不通：
```java
var arr = $("#div1").siblings();
arr.forEach(function(obj){
    $(obj).removeClass("active");
});
$("#div1").addClass("active");
```
改为：
```
$("#div1").siblings().removeClass("active");
$("#div1").addClass("active");
```
不懂。仅作记录。
以上说法不确切，其实是返回数组了。
