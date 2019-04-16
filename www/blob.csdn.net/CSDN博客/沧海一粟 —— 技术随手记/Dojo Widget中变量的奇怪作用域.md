# Dojo Widget中变量的奇怪作用域 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月28日 08:53:36[慢游](https://me.csdn.net/eengel)阅读数：1322








说实话，在出问题之前，下面要讨论的Dojo Widget特性是我无论如何也想不到的。问题是这样的。我用一个继承自dijit.layout.ContentPane的Dojo Widget  “Test”封装了一块页面内容。由于这块内容包含了很多事件句柄，我希望在每次刷新或关闭这块内容时销毁这些句柄。因此我在Test中定义了一个数组变量handles，用于储存句柄列表以便操作。不幸的是，当我的页面上存在多个

Test实例时，关闭一个Test实例导致了所有其他Test实例中事件句柄的销毁，从而它们的事件全部不能触发了。要知道这对于不知情的用户来说实在是一个致命的问题，页面上的按钮突然全都没用了，而浏览器甚至不会给任何提示。



既然问题出在Dojo Widget的变量作用域上，这儿就来研究一下。



首先构建一个测试案例：

```
<div>
	t1: outter: <span id="t1outter">0</span><br/>
	t1: oarray: <span id="t1oarray"></span><br/>
	t1: oobj: <span id="t1oobj"></span><br/>
	t1: inner: <span id="t1inner">0</span><br/>
	t1: iarray: <span id="t1iarray"></span><br/>
	t1: iobj: <span id="t1iobj"></span><br/>
	<br/>
	t2: outter: <span id="t2outter">0</span><br/>
	t2: oarray: <span id="t2oarray"></span><br/>
	t2: oobj: <span id="t2oobj"></span><br/>
	t2: inner: <span id="t2inner">0</span><br/>
	t2: iarray: <span id="t2iarray"></span><br/>
	t2: iobj: <span id="t2iobj"></span><br/>
</div>
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
	<div id="t1" dojoType="Test" title="Tab1" region="center">
<input type="button" value="Add1" onclick="c1();"/>
	</div>
	<div id="t2" dojoType="Test" title="Tab2" region="center">
<input type="button" value="Add2" onclick="c2();"/>
	</div>
</div>
```


这个页面上有一个dijit TabContainer，包含了两个Test实例。每个实例里面各有一个按钮，可操作相应实例中的变量。



下面是Test的定义：

```java
dojo.declare("Test", [ dijit.layout.ContentPane ], {
	outter: 0,  //integer
	oarray: [], //array object
	oobj: {}, //object
	
	constructor: function(){
this.inner = 0; //integer
this.iarray = [],//array object
this.iobj = {}, //object
	}
});
```


里面定义了6个变量。



每当点击页面上的按钮时，相应实例中的变量值会被改变，并显示在页面上部。

```java
function c1(){
	var w1 = dijit.byId("t1");
	var w2 = dijit.byId("t2");
	w1.outter++;
	w1.oarray.push(w1.outter);
	w1.oobj["t1-"+w1.outter] = w1.outter;
	w1.inner++;
	w1.iarray.push(w1.outter);
	w1.iobj["t1-"+w1.inner] = w1.inner;
	
	this.refreshResult(w1, w2);
}

function c2(){
	var w1 = dijit.byId("t1");
	var w2 = dijit.byId("t2");
	w2.outter++;
	w2.oarray.push(w2.outter);
	w2.oobj["t2-"+w2.outter] = w2.outter;
	w2.inner++;
	w2.iarray.push(w2.outter);
	w2.iobj["t1-"+w2.inner] = w2.inner;
	
	this.refreshResult(w1, w2);
}

function refreshResult(w1, w2){
	dojo.byId("t1outter").innerHTML = w1.outter;
	dojo.byId("t1oarray").innerHTML = w1.oarray;
	dojo.byId("t1oobj").innerHTML = w1.oobj.tos();
	dojo.byId("t1inner").innerHTML = w1.inner;
	dojo.byId("t1iarray").innerHTML = w1.iarray;
	dojo.byId("t1iobj").innerHTML = w1.iobj.tos();
	
	dojo.byId("t2outter").innerHTML = w2.outter;
	dojo.byId("t2oarray").innerHTML = w2.oarray;
	dojo.byId("t2oobj").innerHTML = w2.oobj.tos();
	dojo.byId("t2inner").innerHTML = w2.inner;
	dojo.byId("t2iarray").innerHTML = w2.iarray;
	dojo.byId("t2iobj").innerHTML = w2.iobj.tos();
}
```


准备完毕，下面就来看看页面上会发生什么。

初始界面：

![](http://hi.csdn.net/attachment/201107/28/0_1311814147441Z.gif)



点击按钮Add1 2次后的结果：
![](http://hi.csdn.net/attachment/201107/28/0_13118144037qkE.gif)


接着点击按钮Add2 4次后的结果：
![](http://hi.csdn.net/attachment/201107/28/0_1311814448wr0Q.gif)


从最后一个图中可以很清楚地看到，当操作一个Test实例的变量oarray和oobj时，会同时作用在其他Test实例的变量oarray和oobj上！当我将变量handles定义成oarray及oobj这样的时，开头提到的致命问题就出现了。



**结论：**

1. 当变量类型既不是JavaScript object，也不是JavaScript function时，放那儿作用域都一样，都只能被当前实例操作。比如上面的outter和inner。

2. 当变量是JavaScript object或JavaScript function类型时，就要小心了。如果只想让它被当前实例操作，就放到constructor中（估计也能放在其他function比如startup中，没具体测试过），比如iarray和iobj。如果想跨实例作用，则大胆地放在实例中function的外面，如oarray和oobj。





