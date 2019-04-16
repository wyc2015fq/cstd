# 一个de两种意思——谈谈Dojo define 和declare的区别 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月25日 15:52:06[慢游](https://me.csdn.net/eengel)阅读数：19096








# 介绍

同样是de打头，我经常被dojo define和dojo declare弄晕。单词长的像，表面意思似乎也很像。翻译成中文叫“定义”和“声明”，可也不是Java中说的那样。那Dojo中是怎么安排这两个词的呢（Dojo 1.7.3）？



# Define

先看[define](https://dojotoolkit.org/documentation/tutorials/1.8/modules/)。作用是定义一个模块（module）。这个模块可以被require引用，引用了之后就可以使用define里面的东西。一个模块想当然，里面干什么事情，不一定全部自己实现。就像人要coding，除了脑子，也不能没有电脑、键盘。因此，define的第一个参数就是将要用到的其他模块引进来。第二个参数描述这个模块具体干什么，并且给第一个参数中的模块分别起一个朗朗上口的名字。就像下面这段代码描述的样子。



util.js



```java
define([ "dojo/dom"], function(dom) {
	return {
		setRed: function(id){
			dom.byId(id).style.color = "red";
		}
	};
});
```




这是一个工具模块，其中一个功能就是把网页上id对应的DOM节点变成红色。当我们要使用它的时候，就可以用require了。



test.jsp

```
<script>
	require(
		[ "dojo/ready", "test/util" ],
		function(ready, util) {
			ready(function() {
				var id = "selected_text";
				util.setRed(id);
			});
		});
</script>
```


# Declare

可以看到，上面的模块util作为工具模块，可以在被引用后任意调用其功能。这是无状态的，就好象是一个singleton的对象。但如果我们想定义“类”一样的东西，有状态，可以创建多个对象，就需要在define里用[declare](https://dojotoolkit.org/documentation/tutorials/1.8/declare/)。最典型的例子就是dijit下面的诸多UI小控件。

举个很简单的例子，我希望基于dijit.Dialog，创建一个有特殊功能的dialog，每次打开后能把上面的一段text标记为红色。



RedTextDialog.js

```java
define([ "dojo/_base/declare", "dijit/Dialog", "dijit/_WidgetBase",
 		"dijit/_TemplatedMixin", "test/util" ], function(declare,
 		Dialog, _WidgetBase, _TemplatedMixin, util) {
 	return declare("test.RedTextDialog", [ Dialog, _WidgetBase, _TemplatedMixin ], {
 		title: "Dialog with Red Text",
 		
 		onDownloadEnd : function() {
 			var id = "selected_text";
			util.setRed(id);
 		}
 	});
});
```


RedTextDialog可以重写dijit.Dialog所有的方法，也可以自创方法、变量，实现自己想要的任意功能。接下来可以用require使用它。

```
<script>
	require(
		[ "dojo/ready", "test/RedTextDialog" ],
		function(ready, RedTextDialog) {
			ready(function() {
				var dialog = new RedTextDialog();
				dialog.show();
			});
		});
</script>
```


可以看到，每次使用RedTextDialog时，都可以创建一个新的对象实例，因此可以做到互相之间没有关系。

# 目录结构

![](https://img-blog.csdn.net/20140103092742203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


为了在test.jsp中调用上述js文件，需要在test.jsp声明js文件的位置。



```java
<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true,
			packages : [ 
			    {
					name : "test",
					location : "../../js/test"
				}
			]
		};
	</script>
```



最后，约定俗成地，一般define类似util的singleton模块，js文件的名字第一个字母小写；而类似RedTextDialog的类模块，第一个字母大写。



