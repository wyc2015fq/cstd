# dijit.Dialog样例及其原型关系 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月17日 14:41:00[慢游](https://me.csdn.net/eengel)阅读数：3459








Dojo中dijit.Dialog对话框控件由于其良好的设计和封装，得到了很多人的喜爱。而通过继承dijit.Dialog类，我们也可以设计出内在逻辑复杂，功能各异的各种自定义对话框。

本文通过分析两个样例及其JavaScript原型之间的关系，来更深入地了解dijit.Dialog是如何工作的。其中原型关系图参考了[Dmitry A. Soshnikov](http://dmitrysoshnikov.com/)

的“[JavaScript. The Core.](http://dmitrysoshnikov.com/ecmascript/javascript-the-core/)

”（中文版：“[JavaScript核心指南](http://blog.csdn.net/eengel/archive/2011/01/14/6140772.aspx)
”）一文。

样例代码如下：

<mce:script type="text/javascript"><!--
	dojo.require("dijit.Dialog");
	dojo.declare("test.dialog", [dijit.Dialog], {
		a: "hello",
		title: "test dialog"
	});
	var tdlg = new test.dialog();
	tdlg.b = "world";
// --></mce:script>
<div id="showdialog" dojoType="test.dialog" style="width:400px;height:300px;"></div>

其中，test.dialog是继承自dijit.Dialog的自定义对话框对象，tdlg和<div "showdialog">是test.dialog的两个实例。

下面是原型关系图：

![](http://hi.csdn.net/attachment/201101/17/0_12952469398E75.gif)

1. Widget test.dialog继承了dijit.Dialog，而后者也继承了很多层不同的Dojo object。因此test.dialog可以使用或覆盖继承链上的绝大部分变量和方法。比如通过重写show()方法和hide()方法，可以自定义对话框显示和隐藏时的额外行为。

2. “test.dialog”做为构造函数名，可以用于构造Widget test.dialog实例。比如tdlg就是这样定义的。而在HTML中通过声明dojoType="test.dialog"，也可以达到定义Widget test.dialog实例的作用。

3. 根据图上箭头所指的关系，可以从一个结点访问到路径上的其他结点（这儿__proto__的写法仅限于Mozilla。用Firebug console在Firefox中可以查看到）。



