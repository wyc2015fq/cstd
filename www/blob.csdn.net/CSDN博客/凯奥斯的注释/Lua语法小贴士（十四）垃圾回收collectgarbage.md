# Lua语法小贴士（十四）垃圾回收collectgarbage - 凯奥斯的注释 - CSDN博客





2016年11月24日 22:17:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：8692
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua垃圾回收只有一个函数collectgarbage，第一个参数是字符串，代表操作类型，第二个参数只有某些操作有，是该操作所需要的参数。




操作类型：

stop停止垃圾收集。

restart重启垃圾收集。

collect执行一个完整的垃圾收集循环。

count返回当前使用的内存，单位为千字节。

step单步执行垃圾收集，第二个参数代表多少步。如果步长可以完成一次垃圾收集，则返回true。

setstepmul第二参数/100代表单步的速度，默认值为200，代表是内存分配速度的两倍。

setpause第二个参数/100代表在开始一个新的收集周期之前要等待多久。当这个值小于等于100的时候，就代表执行完一个周期之后不会等待，直接进入下一个周期。当这个值为200的时候，就代表当内存达到上一个周期结束时的两倍的时候，再进入下一个周期。



当设置了setstepmul和setpause，Lua便会开启自动垃圾回收。


例如cocos2d-x中的设置：



```
collectgarbage("setpause",100)
collectgarbage("setstepmul",5000)
```
setpause为100代表，垃圾收集不会停止，setsetpmul为5000代表是内存分配速度的50倍。




我们只需要将不适用的变量设置为nil，它之前所引用的类型就会在一定时间内被自动回收。

当设置了setstepmul和setpause，Lua便会开启自动垃圾回收。



