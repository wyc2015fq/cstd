# setTimeout调用匿名函数 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月14日 15:45:24[左直拳](https://me.csdn.net/leftfist)阅读数：1282
javascript中的setTimeout，大家都知道是这样调用：
```bash
setTimeout(函数,毫秒)
```
我以前不了解匿名函数，极死板，总要预先定义一个函数，然后再赋给setTimeout里的这个“函数”参数：
```
function f(){
}
setTimeout(f,1000);
```
这种方式有个限制，就是函数 f 一定是全局函数，否则setTimeout调用不了。如果这个f是嵌套在一些事件或其他函数体内，setTimeout将无法调用。
其实也可以在setTimeout里直接定义匿名函数：
```java
setTimeout(function(){},1000);
```
