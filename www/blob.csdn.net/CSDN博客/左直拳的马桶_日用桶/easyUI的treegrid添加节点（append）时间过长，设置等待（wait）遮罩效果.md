# easyUI的treegrid添加节点（append）时间过长，设置等待（wait）遮罩效果 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月16日 00:45:08[左直拳](https://me.csdn.net/leftfist)阅读数：6873
如题所述，在treegrid中，一次添加多个节点时，时间很长，但easyUI并无显示等待信息，让人以为是陷入了死循环，或者死机了，用户体验很差。
本来，treegrid（或者datagrid）有所谓的loadMsg，但只在加载数据时显示。添加节点，则不见有这种机制。
只能自己动手，丰衣足食了。
像很多次寻找答案一样，在网上搜索了许久，基本上都是一些互相抄袭，似是而非，不知所谓的垃圾。作者煞有介事地这样说，但有没有实际应用过，到底行不行，很让人怀疑。
或者只贴一点代码，缺乏足够的解释，让人变成丈二金刚。
经过一番努力，终于达到我自己想要的效果：
从一棵树中选中了许多节点，一次性复制到另一棵树。期间耗时比较长，会先显示等待信息；添加完成以后，等待消息消失。
没有代码我说个J8：
应用场景：
easyUI的一个对话框：
```
<div id="dlg_Add_template" class="easyui-dialog"   style=" width:600px; ">……</div>
<script type="text/javascript">
$('#dlg_Add_template').dialog('open');
</script>
```
这个对话框上有1个tree，1个treegrid。需要从tree上选择节点，复制到treegrid。
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/42752187#)[copy](http://blog.csdn.net/leftfist/article/details/42752187#)
- function _loadwaiting(){//显示等待信息
- var wrap = $("#dlg_Add_template");  
-     $("<div class=\"datagrid-mask\"></div>").css({display:"block",width:wrap.width(),height:wrap.height()}).appendTo(wrap);  
-     $("<div class=\"datagrid-mask-msg\"></div>").html("please wait...").appendTo(wrap).css({display:"block",left:(wrap.width()-$("div.datagrid-mask-msg",wrap).outerWidth())/2,top:(wrap.height()-$("div.datagrid-mask-msg",wrap).outerHeight())/2});  
- }  
- function _removeloading(){//隐藏等待信息
- var wrap = $("#dlg_Add_template");  
-     wrap.find("div.datagrid-mask-msg").remove();  
-     wrap.find("div.datagrid-mask").remove();  
- }  
以上代码从参考文章摘取，但经过改写。原文是对easyUI的所谓扩展，仍然只支持数据加载才显示。
然后调用：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/42752187#)[copy](http://blog.csdn.net/leftfist/article/details/42752187#)
- function _copyTo() {  
-     _loadwaiting();//显示等待信息
- 
- //将选中节点复制到目标树
-     (代码略)  
- 
-     _removeloading();//隐藏等待信息
- }  
结果意料之中，开始复制节点时，等待信息根本没来得及显示，直到复制完成才一闪而过，根本没有达到应有的效果！
这时应引入异步机制，最好有回调函数！
但treegrid并无提供相应的事件。受阮一峰的文章启发，可以模拟一下，用setTimeout：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/42752187#)[copy](http://blog.csdn.net/leftfist/article/details/42752187#)
- function copy(){  
-     _loadwaiting();//显示等待信息
- 
-     setTimeout(_copyTo,500);//500毫秒后开始复制工作
- }  
- }  
- function _copyTo() {  
- //将选中节点复制到目标树
-     (代码略)  
- 
-     _removeloading();//隐藏等待信息
- }  
参考文章：
[http://www.cnblogs.com/dreamcat/archive/2010/10/30/easyui_datagrid_mask.html](http://www.cnblogs.com/dreamcat/archive/2010/10/30/easyui_datagrid_mask.html)
[http://www.ruanyifeng.com/blog/2012/12/asynchronous%EF%BC%BFjavascript.html](http://www.ruanyifeng.com/blog/2012/12/asynchronous%EF%BC%BFjavascript.html)
