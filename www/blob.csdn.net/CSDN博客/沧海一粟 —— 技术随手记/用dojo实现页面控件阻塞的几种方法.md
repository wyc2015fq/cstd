# 用dojo实现页面控件阻塞的几种方法 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年04月28日 17:28:37[慢游](https://me.csdn.net/eengel)阅读数：2562
个人分类：[Dojo](https://blog.csdn.net/eengel/article/category/773224)









经常碰到这样一种需求，当点击某个按钮向服务器发送请求时，需要将整个页面或页面的某些控件阻塞住，直到请求返回，才允许用户操作。

要实现这个功能，dojo中提供了多种方法。这儿就列举几种出来。（dojo 1.7.3）



**1 阻塞整个页面**

dijit/Dialog



```
<div class="dijitHidden">
		<div id="dialog" data-dojo-type="dijit.Dialog" style="width: 400px;">
			Loading...
		</div>
</div>
on(dom.byId("show_dialog"), "click", function(e) {
                            var dialog = registry.byId("dialog");
							//dialog.titleBar.style.display='none';
							dialog.show();
						});
```

好处：当显示Dialog的titlebar时，可随时关掉这个dialog取消阻塞。当然也可以把titlebar隐藏起来。




dijit/DialogUnderlay

```
div.loading {
    background-image: url(/img/loading.gif);
    background-repeat: no-repeat;
    background-position: center;
}
	<div class="dijitHidden">
		<div id="underlay" data-dojo-type="dijit.DialogUnderlay" class="loading" >
		</div>
	</div>
on(dom.byId("show_underlay"), "click", function(e) {
                            var underlay = registry.byId("underlay");
					        underlay.show();
						});
```
好处：也不需要定义复杂的jsobject各种属性，只要加一个cssstyle就可以改变样子。



**2. 阻塞页面某些控件**

dojox.widget.Standby

```
<div id="standby" data-dojo-type="dojox.widget.Standby"
		data-dojo-props="target:container, color:'lightgray', 
			text:'Loading...'">
	</div>
on(dom.byId("show_standby"), "click", function(e) {
                            var standby = registry.byId("standby");
							standby.show();
						});
on(dom.byId("stop"), "click", function(e) {
                            var standby = registry.byId("standby");
							standby.hide();
						});
```

好处：Standby属性中，target写谁的id，就阻塞谁，页面其他地方都不影响。




dojox.form.BusyButton

```
<button data-dojo-type="dojox.form.BusyButton" 
			id="button_noTimeout" busyLabel="Starting..." >Start</button>
<button data-dojo-type="dijit.form.Button" id="buttonCancel">Cancel button</button>
on(dom.byId("buttonCancel"), "click", function(e) {
							registry.byId("button_noTimeout").cancel();
						});
```

好处：只针对Button的阻塞方法。可设置多种属性比如timeout。







