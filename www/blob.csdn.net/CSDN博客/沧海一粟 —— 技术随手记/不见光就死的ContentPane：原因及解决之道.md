# 不见光就死的ContentPane：原因及解决之道 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年08月01日 21:18:26[慢游](https://me.csdn.net/eengel)阅读数：4658








dijit.layout.TabContainer是非常好用的一款tab组件，而创建每个tab的内容，最常用的就是dijit.layout.ContentPane。ContentPane 可以设置href，载入一块独立的html内容。每次在一个TabContainer中创建多个tab时，总是有一个tab的内容是当前可见的，而其他tab的内容看不见。这儿就出来一个需求：当每个tab包含的内容都很多时（比如显示一张大数据表格），我们并不希望一口气将所有tab的内容在第一时间都下载下来，而是每次只加载当前要显示的内容。也就是通常所说的“延迟加载”。

“延迟加载”的功能可以很好地改善用户体验，让网页浏览变得更高效，因此是一个可取的优化方法。但采用上面所说的实现，在某些场景中，却存在一个问题：有些ContentPane不见光就死了。



下面就来说说这个现象，分析原因并提供了一个解决方案。



首先构建一个TabContainer，包含了两个ContentPane。

```
<input type="button" value="Replace current tabs" onclick="rep();"/><br/>
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
    <div dojoType="dijit.layout.ContentPane" title ="Tab 1 of Rould 1" style="width: 200px;">11111</div>
    <div dojoType="Test" title="Tab 2 of Round 1" href="test0727-2.html" preload="false"></div>
</div>
```


其中Test[1]是一个自定义的ContentPane，功能不变，只是为每个标准生命周期函数加了控制台打印语句，从而便于我们观察它们的加载顺序。Test被设置为延迟加载，从而在不点击第二个tab之前，它的内容不会被加载。可以看到，当刷新整个页面时，firebug的控制台的输出结果，从而证明Test的内容并未被加载：



```
preamble
constructor
postMixInProperties
buildRendering
cancel
postCreate
startup
```




当点击按钮“Replace current tabs”时，TabContainer原有的tab会被清除掉，代之以新的tab。



```java
function rep(){
	var con = dijit.byId("container");
	con.destroyDescendants();
				
	var list = ["dijit.layout.ContentPane", "dijit.layout.ContentPane"];
	for(var i = 0; i < list.length; i++){
		constructor = dojo.getObject(list[i]);
		var widget = new constructor({
			title: "Tab "+ (i+1) + " of Rould 2"
		});
		widget.startup();
		con.addChild(widget);
	}
}
```


这个时候错误就出现了：




`Error: xhr cancelled`



从表面意思上来看，这个错误说的是一个xhr请求被取消了。那到底哪里来的xhr请求呢？

其实这要归因于TabContainer的destroyDescendants工作原理。destroyDescendants在运行时，会先销毁当前可见的tab。因此对于一个隐藏tab而言，在被销毁之前，它的显示功能会先被调用。而由于这个tab设置了href，根据延迟加载的原理，这个时候它的内容会通过发出一个xhr请求加载进来。但很不幸地，它的销毁功能紧接着显示功能被调用了，而且由于本地代码运行速度大于xhr请求速度，这个刚发出还没来得及完成的xhr请求就被销毁功能取消了。



知道了错误产生的原理，解决方法可以有很多种。为了保持延迟加载，有一种较好的解决方法[2]，就是在销毁各个tab时，阻止其显示功能被触发。重写点击“Replace current tabs”按钮的功能如下：

```java
function rep(){
	var con = dijit.byId("container");
	dojo.forEach(con.getChildren(), function(child){
		if(!child.selected){
			con.removeChild(child); //con.closeChild(child);
		}
	});
	con.destroyDescendants(); // destroy the final visible tab
				
	var list = ["dijit.layout.ContentPane", "dijit.layout.ContentPane"];
	for(var i = 0; i < list.length; i++){
		constructor = dojo.getObject(list[i]);
		var widget = new constructor({
			title: "Tab "+ (i+1) + " of Rould 2"
		});
		widget.startup();
		con.addChild(widget);
	}
}
```


参考文献：

[1] dijit.layout.ContentPane 中的标准方法/事件在不同情况下的调用, [http://blog.csdn.net/eengel/article/details/6595473](http://blog.csdn.net/eengel/article/details/6595473)



[2] dojo toolkit mailing list, [http://dojo-toolkit.33424.n3.nabble.com/Set-preload-false-not-working-when-programmatically-creating-a-content-pane-as-a-hidden-child-of-a-tr-td3196970.html](http://dojo-toolkit.33424.n3.nabble.com/Set-preload-false-not-working-when-programmatically-creating-a-content-pane-as-a-hidden-child-of-a-tr-td3196970.html)






