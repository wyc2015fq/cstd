# dijit.layout.ContentPane 中的标准方法/事件在不同情况下的调用 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月09日 23:49:26[慢游](https://me.csdn.net/eengel)阅读数：6112








前一篇帖子讨论了[Dijit 传入参数处理机制](http://blog.csdn.net/eengel/article/details/6592879)，这儿就Dijit中最常用的一个组件dijit.layout.ContentPane，讨论一下dijit标准方法和事件的调用。


dijit.layout.ContentPane[1]包含有一系列的标准方法和事件，又有诸多参数可以设置不同的值来对这些方法/事件的调用施加影响，导致对它的使用看起来非常的纷繁复杂。这儿通过firebug的console输出，尝试查看一下不同情况下，到底dijit.layout.ContentPane的各方法/事件的调用顺序是什么样的。



下面定义了一个继承自dijit.layout.ContentPane的自定义dijit组件。在后面，会一一列举不同的实例化方式和配置，及其在相应情景下，各方法/事件的调用情况。时间有限，这儿没有对结果进行分析，只是单纯列出来了。

```java
dojo.declare("Test", [ dijit.layout.ContentPane ], {

	preamble: function(){
		console.log("preamble");
		this.inherited(arguments);
	},
	
	constructor: function(){
		console.log("constructor");
	},

	postMixInProperties: function(){
		console.log("postMixInProperties");
		this.inherited(arguments);
	},

	buildRendering: function(){
		console.log("buildRendering");
		this.inherited(arguments);
	},
	
	postCreate: function(){
		console.log("postCreate");
		this.inherited(arguments);
	},
	
	startup: function(){
		console.log("startup");
		this.inherited(arguments);
	},
	
	cancel: function(){
		console.log("cancel");
		this.inherited(arguments);
	},
	
	destroy: function(){
		console.log("destroy");
		this.inherited(arguments);
	},
	
	destroyRecursive: function(){
		console.log("destroyRecursive");
		this.inherited(arguments);
	},
	
	destroyRendering: function(){
		console.log("destroyRendering");
		this.inherited(arguments);
	},
	
	refresh: function(){
		console.log("refresh");
		this.inherited(arguments);
	},
	
	uninitialize: function(){
		console.log("uninitialize");
		this.inherited(arguments);
	},
	
	onClose: function(){
		console.log("onClose");
		this.inherited(arguments);
	},
	
	onDownloadEnd: function(){
		console.log("onDownloadEnd");
		this.inherited(arguments);
	},
	
	onDownloadStart: function(){
		console.log("onDownloadStart");
		this.inherited(arguments);
	},
	
	onHide: function(){
		console.log("onHide");
		this.inherited(arguments);
	},
	
	onLoad: function(){
		console.log("onLoad");
		this.inherited(arguments);
	},
	
	onShow: function(){
		console.log("onShow");
		this.inherited(arguments);
	},
	
	onUnload: function(){
		console.log("onUnload");
		this.inherited(arguments);
	}
});
```


注意，除了constructor，别的方法都需要调用this.inherited(arguments)。



1. 声明式，不设href
`<div dojoType="Test" title="Space">This is a customized content pane.</div>`

（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
postCreate
startup
onShow
```


2. 声明式，在html tag中设置href
`<div dojoType="Test" title="Space" href=”/simple.do”>This is a customized content pane.</div>`

（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


3. 声明式，在Test constructor中设置href

```
<div dojoType="Test" title="Space">This is a customized content pane.</div>

constructor: function(){
	console.log("constructor");
	this.set("href", "/simple.do");
}
```


（1）加载Test所在页面：

```
preamble
constructor
cancel
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


4. 编程式，不设href

```java
dojo.ready(function(){
	var test = new Test({}, dojo.byId("container"));
	test.startup();
});
```


（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
postCreate
startup
onShow
```


（2）调用Test refresh()

```
refresh
cancel
onDownloadStart
onUnload
uncaught exception: [Exception... "Component returned failure code: 0x80070057 (NS_ERROR_ILLEGAL_VALUE) [nsIXMLHttpRequest.open]" nsresult: "0x80070057 (NS_ERROR_ILLEGAL_VALUE)" location: "JS frame :: http://localhost:8080/js/dojo-1.5.0/dojo/dojo.js.uncompressed.js :: anonymous :: line 10474" data: no]
```
注意，这儿没有设置href，refresh时就会出错。


5. 编程式，在new时设置href

```java
dojo.ready(function(){
	var test = new Test({
        href: “/simple.do”
}, dojo.byId("container"));
	test.startup();
});
```


（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


（2）调用Test refresh()

```
refresh
cancel
onDownloadStart
onUnload
onLoad
onDownloadEnd
```


（3）设置一个新的href

```
cancel
onDownloadStart
onUnload
onLoad
onDownloadEnd
```


（4）调用Test destroy()

```
destroy
uninitialize
cancel
destroyRendering
```






（5）调用Test destroyRecursive()

```
destroyRecursive
onUnload
destroy
uninitialize
cancel
destroyRendering
```



6. 编程式，在Test constructor中设置href

```java
dojo.ready(function(){
	var test = new Test({}, dojo.byId("container"));
	test.startup();
});

constructor: function(){
	console.log("constructor");
	this.set("href", "/simple.do");
}
```


（1）加载Test所在页面：

```
preamble
constructor
cancel
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


（2）调用Test refresh()

```
refresh
cancel
onDownloadStart
onUnload
onLoad
onDownloadEnd
```


（3）设置一个新的href

```
cancel
onDownloadStart
onUnload
onLoad
onDownloadEnd
```


（4）调用Test destroy()

```
destroy
uninitialize
cancel
destroyRendering
```






（5）调用Test destroyRecursive()



```
destroyRecursive
onUnload
destroy
uninitialize
cancel
destroyRendering
```



7. 声明式，Test实例挂在TabContainer下，并在一开始隐藏。在Test constructor中设置href，preload为false

```
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
	<div dojoType="dijit.layout.ContentPane" title ="22222" >2222</div>
	<div dojoType="Test" title="Space" preload="false">
		This is a customized content pane.
	</div>
</div>

constructor: function(){
	console.log("constructor");
	this.set("href", "/simple.do");
}
```


（1）加载Test所在页面：

```
preamble
constructor
cancel
postMixInProperties
buildRendering
cancel
postCreate
startup
```


（2）选中Test实例所在的tab

```
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


8. 声明式，Test实例挂在TabContainer下，并在一开始隐藏。在Test constructor中设置href，preload为true

```
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
	<div dojoType="dijit.layout.ContentPane" title ="22222" >2222</div>
	<div dojoType="Test" title="Space" preload="true">
		This is a customized content pane.
	</div>
</div>

constructor: function(){
	console.log("constructor");
	this.set("href", "/simple.do");
}
```


（1）加载Test所在页面：

```
preamble
constructor
cancel
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


（2）选中Test实例所在的tab
`onShow`

9. 声明式，Test实例挂在TabContainer下，并在一开始隐藏。在html tag中设置href，preload为false

```
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
	<div dojoType="dijit.layout.ContentPane" title ="22222" >2222</div>
	<div dojoType="Test" title="Space" preload="false" href="/simple.do" >
		This is a customized content pane.
	</div>
</div>
```


（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
cancel
postCreate
startup
```


（2）选中Test实例所在的tab

```
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


10. 声明式，Test实例挂在TabContainer下，并在一开始隐藏。在html tag中设置href，preload为true

```
<div id="container" dojoType="dijit.layout.TabContainer" style="width:1000px;height:400px;">
	<div dojoType="dijit.layout.ContentPane" title ="22222" >2222</div>
	<div dojoType="Test" title="Space" preload="true" href="/simple.do" >
		This is a customized content pane.
	</div>
</div>
```


（1）加载Test所在页面：

```
preamble
constructor
postMixInProperties
buildRendering
cancel
postCreate
startup
refresh
cancel
onDownloadStart
onShow
onLoad
onDownloadEnd
```


（2）选中Test实例所在的tab
`onShow`





（3）调用tabcontainer removeChild()
`  `

（4）关闭Test实例所在的tab （点击tab上的X，或者调用tab container closeChild()）

```
destroyRecursive
onUnload
destroy
uninitialize
cancel
destroyRendering
```


这儿需要将Test的onClose注释掉。不然关闭tab功能将不起作用。




（5）调用tabcontainer destroyDescendants()



```
onShow
destroyRecursive
onUnload
destroy
uninitialize
cancel
destroyRendering
```





参考文献：

[1] Dojo Toolkit API, [http://dojotoolkit.org/api/](http://dojotoolkit.org/api/)




