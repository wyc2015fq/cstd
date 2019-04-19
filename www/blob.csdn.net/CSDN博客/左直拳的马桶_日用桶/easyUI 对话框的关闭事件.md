# easyUI 对话框的关闭事件 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月04日 22:34:20[左直拳](https://me.csdn.net/leftfist)阅读数：36224
有一个easyUI的dialog：
```
<div id="dlg_Add" class="easyui-dialog"   style=" width:540px; "   data-options="
		iconCls:'icon-allShow',
		buttons: '#dlg-buttons_Add',
		modal:true,
		border:true,
		shadow:false,
		closed:true,
		style:{padding:0},
		title:'Add'
	">
```
在页面运行过程中，对话框右上有一个叉，点击了之后会关闭此对话框。
因为我写的代码里面，根本没有这个叉，显然它是easyUI自动设置的。
现在问题来了，这个关闭事件，我们能不能干预，在里边写一些自己的代码呢？比如，询问一下是否确定关闭什么的。
答案是肯定的：
```java
$("#dlg_Add").dialog({
	onClose: function () {
		alert("blablabla");
	}
});
```
如果这个对话框在页面中可以反复打开，也许是因为缓存的原因，里面的数据会是上次打开时，操作后的样子，有时候可能会带来一些意想不到的异常。因此，关闭这个对话框的时候，最好将它销毁：
```java
$("#dlg_Add").dialog({
	onClose: function () {
		alert("blablabla");
		$(this).dialog('destroy');//销毁
	}
});
```
这两天在弄这个easyUI，一个第三方的成熟JS控件，功能很强。但对我而言，感觉没多大意思。我搞网站出身，前端，后端，数据库都会一点，但现在还是感到有点吃力。业界发展，分工越来越精细，全才是不可能的，也弄不好，前端最好还是由专门搞前端的人来负责。
