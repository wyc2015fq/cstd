# dijit.form.TextBox的一些用法 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年11月01日 15:47:15[慢游](https://me.csdn.net/eengel)阅读数：2782








# 介绍

可以说dijit.form.TextBox是最常用的一款Dojo UI控件之一。那如何能操作它呢？这儿就列举了一些用法共参考。



# HTML代码



```
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
@import "../css/main.css";
</style>

</head>
<body class="tundra">
<input type="text" id="input_box" style="width: 300px;" required
		data-dojo-type="dijit.form.TextBox" />
<button data-dojo-type="dijit.form.Button">OK</button>
</body>
</html>
```


# JavaScript代码

```java
<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require([ "dojo/parser", "dijit/form/TextBox", "dijit/form/Button"]);
	</script>
	<script>
		require(
				[ "dojo/ready", 
				  "dijit/registry",
				  "dojo/dom",
				  "dojo/on"
				  ],
				function(ready, registry, dom, on) {
					ready(function() {
						//取得dom节点
						var ib = dojo.byId("input_box");
						//取得dijit.form.TextBox控件
						var ibw = dijit.byId("input_box"); //[Widget dijit.form.TextBox, input_box] { _attachPoints=[2], _attachEvents=[0], _inherited={...}, more...}
						
						//操作属性：得到属性type的值
						ib.type; //"text"
						ibw.get("type"); //"text"
						ibw.type; //"text"
						
						//操作属性：得到属性value的值
						ib.value; //"hello"
						ibw.get("value"); //"hello"
						ibw.value; //"hello"
						
						//操作属性：设置属性value的值
						ib.value = "ib";
						ibw.setValue("ibw");
						ibw.value = "ibw"; //not work
						
						//比较相应的dom节点
						ibw.domNode; //<div id="widget_input_box" class="dijit dijitReset dijitInline dijitLeft dijitTextBox" role="presentation" style="width: 300px;" widgetid="input_box">
						ibw.focusNode; //<input id="input_box" class="dijitReset dijitInputInner" type="text" autocomplete="off" data-dojo-attach-point="textbox,focusNode" tabindex="0" required="" value="">
						ib; //<input id="input_box" class="dijitReset dijitInputInner" type="text" autocomplete="off" data-dojo-attach-point="textbox,focusNode" tabindex="0" value="">
						ibw.focusNode == ib; //true
						
						//设置style：长度
						ib.style.width = "200px"; //not work
						ibw.setAttribute("style", "width:200px;");
						
						//禁用
						ib.disabled = true; //not work
						ibw.setDisabled(true);
					});
				});
	</script>
```







