# javascript里将函数名字符串转为函数并执行 - 左直拳的马桶_日用桶 - CSDN博客
2017年12月09日 19:01:02[左直拳](https://me.csdn.net/leftfist)阅读数：6897
javascript中，如何将函数名字符串转变为函数并执行？
“用 eval ”，西蒙说。
咋弄呢？类似地：
```
var mess = "hello world!";
var func = "alert";
var f = eval(func + "(mess);");//hello world!
```
可以写得复杂点：
```java
var frameRightSub = document.getElementById("frameSlider").contentDocument.getElementById("frame");
var func = "frameRightSub.contentWindow." + funcName;//需要执行的函数在嵌在某个iframe里的页面定义
if (typeof (eval(func)) != "function") return false;//检测是否为函数
var f = eval(func + "(node);");
```
另外，这个eval是把双刃剑，有被利用来进行攻击的危险，所以我们要谨慎使用。
