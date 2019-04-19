# javascript匿名函数 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月13日 19:02:52[左直拳](https://me.csdn.net/leftfist)阅读数：1381
此前在做node.js项目的时候，整天与匿名函数打交道，似乎习惯成自然，熟悉得很了。
这次天降大任，搞了许久前端，忽然有一次觉得应该写个匿名函数，竟然不知道该怎么写！
主要是搞不清楚匿名函数应该写在哪里。
函数有定义，有调用。而匿名函数是作为参数定义在一个函数里面的。
那么它是定义在函数定义里，还是定义在函数调用里？
定义在函数调用里。
我第一次是写在函数定义里，如
function A(id,function(){}){
}
结果语法就通不过。
后来想清楚了，匿名函数的意义在于，让你区分不同情况来做处理：
```java
function HelloWorld(getWho){
    alert(getWho() + ": Hello World!");
}
HelloWorld(function(){
    return "zhang3";
});
HelloWorld(function(){
    return "Li4";
});
//匿名函数，在调用的时候才定义。。。
```
.NET里也是一样。javascript里的匿名函数跟委托、lambda表达式有所关联：
```
delegate string DlgGetOneShowItem(DataMgrService.ViewGroupItem vgi);
DlgGetOneShowItem dlgGetOneCutomViewItem = (DataMgrService.ViewGroupItem vgi) =>
{
	//……
};
DlgGetOneShowItem dlgGetOneOjbectItem = (DataMgrService.ViewGroupItem vgi) =>
{
	//……
};
```
将函数赋给委托，一步到位，省却了定义函数，然后再赋给委托的麻烦。
[http://jsfiddle.net/](http://jsfiddle.net/) 是个好网站，在这里可以很方便地检测js、html、css。
