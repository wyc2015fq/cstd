# 如臂使指——Dojo框架下让浮动窗口跟随鼠标而动 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年10月28日 16:26:53[慢游](https://me.csdn.net/eengel)阅读数：3026








# 介绍

在web应用的很多场合，需要让浮动窗口跟随鼠标而动。比如这儿举的一个例子：当用户选中网页上一段内容时，弹出工具条让用户进行标记。可以想象一下，有一个在线看书的应用，支持读者在阅读的时候随时做笔记。当读者选中一段文字时，弹出一个小工具条，让用户保存成笔记标题或笔记内容。在这种场景下，让小工具条出现在选中文字的旁边是必要的用户体验。在实际应用场合，还必须确保在不同的浏览器中、当文字内容有滚动条时，小工具条的位置都不错。



下面就来看看如何实现这种效果（代码已经在Firefox 17.0.9，Chrome 30.0.1599.101，IE 10下面测过）。

首先用Dojo widget（Dojo 1.7.3）搭建一个网页框架。在主模块中放入文字内容。接下来要做的就是创建一个浮动窗口，让它在用户选中一段文字内容的时候出现。



# 代码实现

test.jsp



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
@import "../css/main.css";
</style>

</head>
<body class="tundra">
<div data-dojo-type="dijit.layout.BorderContainer"
		style="width: 100%; height: 100%; margin: 0px; padding: 0px; ">
	<div data-dojo-type="dojox.layout.ContentPane"
		data-dojo-props="region:'top'"
		style="height:38px;width:100%;background-color: black; color: white;font-family: arial;font-size: 28px;">
		在线阅读器
	</div>
	<div data-dojo-type="dojox.layout.ContentPane"
		data-dojo-props="region:'center', splitter: true"
		style="width: 100%; height: 100%; border: none; padding: 0px; background-color: #ffffff;">
		<div style="width: 99%; margin: -3px 0px -2px 2px;">
			<button id="open_model_btn" data-dojo-type="dijit.form.Button"
				class="menu_button" type="button">打开文章</button>
		</div>
		<div data-dojo-type="dijit.layout.BorderContainer"
			data-dojo-props="design:'sidebar', gutters:true, liveSplitters:true"
			style="width: 100%; height: 300px;">
			<div data-dojo-type="dojox.layout.ContentPane" data-dojo-props="splitter:true,region:'left'"
				style="width: 300px;">
				<div style="background-color: lightgray;">笔记列表</div>
			</div>
			<div data-dojo-type="dojox.layout.ContentPane" style="font-size: 16px;"
				data-dojo-props="splitter:true,region:'center'">
				<div id="resource_text_container">
					<div>阅读 reading</div>
					<div>1、从书面材料中获取信息的过程。书面材料主要是文字，也包括符号、公式、图表等。首先是把文字符号变成声音，后达到对书面材料的理解。阅读是一种主动的过程，是由阅读者根据不同的目的加以调节控制的。</div>
					<div>2、朗读是一种阅读方式。朗读是指出声诵读，默读则指没有明显发声的诵读。在某些情况下，如诗词欣赏。朗读有特殊功用，可高度集中注意力，但就从书面材料中获取知识而言，默读更为重要，理解文字材料主要靠默读。阅读时的眼动是一系列的跳动，跳动本身历时很短，而且不能产生对文字的清晰视觉。对文字的清晰视觉都是在注视时得到的。</div>
					<div>3、影响阅读理解的外部因素包括文字材料和情境的物理特点，如照明条件，文字的字体、型号等；文字材料的易读度，如字词的常用程度
						，句子的长短与结构的繁简
						，命题密度（即在一定长度的材料中出现的概念数）等；材料的概括与抽象的程度；由外部确定的阅读目的等等。影响阅读理解的内部因素主要是阅读者的知识基础。此外，阅读者的注意、记忆和思维也都是重要的内部因素。</div>
					<div>阅读可以分成四种情况。第一种是信息式阅读法。这类阅读的目的只是为了了解情况。我们阅读报纸、广告、说明书等属于这种阅读方法。对于大多数这类资料，读者应该使用一目十行的速读法，眼睛象电子扫描一样地在文字间快速浏览，及时捕捉自己所需的内容，舍弃无关的部分。任何人想及时了解当前形势或者研究某一段历史，速读法是不可少的，然而，是否需要中断、精读或停顿下来稍加思考，视所读的材料而定。</div>
					<div>第二种是文学作品阅读法。文学作品除了内容之外，还有修辞和韵律上的意义。因此阅读时应该非常缓慢，自己能听到其中每一个词的声音，嘴唇没动，是因为偷懒。例如读“压力”这个词时，喉部肌肉应同时运动。阅读诗词更要注意听到声音，即使是一行诗中漏掉了一个音节，照样也能听得出来。阅读散文要注意它的韵律，聆听词句前后的声音，还需要从隐喻或词与词之间的组合中获取自己的感知。文学家的作品，唯有充分运用这种接受语言的能力，才能汲取他们的聪明才智、想象能力和写作技巧。这种依赖耳听—一通过眼睛接受文字信号，将它们转译成声音，到达喉咙，然后加以理解的阅读方法，最终同我们的臆想能力相关。</div>
					<div>第三种是经典著作阅读法，这种方法用来阅读哲学、经济、军事和古典著作。阅读这些著作要象读文学作品一样的慢，但读者的眼睛经常离开书本，对书中的一字一句都细加思索，捕捉作者的真正的用意。从而理解其中的深奥的哲理。值得注意的是，如果用经典著作阅读法阅读文学作品，往往容易忽略文学作品的特色，以
						使读者自己钻进所谓文学观念史的牛角尖中去。</div>
					<div>第四种阅读方法是麻醉性的阅读法。这种阅读只是为了消遣。如同服用麻醉品那样使读者忘却了自己的存在，飘飘然于无限的幻想之中。这类读者一般对自己的经历和感受不感兴趣，把自己完全置身于书本之外。如果使用麻醉性的阅读方法阅读名著，读者只能得到一些已经添加了自己的幻想的肤浅的情节，使不朽的名著下降到鸳鸯蝴蝶派作家的庸俗作品的水平。如果漫不经心地阅读《安娜•卡列尼娜》，犹如读一本拙劣的三角恋爱小说。麻醉性的阅读在将进入成年的时候达到顶峰。年轻人的麻醉阅读是造成大量的文学作品质量低劣的原因。</div>
				</div>
				<div id="resource_text_floating_pane" 
					style="border: 1px solid #BBBBBB;position: absolute;width: 85px;z-index: 10;display:none;">
					<div data-dojo-type="dijit.Toolbar" style="padding: 3px 0 0 3px;">
					    <div id="add_paragraph" data-dojo-type="dijit.form.Button" data-dojo-props="showLabel:true">
					        	段落
					    </div>
						<div id="add_content" data-dojo-type="dijit.form.Button" data-dojo-props="showLabel:true">
					        	内容
					    </div>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require([ "dojo/parser", "dijit/form/Button", "dijit/Toolbar",
				"dijit/layout/BorderContainer", "dojox/layout/ContentPane"]);
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
						var resource_text_container = dom.byId("resource_text_container");
						on(resource_text_container, "mouseup", function(e){
							//拿到鼠标位置
							var selection = document.getSelection();
							var start = selection.anchorOffset;
							var end = selection.focusOffset;
							
							var fp = dom.byId("resource_text_floating_pane");
							if(start-end != 0){
								//根据鼠标位置，计算浮动窗口的位置
								if(dojo.isChrome != undefined){//Chrome
									var cx = e.layerX;
									var cy = resource_text_container.parentElement.scrollTop + e.layerY;								
								}else if(dojo.isIE != undefined){//IE
									var cx = e.layerX - resource_text_container.parentElement.offsetLeft;
									var cy = resource_text_container.parentElement.scrollTop + e.layerY;	
								}else {//Firefox
									var cx = e.layerX;
									var cy = e.layerY;
								}
								fp.style.left = (cx + 0) + "px";
								fp.style.top = (cy + 0) + "px";
								fp.style.display = "block";
							}else{
								fp.style.display = "none";
							}
						});
						
						on(registry.byId("add_paragraph"), "click", function(e){
							alert("新建段落");
						});
						
						on(registry.byId("add_content"), "click", function(e){
							alert("新建内容");
						});
					});
				});
	</script>
</body>
</html>
```


# 总结



在确定浮动窗口位置时，我们主要使用了鼠标事件的layerX和layerY属性。关于Event对象与定位相关的属性，和浏览器的版本密切相关。想支持其他版本的，可以参考这篇[文章](http://www.doc88.com/p-970313518669.html)。




