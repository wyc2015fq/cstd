# 浅析dojo.connect的几种用法（一）——关联事件，不仅仅是DOM Event - 沧海一粟 —— 技术随手记 - CSDN博客





2011年05月13日 09:56:00[慢游](https://me.csdn.net/eengel)阅读数：9990








# 介绍

    每个流行的工具包中，总有一些异常出彩的闪光点。dojo.connect就是dojo工具包中，与JavaScript事件机制相关的重磅功能。
    在JavaScript的使用场景中，我们经常需要侦听某些事件的触发，然后进行相应的（函数）处理。比如最常见的，当点击登录页面的登录节点时，JavaScript能够察觉到，并随之将用户登录信息发送到后台。下面先来看看dojo.connect的参数[[http://docs.dojocampus.org/dojo/connect](http://docs.dojocampus.org/dojo/connect)]：


**dojo.connect(obj, event, context, method, dontFix);**




|**参数**|**类型**|**描述**|
|----|----|----|
|obj|Object|null|事件关联的对象。最常见的是DOM node，关联的事件会被委托到DOM事件管理器（除非dontFix为true）；也可以是其他JavaScript Object；或者干脆就是null，这时就是默认为dojo.global （window）。|
|event|String|上面obj需要关联的事件的名称。比如DOM node的”onclick”之类的。|
|context|Object|null|前面两个参数讲的都是关联的“源”，这儿开始就是讲关联的“目标”了。context为后面的method提供了执行上下文环境，相当于method的this。如果context为null且method是一个function，则method会继承event的context。如果method是一个string，则context必须是method的源对象。如果context为null，则默认为dojo.global。|
|method|String|Function|当event被触发后调用的目标函数。可以是函数引用，或者context中的函数名称。此method会拿到和事件同样的参数。|
|dontFix|Boolean|可选项。如果obj参数是DOM node，当设置dontFix为true时，可以阻止关联被委托到DOM事件管理器。（貌似很强大的功能）|



    从表面上看，dojo.connect就是一个单纯的函数，完成单纯的事件关联功能。但由于支持参数的灵活配置及和其他dojo函数的有机组合，有时候可以造成一些奇妙效果。而探索这些效果并用于解决一些特定的问题，正是技术的魅力所在吧！
    这儿很浅显地讨论一些目前能想到的dojo.connect特性。也欢迎有兴趣的同志给出更多的应用场景。 

# 关联事件，不仅仅是DOM Event

    当然dojo.connect最常用的地方还是关联DOM Event。但之所以先撇开DOM不谈，一方面是在用惯（烦？）了用dojo.connect关联DOM Event的方式后，对非DOM的事件关联正有新鲜感，另一方面，通过下面的例子，也可以帮助更好地理解上面dojo.connect的参数说明。
    首先是一个比较“官方”的例子：

function a(){
    console.log("a")
};
function b(){
    console.log("b");
}
dojo.connect(null,"a",null,b);
a();

    在firebug console中执行后，可以看到打印出：
a
b
    意思是，只要执行了函数a()，函数b()会同时被调用。 这个没有任何实际意义的例子对大家可能不太有吸引力。那下面就举一个context更复杂的实际例子：现在我们有一个页面，上面内嵌了带一个dijit.layout.ContentPane的dijit.layout.TabContainer。content pane里面是页面内容。当点击上面的save按钮时，输入框中的内容就会被保存，且同时在页面上出现保存提示。从需求中可以看到，“出现提示”这个功能是伴随着其他事件的发生而发生的，因此这儿就可以用dojo.connect来实现。代码如下：

<html>
	<head>
		<title>Test dojo.connect</title>
		<link rel="stylesheet" type="text/css" href="js/dojo-1.5.0/dijit/themes/dijit.css" mce_href="js/dojo-1.5.0/dijit/themes/dijit.css"></link>
		<link rel="stylesheet" type="text/css" href="js/dojo-1.5.0/dijit/themes/tundra/tundra.css" mce_href="js/dojo-1.5.0/dijit/themes/tundra/tundra.css"></link>
		<mce:script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/dojo/1.6/dojo/dojo.xd.js" mce_src="http://ajax.googleapis.com/ajax/libs/dojo/1.6/dojo/dojo.xd.js" djConfig="parseOnLoad:false"></mce:script>
		<mce:script type="text/javascript"><!--
			dojo.require("dojo.parser");
			dojo.require("dijit.layout.TabContainer");
			dojo.require("dijit.layout.ContentPane");
			
			dojo.addOnLoad(function(){
				dojo.declare("Test", [ dijit.layout.ContentPane ], {
					postCreate: function(){
						dojo.connect(this,"save",null,this.showMsg);
						dojo.connect(dojo.byId("save"), "onclick", this, this.save);
					},
					
				    save:function(){
				        console.log("save");
				    },
				    
					showMsg:function(){
						var node = dojo.byId("showMsg");
							node.innerHTML = "You are saving your mostly used dojo object...";
						dojo.style(node, "display","block");
					}
				});
				dojo.parser.parse();
			});
			
		
// --></mce:script>
	</head>
	<body class="tundra">
		<div style="width: 800px; height: 200px">
			<div dojoType="dijit.layout.TabContainer" style="width: 100%; height: 100%;">
				<div dojoType="Test" title="Space" id="cp">
					<div style="background-color:yellow;display:none;" mce_style="background-color:yellow;display:none;" id="showMsg">
					</div>
					<br/>
					Please input your mostly used dojo object:
					<br/>
					<input type="text" id="text"/>
					<br/>
					<input type="button" id="save" value="Save" />
				</div>
			</div>
		</div>
	</body>
</html>

    这儿**dojo.connect(this,"save",null,this.showMsg)**中，由于save函数属于Test，因此dojo.connect的第一个参数必须指定save的对象是this。第二个参数给定关联的事件为save。第四个参数也需要指定正确的关联目标事件，也就是this中的showMsg函数。还有一种等价的写法就是：**dojo.connect(this,"save", this,”showMsg”)**。
    现在开发者决定在页面上增加一个reset按钮，当点击时，输入框中的内容则会被清楚，同时出现清除内容提示。可以看到，同样是显示提示的需求，我们可以采用与关联save相同的办法将reset与showMsg关联。更多的，为了在不同的事件触发showMsg后会显示不同内容的提示，需要对dojo.connect的参数做一些变动。例子如下：

<html>
	<head>
		<title>Test Startup</title>
		<link rel="stylesheet" type="text/css" href="js/dojo-1.5.0/dijit/themes/dijit.css" mce_href="js/dojo-1.5.0/dijit/themes/dijit.css"></link>
		<link rel="stylesheet" type="text/css" href="js/dojo-1.5.0/dijit/themes/tundra/tundra.css" mce_href="js/dojo-1.5.0/dijit/themes/tundra/tundra.css"></link>
		<mce:script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/dojo/1.6/dojo/dojo.xd.js" mce_src="http://ajax.googleapis.com/ajax/libs/dojo/1.6/dojo/dojo.xd.js" djConfig="parseOnLoad:false"></mce:script>
		<mce:script type="text/javascript"><!--
			dojo.require("dojo.parser");
			dojo.require("dijit.layout.TabContainer");
			dojo.require("dijit.layout.ContentPane");
			
			dojo.addOnLoad(function(){
				dojo.declare("Test", [ dijit.layout.ContentPane ], {
					postCreate: function(){
						dojo.connect(this,"save",{"msg":"You are saving your mostly used dojo object..."},this.showMsg);
						dojo.connect(this,"reset",{"msg":"Your input is reset"},this.showMsg);
						dojo.connect(dojo.byId("save"), "onclick", this, this.save);
						dojo.connect(dojo.byId("reset"), "onclick", this, this.reset);
					},
					
				    save:function(){
				        console.log("save");
				    },
				    
				    reset:function(){
				    	dojo.byId("text").value = "";
				    },
					
					showMsg:function(){
						console.log("showMsg");
						var node = dojo.byId("showMsg");
						node.innerHTML = this.msg;
							
						dojo.style(node, "display","block");
					}
				});
				dojo.parser.parse();
			});
			
		
// --></mce:script>
	</head>
	<body class="tundra">
		<div style="width: 800px; height: 200px">
			<div dojoType="dijit.layout.TabContainer" style="width: 100%; height: 100%;">
				<div dojoType="Test" title="Space" id="cp">
					<div style="background-color:yellow;display:none;" mce_style="background-color:yellow;display:none;" id="showMsg">
					</div>
					<br/>
					Please input your mostly used dojo object:
					<br/>
					<input type="text" id="text"/>
					<br/>
					<input type="button" id="reset" value="Reset" />
					<input type="button" id="save" value="Save" />
				</div>
			</div>
		</div>
	</body>
</html>

    在最新的**dojo.connect(this,"save",{"msg":"You are saving your mostly used dojo object..."},this.showMsg)** 中，第三个参数变成了一个自定义对象，并在showMsg中用this取到值后显示相应的提示信息。想象一下如果有很多个按钮的点击事件需要关联showMsg函数，比起在每个事件中直接调用showMsg，像例子中这样集中用dojo.connect进行事件关联，将会大大提高代码的整洁性和可扩展性。更进一步可以将上面的dojo.connect调用改造成：

var list = [
	{
		func:"save",
		msg:"You are saving your mostly used dojo object..."
	},
	{
		func:"reset",
		msg:"Your input is reset"
	}];
var that = this;
dojo.forEach(list, function(item){
	dojo.connect(that, item.func, item, that.showMsg);
});

    最后总结一下，dojo.connect在非DOM event的事件关联应用中，也可以扮演关键角色，在实现功能的同时，提高代码的整洁性和可扩展性。
    下面的文章中，还将继续讨论dojo.connect的用法，挖掘挖掘dojo.connect一些有趣的特性。





